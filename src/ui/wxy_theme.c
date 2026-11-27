/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 主题引擎实现（契约见 theme.h）
 *
 * 三张"碟"：ink / pixel / cyber。令牌值直接取自 ui-logic-prototype.html v0.4
 * 的 --bg/--panel/--panel2/--text/--dim/--accent/--accent2/--line，动效取
 * "融合动效参数"面板的默认值。
 *
 * 切换流程：查表 -> 换指针 -> 遍历已登记控件重刷。不销毁任何对象，
 * 因而唱片、唱针、读碟窗的视觉状态在换主题时保持连续。
 */

#include <string.h>

#include <rtthread.h>

#include "theme.h"

#define LOG_TAG "WXY.THEME"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

/* 登记上限：整机 UI 约 250 个着色控件，留一倍余量。
 * 定长数组放 BSS，不动态分配，避免切换主题时堆碎片。 */
#define WXY_THEME_REG_MAX 512

typedef struct {
    lv_obj_t *obj;
    wxy_theme_apply_cb_t cb;
} theme_reg_t;

static theme_reg_t s_reg[WXY_THEME_REG_MAX];
static uint16_t s_reg_cnt;
static const wxy_theme_t *s_cur;

/* 控件被删除时自动摘除登记项。
 * 有些控件（播放/暂停图标、碟面图标）会在运行时重建，若不摘除，
 * 下次切主题就会写到已释放的内存上。 */
static void theme_obj_deleted(lv_event_t *e)
{
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);

    for (uint16_t i = 0; i < s_reg_cnt; i++)
    {
        if (s_reg[i].obj == obj)
        {
            s_reg[i] = s_reg[s_reg_cnt - 1];
            s_reg_cnt--;
            return;
        }
    }
}

/* ------------------------------------------------------------------ */
/* 注册表：三套令牌值                                                  */
/* ------------------------------------------------------------------ */

const wxy_theme_t wxy_themes[] = {
    {
        .id   = "ink",
        .name = "Calm / Ink",
        .mood = WXY_MOOD_CALM,
        .tokens = {
            .color = {
                0x141513,   /* BG */
                0x20211E,   /* PANEL */
                0x2A2B26,   /* PANEL2 */
                0xEFEDE3,   /* TEXT */
                0x9B9C8E,   /* MUTED */
                0xE0B877,   /* ACCENT */
                0x8FC7AA,   /* ACCENT2 */
                0x3A3B34,   /* LINE */
                0x1B1A16,   /* ON_ACCENT */
            },
            .radius = { 12, 8, 999 },
            .motion = { 560, 340, 520, 620, 1200, 9, 14 },
            .path_enter = lv_anim_path_ease_out,
            .path_exit  = lv_anim_path_ease_in,
            .path_swap  = lv_anim_path_ease_in_out,
            .font_body  = &lv_font_montserrat_16,
            .font_title = &lv_font_montserrat_20,
            .min_contrast_x100 = 780,
        },
        .font_pack = WXY_FONT_PACK_BASE,
    },
    {
        .id   = "pixel",
        .name = "Playful / Pixel",
        .mood = WXY_MOOD_PLAYFUL,
        .tokens = {
            .color = {
                0x13121B, 0x1F1D2E, 0x29263E, 0xF0EEF6, 0x9E98B8,
                0x9BD465, 0xEFAF34, 0x3C3856, 0x12130F,
            },
            .radius = { 10, 6, 999 },
            .motion = { 480, 300, 460, 560, 1200, 9, 14 },
            .path_enter = lv_anim_path_ease_out,
            .path_exit  = lv_anim_path_ease_in,
            .path_swap  = lv_anim_path_ease_in_out,
            .font_body  = &lv_font_montserrat_16,
            .font_title = &lv_font_montserrat_20,
            .min_contrast_x100 = 760,
        },
        .font_pack = WXY_FONT_PACK_PIXEL,
    },
    {
        .id   = "cyber",
        .name = "Fiery / Cyber",
        .mood = WXY_MOOD_FIERY,
        .tokens = {
            .color = {
                0x101114, 0x1C1D22, 0x26272E, 0xF3F3F6, 0x9798A1,
                0xF6C86A, 0xE2758F, 0x3A3B43, 0x17181A,
            },
            .radius = { 14, 8, 999 },
            .motion = { 600, 360, 560, 640, 1200, 9, 14 },
            .path_enter = lv_anim_path_ease_out,
            .path_exit  = lv_anim_path_ease_in,
            .path_swap  = lv_anim_path_ease_in_out,
            .font_body  = &lv_font_montserrat_16,
            .font_title = &lv_font_montserrat_20,
            .min_contrast_x100 = 760,
        },
        .font_pack = WXY_FONT_PACK_BOLD,
    },
};

