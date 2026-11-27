/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * 换碟唱机实现：碟片身份、唱臂物理模型、轨道点、角落小 CD、碟仓、走带区。
 *
 * 坐标全部来自 wxy_kit.h，本文件不出现魔法数字以外的色值 ——
 * 唯一的例外是每张碟的 hue（碟面主色），那是"碟的身份"，不属于主题。
 */

#include <string.h>

#include <rtthread.h>

#include "wxy_kit.h"
#include "wxy_disc.h"
#include "wxy_icon.h"

#define LOG_TAG "WXY.TT"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

/* ------------------------------------------------------------------ */
/* 十张碟                                                              */
/* ------------------------------------------------------------------ */

const wxy_disc_t wxy_discs[] = {
    /* id          num    name      slot                    hue        arm  kind */
    { "chat",      "01",  "Talk",    WXY_SLOT_CHAT,      0xE0B877, 132, WXY_DISC_KIND_TRACK },
    { "home",      "02",  "Desk",    WXY_SLOT_HOME,      0x8FC7AA, 138, WXY_DISC_KIND_TRACK },
    { "tools",     "03",  "Tools",   WXY_SLOT_TOOLS,     0x9BD465, 144, WXY_DISC_KIND_TRACK },
    { "set",       "04",  "Set",     WXY_SLOT_SET,       0x7FA6D8, 149, WXY_DISC_KIND_TRACK },
    { "theme",     "04a", "Theme",   WXY_SLOT_EYE,       0xCECBF6, 152, WXY_DISC_KIND_INNER },
    { "music",     "M1",  "Music",   WXY_SLOT_MUSIC,     0xF6C86A, 136, WXY_DISC_KIND_APP },
    { "timer",     "T1",  "Timer",   WXY_SLOT_CLOCK,     0x7FA6D8, 142, WXY_DISC_KIND_APP },
    { "weather",   "W1",  "Weather", WXY_SLOT_WEATHER,   0xEFAF34, 146, WXY_DISC_KIND_APP },
    { "note",      "N1",  "Notes",   WXY_SLOT_NOTE,      0xCECBF6, 149, WXY_DISC_KIND_APP },
    { "pal",       "C1",  "Pal",     WXY_SLOT_HEART,     0xE2758F, 134, WXY_DISC_KIND_APP },
};

const uint8_t WXY_DISC_COUNT = (uint8_t)(sizeof(wxy_discs) / sizeof(wxy_discs[0]));

#define TRACK_COUNT 5   /* 外圈五个轨道点 */
#define RING_COUNT  7

const wxy_disc_t *wxy_disc_get(const char *id)
{
    if (id == NULL)
        return NULL;

    for (uint8_t i = 0; i < WXY_DISC_COUNT; i++)
    {
        if (rt_strcmp(wxy_discs[i].id, id) == 0)
            return &wxy_discs[i];
    }
    return NULL;
}

const wxy_disc_t *wxy_disc_nth(uint8_t i)
{
    return (i < WXY_DISC_COUNT) ? &wxy_discs[i] : NULL;
}

uint32_t wxy_disc_hue(const char *id)
{
    const wxy_disc_t *d = wxy_disc_get(id);
    return (d != NULL) ? d->hue : 0xFFFFFF;
}

/* ------------------------------------------------------------------ */
/* 控件集合                                                            */
/* ------------------------------------------------------------------ */

static struct {
    lv_obj_t *world;
    /* WXY: glow[] 已移除（alpha 光晕改预烘焙图片，见 F-DISP 契约） */

    lv_obj_t *disc;
    lv_obj_t *ring[RING_COUNT];
    lv_obj_t *label;
    lv_obj_t *label_icon;
    lv_obj_t *label_num;
    lv_obj_t *label_name;
    lv_obj_t *read_dot;

    lv_obj_t *orbit_dot[TRACK_COUNT];
    lv_obj_t *orbit_icon[TRACK_COUNT];
    const wxy_disc_t *orbit_map[TRACK_COUNT];

    lv_obj_t *arm_line;
    lv_obj_t *arm_head;
    lv_obj_t *arm_tip;
    lv_obj_t *pivot;

    lv_obj_t *rack;
    lv_obj_t *rack_cd;
    lv_obj_t *rack_icon;
    lv_obj_t *rack_name;
    lv_obj_t *rack_num;

    lv_obj_t *tray;
    lv_obj_t *tray_head_cur;
    lv_obj_t *tray_item[WXY_DISC_MAX];
    lv_obj_t *tray_icon[WXY_DISC_MAX];
    bool      tray_built;

    lv_obj_t *track_num;
    lv_obj_t *track_name;
    lv_obj_t *ruler_dot[TRACK_COUNT];

    lv_obj_t *play_icon;
} s_tt;

