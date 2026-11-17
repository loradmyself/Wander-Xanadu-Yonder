# SF32LB57GYD7N6 硬件能力评估

> 依据：`datasheet/` 下三份文档（DS5702 V0.2.2 芯片技术规格书、PB5702 V0.4 产品简介、UM5701 V0.1 用户手册）
> 目的：判断这颗芯片能支撑 [FEATURES.md](FEATURES.md) 里的哪些功能，哪些必须外挂，IO 够不够
> 状态：v0.1 初评（2026-11-17）· 未经实物验证的条目已标注

## 一、型号解码（来自 DS 表 6-1 订购信息）

| 字段 | 值 | 含义 |
|---|---|---|
| SF32LB57 | 系列 | Cortex-M33 双核 + 2.5D GPU + BT/BLE |
| G | 供电/版本 | **常规供电版本**（57E/57G/57Z），非锂电池直供版本（573/575/577） |
| Y | 封装 | **QFN80，8×8×0.9mm，58 GPIO** |
| D7 | 合封 | **128Mb OPI-PSRAM（16MB）+ 128Mb NOR Flash（16MB）** |
| N6 | 温度/批次 | 常规 |

关键含义：**内置 16MB PSRAM + 16MB Flash**，不需要外挂存储就能跑 LVGL 双缓冲 + 应用包 + 表情资源。这是选它的最大理由。

## 二、片上资源清单

| 类别 | 资源 | 数量/规格 | 对平台的意义 |
|---|---|---|---|
| 算力 | HCPU + ACPU | 2× STAR-MC1 @240MHz，1968 CoreMark，FPU/MPU | HCPU 跑 UI/会话，ACPU 卸载解码与渲染 |
| 算力 | LCPU | STAR-MC1 @24MHz，80KB SRAM | BT 协议栈、低功耗传感轮询 |
| 内存 | SRAM | 512KB（HPSYS，全 Retention）+ 80KB（LPSYS） | 流式文本 + LVGL 缓冲够用，大图走 PSRAM |
| 内存 | SiP | 16MB OPI-PSRAM @156MHz + 16MB QSPI-NOR @104MHz | 应用包、表情、音效、模型文件都有地方放 |
| 图形 | ePicasso 3.0 | 3 前景层 + 掩膜 + 背景，旋转/缩放/投影/镜像，最大 2048×2048 | **表情动画、表盘旋转、卡片转场的硬件加速** |
| 图形 | eZip 3.0 | eZip 无损 + **JPEG/MJPEG 解码** | 表情序列帧、JPEG 壁纸省 PSRAM 与带宽（只解不编） |
| 显示 | LCDC | SPI/Dual/Quad-SPI、8080(8/16/24b)、DPI RGB-24b、JDI、EPD 8/16b | 屏型选择宽；**无 MIPI-DSI PHY** |
| 摄像头 | DCMI | DVP 8/10/12/14/16bit；SPI 1/2/4 线；裁剪 | 能接摄像头，**无 ISP、无 JPEG 编码、无缩放** |
| 音频 | DAC | 1× 24bit，109dB SNR，差分输出 | 直接推耳机或经 PA 推喇叭 |
| 音频 | ADC | 2× 24bit，97dB SNR，8–96kHz | **双麦可同时采**（AEC/波束） |
| 音频 | PDM | 2 路（可立体声），3.072MHz 时钟 | 数字麦方案，省模拟脚 |
| 音频 | I2S | 2 路，主从/全双工/TDM | 接外部 codec 或数字麦阵列 |
| 音频 | AUDPRC | 4 路混音 + 双路 SRC + 16 段 EQ | TTS/提示音/音乐混音，采样率不用软件重采样 |
| 无线 | BT | 双模 6.3，LE Audio、LE LR，17dBm(BR/BLE)，-100dBm | 连耳机/手机/传感器；**无 WiFi** |
| USB | USBC | 1× USB2.0 **FS**，Host/Device，片内 PHY，ep0–7 | 能 Host 也能 Device，但只有 12Mbps |
| 存储接口 | MPI | MPI1/2 已被 SiP 占用；**MPI3 可用**（104MHz，NOR/NAND） | 想扩 Flash 有路 |
| 存储接口 | SDMMC | 2×，SD3.0/SDIO3.0/eMMC4.51，SDR 单线/4 线，48MHz | **一张 TF 卡 + 一路 SDIO WiFi，正好** |
| 模拟 | GPADC | 12bit，11 通道，最高 3.7Msps，单端/差分 | 电量、按键梯、光感、温度 |
| 模拟 | 温度 | 片内 TSEN | 电池/整机温度保护 |
| 定时器 | GPTIM×2(4ch) / BTIM×4 / ATIM×2(6ch，带死区互补) / LPTIM×2 / PWM1×**5ch** / RTC / 2×WDT + IWDT | 22 路以上 PWM 能力 | 背光、马达、振动、蜂鸣、相机 MCLK 都够 |
| DMA | GPDMA | **8 通道**，链表模式，128 个外设请求可选 | 屏 + 相机 + 音频 + SD 并发时偏紧，需排优先级 |
| 安全 | AES/HASH/CRC/TRNG + eShield + XIP 实时解密 + PSA L1 | — | 应用包签名、密钥不入 RAM、Flash 加密存储 |
| 电源 | Buck + LDO | hibernate **1µA@3.3V**，PVDD 支持 1.8–3.3V | 待机指标优秀 |
| 电源 | VBATS | 专用电池电压测量脚（内部分压） | 电量检测**不占 GPIO** |
| 电源 | 充电 | **本型号无充电器**（573/575/577 才有，400mA 线性） | 必须外挂 Charger IC |

