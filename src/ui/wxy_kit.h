/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY UI 组件工具箱（内部头文件，页面层专用）
 *
 * 这一层只解决三个问题：
 *   1) 布局常量集中：所有坐标基于 1024x600，改分辨率只动这里；
 *   2) 控件工厂：LVGL 原生控件的默认样式（边框/阴影/内边距）全部清掉，
 *      否则每处都要写 6 行 reset，页面代码会被淹没；
 *   3) 主题回调：颜色永远从 theme.h 的令牌取，控件创建时登记，
 *      切主题时由主题引擎统一重刷，页面不自己存颜色。
 *
 * 页面层（wxy_pages.c）只用这里的 wxy_box / wxy_text / wxy_tap，
 * 不直接调 lv_obj_set_style_bg_color —— 这是"换主题不改页面代码"的边界。
 */

#ifndef WXY_KIT_H
#define WXY_KIT_H

#include <stdint.h>
#include <stdbool.h>

#include "lvgl.h"
#include "theme.h"

/* ------------------------------------------------------------------ */
/* 布局（对应 ui-logic-prototype.html v0.4）                            */
/* ------------------------------------------------------------------ */

#define WXY_SCR_W        1024
#define WXY_SCR_H        600
#define WXY_STATUS_H     28
#define WXY_TRANSPORT_H  66

/* 唱片：560x560，圆心落在读碟窗正中 */
#define WXY_DISC_X       114
#define WXY_DISC_Y       20
#define WXY_DISC_SZ      560
#define WXY_DISC_CX      (WXY_DISC_X + WXY_DISC_SZ / 2)
#define WXY_DISC_CY      (WXY_DISC_Y + WXY_DISC_SZ / 2)

/* 读碟窗（内容区）：原型 focus-well 168/118/452x364 */
#define WXY_WELL_X       168
#define WXY_WELL_Y       118
#define WXY_WELL_W       452
#define WXY_WELL_H       364

/* 唱臂支点：唱片右上方，臂长 470 */
#define WXY_ARM_PX       952
#define WXY_ARM_PY       96
#define WXY_ARM_LEN      470

/* 轨道点半径（落在盘面内圈边缘） */
#define WXY_ORBIT_R      252

/* 字号档：只用固件里编进去的四档，不引入新字库 */
#define WXY_F_TINY       (&lv_font_montserrat_14)
#define WXY_F_BODY       (&lv_font_montserrat_16)
#define WXY_F_TITLE      (&lv_font_montserrat_20)
#define WXY_F_DISP       (&lv_font_montserrat_28)

/* 令牌读取助手：页面写 WXY_C(WXY_TOK_COLOR_ACCENT)，不写色值 */
#define WXY_TOK()        (&wxy_theme_current()->tokens)
#define WXY_C(tok)       ((WXY_TOK())->color[(tok)])
#define WXY_R(tok)       ((WXY_TOK())->radius[(tok)])
#define WXY_MS(tok)      ((WXY_TOK())->motion[(tok)])

/* ------------------------------------------------------------------ */
/* 颜色                                                                */
/* ------------------------------------------------------------------ */

lv_color_t wxy_col(uint32_t rgb);

/* 线性混色：k=0 取 a，k=255 取 b。用于碟面纹路/标签这类
 * "同色系深浅"，避免为每种组合再定义令牌。 */
uint32_t   wxy_mix(uint32_t a, uint32_t b, uint8_t k);

/* ------------------------------------------------------------------ */
/* 控件工厂                                                            */
/* ------------------------------------------------------------------ */

/* 纯色圆角面板：无边框、无阴影、无滚动、无内边距 */
lv_obj_t *wxy_box(lv_obj_t *parent, int32_t x, int32_t y,
                  int32_t w, int32_t h, int32_t r, uint32_t bg);

/* 同上，带透明度（0..255），用于雾面/涟漪层 */
lv_obj_t *wxy_box_opa(lv_obj_t *parent, int32_t x, int32_t y,
                      int32_t w, int32_t h, int32_t r, uint32_t bg, lv_opa_t opa);

/* 空心圆环：唱片纹路、状态点 */
lv_obj_t *wxy_ring(lv_obj_t *parent, int32_t x, int32_t y,
                   int32_t d, int32_t bw, uint32_t line);

/* 文本。坐标即左上角，不参与布局计算 */
lv_obj_t *wxy_text(lv_obj_t *parent, int32_t x, int32_t y,
                   const char *s, uint32_t color, const lv_font_t *font);

/* 定宽文本，超出用省略号：卡片标题、碟名这类不能撑破布局的地方用 */
lv_obj_t *wxy_text_w(lv_obj_t *parent, int32_t x, int32_t y, int32_t w,
                     const char *s, uint32_t color, const lv_font_t *font);

/* 按下反馈：0.94 缩放，对应原型 .tap */
void wxy_tap(lv_obj_t *o);

/* ------------------------------------------------------------------ */
/* 动效                                                                */
/* ------------------------------------------------------------------ */

void wxy_fade(lv_obj_t *o, lv_opa_t from, lv_opa_t to, uint16_t ms,
              lv_anim_path_cb_t path, lv_anim_ready_cb_t ready);

void wxy_slide_y(lv_obj_t *o, int32_t from, int32_t to, uint16_t ms,
                 lv_anim_path_cb_t path);

/* 泛起 / 沉入：位移 + 透明度一起走，两条动画共用 ready 回调（挂在位移上） */
void wxy_fade_slide(lv_obj_t *o, int32_t y_from, int32_t y_to,
                    lv_opa_t from, lv_opa_t to, uint16_t ms,
                    lv_anim_path_cb_t path, lv_anim_ready_cb_t ready);

/* ------------------------------------------------------------------ */
/* 主题应用回调：作为 wxy_theme_register_obj 的回调传入                  */
/* ------------------------------------------------------------------ */

void wxy_apply_bg(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_panel(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_panel2(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_line(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_text(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_muted(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_accent(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_accent2(lv_obj_t *o, const wxy_tokens_t *t);
void wxy_apply_on_accent(lv_obj_t *o, const wxy_tokens_t *t);

/* 强调色雾面底：主色 18% 透明度，用于图标圆底 / 选中态 */
void wxy_apply_accent_tint(lv_obj_t *o, const wxy_tokens_t *t);

/* 强调色实体底：TTS 提示条这类需要整块实色的控件 */
void wxy_apply_accent_bg(lv_obj_t *o, const wxy_tokens_t *t);

/* 登记并立即应用一次，返回 0 成功。页面创建控件后紧跟一行即可。 */
int  wxy_reg(lv_obj_t *o, wxy_theme_apply_cb_t cb);

#endif /* WXY_KIT_H */