static lv_obj_t *s_screen;        /* 唱机挂在屏幕根上，便于把唱臂抬到内容层之上 */
static const wxy_disc_t *s_cur;
static const wxy_disc_t *s_swap_target;
static const char *s_label_override;

static wxy_tt_cb_t s_cb_prev;
static wxy_tt_cb_t s_cb_play;
static wxy_tt_cb_t s_cb_next;
static wxy_tt_pick_cb_t s_cb_pick;

static int32_t s_arm_tenth;      /* 唱针当前角度，0.1 度 */
static bool    s_swap_painted;
static bool    s_swapping;      /* 换碟动画进行中：此时不要直接改唱针角度 */
static bool    s_playing;
static bool    s_tray_open;
static uint8_t s_progress;

static wxy_tt_cb_t s_swap_done;  /* 换碟结束回调（同一时刻只允许一次换碟） */

/* 走带区上方的渐变遮罩：只改颜色，保留各自的透明度 */
static void scrim_apply(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_BG]), 0);
}

static void swap_done_timer(lv_timer_t *t)
{
    (void)t;
    wxy_tt_cb_t cb = s_swap_done;
    s_swap_done = NULL;
    if (cb)
        cb();
}

/* ------------------------------------------------------------------ */
/* 唱臂                                                                */
/* ------------------------------------------------------------------ */

static void arm_apply(int32_t deg10)
{
    if (s_tt.arm_line == NULL)
        return;

    int32_t c = lv_trigo_cos((int16_t)deg10);
    int32_t s = lv_trigo_sin((int16_t)deg10);
    int32_t hx = WXY_ARM_PX + (WXY_ARM_LEN * c) / 32767;
    int32_t hy = WXY_ARM_PY + (WXY_ARM_LEN * s) / 32767;

    lv_point_precise_t pts[2] = { { WXY_ARM_PX, WXY_ARM_PY }, { hx, hy } };
    lv_line_set_points(s_tt.arm_line, pts, 2);

    lv_obj_set_pos(s_tt.arm_head, hx - 17, hy - 13);
    if (s_tt.arm_tip)
        lv_obj_set_pos(s_tt.arm_tip, hx - 3, hy + 9);
}

static void arm_exec(void *var, int32_t v)
{
    (void)var;
    s_arm_tenth = v;
    arm_apply(v);
}

static void arm_to(int32_t deg10, uint16_t ms)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, &s_arm_tenth);
    lv_anim_set_exec_cb(&a, arm_exec);
    lv_anim_set_values(&a, s_arm_tenth, deg10);
    lv_anim_set_duration(&a, ms);
    lv_anim_set_path_cb(&a, WXY_TOK()->path_swap);
    lv_anim_start(&a);
}

/* 读纹光点：按进度在盘面上绕一圈 */
static void read_dot_place(uint8_t pct)
{
    if (s_tt.read_dot == NULL)
        return;

    int16_t ang = (int16_t)(2400 + pct * 80);        /* 240deg -> 320deg，0.1 度 */
    int32_t c = lv_trigo_cos(ang);
    int32_t s = lv_trigo_sin(ang);
    int32_t r = 198;

    int32_t cx = WXY_DISC_CX + (r * c) / 32767;
    int32_t cy = WXY_DISC_CY + (r * s) / 32767;

    lv_obj_set_pos(s_tt.read_dot, cx - 4, cy - 4);
}

/* ------------------------------------------------------------------ */
/* 涂装                                                                */
/* ------------------------------------------------------------------ */

static void label_icon_set(wxy_slot_t slot, uint32_t color, int32_t px)
{
    if (s_tt.label_icon)
        lv_obj_delete(s_tt.label_icon);

    s_tt.label_icon = wxy_icon_create(s_tt.label, slot, px, color);
    if (s_tt.label_icon)
        lv_obj_set_pos(s_tt.label_icon, (156 - px) / 2, 26);
}

static void rack_icon_set(wxy_slot_t slot, uint32_t color)
{
    if (s_tt.rack_icon)
        lv_obj_delete(s_tt.rack_icon);

    s_tt.rack_icon = wxy_icon_create(s_tt.rack_cd, slot, 16, color);
    if (s_tt.rack_icon)
        lv_obj_set_pos(s_tt.rack_icon, 9, 9);
}

/* 与主题/碟身份相关的所有颜色，集中一处刷新：
 * 主题引擎只登记一个回调，就能把整台唱机的配色带过来。 */
