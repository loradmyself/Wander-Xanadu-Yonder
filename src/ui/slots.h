/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 图标槽位契约（初版资源约定）
 *
 * 设计约定：图标是"槽位"，不是散落的文件路径。页面代码只引用 wxy_slot_t 枚举，
 * 主题按槽位提供资源；页面永远不写死图片路径——这是初版就用真图片占位、
 * 后期换主题不返工的前提。
 *
 * 资源形态：单色 alpha 遮罩图，运行期由 lv_image 的 recolor 按令牌染色，
 * 一套图形适配三套主题，避免三套主题各存一份把 Flash 吃爆。
 * 表情动画（F-DISP-05，序列帧 / Lottie / RLE）是独立资源类别，
 * 虽在本表占位，但按情绪状态机映射动画 ID，不按主题染色。
 *
 * 命名规则：资源文件名 wxy_ic_<槽位小写名>_<尺寸>.png，例如
 *   wxy_ic_mic_24.png、wxy_ic_face_calm_32.png
 * 槽位名即文件名中段，枚举名去掉 WXY_SLOT_ 前缀转小写、连字符替下划线。
 *
 * 回退规则：主题未提供的槽位自动回退到基准主题 ink 的同名槽位；
 * 基准主题也缺失时回退到 24px 的通用占位遮罩（构建期保证该占位一定存在）。
 *
 * 依赖：仅 stdint，供 theme.h 的实现文件与资源层同时包含。
 */

#ifndef WXY_SLOTS_H
#define WXY_SLOTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* 一、槽位枚举：与原型 v0.4 的 29 个图标槽位一一对应                     */
/* ------------------------------------------------------------------ */

typedef enum {
    /* 语音会话 */
    WXY_SLOT_MIC = 0,           /* 按住说话 */
    WXY_SLOT_SEND,              /* 发送 */
    WXY_SLOT_BACK,              /* 返回 / 唱针退回 */

    /* 导航与轨道 */
    WXY_SLOT_CHAT,              /* 对谈碟 */
    WXY_SLOT_HOME,              /* 桌面碟 */
    WXY_SLOT_TOOLS,             /* 工具碟 */
    WXY_SLOT_SET,               /* 设置碟 */
    WXY_SLOT_EYE,               /* 主题 / 预览 */
    WXY_SLOT_DISC,              /* 碟仓入口 */
    WXY_SLOT_EJECT,             /* 换碟 / 弹碟 */

    /* 状态栏 */
    WXY_SLOT_BATTERY,
    WXY_SLOT_WIFI,
    WXY_SLOT_SIGNAL,

    /* 应用图标 */
    WXY_SLOT_CLOCK,             /* 计时 */
    WXY_SLOT_WEATHER,
    WXY_SLOT_MUSIC,
    WXY_SLOT_NOTE,
    WXY_SLOT_HEART,             /* 陪伴 */

    /* 播放控制 */
    WXY_SLOT_PLAY,
    WXY_SLOT_PAUSE,
    WXY_SLOT_PREV,
    WXY_SLOT_NEXT,
    WXY_SLOT_STOP,
    WXY_SLOT_REFRESH,

    /* 通用 */
    WXY_SLOT_CHECK,
    WXY_SLOT_PLUS,
    WXY_SLOT_CLOUD,

    /* 表情（F-DISP-05，独立资源类别：动画 ID，不参与主题染色） */
    WXY_SLOT_FACE_CALM,
    WXY_SLOT_FACE_HAPPY,
    WXY_SLOT_FACE_FIRE,

    WXY_SLOT_COUNT
} wxy_slot_t;

/* 表情槽位段：便于按情绪轴批量取用，与 theme.h 的 wxy_mood_t 顺序一致。 */
#define WXY_SLOT_FACE_BASE      WXY_SLOT_FACE_CALM
#define WXY_SLOT_FACE_BY_MOOD(m) ((wxy_slot_t)(WXY_SLOT_FACE_BASE + (m)))
#define WXY_SLOT_IS_FACE(s)     ((s) >= WXY_SLOT_FACE_BASE && (s) <= WXY_SLOT_FACE_FIRE)

/* ------------------------------------------------------------------ */
/* 二、尺寸档：逻辑尺寸 px，只取三档，避免资源碎片                        */
/* ------------------------------------------------------------------ */

typedef enum {
    WXY_SLOT_SIZE_16 = 16,      /* 状态栏 / 小标记 */
    WXY_SLOT_SIZE_24 = 24,      /* 默认控件图标 */
    WXY_SLOT_SIZE_32 = 32       /* 碟仓 / 大按钮 / 表情静态帧 */
} wxy_slot_size_t;

#define WXY_SLOT_SIZE_COUNT 3

/* 槽位默认档：页面不指定时用这个。 */
#define WXY_SLOT_SIZE_DEFAULT WXY_SLOT_SIZE_24

/* ------------------------------------------------------------------ */
/* 三、槽位元数据：供资源加载器与自检使用（const 表，放 Flash）            */
/* ------------------------------------------------------------------ */

typedef struct {
    wxy_slot_t     slot;        /* 槽位 ID */
    const char    *name;        /* 小写名，用于拼文件名，如 "mic" */
    wxy_slot_size_t default_size; /* 该槽位常用尺寸档 */
    bool           face_anim;  /* true = 表情动画类，不走主题染色 */
} wxy_slot_meta_t;

extern const wxy_slot_meta_t wxy_slot_meta[];
extern const uint8_t WXY_SLOT_META_COUNT;

/* ------------------------------------------------------------------ */
/* 四、查询 API                                                        */
/* ------------------------------------------------------------------ */

/* 按槽位取元数据（名字 / 默认尺寸 / 是否动画类）。未找到返回 NULL。 */
const wxy_slot_meta_t *wxy_slot_info(wxy_slot_t slot);

/* 拼出资源文件名到 buf，返回写入长度（不含结尾 0）；buf 不足返回 0。
 * 例：wxy_slot_filename(WXY_SLOT_MIC, 24, buf, 64) -> "wxy_ic_mic_24.png" */
uint16_t wxy_slot_filename(wxy_slot_t slot, wxy_slot_size_t size,
                           char *buf, uint16_t buf_len);

/* 槽位合法性检查，供构建期自检遍历用。 */
bool wxy_slot_valid(wxy_slot_t slot);

#ifdef __cplusplus
}
#endif

#endif /* WXY_SLOTS_H */
