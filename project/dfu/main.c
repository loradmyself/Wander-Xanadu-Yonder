/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * TF 卡 DFU 装载器（裸机）。
 *
 * 现状：第一阶段只搭骨架，不含显示、文件系统与包解析。
 * 启用条件（缺一不可）：
 *   1. ptab.json 补上 DFU_LOADER_START_ADDR/SIZE 与 DFU_EXEC_START_ADDR/SIZE
 *      —— 本装载器在 PSRAM 中执行，再从那里写 flash；
 *   2. project/proj.conf 打开 CONFIG_WXY_TF_DFU=y；
 *   3. 补齐 dfu_display / dfu_media / dfu_package 三个模块。
 *
 * 在以上条件满足前，本工程不参与构建（Kconfig 默认 n），因此不影响主固件编译。
 */

#include <stdint.h>
#include "bf0_hal.h"
#include "ptab.h"

/* startup.S 的向量表引用，必须提供。 */
void dfu_fault(void)
{
    for (;;)
    {
    }
}

void SysTick_Handler(void)
{
}

/* startup.S 在清零 bss、搬运 .flash_ops 之后跳到这里，不返回。 */
void dfu_main(void)
{
#if defined(DFU_LOADER_SIZE) && defined(DFU_EXEC_START_ADDR)
    volatile uint32_t loader_size = DFU_LOADER_SIZE;
    volatile uint32_t exec_addr = DFU_EXEC_START_ADDR;
    (void)loader_size;
    (void)exec_addr;
#endif

    /* TODO: TF 卡挂载 → 扫描升级包 → 校验 → 写 flash → 回显进度 */
    for (;;)
    {
    }
}
