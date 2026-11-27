#include <string.h>

#include <rtthread.h>
#include "littlevgl2rtt.h"

#include "wxy_kit.h"
#include "wxy_icon.h"
#include "wxy_disc.h"
#include "wxy_scene.h"

#define LOG_TAG "WXY.UI.STATUS_BAR"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

extern const lv_image_dsc_t wxy_ic_wifi_16;
extern const lv_image_dsc_t wxy_ic_battery_16;

/* 状态栏内容
 * 用于显示当前播放状态、时间等信息。
 * Wifi 、蓝牙、电池状态等。
 */

void status_bar_init(void)
{
    lv_obj_t *st = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(st, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_align(st, LV_TEXT_ALIGN_CENTER, 0);
    wxy_reg(st, wxy_apply_text);
}
