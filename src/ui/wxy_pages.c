/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * 读碟窗内容层：五个主场景 + 五个应用视图。
 *
 * 全部按 452x364 绝对定位排版，坐标以读碟窗左上角为原点。
 * 页面只做两件事：建控件（并按令牌登记）、把点击转成 wxy_ui_* 调用；
 * 状态（播放进度、计时值）由 UI 层计算后通过 wxy_pages_*_show() 回填。
 */

#include <string.h>

#include <rtthread.h>

#include "wxy_kit.h"
#include "wxy_icon.h"
#include "wxy_disc.h"
#include "wxy_scene.h"
#include "wxy_ui.h"

#define LOG_TAG "WXY.PAGE"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

#define WELL_W WXY_WELL_W
#define WELL_H WXY_WELL_H
#define PAD    18
#define INNER  (WELL_W - PAD * 2)   /* 416 */

#define TK(c)  (WXY_TOK()->color[(c)])

wxy_scene_t wxy_scenes[WXY_SCENE_MAX];
uint8_t wxy_scene_count;

static lv_obj_t *s_well;

/* 需要回填数据的控件 */
static lv_obj_t *s_timer_val;
static lv_obj_t *s_timer_ring;
static lv_obj_t *s_timer_tx;
static lv_obj_t *s_timer_icon;

static lv_obj_t *s_music_title;
static lv_obj_t *s_music_artist;
static lv_obj_t *s_music_cur;
static lv_obj_t *s_music_dur;
static lv_obj_t *s_music_bar;
static lv_obj_t *s_music_main;      /* 播放/暂停主按钮 */
static lv_obj_t *s_music_main_icon;

static lv_obj_t *s_pal_face;
static lv_obj_t *s_pal_state;
static lv_obj_t *s_pal_hint;
static lv_obj_t *s_pal_root;

static lv_obj_t *s_theme_check[3];

/* 事件回调前向声明：app_bar 用到了 back_cb，而它定义在后面 */
static void back_cb(lv_event_t *e);
static void rack_cb(lv_event_t *e);

/* ------------------------------------------------------------------ */
/* 控件助手                                                            */
/* ------------------------------------------------------------------ */

static wxy_theme_apply_cb_t apply_for(wxy_tok_color_t tok)
{
    switch (tok)
    {
    case WXY_TOK_COLOR_MUTED:   return wxy_apply_muted;
    case WXY_TOK_COLOR_ACCENT:  return wxy_apply_accent;
    case WXY_TOK_COLOR_ACCENT2: return wxy_apply_accent2;
    case WXY_TOK_COLOR_LINE:    return wxy_apply_line;
    default:                    return wxy_apply_text;
    }
}

/* 图标染色回调：图标是图元拼的，必须显式重染 */
static void ic_accent(lv_obj_t *o, const wxy_tokens_t *t)
{
    wxy_icon_recolor(o, t->color[WXY_TOK_COLOR_ACCENT]);
}

static void ic_text(lv_obj_t *o, const wxy_tokens_t *t)
{
    wxy_icon_recolor(o, t->color[WXY_TOK_COLOR_TEXT]);
}

static void ic_muted(lv_obj_t *o, const wxy_tokens_t *t)
{
    wxy_icon_recolor(o, t->color[WXY_TOK_COLOR_MUTED]);
}

static void ic_pal(lv_obj_t *o, const wxy_tokens_t *t)
{
    (void)t;
    wxy_icon_recolor(o, wxy_disc_hue("pal"));   /* 陪伴碟的固定身份色，跟碟表走 */
}

static lv_obj_t *T(lv_obj_t *p, int32_t x, int32_t y, const char *s,
                   wxy_tok_color_t tok, const lv_font_t *f)
{
    lv_obj_t *l = wxy_text(p, x, y, s, TK(tok), f);
    wxy_reg(l, apply_for(tok));
    return l;
}

static lv_obj_t *TW(lv_obj_t *p, int32_t x, int32_t y, int32_t w, const char *s,
                    wxy_tok_color_t tok, const lv_font_t *f)
{
    lv_obj_t *l = wxy_text_w(p, x, y, w, s, TK(tok), f);
    wxy_reg(l, apply_for(tok));
    return l;
}

static lv_obj_t *P(lv_obj_t *p, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r)
{
    lv_obj_t *o = wxy_box(p, x, y, w, h, r, TK(WXY_TOK_COLOR_PANEL));
    wxy_reg(o, wxy_apply_panel);
    return o;
}

static lv_obj_t *P2(lv_obj_t *p, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r)
{
    lv_obj_t *o = wxy_box(p, x, y, w, h, r, TK(WXY_TOK_COLOR_PANEL2));
    wxy_reg(o, wxy_apply_panel2);
    return o;
}

