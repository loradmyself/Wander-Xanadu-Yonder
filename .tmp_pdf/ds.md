<!-- Page 1 -->

# SF32LB57X

# 芯片技术规格书

V0.2.2

文档编号：DS5702‐SF32LB57X‐CN

思澈科技（南京）有限公司 www.sifli.com 版权 ©2026

<!-- Image omitted -->


<!-- Page 2 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 更新历史

## 文档状态说明

文档状态 版本范围 说明

草稿 0.0.0 ~0.9.9 初稿，非正式发布，信息为初步数据，反映量产前产品的规格与性能，不能保 证准确性，随时可能更改，思澈科技将不会主动通知

发布版 1.0.0 ~1.9.9 正式发布，信息有可能还会小范围修正，以便更准确地反映量产产品的规格与 性能；如有更改，思澈科技将不会主动通知

## 本文档更新历史

日期 版本 发布说明

2026-05-29 0.1 初稿 2026-08-29 0.2 更新封装信息，IO 信息，蓝牙相关指标 2026-09-16 0.2.1 封装信息勘误 2026-09-24 0.2.2 增加了 CPU 以及蓝牙的功耗数据

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com i/74

<!-- Image omitted -->


<!-- Page 3 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 产品概述

SF32LB57X 是一系列用于超低功耗人工智能物联网 （AIoT）场景下的高集成度、高性能 MCU 芯片。芯

片采用了基于 Arm Cortex-M33 STAR-MC1 处理器的

多核架构，集成高性能 2D/2.5D 图形引擎，双模蓝牙

6.3，以及音频 codec，可广泛用于可穿戴电子设备、便 携式智能移动终端、智能家居等各种应用场景。

芯片中主系统配备两个 Cortex-M33 STAR-MC1 处理

器，最高工作频率达 240MHz，双大核性能达 1968

CoreMark，支持动态频率功耗调节，功耗效率最高可 达 5uA/CoreMark，在用于提供丰富应用和流畅人机

交互所需的高性能算力的同时，还可作为 Sensor Hub 控制多种传感器以及运行蓝牙协议栈，从而很好地兼 顾流畅人机交互所需的高计算性能与长待机时间所 需的超低功耗运行之间的平衡关系。 芯片内集成 2D/2.5D GPU，主频最高达到 240MHz，支 持多图层叠加，alpha 混叠，硬件加速的实时旋转/缩 放/投影变换，以及各种常用图形格式转换。支持硬件 加速无损压缩图形解压缩，支持原生动画，可以大幅 提高带宽利用率，降低存储成本。芯片内置 LCD 控

制器，支持 8080/QSPI/RGB/EPD/JDI 等多种接口，可

不依赖于 CPU 自主实现最高 60fps 的全屏刷新帧率， 并支持低功耗息屏常显。

集成世界水平的双模蓝牙 6.3 收发机，经典蓝牙

EDR2 模式最高发射功率 13dBm，接收峰值功耗低 至 3mA@3.3V，低功耗蓝牙接收灵敏度达到-100dBm （1Mbps），经典蓝牙 EDR2 模式灵敏度-95dBm。集成

高保真音频 ADC 和 DAC，支持蓝牙通话和连接耳机

MP3 播放。

## 功能框图

双核 Arm STAR-MC1 最高 240MHz HCPU 32KB I$ + 16KB D$ ACPU 16KB I$ + 8KB D$ FPU，MPU

eShield 安全架构

PTM微控制器 自定义外设任务

双模蓝牙PHY 存储

专用LCD控制器 单/双/四数据线 SPI DBI(8080)，DPI(RGB)-24bit JDI，EPD-16bit 专用摄像头控制器 SPI/DVP 显示/摄像头接口 超低功耗双模BT6.3, BR/EDR2/EDR3, BLE 灵敏度-100dBm@BLE 1Mbps -95dBm@EDR2 2.0mA接收电流，13dBm EDR2发射功率 无线连接 片上592KB SRAM 内置(SiP) OPI-PSRAM/QSPI-NOR Flash 外挂QSPI-NOR/NAND Flash 外挂SD-NAND/eMMC 支持Flash实时(on-the-y)解密 外围接口

ePicasso 3.0 + eZip3.0 多图层混叠 + 背景层 实时高精度（0.1°/60fps）旋转 实时缩放，投影特效 eZip无损解码 JPEG解码 硬件图形系统

1 x 12-bit SAR ADC 2 x 24bit Audio ADC，1 x 24bit Audio DAC 1 x 温度传感器 模拟外设 3 x UART 4 x I2C 2 x SPI, 2 x CAN 2 x I2S, 2 x PDM 2 x SD/SDIO/eMMC 1x USB2.0 FS 最多58个 GPIO

2 x 16b 通用定时器 GPTIM 4 x 32b 基本定时器 BTIM 2 x 32b 高级定时器 ATIM 2 x 24b 低功耗定时器 LPTIM 1 x RTC， 2 x WDT， 1 x 独立 WDT 定时器

双模蓝牙控制器 Arm STAR-MC1 最高 24MHz FreeIO IO任意组合 GPDMA

图 0-1: 功能框图

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com ii/74

<!-- Image omitted -->


<!-- Page 4 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 产品特性

CPU 与内存

- 高性能系统/双性能核（HPSYS）

– 处理器：Dual Arm Cortex-M33 STAR-MC1

– 主频：最高 240MHz，可调节，1968 CoreMark \* HCPU:32KB(2-way) I$ + 16KB(4-way) D$ \* ACPU:16KB(2-way) I$ + 8KB(4-way) D$

– SRAM：512KB（全部为 Retention SRAM）

– CoreMark 功耗效率：低至 26uA/MHz @3.3V

– 单精度浮点运算单元（FPU）

– 内存保护单元（MPU）

- 低功耗系统/能效核（LPSYS）

– 处理器：Arm Cortex-M33 STAR-MC1

– 主频：最高 24MHz，可调节

– SRAM：80KB（全部为 Retention SRAM）

无线连接

- 双模蓝牙 6.3，支持 BLE Audio，支持 LE LR

- 灵敏度：-100dBm（BLE/1Mbps） ，-107.5dBm（BLE/

125Kbps），-96dBm（BR），-95dBm（EDR2）

- 最大发射功率：13dBm（EDR2/3），17dBm（BR/BLE）

- 接收机峰值功耗（BR）：3.0mA@3.3V

音频

- 1× 高保真 24-bit 音频 DAC

– Noise floor: 3.7uVrms

– SNR(with 10kohm load and A-Weighted): 109dB

– Sample rate: 8k/ 16k/ 11.025k/ 22.05k/ 24k/ 32k/ 44.1k/ 48k/ 88.2k/ 96kHz

– 支持 192 级数字音量，具有过零检测功能

- 2× 高保真 24-bit 音频 ADC

– SNR(A-Weighted): 97dB

– Sample rate: 8k/ 11.025k/ 12k/ 16k/ 22.05k/ 24k/ 32k/ 44.1k/ 48k/ 88.2k/ 96kHz

– 数字高通滤波器可用于消除 ADC 的直流偏置

– 支持单端和全差分输入麦克风

– Micbias LDO，输出电压为 1.4V~2.8V，输出电流为 0~2mA

图形显示

- 2D/2.5D 图形引擎—ePicassoTM3.0

– 支持硬件加速的旋转、缩放，投影和镜像

– 最大解析度 2048×2048

– 支持 RGB 色彩格式包括 RGB888，RGB565， aRGB8888，aRGB8565

– 支持其他色彩格式 L8/4，G8/4/2，A8/4/2，YUV

– 支持多种 alpha 混叠方式

- 通用解压缩加速器—eZipTM3.0

– 支持无损压缩 eZip 格式和 eZip-A 动画

– 支持有损压缩 JPEG 格式图片和 MJPEG 动画

– 支持与 ePicassoTM3.0 联动，无需中间缓存

- LCD 控制器

– 支持 8080，SPI/Dual-SPI/Quad-SPI，DPI/RGB，JDI， 8/16b-EPD（电子纸）显示接口

- 摄像头控制器 DCMI

– 支持 DVP，SPI/Dual-SPI/Quad-SPI，摄像头接口

存储接口

- 支持合封（SiP）NOR-Flash，接口最高频率 104MHz

- 支持合封（SiP）OPI-PSRAM，接口最高频率 156MHz

- 3×MPI(QSPI)，支持 NOR、NAND、QPI-PSRAM

- 2×SD/SDIO，支持 SD3.0，SDIO3.0，eMMC

DMA

- GPDMA：通用高效率 DMA，支持链表模式

安全

- AES/HASH/CRC 加速器

- 真随机数发生器（TRNG）

- eShield 安全架构

- PSA Certified Level 1 认证

定时器

- 2×16b GPTIM，4×32b BTIM，2×32b ATIM

- 2×24b LPTIM，1×PWM

- 1×RTC

- 2× 看门狗 24b WDT，1× 独立看门狗 IWDT

模拟

- 1×12-bit 通用 SAR ADC，共 12 通道

- 1× 片上温度传感器

连接外设

- 最多 58 个 GPIO + 6 个 GPO 或 MPI 接口 IO

- FreeIO 功能，支持 GPIO 灵活配置

- 3×UART，4× I

2C，2×SPI

- 2× I

2 S，2× PDM

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com iii/74

<!-- Image omitted -->


<!-- Page 5 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

- 1×USB2.0 FS，2×CAN

- 外设任务控制器（PTM）

电源管理

- 内置高效率 Buck 及低功耗 LDO

- 提供两路对外 3.3V 供电，每路最大电流 150mA

- 休眠功耗：1uA@3.3V 供电

其它

- 工作温度：-40 到 85◦C

- 封装：

– QFN68L，7mm×7mm，3.3V 供电，47 个 GPIO

– QFN80L，8mm×8mm，3.3V 供电，58 个 GPIO

– BGA112，5.4mm×4.4mm，3.3V 供电，最多 58 个 GPIO + 6 个 GPO 或 MPI 接口 IO

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com

### iv/74

<!-- Image omitted -->


<!-- Page 6 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 应用场景

智能穿戴

- 高端智能手表

- 智能手环

- 可穿戴医疗器材

- 健身器材

工业

- 高性价比显示方案

- 图形化人机交互设备

- 工业传感器控制中心

- 工业设备监测

- 工业仪器仪表

车载

- 电动车中控设备

- 汽车钥匙

- 穿戴式汽车遥控设备

家庭自动化

- 智能家居中控

- 中小型智能家电

- 智能门锁

智能设备

- 智能录音卡

- 智能蓝牙对讲机

- 电子纸阅读器

- 电子相框

通用

- 低功耗传感器中心

- 蓝牙 mesh

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com v/74

<!-- Image omitted -->


<!-- Page 7 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 目录

更新历史 i

产品概述 ii 功能框图 . . . . . . . . . . . . . . . . . . . . . . ii 产品特性 . . . . . . . . . . . . . . . . . . . . . . iii 应用场景 . . . . . . . . . . . . . . . . . . . . . . v

1 芯片总览 1 1.1 系统架构 . . . . . . . . . . . . . . . . . . 1 1.2 Cortex-M33 STAR-MC1 “星辰”处理器 1 1.3 高性能（双性能核）系统（HPSYS） . . 2 1.3.1 总线架构 . . . . . . . . . . . . . . 2 1.3.2 时钟架构 . . . . . . . . . . . . . . 3 1.3.3 存储器类型 . . . . . . . . . . . . 4 1.3.3.1 Cache . . . . . . . . . . 4 1.3.3.2 TCM . . . . . . . . . . . 4 1.3.3.3 SRAM . . . . . . . . . . 4 1.3.3.4 片外 RAM . . . . . . . 4 1.3.3.5 片外 Flash . . . . . . . 4 1.3.4 地址映射 . . . . . . . . . . . . . . 4 1.3.5 中断列表 . . . . . . . . . . . . . . 6 1.4 电源管理 . . . . . . . . . . . . . . . . . . 7

2 高性能专用计算 8 2.1 ePicassoTM 高性能 2.5D 图形引擎 . . . . 8 2.1.1 图层叠加 . . . . . . . . . . . . . . 8 2.1.2 普通图层 . . . . . . . . . . . . . . 8 2.1.3 功能图层 . . . . . . . . . . . . . . 8 2.1.4 投射图层 . . . . . . . . . . . . . . 8 2.2 LCD 控制器 . . . . . . . . . . . . . . . . . 8 2.2.1 显示接口 . . . . . . . . . . . . . . 8 2.2.1.1 MIPI-DBI . . . . . . . . 9 2.2.1.2 MIPI-DPI . . . . . . . . 9 2.2.1.3 JDI 反射屏 . . . . . . . 9 2.2.1.4 EPD 电子纸屏幕 . . . . 9 2.3 eZipTM 通用压缩解码器 . . . . . . . . . . 9

3 外设 11 3.1 双模蓝牙 6.3 . . . . . . . . . . . . . . . . 11 3.1.1 射频和基带 . . . . . . . . . . . . 11 3.1.2 BT MAC . . . . . . . . . . . . . . 11 3.2 模拟外设 . . . . . . . . . . . . . . . . . . 12 3.2.1 12 比特模/数转换器 . . . . . . . 12 3.2.2 音频 DAC . . . . . . . . . . . . . . 12 3.2.3 音频 PLL . . . . . . . . . . . . . . 12 3.2.4 音频 ADC . . . . . . . . . . . . . 12 3.3 GPDMA . . . . . . . . . . . . . . . . . . . 12 3.3.1 GPDMA . . . . . . . . . . . . . . . 12 3.4 音频处理模块 AUDPRC . . . . . . . . . . 13 3.4.1 DAC 通路 . . . . . . . . . . . . . . 13 3.4.2 ADC 通路 . . . . . . . . . . . . . 13 3.5 连接外设 . . . . . . . . . . . . . . . . . . 14 3.5.1 通用输入/输出接口（GPIO） . . 14 3.5.2 通用异步收发器（UART） . . . 14 3.5.3 I2C . . . . . . . . . . . . . . . . . 15 3.5.4 PDM . . . . . . . . . . . . . . . . . 15 3.5.5 I2S . . . . . . . . . . . . . . . . . . 16 3.5.6 串行外设接口（SPI） . . . . . . 16 3.5.7 外设任务控制器（PTM） . . . . 18 3.5.8 USB2.0 FS . . . . . . . . . . . . . 19 3.5.9 DCMI . . . . . . . . . . . . . . . . 19 3.6 定时器 . . . . . . . . . . . . . . . . . . . . 19 3.6.1 通用定时器 . . . . . . . . . . . . 19 3.6.2 高级定时器 . . . . . . . . . . . . 20 3.6.3 基本定时器 . . . . . . . . . . . . 21 3.6.4 低功耗定时器 . . . . . . . . . . . 22 3.6.5 看门狗 . . . . . . . . . . . . . . . 22 3.7 加密校验 . . . . . . . . . . . . . . . . . . 23 3.7.1 AES 引擎 . . . . . . . . . . . . . . 23 3.7.2 HASH 引擎 . . . . . . . . . . . . . 23 3.7.3 CRC . . . . . . . . . . . . . . . . . 23 3.7.4 真随机数发生器（TRNG） . . . 24 3.8 存储接口 . . . . . . . . . . . . . . . . . . 24 3.8.1 MPI 接口 . . . . . . . . . . . . . . 24 3.8.2 SD/SDIO/eMMC . . . . . . . . . . 25 3.9 CAN . . . . . . . . . . . . . . . . . . . . . 26 3.10 各种外设接口速率汇总 . . . . . . . . . . 26

4 电气特性 27 4.1 基本电气特性 . . . . . . . . . . . . . . . . 27 4.2 可靠性 . . . . . . . . . . . . . . . . . . . . 28 4.3 功耗特性 . . . . . . . . . . . . . . . . . . 28 4.3.1 处理器功耗 . . . . . . . . . . . . 29 4.3.2 BT 和 BLE 功耗 . . . . . . . . . . 29 4.4 蓝牙射频 . . . . . . . . . . . . . . . . . . 30 4.4.1 BLE 射频 . . . . . . . . . . . . . . 30 4.4.1.1 BLE 发射机性能 . . . . 30 4.4.1.2 BLE 接收机性能 . . . . 31 4.4.2 经典蓝牙 . . . . . . . . . . . . . . 33 4.4.2.1 发射机性能 . . . . . . . 34 4.4.2.2 接收机性能 . . . . . . . 35 4.5 音频性能 . . . . . . . . . . . . . . . . . . 36

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com vi/74

<!-- Image omitted -->


<!-- Page 8 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

4.6 IO 驱动能力 . . . . . . . . . . . . . . . . 37 4.7 GPADC 电气特性 . . . . . . . . . . . . . 37

5 封装与硬件 39 5.1 管脚布局 . . . . . . . . . . . . . . . . . . 39 5.2 管脚描述 . . . . . . . . . . . . . . . . . . 42 5.2.1 大核域 GPIO（PA）列表 . . . . 43 5.2.2 专用管脚（电源、射频、模拟、 数字）列表 . . . . . . . . . . . . 50

5.3 封装尺寸 . . . . . . . . . . . . . . . . . . 56 5.3.1 QFN68 封装尺寸 . . . . . . . . . 56 5.3.2 QFN80 封装尺寸 . . . . . . . . . 59 5.3.3 BGA112 封装尺寸 . . . . . . . . 62 5.4 分级回流焊 . . . . . . . . . . . . . . . . . 65 5.5 硬件最小系统 . . . . . . . . . . . . . . . . 66 5.5.1 QFN68 硬件最小系统 . . . . . . 67 5.5.2 QFN80 硬件最小系统 . . . . . . 69 5.5.3 BGA112 硬件最小系统 . . . . . . 71

6 订购信息 72

相关资源 74

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com vii/74

<!-- Image omitted -->


<!-- Page 9 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 插图

0-1 功能框图 . . . . . . . . . . . . . . . . . . ii

1-1 高性能系统总线架构 . . . . . . . . . . . 2 1-2 HPSYS 时钟架构 . . . . . . . . . . . . . . 3 1-3 电源管理架构 . . . . . . . . . . . . . . . . 7

