/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 界面编排层。
 *
 * 职责边界：
 *   - 建壳：状态栏 / 唱机（wxy_disc）/ 读碟窗（wxy_pages）/ 泛层
 *   - 导航状态机：轨道 <-> 内圈 <-> 应用，三种层级共用一台唱机
 *   - 时间与运行时：计时、音乐进度、读纹占位
 *   - 主题：切主题后通知页面层重画
 *
 * 页面层不持有导航状态，只把点击转成 wxy_ui_* 调用；
 * 于是"AI 也要能驱动界面"这件事，只要调同名 API 即可，不需要理解界面结构。
 */

#include <string.h>

#include <rtthread.h>
#include "littlevgl2rtt.h"

#include "wxy_kit.h"
#include "wxy_icon.h"
#include "wxy_disc.h"
#include "wxy_scene.h"
#include "wxy_ui.h"

#define LOG_TAG "WXY.UI"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

#define TK(c) (WXY_TOK()->color[(c)])

/* ------------------------------------------------------------------ */
/* 状态                                                                */
/* ------------------------------------------------------------------ */

static lv_obj_t *s_scr;
static lv_obj_t *s_well;
static lv_obj_t *s_overlay;     /* 泛层容器：在唱臂之上、读碟窗坐标一致 */
static lv_obj_t *s_fade;        /* 转场覆盖层：BG 色矩形。动画它的 opa 而非整棵树，
                                 * 否则每帧重绘 30+ 个软件字形 => render 540ms（实测） */

static lv_obj_t *s_detail;
static lv_obj_t *s_voice;
static lv_obj_t *s_ghost;
static lv_obj_t *s_tts;

static const wxy_disc_t *s_cur;     /* 当前显示在唱机上的碟 */
static const wxy_disc_t *s_track;   /* 母轨：应用/内圈的返回目标 */
static bool s_app_open;
static bool s_inner;
static bool s_busy;
static bool s_ready;
static bool s_theme_user;           /* 用户手动选过主题 */

static const char *s_fade_in;       /* 正在泛起的场景 id，用于收尾时隐藏其它层 */

/* 计时 */
static bool     s_timer_run;
static uint32_t s_timer_ms;
static lv_timer_t *s_timer_tick;

/* 音乐 */
static bool     s_music_on;
static uint32_t s_music_sec;
static uint8_t  s_music_idx;

static void ui_play_toggle(void);
static void ui_track_step(int8_t dir);

static void ui_prev(void) { ui_track_step(-1); }
static void ui_next(void) { ui_track_step(1); }

/* 应用开着时点别处：先换回母碟，落位后再去目标碟 */
static const char *s_pending_disc;

static const struct {
    const char *title;
    const char *artist;
    uint16_t    dur;
} k_songs[3] = {
    { "Night Passage", "Wander . ambient", 200 },
    { "Pixel Rain",    "Chiptune . game",  168 },
    { "Neon Pulse",    "Synth . city",     214 },
};

/* ------------------------------------------------------------------ */
/* 场景切换                                                            */
/* ------------------------------------------------------------------ */

