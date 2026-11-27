/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY UI 组件工具箱实现。见 wxy_kit.h 的分层说明。
 */

#include "wxy_kit.h"

#define LOG_TAG "WXY.KIT"
#define LOG_LVL LOG_LVL_WARN
#include <ulog.h>

/* ---------------- 颜色 ---------------- */

lv_color_t wxy_col(uint32_t rgb)
{
    return lv_color_hex(rgb);
}

uint32_t wxy_mix(uint32_t a, uint32_t b, uint8_t k)
{
    uint32_t ar = (a >> 16) & 0xFFu, ag = (a >> 8) & 0xFFu, ab = a & 0xFFu;
    uint32_t br = (b >> 16) & 0xFFu, bg = (b >> 8) & 0xFFu, bb = b & 0xFFu;
    uint32_t ik = 255u - k;

    uint32_t r = (ar * ik + br * k) / 255u;
    uint32_t g = (ag * ik + bg * k) / 255u;
    uint32_t bl = (ab * ik + bb * k) / 255u;

    return (r << 16) | (g << 8) | bl;
}

/* ---------------- 控件 ---------------- */

/* LVGL 默认主题给 lv_obj 带了边框、阴影和 16px 内边距，
 * 我们的版式全部是绝对坐标，这些默认值必须一次清干净。 */
static void reset_obj(lv_obj_t *o, int32_t r, uint32_t bg, lv_opa_t opa)
{
    lv_obj_remove_flag(o, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(o, 0, 0);
    lv_obj_set_style_border_width(o, 0, 0);
    lv_obj_set_style_outline_width(o, 0, 0);
    lv_obj_set_style_shadow_width(o, 0, 0);
    lv_obj_set_style_radius(o, r, 0);
    lv_obj_set_style_bg_color(o, wxy_col(bg), 0);
    lv_obj_set_style_bg_opa(o, opa, 0);
}

lv_obj_t *wxy_box(lv_obj_t *parent, int32_t x, int32_t y,
                  int32_t w, int32_t h, int32_t r, uint32_t bg)
{
    lv_obj_t *o = lv_obj_create(parent);
    reset_obj(o, r, bg, LV_OPA_COVER);
    lv_obj_set_pos(o, x, y);
    lv_obj_set_size(o, w, h);
    return o;
}

lv_obj_t *wxy_box_opa(lv_obj_t *parent, int32_t x, int32_t y,
                      int32_t w, int32_t h, int32_t r, uint32_t bg, lv_opa_t opa)
{
    lv_obj_t *o = lv_obj_create(parent);
    reset_obj(o, r, bg, opa);
    lv_obj_set_pos(o, x, y);
    lv_obj_set_size(o, w, h);
    return o;
}

lv_obj_t *wxy_ring(lv_obj_t *parent, int32_t x, int32_t y,
                   int32_t d, int32_t bw, uint32_t line)
{
    lv_obj_t *o = lv_obj_create(parent);
    lv_obj_remove_flag(o, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(o, 0, 0);
    lv_obj_set_style_radius(o, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_TRANSP, 0);
    lv_obj_set_style_shadow_width(o, 0, 0);
    lv_obj_set_style_outline_width(o, 0, 0);
    lv_obj_set_style_border_width(o, bw, 0);
    lv_obj_set_style_border_color(o, wxy_col(line), 0);
    lv_obj_set_style_border_opa(o, LV_OPA_COVER, 0);
    lv_obj_set_pos(o, x, y);
    lv_obj_set_size(o, d, d);
    return o;
}

lv_obj_t *wxy_text(lv_obj_t *parent, int32_t x, int32_t y,
                   const char *s, uint32_t color, const lv_font_t *font)
{
    lv_obj_t *l = lv_label_create(parent);
    lv_label_set_text(l, s);
    lv_obj_set_pos(l, x, y);
    lv_obj_set_style_text_color(l, wxy_col(color), 0);
    lv_obj_set_style_text_font(l, font, 0);
    return l;
}

lv_obj_t *wxy_text_w(lv_obj_t *parent, int32_t x, int32_t y, int32_t w,
                     const char *s, uint32_t color, const lv_font_t *font)
{
    lv_obj_t *l = wxy_text(parent, x, y, s, color, font);
    lv_obj_set_width(l, w);
    lv_label_set_long_mode(l, LV_LABEL_LONG_MODE_DOTS);
    return l;
}

void wxy_tap(lv_obj_t *o)
{
    lv_obj_add_flag(o, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_transform_scale_x(o, 244, LV_STATE_PRESSED);
    lv_obj_set_style_transform_scale_y(o, 244, LV_STATE_PRESSED);
}

/* ---------------- 动效 ---------------- */

static void opa_exec(void *var, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t *)var, (lv_opa_t)v, 0);
}

void wxy_fade(lv_obj_t *o, lv_opa_t from, lv_opa_t to, uint16_t ms,
              lv_anim_path_cb_t path, lv_anim_ready_cb_t ready)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, o);
    lv_anim_set_exec_cb(&a, opa_exec);
    lv_anim_set_values(&a, from, to);
    lv_anim_set_duration(&a, ms);
    lv_anim_set_path_cb(&a, path ? path : lv_anim_path_ease_out);
    if (ready)
        lv_anim_set_ready_cb(&a, ready);
    lv_anim_start(&a);
}