3-1 通用异步收发器 . . . . . . . . . . . . . . 14 3-2 SSP 格式单次收发时序 . . . . . . . . . . 16 3-3 SSP 格式连续收发时序 . . . . . . . . . . 17 3-4 SPI 格式单次收发时序 . . . . . . . . . . 17 3-5 SPI 格式连续收发时序 . . . . . . . . . . 17 3-6 SPH=0 时的 SPI 时序 . . . . . . . . . . . 17 3-7 SPH=1 时的 SPI 时序 . . . . . . . . . . . 18 3-8 Microwire 格式的单次收发时序 . . . . . 18 3-9 Microwire 格式的多次收发时序 . . . . . 18 3-10 MPI 控制器框图 . . . . . . . . . . . . . . 24 3-11 寄存器模式单个和多个命令时序的序列 25

5-1 SF32LB57XUxxx6 QFN68 管脚分布图 . 39 5-2 SF32LB57XYxxx6 QFN80 管脚分布图 . 40 5-3 SF32LB57XVxxx6 BGA112 管脚分布图 . 41 5-4 QFN68-7x7-T0.9-P0.35 封装尺寸 . . . . 56 5-5 QFN68-7x7-T1.1-P0.35 封装尺寸 . . . . 56 5-6 QFN68-7x7 的 PCB 封装制作要求（第

一部分） . . . . . . . . . . . . . . . . . . 57 5-7 QFN68-7x7 的 PCB 封装制作要求（第 二部分） . . . . . . . . . . . . . . . . . . 58 5-8 QFN80-8x8-T0.9-P0.35 封装尺寸 . . . . 59 5-9 QFN80-8x8-T1.1-P0.35 封装尺寸 . . . . 59 5-10 QFN80-8x8 的 PCB 封装制作要求（第

一部分） . . . . . . . . . . . . . . . . . . 60 5-11 QFN80-8x8 的 PCB 封装制作要求（第 二部分） . . . . . . . . . . . . . . . . . . 61 5-12 BGA112-5.4x4.4-T0.98-P0.4 封装尺寸 . 62 5-13 BGA112 的 PCB 封装制作要求（第一 部分） . . . . . . . . . . . . . . . . . . . . 63

5-14 BGA112 的 PCB 封装制作要求（第二 部分） . . . . . . . . . . . . . . . . . . . . 64

5-15 分级回流焊 . . . . . . . . . . . . . . . . . 65 5-16 SF32LB57XUxxx6 常规 3.3V 供电最小 系统原理图 . . . . . . . . . . . . . . . . . 67

5-17 SF32LB57XYxxx6 常规 3.3V 供电最小 系统原理图 . . . . . . . . . . . . . . . . . 69

5-18 SF32LB57XVxxx6 常规 3.3V 供电最小 系统原理图 . . . . . . . . . . . . . . . . . 71 6-1 SF32LB57X ordering information scheme 72

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com viii/74

<!-- Image omitted -->


<!-- Page 10 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 表格

1-1 HPSYS 地址映射 . . . . . . . . . . . . . . 4 1-2 HCPU IRQ . . . . . . . . . . . . . . . . . . 6

3-1 常用接口速率 . . . . . . . . . . . . . . . . 26

4-1 运行条件 . . . . . . . . . . . . . . . . . . 27 4-2 绝对最大值 . . . . . . . . . . . . . . . . . 27 4-3 I/O 特性 @3.3V . . . . . . . . . . . . . . . 27 4-4 可靠性测试 . . . . . . . . . . . . . . . . . 28 4-5 处理器功耗 . . . . . . . . . . . . . . . . . 29 4-6 BT 和 BLE 功耗 . . . . . . . . . . . . . . 29 4-7 BLE 发射机性能—1Mbps 模式 . . . . . . 30 4-8 BLE 发射机性能—2Mbps 模式 . . . . . . 31 4-9 BLE 接收机性能—1Mbps 模式 . . . . . . 31 4-10 BLE 接收机性能—2Mbps 模式 . . . . . . 32 4-11 BLE 接收机性能—S2 模式 . . . . . . . . 33 4-12 BLE 接收机性能—S8 模式 . . . . . . . . 33 4-13 发射机性能—Basic Data Rate . . . . . . . 34 4-14 发射机性能—Enhanced Data Rate . . . . 35 4-15 接收机性能—Basic Data Rate . . . . . . . 35 4-16 接收机性能—Enhanced Data Rate-π/4 DQPSK . . . . . . . . . . . . . . . . . . . . 36 4-17 接收机性能—Enhanced Data Rate-8DPSK 36 4-18 音频 ADC 性能 . . . . . . . . . . . . . . . 36 4-19 音频 DAC 性能 . . . . . . . . . . . . . . . 37 4-20 IO 驱动能力 . . . . . . . . . . . . . . . . 37 4-21 12-bit GPADC 电气特性 . . . . . . . . . . 37

5-1 管脚类型 . . . . . . . . . . . . . . . . . . 42 5-2 大核域 GPIO（PA）管脚列表 . . . . . . 43 5-3 专用管脚（电源、射频、模拟、数字） 列表 . . . . . . . . . . . . . . . . . . . . . 50

5-4 专用管脚（电源、射频、模拟、数字） 列表 . . . . . . . . . . . . . . . . . . . . . 51

5-5 专用管脚（电源、射频、模拟、数字） 列表 . . . . . . . . . . . . . . . . . . . . . 52

5-6 Matrix Index Table . . . . . . . . . . . . . 54 5-7 LCDC mapping table . . . . . . . . . . . . 55 5-8 分级回流焊对照表 . . . . . . . . . . . . . 65 5-9 无铅工艺峰值回流温度对照表 . . . . . . 65 5-10 无铅工艺分级回流温度对照表 . . . . . . 66

6-1 订购信息 . . . . . . . . . . . . . . . . . . 73

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com ix/74

<!-- Image omitted -->


<!-- Page 11 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 1 芯片总览

### 1.1 系统架构

SF32LB57X 是一系列用于超低功耗人工智能物联网（AIoT）场景下的高集成度、高性能 MCU 芯片。芯片采用 了基于 Arm Cortex-M33 STAR-MC1 处理器的大小核架构，其中：

- 高性能系统/双性能核（HPSYS）：HCPU 配备 32KB 指令缓存（I-Cache）和 16KB 数据缓存（D-Cache），ACPU

配备 16KB 指令缓存（I-Cache）和 8KB 数据缓存（D-Cache），512KB SRAM（全部为 Retention SRAM）。 最高主频 240MHz，可动态切换基础工作模式和增强工作模式，高效访问片内和片外存储。作为系统主控， 增强工作模式主要用于系统控制、人机交互、高性能计算等；同时作为低功耗传感器中心，基础工作模式 可用于低功耗场景下的各种数据采集和处理。

- 低功耗系统/能效核（LPSYS）：最高主频 24MHz，80KB SRAM（全部为 Retention SRAM），用于低功耗蓝

牙的传输控制和基本数据处理。

### 1.2 Cortex-M33 STAR-MC1 “星辰”处理器

Cortex-M33 STAR-MC1 处理器是安谋中国（Arm China）推出的“星辰”系列产品的第一款处理器，该处理器继 承了 Cortex-M33 的主要特点，支持现有的 Arm v8-M 架构的全部功能，具有有序（in order）三级流水线，可显 著降低系统功耗，具有部分双发射 16 位指令能力，并进一步改进了协处理器接口，增加了对缓存（Cache）的 支持。

Cortex-M33 STAR-MC1 性能达到 1.5DMIPS/MHz 和 4.02Coremark/MHz，与上一代同档位 Arm 处理器相比，在 相同主频下，Cortex-M33 STAR-MC1 的性能提升 20%。

Cortex-M33 STAR-MC1 提供了协处理器（Coprocessor）接口，以便根据不同场景需求进一步提高定制计算的能 力。通过 MCR（Move from Coprocessor to Register）和 MRC（Move from Register to Coprocessor）指令，可以在

Cortex-M33 STAR-MC1 和协处理器之间转移寄存器数据和计算结果数据，非常适合所需数据量不大、计算复杂 但相对碎片化、延迟较小的运算。在协处理器计算的同时，Cortex-M33 STAR-MC1 处理器仍然可以并行执行其 它指令，从而明显提高执行效率。

此外，该处理器还支持数字信号处理（DSP）指令集和浮点数运算单元（FPU）。

Cortex-M33 STAR-MC1 引入了紧耦合内存（TCM）和缓存（Cache）技术，增强了各种不同特点的内置和外置存 储系统的使用灵活性，确保在各种不同场景下处理器响应的实时性和计算效率。

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 1/74

<!-- Image omitted -->


<!-- Page 12 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 1.3 高性能（双性能核）系统（HPSYS）

1.3.1 总线架构

HPSYS 内部提供了基于 AHB 协议的总线矩阵，支持多个主设备并行访问多个从设备地址空间。

如图1-1所示，总线主设备位于上侧，从设备地址空间位于右侧，交叉处的黑色圆点代表总线连通。

HCPU 与 DMAC1 能够访问 HPSYS 的所有地址空间。

DTCM 与 HPSYS\_RAM0 共享 128KB 地址空间，可由 HCPU 及其它主设备访问。

HP\_PERI 包含 APB 相关外设以及 AHB 相关外设，可由 HCPU、DMAC1 以及 PTC1 访问。

多个主设备同时访问同一个从设备地址空间时，基于轮询仲裁原则决定访问次序。

图中边框不相连的多个主设备同时访问不同从设备地址空间时，互相不受影响。边框相连的两个主设备同时发 起访问时，基于固定优先级或轮询仲裁原则决定访问次序。

HCPU\_C HCPU\_S

DMAC1 EPIC\_A

EPIC\_B LCDC USBC

MPI1

MPI2 HPSYS\_RAM0

HP\_PERI1 EZIP JPEGD AES

PTM1

HPSYS\_RAM1

HPSYS\_RAM2

HP\_PERI2

HP2LP LP2HP ACPU\_S

HCPU\_D

ACPU\_D S0 S1 S2 S3 S4 S5 S6 S7 S8 M0

M1

M2

M3

M4

M7 M5

M6

M8

M9

M10 MPI3

图 1-1: 高性能系统总线架构

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 2/74

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 13 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 1.3.2 时钟架构

HPAON

DLL1 clk\_hpsys

clk\_dll1 1/N 1/2N

DLL2

clk\_peri\_hpsys

CG clk\_dll2

I2C1/2/3/4 SPI1/2 USART1/2/3 EFUSEC CAN1/2 MPI1/2/3 CG

PLL clk\_aud\_pll I2S1/2 AUDCODEC AUDPRC PDM1/2 CG 48M CG

HCPU CG clk\_wdt

1/2N 1/2 vddaon\_clk clk\_lxt32 clk\_rtc

PMUC IWDT RTC LPTIM

CG

CG BTIM2 GPTIM2 PWM1 GPADC1 clk\_lrc10

clk\_lrc32

clk\_hrc48

clk\_hxt48 pclk2\_hpsys

pclk\_hpsys APB modules

AHB modules hclk\_hpsys

CG I2S1/2 AUDCODEC AUDPRC PDM1/2 ACPU

CG

CG LCDC CG

1/N USBC

USBC

### 图 1-2: HPSYS 时钟架构

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 3/74

<!-- Image omitted -->


<!-- Page 14 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 1.3.3 存储器类型

1.3.3.1 Cache

HCPU 配置有 32KB 2-way I-Cache（一级指令缓存）和 16KB 4-way D-Cache（一级数据缓存），可大幅提升 XIP 时 CPU 执行效率。软件需合理配置 MPU（Memory Protection Unit）设置 cache 地址段和非 cache 地址段，兼顾 效率和易用性。

1.3.3.2 TCM

HCPU 配置有 128KB zero-wait-cycle D-TCM，地址空间为 0x2000\_0000–0x2001\_FFFF，可用于放置对实时性要 求较高的代码和数据。该 TCM memory 挂在总线上，可以被其他 AHB master 访问。

1.3.3.3 SRAM

HPSYS 总线上共有 512KB SRAM，其中包括：

- 0x2000\_0000-0x2001\_FFFF，128KB zero-wait-cycle SRAM（与 D-TCM 共享），所有 AHB master 均可访问，

最高频率为 240MHz。

- 0x2002\_0000-0x2007\_FFFF，384KB zero-wait-cycle SRAM，所有 AHB master 均可访问，最高频率为 240MHz。

1.3.3.4 片外 RAM

HPSYS 支持合封四线和八线 pSRAM，地址空间为 0x6000\_0000–0x63FF\_FFFF(0x1000\_0000-0x13FF\_FFFF)，实 际可访问地址由外挂颗粒容量决定。接口最高频率为 DDR 156MHz，数据位宽为 8-bit。

1.3.3.5 片外 Flash

HPSYS 支持外挂 NOR/NAND FLASH，其中：

- 0x6000\_0000–0x63FF\_FFFF(0x1000\_0000-0x13FF\_FFFF) 地址段可挂合封 FLASH，推荐使用频率为 DTR

104MHz

- 0x6400\_0000–0x9FFF\_FFFF(0x1400\_0000-0x9FFF\_FFFF) 地址段可挂外置 FLASH，推荐使用频率为 SDR

104MHz

1.3.4 地址映射

表 1-1: HPSYS 地址映射

HCPU LCPU Category Memory /IP Address Space Starting Address Ending Address Starting Address Ending Address HPSYS\_ITCM 64KB 0x0000\_0000 0x0000\_FFFF NA NA ROM 64KB 0x0000\_0000 0x0000\_FFFF - - Reserved - - - - - External Memory 1024MB 0x1000\_0000 0x6000\_0000 0x1FFF\_FFFF 0x9FFF\_FFFF 0x6000\_0000 0x9FFF\_FFFF MPI1 Memory 32MB 0x1000\_0000 0x6000\_0000 0x11FF\_FFFF 0x61FF\_FFFF 0x6000\_0000 0x61FF\_FFFF MPI2 Memory 32MB 0x1200\_0000 0x6200\_0000 0x13FF\_FFFF 0x63FF\_FFFF 0x6200\_0000 0x63FF\_FFFF MPI3 Memory 192MB/960MB 0x1400\_0000 0x6400\_0000 0x1FFF\_FFFF 0x9FFF\_FFFF 0x6400\_0000 0x9FFF\_FFFF HPSYS\_RAM 512KB 0x2000\_0000 0x2007\_FFFF 0x2A00\_0000 0x2A07\_FFFF RAM0 (DTCM) 128KB 0x2000\_0000 0x2001\_FFFF 0x2A00\_0000 0x2A01\_FFFF RAM1 128KB 0x2002\_0000 0x2003\_FFFF 0x2A02\_0000 0x2A03\_FFFF RAM2 256KB 0x2004\_0000 0x2007\_FFFF 0x2A04\_0000 0x2A07\_FFFF HPSYS\_APB1 256KB 0x5000\_0000 0x5003\_FFFF 0x5000\_0000 0x5003\_FFFF 续表下页...

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 4/74

<!-- Image omitted -->


<!-- Page 15 -->

# SF32LB57X

# 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

# 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 表 1-1: HPSYS 地址映射（续）

HCPU LCPU Category Memory /IP Address Space Starting Address Ending Address Starting Address Ending Address

RCC1 4KB 0x5000\_0000 0x5000\_0FFF 0x5000\_0000 0x5000\_0FFF

JPEGD 4KB 0x5000\_1000 0x5000\_1FFF 0x5000\_1000 0x5000\_1FFF

SECU1 4KB 0x5000\_2000 0x5000\_2FFF 0x5000\_2000 0x5000\_2FFF

PINMUX1 4KB 0x5000\_3000 0x5000\_3FFF 0x5000\_3000 0x5000\_3FFF

ATIM1 4KB 0x5000\_4000 0x5000\_4FFF 0x5000\_4000 0x5000\_4FFF

ATIM2 4KB 0x5000\_5000 0x5000\_5FFF 0x5000\_5000 0x5000\_5FFF

EZIP1 4KB 0x5000\_6000 0x5000\_6FFF 0x5000\_6000 0x5000\_6FFF

EPIC 4KB 0x5000\_7000 0x5000\_7FFF 0x5000\_7000 0x5000\_7FFF

LCDC1 4KB 0x5000\_8000 0x5000\_8FFF 0x5000\_8000 0x5000\_8FFF

I2S1 4KB 0x5000\_9000 0x5000\_9FFF 0x5000\_9000 0x5000\_9FFF

CAN1 4KB 0x5000\_A000 0x5000\_AFFF 0x5000\_A000 0x5000\_AFFF

SYSCFG1 4KB 0x5000\_B000 0x5000\_BFFF 0x5000\_B000 0x5000\_BFFF

EFUSEC 4KB 0x5000\_C000 0x5000\_CFFF 0x5000\_C000 0x5000\_CFFF

AES 4KB 0x5000\_D000 0x5000\_DFFF 0x5000\_D000 0x5000\_DFFF

CAN2 4KB 0x5000\_E000 0x5000\_EFFF 0x5000\_E000 0x5000\_EFFF

TRNG 4KB 0x5000\_F000 0x5000\_FFFF 0x5000\_F000 0x5000\_FFFF

Reserved 192KB 0x5002\_0000 0x5001\_0000 0x5001\_0000 0x5003\_FFFF

HPSYS\_AHB1 256KB 0x5004\_0000 0x5007\_FFFF 0x5004\_0000 0x5007\_FFFF

DCMI 4KB 0x5004\_0000 0x5004\_0FFF 0x5004\_0000 0x5004\_0FFF

MPI1 4KB 0x5004\_1000 0x5004\_1FFF 0x5004\_1000 0x5004\_1FFF

MPI2 4KB 0x5004\_2000 0x5004\_2FFF 0x5004\_2000 0x5004\_2FFF

MPI3 4KB 0x5004\_3000 0x5004\_3FFF 0x5004\_3000 0x5004\_3FFF

Reserved 4KB 0x5004\_4000 0x5004\_4FFF 0x5004\_4000 0x5004\_4FFF

SDMMC1 4KB 0x5004\_5000 0x5004\_5FFF 0x5004\_5000 0x5004\_5FFF

SDMMC1 4KB 0x5004\_6000 0x5004\_6FFF 0x5004\_6000 0x5004\_6FFF

USBC 4KB 0x5004\_7000 0x5004\_7FFF 0x5004\_7000 0x5004\_7FFF

CRC1 4KB 0x5004\_8000 0x5004\_8FFF 0x5004\_8000 0x5004\_8FFF

Reserved 28KB 0x5004\_9000 0x5004\_FFFF 0x5004\_9000 0x5004\_FFFF