## 三、IO 能力（这是最需要算清楚的部分）

### 3.1 三种引脚，规则完全不同

| 类型 | 机制 | 引脚 | 约束 |
|---|---|---|---|
| **专用功能** | FSEL = 1–15，**不过 IO 矩阵** | 见 3.2 | 高速/严格时序必须用，如 LCDC-SPI、SD1/SD2、USB、MPI3 |
| **通用矩阵功能** | FSEL > 15，过 IO 矩阵 | PA00–PA57 全部 | **任一信号可映射到任一通用 IO**（UM 5.3 原文）；延时大，适合 I2C/UART/PDM/PWM/DCMI 等 |
| **模拟/固定** | 物理绑定 | USB DP/DM = PA35/PA36；ADC1 = PA55/PA09；ADC2 = PA10/PA11；MICBIAS = PA56；GPADC = PA28–PA38（11 路）；32.768k 晶振 = PA22/PA23/PA25；SWD = PA43/PA44；DAC = 独立模拟脚 | **不可换脚** |

矩阵里可选的信号共 160 个（func_sel 16–175），含 I2C×4、SPI×2、UART×3、CAN×2、I2S×2、PDM×2、PTM×16、ATIM/GPTIM/LPTIM/PWM、LCDC_IF0–29、DCMI 全信号。

### 3.2 专用功能分布（PA00–PA57，QFN80 脚号）

| 引脚 | 专用功能 |
|---|---|
| PA00–PA01 | LCDC1_SPI_RSTB、SD2_DIO2/3 |
| PA02–PA05 | LCDC1_SPI_TE / CS / CLK / DIO0，SD2_CLK/CMD/DIO0/1 |
| PA06–PA08 | LCDC1_SPI_DIO1/2/3，SD1_DIO2/3/CLK |
| PA09–PA11 | SD1_CMD/DIO0/DIO1，**ADC1N / ADC2P / ADC2N** |
| PA12–PA17 | MPI3_CS/DIO1/2/0/CLK/DIO3，SD1_DIO2/3/CLK/CMD/DIO0/1 |
| PA18–PA20 | SWCLK / SWDIO / SWO（BOOTMODE1 时 PA18/19 被 ROM 当 UART1 用） |
| PA22–PA27 | 32.768k 晶振、SD1/SD2、RTC_CLK、LPTIM2_OUT |
| PA28–PA38 | SPI1、SPI2、SD2、**GPADC_CH0–CH10** |
| PA30 | EFUSE_PWR（复用时注意） |
| PA33–PA42 | **#WKUP_PIN（唯一的休眠唤醒脚区）**、SD1、SPI2 |
| PA35–PA36 | **USB_DP / USB_DM** |
| PA43–PA44 | SWDIO / SWCLK（默认 SWD） |
| PA45–PA50 | **SD2 专用组**（DIO2/3/CLK/CMD/DIO0/1） |
| PA55–PA57 | ADC1P、MICBIAS、GPIO |

> QFN80 没有 SB01–SB09（那 6 个 MPI3 专用脚只有 BGA112 才有），所以 QFN80 是 **58 脚全部可用**。

### 3.3 三个硬约束

1. **休眠唤醒只能用 PA33–PA42**（10 个脚）。唤醒 PIN 功能不受 FSEL 影响，但只有这一段脚支持。也就是说：电源键、按键梯必须落在这 10 个脚里；IMU 中断、触摸中断这类脚**不能从 hibernate 唤醒**，只能从 sleep/deepsleep 唤醒。
2. **高速信号必须走专用脚**。屏的 QSPI、TF 卡的 SD2、WiFi 的 SD1、USB 都只能用 FSEL≤15 的专用功能，用矩阵会牺牲时序裕量。
3. **摄像头所有信号只有矩阵通道**（DCMI_CLK/HSYNC/VSYNC/DI0–15 都只在 func_sel 157–175）。8bit DVP 要 11 个脚全部过矩阵，PCLK 上升沿采样时会有固定路由延时，**PCLK 上限必须实测**，不能照抄 24MHz。