static void hairline(lv_obj_t *p, int32_t x, int32_t y, int32_t w)
{
    lv_obj_t *o = wxy_box(p, x, y, w, 1, 0, TK(WXY_TOK_COLOR_LINE));
    lv_obj_remove_flag(o, LV_OBJ_FLAG_CLICKABLE);
    wxy_reg(o, wxy_apply_line);
}

static lv_obj_t *icon_at(lv_obj_t *p, wxy_slot_t slot, int32_t x, int32_t y,
                         int32_t px, wxy_theme_apply_cb_t cb, uint32_t color)
{
    lv_obj_t *o = wxy_icon_create(p, slot, px, color);
    if (o)
    {
        lv_obj_set_pos(o, x, y);
        wxy_reg(o, cb);
    }
    return o;
}

static lv_obj_t *root_new(void)
{
    lv_obj_t *r = wxy_box_opa(s_well, 0, 0, WELL_W, WELL_H, 0, 0x000000, LV_OPA_TRANSP);
    lv_obj_add_flag(r, LV_OBJ_FLAG_HIDDEN);
    return r;
}

static void scene_add(const char *id, lv_obj_t *root)
{
    if (wxy_scene_count >= WXY_SCENE_MAX)
        return;
    wxy_scenes[wxy_scene_count].id = id;
    wxy_scenes[wxy_scene_count].root = root;
    wxy_scenes[wxy_scene_count].on_enter = NULL;
    wxy_scenes[wxy_scene_count].on_leave = NULL;
    wxy_scenes[wxy_scene_count].on_refresh = NULL;
    wxy_scene_count++;
}

/* 建根并登记，页面 builder 拿到根后直接往里摆控件 */
static lv_obj_t *scene_begin(const char *id)
{
    lv_obj_t *r = root_new();
    scene_add(id, r);
    return r;
}

static void head_at(lv_obj_t *r, int32_t y, const char *tk, const char *title, const char *sub)
{
    T(r, PAD, y + 6, tk, WXY_TOK_COLOR_ACCENT, WXY_F_TINY);

    lv_obj_t *t = T(r, PAD + 48, y, title, WXY_TOK_COLOR_TEXT, WXY_F_TITLE);
    (void)t;

    lv_obj_t *s = TW(r, 0, y + 8, 190, sub, WXY_TOK_COLOR_MUTED, WXY_F_TINY);
    lv_obj_align(s, LV_ALIGN_TOP_RIGHT, -PAD, y + 8);

    hairline(r, PAD, y + 32, INNER);
}

/* 通用卡片：图标圆底 + 标题 + 副标题 */
static lv_obj_t *card(lv_obj_t *r, int32_t x, int32_t y, int32_t w, int32_t h,
                      wxy_slot_t slot, const char *title, const char *sub)
{
    lv_obj_t *c = P2(r, x, y, w, h, WXY_R(WXY_TOK_RADIUS_CHIP));

    lv_obj_t *ci = wxy_box(c, 10, (h - 32) / 2, 32, 32, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_ACCENT));
    lv_obj_set_style_bg_opa(ci, 46, 0);
    wxy_reg(ci, wxy_apply_accent_tint);
    lv_obj_remove_flag(ci, LV_OBJ_FLAG_CLICKABLE);

    icon_at(c, slot, 16, (h - 20) / 2, 20, ic_accent, TK(WXY_TOK_COLOR_ACCENT));

    T(c, 50, sub ? 7 : (h - 18) / 2, title, WXY_TOK_COLOR_TEXT, WXY_F_BODY);
    if (sub)
        TW(c, 50, 26, w - 62, sub, WXY_TOK_COLOR_MUTED, WXY_F_TINY);

    return c;
}