Reserved 192KB 0x5005\_0000 0x5007\_FFFF 0x5005\_0000 0x5007\_FFFF

HPSYS\_APB2 128KB 0x5008\_0000 0x5008\_FFFF 0x5008\_0000 0x5008\_FFFF

Reserved 4KB 0x5008\_0000 0x5008\_0FFF 0x5008\_0000 0x5008\_0FFF

DMAC1 4KB 0x5008\_1000 0x5008\_1FFF 0x5008\_1000 0x5008\_1FFF

MAILBOX1 4KB 0x5008\_2000 0x5008\_2FFF 0x5008\_2000 0x5008\_2FFF

Reserved 4KB 0x5008\_3000 0x5008\_3FFF 0x5008\_3000 0x5008\_3FFF

USART1 4KB 0x5008\_4000 0x5008\_4FFF 0x5008\_4000 0x5008\_4FFF

USART2 4KB 0x5008\_5000 0x5008\_5FFF 0x5008\_5000 0x5008\_5FFF

USART3 4KB 0x5008\_6000 0x5008\_6FFF 0x5008\_6000 0x5008\_6FFF

AUDPRC 4KB 0x5008\_7000 0x5008\_7FFF 0x5008\_7000 0x5008\_7FFF

AUDCODEC 4KB 0x5008\_8000 0x5008\_8FFF 0x5008\_8000 0x5008\_8FFF

TSEN 4KB 0x5008\_9000 0x5008\_9FFF 0x5008\_9000 0x5008\_9FFF

I2S2 4KB 0x5008\_A000 0x5008\_AFFF 0x5008\_A000 0x5008\_AFFF

Reserved 20KB 0x5008\_B000 0x5008\_FFFF 0x5008\_B000 0x5008\_FFFF

HPSYS\_APB3 64KB 0x5009\_0000 0x5009\_FFFF 0x5009\_0000 0x5009\_FFFF

GPTIM1 4KB 0x5009\_0000 0x5009\_0FFF 0x5009\_0000 0x5009\_0FFF

Reserved 4KB 0x5009\_1000 0x5009\_1FFF 0x5009\_1000 0x5009\_1FFF

BTIM1 4KB 0x5009\_2000 0x5009\_2FFF 0x5009\_2000 0x5009\_2FFF

Reserved 4KB 0x5009\_3000 0x5009\_3FFF 0x5009\_3000 0x5009\_3FFF

WDT1 4KB 0x5009\_4000 0x5009\_4FFF 0x5009\_4000 0x5009\_4FFF

SPI1 4KB 0x5009\_5000 0x5009\_5FFF 0x5009\_5000 0x5009\_5FFF

SPI2 4KB 0x5009\_6000 0x5009\_6FFF 0x5009\_6000 0x5009\_6FFF

Reserved 4KB 0x5009\_7000 0x5009\_7FFF 0x5009\_7000 0x5009\_7FFF

Reserved 4KB 0x5009\_8000 0x5009\_8FFF 0x5009\_8000 0x5009\_8FFF

Reserved 4KB 0x5009\_9000 0x5009\_9FFF 0x5009\_9000 0x5009\_9FFF

PDM1 4KB 0x5009\_A000 0x5009\_AFFF 0x5009\_A000 0x5009\_AFFF

PDM2 4KB 0x5009\_B000 0x5009\_BFFF 0x5009\_B000 0x5009\_BFFF

I2C1 4KB 0x5009\_C000 0x5009\_CFFF 0x5009\_C000 0x5009\_CFFF

I2C2 4KB 0x5009\_D000 0x5009\_DFFF 0x5009\_D000 0x5009\_DFFF

I2C3 4KB 0x5009\_E000 0x5009\_EFFF 0x5009\_E000 0x5009\_EFFF

I2C4 4KB 0x5009\_F000 0x5009\_FFFF 0x5009\_F000 0x5009\_FFFF

HPSYS\_AHB2 64KB 0x500A\_0000 0x500A\_FFFF 0x500A\_0000 0x500A\_FFFF

GPIO1 4KB 0x500A\_0000 0x500A\_0FFF 0x500A\_0000 0x500A\_0FFF

PTM1 4KB 0x500A\_1000 0x500A\_1FFF 0x500A\_1000 0x500A\_1FFF

Reserved 56KB 0x500A\_2000 0x500A\_FFFF 0x500A\_2000 0x500A\_FFFF

HPSYS\_APB4 64KB 0x500B\_0000 0x500B\_FFFF 0x500B\_0000 0x500B\_FFFF

GPTIM2 4KB 0x500B\_0000 0x500B\_0FFF 0x500B\_0000 0x500B\_0FFF

BTIM2 4KB 0x500B\_1000 0x500B\_1FFF 0x500B\_1000 0x500B\_1FFF

GPADC 4KB 0x500B\_2000 0x500B\_2FFF 0x500B\_2000 0x500B\_2FFF

PWM1 4KB 0x500B\_3000 0x500B\_3FFF 0x500B\_3000 0x500B\_3FFF

Reserved 48KB 0x500B\_4000 0x500B\_FFFF 0x500B\_4000 0x500B\_FFFF

HPSYS\_APB5 256KB 0x500C\_0000 0x500F\_FFFF 0x500C\_0000 0x500F\_FFFF

HPSYS\_AON 4KB 0x500C\_0000 0x500C\_0FFF 0x500C\_0000 0x500C\_0FFF

LPTIM1 4KB 0x500C\_1000 0x500C\_1FFF 0x500C\_1000 0x500C\_1FFF

LPTIM2 4KB 0x500C\_2000 0x500C\_2FFF 0x500C\_2000 0x500C\_2FFF

Reserved 4KB 0x500C\_3000 0x500C\_3FFF 0x500C\_3000 0x500C\_3FFF

Reserved 24KB 0x500C\_4000 0x500C\_9FFF 0x500C\_4000 0x500C\_9FFF

PMUC 4KB 0x500C\_A000 0x500C\_AFFF 0x500C\_A000 0x500C\_AFFF

RTC 4KB 0x500C\_B000 0x500C\_BFFF 0x500C\_B000 0x500C\_BFFF

IWDT 4KB 0x500C\_C000 0x500C\_CFFF 0x500C\_C000 0x500C\_CFFF

Reserved 12KB 0x500C\_D000 0x500C\_FFFF 0x500C\_D000 0x500C\_FFFF 续表下页...

# DS5702‐SF32LB57X‐CN V0.2.2

## ©2026 思澈科技（南京）有限公司 www.sifli.com

5/74

<!-- Image omitted -->


<!-- Page 16 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 1-1: HPSYS 地址映射（续） HCPU LCPU Category Memory /IP Address Space Starting Address Ending Address Starting Address Ending Address Reserved 64KB 0x500D\_0000 0x500D\_FFFF 0x500D\_0000 0x500D\_FFFF Reserved 64KB 0x500E\_0000 0x500E\_FFFF 0x500E\_0000 0x500E\_FFFF EUROPA 4KB 0x500F\_0000 0x500F\_0FFF 0x500F\_0000 0x500F\_0FFF Reserved 60KB 0x500F\_1000 0x500F\_FFFF 0x500F\_1000 0x500F\_FFFF

### 1.3.5 中断列表

表 1-2: HCPU IRQ

NMI WDT1 IRQ\[26\] rsvd IRQ\[53\] DMAC1\_CH4 IRQ\[80\] MAILBOX2\_CH2

IRQ\[0\] AON IRQ\[27\] rsvd IRQ\[54\] DMAC1\_CH5 IRQ\[81\] SDMMC2

IRQ\[1\] LCPU\_IRQ\[1\] IRQ\[28\] rsvd IRQ\[55\] DMAC1\_CH6 IRQ\[82\] PDM1

IRQ\[2\] LCPU\_IRQ\[2\] IRQ\[29\] rsvd IRQ\[56\] DMAC1\_CH7 IRQ\[83\] CAN1

IRQ\[3\] LCPU\_IRQ\[3\] IRQ\[30\] rsvd IRQ\[57\] DMAC1\_CH8 IRQ\[84\] GPIO1

IRQ\[4\] LCPU\_IRQ\[4\] IRQ\[31\] rsvd IRQ\[58\] MAILBOX2\_CH1 IRQ\[85\] MPI1

IRQ\[5\] LCPU\_IRQ\[5\] IRQ\[32\] PTM1\_CORE0\_IRQ IRQ\[59\] USART1 IRQ\[86\] MPI2

IRQ\[6\] LCPU\_IRQ\[6\] IRQ\[33\] PTM1\_CORE1\_IRQ IRQ\[60\] SPI1 IRQ\[87\] MPI3

IRQ\[7\] LCPU\_IRQ\[7\] IRQ\[34\] PTM1\_CORE2\_IRQ IRQ\[61\] I2C1 IRQ\[88\] PDM2

IRQ\[8\] LCPU\_IRQ\[8\] IRQ\[35\] PTM1\_CORE3\_IRQ IRQ\[62\] EPIC IRQ\[89\] EZIP1

IRQ\[9\] LCPU\_IRQ\[9\] IRQ\[36\] PTM1\_CORE4\_IRQ IRQ\[63\] LCDC1 IRQ\[90\] AUDPRC

IRQ\[10\] LCPU\_IRQ\[10\] IRQ\[37\] PTM1\_CORE5\_IRQ IRQ\[64\] I2S1 IRQ\[91\] TSEN

IRQ\[11\] LCPU\_IRQ\[11\] IRQ\[38\] PTM1\_CORE6\_IRQ IRQ\[65\] GPADC IRQ\[92\] USBC

IRQ\[12\] LCPU\_IRQ\[12\] IRQ\[39\] PTM1\_CORE7\_IRQ IRQ\[66\] EFUSEC IRQ\[93\] I2C3

IRQ\[13\] LCPU\_IRQ\[13\] IRQ\[40\] rsvd IRQ\[67\] AES IRQ\[94\] ATIM1

IRQ\[14\] LCPU\_IRQ\[14\] IRQ\[41\] rsvd IRQ\[68\] rsvd IRQ\[95\] USART3

IRQ\[15\] LCPU\_IRQ\[15\] IRQ\[42\] rsvd IRQ\[69\] TRNG IRQ\[96\] AUD\_HP

IRQ\[16\] LCPU\_IRQ\[16\] IRQ\[43\] rsvd IRQ\[70\] GPTIM1 IRQ\[97\] DCMI

IRQ\[17\] LCPU\_IRQ\[17\] IRQ\[44\] rsvd IRQ\[71\] GPTIM2 IRQ\[98\] SECU1

IRQ\[18\] LCPU\_IRQ\[18\] IRQ\[45\] rsvd IRQ\[72\] BTIM1 IRQ\[99\] JPEGD

IRQ\[19\] LCPU\_IRQ\[19\] IRQ\[46\] LPTIM1 IRQ\[73\] BTIM2 IRQ\[100\] CAN2

IRQ\[20\] LCPU\_IRQ\[20\] IRQ\[47\] LPTIM2 IRQ\[74\] USART2 IRQ\[101\] ATIM2

IRQ\[21\] rsvd IRQ\[48\] PMUC IRQ\[75\] SPI2 IRQ\[102\] rsvd

IRQ\[22\] rsvd IRQ\[49\] RTC IRQ\[76\] I2C2 IRQ\[103\] I2S2

IRQ\[23\] rsvd IRQ\[50\] DMAC1\_CH1 IRQ\[77\] rsvd IRQ\[104\] rsvd

IRQ\[24\] rsvd IRQ\[51\] DMAC1\_CH2 IRQ\[78\] I2C4 IRQ\[105\] PWM1

IRQ\[25\] rsvd IRQ\[52\] DMAC1\_CH3 IRQ\[79\] SDMMC1

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 6/74

<!-- Image omitted -->


<!-- Page 17 -->

# SF32LB57X

## 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

## 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 1.4 电源管理

RF LDO RF Circuitry RTC LDO

HPSYS LDO RC10K XTAL32K RTC/IWDT

RET LDO LCPU

Ret SRAM Peripherals LPSYS HCPU

Off SRAM

Ret SRAM Peripherals HPSYS PVDD

VDD\_VOUT1 BUCK

BUCK\_FB

VDD\_RET VDD\_RTC

PHY/MAC PMUC AON

LPSYS LDO

VDD\_VOUT2 GPU

AVDD\_BRF

GPADC/TSEN GPADC\_VREF

AVDD33\_AUD

AUDIO AUD\_VREF

MIC\_BIAS VDD18\_LDO

VDD18\_VOUT VBATS

VDDIOA

### 图 1-3: 电源管理架构

## DS5702‐SF32LB57X‐CN V0.2.2

### ©2026 思澈科技（南京）有限公司 www.sifli.com

7/74

<!-- Image omitted -->


<!-- Page 18 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 2 高性能专用计算

### 2.1 ePicassoTM 高性能 2.5D 图形引擎

在 2.5D 图像处理中，有许多常见的图像运算会耗费大量的 CPU 计算资源。ePicassoTM 则是专为 2.5D 图像运算

设计的加速引擎，能够对 2.5D 图像运算中常见图层叠加、缩放、旋转、投影，镜像等功能提供指数级的速度提

升。除此以外，ePicassoTM 能够兼容各种常见的图像格式，简化了系统中不同格式的图像格式转换。

2.1.1 图层叠加

ePicassoTM 最多支持三个前景图层，分别为普通图层，功能图层和投射图层，一个专用的掩膜图层，和一个单色 背景图层叠加，输入格式包括常用 RGB565、RGB888、ARGB8565、ARGB8888 还包含 L8、A8、A4、A2、YUV 等。每个前景图层有独立的叠加模式和叠加区域，掩膜图层主要是提取图像中特定的形状。除此以外，每个图

层还提供了单独的 filter 配置选项，可以使图层滤除某一特定的颜色，该功能可用于简单的图像捕获。

2.1.2 普通图层

ePicassoTM 的前景图层中有一个为普通图层，该图层支持多种模式的色彩混叠，独立的混叠区域，自定义色彩滤 除，垂直镜像等功能。

2.1.3 功能图层

ePicassoTM 的前景图层中有一个为功能图层，该图层相比普通图层增加了高精度缩放，高精度旋转和水平镜像的 功能。缩放比例从 1/1024~1024，精度可以达到 1/65536。高精度旋转则通过用户自定义旋转角的 sin/cos 值来 满足任意角度的旋转需求。旋转和缩放的功能可以同时启用，一次性完成图像的两种操作，提高了图像处理的 性能。

2.1.4 投射图层

ePicassoTM 的前景图层中有一个为投射图层，该图层通过矩阵变换的方式，实现所有 2.5D 场景下的投影变换运 算。

### 2.2 LCD 控制器

LCD 控制器主要用于将 Framebuffer 内数据输出至外部显示，现有的 LCD 控制器可以支持常用的屏幕接口包括

DBI、DPI。此外，LCD 控制器还支持压缩格式的图像，使用压缩格式的图像可以显著降低 memory 的使用带宽， 提升系统的性能。

2.2.1 显示接口

LCD 控制器主要完成显示用数据到主流显示接口之间的适配，本芯片支持的显示接口有：

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 8/74

<!-- Image omitted -->


<!-- Page 19 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

2.2.1.1 MIPI-DBI

LCD 控制器可以支持 DBI 接口中的串行 SPI 模式和并行的 8080 模式。对于 SPI 模式，LCD 控制器可以支持 3 线 和 4 线两种模式，每一种模式都可以支持单/双/四数据线的传输方式，除此以外，LCD 控制器也支持 DDR 的串

行 SPI 模式，相比常见的 SDR SPI 模式，传输带宽可以提升一倍。色彩格式上支持 8-bit RGB332、16-bit RGB565

和 24-bit RGB888。对于 8080 模式，LCD 控制器可以支持 8-bit、16-bit 和 24-bit 的总线位宽，同时支持 RGB332、

RGB444、RGB565、RGB666、RGB888 等色彩格式。

2.2.1.2 MIPI-DPI

为了支持没有内置缓存的屏幕，LCD 控制器加入了对 DPI 接口的支持。LCD 控制的 DPI 接口数据位宽为 24bit，

可以很好地支持 RGB565 和 RGB888 的色彩格式。此外，LCD 控制器提供了灵活的 VBP、VFP、HBP 和 HFP 控 制，来应对不同类型屏幕的需求。

2.2.1.3 JDI 反射屏

为了适应可穿戴产品的低功耗需求，JDI 研发超低功耗的反射屏。该屏幕利用太阳光线呈现图像，相较传统 LCD

屏幕耗电可以降低 95% 以上，配备在可穿戴产品上可以实现超长续航。LCD 控制器内也加入了 JDI 反射屏幕接

口支持，包括串行接口和并行接口。两种接口最高可以支持到 64 色显示，支持局部刷新和整屏刷新，从而进一 步降低屏幕刷新功耗，满足超长续航的需求。

2.2.1.4 EPD 电子纸屏幕

为了支持电子纸类屏幕，LCD 控制器内部集成了电子纸屏幕驱动器，能够支持 8-bit 和 16-bit 的并口电子纸屏

幕，相比 SPI 电子纸屏幕，并口能够实现更高的传输带宽，更快的刷新速率和更好的显示效果。

### 2.3 eZipTM 通用压缩解码器

eZipTM 解码器是通用的解压缩模块，对于数据方面，eZip 支持基于自有算法的无损格式，其压缩率与 Zip 格式相 当。它可以用于将通用数据解码后保存，以此加快数据的实时加载能力。如果数据是从芯片外部传输，压缩后 的传输有助于缩短传输时间，减少传输功耗。

对于图像方面，eZipTM 可以支持专有无损格式的图片和通用有损 JPEG 格式图片，无损格式压缩率与 PNG 格式

相当，并支持独立 DMA 操作或与 ePicassoTM 联动读取。当独立操作时，eZipTM 可通过 DMA 机制，可以灵活地将

存储在 Flash 或 RAM 的压缩图片解压缩并搬运至目标缓存中。在联动模式下，ePicassoTM 通过 eZipTM 模块，实时

从存储中读取图片并实时解压缩，然后按照一般的图形流程进行所需要的 2.5D 计算，从而省去了暂存解压缩图 片的缓存。

通过以上机制，eZipTM 可以有效地降低图像素材对存储容量的需求，在有限的存储中最大化素材的丰富度，减小 对片外存储的带宽要求，从而提高大大系统的整体运行效率。

