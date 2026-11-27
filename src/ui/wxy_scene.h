/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * 场景注册表：读碟窗里的每一层内容都是一个"场景"。
 *
 * 场景在启动时全部建好，之后只切显隐与透明度，不创建不销毁 ——
 * 与 theme.h 的"换主题不重建控件"、disc 的"换碟不重建页面"是同一条约定：
 * 页面上任何一次点击都不应该触发 malloc。
 *
 * 新增一屏 = 在 wxy_pages.c 里加一个 build 函数 + 表里加一行。
 */

#ifndef WXY_SCENE_H
#define WXY_SCENE_H

#include <stdint.h>
#include <stdbool.h>

#include "lvgl.h"

typedef struct {
    const char *id;              /* 与碟 id 一致，导航按 id 找场景 */
    lv_obj_t   *root;            /* 根容器（452x364，绝对定位） */
    void      (*on_enter)(void); /* 可选：进入时的数据刷新 */
    void      (*on_leave)(void); /* 可选：离开时停掉本页定时器 */
    void      (*on_refresh)(void);/* 可选：切主题后重画图元类图标 */
} wxy_scene_t;

#define WXY_SCENE_MAX 12

extern wxy_scene_t wxy_scenes[];
extern uint8_t wxy_scene_count;

/* 在 well 里建全部场景（调用一次，LVGL 线程） */
void wxy_pages_create(lv_obj_t *well);

wxy_scene_t *wxy_scene_get(const char *id);

/* 切主题后由 UI 层调用：让场景重画那些不吃令牌、需要重新染色的图元图标 */
void wxy_pages_refresh(void);

/* 数据回填：UI 层算好状态，页面只负责显示 */
void wxy_pages_timer_show(uint32_t ms, bool running);
void wxy_pages_music_show(const char *title, const char *artist,
                          const char *cur, const char *dur, uint8_t pct);
void wxy_pages_music_state(bool playing);
void wxy_pages_pal_show(uint8_t act);

#endif /* WXY_SCENE_H */
