/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 图标渲染器（槽位契约见 slots.h）
 *
 * 契约说"图标是槽位，不是文件路径"，这一层就是槽位的当前实现：
 *   - 有对应的 FontAwesome 字形的槽位 -> 用符号字形（LV_SYMBOL_*），
 *     颜色走 text_color，天然支持按主题染色；
 *   - 没有合适字形的槽位（心形、三张脸、钟、太阳）-> 用 LVGL 基本图元拼，
 *     同样单色，同样可染色。
 *
 * 以后真图片资源（wxy_ic_<slot>_<size>.png）到位时，只替换本文件里的
 * wxy_icon_create()，页面代码一行都不用改 —— 这就是槽位契约的价值。
 */

#include <stdio.h>

#include "wxy_icon.h"
#include "wxy_icon_ezip.h"

/* ------------------------------------------------------------------ */
/* 槽位元数据表                                                        */
/* ------------------------------------------------------------------ */

const wxy_slot_meta_t wxy_slot_meta[] = {
    { WXY_SLOT_MIC,        "mic",        WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_SEND,       "send",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_BACK,       "back",       WXY_SLOT_SIZE_24, false },

    { WXY_SLOT_CHAT,       "chat",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_HOME,       "home",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_TOOLS,      "tools",      WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_SET,        "set",        WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_EYE,        "eye",        WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_DISC,       "disc",       WXY_SLOT_SIZE_32, false },
    { WXY_SLOT_EJECT,      "eject",      WXY_SLOT_SIZE_24, false },

    { WXY_SLOT_BATTERY,    "battery",    WXY_SLOT_SIZE_16, false },
    { WXY_SLOT_WIFI,       "wifi",       WXY_SLOT_SIZE_16, false },
    { WXY_SLOT_SIGNAL,     "signal",     WXY_SLOT_SIZE_16, false },

    { WXY_SLOT_CLOCK,      "clock",      WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_WEATHER,    "weather",    WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_MUSIC,      "music",      WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_NOTE,       "note",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_HEART,      "heart",      WXY_SLOT_SIZE_24, false },

    { WXY_SLOT_PLAY,       "play",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_PAUSE,      "pause",      WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_PREV,       "prev",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_NEXT,       "next",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_STOP,       "stop",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_REFRESH,    "refresh",    WXY_SLOT_SIZE_24, false },

    { WXY_SLOT_CHECK,      "check",      WXY_SLOT_SIZE_16, false },
    { WXY_SLOT_PLUS,       "plus",       WXY_SLOT_SIZE_24, false },
    { WXY_SLOT_CLOUD,      "cloud",      WXY_SLOT_SIZE_24, false },

    { WXY_SLOT_FACE_CALM,  "face_calm",  WXY_SLOT_SIZE_32, true },
    { WXY_SLOT_FACE_HAPPY, "face_happy", WXY_SLOT_SIZE_32, true },
    { WXY_SLOT_FACE_FIRE,  "face_fire",  WXY_SLOT_SIZE_32, true },
};

const uint8_t WXY_SLOT_META_COUNT =
    (uint8_t)(sizeof(wxy_slot_meta) / sizeof(wxy_slot_meta[0]));

const wxy_slot_meta_t *wxy_slot_info(wxy_slot_t slot)
{
    for (uint8_t i = 0; i < WXY_SLOT_META_COUNT; i++)
    {
        if (wxy_slot_meta[i].slot == slot)
            return &wxy_slot_meta[i];
    }
    return NULL;
}

uint16_t wxy_slot_filename(wxy_slot_t slot, wxy_slot_size_t size,
                           char *buf, uint16_t buf_len)
{
    const wxy_slot_meta_t *m = wxy_slot_info(slot);
    if (m == NULL || buf == NULL || buf_len == 0)
        return 0;

    int n = snprintf(buf, buf_len, "wxy_ic_%s_%d.png", m->name, (int)size);
    if (n <= 0 || n >= (int)buf_len)
        return 0;
    return (uint16_t)n;
}