static void tt_colors(const wxy_tokens_t *t)
{
    uint32_t hue = s_cur ? s_cur->hue : 0xE0B877;

    uint32_t disc_bg  = wxy_mix(t->color[WXY_TOK_COLOR_PANEL], 0x000000, 168);
    uint32_t groove   = wxy_mix(disc_bg, t->color[WXY_TOK_COLOR_TEXT], 26);
    uint32_t label_bg = wxy_mix(hue, 0x000000, 78);
    uint32_t deep     = wxy_mix(hue, 0x000000, 40);

    /* 碟身 */
    lv_obj_set_style_bg_color(s_tt.disc, wxy_col(disc_bg), 0);
    lv_obj_set_style_border_color(s_tt.disc, wxy_col(t->color[WXY_TOK_COLOR_LINE]), 0);

    for (int i = 0; i < RING_COUNT; i++)
    {
        lv_obj_set_style_border_color(s_tt.ring[i], wxy_col(groove), 0);
        lv_obj_set_style_border_opa(s_tt.ring[i], 120, 0);
    }

    /* 碟心标签：底色由碟的 hue 决定，字用深色保证对比 */
    lv_obj_set_style_bg_color(s_tt.label, wxy_col(label_bg), 0);
    lv_obj_set_style_border_color(s_tt.label, wxy_col(deep), 0);

    if (s_tt.label_num)
        lv_obj_set_style_text_color(s_tt.label_num, wxy_col(0x1B1A16), 0);
    if (s_tt.label_name)
        lv_obj_set_style_text_color(s_tt.label_name, wxy_col(0x1B1A16), 0);

    if (s_tt.label_icon)
        wxy_icon_recolor(s_tt.label_icon, 0x1B1A16);

    /* WXY: 光晕已移除，染色循环同步删除（见上） */

    /* 读纹光点 */
    if (s_tt.read_dot)
        lv_obj_set_style_bg_color(s_tt.read_dot, wxy_col(hue), 0);

    /* 轨道点：当前碟点亮 */
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        bool on = s_cur && s_tt.orbit_map[i] && rt_strcmp(s_tt.orbit_map[i]->id, s_cur->id) == 0;
        lv_obj_set_style_border_color(s_tt.orbit_dot[i],
                                      wxy_col(on ? hue : t->color[WXY_TOK_COLOR_LINE]), 0);
        lv_obj_set_style_bg_opa(s_tt.orbit_dot[i], on ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_color(s_tt.orbit_dot[i], wxy_col(t->color[WXY_TOK_COLOR_PANEL2]), 0);
        if (s_tt.orbit_icon[i])
            wxy_icon_recolor(s_tt.orbit_icon[i],
                             on ? hue : t->color[WXY_TOK_COLOR_MUTED]);
    }

    /* 走带刻度 */
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        bool on = s_cur && s_tt.orbit_map[i] && rt_strcmp(s_tt.orbit_map[i]->id, s_cur->id) == 0;
        lv_obj_set_style_bg_color(s_tt.ruler_dot[i],
                                  wxy_col(on ? hue : t->color[WXY_TOK_COLOR_LINE]), 0);
    }

    /* 角落小 CD */
    lv_obj_set_style_bg_color(s_tt.rack, wxy_col(t->color[WXY_TOK_COLOR_PANEL]), 0);
    lv_obj_set_style_border_color(s_tt.rack, wxy_col(hue), 0);
    lv_obj_set_style_border_opa(s_tt.rack, 120, 0);
    lv_obj_set_style_bg_color(s_tt.rack_cd, wxy_col(deep), 0);
    if (s_tt.rack_icon)
        wxy_icon_recolor(s_tt.rack_icon, 0x1B1A16);

    /* 碟仓 */
    if (s_tt.tray)
    {
        lv_obj_set_style_bg_color(s_tt.tray, wxy_col(t->color[WXY_TOK_COLOR_PANEL]), 0);
        lv_obj_set_style_border_color(s_tt.tray, wxy_col(hue), 0);
    }

    for (uint8_t i = 0; i < WXY_DISC_COUNT; i++)
    {
        if (s_tt.tray_item[i] == NULL)
            continue;

        bool on = s_cur && rt_strcmp(wxy_discs[i].id, s_cur->id) == 0;
        lv_obj_set_style_border_color(s_tt.tray_item[i],
                                      wxy_col(on ? wxy_discs[i].hue : t->color[WXY_TOK_COLOR_LINE]), 0);
        lv_obj_set_style_bg_color(s_tt.tray_item[i],
                                  wxy_col(on ? wxy_mix(wxy_discs[i].hue, t->color[WXY_TOK_COLOR_PANEL2], 40)
                                             : t->color[WXY_TOK_COLOR_PANEL2]), 0);
        if (s_tt.tray_icon[i])
            wxy_icon_recolor(s_tt.tray_icon[i],
                             on ? wxy_discs[i].hue : t->color[WXY_TOK_COLOR_MUTED]);
    }

    /* 走带文字 */
    if (s_tt.track_num)
        lv_obj_set_style_text_color(s_tt.track_num, wxy_col(hue), 0);
    if (s_tt.track_name)
        lv_obj_set_style_text_color(s_tt.track_name, wxy_col(t->color[WXY_TOK_COLOR_MUTED]), 0);
}

