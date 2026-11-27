/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * 图标渲染器对外接口。
 *
 * 与 slots.h 分开：slots.h 是"槽位契约"（资源层/页面层/构建自检共用，
 * 只依赖 stdint），这里多带了 LVGL 依赖，属于"当前实现"，将来换真图片
 * 资源时只改实现，不动契约。
 */

#ifndef WXY_ICON_H
#define WXY_ICON_H

#include <stdint.h>
#include <stdbool.h>

#include "lvgl.h"
#include "slots.h"

/* 在 parent 里创建一个 px 见方的单色图标，返回控件（可能是 label，
 * 也可能是由基本图元拼出的容器）。px 只取 16/24/32 三档效果最好。 */
lv_obj_t *wxy_icon_create(lv_obj_t *parent, wxy_slot_t slot, int32_t px, uint32_t color);

/* 重新染色。字体图标一行搞定；图元图标会遍历一层子控件改色。 */
void wxy_icon_recolor(lv_obj_t *icon, uint32_t color);

#endif /* WXY_ICON_H */