/* 小按钮：图标 + 文字 */
static lv_obj_t *pill(lv_obj_t *r, int32_t x, int32_t y, int32_t w, int32_t h,
                      wxy_slot_t slot, const char *label,
                      lv_event_cb_t cb, void *data)
{
    lv_obj_t *b = P2(r, x, y, w, h, h / 2);
    lv_obj_set_style_border_width(b, 1, 0);
    lv_obj_set_style_border_color(b, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    wxy_tap(b);
    if (cb)
        lv_obj_add_event_cb(b, cb, LV_EVENT_CLICKED, data);

    icon_at(b, slot, 12, (h - 16) / 2, 16, ic_accent, TK(WXY_TOK_COLOR_ACCENT));
    T(b, 34, (h - 16) / 2 - 1, label, WXY_TOK_COLOR_TEXT, WXY_F_TINY);
    return b;
}

/* 应用视图顶栏：返回 + 标题 + 碟号 */
static void app_bar(lv_obj_t *r, const char *title, const char *num)
{
    lv_obj_t *back = wxy_box(r, PAD, 0, 26, 26, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_PANEL2));
    lv_obj_set_style_border_width(back, 1, 0);
    lv_obj_set_style_border_color(back, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    wxy_tap(back);
    lv_obj_add_event_cb(back, back_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(back, wxy_apply_panel2);
    icon_at(back, WXY_SLOT_BACK, 5, 5, 16, ic_accent, TK(WXY_TOK_COLOR_ACCENT));

    T(r, PAD + 38, 3, title, WXY_TOK_COLOR_TEXT, WXY_F_TITLE);

    char buf[16];
    rt_snprintf(buf, sizeof(buf), "DISC %s", num);
    lv_obj_t *tag = T(r, 0, 8, buf, WXY_TOK_COLOR_MUTED, WXY_F_TINY);
    lv_obj_align(tag, LV_ALIGN_TOP_RIGHT, -PAD, 8);

    hairline(r, PAD, 32, INNER);
}

/* ------------------------------------------------------------------ */
/* 事件                                                                */
/* ------------------------------------------------------------------ */

static void launch_cb(lv_event_t *e)
{
    wxy_ui_app((const char *)lv_event_get_user_data(e));
}

static void surface_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_surface(true);
}

static void voice_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_voice(true);
}

static void descend_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_descend();
}

static void return_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_return();
}

static void back_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_back();
}

static void rack_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_rack();
}

static void theme_pick_cb(lv_event_t *e)
{
    wxy_ui_theme((const char *)lv_event_get_user_data(e));
}

static void music_toggle_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_music(!wxy_ui_music_playing());
}

static void music_prev_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_music_skip(-1);
}

static void music_next_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_music_skip(1);
}

static void timer_toggle_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_timer(!wxy_ui_timer_running());
}

static void timer_reset_cb(lv_event_t *e)
{
    (void)e;
    wxy_ui_timer_reset();
}

static void pal_cb(lv_event_t *e)
{
    uint8_t act = (uint8_t)(rt_base_t)lv_event_get_user_data(e);
    wxy_pages_pal_show(act);
}

/* ------------------------------------------------------------------ */
/* 场景：对谈                                                          */
/* ------------------------------------------------------------------ */

