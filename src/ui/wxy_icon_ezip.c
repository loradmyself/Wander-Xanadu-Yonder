/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * ezip 图标资源注册表。
 *
 * 资源由 SDK 工具生成（png2ezip/ezip.exe，lvgl_version 9）：
 *   ezip -convert <png> -rgb565 -cfile 2 -section ROM3_IMG \
 *         -winsize 2048 -lvgl_version 9
 * 生成的 C 文件放在 icons_ezip/，段名 .ROM3_IMG_EZIP.*，链接进 ROM2 (flash2 XIP)。
 * 运行时 EPIC 硬件解压 blit（lv_draw_epic_img.c 的 EPIC_INPUT_EZIP 路径）。
 *
 * 槽位映射依据 slots.h 的命名契约：wxy_ic_<slot中段>_<px>.png。
 */
#include "wxy_icon_ezip.h"
#include "lvgl.h"

/* ---- 生成资源声明（icons_ezip/*.c 定义） ---- */
#define WXY_EZIP_EXTERN(name) \
    extern const lv_image_dsc_t name

WXY_EZIP_EXTERN(wxy_ic_back_24);
WXY_EZIP_EXTERN(wxy_ic_battery_16);
WXY_EZIP_EXTERN(wxy_ic_chat_24);
WXY_EZIP_EXTERN(wxy_ic_check_16);
WXY_EZIP_EXTERN(wxy_ic_clock_24);
WXY_EZIP_EXTERN(wxy_ic_cloud_24);
WXY_EZIP_EXTERN(wxy_ic_disc_32);
WXY_EZIP_EXTERN(wxy_ic_eject_24);
WXY_EZIP_EXTERN(wxy_ic_eye_24);
WXY_EZIP_EXTERN(wxy_ic_face_calm_32);
WXY_EZIP_EXTERN(wxy_ic_face_fire_32);
WXY_EZIP_EXTERN(wxy_ic_face_happy_32);
WXY_EZIP_EXTERN(wxy_ic_heart_24);
WXY_EZIP_EXTERN(wxy_ic_home_24);
WXY_EZIP_EXTERN(wxy_ic_mic_24);
WXY_EZIP_EXTERN(wxy_ic_music_24);
WXY_EZIP_EXTERN(wxy_ic_next_24);
WXY_EZIP_EXTERN(wxy_ic_note_24);
WXY_EZIP_EXTERN(wxy_ic_pause_24);
WXY_EZIP_EXTERN(wxy_ic_play_24);
WXY_EZIP_EXTERN(wxy_ic_plus_24);
WXY_EZIP_EXTERN(wxy_ic_prev_24);
WXY_EZIP_EXTERN(wxy_ic_refresh_24);
WXY_EZIP_EXTERN(wxy_ic_send_24);
WXY_EZIP_EXTERN(wxy_ic_set_24);
WXY_EZIP_EXTERN(wxy_ic_signal_16);
WXY_EZIP_EXTERN(wxy_ic_stop_24);
WXY_EZIP_EXTERN(wxy_ic_tools_24);
WXY_EZIP_EXTERN(wxy_ic_weather_24);
WXY_EZIP_EXTERN(wxy_ic_wifi_16);

/* ---- 槽位 -> 资源映射（按 px 档） ---- */

typedef struct {
    wxy_slot_t slot;
    const lv_image_dsc_t *img[3];   /* 下标 [px/16 - 1]：16 / 24 / 32 */
} wxy_ezip_entry_t;

#define IMG16(v) [(WXY_SLOT_SIZE_16 / 16) - 1] = (v)
#define IMG24(v) [(WXY_SLOT_SIZE_24 / 16) - 1] = (v)
#define IMG32(v) [(WXY_SLOT_SIZE_32 / 16) - 1] = (v)

static const wxy_ezip_entry_t s_ezip_map[] = {
    { WXY_SLOT_BACK,    { IMG24(&wxy_ic_back_24) } },
    { WXY_SLOT_BATTERY, { IMG16(&wxy_ic_battery_16) } },
    { WXY_SLOT_CHAT,    { IMG24(&wxy_ic_chat_24) } },
    { WXY_SLOT_CHECK,   { IMG16(&wxy_ic_check_16) } },
    { WXY_SLOT_CLOCK,   { IMG24(&wxy_ic_clock_24) } },
    { WXY_SLOT_CLOUD,   { IMG24(&wxy_ic_cloud_24) } },
    { WXY_SLOT_DISC,    { IMG32(&wxy_ic_disc_32) } },
    { WXY_SLOT_EJECT,   { IMG24(&wxy_ic_eject_24) } },
    { WXY_SLOT_EYE,     { IMG24(&wxy_ic_eye_24) } },
    { WXY_SLOT_FACE_CALM,  { IMG32(&wxy_ic_face_calm_32) } },
    { WXY_SLOT_FACE_FIRE,  { IMG32(&wxy_ic_face_fire_32) } },
    { WXY_SLOT_FACE_HAPPY, { IMG32(&wxy_ic_face_happy_32) } },
    { WXY_SLOT_HEART,   { IMG24(&wxy_ic_heart_24) } },
    { WXY_SLOT_HOME,    { IMG24(&wxy_ic_home_24) } },
    { WXY_SLOT_MIC,     { IMG24(&wxy_ic_mic_24) } },
    { WXY_SLOT_MUSIC,   { IMG24(&wxy_ic_music_24) } },
    { WXY_SLOT_NEXT,    { IMG24(&wxy_ic_next_24) } },
    { WXY_SLOT_NOTE,    { IMG24(&wxy_ic_note_24) } },
    { WXY_SLOT_PAUSE,   { IMG24(&wxy_ic_pause_24) } },
    { WXY_SLOT_PLAY,    { IMG24(&wxy_ic_play_24) } },
    { WXY_SLOT_PLUS,    { IMG24(&wxy_ic_plus_24) } },
    { WXY_SLOT_PREV,    { IMG24(&wxy_ic_prev_24) } },
    { WXY_SLOT_REFRESH, { IMG24(&wxy_ic_refresh_24) } },
    { WXY_SLOT_SEND,    { IMG24(&wxy_ic_send_24) } },
    { WXY_SLOT_SET,     { IMG24(&wxy_ic_set_24) } },
    { WXY_SLOT_SIGNAL,  { IMG16(&wxy_ic_signal_16) } },
    { WXY_SLOT_STOP,    { IMG24(&wxy_ic_stop_24) } },
    { WXY_SLOT_TOOLS,   { IMG24(&wxy_ic_tools_24) } },
    { WXY_SLOT_WEATHER, { IMG24(&wxy_ic_weather_24) } },
    { WXY_SLOT_WIFI,    { IMG16(&wxy_ic_wifi_16) } },
};

const lv_image_dsc_t *wxy_icon_ezip_get(wxy_slot_t slot, int32_t px)
{
    if (!wxy_slot_valid(slot))
        return NULL;

    int idx;
    if (px <= WXY_SLOT_SIZE_16)      idx = 0;
    else if (px <= WXY_SLOT_SIZE_24) idx = 1;
    else                             idx = 2;

    for (unsigned i = 0; i < sizeof(s_ezip_map) / sizeof(s_ezip_map[0]); i++)
    {
        if (s_ezip_map[i].slot == slot)
            return s_ezip_map[i].img[idx];
    }
    return NULL;
}