eZipTM 模块是将 eZipTM 压缩图片进行解码输出的模块。该模块通过 AHB 总线读入压缩数据，解码后的图像数据 可配置通过 AHB 总线输出或直接送给 epic 模块进行后续处理。

该模块具有以下特点：

- 通过 AHB 总线输入\\输出的数据地址可配

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 9/74

<!-- Image omitted -->


<!-- Page 20 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

- 输出图片数据可直接送给 epic 模块

- 可输出一个指定区域的图片数据

- 支持解码参数 cache 功能，cache 命中的情况下可缩短解码时间

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 10/74

<!-- Image omitted -->


<!-- Page 21 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 3 外设

### 3.1 双模蓝牙 6.3

3.1.1 射频和基带

射频和基带包括发射机和接收机。发射机将基带信号调制到 2.4G 频段上的信号发射出去，接收机将 2.4G 频段

空中信号接收解调到基带信号。主要特性为：

- 支持蓝牙 6.3 协议：1M PHY（1Mbps）、2M PHY（2Mbps）、Long Range、BR PHY（1Mbps）、EDR2 PHY

（2Mbps）、EDR3 PHY（3Mbps）

- 集成 AGC

- 支持 RSSI

- 接收机支持自动频偏纠正

- 可调发射功率，BLE/BR PHY 最大发射功率 17dBm，EDR2/EDR3 最大发射功率 13dBm

- 集成 Balun 和天线匹配网络，无需片外匹配

3.1.2 BT MAC

BT MAC 是完全支持蓝牙协议 v6.3 并向下兼容协议 v4.2、v4.1、4.0 的双模基带控制器，主要是负责包的编解码 以及事件的调度。

主要的功能如下：

- 低功耗蓝牙模式：

1. 支持所有速率（1M/2M/Long Range）；

2. 支持所有的包格式（广播包/拓展广播包/数据包等）；

3. 支持数据的加解密；

4. 支持数据流的处理（冗余校验、白化）；

5. 支持两种跳频模式；

- 经典蓝牙模式：

1. 支持 ACL、CSB、SCO 以及 eSCO 的所有包类型；

2. 支持数据加解密（E0 加密以及 AES-CCM 加密）；

3. 支持数据流的处理（HEC、CRC、Whitening、FEC2/3、FEC1/3）；

4. 支持音频数据的编解码处理（CVSD 以及 a/µ-Law）；

5. 支持自适应跳频；

- 以及：

– 支持 AMBA AHB 总线的访问；

– 支持 WLAN/MWS 的共存机制。

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 11/74

<!-- Image omitted -->


<!-- Page 22 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 3.2 模拟外设

3.2.1 12 比特模/数转换器

GPADC 是一个 12bit 精度 SARADC，支持 0-AVDD 输入电压，输出为 12bit 数据。输入电压可为单端或差分，输 出数据可通过 APB 总线或 DMA 接口读取。

- 支持 3.3V±10% AVDD

- 输入电压范围：0~AVDD，12Bit 分辨率

- 支持单端输入和差分输入

- 支持 11 路单端模拟输入和 1 路专用电池电压测量输入或 5 对差分模拟输入

- 支持单次测量模式和循环测量模式

- 循环测量模式下支持过采样滤波

- 每次测量可以划分为 12 个时隙，各时隙可以单独配置模拟输入通道

- 支持软件（写寄存器）和硬件（如计时器）触发方式

- 支持 DMA 通道

- 采样频率可配，最高采样频率 3.7MHz

- 转换完成后产生中断

3.2.2 音频 DAC

音频 DAC 是将数字音频信号转化为模拟电压输出的模块，本芯片共集成了一路 24-bit DAC，支持音频采样率从

8KHz 到 96KHz，输出方式支持差分输出。

3.2.3 音频 PLL

音频 PLL 主要功能为音频系统提供高精度的时钟，其支持小数分频功能，调节精度为 48MHz/2^18，能够满足

48KHz、32KHz、44.1KHz、96KHz 和 88.2KHz 等不同采样率的需求。

3.2.4 音频 ADC

音频 ADC 是将外部模拟信号转化为内部的数字音频信号，本芯片共集成了两路 24-bit ADC，支持的音频采样率

从 8KHz 到 96KHz，每一路 ADC 有单独的增益调节。

### 3.3 GPDMA

3.3.1 GPDMA

GPDMA(General Purpose Direct Memory Access Controller) 用于实现总线上两个不同地址区间内数据的搬运工作。 GPDMA 共有 8 个独立通道，每个通道可配置源地址区间与目标地址区间，分别映射到各存储器或外设的地址范 围内，从而实现存储器-存储器，存储器-外设，外设-存储器，外设-外设之间的高效率传输，有效缓解 CPU 的工 作量。GPDMA 的传输支持外设响应模式和存储器搬运模式：在外设响应模式下，GPDMA 基于外设的 DMA 请

求进行搬运，从而适配外设的带宽；在存储器搬运模式下，GPDMA 不等待外设的 DMA 请求，尽快完成数据搬

运。当多个通道同时使能时，GPDMA 依照优先级由高至低的次序依次搬运；并且在较低优先级的通道搬运过程

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 12/74

<!-- Image omitted -->


<!-- Page 23 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

中，较高优先级的通道能够进行抢占搬运。每个通道传输过半或完成时，能够产生中断或 PTC 触发。每个通道

可设置独立触发源，仅当触发条件满足时才启动传输。每个通道内置 FIFO 用于缓存传输数据，支持数据位宽转 换与字节交换。GPDMA 的工作模式支持基础模式，循环模式，重复模式，链表模式。基础模式可完成单次搬运

工作。循环模式可无限循环进行搬运。重复模式可进行特定次数的搬运，并支持每次搬运后起始地址自增。链表 模式下能够自动执行多个串联任务，其中每个任务能够独立设置地址、模式、数据量、延时、触发条件等参数。

DMAC 主要特性：

- 单 AHB 主控总线，可访问 SRAM，PSRAM，FLASH，AHB 和 APB 外设等，支持 BURST 传输

- 8 个独立的可配置通道

- 支持外设到存储器、存储器到外设、存储器到存储器以及外设到外设的数据传输

- 每个通道的 DMA 请求可在至多 128 个外设 DMA 请求中选择 1 个，或由软件请求

- 每个通道支持 4 档优先级配置，优先级相同时依照通道编号大小判决

- 源地址和目标地址均独立支持单字节、双字节、四字节访问。源和目标的地址必须根据传输数据单元的大

小进行对齐，并支持地址自动递增

- 单次传输单元数可配置为 1 到 1048575

- 支持循环缓冲模式，单次传输完成后自动重新启动

- 每个通道支持 4 种事件标志：传输完成、过半传输、传输错误、链表/重复完成，并能独立生成中断请求及

PTM 事件触发

- 每个通道独立选择触发源，仅当触发事件发生后才启动传输

- 通道 1~2 支持重复模式，可进行特定次数的搬运，并支持每次搬运后起始地址自增

- 所有通道支持链表模式，可自动执行多个串联任务，每个任务独立设置地址、模式、数据量、延时、触发

条件

- 通道 1~2 内置 64-byte FIFO，通道 3~10 内置 8-byte FIFO，用于暂存待传输数据

- 支持数据位宽转换，包括截取/补零/符号扩展/打包/拆包等转换方式

- 支持数据字节交换

### 3.4 音频处理模块 AUDPRC

音频处理模块针对不同来源的音频数据进行采样率转换、混音以及均衡，将处理后的音频数据送到对应的播放 或者存储设备。其主要包含两条主要的数据通路，处理播放数据的 DAC 通路和处理音频采集数据的 ADC 通路。

3.4.1 DAC 通路

DAC 通路音频数据来源于内存，AUDPRC 最多支持四路 24bit 音频数据。AUDPRC 内 DAC 通路支持对两路数据 进行采样率转换，采样率转换范围为 1/8~8 倍，信噪比不低于 96dB。经过采样率转换模块的数据再与另外两路

数据进行混音处理后，进入 16 级的音频均衡器。每一级均衡器参数均可配，用户可以根据需求进行配置。最后

经过均衡器的两路音频数据可以送至模拟 DAC 模块或者 I2S 接口作为输出。

3.4.2 ADC 通路

ADC 通路音频数据来源于模拟 ADC 或者 I2S 接口，最多支持两路 24bit 音频数据。经过采样率转换模块的数据 可以被 DMA 保存到内存中。

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 13/74

<!-- Image omitted -->


<!-- Page 24 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 3.5 连接外设

3.5.1 通用输入/输出接口（GPIO）

通过配置对应的寄存器，可以为这些管脚分配不同的功能。

当配置为输出功能时，可以通过寄存器配置输出值。

当配置为输入功能时，输入值可以通过相应寄存器查询，同时支持输入信号中断触发，中断触发模式可设置为 电平触发和沿触发，沿触发支持上下双沿触发。

3.5.2 通用异步收发器（UART）

通用异步收发器支持全双工模式，提供高达 6Mbps 的波特率和多种可配置的数据格式，为与外部标准化设备通 信提供了灵活而有效的数据交互手段。同时它还支持 DMA，实现多包收发。

UART1、UART2 和 UART3 位于 HPSYS

图 3-1: 通用异步收发器

通用异步收发器主要特性：

- 全双工异步通信

- 可配置 16 倍过采样或 8 倍过采样，选择频率优先或者时钟容忍度优先

- 灵活波特率配置，当输入时钟为 48MHz 且过采样率为 16 时，波特率为 3Mbps

- 可配置包长度（7/8/9 bits）

- 可配置停止位（1/2 bits）

- 硬件流控（CTS/RTS）

- DMA 多包发送和接收

- 接收奇偶校验和发送奇偶生成

- 接收和发送中断，以及其他错误中断

波特率计算说明

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 14/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 25 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

假设输入时钟固定为 48MHz，波特率计算公式如下：

Baud Rate = 48MHz (BRRINT + BRRF RAC 16 )(16 or 8) (3.1)

3.5.3 I2C

I2C(Inter-Integrated Circuit) 接口同时支持主设备与从设备角色，可作为主设备与 I2C 外设通信，也可作为从设备 响应外部的 I2C 主设备。I2C 内置 8 字节 FIFO，可以进行单笔读写，也可通过 DMA 进行批量数据读写。I2C 支持标

准模式 (standard-mode)、快速模式 (fast-mode)、增强快速模式 (fast-mode plus) 以及高速模式 (high-speed-mode)，

最高速率可达到 3.4Mbps。

I2C 做主设备时，支持自动任务 (autonomous task) 模式，能够在配置后自动完成读写从设备的完整操作。

I2C 主要特性：

- 可同时作为主设备与从设备

- 支持总线多主设备

- 支持标准模式 (最高 100kbps)

- 支持快速模式 (最高 400kbps)

- 支持快速模式 + (最高 1Mbps)

- 支持高速模式 (最高 3.4Mbps)

- 作为主设备支持访问 7 比特或 10 比特寻址

- 作为从设备支持 7 比特寻址

- 可配置的总线时序

- 支持时钟延展 (clock stretching)

- 8 字节 FIFO，支持 DMA

- 可配置的数字防抖动电路

- 独立的功能时钟，支持系统时钟动态调节

- 自动任务模式 (AT)

3.5.4 PDM

PDM (Pulse Density Modulation) 脉冲密度调制接口主要是用于将 PDM 麦克风采集到的 PDM 音频信号转化为 PCM（Pulse Code Modulation）脉冲编码调制信号以供后续的音频处理。

PDM 主要特性：

- 同时支持左右两路立体声信号，也可以单独采集单声道信号

- 可提供的 PDM 麦克风时钟速率：3.072MHz、1.536MHz、0.768MHz、1.024MHz、2.4MHz、1.6MHz、0.8MHz

- 支持 PCM 数据的速率：96kHz、48kHz、32kHz、24kHz、16kHz、12kHz、8kHz

- 支持 32bit、24bit、16bit、8bit 的 PCM 信号

- 支持分辨率为 0.5dB 并且从 -15dB 到 45dB 增益可调

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 15/74

<!-- Image omitted -->


<!-- Page 26 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

3.5.5 I2S

I2S 接口是应用于音频的输入输出，可以用来连接外部音频芯片、数字麦克风等设备。相较模拟音频接口，I2S 数字音频接口有更好的抗干扰能力以及更精简的接口协议。

I2S 主要特性：

- 支持 master 和 slave 两种模式

- 支持全双工模式

- 支持 TDM 模式

- 可配置的 I2S 数据格式，包括左对齐、右对齐和标准格式

- 支持多种音频数据格式，包括 8-bit 和 16-bit 的单声道和立体声格式

- 可配置的 I2S PCM 信号位宽，最高到 24-bit

### 3.5.6 串行外设接口（SPI）

SPI 支持 4 种通信格式：SSP/SPI/Microwire 以及自定义的帧格式。SSP/SPI 为全双工通信协议，控制器可以配置为

Master 或 Slave 模式。Microwire 为半双工通信协议，控制器仅可配置为 Master 模式。SPI 控制器内置发送/接收

FIFO。发送 FIFO 和接收 FIFO 共享同一个地址，读该地址时访问接收 FIFO，写该地址时访问发送 FIFO。FIFO 支持软件访问模式和 DMA 访问模式。

SPI 特性如下：

- 支持 4 种通信格式：SSP/SPI/Microwire/自定义帧格式

- 支持 4 到 32Bit 的数据宽度

- SPI 格式下时钟极性和相位可通过寄存器 SPO 和 SPH 设置

- 片选信号极性可配

- FIFO 深度为 32Bits×16Entry

- 接收发送都支持 DMA 模式

- HPSYS 中的 SPI 最大时钟频率为 48MHz

各种通信格式工作时序如下：

图 3-2: SSP 格式单次收发时序

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 16/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 27 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 3-3: SSP 格式连续收发时序

图 3-4: SPI 格式单次收发时序

图 3-5: SPI 格式连续收发时序

下面图例用于说明 SPI 格式下 SPH/SPO 设置的效果：

图 3-6: SPH=0 时的 SPI 时序

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 17/74

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 28 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 3-7: SPH=1 时的 SPI 时序

图 3-8: Microwire 格式的单次收发时序

图 3-9: Microwire 格式的多次收发时序

### 3.5.7 外设任务控制器（PTM）

PTM(Parallel Task Machine) 是一个能够并行处理多个任务的小型处理器。PTM 内部包含 4 个独立运行的处理核， 每个核能够从内置存储器 (TCM) 或系统总线获取自定义格式的指令并执行，实现总线地址读写、数据运算、判

断、跳转、IO 读写、等待触发、延时等多种功能。利用一套简洁高效的自定义指令集，用户能够以周期级的精

度控制每个核的运行，可满足 IO 读写的时序，也可实现对特定触发条件的快速响应。每个处理核可同时处理多

达 16 路 IO 的输入输出，并内置带有 DMA 接口的读写 FIFO，便于构造自定义通信接口。处理核可访问 AHB 总 线，从总线地址获取指令执行，或读写总线上的存储器和寄存器数据。处理核运行过程中可自定义产生中断与

CPU 交互。多个核之间也可基于事件通知机制协同工作。用户可基于 PTM 开发多种自定义功能，例如自定义接 口协议，自定义 PWM，内存数据批量运算，多模块互联调度，自动传感器采集等。

PTM 主要特性：

- 两级流水线，4 核并行处理

- 32 位自定义指令集，包括 4 种基础指令与 4 种访存指令

- 每个核在 TCM 上执行的指令数目为 2~64，从总线执行时无数目上限

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 18/74

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 29 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

- 基础指令具有严格的时序控制，1 个周期可执行 1 条指令

- 运算包括与、或、异或、移位、有符号数加法、比特交换等

- 可基于条件判断并跳转，实现分支和循环

- 可等待事件触发或等待固定延时

- 每个核内置 2 个 32 位和 2 个 16 位通用寄存器

- 每个核内置支持 DMA 接口的 32 位 FIFO，深度为 4 或 8

- 16 个 IO 通道 CH0~CH15，可映射到每个核的 16 路 IO

- 每个核可基于 FIFO 输出或输入多达 16 路 IO，同时单独控制其中 2 个 IO 输出

- 16 个内部事件通知用于多核协同，其中 4 个可对应 CPU 中断

- 每个核可独立复位、启动、暂停以及单步调试

3.5.8 USB2.0 FS

本芯片集成了一路全速（FS）USB2.0 Host/Device 接口，符合 USB 2.0 的协议规范，具有如下功能：

- 软件可配置的端点设置，支持挂起/恢复

- 支持动态 FIFO 大小

- 支持会话请求协议和主机协商协议

- 支持全速以及慢速模式

- 片内集成 USB2.0 FS PHY

- 拥有 ep0~ep7 8 个通道

3.5.9 DCMI

DCMI (Digital Camera Interface) 用于接收摄像头送出的 DVP/SPI 接口信号，已用于后续的处理。

DCMI 主要特性：

- 支持 8-bit/10-bit/12-bit/14-bit/16-bit DVP 格式数据

- 支持 4 线/双线/单线 SPI 数据

- 支持连续模式以及单帧模式

- 支持裁剪模式

### 3.6 定时器

3.6.1 通用定时器

GPTIM (General-Purpose Timer) 基于一个 16 比特计数器，可实现计时、测量输入信号的脉冲长度 (输入捕获) 或者产生输出波形 (输出比较和 PWM ) 等功能。计数器本身可以进行递增、递减或者递增/递减计数，计数时钟

可选系统 PCLK、IO 输入信号或级联输入信号，并可进行 1~65536 倍的预分频。GPTIM 共有 4 个通道，可以分

别独立配置为输入捕获或输出模式。计数、输入捕获和输出比较的结果可以产生中断、DMA 请求或 PTC 事件。

GPTIM 包含主从模式接口，可以进行多级级联，实现多级计数或同步触发等功能。

GPTIM 主要特性：

- 16 位递增、递减、递增/递减自动重装载计数器，最大计数 65535

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 19/74

<!-- Image omitted -->


<!-- Page 30 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

- 16 位可编程 (可以实时修改) 预分频器，计数器时钟频率的分频系数为 1~65536 之间的任意数值

- 8 位可配置重复计数

- 支持单笔计数模式 (OPM)，当重复计数完成后自动停止计数器

- 4 个独立通道，可分别配置为输入或输出模式

- 输入模式

– 上升沿/下降沿捕获

– PWM 脉宽和周期捕获 (需占用两个通道)

