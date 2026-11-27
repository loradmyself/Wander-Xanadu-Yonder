/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 主题契约（F-DISP-10 主题 / 深色模式，P1）
 *
 * 设计约定：主题是"一份可注册的常量数据"，不是代码分支，也不是 if-else。
 * 页面代码只引用语义令牌（WXY_TOK_*）与图标槽位（wxy_slot_t），
 * 永远不写死色值、时长、图片路径——否则换主题等于改 N 处代码，契约即失效。
 *
 * 切换模型（对应 ui-logic-prototype.html v0.4）：
 *   选主题 ID -> 查注册表 -> 换当前指针 -> 对已有控件重刷样式 -> 完成
 *   全程不销毁、不重建页面对象，与 wxy_ui.c 里"页面常驻、靠 HIDDEN 切换"一致。
 *
 * 资源约束：
 *   - 本头文件只定义"引用"，不携带字体/图片实体；三套主题共用同一套中文字库基线
 *     （F-DISP-03 按需加载），毛笔/点阵/粗黑等差异字体走可选资源包，按 font_pack_id 引用；
 *   - 令牌表与主题表全部 const 放 Flash，切换不占 RAM；
 *   - 深色优先：基准主题 ink 的对比度按正文 >= 4.5:1 取（夜间陪伴场景，F-DISP-08）。
 *
 * 依赖：LVGL 9（lv_color_t / lv_font_t / lv_anim_path_t 由 littlevgl2rtt 提供）。
 * 槽位枚举见 slots.h；本头文件不 include 它，避免循环依赖，由实现文件同时包含两者。
 */

#ifndef WXY_THEME_H
#define WXY_THEME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "lvgl.h"

/* ------------------------------------------------------------------ */
/* 一、语义令牌 ID：页面代码引用这些名字，不引用具体值                    */
/* ------------------------------------------------------------------ */

/* 颜色令牌。命名规则：WXY_TOK_COLOR_<语义>，不用具体色名（红/蓝），
 * 这样换主题时语义不变、值变。页签/选中态用 ACCENT，次要信息用 MUTED。 */
typedef enum {
    WXY_TOK_COLOR_BG = 0,       /* 全局背景 */
    WXY_TOK_COLOR_PANEL,        /* 卡片 / 面板底 */
    WXY_TOK_COLOR_PANEL2,       /* 次级面（输入框、chip） */
    WXY_TOK_COLOR_TEXT,         /* 正文 */
    WXY_TOK_COLOR_MUTED,        /* 次要文字 / 占位 */
    WXY_TOK_COLOR_ACCENT,       /* 主强调（当前碟 / 选中） */
    WXY_TOK_COLOR_ACCENT2,      /* 副强调（情绪 / 链接） */
    WXY_TOK_COLOR_LINE,         /* 分割线 / 边框 */
    WXY_TOK_COLOR_ON_ACCENT,    /* 强调色底上的前景（保证对比度） */
    WXY_TOK_COLOR_COUNT
} wxy_tok_color_t;

/* 圆角令牌，单位 px，对应 1024x600 横屏。 */
typedef enum {
    WXY_TOK_RADIUS_CARD = 0,    /* 卡片，原型 12 */
    WXY_TOK_RADIUS_CHIP,        /* 小 chip / 按钮，原型 8 */
    WXY_TOK_RADIUS_PILL,        /* 胶囊 / 输入框，原型 999 -> 取高度一半 */
    WXY_TOK_RADIUS_COUNT
} wxy_tok_radius_t;

/* 动效令牌 ID。数值来自原型"融合动效参数"面板，单位为 ms / px / s。
 * 页面只引用 ID，曲线由主题提供函数指针，避免各处写贝塞尔字符串。 */
typedef enum {
    WXY_TOK_MOTION_ENTER = 0,   /* 泛起 / 新碟落位，原型 560 */
    WXY_TOK_MOTION_EXIT,        /* 沉入 / 旧碟退出，原型 340（退场快于进场） */
    WXY_TOK_MOTION_SWAP,        /* 换碟总时长，原型 520 */
    WXY_TOK_MOTION_ARM,         /* 唱针行程，原型 620 */
    WXY_TOK_MOTION_GHOST,       /* 读纹骨架，原型 1200 */
    WXY_TOK_MOTION_BLUR,        /* 泛起模糊 px，原型 9 */
    WXY_TOK_MOTION_SPIN,        /* 唱片转一圈 s，原型 14 */
    WXY_TOK_MOTION_COUNT
} wxy_tok_motion_t;

/* ------------------------------------------------------------------ */
/* 二、令牌值表：一组语义字段的实际值                                    */
/* ------------------------------------------------------------------ */

typedef struct {
    /* 颜色。存 uint32_t RGB 十六进制（0xRRGGBB），由 apply 时转 lv_color_t，
     * 与 wxy_ui.c 现有 col() helper 的口径一致。 */
    uint32_t color[WXY_TOK_COLOR_COUNT];
    int32_t  radius[WXY_TOK_RADIUS_COUNT];

    /* 动效。时长 ms / 模糊 px / 转速 s，字段顺序对应 wxy_tok_motion_t。 */
    uint16_t motion[WXY_TOK_MOTION_COUNT];

    /* 曲线：进场 ease-out、退场 ease-in、换碟缓冲。NULL 表示用 LVGL 默认。
     * LVGL 9 的动画曲线是回调指针 lv_anim_path_cb_t（如 lv_anim_path_ease_out），
     * 不是结构体；这里直接存函数指针。 */
    lv_anim_path_cb_t path_enter;
    lv_anim_path_cb_t path_exit;
    lv_anim_path_cb_t path_swap;

    /* 字体：正文与标题。中文字库基线随固件；差异字体走可选资源包 ID，
     * 由资源层按 id 加载后填入（见"四、可选资源包"）。 */
    const lv_font_t *font_body;
    const lv_font_t *font_title;

    /* 对比度守门：正文色对底色实测的最小对比度（目标 >= 4.5）。
     * 由构建期/启动期自检填充，运行期只读，供夜间模式校验。 */
    uint16_t min_contrast_x100;
} wxy_tokens_t;

