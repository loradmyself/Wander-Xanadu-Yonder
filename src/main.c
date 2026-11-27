/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 平台入口。
 *
 * 这一阶段只做三件事：打 banner、起 UI 壳、维持心跳日志。
 * 存储挂载在 src/services/wxy_storage.c（INIT_ENV_EXPORT，早于 main 执行）；
 * 具体界面在 src/ui/wxy_ui.c，后续 AI Core / .xzp 装载都挂到那套壳上。
 */

#include <rtthread.h>
#include "xyz/xyz_version.h"
#include "wxy_ui.h"

#define LOG_TAG "WXY"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

/* 板级标识：后续换板时只改这一处。 */
#if defined(BSP_USING_BOARD_WANDER_XANADU_YONDER)
#define WXY_BOARD_NAME "Wander-Xanadu-Yonder"
#else
#define WXY_BOARD_NAME "unknown"
#endif

static void wxy_banner(void)
{
    rt_kprintf("\n");
    rt_kprintf("========================================\n");
    rt_kprintf(" %s\n", XYZ_PRODUCT_NAME);
    rt_kprintf(" platform v%s / ABI v%d\n", XYZ_VERSION_STRING, XYZ_ABI_VERSION);
    rt_kprintf(" board: %s\n", WXY_BOARD_NAME);
    rt_kprintf(" core : HCPU (SF32LB57X)\n");
    rt_kprintf("========================================\n");
}

int main(void)
{
    wxy_banner();
    LOG_I("boot ok, tick=%d", (int)rt_tick_get());

    if (wxy_ui_start() != RT_EOK)
    {
        LOG_E("ui failed to start, running headless");
    }

    /* 心跳：证明调度器在跑，同时给串口一个可观测的存活信号。 */
    for (;;)
    {
        rt_size_t total = 0, used = 0, max_used = 0;
        rt_thread_mdelay(5000);
        rt_memory_info(&total, &used, &max_used);
        LOG_D("alive, tick=%d, heap %d/%d (peak %d)", (int)rt_tick_get(),
              (int)used, (int)total, (int)max_used);
    }
}