– 可选 4 个输入端口之一或 1 个外部触发端口，支持防抖动滤波和预降频

- 输出模式

– 强制输出高/低电平

– 计数到比较值时输出高/低/翻转电平

– PWM 输出，可配脉宽和周期

– 多通道 PWM 组合输出，可产生有相互关系的多路 PWM

– 单脉冲/重触发单脉冲模式输出

- 主从模式

– 支持多计数器互连，可在作为主设备产生控制信号的同时，作为从设备被外部输入或其它主设备控制

– 控制模式包括复位、触发、门控等

– 支持多计数器同步启动、复位等

- 编码模式输入，控制计数器递增/递减计数

- 如下事件发生时产生中断/DMA 请求/PTC 触发：

– 更新：计数器递增溢出/递减溢出，计数器初始化 (通过软件或者内部/外部触发)

– 触发事件 (计数器启动、停止、初始化或者由内部/外部触发计数)

– 输入捕获

– 输出比较

3.6.2 高级定时器

ATIM (Advanced Timer) 基于一个 32 比特计数器，可实现计时、测量输入信号的脉冲长度 (输入捕获) 或者产生 输出波形 (输出比较和 PWM) 等功能。ATIM 支持 6 路带死区保护的 PWM 互补输出，支持多路 PWM 同时换相，

并有 2 路刹车输入可快速将输出切换至安全状态。计数器本身可以进行递增、递减或者递增/递减计数，计数时

钟可选系统 PCLK、IO 输入信号或级联输入信号，并可进行 1~65536 倍的预分频。ATIM 共有 6 个通道，可以分

别独立配置为输入捕获或输出模式。计数、输入捕获和输出比较的结果可以产生中断、DMA 请求或 PTC 事件。

ATIM 包含主从模式接口，可以进行多级级联，实现多级计数或同步触发等功能。

ATIM 主要特性：

- 32 位递增、递减、递增/递减自动重装载计数器

- 16 位可编程 (可以实时修改) 预分频器，计数器时钟频率的分频系数为 1~65536 之间的任意数值

- 16 位可配置重复计数

- 支持单笔计数模式 (OPM)，当重复计数完成后自动停止计数器

- 6 个独立通道

– 通道 1~3 可分别配置为输入或输出模式，其中每个通道可输出两路带死区保护的互补 PWM

– 通道 4 可配置为输入或输出模式，可输出单路 PWM

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 20/74

<!-- Image omitted -->


<!-- Page 31 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

– 通道 5~6 可配置为输出比较模式

- 输入模式

– 上升沿/下降沿捕获

– PWM 脉宽和周期捕获 (需占用两个通道)

– 可选 4 个输入端口之一或 1 个外部触发端口，支持防抖动滤波和预降频

- 输出模式

– 强制输出高/低电平

– 计数到比较值时输出高/低/翻转电平

– PWM 输出，可配脉宽和周期

– 多通道 PWM 组合输出，可产生有相互关系的多路 PWM

– 单脉冲/重触发单脉冲模式输出

- 主从模式

– 支持多计数器互连，可在作为主设备产生控制信号的同时，作为从设备被外部输入或其它主设备控制

– 控制模式包括复位、触发、门控等

– 支持多计数器同步启动、复位等

- 编码模式输入，控制计数器递增/递减计数

- 支持用于定位的霍尔传感器电路

- 2 路刹车输入，支持防抖动滤波，可将输出快速置于安全状态。刹车信号源包括：

– CPU lockup

– 外部输入

– 软件触发

- 如下事件发生时产生中断/DMA 请求/PTC 触发：

– 更新：计数器递增溢出/递减溢出，计数器初始化 (通过软件或者内部/外部触发)

– 触发事件 (计数器启动、停止、初始化或者由内部/外部触发计数)

– 输入捕获

– 输出比较

– 刹车

– 换相

3.6.3 基本定时器

BTIM (Basic Timer) 基于一个 32 比特递增计数器，可实现计时功能。计数时钟为系统 PCLK 或级联输入信号，并 可进行 1~65536 倍的预分频。计时结果可以产生中断、DMA 请求或 PTC 事件。BTIM 包含主从模式接口，可以 进行多级级联，实现多级计数或同步触发等功能。

BTIM 主要特性：

- 32 位递增自动重装载计数器

- 16 位可编程预分频器，计数器时钟频率的分频系数为 1~65536 之间的任意数值

- 支持单笔计数模式 (OPM)，计数完成后自动停止计数器

- 主从模式

– 支持与 BTIM 及 GPTIM 互连，可在作为主设备产生控制信号的同时，作为从设备被外部输入或其它 主设备控制

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 21/74

<!-- Image omitted -->


<!-- Page 32 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

– 控制模式包括复位、触发、门控等

– 支持多定时器同步启动、复位等

- 计数器溢出或初始化时产生中断，DMA 请求及 PTC 触发

3.6.4 低功耗定时器

LPTIM (Low-Power Timer) 基于一个 24 比特递增计数器，可实现计时、产生输出波形 (输出比较和 PWM) 和唤 醒系统等功能。计数时钟可以为系统时钟、低功耗时钟、IO 输入信号或比较器输出，并可进行最多 128 倍的预

分频以及最多 256 次的循环计数。根据计数结果可以产生 PWM 输出，并可产生中断，或产生唤醒信号将系统

从低功耗模式唤醒。当用 IO 输入信号作为计数时钟时，支持不依赖于内部时钟进行计数并产生唤醒信号，从而 允许系统关闭内部时钟。

LPTIM 主要特性：

- 24 位向上自动重装载计数器，最大计数 16777215(2^24-1)

- 计数时钟选择

– 内部时钟，PCLK2 或低功耗时钟

– 可选边沿的 IO 输入信号或比较器输出，可利用内部时钟进行防抖动，也可不依赖内部时钟独立计数

- 8 档预分频，计数时钟分频系数为 2 的 0~7 次方

- 1~256 循环次数

- 计数模式

– 连续计数模式

– 单笔计数模式，循环次数完成后计数结束

- 可配极性的输出模式

– PWM 输出，可配脉宽、周期

– 单次翻转输出

– 单脉冲或指定个数脉冲输出

- 触发模式

– 软件触发

– IO 输入信号边沿触发，支持防抖动滤波

- 超时检测，每次外部触发时计数器复位

- 如下事件发生时产生中断或唤醒信号：

– 更新

– 计数器溢出

– 输出比较

– 外部触发

3.6.5 看门狗

看门狗计时器作为一种计数器主要是用于在到达设定好的时间之后重置系统，以防止软件挂死。

看门狗计时器基本功能：

- 支持两种工作模式：

– Mode0

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 22/74

<!-- Image omitted -->


<!-- Page 33 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

\* wdt 不会产生中断，在到达设定的时间之后会直接重置系统

\* 最高支持 24bit 的计数器

– Mode1 \* 分为两段计数，在到达第一段设定的时间之后，会产生中断，在到达第二段设定的时间之后，再 重置系统

\* 每个时间段最高支持 24bit 的计数器

- 支持写保护，以防止软件对 wdt 进行误操作

### 3.7 加密校验

3.7.1 AES 引擎

AES 引擎是一个针对对称加密算法的运算加速器，用户可以自行配置加解密算法的密钥以及初始向量对内存中 的数据进行加解密运算操作，同时将结果存储到指定的内存区域。

相较软件的加解密运算，AES 引擎有更高的运算速度，更灵活的配置，更好的外设存储设备访问效率。除此以 外，在 bypass 模式下，AES 引擎也可以作为数据传输的 DMA 来使用。

AES 主要特性：

- 支持 AES-128、AES-192、AES-256 以及国密 SM4 算法标准

- 支持 ECB、CTR 以及 CBC 模式

- 可以调用 RootKey 进行加解密的运算，同时保证 RootKey 不被外部程序读取

3.7.2 HASH 引擎

HASH 引擎是一个针对散列数列算法的运算加速器，用户可以选择不同的散列算法计算内存中特定数据的散列 值。HASH 引擎相比软件算法速度更快，配置也灵活，用户也可以通过自定义初始向量，实现多线程 HASH 运

算。HASH 引擎支持算法包括 SHA1、SHA224、SHA256 和 SM3。

3.7.3 CRC

CRC (Cyclic Redundancy Check) 可进行特定位宽、任意生成多项式、任意初始值的 CRC 计算。数据可以通过 CPU 或 DMA 输入，最小输入单元为单字节，没有最长字节数限制。单 HCLK 周期即能够完成单字节输入的计算。数 据输入全部完成后即时得到校验结果。支持输入数据高低位倒转和输出数据高低位倒转。支持不同有效位宽的 输入数据。

CRC 主要特性：

- 7/8/16/32 比特 CRC 计算

- 任意自定义多项式

- 任意初始值

- 输入数据支持单字节/双字节/三字节/四字节有效位宽

- 输入数据支持字节/双字节/四字节高低位比特倒转

- 输出数据支持高低位比特倒转

- 计算速度为每 HCLK 周期 1 字节

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 23/74

<!-- Image omitted -->


<!-- Page 34 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

3.7.4 真随机数发生器（TRNG）

TRNG（True Random Number Generator）是一个借助振荡电路的不稳定性来产生随机数的模块。该模块无需外 部的随机熵源，可以通过激活内部多个振荡电路，通过一定的熵源处理逻辑，生成随机数。

TRNG 主要特性：

- 独立的内部熵源

- 单次产生 256-bit 的种子和 256-bit 随机数

- 针对熵源的死锁校验

### 3.8 存储接口

3.8.1 MPI 接口

MPI (Memory Peripheral Interface) 控制器是一个专用的 memory 通信接口，支持多种片外存储颗粒，包括：

- SPI NOR Flash，支持 1 线/2 线/4 线，支持 DTR 模式

- SPI NAND Flash，支持 1 线/2 线/4 线

- pSRAM，支持 x8 数据位宽，支持 Xccela 标准接口，兼容 Legacy 接口

- HyperRAM，支持 x8 数据位宽，支持 HyperBus 标准接口

图 3-10: MPI 控制器框图

MPI 控制器支持两种操作模式：（1）寄存器模式和（2）地址映射模式。两种模式的切换由硬件自动完成，可动 态穿插执行。且无论哪种模式，都支持高度可定制的接口时序，以兼容各种存储颗粒。

寄存器模式

- 通过寄存器操作，发送一个命令时序。也可以将该命令设置为状态查询命令反复发送，直到读回的数据满

足某个预设状态

- 支持发送包含两个命令时序的序列，其中第二个命令可设置为状态查询命令反复发送，直到读回的数据满

足某个预设状态

- 支持 DMA 通道，通过寄存器 FIFO 接口完成数据搬运

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 24/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 35 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 3-11: 寄存器模式单个和多个命令时序的序列

地址映射模式

- 外部 memory 映射为 AHB 地址空间，自动将 AHB 总线读写转换为预设的 Memory 接口时序，实现 XIP 功

能

- 支持 Byte (8-bit)、Half-word (16-bit) 和 Word (32-bit) AHB 访问

- 高效转换 AHB Wrap 操作，不依赖于颗粒是否支持 Wrap

- 支持 XIP 实时（On-The-Fly）解密，模式为 AES128-CTR 或者 AES256-CTR

- 支持连续读写功能，如果当前一笔 AHB 的读写地址与前一笔连续，则直接开始数据传输，省略命令和地

址部分。该功能可大幅提升大块数据搬运时的有效带宽

- 针对 pSRAM 和 HyperRAM 的内部动态刷新特性，自动处理颗粒最长 CS 拉低时间、最近 CS 访问间隔、最

大 burst 数据长度等限制，无需软件处理

### 3.8.2 SD/SDIO/eMMC

SDMMC 支持 SD 协议 3.0 以及 eMMC 标准 4.51，可作为 HOST 控制器与 SD/SDIO/eMMC 设备交互。并与 DMAC 配合进行数据读写。SDMMC 支持 SDR 单线和 4 线模式，不支持 DDR。

SDMMC 主要特性：

- 兼容 SD Host Controller Standard Specification Version 3.0

- 兼容 SD 3.0 Physical Layer Specification Version 3.01

- 兼容 SDIO Specification Version 3.0

- 兼容 JEDEC JESD84-B451 eMMC 4.51 Specification

- 支持 SDSC/SDHC/SDXC/SDHS 卡

- 支持 SDR12/SDR25/SDR50

- 支持 SDR 单线，4 线模式

- 内置 2K 字节 FIFO，最大支持单 block 512 字节

- 可配置时钟

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 25/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 36 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

- 配合 DMAC 进行数据搬运

### 3.9 CAN

CAN(Controller Area Network) 兼容 CAN 协议 2.0b，不支持 CAN FD。CAN 的基础传输速率可配置，内部包含 接收 FIFO 与发送 FIFO，支持多帧自动收发、自动重传、抢占发送等功能。CAN 包含灵活可配置的接收 ID 自动

过滤器，仅在接收 FIFO 中保存过滤后的帧。发现总线和收发异常时，CAN 具有完备的错误通知机制。

CAN 主要特性:

- 兼容 CAN 2.0b，不支持 CAN FD

- 可配置数据率至 1Mbps

- 支持基础帧和扩展帧

- 帧数据长度可达 8byte

- 接收 FIFO 深度 7，发送 FIFO 深度 7+1

- 16 个灵活配置的 ID 过滤器

- 支持帧抢占发送

- 错误检测与通知机制

## 3.10 各种外设接口速率汇总

表 3-1: 常用接口速率

控制器 最高速率 单位 备注

MPI1 156 MHz 合封 OPI-PSRAM(支持 DDR)

104 MHz 合封 QSPI-NOR Flash (支持 DTR)

MPI2 156 MHz 合封 OPI-PSRAM(支持 DDR)

104 MHz 合封 QSPI-NOR Flash (支持 DTR)

MPI3 104 MHz 外挂 QSPI-NOR，QSPI-NAND Flash(支持 SDR)

SDMMC 48 MHz 外挂 eMMC

I2C 3.4 MHz

SPI 48 MHz

UART 3 Mbaud

I2S 48 KHz 采样率 48KHz，32-bit×2 channel

PDM 3.072 MHz

GPADC 4 Msps

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 26/74

<!-- Image omitted -->


<!-- Page 37 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 4 电气特性

## 4.1 基本电气特性

表 4-1: 运行条件

Symbol Description Min Typ Max Unit

PVDD System power input 2.97 3.63 V

Tamb Ambient temperature -40 85 °C

VIL CMOS low level input voltage 0 0.3×VIO V

VIH CMOS high level input voltage 0.7×VIO VIO V

VTH CMOS threshold voltage 0.5×VIO V

\* 开机电压 3.5V±10%

表 4-2: 绝对最大值

Symbol Description Min Typ Max Unit

PVDD System power input 3.63 V TStorage Storage temperature -40 125 °C

表 4-3: I/O 特性 @3.3V

符号 参数 最小值 典型值 最大值 单位

CIN 管脚电容 2.5 3 3.5 pF

VIH 高电平输入电压 0.7\*VDD - VDD V

VIL 低电平输入电压 VSS - 0.3\*VDD V

IIH 高电平输入电流 - 10 40 nA

IIL 低电平输入电流 - 10 40 nA

VOH 高电平输出电压 (高阻负载) 0.8\*VDD - VDD V

VOL 低电平输出电压 (高阻负载) VSS - 0.2\*VDD V

IOH 高电平驱动电流 12 30 38 mA （VOH=0.8\*VDD, 最大驱动强度）

IOL 低电平驱动电流 12 30 38 mA （VOH=0.2\*VDD, 最大驱动强度）

RPU 内部上拉电阻（Vpad=0.8\*VDD） 7 10 20 kΩ

RPD 内部下拉电阻（Vpad=0.2\*VDD） 7 10 20 kΩ

VIH\_nRST 芯片复位释放电压 0.7\*VDD - VDD V

VIL\_nRST 芯片复位电压 VSS - 0.3\*VDD V

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 27/74

<!-- Image omitted -->


<!-- Page 38 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 4.2 可靠性

表 4-4: 可靠性测试

测试项目 测试条件 适用产品 测试标准

HTOL（高温工作寿命） Tj=125◦C，1000 小时 SF32LB57X JESD22-A108

ESD（静电放电敏感度) HBM（人体放电模式）± 4000 V SF32LB57X JS-001-2017

CDM（充电器件模式）±1000V SF32LB57X JS-002-2018

Latch-up (闩锁测试) I-Test: ± 200mA SF32LB57X JESD78E OVT: +1.5×VddMAX SF32LB57X

MSL3(湿度敏感等级) 烘烤：125◦C，24 小时

浸泡：30◦C，60% RH，192 小时

回流焊：260 + 0◦C，20 秒，三次 SF32LB57X JESD22-A113

TCT（高低温循环试验） -65◦C~150◦C，Dwell=15min，1000 次循环 SF32LB57X JESD22-A104

uHAST（无偏压高加速 温湿度应力试验） 130◦C，85% RH，33.3psig，96 小时 SF32LB57X JESD22-A118

HTSL（高温贮存寿命） 150◦C，1000 小时 SF32LB57X JESD22-A103

PCT(高压蒸煮实验) 121◦C，100% RH，205kPa，96 小时 SF32LB57X JESD22-A102

Solderability(可焊性实验) 245◦C，steam aging 8 小时 SF32LB57X J-STD-002

## 4.3 功耗特性

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 28/74

<!-- Image omitted -->


<!-- Page 39 -->

# SF32LB57X

## 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 4.3.1 处理器功耗

### 表 4-5: 处理器功耗

时钟频率

### 电源电压 3.3V

### 电流（uA)

### 电源电压 3.3V

### 电流增量（uA/MHz)

### CoreMark

### 240MHz 11554

45

### 216MHz 10471

### 192MHz 9408

45

### 168MHz 8333

### 144MHz 5841

36

### 120MHz 4970

48MHz 1823 31

### 24MHz 860

26

### 12MHz 546

### WhileLoop

### 240MHz 9483

36

### 216MHz 8614

### 192MHz 7763

36

### 168MHz 6891

### 144MHz 4853

30

### 120MHz 4145

48MHz 1586 26

### 24MHz 767

17

### 12MHz 566

关机 1.0

### 4.3.2 BT 和 BLE 功耗

表 4-6: BT 和 BLE 功耗

模式 条件 3.3V @TXpower = 0dBm 3.3V @TXpower = 4dBm 3.3V @TXpower =10dBm 单位

