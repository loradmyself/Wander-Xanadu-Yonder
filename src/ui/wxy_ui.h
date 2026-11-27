/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 界面对外接口。
 *
 * 三个使用方走同一套入口：
 *   1) src/main.c —— 只调 wxy_ui_start()；
 *   2) 页面层 —— 点击后调 wxy_ui_disc / wxy_ui_app / ... 做导航；
 *   3) 将来的 AI Core —— 工具执行完直接调同名函数把界面推到对应碟，
 *      比如判定要开计时器就 wxy_ui_app("timer")，不需要知道界面结构。
 *
 * 约定：所有函数必须在 LVGL 线程调用。AI Core 从别的线程调时，
 * 自己用消息队列投递到 UI 线程（该入口在 AI Core 落地时补）。
 */

#ifndef WXY_UI_H
#define WXY_UI_H

#include <stdint.h>
#include <stdbool.h>

#include "theme.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 启动 UI 线程（LVGL 初始化 + 建壳）。返回 0 成功。 */
int  wxy_ui_start(void);

/* 界面是否已经建好（AI Core 正式接 LLM 前可用它做就绪判断） */
bool wxy_ui_ready(void);

/* ------------------------------------------------------------------ */
/* 导航                                                                */
/* ------------------------------------------------------------------ */

/* 换到某张碟：外圈轨道直接滑针，应用碟等价于 wxy_ui_app，内圈等价于深入 */
void wxy_ui_disc(const char *id);

/* 换入应用碟（music / timer / weather / note / pal），母轨虚化保留 */
void wxy_ui_app(const char *id);

/* 换回母碟（应用沉入，母碟旋回） */
void wxy_ui_back(void);

/* 向内深入一层（设置 -> 主题） */
void wxy_ui_descend(void);

/* 退回外圈 */
void wxy_ui_return(void);

/* ------------------------------------------------------------------ */
/* 泛层与读纹（不是页面切换）                                            */
/* ------------------------------------------------------------------ */

void wxy_ui_surface(bool show);   /* 详情浮出唱片表面 */
void wxy_ui_voice(bool show);     /* 按住说话的柔光泛层 */
void wxy_ui_ripple(void);         /* 呼吸读纹 + 流式占位 */
void wxy_ui_rack(void);           /* 打开/收起碟仓 */

/* ------------------------------------------------------------------ */
/* 主题与情绪                                                          */
/* ------------------------------------------------------------------ */

void wxy_ui_theme(const char *id);      /* 切主题（不重建任何控件） */
void wxy_ui_mood(wxy_mood_t mood);      /* 情绪推荐主题，用户手动选过则不覆盖 */

/* ------------------------------------------------------------------ */
/* 应用运行时（页面按钮与 AI Core 共用）                                 */
/* ------------------------------------------------------------------ */

void wxy_ui_music(bool play);
void wxy_ui_music_skip(int8_t dir);
bool wxy_ui_music_playing(void);

void wxy_ui_timer(bool run);
void wxy_ui_timer_reset(void);
bool wxy_ui_timer_running(void);

#ifdef __cplusplus
}
#endif

#endif /* WXY_UI_H */
