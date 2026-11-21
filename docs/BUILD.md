# 编译指南

第一阶段目标只有一个：**编译出一个能启动、能打日志的固件**。
显示、音频、网络、应用装载都还没接，这是刻意的。

## 一、前置条件

| 项 | 要求 |
|---|---|
| 工具链 | SiFli SDK 自带的 `arm-none-eabi-gcc`（运行过 `SiFli-SDK/set_env.bat`） |
| SDK | `SiFli-SDK/` 子模块已就位，环境变量 `SIFLI_SDK` 指向它 |
| Python / SCons | SDK 自带的 `scons` 与 Python |
| 板级 | `boards/Wander-Xanadu-Yonder/{hcpu,acpu,lcpu}` |

首次克隆仓库后：

```bat
git submodule update --init --depth 1
cd SiFli-SDK
set_env.bat
```

## 二、编译

在 **`project/`** 目录执行（不是在仓库根）：

```bash
scons --board=Wander-Xanadu-Yonder -j8
```

产物：

```
project/build_Wander-Xanadu-Yonder_hcpu/bf0_ap.bin     ← 主固件
project/build_Wander-Xanadu-Yonder_hcpu/bootloader/    ← 二级引导
project/build_Wander-Xanadu-Yonder_hcpu/acpu/acpu.bin  ← ACPU 固件
```

VSCode 用 SiFli 插件时，打开 **`project/`** 目录作为工程根；插件会通过
`boards/` 发现板级。若板级列表为空，在工程设置里把 board search path 指到
仓库的 `boards/`。

## 三、这两个开关决定编译范围

都在 `project/proj.conf`：

```conf
CONFIG_WXY_COMPUTE_ACPU=y    # 编 ACPU 子工程（默认开）
# CONFIG_WXY_TF_DFU is not set  # 编 TF 卡 DFU 装载器（默认关）
```

**DFU 为什么默认关**：它在 PSRAM 里执行，链接脚本需要 `ptab.h` 提供
`DFU_EXEC_START_ADDR` 与 `DFU_LOADER_SIZE`，而当前 `boards/Wander-Xanadu-Yonder/ptab.json`
还没有 `dfu_exec` / `dfu_loader` 这两个区。分区表补齐后，把开关打开即可，
`project/dfu/` 里的 `main.c`、`startup.S`、`link.lds`、`SConscript` 已经就位。

## 四、预期现象

串口（默认 115200，UART1）应看到：

```
========================================
 Wander-Xanadu-Yonder
 platform v0.1.0 / ABI v1
 board: Wander-Xanadu-Yonder
 core : HCPU @ 240 MHz
========================================
[I/WXY] boot ok, tick=...
```

之后每 5 秒一条 debug 心跳（`LOG_LVL` 为 INFO 时不打印心跳，属正常）。

## 五、这一阶段故意没做的事

| 缺项 | 原因 |
|---|---|
| 屏幕点亮 | 具体屏驱动还没定（QSPI / 8080 / DPI 三选一），先在 UART 上验证启动 |
| LVGL | 依赖屏驱动，接屏时一起开 |
| 文件系统 / 应用装载 | 需要先冻结 `.xzp` 的 ABI，否则装进去也跑不了 |
| 蓝牙 / Wi-Fi | 关掉了 `BSP_USING_LCPU_PATCH`，避免链接缺 `lcpu_img` 符号 |
| DFU | 见上，等分区表补 `dfu_exec` / `dfu_loader` |

## 六、常见报错

| 现象 | 原因 | 处理 |
|---|---|---|
| `Please run set_env.bat` | 没设 `SIFLI_SDK` | 运行 `SiFli-SDK/set_env.bat`，或在当前 shell `set SIFLI_SDK=...` |
| 找不到板级 | 在仓库根执行了 scons | 必须进 `project/` 再执行 |
| `DFU_EXEC_START_ADDR` 未定义 | 打开了 `CONFIG_WXY_TF_DFU` 但分区表没补 | 先关掉开关，或补 ptab 分区 |
| 链接缺 `lcpu_img` 符号 | 打开了 `BSP_USING_LCPU_PATCH` | 保持关闭，等补 `src/lcpu_img.c` |
| 头文件路径过期 | 误用了参考工程的 `rtua.py` | 删除 `project/rtua.py`，让它重新生成 |