static void build_chat(lv_obj_t *r)
{
    head_at(r, 0, "DISC 01", "Talk", "voice only");

    lv_obj_t *u = P2(r, 166, 48, 250, 36, 8);
    TW(u, 12, 9, 226, "How is the weather tonight?", WXY_TOK_COLOR_TEXT, WXY_F_TINY);

    lv_obj_t *a = P(r, PAD, 92, 320, 44, 8);
    lv_obj_set_style_border_width(a, 1, 0);
    lv_obj_set_style_border_color(a, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    TW(a, 12, 12, 296, "Checked: colder after 9 pm. Take a jacket.", WXY_TOK_COLOR_TEXT, WXY_F_TINY);

    lv_obj_t *wc = card(r, PAD, 146, INNER, 52, WXY_SLOT_WEATHER, "Weather . tonight", "tap to surface a detail");
    wxy_tap(wc);
    lv_obj_add_event_cb(wc, surface_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *ptt = card(r, PAD, 296, INNER, 48, WXY_SLOT_MIC, "Hold to talk", "ripples in place, no new page");
    wxy_tap(ptt);
    lv_obj_add_event_cb(ptt, voice_cb, LV_EVENT_CLICKED, NULL);
}

/* ------------------------------------------------------------------ */
/* 场景：桌面                                                          */
/* ------------------------------------------------------------------ */

static const struct {
    const char *id;
    const char *name;
    wxy_slot_t  slot;
} k_tiles[6] = {
    { "timer",   "Timer",   WXY_SLOT_CLOCK },
    { "weather", "Weather", WXY_SLOT_WEATHER },
    { "music",   "Music",   WXY_SLOT_MUSIC },
    { "note",    "Notes",   WXY_SLOT_NOTE },
    { "pal",     "Pal",     WXY_SLOT_HEART },
    { "rack",    "Rack",    WXY_SLOT_DISC },
};

static void build_home(lv_obj_t *r)
{
    head_at(r, 0, "DISC 02", "Desk", "every app is a disc");

    for (int i = 0; i < 6; i++)
    {
        int32_t col = i % 3, row = i / 3;
        lv_obj_t *t = P2(r, PAD + col * 142, 44 + row * 72, 132, 64,
                         WXY_R(WXY_TOK_RADIUS_CHIP));
        wxy_tap(t);

        lv_obj_t *ic = wxy_icon_create(t, k_tiles[i].slot, 24,
                                       TK(WXY_TOK_COLOR_ACCENT));
        if (ic)
        {
            lv_obj_set_pos(ic, 10, 8);
            wxy_reg(ic, k_tiles[i].slot == WXY_SLOT_HEART ? ic_pal : ic_accent);
        }

        T(t, 10, 38, k_tiles[i].name, WXY_TOK_COLOR_TEXT, WXY_F_TINY);

        if (rt_strcmp(k_tiles[i].id, "rack") == 0)
            lv_obj_add_event_cb(t, rack_cb, LV_EVENT_CLICKED, NULL);
        else
            lv_obj_add_event_cb(t, launch_cb, LV_EVENT_CLICKED, (void *)k_tiles[i].id);
    }

    T(r, PAD, 324, "Mood follows the disc . themes are data, not code", WXY_TOK_COLOR_MUTED, WXY_F_TINY);
}

/* ------------------------------------------------------------------ */
/* 场景：工具                                                          */
/* ------------------------------------------------------------------ */

static void build_tools(lv_obj_t *r)
{
    head_at(r, 0, "DISC 03", "Tools", "one card for every result");

    T(r, PAD, 44, "Every tool returns the same card, so the AI never draws its own UI",
      WXY_TOK_COLOR_MUTED, WXY_F_TINY);

    card(r, PAD, 76, INNER, 52, WXY_SLOT_CLOCK, "Timer", "timer.create . timer.query");
    card(r, PAD, 136, INNER, 52, WXY_SLOT_WEATHER, "Weather", "weather.now . weather.hourly");
    card(r, PAD, 196, INNER, 52, WXY_SLOT_NOTE, "Notes", "notes.write . notes.list");

    T(r, PAD, 300, "Registry + JSON schema land with the ABI freeze", WXY_TOK_COLOR_MUTED, WXY_F_TINY);
}

/* ------------------------------------------------------------------ */
/* 场景：设置                                                          */
/* ------------------------------------------------------------------ */

static lv_obj_t *set_row(lv_obj_t *r, int32_t y, wxy_slot_t slot, const char *label,
                         lv_event_cb_t cb, void *data)
{
    lv_obj_t *row = P2(r, PAD, y, INNER, 50, WXY_R(WXY_TOK_RADIUS_CHIP));
    wxy_tap(row);

    icon_at(row, slot, 14, 15, 20, ic_accent, TK(WXY_TOK_COLOR_ACCENT));
    T(row, 48, 15, label, WXY_TOK_COLOR_TEXT, WXY_F_BODY);
    icon_at(row, WXY_SLOT_NEXT, INNER - 34, 17, 16, ic_muted, TK(WXY_TOK_COLOR_MUTED));

    if (cb)
        lv_obj_add_event_cb(row, cb, LV_EVENT_CLICKED, data);

    return row;
}

static void build_set(lv_obj_t *r)
{
    head_at(r, 0, "DISC 04", "Set", "the frame inside the frame");

    set_row(r, 44, WXY_SLOT_EYE, "Theme and mood", descend_cb, NULL);
    set_row(r, 104, WXY_SLOT_MUSIC, "Sound and haptics", NULL, NULL);
    set_row(r, 164, WXY_SLOT_WIFI, "Network", NULL, NULL);
    set_row(r, 224, WXY_SLOT_BATTERY, "Power and sleep", NULL, NULL);
}

/* ------------------------------------------------------------------ */
/* 场景：主题（内圈）                                                   */
/* ------------------------------------------------------------------ */

static void build_theme(lv_obj_t *r)
{
    T(r, PAD, 0, "SET / THEME . the needle went one groove deeper",
      WXY_TOK_COLOR_MUTED, WXY_F_TINY);

    head_at(r, 20, "INNER 04a", "Theme", "tap to switch");

    static const char *ids[3] = { "ink", "pixel", "cyber" };
    static const char *names[3] = { "Calm . Ink", "Playful . Pixel", "Fiery . Cyber" };
    static const wxy_slot_t faces[3] = { WXY_SLOT_FACE_CALM, WXY_SLOT_FACE_HAPPY, WXY_SLOT_FACE_FIRE };

    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *row = set_row(r, 76 + i * 58, faces[i], names[i], theme_pick_cb, (void *)ids[i]);
        s_theme_check[i] = icon_at(row, WXY_SLOT_CHECK, INNER - 30, 17, 16,
                                  ic_accent, TK(WXY_TOK_COLOR_ACCENT));
    }

    lv_obj_t *back = card(r, PAD, 296, INNER, 44, WXY_SLOT_BACK, "Back to settings",
                          "the needle returns to the outer groove");
    wxy_tap(back);
    lv_obj_add_event_cb(back, return_cb, LV_EVENT_CLICKED, NULL);
}

/* ------------------------------------------------------------------ */
/* 应用：音乐                                                          */
/* ------------------------------------------------------------------ */

static void build_music(lv_obj_t *r)
{
    app_bar(r, "Music", "M1");

    lv_obj_t *cover = P2(r, PAD, 44, 150, 150, WXY_R(WXY_TOK_RADIUS_CARD));
    lv_obj_t *disc = wxy_box(cover, 27, 27, 96, 96, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_BG));
    lv_obj_set_style_border_width(disc, 1, 0);
    lv_obj_set_style_border_color(disc, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    lv_obj_remove_flag(disc, LV_OBJ_FLAG_CLICKABLE);
    wxy_reg(disc, wxy_apply_bg);
    lv_obj_t *core = wxy_box(disc, 33, 33, 30, 30, LV_RADIUS_CIRCLE, wxy_disc_hue("music"));
    lv_obj_remove_flag(core, LV_OBJ_FLAG_CLICKABLE);

    s_music_title = TW(r, 182, 46, 250, "Night Passage", WXY_TOK_COLOR_TEXT, WXY_F_BODY);
    s_music_artist = TW(r, 182, 72, 250, "Wander . ambient", WXY_TOK_COLOR_MUTED, WXY_F_TINY);

    lv_obj_t *ly = TW(r, 182, 100, 250,
                      "Night falls on the record\nround and round it turns\nstay until the morning",
                      WXY_TOK_COLOR_MUTED, WXY_F_TINY);
    lv_obj_set_style_text_line_space(ly, 6, 0);

    lv_obj_t *bar = wxy_box(r, 182, 210, 250, 3, 2, TK(WXY_TOK_COLOR_LINE));
    lv_obj_remove_flag(bar, LV_OBJ_FLAG_CLICKABLE);
    wxy_reg(bar, wxy_apply_line);

    s_music_bar = wxy_box(bar, 0, 0, 60, 3, 2, TK(WXY_TOK_COLOR_ACCENT));
    lv_obj_remove_flag(s_music_bar, LV_OBJ_FLAG_CLICKABLE);
    wxy_reg(s_music_bar, wxy_apply_accent);

    s_music_cur = T(r, 182, 218, "00:42", WXY_TOK_COLOR_MUTED, WXY_F_TINY);
    s_music_dur = T(r, 0, 218, "03:20", WXY_TOK_COLOR_MUTED, WXY_F_TINY);
    lv_obj_align(s_music_dur, LV_ALIGN_TOP_RIGHT, -PAD - 20, 218);

    /* 控制条 */
    lv_obj_t *prev = wxy_box(r, 146, 262, 42, 42, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_PANEL2));
    lv_obj_set_style_border_width(prev, 1, 0);
    lv_obj_set_style_border_color(prev, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    wxy_tap(prev);
    lv_obj_add_event_cb(prev, music_prev_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(prev, wxy_apply_panel2);
    icon_at(prev, WXY_SLOT_PREV, 13, 13, 16, ic_text, TK(WXY_TOK_COLOR_TEXT));

    lv_obj_t *main = wxy_box(r, 198, 256, 54, 54, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_PANEL2));
    lv_obj_set_style_border_width(main, 1, 0);
    lv_obj_set_style_border_color(main, wxy_col(TK(WXY_TOK_COLOR_ACCENT)), 0);
    wxy_tap(main);
    lv_obj_add_event_cb(main, music_toggle_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(main, wxy_apply_panel2);
    s_music_main = main;
    s_music_main_icon = icon_at(main, WXY_SLOT_PAUSE, 19, 19, 16, ic_accent, TK(WXY_TOK_COLOR_ACCENT));

    lv_obj_t *next = wxy_box(r, 262, 262, 42, 42, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_PANEL2));
    lv_obj_set_style_border_width(next, 1, 0);
    lv_obj_set_style_border_color(next, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    wxy_tap(next);
    lv_obj_add_event_cb(next, music_next_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(next, wxy_apply_panel2);
    icon_at(next, WXY_SLOT_NEXT, 13, 13, 16, ic_text, TK(WXY_TOK_COLOR_TEXT));
}

/* ------------------------------------------------------------------ */
/* 应用：计时                                                          */
/* ------------------------------------------------------------------ */

static void timer_arc_apply(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_arc_color(o, wxy_col(t->color[WXY_TOK_COLOR_LINE]), LV_PART_MAIN);
    lv_obj_set_style_arc_color(o, wxy_col(t->color[WXY_TOK_COLOR_ACCENT]), LV_PART_INDICATOR);
}

static void build_timer(lv_obj_t *r)
{
    app_bar(r, "Timer", "T1");

    s_timer_ring = lv_arc_create(r);
    lv_obj_set_size(s_timer_ring, 188, 188);
    lv_obj_set_pos(s_timer_ring, (WELL_W - 188) / 2, 48);
    lv_obj_set_style_bg_opa(s_timer_ring, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(s_timer_ring, 0, 0);
    lv_obj_set_style_arc_width(s_timer_ring, 10, LV_PART_MAIN);
    lv_obj_set_style_arc_width(s_timer_ring, 10, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(s_timer_ring, true, LV_PART_INDICATOR);
    lv_obj_set_style_opa(s_timer_ring, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_arc_set_rotation(s_timer_ring, 270);
    lv_arc_set_bg_angles(s_timer_ring, 0, 360);
    lv_arc_set_angles(s_timer_ring, 0, 0);
    wxy_reg(s_timer_ring, timer_arc_apply);

    s_timer_val = wxy_text(s_timer_ring, 0, 78, "00:00.0", TK(WXY_TOK_COLOR_TEXT), WXY_F_DISP);
    lv_obj_set_width(s_timer_val, 188);
    lv_obj_set_style_text_align(s_timer_val, LV_TEXT_ALIGN_CENTER, 0);
    wxy_reg(s_timer_val, wxy_apply_text);

    lv_obj_t *b = P2(r, 146, 262, 160, 44, 22);
    lv_obj_set_style_border_width(b, 1, 0);
    lv_obj_set_style_border_color(b, wxy_col(TK(WXY_TOK_COLOR_ACCENT)), 0);
    wxy_tap(b);
    lv_obj_add_event_cb(b, timer_toggle_cb, LV_EVENT_CLICKED, NULL);

    s_timer_icon = wxy_icon_create(b, WXY_SLOT_PLAY, 16, TK(WXY_TOK_COLOR_ACCENT));
    if (s_timer_icon)
    {
        lv_obj_set_pos(s_timer_icon, 40, 14);
        wxy_reg(s_timer_icon, ic_accent);
    }
    s_timer_tx = T(b, 62, 13, "Start", WXY_TOK_COLOR_TEXT, WXY_F_TINY);

    lv_obj_t *rst = wxy_box(r, 314, 262, 44, 44, LV_RADIUS_CIRCLE, TK(WXY_TOK_COLOR_PANEL2));
    lv_obj_set_style_border_width(rst, 1, 0);
    lv_obj_set_style_border_color(rst, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    wxy_tap(rst);
    lv_obj_add_event_cb(rst, timer_reset_cb, LV_EVENT_CLICKED, NULL);
    wxy_reg(rst, wxy_apply_panel2);
    icon_at(rst, WXY_SLOT_REFRESH, 13, 13, 16, ic_muted, TK(WXY_TOK_COLOR_MUTED));
}

/* ------------------------------------------------------------------ */
/* 应用：天气                                                          */
/* ------------------------------------------------------------------ */

static void build_weather(lv_obj_t *r)
{
    app_bar(r, "Weather", "W1");

    icon_at(r, WXY_SLOT_WEATHER, PAD, 46, 40, ic_accent, TK(WXY_TOK_COLOR_ACCENT));

    T(r, 66, 44, "18", WXY_TOK_COLOR_TEXT, WXY_F_DISP);
    T(r, 100, 48, "o", WXY_TOK_COLOR_MUTED, WXY_F_BODY);

    lv_obj_t *desc = TW(r, 176, 48, 240, "Cloudy then clear\nfeels 16 . NE wind 2", WXY_TOK_COLOR_MUTED, WXY_F_TINY);
    lv_obj_set_style_text_line_space(desc, 4, 0);

    static const char *hours[3] = { "now", "21h", "22h" };
    static const char *temps[3] = { "18", "17", "16" };
    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *cell = P2(r, PAD + i * 142, 124, 132, 64, WXY_R(WXY_TOK_RADIUS_CHIP));
        T(cell, 10, 8, hours[i], WXY_TOK_COLOR_MUTED, WXY_F_TINY);
        icon_at(cell, i == 0 ? WXY_SLOT_WEATHER : WXY_SLOT_CLOUD, 10, 28, 16,
                ic_accent, TK(WXY_TOK_COLOR_ACCENT));
        T(cell, 34, 27, temps[i], WXY_TOK_COLOR_TEXT, WXY_F_BODY);
    }

    static const char *days[3] = { "Tomorrow", "Day after", "Thu" };
    static const char *hi[3] = { "21", "23", "24" };
    static const char *lo[3] = { "12", "13", "14" };
    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *row = P2(r, PAD, 200 + i * 36, INNER, 32, WXY_R(WXY_TOK_RADIUS_CHIP));
        icon_at(row, WXY_SLOT_WEATHER, 8, 8, 16, ic_accent, TK(WXY_TOK_COLOR_ACCENT));
        T(row, 32, 7, days[i], WXY_TOK_COLOR_MUTED, WXY_F_TINY);

        lv_obj_t *l = T(row, 0, 7, lo[i], WXY_TOK_COLOR_MUTED, WXY_F_TINY);
        lv_obj_align(l, LV_ALIGN_RIGHT_MID, -48, 0);
        lv_obj_t *h = T(row, 0, 7, hi[i], WXY_TOK_COLOR_TEXT, WXY_F_TINY);
        lv_obj_align(h, LV_ALIGN_RIGHT_MID, -14, 0);
    }
}

/* ------------------------------------------------------------------ */
/* 应用：笔记                                                          */
/* ------------------------------------------------------------------ */

static void build_note(lv_obj_t *r)
{
    app_bar(r, "Notes", "N1");

    static const char *notes[3] = { "Tonight: read a chapter, water the plants",
                                    "Idea: theme follows the mood",
                                    "Buy coffee beans, cable, new stylus" };
    static const char *times[3] = { "21:04", "19:30", "yesterday" };

    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *it = P2(r, PAD, 44 + i * 46, INNER, 40, WXY_R(WXY_TOK_RADIUS_CHIP));
        lv_obj_set_style_border_width(it, 1, 0);
        lv_obj_set_style_border_color(it, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
        wxy_tap(it);

        TW(it, 12, 4, 300, notes[i], WXY_TOK_COLOR_TEXT, WXY_F_TINY);
        lv_obj_t *t = T(it, 0, 22, times[i], WXY_TOK_COLOR_MUTED, WXY_F_TINY);
        lv_obj_align(t, LV_ALIGN_TOP_RIGHT, -12, 22);
    }

    lv_obj_t *ta = lv_textarea_create(r);
    lv_obj_set_pos(ta, PAD, 190);
    lv_obj_set_size(ta, INNER, 60);
    lv_obj_set_style_bg_color(ta, wxy_col(TK(WXY_TOK_COLOR_PANEL)), 0);
    lv_obj_set_style_bg_opa(ta, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(ta, WXY_R(WXY_TOK_RADIUS_CHIP), 0);
    lv_obj_set_style_border_width(ta, 1, 0);
    lv_obj_set_style_border_color(ta, wxy_col(TK(WXY_TOK_COLOR_LINE)), 0);
    lv_obj_set_style_text_color(ta, wxy_col(TK(WXY_TOK_COLOR_TEXT)), 0);
    lv_obj_set_style_text_font(ta, WXY_F_TINY, 0);
    lv_obj_set_style_pad_all(ta, 10, 0);
    lv_obj_set_style_bg_opa(ta, LV_OPA_COVER, LV_PART_CURSOR);
    lv_obj_set_style_bg_color(ta, wxy_col(TK(WXY_TOK_COLOR_ACCENT)), LV_PART_CURSOR);
    lv_textarea_set_text(ta, "Read one chapter, then sleep.");
    wxy_reg(ta, wxy_apply_panel);
}

/* ------------------------------------------------------------------ */
/* 应用：陪伴                                                          */
/* ------------------------------------------------------------------ */

static void build_pal(lv_obj_t *r)
{
    app_bar(r, "Pal", "C1");
    s_pal_root = r;

    s_pal_face = wxy_icon_create(r, WXY_SLOT_FACE_CALM, 96, wxy_disc_hue("pal"));
    if (s_pal_face)
    {
        lv_obj_set_pos(s_pal_face, (WELL_W - 96) / 2, 54);
        wxy_reg(s_pal_face, ic_pal);
    }

    s_pal_state = wxy_text(r, 0, 166, "Quietly here with you", TK(WXY_TOK_COLOR_TEXT), WXY_F_BODY);
    lv_obj_set_width(s_pal_state, WELL_W);
    lv_obj_set_style_text_align(s_pal_state, LV_TEXT_ALIGN_CENTER, 0);
    wxy_reg(s_pal_state, wxy_apply_text);

    s_pal_hint = wxy_text(r, 0, 190, "mood: calm . same source as the theme",
                          TK(WXY_TOK_COLOR_MUTED), WXY_F_TINY);
    lv_obj_set_width(s_pal_hint, WELL_W);
    lv_obj_set_style_text_align(s_pal_hint, LV_TEXT_ALIGN_CENTER, 0);
    wxy_reg(s_pal_hint, wxy_apply_muted);

    pill(r, PAD, 268, 120, 36, WXY_SLOT_HEART, "Pet", pal_cb, (void *)0);
    pill(r, 166, 268, 120, 36, WXY_SLOT_CHAT, "Talk", pal_cb, (void *)1);
    pill(r, 314, 268, 120, 36, WXY_SLOT_PLAY, "Play", pal_cb, (void *)2);
}

/* ------------------------------------------------------------------ */
/* 组装                                                                */
/* ------------------------------------------------------------------ */

void wxy_pages_create(lv_obj_t *well)
{
    s_well = well;
    wxy_scene_count = 0;

    build_chat(scene_begin("chat"));
    build_home(scene_begin("home"));
    build_tools(scene_begin("tools"));
    build_set(scene_begin("set"));
    build_theme(scene_begin("theme"));
    build_music(scene_begin("music"));
    build_timer(scene_begin("timer"));
    build_weather(scene_begin("weather"));
    build_note(scene_begin("note"));
    build_pal(scene_begin("pal"));

    LOG_I("scenes ready (%d)", wxy_scene_count);
}

wxy_scene_t *wxy_scene_get(const char *id)
{
    if (id == NULL)
        return NULL;
    for (uint8_t i = 0; i < wxy_scene_count; i++)
    {
        if (rt_strcmp(wxy_scenes[i].id, id) == 0)
            return &wxy_scenes[i];
    }
    return NULL;
}

void wxy_pages_refresh(void)
{
    const wxy_theme_t *th = wxy_theme_current();
    for (int i = 0; i < 3; i++)
    {
        if (s_theme_check[i] == NULL)
            continue;
        bool on = rt_strcmp(wxy_themes[i].id, th->id) == 0;
        if (on)
            lv_obj_remove_flag(s_theme_check[i], LV_OBJ_FLAG_HIDDEN);
        else
            lv_obj_add_flag(s_theme_check[i], LV_OBJ_FLAG_HIDDEN);
    }
}

void wxy_pages_timer_show(uint32_t ms, bool running)
{
    if (s_timer_val == NULL)
        return;

    char buf[16];
    rt_snprintf(buf, sizeof(buf), "%02d:%02d.%d",
                (int)(ms / 60000), (int)((ms / 1000) % 60), (int)((ms / 100) % 10));
    lv_label_set_text(s_timer_val, buf);

    uint32_t deg = (ms % 60000) * 360 / 60000;
    lv_arc_set_angles(s_timer_ring, 0, (int32_t)deg);

    if (s_timer_tx)
        lv_label_set_text(s_timer_tx, running ? "Pause" : "Start");

    /* 只有播放态变化时才重建图标：这个函数每 100ms 被调一次，
     * 无条件重建会持续制造堆碎片。 */
    static bool shown_run;
    static bool shown_once;

    if (!shown_once || shown_run != running)
    {
        shown_once = true;
        shown_run = running;

        if (s_timer_icon)
            lv_obj_delete(s_timer_icon);

        s_timer_icon = wxy_icon_create(lv_obj_get_parent(s_timer_tx),
                                       running ? WXY_SLOT_PAUSE : WXY_SLOT_PLAY, 16,
                                       TK(WXY_TOK_COLOR_ACCENT));
        if (s_timer_icon)
        {
            lv_obj_set_pos(s_timer_icon, 40, 14);
            wxy_reg(s_timer_icon, ic_accent);
        }
    }
}

void wxy_pages_music_show(const char *title, const char *artist,
                          const char *cur, const char *dur, uint8_t pct)
{
    if (s_music_title == NULL)
        return;

    if (title)
        lv_label_set_text(s_music_title, title);
    if (artist)
        lv_label_set_text(s_music_artist, artist);
    if (cur)
        lv_label_set_text(s_music_cur, cur);
    if (dur)
        lv_label_set_text(s_music_dur, dur);

    lv_obj_set_width(s_music_bar, (250 * (int32_t)pct) / 100);
}

void wxy_pages_music_state(bool playing)
{
    if (s_music_main == NULL)
        return;

    if (s_music_main_icon)
        lv_obj_delete(s_music_main_icon);

    s_music_main_icon = wxy_icon_create(s_music_main,
                                        playing ? WXY_SLOT_PAUSE : WXY_SLOT_PLAY,
                                        16, TK(WXY_TOK_COLOR_ACCENT));
    if (s_music_main_icon)
    {
        lv_obj_set_pos(s_music_main_icon, 19, 19);
        wxy_reg(s_music_main_icon, ic_accent);
    }
}

void wxy_pages_pal_show(uint8_t act)
{
    static const wxy_slot_t faces[3] = { WXY_SLOT_FACE_HAPPY, WXY_SLOT_FACE_CALM, WXY_SLOT_FACE_FIRE };
    static const char *state[3] = { "Petted. That felt nice", "Listening, take your time", "Let us play something loud" };
    static const char *hint[3] = { "mood: playful . in tune with you", "mood: calm . quiet company", "mood: fiery . let us run" };

    if (act > 2 || s_pal_face == NULL)
        return;

    lv_obj_delete(s_pal_face);
    s_pal_face = wxy_icon_create(s_pal_root, faces[act], 96, wxy_disc_hue("pal"));
    if (s_pal_face)
    {
        lv_obj_set_pos(s_pal_face, (WELL_W - 96) / 2, 54);
        wxy_reg(s_pal_face, ic_pal);
    }

    lv_label_set_text(s_pal_state, state[act]);
    lv_label_set_text(s_pal_hint, hint[act]);
}