static void tt_apply_cb(lv_obj_t *obj, const wxy_tokens_t *t)
{
    (void)obj;
    tt_colors(t);
}

static void paint(const wxy_disc_t *d)
{
    if (d == NULL)
        return;

    s_cur = d;
    tt_colors(WXY_TOK());

    /* 碟面文字 */
    char buf[24];
    rt_snprintf(buf, sizeof(buf), "DISC %s", d->num);
    lv_label_set_text(s_tt.label_num, buf);
    lv_label_set_text(s_tt.label_name, s_label_override ? s_label_override : d->name);

    /* 碟面图标：槽位换符号，需要重建 */
    label_icon_set(d->slot, 0x1B1A16, 32);

    /* 角落小 CD */
    rack_icon_set(d->slot, 0x1B1A16);
    lv_label_set_text(s_tt.rack_name, s_label_override ? s_label_override : d->name);
    rt_snprintf(buf, sizeof(buf), "DISC %s", d->num);
    lv_label_set_text(s_tt.rack_num, buf);

    /* 走带区 */
    rt_snprintf(buf, sizeof(buf), "DISC %s", d->num);
    lv_label_set_text(s_tt.track_num, buf);
    rt_snprintf(buf, sizeof(buf), "%s . %s", d->num, s_label_override ? s_label_override : d->name);
    lv_label_set_text(s_tt.track_name, buf);

    if (s_tt.tray_head_cur)
    {
        rt_snprintf(buf, sizeof(buf), "now %s", d->num);
        lv_label_set_text(s_tt.tray_head_cur, buf);
    }

    /* 换碟过程中角度由 swap_ready 收尾，这里直接落针会和抬针动画打架 */
    if (!s_swapping)
    {
        s_arm_tenth = (int32_t)d->arm_deg * 10;
        arm_apply(s_arm_tenth);
    }
    read_dot_place(s_progress);
}

/* ------------------------------------------------------------------ */
/* 碟仓                                                                */
/* ------------------------------------------------------------------ */

static void tray_pick_cb(lv_event_t *e)
{
    const wxy_disc_t *d = (const wxy_disc_t *)lv_event_get_user_data(e);
    wxy_tt_tray(false);
    if (s_cb_pick && d)
        s_cb_pick(d);
}

static void tray_build(void)
{
    if (s_tt.tray_built)
        return;

    s_tt.tray_built = true;

    lv_obj_t *cap = wxy_text(s_tt.tray, 13, 10, "RACK . tap a disc", WXY_TOK()->color[WXY_TOK_COLOR_MUTED], WXY_F_TINY);
    wxy_reg(cap, wxy_apply_muted);

    s_tt.tray_head_cur = wxy_text(s_tt.tray, 240, 10, "now", WXY_TOK()->color[WXY_TOK_COLOR_MUTED], WXY_F_TINY);
    wxy_reg(s_tt.tray_head_cur, wxy_apply_muted);

    for (uint8_t i = 0; i < WXY_DISC_COUNT; i++)
    {
        int32_t col = i % 5, row = i / 5;
        int32_t x = 12 + col * 64, y = 34 + row * 66;

        lv_obj_t *it = wxy_box(s_tt.tray, x, y, 60, 62, 8, WXY_TOK()->color[WXY_TOK_COLOR_PANEL2]);
        lv_obj_set_style_border_width(it, 1, 0);
        lv_obj_set_style_border_color(it, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_LINE]), 0);
        wxy_tap(it);
        lv_obj_add_event_cb(it, tray_pick_cb, LV_EVENT_CLICKED, (void *)&wxy_discs[i]);

        lv_obj_t *cd = wxy_box(it, 13, 4, 34, 34, LV_RADIUS_CIRCLE,
                               wxy_mix(wxy_discs[i].hue, 0x000000, 180));
        lv_obj_remove_flag(cd, LV_OBJ_FLAG_CLICKABLE);

        s_tt.tray_icon[i] = wxy_icon_create(it, wxy_discs[i].slot, 16, WXY_TOK()->color[WXY_TOK_COLOR_MUTED]);
        if (s_tt.tray_icon[i])
            lv_obj_set_pos(s_tt.tray_icon[i], 22, 13);

        lv_obj_t *nm = wxy_text(it, 0, 42, wxy_discs[i].name,
                                WXY_TOK()->color[WXY_TOK_COLOR_MUTED], WXY_F_TINY);
        lv_obj_set_width(nm, 60);
        lv_obj_set_style_text_align(nm, LV_TEXT_ALIGN_CENTER, 0);

        s_tt.tray_item[i] = it;
    }

    tt_colors(WXY_TOK());
}

