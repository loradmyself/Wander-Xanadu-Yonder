/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * ezip 图标资源查询接口（wxy_icon_ezip.c 提供实现）。
 * 由 wxy_slots.c 调用：查到资源走 lv_image，查不到回退字体/图元渲染。
 */
#ifndef WXY_ICON_EZIP_H
#define WXY_ICON_EZIP_H

#include <stdint.h>
#include "lvgl.h"
#include "slots.h"

/* 返回槽位在 px 档（16/24/32）的 ezip 图片描述符；无资源返回 NULL。 */
const lv_image_dsc_t *wxy_icon_ezip_get(wxy_slot_t slot, int32_t px);

#endif /* WXY_ICON_EZIP_H */