bool wxy_slot_valid(wxy_slot_t slot)
{
    return slot >= 0 && slot < WXY_SLOT_COUNT;
}

/* ------------------------------------------------------------------ */
/* 字形映射：能用 FontAwesome 的就不画图元                              */
/* ------------------------------------------------------------------ */

static const char *slot_symbol(wxy_slot_t slot)
{
    switch (slot)
    {
    case WXY_SLOT_MIC:     return LV_SYMBOL_AUDIO;
    case WXY_SLOT_SEND:    return LV_SYMBOL_UPLOAD;
    case WXY_SLOT_BACK:    return LV_SYMBOL_LEFT;

    case WXY_SLOT_CHAT:    return LV_SYMBOL_ENVELOPE;
    case WXY_SLOT_HOME:    return LV_SYMBOL_HOME;
    case WXY_SLOT_TOOLS:   return LV_SYMBOL_LIST;
    case WXY_SLOT_SET:     return LV_SYMBOL_SETTINGS;
    case WXY_SLOT_EYE:     return LV_SYMBOL_EYE_OPEN;
    case WXY_SLOT_DISC:    return LV_SYMBOL_DRIVE;
    case WXY_SLOT_EJECT:   return LV_SYMBOL_EJECT;

    case WXY_SLOT_BATTERY: return LV_SYMBOL_BATTERY_FULL;
    case WXY_SLOT_WIFI:    return LV_SYMBOL_WIFI;
    case WXY_SLOT_SIGNAL:  return LV_SYMBOL_BARS;

    case WXY_SLOT_MUSIC:   return LV_SYMBOL_VOLUME_MAX;
    case WXY_SLOT_NOTE:    return LV_SYMBOL_FILE;

    case WXY_SLOT_PLAY:    return LV_SYMBOL_PLAY;
    case WXY_SLOT_PAUSE:   return LV_SYMBOL_PAUSE;
    case WXY_SLOT_PREV:    return LV_SYMBOL_PREV;
    case WXY_SLOT_NEXT:    return LV_SYMBOL_NEXT;
    case WXY_SLOT_STOP:    return LV_SYMBOL_STOP;
    case WXY_SLOT_REFRESH: return LV_SYMBOL_REFRESH;

    case WXY_SLOT_CHECK:   return LV_SYMBOL_OK;
    case WXY_SLOT_PLUS:    return LV_SYMBOL_PLUS;
    case WXY_SLOT_CLOUD:   return LV_SYMBOL_DOWNLOAD;

    /* 心形 / 脸 / 钟 / 太阳没有合适字形，走图元绘制 */
    default:               return NULL;
    }
}

static const lv_font_t *font_for(int32_t px)
{
    if (px >= 28)
        return &lv_font_montserrat_28;
    if (px >= 20)
        return &lv_font_montserrat_20;
    return &lv_font_montserrat_16;
}

/* ------------------------------------------------------------------ */
/* 图元绘制                                                            */
/* ------------------------------------------------------------------ */