/* ------------------------------------------------------------------ */
/* 三、情绪：与主题正交，不直接改令牌值                                  */
/* ------------------------------------------------------------------ */

/* F-AI-15 情绪状态机。情绪决定"倾向哪个主题"，不是直接改令牌值；
 * 用户手动选主题时以手动为准，不被情绪覆盖回去。两者是正交的轴。 */
typedef enum {
    WXY_MOOD_CALM = 0,          /* 静 -> 倾向 ink */
    WXY_MOOD_PLAYFUL,           /* 趣 -> 倾向 pixel */
    WXY_MOOD_FIERY,             /* 燃 -> 倾向 cyber */
    WXY_MOOD_COUNT
} wxy_mood_t;

/* ------------------------------------------------------------------ */
/* 四、可选资源包：字体 / 表情动画不进主题结构本体                        */
/* ------------------------------------------------------------------ */

/* 差异字体包 ID（F-DISP-03 按需加载，放 TF 卡 / .xzp）。
 * 主题只引用 ID，不携带字体实体，避免 16MB Flash 被三套字库吃爆。 */
typedef enum {
    WXY_FONT_PACK_BASE = 0,     /* 基础中文字库，随固件 */
    WXY_FONT_PACK_BRUSH,        /* 水墨毛笔（可选包） */
    WXY_FONT_PACK_PIXEL,        /* 点阵（可选包） */
    WXY_FONT_PACK_BOLD,         /* 赛博粗黑（可选包） */
    WXY_FONT_PACK_COUNT
} wxy_font_pack_t;

/* 表情动画 ID 段（F-DISP-05 序列帧 / Lottie / RLE），每个情绪一组。
 * 具体资源由 wxy_face_anim_t 管理，本契约只约定按情绪分组的命名口径。 */
#define WXY_FACE_ANIM_BASE(mood) ((mood) * 16)

/* ------------------------------------------------------------------ */
/* 五、主题对象：注册到表里的常量数据                                    */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *id;             /* "ink" / "pixel" / "cyber"，与 ui-logic 原型一致 */
    const char *name;           /* 展示名，如 "静 · 墨盘" */
    wxy_mood_t  mood;           /* 该主题归属的情绪轴 */
    wxy_tokens_t tokens;        /* 令牌值表（const 数据） */
    wxy_font_pack_t font_pack;  /* 差异字体包 ID；BASE 表示用固件基线 */
} wxy_theme_t;

/* 主题注册表：三张"碟"。数组元素全部 const，放 Flash。 */
extern const wxy_theme_t wxy_themes[];
extern const uint8_t WXY_THEME_COUNT;

/* 注册表下标 -> ID 的便捷宏，页面/设置页做循环列举时用。 */
#define WXY_THEME_BY_IDX(i) (&wxy_themes[(i)])
#define WXY_THEME_FOREACH(i) for (uint8_t (i) = 0; (i) < WXY_THEME_COUNT; (i)++)

/* ------------------------------------------------------------------ */
/* 六、切换 API                                                        */
/* ------------------------------------------------------------------ */

/* 初始化：选定初始主题并把令牌应用到当前控件树。
 * 必须在 LVGL 线程调用（LVGL 非线程安全）。返回 0 成功。 */
int  wxy_theme_init(const char *initial_id);

/* 当前主题：返回只读指针，页面取令牌时用 wxy_theme_current()->tokens.color[...]。 */
const wxy_theme_t *wxy_theme_current(void);

/* 按 ID 切换主题。流程：查注册表 -> 换指针 -> 对已注册控件重刷样式。
 * 不销毁页面对象。返回 0 成功，-1 表示 ID 不存在。
 * 必须在 LVGL 线程调用。 */
int  wxy_theme_set(const char *id);

/* 情绪推荐：返回该情绪倾向的主题 ID（不自动切换，由调用方决策，
 * 以免覆盖用户手动选择）。返回 NULL 表示无推荐。 */
const char *wxy_theme_suggest(wxy_mood_t mood);

/* 控件注册：把需要随主题重刷的控件交给主题引擎登记。
 * 页面创建时调用一次，切换时引擎遍历重刷；页面不销毁则无需注销。
 * user_cb 可为 NULL，表示用控件的默认样式刷新策略。 */
typedef void (*wxy_theme_apply_cb_t)(lv_obj_t *obj, const wxy_tokens_t *tokens);
int  wxy_theme_register_obj(lv_obj_t *obj, wxy_theme_apply_cb_t apply_cb);

/* 令牌读取助手：把 uint32_t 十六进制转成 lv_color_t。 */
lv_color_t wxy_theme_color(wxy_tok_color_t tok);

#ifdef __cplusplus
}
#endif

#endif /* WXY_THEME_H */