∆BT Sniff Mode 50ms (attempt=4) 177.9 193.7 226.6 uA 100ms (attempt=4) 88.9 96.8 113.2 uA 200ms (attempt=4) 44.4 48.4 56.6 uA 500ms (attempt=4) 17.8 19.3 22.6 uA 1s (attempt=4) 8.9 9.7 11.3 uA

∆BLE ADV 50ms 116.8 178.6 293.8 uA 100ms 66.0 91.9 146.8 uA 200ms 33.1 46.6 77.8 uA 500ms 13.3 18.8 31.9 uA 1s 6.7 9.4 15.8 uA

∆BLE Connection 50ms 78.5 91.3 110.6 uA 100ms 41.3 48.1 55.8 uA 200ms 20.7 24.1 29.0 uA 500ms 8.3 9.7 12.6 uA 1s 5.6 6.3 7.3 uA ∆Both Scan Inquiry Scan and Page Scan 76 80.3 81.3 uA

Deepsleep 28 uA

\* 1. Scan 每 1.28s 接收 11.25ms，Both Scan 每 1.28s 接收 22.5ms。 2. bt 500ms sniff@TXpower10dBm：=22.6+28=50.6uA

## DS5702‐SF32LB57X‐CN V0.2.2

### ©2026 思澈科技（南京）有限公司 www.sifli.com

29/74

<!-- Image omitted -->


<!-- Page 40 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 4.4 蓝牙射频

### 4.4.1 BLE 射频

4.4.1.1 BLE 发射机性能

表 4-7: BLE 发射机性能—1Mbps 模式

Parameter Condition Min Typ Max Unit

Maximum RF transmit power 17 dBm

RF power control range -20 dBm

Adjacent channel transmit power (@+17dBm ) F = F0+2MHz -35 -20 dBm

F = F0-2MHz -35 -20 dBm

F = F0+3MHz -40 -30 dBm

F = F0-3MHz -40 -30 dBm

F = F0+>3MHz -45 -30 dBm

F = F0->3MHz -45 -30 dBm

∆f1avg Maximum modulation 225 250 275 kHz

∆f2max Minimum modulation 185 205 kHz

∆f2avg/∆f1avg 0.8 0.85

ICFT (initial carriar frequency tolerance) -150 ±20 150 kHz

Drift rate -20 ±6 20 kHz/50us

Drift -50 ±6 50 kHz

Harmonic spur (@+17dBm transmit power) Second harmonic -50∗ dBm

Third harmonic -40∗ dBm

∗ With external π type matching network

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 30/74

<!-- Image omitted -->


<!-- Page 41 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 4-8: BLE 发射机性能—2Mbps 模式

Parameter Condition Min Typ Max Unit

Maximum RF transmit power 17 dBm

RF power control range -20 dBm

Adjacent channel transmit power (@+17dBm ) F = F0+4MHz -35 dBm

F = F0-4MHz -35 dBm

F = F0+5MHz -40 dBm

F = F0-5MHz -40 dBm

F = F0+>5MHz -45 dBm

F = F0->5MHz -45 dBm

∆f1avg Maximum modulation 450 500 550 kHz

∆f2max Minimum modulation 370 420 kHz

∆f2avg/∆f1avg 0.8 0.85

ICFT -150 ±20 150 kHz

Drift rate -20 ±6 20 kHz/50us

Drift -50 ±6 50 kHz

Harmonic Spur (@+17dBm transmit power) Second harmonic -50∗ dBm

Third harmonic -40∗ dBm

∗ With external π type matching network

### 4.4.1.2 BLE 接收机性能

表 4-9: BLE 接收机性能—1Mbps 模式

Parameter Condition Min Typ Max Unit

Frequency Range 2402 2480 MHz

Sensitivity with dirty transmit off@30.8% PER & 37bytes -100 dBm

Sensitivity with dirty transmit on@30.8% PER & 37bytes -99 dBm

Maximun received signal@30.8% PER & 37bytes 0 dBm

C/I Co-channel 7 dB

Adjacent channel selectivity C/I F=F0+1MHz -10 dB

F=F0-1MHz -7 dB

F=F0+2MHz -42 dB

F=F0-2MHz -42 dB

F=F0+3MHz -50 dB

F=F0-3MHz -40 dB

F=Fimage(F0-4MHz) -24 dB

F=Fimage+1MHz(F0-3MHz) -40 dB

F=Fimage-1MHz(F0-5MHz) -40 dB

Intermodulation -24 dBm

Out of band blocking performance 30MHz~2000MHz -11 -11 dBm

2000MHz~2400MHz -25 -10 dBm

2500MHz~3000MHz -25 -10 dBm

3000MHz~12.5GHz -10 -10 dBm

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 31/74

<!-- Image omitted -->


<!-- Page 42 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 4-10: BLE 接收机性能—2Mbps 模式

Parameter Condition Min Typ Max Unit

Frequency Range 2402 2480 MHz

Sensitivity with dirty transmit off@30.8% PER & 37bytes -97 dBm

Sensitivity with dirty transmit on@30.8% PER & 37bytes -96 dBm

Maximun received signal@30.8% PER & 37bytes 0 dBm

C/I co-channel 7 dB

Adjacent channel selectivity C/I F = F0+2MHz -10 dB

F = F0-2MHz -8 dB

F = F0+4MHz -44 dB

F = F0-4MHz -34 dB

F = F0+6MHz -48 dB

F = F0-6MHz -24 dB

F = Fimage(F0-6MHz) -24 dB

F = Fimage+2MHz(F0-4MHz) -34 dB

F = Fimage-2MHz(F0-8MHz) -24 dB

Intermodulation -24 dBm

Out of band blocking performance 30MHz~2000MHz -11 -11 dBm

2000MHz~2400MHz -25 -10 dBm

2500MHz~3000MHz -25 -10 dBm

3000MHz~12.5GHz -10 -10 dBm

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 32/74

<!-- Image omitted -->


<!-- Page 43 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 4-11: BLE 接收机性能—S2 模式

Parameter Condition Min Typ Max Unit

Frequency Range 2402 2480 MHz

Sensitivity with dirty transmit off@30.8% PER & 37bytes -104 dBm

Sensitivity with dirty transmit on@30.8% PER & 37bytes -103.5 dBm

Maximum received signal@30.8% PER 0 dBm

C/I co-channel 3 dB

Adjacent channel selectivity C/I F = F0+1MHz -12 dB

F = F0-1MHz -9 dB

F = F0+2MHz -48 dB

F = F0-2MHz -43 dB

F = F0+3MHz -58 dB

F = F0-3MHz -44 dB

F = Fimage(F0-4MHz) -32 dB

F = Fimage+1MHz(F0-3MHz) -44 dB

F = Fimage+1MHz(F0-5MHz) -44 dB

表 4-12: BLE 接收机性能—S8 模式

Parameter Condition Min Typ Max Unit

Frequency Range 2402 2480 MHz

Sensitivity with dirty transmit off@30.8% PER & 37bytes -107.5 dBm

Sensitivity with dirty transmit on@30.8% PER & 37bytes -107 dBm

Maximum received signal@30.8% PER 0 dBm

C/I co-channel 1 dB

Adjacent channel selectivity C/I F = F0+1MHz -12 dB

F = F0-1MHz -8 dB

F = F0+2MHz -48 dB

F = F0-2MHz -44 dB

F = F0+3MHz -58 dB

F = F0-3MHz -44 dB

F = Fimage(F0-4MHz) -32 dB

F = Fimage+1MHz(F0-3MHz) -44 dB

F = Fimage+1MHz(F0-5MHz) -44 dB

### 4.4.2 经典蓝牙

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 33/74

<!-- Image omitted -->


<!-- Page 44 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 4.4.2.1 发射机性能

### 表 4-13: 发射机性能—Basic Data Rate

Parameter Condition Min Typ Max Unit

Maximum RF transmit power RF output power 17 dBm

RF power control step 2 4 8 dB

Adjacent channel transmit power F = F0+2MHz -40 -20 dBm

F = F0-2MHz -40 -20 dBm

F = F0+3MHz -43 -40 dBm

F = F0-3MHz -43 -40 dBm

F = F0+>3MHz -45 -40 dBm

F = F0->3MHz -45 -40 dBm

∆f1avg modulation 140 160 175 kHz

∆f2max modulation 120 150 175 kHz

∆f2avg/∆f1avg 0.8 0.9

ICFT -75 ±20 75 kHz

Drift rate -20 ±4 20 kHz/50us

Drift (1 slot packet) -25 ±6 25 kHz

Drift (5 slot packet) -40 ±8 40 kHz

Harmonic spur(@ +17dbm transimit power) 3G-20GHz −40∗ dBm

∗ With external π type matching network

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 34/74

<!-- Image omitted -->


<!-- Page 45 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 表 4-14: 发射机性能—Enhanced Data Rate

Parameter Condition Min Typ Max Unit

Maximum RF transimit power RF output power 13 dBm

DPSK Power - GFSK Power 2-DH5 -2 dB

π/4 DQPSK max w0 -10 ±2 10 kHz

π/4 DQPSK max wi -75 ±20 75 kHz

π/4 DQPSK max wi + w0 -75 ±20 75 kHz 8DPSK max w0 -10 ±2 10 kHz

8DPSK max wi -75 ±20 75 kHz

8DPSK max wi + w0 -75 ±20 75 kHz

π/4 DQPSK modulation accuracy RMS DEVM 4 20 %

99% DEVM 7 30 %

Peak DEVM 13 35 %

8DPSK modulation accuracy RMS DEVM 4 13 %

99% DEVM 7 20 %

Peak DEVM 13 25 %

Adjacent channel transmit power (@+12.5dBm) F=F0+2MHz -23 -20 dBm

F=F0-2MHz -21 -20 dBm

F=F0+3MHz -34 -40 dBm

F=F0-3MHz -32 -40 dBm

F=F0+>3MHz -42 -40 dBm

F=F0->3MHz -42 -40 dBm

### 4.4.2.2 接收机性能

### 表 4-15: 接收机性能—Basic Data Rate

Parameter Condition Min Typ Max Unit

Sensitivity with dirty transmit off@0.1% BER -96 dBm

Sensitivity with dirty transmit on@0.1% BER -95 dBm

Maximum received signal@0.1% BER 0 dBm

C/I co-channel 8.5 dB

Adjacent channel selectivity C/I F = F0+1MHz -13 dB

F = F0-1MHz -10 dB

F = F0+2MHz -43 dB

F = F0-2MHz -42 dB

F = F0+3MHz -47 dB

F = F0-3MHz -45 dB

F = Fimage(F0-5MHz) -28 dB

F = Fimage+1Mhz(F0-4MHz) -40 dB

F = Fimage-1Mhz(F0-6MHz) -40 dB

Intermodulation -21 dBm

Out of band blocking performance 30MHz~2000MHz -10 dBm

2000MHz~2400MHz -10 dBm

2500MHz~3000MHz -10 dBm

3000MHz~12.5GHz -10 dBm

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 35/74

<!-- Image omitted -->


<!-- Page 46 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 表 4-16: 接收机性能—Enhanced Data Rate-π/4 DQPSK

Parameter Condition Min Typ Max Unit

Sensitivity with dirty transmit off@0.01% BER -95 dBm

Sensitivity with dirty transmit on@0.01% BER -94.5 dBm

Maximum received signal@0.01% BER 0 dBm

C/I co-channel 10 dB

Adjacent channel selectivity C/I F = F0+1MHz -13 dB

F = F0-1MHz -11 dB

F = F0+2MHz -40 dB

F = F0-2MHz -40 dB

F = F0+3MHz -45 dB

F = F0-3MHz -45 dB

F = Fimage(F0-5MHz) -25 dB

F = Fimage+1MHz(F0-4MHz) -40 dB

F = Fimage-1MHz(F0-6MHz) -40 dB

### 表 4-17: 接收机性能—Enhanced Data Rate-8DPSK

Parameter Condition Min Typ Max Unit

Sensitivity with dirty transmit off@0.01% BER -88.5 dBm

Sensitivity with dirty transmit on@0.01% BER -87.5 dBm

Maximum received signal@0.01% BER 0 dBm

C/I co-channel 17 dB

Adjacent channel selectivity C/I F = F0+1MHz -6 dB

F = F0-1MHz -6 dB

F = F0+2MHz -33 dB

F = F0-2MHz -33 dB

F = F0+3MHz -37 dB

F = F0-3MHz -37 dB

F = Fimage(F0-5MHz) -20 dB

F = Fimage+1Mhz(F0-4MHz) -30 dB

F = Fimage-1Mhz(F0-6MHz) -30 dB

## 4.5 音频性能

表 4-18: 音频 ADC 性能

Analogue to Digital Converter under 3.3V

Parameter Test Condition Min Typ Max Unit

Resolution / / 24 Bits

Sample Frequency 8 / 96 kHz

Analog Gain Range 6dB/Step -6 18 dB

Input Resistance Analog Gain = 0dB, @48kHz Sample Frequency / 23 / KΩ

Signal to Noise Ratio 1kHz Input, @48kHz Sample Frequency, Output A-Weighted / 97 / dB

Total Harmonic Distortion+Noise Analog Gain = 0dB, 1kHz Input,

@48kHz Sample Frequency / -86 / dB

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 36/74

<!-- Image omitted -->


<!-- Page 47 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 4-19: 音频 DAC 性能

Digital to Analogue Converter under 3.3V

Parameter Test Condition Min Typ Max Unit

Resolution / / 24 Bits

Output Swing 1.1 Vrms

Sample Frequency 8 / 96 kHz

Total Harmonic Distortion+Noise 1kHz Output, 0dBFS, with 10kOhm Loading,

@48kHz Sample Frequency, Output A-Weighted / -95 / dB

Dynamic Range 1kHz Output, -60dBFS, with 10kOhm Loading,

@48kHz Sample Frequency, Output A-Weighted / 109 / dB

Noise Floor / 3.7 / µV rms

Signal to Noise Ratio 1kHz Output, 0dBFS, with 10kOhm Loading,

@48kHz Sample Frequency, Output A-Weighted / 109 / dB

### 4.6 IO 驱动能力

表 4-20: IO 驱动能力

DS1 DS0 Driving Capability

0 0 2mA

0 1 4mA

1 0 8mA

1 1 12mA

## 4.7 GPADC 电气特性

表 4-21: 12-bit GPADC 电气特性

Min. Typ. Max. Unit Comments

Resolution 12 bit

Tsample (Differential) 125n 1/3 s

fs=1/(Tsample+Tconversion) Tsample (Single-Ended) 166.66n 1/3 s

Tconversion 145n 5.33u s

Sample rate (fs) 3.7 Msps

ENOB (Differential) 10.6 bit Vin=-1dBFs, no averaging

ENOB (Single-Ended) 10.2 bit Vin=-3dBFs, no averaging

SNDR (Differential) 65.6 dB Vin=-1dBFs, no averaging

SNDR (Single-Ended) 63.16 dB Vin=-3dBFs, no averaging

Current Consumption 466 uA fs=3.7Msps

130 uA fs=500ksps

90 uA fs=100ksps

GPADC 外部源端电路电阻 RAIN 与采样时间的关系表如下：

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 37/74

<!-- Image omitted -->


<!-- Page 48 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

Resolution (bit) Number of TPCLK Cycle @48MHz Tsample (ns) Maximum source resistance RAIN (kOhm)

12 8 166 1

30 625 5

60 1250 10

300 6250 50

600 12500 100

3000 62500 500

30000 625000 5000

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 38/74

<!-- Image omitted -->


<!-- Page 49 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 5 封装与硬件

## 5.1 管脚布局

2 1

3

4

5 6

7 8

9

10

11

12

13 14

15

16 17 50 51

49

48

47

46

45

44

43

42

41

40

39

38

37

36 35

# SF32LB57X

# GND PAD

# Uxxx6

PA30

PA29 PA32 PA31

PA28 PA27

PA26

PA25

PA24

PA23

PA22

VDD\_RTC

VDD\_RET AVDD33

VDD\_VOUT2

VDD\_VOUT1 BUCK\_FB PA2 PA3 PA0

PA1

PA4

PA5

PA6

PA7

PA8

PA9

PA55

PA56 AUD\_VREF

AVDD33\_AUD

DACP

DACN

PA10

图 5-1: SF32LB57XUxxx6 QFN68 管脚分布图

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 39/74

<!-- Image omitted -->


<!-- Page 50 -->

# SF32LB57X

## 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# SF32LB57X

# GND PAD

# Yxxx6

62 61

63

64

65

66

67

68

69

70

71

72

73

74

75

76

77 39 40

38

37

36

35

34

33

32

31

30

29

28

27

26

25

24 GPADC\_VREF

PA32 PA34

PA33

PA31

PA30

PA29

PA28

PA27

PA26

PA25

PA24

PA23

PA22

VDD\_RTC

VDD\_RET

AVDD33 PA54

PA0 PA52

PA53

PA1

PA2

PA3

PA4

PA5

PA6

PA7

PA8

PA9

PA55

PA56

AUD\_VREF

AVDD33\_AUD 78

79

80 VDD\_VOUT2

VDD\_VOUT1

BUCK\_FB 23

22

21 DACP PA57 PA51

### 图 5-2: SF32LB57XYxxx6 QFN80 管脚分布图

## DS5702‐SF32LB57X‐CN V0.2.2

### ©2026 思澈科技（南京）有限公司 www.sifli.com

40/74

<!-- Image omitted -->


<!-- Page 51 -->

# SF32LB57X

# 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

# 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

PA32 GPADC VREFP PA34 PA40 PA44 XTAL48 MXI XTAL48 MXO BRF ANT AVSS TRF2 AVDD BRF

PA31 PA30 PA35 PA36 PA39 PA43 PA47 PA50 AVSS LO AVSS BB AVSS RRF AVSS TRF PA51

PA28 PA37 PA38 PA46 PA49 PA01 PA52 PA00

PA22 PA23 PA29 PA33 PA41 PA42 PA45 PA48 PA53 PA04 PA02 PA03

VDD RTC VDD RET PA27 GPADC VREFN AVSS33 ANA VSS VSS VSS PA54 PA8 PA7 PA5 PA6

AVDD33 ANA PA26 VSS VSS VSS VSS AUDVREF GND PA57 PA56 PA9

VDD VOUT2 VDD VOUT1 PA25 PA24 VSS PA21 PA15 PA14 AVSS33 AUD PA55 AUD VREF