static void hide_except(const char *keep)
{
    for (uint8_t i = 0; i < wxy_scene_count; i++)
    {
        lv_obj_t *r = wxy_scenes[i].root;
        if (r == NULL)
            continue;

        if (keep && rt_strcmp(wxy_scenes[i].id, keep) == 0)
            continue;

        lv_obj_add_flag(r, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_opa(r, LV_OPA_COVER, 0);   /* 复位，下次淡入才有起点 */
        lv_obj_set_y(r, 0);
    }
}

static void fade_in_ready(lv_anim_t *a)
{
    (void)a;
    if (s_fade_in)
        hide_except(s_fade_in);
    s_fade_in = NULL;

    if (s_fade)
        lv_obj_add_flag(s_fade, LV_OBJ_FLAG_HIDDEN);
}

/* 泛起一层：场景瞬间切换，动画一个纯色覆盖层来制造淡入。
 * 之前对整棵场景树做 opa+y 动画，每帧要重绘 30+ 个软件字形，render 540ms（实测）。
 * 现在只重绘覆盖层这一个矩形（EPIC fill，<1ms），树是静止的，不重绘。 */
static void scene_in(const char *id)
{
    wxy_scene_t *sc = wxy_scene_get(id);
    if (sc == NULL || sc->root == NULL)
        return;

    s_fade_in = id;

    /* 目标场景瞬间就位 */
    lv_obj_remove_flag(sc->root, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(sc->root, LV_OPA_COVER, 0);
    lv_obj_set_y(sc->root, 0);

    if (s_fade == NULL)
    {
        fade_in_ready(NULL);
        return;
    }

    /* 覆盖层盖住整个读碟窗，从 BG 实色淡出到透明 */
    lv_obj_set_style_bg_color(s_fade, wxy_col(TK(WXY_TOK_COLOR_BG)), 0);
    lv_obj_remove_flag(s_fade, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(s_fade, LV_OPA_COVER, 0);
    wxy_fade(s_fade, LV_OPA_COVER, LV_OPA_TRANSP,
             WXY_MS(WXY_TOK_MOTION_ENTER), WXY_TOK()->path_enter,
             fade_in_ready);
}

/* 沉入一层：覆盖层从透明淡入到 BG 实色，盖住旧场景；实际隐藏由后续 scene_in 处理 */
static void scene_out(const char *id)
{
    (void)id;
    if (s_fade == NULL)
        return;

    lv_obj_set_style_bg_color(s_fade, wxy_col(TK(WXY_TOK_COLOR_BG)), 0);
    lv_obj_remove_flag(s_fade, LV_OBJ_FLAG_HIDDEN);
    wxy_fade(s_fade, lv_obj_get_style_opa(s_fade, 0), LV_OPA_COVER,
             WXY_MS(WXY_TOK_MOTION_EXIT), WXY_TOK()->path_exit, NULL);
}

/* ------------------------------------------------------------------ */
/* 导航                                                                */
/* ------------------------------------------------------------------ */

static void seek_done(void)
{
    s_busy = false;
}

static void seek(const wxy_disc_t *d)
{
    if (d == NULL || s_busy)
        return;

    s_busy = true;

    scene_out(s_cur ? s_cur->id : NULL);

    s_cur = d;
    s_app_open = false;
    s_inner = false;

    scene_in(d->id);

    wxy_tt_swap(d, WXY_MS(WXY_TOK_MOTION_SWAP), seek_done);
}

static void open_done(void)
{
    s_busy = false;

    if (s_cur && rt_strcmp(s_cur->id, "music") == 0)
        wxy_ui_music(true);
    if (s_cur && rt_strcmp(s_cur->id, "timer") == 0)
        wxy_pages_timer_show(s_timer_ms, s_timer_run);
}

static void open_app(const wxy_disc_t *d)
{
    if (s_busy || d == NULL)
        return;

    s_busy = true;

    if (!s_app_open)
        s_track = s_cur;

    s_app_open = true;
    s_inner = false;

    /* WXY: 母轨改为隐藏（原 76 透明度虚化 = 整树 alpha 混合，弃用） */
    wxy_scene_t *p = wxy_scene_get(s_cur ? s_cur->id : NULL);
    if (p && p->root)
        lv_obj_add_flag(p->root, LV_OBJ_FLAG_HIDDEN);

    s_cur = d;
    scene_in(d->id);

    wxy_tt_swap(d, WXY_MS(WXY_TOK_MOTION_SWAP), open_done);
}

static void close_done(void)
{
    hide_except(s_cur ? s_cur->id : NULL);
    wxy_tt_label_name(NULL);          /* 曲名还原成碟名 */
    s_busy = false;

    if (s_pending_disc)
    {
        const char *id = s_pending_disc;
        s_pending_disc = NULL;
        wxy_ui_disc(id);
    }
}

void wxy_ui_back(void)
{
    if (!s_app_open || s_busy)
        return;

    s_busy = true;

    scene_out(s_cur->id);

    const wxy_disc_t *back = s_track ? s_track : &wxy_discs[0];

    wxy_scene_t *p = wxy_scene_get(back->id);
    if (p && p->root)
    {
        /* WXY: 瞬切显示，去掉 76->255 整树 opa 动画（每次重绘全树 alpha 混合） */
        lv_obj_remove_flag(p->root, LV_OBJ_FLAG_HIDDEN);
    }

    s_app_open = false;
    s_cur = back;
    s_fade_in = back->id;

    wxy_tt_swap(back, WXY_MS(WXY_TOK_MOTION_SWAP), close_done);
}

void wxy_ui_descend(void)
{
    if (s_busy || s_inner || s_app_open)
        return;

    const wxy_disc_t *inner = wxy_disc_get("theme");
    if (inner == NULL)
        return;

    s_busy = true;
    s_track = s_cur;
    s_inner = true;

    /* WXY: 父层改为隐藏（原 76 透明度虚化 = 整树 alpha 混合，弃用） */
    wxy_scene_t *p = wxy_scene_get(s_cur->id);
    if (p && p->root)
        lv_obj_add_flag(p->root, LV_OBJ_FLAG_HIDDEN);

    s_fade_in = inner->id;
    wxy_scene_t *t = wxy_scene_get(inner->id);
    if (t && t->root)
    {
        lv_obj_remove_flag(t->root, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_opa(t->root, 0, 0);
        wxy_fade_slide(t->root, 9, 0, 0, 255, WXY_MS(WXY_TOK_MOTION_ENTER),
                       WXY_TOK()->path_enter, NULL);
    }

    s_cur = inner;
    wxy_tt_swap(inner, WXY_MS(WXY_TOK_MOTION_SWAP), seek_done);
}

void wxy_ui_return(void)
{
    if (!s_inner || s_busy)
        return;

    s_busy = true;

    scene_out("theme");
    s_inner = false;

    const wxy_disc_t *back = s_track ? s_track : &wxy_discs[0];
    wxy_scene_t *p = wxy_scene_get(back->id);

    /* 父层从虚化恢复到清晰 */
    if (p && p->root)
    {
        lv_obj_remove_flag(p->root, LV_OBJ_FLAG_HIDDEN);
        wxy_fade(p->root, 76, 255, WXY_MS(WXY_TOK_MOTION_ENTER),
                 WXY_TOK()->path_enter, NULL);
    }

    s_fade_in = back->id;
    s_cur = back;
    wxy_tt_label_name(NULL);
    wxy_tt_swap(back, WXY_MS(WXY_TOK_MOTION_SWAP), seek_done);
}

void wxy_ui_app(const char *id)
{
    const wxy_disc_t *d = wxy_disc_get(id);
    if (d == NULL || d->kind != WXY_DISC_KIND_APP)
        return;

    if (s_app_open && s_cur && rt_strcmp(s_cur->id, id) == 0)
        return;

    if (s_app_open)
    {
        /* 应用之间直接换碟：换掉内容层，母轨不变 */
        if (s_busy)
            return;
        s_busy = true;
        scene_out(s_cur->id);
        s_cur = d;
        scene_in(d->id);
        wxy_tt_swap(d, WXY_MS(WXY_TOK_MOTION_SWAP), open_done);
        return;
    }

    open_app(d);
}

void wxy_ui_disc(const char *id)
{
    const wxy_disc_t *d = wxy_disc_get(id);
    if (d == NULL)
        return;

    if (d->kind == WXY_DISC_KIND_APP)
    {
        wxy_ui_app(id);
        return;
    }

    if (d->kind == WXY_DISC_KIND_INNER)
    {
        wxy_ui_descend();
        return;
    }

    if (s_app_open)
    {
        if (s_busy)
            return;
        s_pending_disc = d->id;
        wxy_ui_back();
        return;
    }

    if (s_inner)
    {
        wxy_ui_return();
        return;
    }

    if (s_cur && rt_strcmp(s_cur->id, id) == 0)
        return;

    seek(d);
}

void wxy_ui_rack(void)
{
    wxy_tt_tray(!wxy_tt_tray_open());
}

static void pick_cb(const wxy_disc_t *d)
{
    if (d)
        wxy_ui_disc(d->id);
}

/* ------------------------------------------------------------------ */
/* 泛层                                                                */
/* ------------------------------------------------------------------ */

static void detail_hidden(lv_anim_t *a)
{
    (void)a;
    lv_obj_add_flag(s_detail, LV_OBJ_FLAG_HIDDEN);
}

static void voice_hidden(lv_anim_t *a)
{
    (void)a;
    lv_obj_add_flag(s_voice, LV_OBJ_FLAG_HIDDEN);
}

void wxy_ui_surface(bool show)
{
    if (s_detail == NULL)
        return;

    if (show)
    {
        lv_obj_remove_flag(s_detail, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_opa(s_detail, 0, 0);
        lv_obj_set_y(s_detail, 56);
        wxy_fade_slide(s_detail, 56, 30, 0, 255,
                       WXY_MS(WXY_TOK_MOTION_ENTER), WXY_TOK()->path_enter, NULL);
    }
    else
    {
        wxy_fade(s_detail, 255, 0, WXY_MS(WXY_TOK_MOTION_EXIT),
                 WXY_TOK()->path_exit, detail_hidden);
    }
}

void wxy_ui_voice(bool show)
{
    if (s_voice == NULL)
        return;

    if (show)
    {
        lv_obj_remove_flag(s_voice, LV_OBJ_FLAG_HIDDEN);
        wxy_fade(s_voice, 0, 255, WXY_MS(WXY_TOK_MOTION_ENTER),
                 WXY_TOK()->path_enter, NULL);
    }
    else
    {
        wxy_fade(s_voice, 255, 0, WXY_MS(WXY_TOK_MOTION_EXIT),
                 WXY_TOK()->path_exit, voice_hidden);
    }
}

static void tts_hidden(lv_anim_t *a)
{
    (void)a;
    lv_obj_add_flag(s_tts, LV_OBJ_FLAG_HIDDEN);
}

static void ripple_done(lv_timer_t *t)
{
    (void)t;
    lv_obj_add_flag(s_ghost, LV_OBJ_FLAG_HIDDEN);

    /* 读纹结束：给一句 TTS 提示，1.4 秒后自己淡出 */
    lv_obj_remove_flag(s_tts, LV_OBJ_FLAG_HIDDEN);
    wxy_fade(s_tts, 255, 0, 380, WXY_TOK()->path_exit, tts_hidden);
}

void wxy_ui_ripple(void)
{
    if (s_ghost == NULL)
        return;

    lv_obj_remove_flag(s_ghost, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(s_ghost, 255, 0);

    lv_timer_t *t = lv_timer_create(ripple_done, WXY_MS(WXY_TOK_MOTION_GHOST), NULL);
    lv_timer_set_repeat_count(t, 1);
}

/* ------------------------------------------------------------------ */
/* 主题                                                                */
/* ------------------------------------------------------------------ */

void wxy_ui_theme(const char *id)
{
    if (wxy_theme_set(id) == 0)
    {
        s_theme_user = true;
        wxy_pages_refresh();
    }
}

void wxy_ui_mood(wxy_mood_t mood)
{
    if (s_theme_user)
        return;             /* 用户手动选过，不再被情绪覆盖 */

    const char *id = wxy_theme_suggest(mood);
    if (id)
    {
        wxy_theme_set(id);
        wxy_pages_refresh();
    }
}

/* ------------------------------------------------------------------ */
/* 音乐 / 计时                                                         */
/* ------------------------------------------------------------------ */

static void music_apply(void)
{
    char cur[8], dur[8];
    uint32_t d = k_songs[s_music_idx].dur;
    uint8_t pct = (uint8_t)((s_music_sec * 100) / (d ? d : 1));

    rt_snprintf(cur, sizeof(cur), "%02d:%02d", (int)(s_music_sec / 60), (int)(s_music_sec % 60));
    rt_snprintf(dur, sizeof(dur), "%02d:%02d", (int)(d / 60), (int)(d % 60));

    wxy_pages_music_show(k_songs[s_music_idx].title, k_songs[s_music_idx].artist, cur, dur, pct);
    wxy_tt_label_name(k_songs[s_music_idx].title);
    wxy_tt_progress(s_music_on ? pct : 0);
}

void wxy_ui_music(bool play)
{
    s_music_on = play;
    wxy_tt_set_playing(play);
    wxy_pages_music_state(play);
    music_apply();
}

bool wxy_ui_music_playing(void)
{
    return s_music_on;
}

void wxy_ui_music_skip(int8_t dir)
{
    if (dir > 0)
        s_music_idx = (uint8_t)((s_music_idx + 1) % 3);
    else
        s_music_idx = (uint8_t)((s_music_idx + 3 - 1) % 3);

    s_music_sec = 0;
    music_apply();
}

void wxy_ui_timer(bool run)
{
    s_timer_run = run;
    wxy_pages_timer_show(s_timer_ms, s_timer_run);
}

bool wxy_ui_timer_running(void)
{
    return s_timer_run;
}

void wxy_ui_timer_reset(void)
{
    s_timer_ms = 0;
    s_timer_run = false;
    wxy_pages_timer_show(s_timer_ms, s_timer_run);
    wxy_tt_progress(0);
}

/* 100ms 心跳：计时与音乐共用，不额外占线程 */
static void runtime_tick(lv_timer_t *t)
{
    (void)t;

    if (s_timer_run && s_app_open && s_cur && rt_strcmp(s_cur->id, "timer") == 0)
    {
        s_timer_ms += 100;
        if (s_timer_ms >= 60000)
            s_timer_ms = 0;
        wxy_pages_timer_show(s_timer_ms, s_timer_run);
    }

    static uint8_t sec_div;
    if (++sec_div < 10)
        return;
    sec_div = 0;

    if (!s_music_on || !s_app_open || s_cur == NULL || rt_strcmp(s_cur->id, "music") != 0)
        return;

    s_music_sec++;
    if (s_music_sec >= k_songs[s_music_idx].dur)
    {
        s_music_sec = 0;
        s_music_idx = (uint8_t)((s_music_idx + 1) % 3);
        LOG_I("auto next -> %s", k_songs[s_music_idx].title);
    }
    music_apply();
}

static void ui_play_toggle(void)
{
    wxy_ui_music(!s_music_on);
}

static void ui_track_step(int8_t dir)
{
    /* 外圈四个轨道循环；应用开着就先换回母轨 */
    if (s_app_open)
    {
        wxy_ui_back();
        return;
    }
    if (s_inner)
    {
        wxy_ui_return();
        return;
    }

    uint8_t i = 0;
    for (uint8_t k = 0; k < 4; k++)
    {
        if (s_cur && rt_strcmp(wxy_discs[k].id, s_cur->id) == 0)
        {
            i = k;
            break;
        }
    }
    i = (uint8_t)((i + 4 + (dir > 0 ? 1 : -1)) % 4);
    seek(&wxy_discs[i]);
}

/* ------------------------------------------------------------------ */
/* 建壳                                                                */
/* ------------------------------------------------------------------ */

static void ic_muted_cb(lv_obj_t *o, const wxy_tokens_t *t)
{
    wxy_icon_recolor(o, t->color[WXY_TOK_COLOR_MUTED]);
}

static void build_status_bar(void)
{
    lv_obj_t *bar = wxy_box_opa(s_scr, 0, 0, WXY_SCR_W, WXY_STATUS_H, 0, 0x000000, LV_OPA_TRANSP);
    lv_obj_remove_flag(bar, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *clk = wxy_text(bar, 18, 5, "21:00", TK(WXY_TOK_COLOR_MUTED), WXY_F_TINY);
    wxy_reg(clk, wxy_apply_muted);

    static const wxy_slot_t icons[3] = { WXY_SLOT_SIGNAL, WXY_SLOT_WIFI, WXY_SLOT_BATTERY };
    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *ic = wxy_icon_create(bar, icons[i], 16, TK(WXY_TOK_COLOR_MUTED));
        if (ic)
        {
            lv_obj_set_pos(ic, WXY_SCR_W - 24 - (2 - i) * 26, 6);
            wxy_reg(ic, ic_muted_cb);
        }
    }
}

static void detail_close_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_surface(false);
}

static void voice_close_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_voice(false);
}

/* TTS 图标不是 label，wxy_apply_on_accent 只改 text_color 对图元无效，
   所以单独给一个回调：切主题时按当时令牌重新染色（图案不变，只换色）。 */
static void tts_icon_apply(lv_obj_t *o, const wxy_tokens_t *t)
{
    wxy_icon_recolor(o, t->color[WXY_TOK_COLOR_ON_ACCENT]);
}

static void build_overlays(void)
{
    /* 读纹占位：三根微光条 */
    s_ghost = wxy_box_opa(s_overlay, 4, 92, WXY_WELL_W - 8, 74, 0, 0x000000, LV_OPA_TRANSP);
    lv_obj_add_flag(s_ghost, LV_OBJ_FLAG_HIDDEN);
    static const int32_t gw[3] = { 420, 380, 300 };
    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *b = wxy_box_opa(s_ghost, 8, i * 24, gw[i], 11, 6, TK(WXY_TOK_COLOR_ACCENT), 60);
        lv_obj_remove_flag(b, LV_OBJ_FLAG_CLICKABLE);
        wxy_reg(b, wxy_apply_accent_tint);
    }

    /* TTS 提示条 */
    s_tts = wxy_box(s_overlay, 10, 4, 268, 26, 6, TK(WXY_TOK_COLOR_ACCENT));
    lv_obj_add_flag(s_tts, LV_OBJ_FLAG_HIDDEN);
    wxy_reg(s_tts, wxy_apply_accent_bg);
    /* 前景必须走 ON_ACCENT：三套主题的 accent 明度差很大，写死会在 pixel/cyber 下变糊 */
    lv_obj_t *ti = wxy_icon_create(s_tts, WXY_SLOT_MIC, 16, TK(WXY_TOK_COLOR_ON_ACCENT));
    if (ti)
    {
        lv_obj_set_pos(ti, 8, 5);
        wxy_reg(ti, tts_icon_apply);
    }
    lv_obj_t *tt = wxy_text(s_tts, 30, 4, "Speaking: colder tonight",
                            TK(WXY_TOK_COLOR_ON_ACCENT), WXY_F_TINY);
    wxy_reg(tt, wxy_apply_on_accent);

    /* 详情：浮出唱片表面 */
    s_detail = wxy_box(s_overlay, 26, 30, 400, 290, 16, TK(WXY_TOK_COLOR_PANEL));
    lv_obj_set_style_border_width(s_detail, 1, 0);
    lv_obj_set_style_border_color(s_detail, wxy_col(TK(WXY_TOK_COLOR_ACCENT)), 0);
    lv_obj_add_flag(s_detail, LV_OBJ_FLAG_HIDDEN);
    wxy_reg(s_detail, wxy_apply_panel);

    lv_obj_t *db = wxy_box(s_detail, 14, 12, 24, 24, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_PANEL2));
    wxy_tap(db);
    lv_obj_add_event_cb(db, detail_close_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(db, wxy_apply_panel2);
    lv_obj_t *dbi = wxy_icon_create(db, WXY_SLOT_BACK, 16, TK(WXY_TOK_COLOR_ACCENT));
    if (dbi) lv_obj_set_pos(dbi, 4, 4);

    lv_obj_t *dt = wxy_text(s_detail, 48, 14, "Weather . tonight", TK(WXY_TOK_COLOR_TEXT), WXY_F_BODY);
    wxy_reg(dt, wxy_apply_text);

    lv_obj_t *hero = wxy_box(s_detail, 14, 48, 372, 92, 12, TK(WXY_TOK_COLOR_PANEL2));
    wxy_reg(hero, wxy_apply_panel2);
    lv_obj_t *hi = wxy_icon_create(hero, WXY_SLOT_WEATHER, 32, TK(WXY_TOK_COLOR_ACCENT));
    if (hi) lv_obj_set_pos(hi, 170, 30);

    lv_obj_t *body = wxy_text_w(s_detail, 16, 154, 368,
                                "18C, humidity 62%, NE wind 2. Feels cool, take a jacket.",
                                TK(WXY_TOK_COLOR_TEXT), WXY_F_TINY);
    (void)body;

    lv_obj_t *hint = wxy_text(s_detail, 16, 196, "floats off the record - not another page",
                              TK(WXY_TOK_COLOR_MUTED), WXY_F_TINY);
    wxy_reg(hint, wxy_apply_muted);

    lv_obj_t *ok = wxy_box(s_detail, 14, 240, 372, 38, 19, TK(WXY_TOK_COLOR_PANEL2));
    wxy_tap(ok);
    lv_obj_add_event_cb(ok, detail_close_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(ok, wxy_apply_panel2);
    lv_obj_t *oki = wxy_icon_create(ok, WXY_SLOT_CHECK, 16, TK(WXY_TOK_COLOR_ACCENT));
    if (oki) lv_obj_set_pos(oki, 150, 11);
    lv_obj_t *okt = wxy_text(ok, 176, 11, "Sink back", TK(WXY_TOK_COLOR_TEXT), WXY_F_TINY);
    (void)okt;

    /* 语音泛层 */
    s_voice = wxy_box(s_overlay, 44, 0, 364, 364, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_ACCENT));
    lv_obj_set_style_bg_opa(s_voice, 40, 0);
    lv_obj_add_flag(s_voice, LV_OBJ_FLAG_HIDDEN);
    wxy_reg(s_voice, wxy_apply_accent_tint);

    for (int i = 0; i < 7; i++)
    {
        int32_t h = (i % 2) ? 18 : 30;
        lv_obj_t *w = wxy_box(s_voice, 112 + i * 20, 150 - h / 2, 4, h, 2, TK(WXY_TOK_COLOR_ACCENT));
        lv_obj_remove_flag(w, LV_OBJ_FLAG_CLICKABLE);
        wxy_reg(w, wxy_apply_accent_bg);
    }

    lv_obj_t *vt = wxy_text(s_voice, 0, 186, "Listening . release to send", TK(WXY_TOK_COLOR_TEXT), WXY_F_TINY);
    lv_obj_set_width(vt, 364);
    lv_obj_set_style_text_align(vt, LV_TEXT_ALIGN_CENTER, 0);
    wxy_reg(vt, wxy_apply_text);

    lv_obj_t *stop = wxy_box(s_voice, 112, 232, 140, 36, 18, TK(WXY_TOK_COLOR_PANEL2));
    wxy_tap(stop);
    lv_obj_add_event_cb(stop, voice_close_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(stop, wxy_apply_panel2);
    lv_obj_t *st = wxy_text(stop, 0, 9, "Stop", TK(WXY_TOK_COLOR_TEXT), WXY_F_TINY);
    lv_obj_set_width(st, 140);
    lv_obj_set_style_text_align(st, LV_TEXT_ALIGN_CENTER, 0);
    wxy_reg(st, wxy_apply_text);
}

static void build(void)
{
    wxy_theme_init("ink");

    s_scr = lv_screen_active();
    lv_obj_remove_flag(s_scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(s_scr, wxy_col(TK(WXY_TOK_COLOR_BG)), 0);
    lv_obj_set_style_bg_opa(s_scr, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(s_scr, 0, 0);
    lv_obj_set_style_border_width(s_scr, 0, 0);

    /* 1. 唱机（碟 / 唱臂 / 轨道点 / 小 CD / 走带区） */
    wxy_tt_create(s_scr);
    wxy_tt_set_pick_cb(pick_cb);
    wxy_tt_set_transport_cb(ui_prev, ui_play_toggle, ui_next);

    /* 2. 读碟窗 */
    s_well = wxy_box_opa(s_scr, WXY_WELL_X, WXY_WELL_Y, WXY_WELL_W, WXY_WELL_H,
                         0, 0x000000, LV_OPA_TRANSP);
    wxy_pages_create(s_well);

    /* 3. 把唱臂/碟仓抬到内容层之上，再建泛层（泛层要盖住唱臂） */
    wxy_tt_raise();
    s_overlay = wxy_box_opa(s_scr, WXY_WELL_X, WXY_WELL_Y, WXY_WELL_W, WXY_WELL_H,
                            0, 0x000000, LV_OPA_TRANSP);
    build_overlays();

    /* 转场覆盖层：盖在内容之上，动画它的 opa 代替整棵树重绘。
     * 不拦截触摸，点击可穿透到下面的场景。 */
    s_fade = wxy_box(s_scr, WXY_WELL_X, WXY_WELL_Y, WXY_WELL_W, WXY_WELL_H,
                     0, TK(WXY_TOK_COLOR_BG));
    lv_obj_add_flag(s_fade, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(s_fade, LV_OBJ_FLAG_CLICKABLE);
    wxy_reg(s_fade, wxy_apply_bg);

    build_status_bar();

    /* 4. 首屏 */
    s_cur = &wxy_discs[0];
    s_track = s_cur;
    scene_in("chat");
    hide_except("chat");
    wxy_tt_paint(s_cur);

    /* 5. 运行时心跳 */
    s_timer_tick = lv_timer_create(runtime_tick, 100, NULL);

    s_ready = true;

    /* LVGL 堆：开了 PSRAM 私有堆后对象都在那里，系统堆只看线程/驱动 */
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    rt_uint32_t sys_total = 0, sys_used = 0, sys_peak = 0;
    rt_memory_info(&sys_total, &sys_used, &sys_peak);
    LOG_I("ui ready (%dx%d, %d scenes), lv heap %d/%d KB (used %d%%, frag %d%%), sys heap %d/%d KB",
          WXY_SCR_W, WXY_SCR_H, wxy_scene_count,
          (int)((mon.total_size - mon.free_size) / 1024), (int)(mon.total_size / 1024),
          (int)mon.used_pct, (int)mon.frag_pct,
          (int)(sys_used / 1024), (int)(sys_total / 1024));
}

/* ------------------------------------------------------------------ */
/* 线程                                                                */
/* ------------------------------------------------------------------ */

static void ui_thread(void *arg)
{
    (void)arg;

    if (littlevgl2rtt_init("lcd") != RT_EOK)
    {
        LOG_E("littlevgl2rtt_init failed");
        return;
    }

    build();

    for (;;)
    {
        uint32_t ms = lv_task_handler();
        rt_thread_mdelay(ms ? ms : 5);
    }
}

int wxy_ui_start(void)
{
    rt_thread_t tid = rt_thread_create("wxy_ui", ui_thread, RT_NULL,
                                       16 * 1024, 20, 10);
    if (tid == RT_NULL)
    {
        rt_uint32_t total = 0, used = 0, peak = 0;
        rt_memory_info(&total, &used, &peak);
        LOG_E("create ui thread failed: sys heap %d/%d KB (peak %d KB), need 16 KB contiguous",
              (int)(used / 1024), (int)(total / 1024), (int)(peak / 1024));
        return -RT_ENOMEM;
    }
    rt_thread_startup(tid);
    return RT_EOK;
}

bool wxy_ui_ready(void)
{
    return s_ready;
}
