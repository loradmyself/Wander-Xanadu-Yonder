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



/* LVGL 演示 UI：200x200 绿色按钮，屏幕居中，按钮上写 HELLO。
 * 必须在 littlevgl2rtt_init() 之后、且只在 LVGL 任务线程里创建：
 * LVGL 对象树不是线程安全的，不要在 main() 里直接调。 */
static void wxy_lvgl_demo_ui(void)
{
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 200, 200);
    lv_obj_center(btn);

    /* 绿色按钮：默认亮绿，按下深绿；去掉边框和阴影避免遮挡纯色 */
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00E676), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00C853), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "HELLO");
    lv_obj_center(label);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
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
    wxy_lvgl_demo_ui();
    /* 运行 lvgl 任务 */
    while (1)
    {
        ms = lv_task_handler();
        rt_thread_mdelay(ms);
    }

}

#if 1
/**
            "offset": "0x008E0000",
                "max_size": "0x00400000",
                "name": "fs_root",
                "tags": [
                    "FS_REGION"
                ]
 */
#include "drv_flash.h"
#include "dfs_file.h"

#define FS_ROOT_NAME "WXY_ROOT"
#define FS_REGION_START_ADDR 0x12000000
#define FS_REGION_START_ADDR_OFFSET 0x00008E00
#define FS_REGION_SIZE 0x00400000

int mnt_init(void)
{
    //TODO: how to get base address
    register_mtd_device(FS_REGION_START_ADDR+FS_REGION_START_ADDR_OFFSET,FS_REGION_SIZE , FS_ROOT_NAME);
    if (dfs_mount(FS_ROOT_NAME, "/", "elm", 0, 0) == 0) // fs exist
    {
        rt_kprintf("mount fs on flash to root success\n");
    }
    else
    {
        // auto mkfs, remove it if you want to mkfs manual
        rt_kprintf("mount fs on flash to root fail\n");
        if (dfs_mkfs("elm",FS_ROOT_NAME ) == 0)
        {
            rt_kprintf("make elm fs on flash sucess, mount again\n");
            if (dfs_mount(FS_ROOT_NAME, "/", "elm", 0, 0) == 0)
                rt_kprintf("mount fs on flash success\n");
            else
                rt_kprintf("mount to fs on flash fail\n");
        }
        else
            rt_kprintf("dfs_mkfs elm flash fail\n");
    }
    return RT_EOK;
}
INIT_ENV_EXPORT(mnt_init);
#endif



int main(void)
{
    rt_device_t p_device;
    rt_thread_t tid;

    wxy_banner();
    LOG_I("boot ok, tick=%d", (int)rt_tick_get());
    p_device = rt_device_find("lcd");
    if (p_device)
    {
        tid = rt_thread_create("lcd_refr", lvgl_test, p_device, 8*1024, 30, 10);
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
