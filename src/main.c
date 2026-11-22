/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * WXY 平台入口。
 *
 * 第一阶段只做一件事：证明"芯片起来 → 内核调度 → 平台层可见"这条链路是通的。
 * 它刻意不初始化显示、音频、网络，也不依赖任何服务，出问题时能一眼定位到
 * 是板级、配置还是启动流程。
 */

#include <rtthread.h>
#include "xyz/xyz_version.h"
#include "littlevgl2rtt.h"
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



void lvgl_test(void* parameter)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t ms;

    /* init littlevGL */
    ret = littlevgl2rtt_init("lcd");
    if (ret != RT_EOK)
    {
        LOG_E("init failed, tick=%d", (int)rt_tick_get());
        return;
    }
    /* 运行 lvgl 任务 */
    while (1)
    {
        ms = lv_task_handler();
        rt_thread_mdelay(ms);
    }

}



int main(void)
{
    rt_device_t p_device;
    rt_thread_t tid;

    wxy_banner();
    LOG_I("boot ok, tick=%d", (int)rt_tick_get());
    p_device = rt_device_find("lcd");
    if (p_device)
    {
        tid = rt_thread_create("lcd_refr", lvgl_test, p_device, 1024, 30, 10);
        rt_thread_startup(tid);
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