void wxy_slide_y(lv_obj_t *o, int32_t from, int32_t to, uint16_t ms,
                 lv_anim_path_cb_t path)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, o);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, from, to);
    lv_anim_set_duration(&a, ms);
    lv_anim_set_path_cb(&a, path ? path : lv_anim_path_ease_out);
    lv_anim_start(&a);
}

void wxy_fade_slide(lv_obj_t *o, int32_t y_from, int32_t y_to,
                    lv_opa_t from, lv_opa_t to, uint16_t ms,
                    lv_anim_path_cb_t path, lv_anim_ready_cb_t ready)
{
    wxy_fade(o, from, to, ms, path, NULL);

    if (y_from == y_to)
    {
        /* 纯淡入淡出：ready 得单独挂一次 */
        if (ready)
        {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, o);
            lv_anim_set_exec_cb(&a, opa_exec);
            lv_anim_set_values(&a, to, to);
            lv_anim_set_duration(&a, ms);
            lv_anim_set_ready_cb(&a, ready);
            lv_anim_start(&a);
        }
        return;
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, o);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, y_from, y_to);
    lv_anim_set_duration(&a, ms);
    lv_anim_set_path_cb(&a, path ? path : lv_anim_path_ease_out);
    if (ready)
        lv_anim_set_ready_cb(&a, ready);
    lv_anim_start(&a);
}

/* ---------------- 主题回调 ---------------- */

void wxy_apply_bg(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_BG]), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
}

void wxy_apply_panel(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_PANEL]), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
}

void wxy_apply_panel2(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_PANEL2]), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
}

void wxy_apply_line(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_LINE]), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
}

void wxy_apply_text(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_text_color(o, wxy_col(t->color[WXY_TOK_COLOR_TEXT]), 0);
}

void wxy_apply_muted(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_text_color(o, wxy_col(t->color[WXY_TOK_COLOR_MUTED]), 0);
}

void wxy_apply_accent(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_text_color(o, wxy_col(t->color[WXY_TOK_COLOR_ACCENT]), 0);
}

void wxy_apply_accent2(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_text_color(o, wxy_col(t->color[WXY_TOK_COLOR_ACCENT2]), 0);
}

void wxy_apply_on_accent(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_text_color(o, wxy_col(t->color[WXY_TOK_COLOR_ON_ACCENT]), 0);
}

void wxy_apply_accent_tint(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_ACCENT]), 0);
    lv_obj_set_style_bg_opa(o, 46, 0);
}

void wxy_apply_accent_bg(lv_obj_t *o, const wxy_tokens_t *t)
{
    lv_obj_set_style_bg_color(o, wxy_col(t->color[WXY_TOK_COLOR_ACCENT]), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
}

int wxy_reg(lv_obj_t *o, wxy_theme_apply_cb_t cb)
{
    if (o == NULL)
        return -1;
    return wxy_theme_register_obj(o, cb);
}