## 四、IO 预算草案（全功能配置）

按"屏 + 双麦 + 摄像头 + TF + WiFi + USB + 按键 + 马达 + IMU + PMIC"全配，58 脚分配如下：

| 子系统 | 脚数 | 分配 | 说明 |
|---|---|---|---|
| 显示 QSPI | 8 | PA00、PA02–PA08 | RSTB/TE/CS/CLK/DIO0–3，全部走专用 FSEL=1 |
| 音频（模拟双麦） | 5 | PA09、PA10、PA11、PA55、PA56 | ADC1 + ADC2 + MICBIAS |
| 触摸/传感器 I2C | 4 | PA12–PA15 | I2C1(矩阵) + TP_INT + TP_RST |
| 摄像头 DVP 8bit | 14 | PA01、PA16–PA21、PA24–PA30 | PCLK/HSYNC/VSYNC/D0–7 + MCLK + RST + PWDN |
| RTC 晶振 | 2 | PA22、PA23 | 32.768kHz，待机走时精度 |
| 按键/马达/IMU | 4 | PA31–PA34 | IMU_INT、马达 PWM、电源键、ADC 按键梯 |
| USB | 2 | PA35、PA36 | DP/DM（+ VBUS 检测/使能另计） |
| WiFi SDIO | 6 | PA37–PA42 | SD1 4bit 专用组 |
| TF 卡 | 7 | PA45–PA51 | SD2 专用组 + 卡检测 |
| WiFi 控制 + PMIC | 4 | PA52–PA54、PA57 | WL_EN、WL_INT、CHG_INT、PMIC_PG |
| SWD | 2 | PA43、PA44 | 调试口，量产可裁 |
| **合计** | **58 / 58** | — | **余量 0** |

结论：**这颗芯片的 58 个 IO 刚好够一个"全功能"配置，一个不剩。** 想留余量必须做减法，按性价比排序：

| 减法 | 省脚 | 代价 |
|---|---|---|
| 麦改用 **PDM 数字麦**（PDM1_CLK + PDM1_DATA + PDM2_DATA） | 2 | 换 mic 型号，抗干扰更好，其实更优 |
| WiFi 用 **1-bit SDIO** | 3 | 吞吐降到 1/4，图片上传会慢 |
| 摄像头改 **SPI/QSPI 模式** | 7 | 帧率大幅下降，只能做"拍一张"而非预览 |
| 去掉摄像头 | 14 | 失去 F-AI-17 视觉能力 |
| 去掉 TF 卡，应用装内部 Flash | 7 | 16MB 要分给固件+资源+应用 |

**推荐组合：PDM 双麦（−2）+ WiFi 1-bit SDIO（−3）→ 留 5 个脚给指示灯/前光/扩展。**

## 五、逐项对照功能规格

