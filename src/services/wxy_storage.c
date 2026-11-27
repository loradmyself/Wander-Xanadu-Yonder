/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * 存储底座：把 fs_root 分区挂到根目录。
 *
 * 地址不再硬编码——直接用 ptab.json 生成的 ptab.h（FS_REGION_START_ADDR /
 * FS_REGION_SIZE），分区表改了这里自动跟着变。
 *
 * 首次上电分区是空的，mount 失败后自动 mkfs（elm/FAT）。正式版要区分
 * "首次使用"和"文件系统损坏"，这里先按 demo 处理。
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_flash.h"
#include "dfs_file.h"
#include "ptab.h"

#define LOG_TAG "WXY.FS"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

#define WXY_FS_DEV_NAME "fs_root"

static int wxy_fs_mount(void)
{
    register_mtd_device(FS_REGION_START_ADDR, FS_REGION_SIZE, WXY_FS_DEV_NAME);

    if (dfs_mount(WXY_FS_DEV_NAME, "/", "elm", 0, 0) == 0)
    {
        LOG_I("mounted fs_root at / (%d KB)", (int)(FS_REGION_SIZE / 1024));
        return RT_EOK;
    }

    LOG_W("mount failed, formatting fs_root");
    if (dfs_mkfs("elm", WXY_FS_DEV_NAME) != 0)
    {
        LOG_E("mkfs failed");
        return -RT_ERROR;
    }

    if (dfs_mount(WXY_FS_DEV_NAME, "/", "elm", 0, 0) != 0)
    {
        LOG_E("mount after mkfs failed");
        return -RT_ERROR;
    }

    LOG_I("formatted and mounted fs_root at /");
    return RT_EOK;
}

/* 早于 main() 执行，保证平台代码能直接读写 / */
INIT_ENV_EXPORT(wxy_fs_mount);
