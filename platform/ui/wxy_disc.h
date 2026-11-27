/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 唱片模型与唱机（换碟唱机）
 *
 * 设计要点（对应 ui-logic-prototype.html v0.4）：
 *   - 每个界面是一张"碟"，碟有独立身份：编号 / 名称 / 图标 / 碟面主色 / 唱针落点；
 *   - 换界面不是创建销毁页面，而是"换碟"：抬针 -> 旧碟旋出 -> 换涂装 -> 新碟旋入 -> 落针；
 *   - 唱臂是一个物理模型：支点固定，落点角度决定针尖位置，越往内角度越大。
 *
 * 页面/导航不直接碰这里的控件，只调下面这几个动作函数，
 * 这样换碟动效以后怎么改都不影响页面代码。
 */

#ifndef WXY_DISC_H
#define WXY_DISC_H

#include <stdint.h>
#include <stdbool.h>

#include "lvgl.h"
#include "slots.h"

#define WXY_DISC_KIND_TRACK 0   /* 外圈轨道：对谈/桌面/工具/设置 */
#define WXY_DISC_KIND_INNER 1   /* 内圈子层：设置 -> 主题 */
#define WXY_DISC_KIND_APP   2   /* 应用碟：换入后读碟窗泛起应用内容 */

typedef struct {
    const char *id;         /* "chat" / "music" ... 与 scene id 一致 */
    const char *num;        /* "01" / "M1" */
    const char *name;       /* 碟名 */
    wxy_slot_t  slot;       /* 碟面图标槽位 */
    uint32_t    hue;        /* 碟面主色（每碟独立，不随主题变） */
    uint16_t    arm_deg;    /* 唱针落点角度（度） */
    uint8_t     kind;       /* WXY_DISC_KIND_* */
} wxy_disc_t;

/* 定长上限：数组维度必须是编译期常量，WXY_DISC_COUNT 是 const 变量不行 */
#define WXY_DISC_MAX 10

extern const wxy_disc_t wxy_discs[];
extern const uint8_t WXY_DISC_COUNT;

const wxy_disc_t *wxy_disc_get(const char *id);
const wxy_disc_t *wxy_disc_nth(uint8_t i);

/* 碟身份色查询。页面里想画"某张碟"的缩略图/角标时用它，
 * 不要自己抄一遍色值——碟表改了页面会跟着飘。查不到返回 0xFFFFFF。 */
uint32_t wxy_disc_hue(const char *id);

/* ------------------------------------------------------------------ */
/* 唱机动作                                                            */
/* ------------------------------------------------------------------ */

typedef void (*wxy_tt_cb_t)(void);
typedef void (*wxy_tt_pick_cb_t)(const wxy_disc_t *d);

/* 在 parent 上建整台唱机：碟、唱臂、轨道点、角落小 CD、底部走带区。
 * 必须在 LVGL 线程调用一次。 */
void wxy_tt_create(lv_obj_t *parent);

void wxy_tt_set_transport_cb(wxy_tt_cb_t prev, wxy_tt_cb_t play, wxy_tt_cb_t next);

/* 点轨道点 / 碟仓里某张碟时的回调，参数是目标碟 */
void wxy_tt_set_pick_cb(wxy_tt_pick_cb_t cb);

/* 只换涂装（不动画、不换碟），用于初始化与外部强制同步 */
void wxy_tt_paint(const wxy_disc_t *d);

/* 换碟主动作：抬针 -> 旋出 -> 换涂装 -> 旋入 -> 落针。dur 用主题的 SWAP 时长。
 * done 在动作结束时回调（可为 NULL）。 */
void wxy_tt_swap(const wxy_disc_t *d, uint16_t dur, wxy_tt_cb_t done);

/* 单独把唱针移到某张碟的落点（deep=true 表示深入内圈一点） */
void wxy_tt_arm(const wxy_disc_t *d, bool deep);

/* 播放进度 0..100：推进唱针与读纹光点 */
void wxy_tt_progress(uint8_t pct);

/* 播放态：读纹光点显隐 + 走带按钮图标 */
void wxy_tt_set_playing(bool on);

/* 把唱臂与碟仓抬到内容层之上（内容层建好后调用一次） */
void wxy_tt_raise(void);

/* 碟仓开合 */
void wxy_tt_tray(bool open);
bool wxy_tt_tray_open(void);

/* 音乐换曲时把碟标签临时改成曲名（NULL 恢复碟名） */
void wxy_tt_label_name(const char *s);

#endif /* WXY_DISC_H */