| 功能组 | 芯片能力 | 判定 | 备注 |
|---|---|---|---|
| 系统 F-SYS | 双核 M33 + 592KB SRAM + MPU | ✅ 直接支持 | 无 MMU，应用隔离靠 MPU + 进程边界 |
| 显示 F-DISP | LCDC：QSPI / 8080 / DPI / JDI / EPD | ⚠️ 受限支持 | **QSPI 屏最合适**（约 6–8 脚）；DPI RGB565 要 20 脚、8080 要 13 脚，与摄像头抢脚；**无 MIPI-DSI**，AMOLED 只能选 QSPI/SPI 带 GRAM 的型号，或加 DBI-to-DSI 桥片 |
| 表情动画 F-DISP-05 | ePicasso 旋转/缩放/多图层 + eZip 解 MJPEG | ✅ 强项 | 表情用 MJPEG/eZip 序列帧，GPU 合成，CPU 占用低 |
| 输入 F-INP | 触摸走 I2C；按键走 ADC 梯（1 脚多键） | ✅ 支持 | 唤醒键只能放 PA33–PA42 |
| 音频 F-AUD | 1 DAC + 2 ADC + 2 PDM + 2 I2S + AUDPRC | ✅ 支持 | **AEC 无硬件**，需软件做（可拿 DAC 前数据当参考）；**无硬件 VAD**，靠软件能量+过零 |
| 无线 F-NET | BT 6.3 双模 + LE Audio | ⚠️ **缺 WiFi** | 必须外挂 SDIO WiFi 模组（建议选支持 coex 的，芯片已提供 WLAN_ACTIVE/BT_ACTIVE/PRIORITY 三条共存信号） |
| 摄像头 F-CAM | DCMI DVP 8–16bit / SPI 1–4 线，带裁剪 | ⚠️ 受限支持 | **无 ISP / 无 JPEG 编码 / 无缩放** → 必须选**自带 JPEG 输出**的 sensor（OV2640/GC0308 类），否则 YUV422 一帧 600KB 会拖死总线；全部信号走矩阵，PCLK 上限待实测 |
| USB F-USB | 1× FS Host/Device + 片内 PHY | ✅ 支持 | Host 需外加 5V 升压 + 限流开关 + VBUS 检测 + ESD；FS 12Mbps，U 盘/HID 够用，视频不行 |
| 存储 F-STO | 16MB NOR + 16MB PSRAM 合封，MPI3 可扩，2× SDMMC | ✅ 强项 | TF 卡走 SD2，WiFi 走 SD1，互不干扰 |
| 电源 F-PWR | Buck+LDO，hibernate 1µA，VBATS 测电量 | ⚠️ 缺充电 | **本型号无充电器**，必须外挂 Charger（如 SGM41511/ETA 系列）+ 电量计或 ADC 估算；建议加负载开关分别断 WiFi/屏/摄像头供电 |
| AI F-AI | 无 NPU，M33@240MHz + CMSIS-NN | ⚠️ 只能云端 | STT/TTS/LLM/VLM 全走云；端侧只做 VAD、简单检测（低分辨率人形/手势会很勉强）；**唤醒词 KWS 不现实**，v1.0 用按键触发 |
| 工具框架 F-TOOL | — | ✅ 纯软件 | 与硬件无关，是平台自己的活 |
| 安全 F-SEC | AES/HASH/CRC/TRNG、eShield、XIP 解密、PSA L1 | ✅ 支持 | 应用包签名、Flash 加密、密钥不出芯片 |
| 运维 F-OPS | 2×WDT + IWDT，DFU 分区 | ✅ 支持 | 16MB Flash 建议划 A/B 双分区做 OTA 回滚 |

## 六、五个必须现在就知道的结论

1. **没有 WiFi 是最大的架构缺口。** 平台的 AI 能力全部依赖云端，WiFi 是生命线。方案：SDIO WiFi 模组（占 SD1 + 4 个控制脚），或产品定位改成"必须连手机"（BLE 中继），后者会废掉"独立玩伴"的卖点。**这个必须先定，它决定 8–10 个脚和 1 个模组的 BOM。**
2. **没有充电器。** 57G 是常规供电版本，充电必须外挂。BOM 要加 Charger + 可能的电量计 + 路径管理，且充电时的热和 EMI 会影响音频（麦克风底噪）。
3. **屏幕必须选带 GRAM 的 QSPI/SPI 屏。** 无 DSI，DPI 要 20 脚、8080 要 13 脚，都会把摄像头挤掉。QSPI@48MHz DDR 理论约 48MB/s，466×466 RGB565 一帧 434KB，带宽够 50fps 以上；配 TE 做局部刷新，功耗与撕裂都可控。
4. **IO 零余量。** 全功能 58/58 用满。要么按第四节做减法（PDM 麦 + 1-bit SDIO 留 5 脚），要么现在就砍一个子系统。
5. **摄像头是唯一"文档看不够、必须实测"的模块。** DCMI 全部信号只有矩阵通道，22 个脚挤在一个 crossbar 上，8bit DVP 的建立/保持时间要打表验证。**建议 P1 阶段先做一块相机小板实测 PCLK 上限，再定 sensor 与分辨率。**

## 七、待确认清单

- [ ] WiFi 方案：SDIO 模组（推荐 RTL8720/ESP32-C 类）还是 BLE 中继？
- [ ] 屏幕：QSPI AMOLED 具体型号（尺寸/分辨率/是否带 TE）
- [ ] 麦克风：模拟双麦还是 PDM 双麦（影响 2 个脚与 AEC 效果）
- [ ] 摄像头：DVP + JPEG 直出（OV2640 类）还是 SPI 慢速
- [ ] 充电：Charger 型号与是否要电量计
- [ ] DS 特性页写"2 路对外 3.3V LDO 150mA×2"，但 UM 的 57G 电源管脚表未列 VDD33 输出脚 —— **需向厂商确认 57G 是否真有这两路 LDO**（影响 WiFi/sensor 供电设计）
- [ ] GPDMA 8 通道在"屏刷新 + 相机 + 音频 + SDIO"并发下是否够，需排优先级表