void wxy_tt_tray(bool open)
{
    if (open)
        tray_build();

    if (s_tt.tray == NULL)
        return;

    s_tray_open = open;

    if (open)
    {
        lv_obj_remove_flag(s_tt.tray, LV_OBJ_FLAG_HIDDEN);
        wxy_fade_slide(s_tt.tray, 103, 112, 0, 255, 220, WXY_TOK()->path_enter, NULL);
    }
    else
    {
        lv_obj_add_flag(s_tt.tray, LV_OBJ_FLAG_HIDDEN);
    }

    tt_colors(WXY_TOK());
}

bool wxy_tt_tray_open(void)
{
    return s_tray_open;
}

/* ------------------------------------------------------------------ */
/* 事件                                                                */
/* ------------------------------------------------------------------ */

static void orbit_cb(lv_event_t *e)
{
    const wxy_disc_t *d = (const wxy_disc_t *)lv_event_get_user_data(e);
    if (s_cb_pick && d)
        s_cb_pick(d);
}

static void rack_cb(lv_event_t *e)
{
    (void)e;
    wxy_tt_tray(!s_tray_open);
}

static void prev_cb(lv_event_t *e) { (void)e; if (s_cb_prev) s_cb_prev(); }
static void play_cb(lv_event_t *e) { (void)e; if (s_cb_play) s_cb_play(); }
static void next_cb(lv_event_t *e) { (void)e; if (s_cb_next) s_cb_next(); }

/* ------------------------------------------------------------------ */
/* 创建                                                                */
/* ------------------------------------------------------------------ */

static lv_obj_t *circle(lv_obj_t *parent, int32_t x, int32_t y, int32_t d,
                        uint32_t bg, lv_opa_t opa)
{
    lv_obj_t *o = wxy_box_opa(parent, x, y, d, d, LV_RADIUS_CIRCLE, bg, opa);
    lv_obj_remove_flag(o, LV_OBJ_FLAG_CLICKABLE);
    return o;
}