static lv_obj_t *icon_base(lv_obj_t *parent, int32_t px)
{
    lv_obj_t *o = lv_obj_create(parent);
    lv_obj_remove_flag(o, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(o, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(o, 0, 0);
    lv_obj_set_style_border_width(o, 0, 0);
    lv_obj_set_style_outline_width(o, 0, 0);
    lv_obj_set_style_shadow_width(o, 0, 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_TRANSP, 0);
    lv_obj_set_size(o, px, px);
    return o;
}

static lv_obj_t *dot(lv_obj_t *parent, int32_t cx, int32_t cy, int32_t d, uint32_t c)
{
    lv_obj_t *o = lv_obj_create(parent);
    lv_obj_remove_flag(o, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(o, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(o, 0, 0);
    lv_obj_set_style_border_width(o, 0, 0);
    lv_obj_set_style_shadow_width(o, 0, 0);
    lv_obj_set_style_radius(o, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(o, lv_color_hex(c), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
    lv_obj_set_pos(o, cx - d / 2, cy - d / 2);
    lv_obj_set_size(o, d, d);
    return o;
}

static lv_obj_t *seg(lv_obj_t *parent, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                     int32_t w, uint32_t c)
{
    lv_obj_t *l = lv_line_create(parent);
    lv_obj_remove_flag(l, LV_OBJ_FLAG_CLICKABLE);
    lv_point_precise_t pts[2] = { { x1, y1 }, { x2, y2 } };
    lv_line_set_points(l, pts, 2);
    lv_obj_set_style_line_width(l, w, 0);
    lv_obj_set_style_line_color(l, lv_color_hex(c), 0);
    lv_obj_set_style_line_rounded(l, true, 0);
    lv_obj_set_pos(l, 0, 0);
    lv_obj_set_size(l, 64, 64);
    return l;
}

static void draw_heart(lv_obj_t *box, int32_t px, uint32_t c)
{
    int32_t lobe = px * 44 / 100;

    dot(box, px * 30 / 100, px * 34 / 100, lobe, c);
    dot(box, px * 70 / 100, px * 34 / 100, lobe, c);
    seg(box, px * 50 / 100, px * 90 / 100, px * 12 / 100, px * 44 / 100, px / 5, c);
    seg(box, px * 50 / 100, px * 90 / 100, px * 88 / 100, px * 44 / 100, px / 5, c);
}

static void draw_clock(lv_obj_t *box, int32_t px, uint32_t c)
{
    lv_obj_t *ring = lv_obj_create(box);
    lv_obj_remove_flag(ring, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(ring, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(ring, 0, 0);
    lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(ring, LV_OPA_TRANSP, 0);
    lv_obj_set_style_shadow_width(ring, 0, 0);
    lv_obj_set_style_border_width(ring, px / 12 + 1, 0);
    lv_obj_set_style_border_color(ring, lv_color_hex(c), 0);
    lv_obj_set_size(ring, px, px);
    lv_obj_set_pos(ring, 0, 0);

    seg(box, px / 2, px / 2, px / 2, px * 26 / 100, px / 12 + 1, c);   /* 分针 */
    seg(box, px / 2, px / 2, px * 72 / 100, px / 2, px / 12 + 1, c);   /* 时针 */
}

static void draw_sun(lv_obj_t *box, int32_t px, uint32_t c)
{
    int32_t d = px * 46 / 100;
    dot(box, px / 2, px / 2, d, c);

    int32_t a = px * 16 / 100, b = px * 34 / 100;
    seg(box, px / 2, a, px / 2, b, px / 12 + 1, c);
    seg(box, px / 2, px - a, px / 2, px - b, px / 12 + 1, c);
    seg(box, a, px / 2, b, px / 2, px / 12 + 1, c);
    seg(box, px - a, px / 2, px - b, px / 2, px / 12 + 1, c);
}

static void draw_face(lv_obj_t *box, int32_t px, uint32_t c, wxy_slot_t slot)
{
    int32_t eye = px / 8;
    int32_t ey = px * 40 / 100;

    if (slot == WXY_SLOT_FACE_FIRE)
    {
        /* 燃：斜眼 + 大笑 */
        seg(box, px * 26 / 100, px * 30 / 100, px * 42 / 100, px * 40 / 100, px / 12, c);
        seg(box, px * 74 / 100, px * 30 / 100, px * 58 / 100, px * 40 / 100, px / 12, c);
    }
    else
    {
        dot(box, px * 34 / 100, ey, eye, c);
        dot(box, px * 66 / 100, ey, eye, c);
    }

    int32_t mouth = px * 52 / 100;
    lv_obj_t *arc = lv_arc_create(box);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(arc, mouth, mouth);
    lv_obj_set_pos(arc, (px - mouth) / 2, px * 40 / 100);
    lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(arc, 0, 0);
    lv_obj_set_style_arc_width(arc, px / 14 + 1, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex(c), LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_set_style_opa(arc, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_arc_set_rotation(arc, 0);

    if (slot == WXY_SLOT_FACE_CALM)
        lv_arc_set_bg_angles(arc, 20, 160);      /* 平嘴 */
    else
        lv_arc_set_bg_angles(arc, 30, 150);      /* 笑 */

    lv_arc_set_angles(arc, 0, 0);
}

/* ------------------------------------------------------------------ */
/* 对外：创建 / 染色                                                    */
/* ------------------------------------------------------------------ */

lv_obj_t *wxy_icon_create(lv_obj_t *parent, wxy_slot_t slot, int32_t px, uint32_t color)
{
    if (!wxy_slot_valid(slot) || px <= 0)
        return NULL;

    /* ezip 图片资源优先（EPIC 硬件解压 blit，固定色，不支持运行时染色）。
     * 命中条件：该槽位在 px 档有资源（16/24/32）；20/96 等档位与未覆盖槽位
     * 回退到下面的字形/图元实现。color 参数对图片版忽略——素材自带设计色。 */
    const lv_image_dsc_t *img = wxy_icon_ezip_get(slot, px);
    if (img != NULL)
    {
        lv_obj_t *im = lv_image_create(parent);
        lv_image_set_src(im, img);
        return im;
    }

    const char *sym = slot_symbol(slot);
    if (sym != NULL)
    {
        lv_obj_t *l = lv_label_create(parent);
        lv_label_set_text(l, sym);
        lv_obj_set_style_text_font(l, font_for(px), 0);
        lv_obj_set_style_text_color(l, lv_color_hex(color), 0);
        lv_obj_set_size(l, px, px);
        lv_obj_set_style_text_align(l, LV_TEXT_ALIGN_CENTER, 0);
        return l;
    }

    lv_obj_t *box = icon_base(parent, px);

    switch (slot)
    {
    case WXY_SLOT_HEART:      draw_heart(box, px, color); break;
    case WXY_SLOT_CLOCK:      draw_clock(box, px, color); break;
    case WXY_SLOT_WEATHER:    draw_sun(box, px, color);   break;
    case WXY_SLOT_FACE_CALM:
    case WXY_SLOT_FACE_HAPPY:
    case WXY_SLOT_FACE_FIRE:  draw_face(box, px, color, slot); break;
    default:                  break;
    }

    return box;
}

void wxy_icon_recolor(lv_obj_t *icon, uint32_t color)
{
    if (icon == NULL)
        return;

    lv_color_t c = lv_color_hex(color);

    /* ezip 图片图标：固定色资源，v9 widget 无 recolor 接口，染色请求忽略 */
    if (lv_obj_check_type(icon, &lv_image_class))
        return;

    /* 字体图标：本身就是一个 label */
    if (lv_obj_check_type(icon, &lv_label_class))
    {
        lv_obj_set_style_text_color(icon, c, 0);
        return;
    }

    lv_obj_set_style_bg_color(icon, c, 0);
    lv_obj_set_style_border_color(icon, c, 0);

    uint32_t n = lv_obj_get_child_count(icon);
    for (uint32_t i = 0; i < n; i++)
    {
        lv_obj_t *o = lv_obj_get_child(icon, i);
        if (lv_obj_check_type(o, &lv_label_class))
            lv_obj_set_style_text_color(o, c, 0);

        lv_obj_set_style_bg_color(o, c, 0);
        lv_obj_set_style_border_color(o, c, 0);
        lv_obj_set_style_line_color(o, c, 0);
        lv_obj_set_style_arc_color(o, c, LV_PART_MAIN);
        lv_obj_set_style_arc_color(o, c, LV_PART_INDICATOR);
    }
}