const uint8_t WXY_THEME_COUNT = (uint8_t)(sizeof(wxy_themes) / sizeof(wxy_themes[0]));

/* ------------------------------------------------------------------ */
/* 内部工具                                                            */
/* ------------------------------------------------------------------ */

static const wxy_theme_t *theme_find(const char *id)
{
    if (id == NULL)
        return NULL;

    for (uint8_t i = 0; i < WXY_THEME_COUNT; i++)
    {
        if (rt_strcmp(wxy_themes[i].id, id) == 0)
            return &wxy_themes[i];
    }
    return NULL;
}

static void theme_reapply_all(void)
{
    if (s_cur == NULL)
        return;

    for (uint16_t i = 0; i < s_reg_cnt; i++)
    {
        if (s_reg[i].obj != NULL && s_reg[i].cb != NULL)
            s_reg[i].cb(s_reg[i].obj, &s_cur->tokens);
    }
}

/* ------------------------------------------------------------------ */
/* 对外 API                                                            */
/* ------------------------------------------------------------------ */

int wxy_theme_init(const char *initial_id)
{
    const wxy_theme_t *t = theme_find(initial_id);
    if (t == NULL)
        t = &wxy_themes[0];

    s_cur = t;
    s_reg_cnt = 0;

    LOG_I("theme base = %s", t->id);
    return 0;
}

const wxy_theme_t *wxy_theme_current(void)
{
    /* 主题未初始化时给一个安全兜底：页面可能在 init 之前被构建 */
    return s_cur ? s_cur : &wxy_themes[0];
}

int wxy_theme_set(const char *id)
{
    const wxy_theme_t *t = theme_find(id);
    if (t == NULL)
    {
        LOG_W("unknown theme: %s", id ? id : "(null)");
        return -1;
    }
    if (t == s_cur && s_reg_cnt > 0)
    {
        /* 重复选中：仍然重刷一次，避免外部改过样式后状态不一致 */
        theme_reapply_all();
        return 0;
    }

    s_cur = t;
    theme_reapply_all();
    LOG_I("theme -> %s", t->id);
    return 0;
}

const char *wxy_theme_suggest(wxy_mood_t mood)
{
    for (uint8_t i = 0; i < WXY_THEME_COUNT; i++)
    {
        if (wxy_themes[i].mood == mood)
            return wxy_themes[i].id;
    }
    return NULL;
}

int wxy_theme_register_obj(lv_obj_t *obj, wxy_theme_apply_cb_t apply_cb)
{
    if (obj == NULL)
        return -1;

    /* 先应用一次：控件创建后立刻拿到当前主题的颜色，不必等下一次切换 */
    if (apply_cb != NULL)
        apply_cb(obj, &wxy_theme_current()->tokens);

    if (s_reg_cnt >= WXY_THEME_REG_MAX)
    {
        LOG_W("theme registry full (%d)", WXY_THEME_REG_MAX);
        return -1;
    }

    s_reg[s_reg_cnt].obj = obj;
    s_reg[s_reg_cnt].cb = apply_cb;
    s_reg_cnt++;

    lv_obj_add_event_cb(obj, theme_obj_deleted, LV_EVENT_DELETE, NULL);
    return 0;
}

lv_color_t wxy_theme_color(wxy_tok_color_t tok)
{
    if ((int)tok < 0 || tok >= WXY_TOK_COLOR_COUNT)
        return lv_color_white();
    return lv_color_hex(wxy_theme_current()->tokens.color[tok]);
}