void wxy_tt_create(lv_obj_t *parent)
{
    rt_memset(&s_tt, 0, sizeof(s_tt));
    s_screen = parent;

    s_tt.world = wxy_box_opa(parent, 0, 0, WXY_SCR_W, WXY_SCR_H, 0, 0x000000, LV_OPA_TRANSP);
    lv_obj_remove_flag(s_tt.world, LV_OBJ_FLAG_CLICKABLE);

    /* WXY: 光晕已移除——3 层 alpha 圆曾是全屏最贵的绘制元素（约 100 万像素/帧）。
     * 若需氛围光，改用预烘焙图片资源。 */

    /* 碟身 */
    s_tt.disc = wxy_box(s_tt.world, WXY_DISC_X, WXY_DISC_Y, WXY_DISC_SZ, WXY_DISC_SZ,
                        LV_RADIUS_CIRCLE, WXY_TOK()->color[WXY_TOK_COLOR_PANEL]);
    lv_obj_set_style_border_width(s_tt.disc, 1, 0);
    lv_obj_remove_flag(s_tt.disc, LV_OBJ_FLAG_CLICKABLE);

    /* 纹路 */
    static const int32_t ring_sz[RING_COUNT] = { 520, 470, 420, 370, 320, 270, 220 };
    for (int i = 0; i < RING_COUNT; i++)
    {
        int32_t off = (WXY_DISC_SZ - ring_sz[i]) / 2;
        s_tt.ring[i] = wxy_ring(s_tt.disc, off, off, ring_sz[i], 1, 0x000000);
    }

    /* 碟心 */
    s_tt.label = wxy_box(s_tt.disc, (WXY_DISC_SZ - 156) / 2, (WXY_DISC_SZ - 156) / 2,
                         156, 156, LV_RADIUS_CIRCLE, WXY_TOK()->color[WXY_TOK_COLOR_PANEL]);
    lv_obj_set_style_border_width(s_tt.label, 1, 0);
    lv_obj_remove_flag(s_tt.label, LV_OBJ_FLAG_CLICKABLE);

    s_tt.label_num = wxy_text(s_tt.label, 0, 62, "DISC", 0x1B1A16, WXY_F_TINY);
    lv_obj_set_width(s_tt.label_num, 156);
    lv_obj_set_style_text_align(s_tt.label_num, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_letter_space(s_tt.label_num, 1, 0);

    s_tt.label_name = wxy_text_w(s_tt.label, 0, 82, 156, "", 0x1B1A16, WXY_F_TITLE);
    lv_obj_set_style_text_align(s_tt.label_name, LV_TEXT_ALIGN_CENTER, 0);

    /* 读纹光点 */
    s_tt.read_dot = circle(s_tt.disc, 0, 0, 8, 0xFFFFFF, LV_OPA_TRANSP);

    /* 轨道点 */
    static const int16_t orbit_deg[TRACK_COUNT] = { -62, -31, 0, 31, 62 };
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        s_tt.orbit_map[i] = wxy_disc_nth((uint8_t)i);

        int32_t c = lv_trigo_cos((int16_t)(orbit_deg[i] * 10));
        int32_t s = lv_trigo_sin((int16_t)(orbit_deg[i] * 10));
        int32_t cx = WXY_DISC_CX + (WXY_ORBIT_R * c) / 32767;
        int32_t cy = WXY_DISC_CY + (WXY_ORBIT_R * s) / 32767;

        lv_obj_t *dot = circle(s_tt.world, cx - 15, cy - 15, 30,
                               WXY_TOK()->color[WXY_TOK_COLOR_PANEL], LV_OPA_TRANSP);
        lv_obj_set_style_border_width(dot, 1, 0);
        lv_obj_set_style_border_color(dot, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_LINE]), 0);
        wxy_tap(dot);
        lv_obj_add_event_cb(dot, orbit_cb, LV_EVENT_CLICKED, (void *)s_tt.orbit_map[i]);

        s_tt.orbit_dot[i] = dot;
        if (s_tt.orbit_map[i])
        {
            s_tt.orbit_icon[i] = wxy_icon_create(dot, s_tt.orbit_map[i]->slot, 16,
                                                 WXY_TOK()->color[WXY_TOK_COLOR_MUTED]);
            if (s_tt.orbit_icon[i])
                lv_obj_set_pos(s_tt.orbit_icon[i], 7, 7);
        }
    }

    /* 唱臂 */
    s_tt.pivot = circle(parent, WXY_ARM_PX - 21, WXY_ARM_PY - 21, 42,
                        WXY_TOK()->color[WXY_TOK_COLOR_PANEL2], LV_OPA_COVER);
    lv_obj_set_style_border_width(s_tt.pivot, 1, 0);
    lv_obj_set_style_border_color(s_tt.pivot, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_LINE]), 0);
    wxy_reg(s_tt.pivot, wxy_apply_panel2);

    lv_obj_t *pivot_core = circle(s_tt.pivot, 13, 13, 16,
                                  WXY_TOK()->color[WXY_TOK_COLOR_PANEL], LV_OPA_COVER);
    wxy_reg(pivot_core, wxy_apply_panel);

    s_tt.arm_line = lv_line_create(parent);
    lv_obj_remove_flag(s_tt.arm_line, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(s_tt.arm_line, WXY_SCR_W, WXY_SCR_H);
    lv_obj_set_pos(s_tt.arm_line, 0, 0);
    lv_obj_set_style_line_width(s_tt.arm_line, 6, 0);
    lv_obj_set_style_line_rounded(s_tt.arm_line, true, 0);
    lv_obj_set_style_line_color(s_tt.arm_line, wxy_col(0xC9CBD3), 0);

    s_tt.arm_head = wxy_box(parent, 0, 0, 34, 26, 5,
                            WXY_TOK()->color[WXY_TOK_COLOR_PANEL2]);
    lv_obj_set_style_border_width(s_tt.arm_head, 1, 0);
    lv_obj_set_style_border_color(s_tt.arm_head, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_LINE]), 0);
    lv_obj_remove_flag(s_tt.arm_head, LV_OBJ_FLAG_CLICKABLE);
    wxy_reg(s_tt.arm_head, wxy_apply_panel2);

    s_tt.arm_tip = circle(parent, 0, 0, 6, 0xFFFFFF, LV_OPA_COVER);
    wxy_reg(s_tt.arm_tip, wxy_apply_accent);

    /* 角落小 CD */
    s_tt.rack = wxy_box(s_tt.world, 18, 40, 214, 68, 15, WXY_TOK()->color[WXY_TOK_COLOR_PANEL]);
    lv_obj_set_style_border_width(s_tt.rack, 1, 0);
    wxy_tap(s_tt.rack);
    lv_obj_add_event_cb(s_tt.rack, rack_cb, LV_EVENT_CLICKED, NULL);

    s_tt.rack_cd = wxy_box(s_tt.rack, 6, 6, 56, 56, LV_RADIUS_CIRCLE, 0x000000);
    lv_obj_remove_flag(s_tt.rack_cd, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *cap = wxy_text(s_tt.rack, 74, 14, "LOADED DISC", WXY_TOK()->color[WXY_TOK_COLOR_MUTED], WXY_F_TINY);
    lv_obj_set_style_text_letter_space(cap, 1, 0);
    wxy_reg(cap, wxy_apply_muted);

    s_tt.rack_name = wxy_text_w(s_tt.rack, 74, 30, 128, "", WXY_TOK()->color[WXY_TOK_COLOR_TEXT], WXY_F_BODY);
    wxy_reg(s_tt.rack_name, wxy_apply_text);

    s_tt.rack_num = wxy_text(s_tt.rack, 74, 50, "DISC", WXY_TOK()->color[WXY_TOK_COLOR_MUTED], WXY_F_TINY);
    wxy_reg(s_tt.rack_num, wxy_apply_muted);

    /* 碟仓（先建壳，内容首次打开时再建，省对象） */
    s_tt.tray = wxy_box(parent, 18, 112, 340, 190, 14,
                        WXY_TOK()->color[WXY_TOK_COLOR_PANEL]);
    lv_obj_set_style_border_width(s_tt.tray, 1, 0);
    lv_obj_add_flag(s_tt.tray, LV_OBJ_FLAG_HIDDEN);

    /* 走带区：三段渐变 + 实体条 */
    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *scrim = wxy_box_opa(s_tt.world, 0, 462 + i * 24, WXY_SCR_W, 26, 0,
                                      WXY_TOK()->color[WXY_TOK_COLOR_BG],
                                      (lv_opa_t)(40 + i * 60));
        lv_obj_remove_flag(scrim, LV_OBJ_FLAG_CLICKABLE);
        wxy_reg(scrim, scrim_apply);
    }

    lv_obj_t *tr = wxy_box(s_tt.world, 0, WXY_SCR_H - WXY_TRANSPORT_H, WXY_SCR_W, WXY_TRANSPORT_H, 0,
                           WXY_TOK()->color[WXY_TOK_COLOR_BG]);
    wxy_reg(tr, wxy_apply_bg);

    s_tt.track_num = wxy_text(tr, 22, 12, "DISC 01", WXY_TOK()->color[WXY_TOK_COLOR_ACCENT], WXY_F_TITLE);
    s_tt.track_name = wxy_text_w(tr, 22, 38, 176, "", WXY_TOK()->color[WXY_TOK_COLOR_MUTED], WXY_F_TINY);

    for (int i = 0; i < TRACK_COUNT; i++)
    {
        lv_obj_t *seg = wxy_box(tr, 212 + i * 120, 32, 120, 2, 1,
                                WXY_TOK()->color[WXY_TOK_COLOR_LINE]);
        lv_obj_remove_flag(seg, LV_OBJ_FLAG_CLICKABLE);
        wxy_reg(seg, wxy_apply_line);

        s_tt.ruler_dot[i] = circle(tr, 212 + i * 120 + 55, 28, 9,
                                   WXY_TOK()->color[WXY_TOK_COLOR_LINE], LV_OPA_COVER);
    }

    /* 走带按钮 */
    lv_obj_t *prev = circle(tr, 851, 16, 34, WXY_TOK()->color[WXY_TOK_COLOR_PANEL2], LV_OPA_COVER);
    lv_obj_set_style_border_width(prev, 1, 0);
    lv_obj_set_style_border_color(prev, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_LINE]), 0);
    wxy_tap(prev);
    lv_obj_add_event_cb(prev, prev_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *pi = wxy_icon_create(prev, WXY_SLOT_PREV, 16, WXY_TOK()->color[WXY_TOK_COLOR_TEXT]);
    if (pi) lv_obj_center(pi);
    wxy_reg(prev, wxy_apply_panel2);

    lv_obj_t *play = circle(tr, 893, 12, 42, WXY_TOK()->color[WXY_TOK_COLOR_PANEL2], LV_OPA_COVER);
    lv_obj_set_style_border_width(play, 1, 0);
    lv_obj_set_style_border_color(play, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_ACCENT]), 0);
    wxy_tap(play);
    lv_obj_add_event_cb(play, play_cb, LV_EVENT_CLICKED, NULL);
    s_tt.play_icon = wxy_icon_create(play, WXY_SLOT_PAUSE, 20, WXY_TOK()->color[WXY_TOK_COLOR_ACCENT]);
    if (s_tt.play_icon) lv_obj_center(s_tt.play_icon);
    wxy_reg(play, wxy_apply_panel2);

    lv_obj_t *next = circle(tr, 943, 16, 34, WXY_TOK()->color[WXY_TOK_COLOR_PANEL2], LV_OPA_COVER);
    lv_obj_set_style_border_width(next, 1, 0);
    lv_obj_set_style_border_color(next, wxy_col(WXY_TOK()->color[WXY_TOK_COLOR_LINE]), 0);
    wxy_tap(next);
    lv_obj_add_event_cb(next, next_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *ni = wxy_icon_create(next, WXY_SLOT_NEXT, 16, WXY_TOK()->color[WXY_TOK_COLOR_TEXT]);
    if (ni) lv_obj_center(ni);
    wxy_reg(next, wxy_apply_panel2);

    /* 整台唱机的配色只登记一个回调：切主题时按当前碟的 hue 重算全部。
     * 必须放在所有控件建好之后，否则回调里会碰到空指针。 */
    wxy_reg(s_tt.disc, tt_apply_cb);

    /* 初始碟 */
    paint(&wxy_discs[0]);
    wxy_tt_set_playing(false);
}

