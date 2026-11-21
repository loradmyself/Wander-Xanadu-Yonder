/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY ACPU 入口。
 *
 * ACPU 不跑 main()：SDK 的 acpu_ctrl 框架在收到 HCPU 的任务请求后，
 * 按 task_name 调到这里。第一阶段只做"上线握手 + 回声"，用来验证：
 *   1. ACPU 固件能被 HCPU 拉起来
 *   2. 跨核消息通道可用
 * 真正的计算组件（图像解码 / 字体光栅化 / 排版）后续挂在 task 分发里。
 *
 * 这里刻意只用 rt_kprintf：ACPU 侧尚未启用 ulog，避免为一个日志宏引入配置依赖。
 */

#include "rtconfig.h"
#include <board.h>
#include <rtthread.h>
#include "acpu_ctrl.h"

/* 由 acpu_ctrl.c 的弱符号覆盖；签名必须与 SDK 保持一致。 */
void __acpu_main(uint8_t task_name, void *param)
{
    (void)param;

    rt_kprintf("[ACPU] online, task=%d\n", (int)task_name);

    /* 第一阶段不实现具体任务，收到请求即回一条可识别日志，
     * 便于在串口上确认跨核链路是通的。 */
    switch (task_name)
    {
    case 0:
        rt_kprintf("[ACPU] idle\n");
        break;
    default:
        rt_kprintf("[ACPU] unhandled task %d\n", (int)task_name);
        break;
    }
}