BUCK FB PVSS PMU VSS VDDIOA PA19 PA18 SB03 PA13 PA11 AVDD33 AUD

BUCK LX VBATS VDDIO SB VDDIO SA PA20 PA17 SB01 SB07 SB08 PA10 DAC1P

PVDD PMU VDD18 VOUT PA16 SB02 SB09 PA12 DAC1N 1 2 3 4 5 6 7 8 9 10 11 12 13

A

B

C

D

E

F

G

H

J

K AVSS PMU PVDD PMU VSS VSS NC

NC

NC

# 图 5-3: SF32LB57XVxxx6 BGA112 管脚分布图

# DS5702‐SF32LB57X‐CN V0.2.2

# ©2026 思澈科技（南京）有限公司 www.sifli.com

41/74

<!-- Image omitted -->


<!-- Page 52 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 5.2 管脚描述

本芯片的管脚类型如表5-1所示，以下将分别对大核域 GPIO、其它专用管脚加以描述。

表 5-1: 管脚类型

Pin Type Description I/O Digital input/output I Digital input O Digital output A, I Analog input A, O Analog output A, I/O Analog input/output PWR Power GND Ground

说明：

- 在低功耗场景下，悬空的输入有可能导致开启了输入功能的数字 I/O 的功耗上升，所以在这类 I/O 上

需要按照功能配置相应的确定电压，具体方法包括连接在其它芯片的有确定输出电压的 I/O 上，或 连接相应的上拉或下拉电阻。

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 42/74

<!-- Image omitted -->


<!-- Page 53 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 5.2.1 大核域 GPIO（PA）列表

表 5-2: 大核域 GPIO（PA）管脚列表

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

51 36 C12 PA00 I/O 0 GPIO\_A0

1 LCDC1\_SPI\_RSTB

3 SD2\_DIO2

16~255 See Matrix index Table Others Reserved

50 35 C10 PA01 I/O 0 GPIO\_A1

3 SD2\_DIO3

16~255 See Matrix index Table Others Reserved

49 34 D12 PA02 I/O 0 GPIO\_A2

1 LCDC1\_SPI\_TE

3 SD2\_CLK

16~255 See Matrix index Table Others Reserved

48 33 D13 PA03 I/O 0 GPIO\_A3

1 LCDC1\_SPI\_CS

3 SD2\_CMD

16~255 See Matrix index Table Others Reserved

47 32 D10 PA04 I/O 0 GPIO\_A4

1 LCDC1\_SPI\_CLK

3 SD2\_DIO0

16~255 See Matrix index Table Others Reserved

46 31 E12 PA05 I/O 0 GPIO\_A5

1 LCDC1\_SPI\_DIO0

3 SD2\_DIO1

16~255 See Matrix index Table Others Reserved

45 30 E13 PA06 I/O 0 GPIO\_A6

1 LCDC1\_SPI\_DIO1

2 SD1\_DIO2

16~255 See Matrix index Table Others Reserved

44 29 E11 PA07 I/O 0 GPIO\_A7

1 LCDC1\_SPI\_DIO2

2 SD1\_DIO3

16~255 See Matrix index Table Others Reserved

43 28 E10 PA08 I/O 0 GPIO\_A8

1 LCDC1\_SPI\_DIO3

2 SD1\_CLK

16~255 See Matrix index Table Others Reserved 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 43/74

<!-- Image omitted -->


<!-- Page 54 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-2: 大核域 GPIO（PA）管脚列表（续）

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

42 26 F12 PA09 I/O 0 GPIO\_A9

2 SD1\_CMD

15 #ADC1N

16~255 See Matrix index Table Others Reserved

35 19 J12 PA10 I/O 0 GPIO\_A10

2 SD1\_DIO0

15 #ADC2P

16~255 See Matrix index Table Others Reserved

34 18 H12 PA11 I/O 0 GPIO\_A11

2 SD1\_DIO1

15 #ADC2N

16~255 See Matrix index Table Others Reserved

33 17 K12 PA12 I/O 0 GPIO\_A12

1 MPI3\_CS

2 SD1\_DIO2

16~255 See Matrix index Table Others Reserved

32 16 H11 PA13 I/O 0 GPIO\_A13

1 MPI3\_DIO1

2 SD1\_DIO3

16~255 See Matrix index Table Others Reserved

31 15 G9 PA14 I/O 0 GPIO\_A14

1 MPI3\_DIO2

2 SD1\_CLK

16~255 See Matrix index Table Others Reserved

30 14 G8 PA15 I/O 0 GPIO\_A15

1 MPI3\_DIO0

2 SD1\_CMD

16~255 See Matrix index Table Others Reserved

29 13 K8 PA16 I/O 0 GPIO\_A16

1 MPI3\_CLK

2 SD1\_DIO0

16~255 See Matrix index Table Others Reserved

28 12 J8 PA17 I/O 0 GPIO\_A17

1 MPI3\_DIO3

2 SD1\_DIO1

16~255 See Matrix index Table Others Reserved 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 44/74

<!-- Image omitted -->


<!-- Page 55 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-2: 大核域 GPIO（PA）管脚列表（续）

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

27 11 H8 PA18 I/O 0 GPIO\_A18

1 SWCLK

16~255 See Matrix index Table Others Reserved

26 10 H7 PA19 I/O 0 GPIO\_A19

1 SWDIO

16~255 See Matrix index Table Others Reserved

25 9 J7 PA20 I/O 0 GPIO\_A20

1 SWO

16~255 See Matrix index Table Others Reserved

24 8 G7 PA21 I/O 0 GPIO\_A21

16~255 See Matrix index Table Others Reserved

11 74 D1 PA22 I/O 0 GPIO\_A22

2 SD1\_DIO2

3 SD2\_DIO2

15 #XTAL32K\_XI

16~255 See Matrix index Table Others Reserved

10 73 D2 PA23 I/O 0 GPIO\_A23

2 SD1\_DIO3

3 SD2\_DIO3

15 #XTAL32K\_XO

16~255 See Matrix index Table Others Reserved

9 72 G5 PA24 I/O 0 GPIO\_A24

1 SPI1\_DIO

2 SD1\_CLK

3 SD2\_CLK

4 RTC\_CLK

5 LPTIM2\_OUT

6 LPTIM2\_OUTB

16~255 See Matrix index Table Others Reserved

8 71 G4 PA25 I/O 0 GPIO\_A25

1 SPI1\_DI

2 SD1\_CMD

3 SD2\_CMD

4 RTC\_CLK

5 LPTIM2\_OUT

6 LPTIM2\_OUTB

15 #XTAL32K\_EXT

16~255 See Matrix index Table Others Reserved 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 45/74

<!-- Image omitted -->


<!-- Page 56 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-2: 大核域 GPIO（PA）管脚列表（续）

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

7 70 F4 PA26 I/O 0 GPIO\_A26

2 SD1\_DIO0

3 SD2\_DIO0

4 RTC\_CLK

5 LPTIM2\_OUT

6 LPTIM2\_OUTB

16~255 See Matrix index Table Others Reserved

6 69 E3 PA27 I/O 0 GPIO\_A27

2 SD1\_DIO1

3 SD2\_DIO1

4 RTC\_CLK

5 LPTIM2\_OUT

6 LPTIM2\_OUTB

16~255 See Matrix index Table Others Reserved

5 68 C2 PA28 I/O 0 GPIO\_A28

1 SPI1\_CLK

3 SD2\_DIO2

15 #GPADC\_CH0

16~255 See Matrix index Table Others Reserved

4 67 D3 PA29 I/O 0 GPIO\_A29

1 SPI1\_CS

3 SD2\_DIO3

15 #GPADC\_CH1

16~255 See Matrix index Table Others Reserved

3 66 B2 PA30 I/O 0 GPIO\_A30

3 SD2\_CLK

4 #EFUSE\_PWR

15 #GPADC\_CH2

16~255 See Matrix index Table Others Reserved

2 65 B1 PA31 I/O 0 GPIO\_A31

3 SD2\_CMD

15 #GPADC\_CH3

16~255 See Matrix index Table Others Reserved

1 64 A1 PA32 I/O 0 GPIO\_A32

3 SD2\_DIO0

15 #GPADC\_CH4

16~255 See Matrix index Table Others Reserved 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 46/74

<!-- Image omitted -->


<!-- Page 57 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-2: 大核域 GPIO（PA）管脚列表（续）

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

67 62 D4 PA33 I/O 0 GPIO\_A33

3 SD2\_DIO1

5 #WKUP\_PIN

15 #GPADC\_CH5

16~255 See Matrix index Table Others Reserved

66 61 A3 PA34 I/O 0 GPIO\_A34

5 #WKUP\_PIN

15 #GPADC\_CH6

16~255 See Matrix index Table Others Reserved

65 60 B3 PA35 I/O 0 GPIO\_A35

4 #USB\_DP

5 #WKUP\_PIN

15 #GPADC\_CH7

16~255 See Matrix index Table Others Reserved

64 59 B4 PA36 I/O 0 GPIO\_A36

4 #USB\_DM

5 #WKUP\_PIN

15 #GPADC\_CH8

16~255 See Matrix index Table Others Reserved

63 58 C4 PA37 I/O 0 GPIO\_A37

1 SPI2\_DIO

2 SD1\_DIO2

5 #WKUP\_PIN

15 #GPADC\_CH9

16~255 See Matrix index Table Others Reserved

62 57 C5 PA38 I/O 0 GPIO\_A38

1 SPI2\_DI

2 SD1\_DIO3

5 #WKUP\_PIN

15 #GPADC\_CH10

16~255 See Matrix index Table Others Reserved

61 56 B5 PA39 I/O 0 GPIO\_A39

1 SPI2\_CLK

2 SD1\_CLK

5 #WKUP\_PIN

16~255 See Matrix index Table Others Reserved 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 47/74

<!-- Image omitted -->


<!-- Page 58 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-2: 大核域 GPIO（PA）管脚列表（续）

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

60 55 A5 PA40 I/O 0 GPIO\_A40

1 SPI2\_CS

2 SD1\_CMD

5 #WKUP\_PIN

16~255 See Matrix index Table Others Reserved

59 54 D5 PA41 I/O 0 GPIO\_A41

2 SD1\_DIO0

5 #WKUP\_PIN

16~255 See Matrix index Table Others Reserved

58 53 D6 PA42 I/O 0 GPIO\_A42

2 SD1\_DIO1

5 #WKUP\_PIN

16~255 See Matrix index Table Others Reserved

57 52 B6 PA43 I/O 0 GPIO\_A43

1 SWDIO

16~255 See Matrix index Table Others Reserved

56 51 A6 PA44 I/O 0 GPIO\_A44

1 SWCLK

16~255 See Matrix index Table Others Reserved

- 50 D7 PA45 I/O

0 GPIO\_A45

3 SD2\_DIO2

16~255 See Matrix index Table Others Reserved

- 49 C7 PA46 I/O

0 GPIO\_A46

3 SD2\_DIO3

16~255 See Matrix index Table Others Reserved

- 48 B7 PA47 I/O

0 GPIO\_A47

3 SD2\_CLK

16~255 See Matrix index Table Others Reserved

- 47 D8 PA48 I/O

0 GPIO\_A48

3 SD2\_CMD

16~255 See Matrix index Table Others Reserved

- 46 C8 PA49 I/O

0 GPIO\_A49

3 SD2\_DIO0

16~255 See Matrix index Table Others Reserved 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 48/74

<!-- Image omitted -->


<!-- Page 59 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-2: 大核域 GPIO（PA）管脚列表（续）

Pin Number Pin Name Type Sel # Function 57XUxxx6 (QFN68) 57XYxxx6 (QFN80) 57XVxxx6 (BGA112)

- 45 B8 PA50 I/O

0 GPIO\_A50

3 SD2\_DIO1

16~255 See Matrix index Table Others Reserved

- 40 B13 PA51 I/O

0 GPIO\_A51

16~255 See Matrix index Table Others Reserved

- 39 C11 PA52 I/O

0 GPIO\_A52

16~255 See Matrix index Table Others Reserved

- 38 D9 PA53 I/O

0 GPIO\_A53

16~255 See Matrix index Table Others Reserved

- 37 E9 PA54 I/O

0 GPIO\_A54

16~255 See Matrix index Table Others Reserved

41 25 G12 PA55 I/O 0 GPIO\_A55

15 #ADC1P

16~255 See Matrix index Table Others Reserved

40 24 F11 PA56 I/O 0 GPIO\_A56

15 #MICBIAS

16~255 See Matrix index Table Others Reserved

- 27 F10 PA57 I/O

0 GPIO\_A57

16~255 See Matrix index Table Others Reserved

- - J9 SB01 I/O

4 MPI3\_DIO2

Others Reserved

- - K9 SB02 I/O

4 MPI3\_DIO1

Others Reserved

- - H10 SB03 I/O

4 MPI3\_CS

Others Reserved

- - J10 SB07 I/O

4 MPI3\_DIO0

Others Reserved

- - J11 SB08 I/O

4 MPI3\_DIO3

Others Reserved

- - K11 SB09 I/O

4 MPI3\_CLK

Others Reserved

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 49/74

<!-- Image omitted -->


<!-- Page 60 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 5.2.2 专用管脚（电源、射频、模拟、数字）列表

### 表 5-3: 专用管脚（电源、射频、模拟、数字）列表

Pin Number Pin Name Type Description 57XUxxx6 (QFN68)

21 VDDIOA PWR GPIO 电源输入，接 1.8V 或 3.3V 电源

19 PVDD PWR PMU 电源输入, 接 3.3V 电源

18 BUCK\_LX A,I/O Buck 电感开关, 接电感一端

17 BUCK\_FB PWR Buck 输出, 接电感另一端并接电容到地

16 VDD\_VOUT1 PWR 内部 LDO1 输出, 内部电源, 接电容到地

15 VDD\_VOUT2 PWR 内部 LDO2 输出, 内部电源, 接电容到地

13 VDD\_RET PWR RET LDO 输出, 内部电源, 接电容到地

12 VDD\_RTC PWR RTC LDO 输出, 内部电源, 接电容到地

23 VDD18\_VOUT PWR 1.8V LDO 输出, 内部电源, 接电容到地

14 AVDD33 PWR 模拟电源输入, 接 3.3V 电源

68 GPADC\_VREF A,I GPADC VREF 输入, 接电容到地, 不用外供电源

55 XTAL48M\_XI A,I 48MHz 晶体接口

54 XTAL48M\_XO A,O 48MHz 晶体接口

53 BRF\_ANT A,I/O 天线接口

52 AVDD\_BRF PWR 射频电源输入, 接 3.3V 电源

39 AUD\_VREF A,I 音频参考电压输入, 接电容到地, 不用外供电源

38 AVDD33\_AUD PWR 音频电源输入, 接 3.3V 电源

37 DACP A,O 音频 DAC 输出

36 DACN A,O 音频 DAC 输出

22 NC - 无信号

69 EPAD GND 地

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 50/74

<!-- Image omitted -->


<!-- Page 61 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 表 5-4: 专用管脚（电源、射频、模拟、数字）列表

Pin Number Pin Name Type Description 57XYxxx6 (QFN80)

4 VDDIOA PWR GPIO 电源输入，接 1.8V 或 3.3V 电源

2 NC - 悬空

1 BUCK\_LX A,I/O Buck 电感开关, 接电感一端

80 BUCK\_FB PWR Buck 输出, 接电感另一端并接电容到地

79 VDD\_VOUT1 PWR 内部 LDO1 输出, 内部电源, 接电容到地

78 VDD\_VOUT2 PWR 内部 LDO2 输出, 内部电源, 接电容到地

76 VDD\_RET PWR RET LDO 输出, 内部电源, 接电容到地

75 VDD\_RTC PWR RTC LDO 输出, 内部电源, 接电容到地

3 PVDD PWR PMU 电源输入, 接 3.3V 电源

6 VDDIOSB PWR SIP SB IO 电源输入 \*

7 VDD18\_VOUT PWR 1.8V LDO 输出, 内部电源, 接电容到地

77 AVDD33 PWR 模拟电源输入, 接 3.3V 电源

63 GPADC\_VREF A,I GPADC VREF 输入, 接电容到地, 不用外供电源

44 XTAL48M\_XI A,I 48MHz 晶体接口

43 XTAL48M\_XO A,O 48MHz 晶体接口

42 BRF\_ANT A,I/O 天线接口

41 AVDD\_BRF PWR 射频电源输入, 电池供电时和 VSYS 接一起; 常规 3.3V 供电时接 3.3V 电源

23 AUD\_VREF A,I 音频参考电压输入, 接电容到地, 不用外供电源

22 AVDD33\_AUD PWR 音频电源输入, 接 3.3V 电源

21 DACP A,O 音频 DAC 输出

20 DACN A,O 音频 DAC 输出

81 EPAD GND 地

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 51/74

<!-- Image omitted -->


<!-- Page 62 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 表 5-5: 专用管脚（电源、射频、模拟、数字）列表

Pin Number Pin Name Type Description 57XVxxx6 (BGA112)

K5 NC - 悬空

J3 VSS GND 地

J1 BUCK\_LX A,I/O Buck 电感开关, 接电感一端

H1 BUCK\_FB PWR Buck 输出, 接电感另一端并接电容到地

G2 VDD\_VOUT1 PWR 内部 LDO1 输出, 内部电源, 接电容到地

G1 VDD\_VOUT2 PWR 内部 LDO2 输出, 内部电源, 接电容到地

E2 VDD\_RET PWR RET LDO 输出, 内部电源, 接电容到地

E1 VDD\_RTC PWR RTC LDO 输出, 内部电源, 接电容到地

K6 VDD18\_VOUT PWR 1.8V LDO 输出, 内部电源, 接电容到地

H5 NC - 悬空

G3 NC - 悬空

K3 PVDD\_PMU PWR PMU 电源输入, 接 3.3V 电源

K2 PVDD\_PMU PWR PMU 电源输入, 接 3.3V 电源

H2 PVSS\_PMU GND PMU 电源地

K1 AVSS\_PMU GND PMU 模拟电源地

H4 VDDIOA PWR GPIO 电源输入，接 1.8V 或 3.3V 电源

J6 VDDIOSA PWR SIP SA IO 电源输入 \*

J5 VDDIOSB PWR SIP SB IO 电源输入 \*

F2 AVDD33\_ANA A,I 模拟电源输入, 接 3.3V 电源