/* ------------------------------------------------------------------ */
/* 对外动作                                                            */
/* ------------------------------------------------------------------ */

void wxy_tt_set_transport_cb(wxy_tt_cb_t prev, wxy_tt_cb_t play, wxy_tt_cb_t next)
{
    s_cb_prev = prev;
    s_cb_play = play;
    s_cb_next = next;
}

void wxy_tt_set_pick_cb(wxy_tt_pick_cb_t cb)
{
    s_cb_pick = cb;
}

void wxy_tt_paint(const wxy_disc_t *d)
{
    paint(d);
}

static void swap_exec(void *var, int32_t v)
{
    (void)var;

    if (v < 500)
    {
        /* 旧碟旋出：向右位移 + 淡出 */
        lv_obj_set_style_opa(s_tt.disc, (lv_opa_t)(255 - (255 * v) / 500), 0);
        lv_obj_set_x(s_tt.disc, WXY_DISC_X + (126 * v) / 500);
    }
    else
    {
        /* 换涂装只做一次，然后从左侧旋入 */
        if (!s_swap_painted)
        {
            s_swap_painted = true;
            paint(s_swap_target);
        }
        int32_t k = v - 500;
        lv_obj_set_style_opa(s_tt.disc, (lv_opa_t)((255 * k) / 500), 0);
        lv_obj_set_x(s_tt.disc, WXY_DISC_X + 78 - (78 * k) / 500);
    }
}

static void swap_ready(lv_anim_t *a)
{
    (void)a;
    s_swapping = false;

    lv_obj_set_style_opa(s_tt.disc, LV_OPA_COVER, 0);
    lv_obj_set_x(s_tt.disc, WXY_DISC_X);

    if (s_swap_target)
        arm_to((int32_t)s_swap_target->arm_deg * 10, WXY_MS(WXY_TOK_MOTION_ARM) / 2);

    LOG_I("disc -> %s", s_cur ? s_cur->id : "?");
}

void wxy_tt_swap(const wxy_disc_t *d, uint16_t dur, wxy_tt_cb_t done)
{
    if (d == NULL)
        return;

    s_swap_target = d;
    s_swap_painted = false;
    s_swapping = true;

    /* 抬针：往盘外退 12 度 */
    arm_to(s_arm_tenth - 120, dur * 40 / 100);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, &s_swap_painted);
    lv_anim_set_exec_cb(&a, swap_exec);
    lv_anim_set_values(&a, 0, 1000);
    lv_anim_set_duration(&a, dur);
    lv_anim_set_path_cb(&a, WXY_TOK()->path_swap);
    lv_anim_set_ready_cb(&a, swap_ready);
    lv_anim_start(&a);

    if (done)
    {
        /* 动画结束后再回调：用一次性定时器，避免在 ready 里递归触发导航 */
        s_swap_done = done;
        lv_timer_t *t = lv_timer_create(swap_done_timer, dur + 30, NULL);
        lv_timer_set_repeat_count(t, 1);
    }
}

void wxy_tt_arm(const wxy_disc_t *d, bool deep)
{
    if (d == NULL)
        return;
    int32_t deg = (int32_t)d->arm_deg + (deep ? 3 : 0);
    arm_to(deg * 10, WXY_MS(WXY_TOK_MOTION_ARM));
}

void wxy_tt_progress(uint8_t pct)
{
    if (pct > 100)
        pct = 100;
    s_progress = pct;

    if (s_cur)
    {
        int32_t deg = (int32_t)s_cur->arm_deg + (18 * pct) / 100;
        s_arm_tenth = deg * 10;
        arm_apply(s_arm_tenth);
    }
    read_dot_place(pct);
}

void wxy_tt_set_playing(bool on)
{
    s_playing = on;

    if (s_tt.read_dot)
        lv_obj_set_style_opa(s_tt.read_dot,
                             on ? (lv_opa_t)220 : LV_OPA_TRANSP, 0);

    if (s_tt.play_icon && lv_obj_check_type(s_tt.play_icon, &lv_label_class))
    {
        lv_label_set_text(s_tt.play_icon, on ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
        lv_obj_center(s_tt.play_icon);
    }
}

void wxy_tt_raise(void)
{
    /* 唱臂与碟仓必须盖在读碟窗之上（原型里 arm z=8 > content z=5），
     * 否则针尖落在内容区时会被面板挡住。 */
    lv_obj_t *items[5] = { s_tt.pivot, s_tt.arm_line, s_tt.arm_head, s_tt.arm_tip, s_tt.tray };
    (void)s_screen;
    for (int i = 0; i < 5; i++)
    {
        if (items[i])
            lv_obj_move_foreground(items[i]);
    }
}

void wxy_tt_label_name(const char *s)
{
    s_label_override = s;

    if (s_cur)
    {
        const char *name = s ? s : s_cur->name;
        lv_label_set_text(s_tt.label_name, name);
        lv_label_set_text(s_tt.rack_name, name);
    }
}