E5 AVSS33\_ANA GND 模拟地

A2 GPADC\_VREFP A,I GPADC VREF 输入, 接电容到地, 不用外供电源

E4 GPADC\_VREFN A,I GPADC VREF 输入, 接地

A8 XTAL48M\_XI A,I 48MHz 晶体接口

A9 XTAL48M\_XO A,O 48MHz 晶体接口

A11 BRF\_ANT A,I/O 天线接口

A13 AVDD\_BRF PWR 射频电源输入, 电池供电时和 VSYS 接一起; 常规 3.3V 供电时接 3.3V 电源

B10 AVSS\_BB GND 射频地

B9 AVSS\_LO GND 射频地

B11 AVSS\_RRF GND 射频地

B12 AVSS\_TRF GND 射频地

A12 AVSS\_TRF2 GND 射频地

G13 AUD\_VREF A,I 音频参考电压输入, 接电容到地, 不用外供电源

F9 AUD\_VREF\_GND GND 音频地

H13 AVDD33\_AUD PWR 音频电源输入, 接 3.3V 电源

G10 AVSS33\_AUD GND 音频地

J13 DACP A,O 音频 DAC 输出

K13 DACN A,O 音频 DAC 输出

E6 VSS GND 地

E7 VSS GND 地

E8 VSS GND 地

F5 VSS GND 地

F6 VSS GND 地

F7 VSS GND 地

F8 VSS GND 地

G6 VSS GND 地

H3 VSS GND 地 续表下页...

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 52/74

<!-- Image omitted -->


<!-- Page 63 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-5: 专用管脚（电源、射频、模拟、数字）列表（BGA112L 封装）（续）

Pin Number Pin Name Type Description 57XVxxx6 (BGA112) J2 VSS GND 地

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 53/74

<!-- Image omitted -->


<!-- Page 64 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 表 5-6: Matrix Index Table

func\_sel func func\_sel func func\_sel func func\_sel func

16 I2C1\_SCL 56 GPTIM1\_CH3 96 I2S1\_SDO 136 LCDC\_IF15

17 I2C1\_SDA 57 GPTIM1\_CH4 97 I2S1\_SDI 137 LCDC\_IF16

18 I2C2\_SCL 58 GPTIM1\_ETR 98 I2S1\_BCK 138 LCDC\_IF17

19 I2C2\_SDA 59 GPTIM2\_CH1 99 I2S1\_LRCK 139 LCDC\_IF18

20 I2C3\_SCL 60 GPTIM2\_CH2 100 I2S2\_MCLK 140 LCDC\_IF19

21 I2C3\_SDA 61 GPTIM2\_CH3 101 I2S2\_SDO 141 LCDC\_IF20

22 I2C4\_SCL 62 GPTIM2\_CH4 102 I2S2\_SDI 142 LCDC\_IF21

23 I2C4\_SDA 63 GPTIM2\_ETR 103 I2S2\_BCK 143 LCDC\_IF22

24 SPI1\_CLK 64 LPTIM1\_IN 104 I2S2\_LRCK 144 LCDC\_IF23

25 SPI1\_CS 65 LPTIM1\_OUT 105 PTM1\_CH0 145 LCDC\_IF24

26 SPI1\_DI 66 LPTIM1\_ETR 106 PTM1\_CH1 146 LCDC\_IF25

27 SPI1\_DIO 67 LPTIM2\_IN 107 PTM1\_CH2 147 LCDC\_IF26

28 SPI2\_CLK 68 LPTIM2\_OUT 108 PTM1\_CH3 148 LCDC\_IF27

29 SPI2\_CS 69 LPTIM2\_ETR 109 PTM1\_CH4 149 LCDC\_IF28

30 SPI2\_DI 70 ATIM1\_CH1 110 PTM1\_CH5 150 LCDC\_IF29

31 SPI2\_DIO 71 ATIM1\_CH2 111 PTM1\_CH6 151 PDM1\_CLK

32 UART1\_TXD 72 ATIM1\_CH3 112 PTM1\_CH7 152 PDM2\_CLK

33 UART1\_RXD 73 ATIM1\_CH4 113 PTM1\_CH8 153 PDM1\_DATA

34 UART1\_RTS 74 ATIM1\_ETR 114 PTM1\_CH9 154 PDM1\_DATA2

35 UART1\_CTS 75 ATIM1\_CH1N 115 PTM1\_CH10 155 PDM2\_DATA

36 UART2\_TXD 76 ATIM1\_CH2N 116 PTM1\_CH11 156 PDM2\_DATA2

37 UART2\_RXD 77 ATIM1\_CH3N 117 PTM1\_CH12 157 DCMI\_CLK

38 UART2\_RTS 78 ATIM1\_BK 118 PTM1\_CH13 158 DCMI\_HSYNC

39 UART2\_CTS 79 ATIM1\_BK2 119 PTM1\_CH14 159 DCMI\_VSYNC

40 UART3\_TXD 80 ATIM2\_CH1 120 PTM1\_CH15 160 DCMI\_DI0

41 UART3\_RXD 81 ATIM2\_CH2 121 LCDC\_IF0 161 DCMI\_DI1

42 UART3\_RTS 82 ATIM2\_CH3 122 LCDC\_IF1 162 DCMI\_DI2

43 UART3\_CTS 83 ATIM2\_CH4 123 LCDC\_IF2 163 DCMI\_DI3

44 CAN1\_TXD 84 ATIM2\_ETR 124 LCDC\_IF3 164 DCMI\_DI4

45 CAN1\_RXD 85 ATIM2\_CH1N 125 LCDC\_IF4 165 DCMI\_DI5

46 CAN2\_TXD 86 ATIM2\_CH2N 126 LCDC\_IF5 166 DCMI\_DI6

47 CAN2\_RXD 87 ATIM2\_CH3N 127 LCDC\_IF6 167 DCMI\_DI7

48 BT\_ACTIVE 88 ATIM2\_BK 128 LCDC\_IF7 168 DCMI\_DI8

49 BT\_PRIORITY 89 ATIM2\_BK2 129 LCDC\_IF8 169 DCMI\_DI9

50 BT\_COLLISION 90 PWM1\_CH1 130 LCDC\_IF9 170 DCMI\_DI10

51 WLAN\_ACTIVE 91 PWM1\_CH2 131 LCDC\_IF10 171 DCMI\_DI11

52 BT\_TX 92 PWM1\_CH3 132 LCDC\_IF11 172 DCMI\_DI12

53 BT\_RX 93 PWM1\_CH4 133 LCDC\_IF12 173 DCMI\_DI13

54 GPTIM1\_CH1 94 PWM1\_CH5 134 LCDC\_IF13 174 DCMI\_DI14

55 GPTIM1\_CH2 95 I2S1\_MCLK 135 LCDC\_IF14 175 DCMI\_DI15

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 54/74

<!-- Image omitted -->


<!-- Page 65 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

### 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 表 5-7: LCDC mapping table

SPI JDI EPD DBI DPI

LCDC\_IF0 LCDC\_SPI\_TE LCDC\_TCON\_GDSP LCDC\_8080\_TE LCDC\_DPI\_CLK

LCDC\_IF1 LCDC\_SPI\_RSTB LCDC\_TCON\_GDCLK LCDC\_8080\_RSTB LCDC\_DPI\_HSYNC

LCDC\_IF2 LCDC\_SPI\_CS LCDC\_TCON\_SDCLK LCDC\_8080\_CS LCDC\_DPI\_VSYNC

LCDC\_IF3 LCDC\_SPI\_CLK LCDC\_TCON\_SDSTL LCDC\_8080\_WR LCDC\_DPI\_DE

LCDC\_IF4 LCDC\_TCON\_SDLE LCDC\_8080\_RD LCDC\_DPI\_SD

LCDC\_IF5 LCDC\_TCON\_SDOE LCDC\_8080\_DC LCDC\_DPI\_CM

LCDC\_IF6 LCDC\_SPI\_DIO0 LCDC\_TCON\_SDAT0 LCDC\_8080\_DIO0 LCDC\_DPI\_R0

LCDC\_IF7 LCDC\_SPI\_DIO1 LCDC\_TCON\_SDAT1 LCDC\_8080\_DIO1 LCDC\_DPI\_R1

LCDC\_IF8 LCDC\_SPI\_DIO2 LCDC\_TCON\_SDAT2 LCDC\_8080\_DIO2 LCDC\_DPI\_R2

LCDC\_IF9 LCDC\_SPI\_DIO3 LCDC\_TCON\_SDAT3 LCDC\_8080\_DIO3 LCDC\_DPI\_R3

LCDC\_IF10 LCDC\_JDI\_SCS LCDC\_TCON\_SDAT4 LCDC\_8080\_DIO4 LCDC\_DPI\_R4

LCDC\_IF11 LCDC\_JDI\_SCLK LCDC\_TCON\_SDAT5 LCDC\_8080\_DIO5 LCDC\_DPI\_R5

LCDC\_IF12 LCDC\_JDI\_SO LCDC\_TCON\_SDAT6 LCDC\_8080\_DIO6 LCDC\_DPI\_R6

LCDC\_IF13 LCDC\_JDI\_DISP LCDC\_TCON\_SDAT7 LCDC\_8080\_DIO7 LCDC\_DPI\_R7

LCDC\_IF14 LCDC\_JDI\_EXTCOMIN LCDC\_TCON\_SDAT8 LCDC\_8080\_DIO8 LCDC\_DPI\_G0

LCDC\_IF15 LCDC\_TCON\_SDAT9 LCDC\_8080\_DIO9 LCDC\_DPI\_G1

LCDC\_IF16 LCDC\_TCON\_SDAT10 LCDC\_8080\_DIO10 LCDC\_DPI\_G2

LCDC\_IF17 LCDC\_TCON\_SDAT11 LCDC\_8080\_DIO11 LCDC\_DPI\_G3

LCDC\_IF18 LCDC\_JDI\_VCK LCDC\_TCON\_SDAT12 LCDC\_8080\_DIO12 LCDC\_DPI\_G4

LCDC\_IF19 LCDC\_JDI\_XRST LCDC\_TCON\_SDAT13 LCDC\_8080\_DIO13 LCDC\_DPI\_G5

LCDC\_IF20 LCDC\_JDI\_HCK LCDC\_TCON\_SDAT14 LCDC\_8080\_DIO14 LCDC\_DPI\_G6

LCDC\_IF21 LCDC\_JDI\_HST LCDC\_TCON\_SDAT15 LCDC\_8080\_DIO15 LCDC\_DPI\_G7

LCDC\_IF22 LCDC\_JDI\_ENB LCDC\_8080\_RWN LCDC\_DPI\_B0

LCDC\_IF23 LCDC\_JDI\_VST LCDC\_8080\_CS2 LCDC\_DPI\_B1

LCDC\_IF24 LCDC\_JDI\_R1 LCDC\_DPI\_B2

LCDC\_IF25 LCDC\_JDI\_G1 LCDC\_DPI\_B3

LCDC\_IF26 LCDC\_JDI\_B1 LCDC\_DPI\_B4

LCDC\_IF27 LCDC\_JDI\_R2 LCDC\_DPI\_B5

LCDC\_IF28 LCDC\_JDI\_G2 LCDC\_DPI\_B6

LCDC\_IF29 LCDC\_JDI\_B2 LCDC\_DPI\_B7

### DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 55/74

<!-- Image omitted -->


<!-- Page 66 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

### 5.3 封装尺寸

5.3.1 QFN68 封装尺寸

- 

图 5-4: QFN68-7x7-T0.9-P0.35 封装尺寸

- 

图 5-5: QFN68-7x7-T1.1-P0.35 封装尺寸

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 56/74

<!-- Image omitted -->


<!-- Page 67 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 5-6: QFN68-7x7 的 PCB 封装制作要求（第一部分）

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 57/74

<!-- Image omitted -->


<!-- Page 68 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 5-7: QFN68-7x7 的 PCB 封装制作要求（第二部分）

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 58/74

<!-- Image omitted -->


<!-- Page 69 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

5.3.2 QFN80 封装尺寸

图 5-8: QFN80-8x8-T0.9-P0.35 封装尺寸

图 5-9: QFN80-8x8-T1.1-P0.35 封装尺寸

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 59/74

<!-- Image omitted -->


<!-- Page 70 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 5-10: QFN80-8x8 的 PCB 封装制作要求（第一部分）

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 60/74

<!-- Image omitted -->


<!-- Page 71 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 5-11: QFN80-8x8 的 PCB 封装制作要求（第二部分）

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 61/74

<!-- Image omitted -->


<!-- Page 72 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

5.3.3 BGA112 封装尺寸

图 5-12: BGA112-5.4x4.4-T0.98-P0.4 封装尺寸

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 62/74

<!-- Image omitted -->


<!-- Page 73 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 5-13: BGA112 的 PCB 封装制作要求（第一部分）

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 63/74

<!-- Image omitted -->


<!-- Page 74 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

图 5-14: BGA112 的 PCB 封装制作要求（第二部分）

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 64/74

<!-- Image omitted -->


<!-- Page 75 -->

# SF32LB57X

### 超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark

高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

## 5.4 分级回流焊

图 5-15: 分级回流焊

表 5-8: 分级回流焊对照表

项目 锡-铅工艺 无铅工艺

平均升温速率 (Tsmax to Tp) 最大 3

- C/秒 最大 3

- C/秒

预热初温 (Tsmin) 100◦C 150◦C

预热目标温度 (Tsmax) 100◦C 200◦C

预热时长 (Tsmin to Tsmax) 60-120 秒 60-180 秒

恒温温度 (TL) 183◦C 217◦C

恒温时长 (tL) 60-150 秒 60-150 秒

尖峰温度（Tp） 225+0/-5◦C 240+0/-5◦C

尖峰温度时长（tp） 10-30 秒 20-40 秒

降温速率 最大 6

- C/秒 最大 6

- C/秒

25◦C 到尖峰温度时长 (t) 最多 6 分钟 最多 8 分钟

表 5-9: 无铅工艺峰值回流温度对照表

封装厚度 体积（mm3）<350 体积（mm3）≥350

<2.5mm 240 + 0/-5◦C 225 + 0/-5◦C ≥2.5mm 225 + 0/-5◦C 225 + 0/-5◦C

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 65/74

<!-- Image omitted -->


<!-- Page 76 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 5-10: 无铅工艺分级回流温度对照表

封装厚度 体积（mm3）<350 体积（mm3）350-2000 体积（mm3）>2000

<1.6mm 260 + 0

- C 260 + 0

- C 260 + 0

- C

1.6mm–2.5mm 260 + 0

- C 250 + 0

- C 245 + 0

- C

≥2.5mm 250 + 0

- C 245 + 0

- C 245 + 0

- C

## 5.5 硬件最小系统

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 66/74

<!-- Image omitted -->


<!-- Page 77 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

5.5.1 QFN68 硬件最小系统

1 2 3 4 1 1 2 2

1 1 2 2 EPAD 9

图 5-16: SF32LB57XUxxx6 常规 3.3V 供电最小系统原理图

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 67/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 78 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 68/74

<!-- Image omitted -->


<!-- Page 79 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

5.5.2 QFN80 硬件最小系统

1 2 3 4 1 1 2 2

1 1 2 2 EPAD 9

图 5-17: SF32LB57XYxxx6 常规 3.3V 供电最小系统原理图

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 69/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 80 -->

## SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 70/74

<!-- Image omitted -->


<!-- Page 81 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

5.5.3 BGA112 硬件最小系统

1 2 3 4 1 1 2 2

1 1 2 2 EPAD 9

图 5-18: SF32LB57XVxxx6 常规 3.3V 供电最小系统原理图

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 71/74

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 82 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 6 订购信息

图 6-1: SF32LB57X ordering information scheme

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 72/74

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->

<!-- Image omitted -->


<!-- Page 83 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

表 6-1: 订购信息

料号 封装 合封规格 供电要求 每盘数量 (PCS)

SF32LB57ZUN3N6 QFN68: 7×7-T0.9-P0.35 8Mb NOR Flash 3.3V 供电 3000

SF32BPRTUN3N6 QFN68: 7×7-T0.9-P0.35 8Mb NOR Flash 3.3V 供电 3000

SF32LB57EYBBN6 QFN80: 8×8-T0.9-P0.35 32Mb×2 OPI-PSRAM 3.3V 供电 3000

SF32LB57GYD7N6 QFN80: 8×8-T0.9-P0.35 128Mb OPI-PSRAM +

128Mb NOR Flash 3.3V 供电 3000

SF32LB57GVD776 BGA112: 5.4x4.4-T1.03-P0.4 128Mb OPI-PSRAM +

128Mb NOR Flash +

128Mb NOR Flash 3.3V 供电 3000

\* Txx 代表 xxmm 厚度，Pxx 代表 xxmm 管脚间距

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 73/74

<!-- Image omitted -->


<!-- Page 84 -->

# SF32LB57X

超低功耗多核双系统 Arm Cortex‐M33 STAR‐MC1@240MHz/24MHz，1968 CoreMark 高性能 2D/2.5D 引擎，592KB 内存，BT/BLE 6.3

# 相关资源

免责声明和版权公告

思澈科技（南京）有限公司保留随时对产品和/或本文档进行更正、修改、改进和其他变更的权利，包括其中的信 息、参数、链接、URL 地址等。如有变更，恕不另行通知。

思澈科技（南京）有限公司在此未授予任何知识产权的明示或暗示的许可。

SiFli 和 SiFli 标识是思澈科技（南京）有限公司的商标。本文档中出现的所有其他商标、服务标记、商品名称、产品 名称和标识均属其各自所有者的财产。

零售样品与小批量可直接在淘宝（sifli.taobao.com）购买 企业大批采购客户可发邮件到 sales@sifli.com 更多产品和文档可在 wiki.sifli.com 获取 遇到问题可在 bbs.sifli.com 进行讨论 开源项目免费申请样品，可加入 QQ 群 674699679 进行交流

欢迎关注我们公众号获取最新产品信息

地址：江苏自贸区南京片区浦滨路 320 号科创总部大厦 B 座 419-13 200131 邮箱：sales@sifli.com

©2026 思澈科技（南京）有限公司。保留所有权利。

DS5702‐SF32LB57X‐CN V0.2.2

©2026 思澈科技（南京）有限公司 www.sifli.com 74/74

<!-- Image omitted -->

<!-- Image omitted -->

