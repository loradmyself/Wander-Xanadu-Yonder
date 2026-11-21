# IO 分配与外设选型（SF32LB57GYD7N6）

> 状态：草案 v0.1 · 基于模组与 DevKit-LCD 两份原理图（V1.0/V1.1）+ DS5702 V0.2.2 + UM5701 V0.1
> 目标：回答"双屏（RGB 1024×600 + QSPI）+ WiFi + 摄像头 + 常规外设"在 58 个 GPIO 上的取舍

## 一、验证阶段硬件架构（原理图实测结论）

### 1. 模组 SF32LB57-MOD-WD（LCC72）

| 器件 | 型号 | 说明 |
|---|---|---|
| 主控 U3 | SF32LB57GYD7N6 | QFN80，58 GPIO，合封 16MB PSRAM + 16MB NOR |
| WiFi U5 | **AIC8800D40LS-N**（QFN-36） | **SDIO 1-bit**（D2/D3 NC）+ UART + 双向唤醒；双天线 ANT + BT_ANT |
| 外挂 Flash U2 | PY25Q128HA（16MB NOR） | 走 MPI3，占 PA12–PA17 共 6 脚 |
| 晶振 | 40MHz + 48MHz | X1 / Y2 |

模组上的关键注释（WiFi 共存策略）：
- **5G WiFi 与蓝牙可同时工作**（AIC8800D40 双频方案），WiFi 电源常供、默认不使能（待机 2µA）
- 2.4G WiFi 与蓝牙硬切换，不能同时工作——**平台应优先用 5G 频段**

### 2. DevKit-LCD 转接板（13 页原理图）

| 部件 | 实现 |
|---|---|
| 屏幕接口 | **40-pin FPC，RGB888 DPI**（R0–R7/G0–G7/B0–B7 + CLK/HSYNC/VSYNC/DE + BL_PWM + TP_* + LCD_RST + 5V）；注释明确 EPD LCM 也可经 DPI 转接 |
| 摄像头 J5 | **OV2640 / OV5640 / OV3660 三选一，DVP 接口**，配 ME6211C12/C15 LDO（1.2V/1.5V），CAM_RESET/PWDN |
| 音频 | NS4150B 扬声器 PA（IOEX 使能）+ 双 PDM 数字麦 |
| 调试 | CH343 USB-UART（V1.1 加了 DTR/RTS 下载控制） |
| 电源 | ETA3425（buck）+ ETA5055（LDO） |
| **IO 扩展 U11** | **AW9523BTQR**（Awinic，I2C 16 位，VFQFPN-24，立创 C148077，约 ¥2） |

### 3. AW9523 上挂的控制信号（实测 11 个）

`IOEX_LCD_RST`、`IOEX_CTP_RST`、`IOEX_TF_DET`、`IOEX_DVP_PDN`、`IOEX_AU_PA_EN`、`IOEX_PDM_PWREN`、`IOEX_LED_R/G/B`、`IOEX_HST_WAK_WF`。

**结论：官方的解法就是"吞吐型信号占 GPIO，低速控制全部塞进 AW9523"**。2 根 I2C 引脚换回 11 个 GPIO，这正是定制板该照抄的架构。

### 4. 官方给出的摄像头/WiFi/TF 复用关系（LCD 板注释原文）

| 组合 | 代价 |
|---|---|
| DVP + 4-bit SDIO WiFi | 无 TF 卡；2 根 DVP 信号与 WiFi D2/D3 复用 |
| **DVP + 1-bit SDIO WiFi + 1-bit TF** | 全功能共存（推荐，模组默认即 1-bit WiFi） |

## 二、双屏方案（两颗 LCDC 的分工）

UM 确认：LCDC0 支持 8080 / SPI-Dual-Quad / **DPI(RGB) 最高 24bit** / JDI / 8-16b EPD；LCDC1 为独立控制器，**只有 SPI（QSPI）接口**，8 根专用脚（CS/CLK/DIO0–3/RSTB/TE），不过 IO 矩阵。

| | 主屏（信息/表情） | 副屏（状态/常显） |
|---|---|---|
| 控制器 | LCDC0 | LCDC1 |
| 接口 | DPI RGB **16-bit 总线** | QSPI 4 线 |
| 引脚 | 20（D0–15 + CLK/DE/HS/VS） | 8（专用脚） |
| 分辨率 | 1024×600 | 建议 ≤390×390 |
| 帧缓冲 | 1.2MB/帧（RGB565），双缓冲 2.4MB | ≤0.3MB/帧 |

三个关键判断：

1. **RGB888 面板可以用 16-bit 总线驱动**：把总线的 R5–G6–B5 接面板的高 6 位（R7–R2 等），这是 40pin RGB888 屏的标准接法。24-bit 总线要 28 脚，在 58 脚芯片上等于放弃其他所有外设，**必须用 16-bit**。
2. **1024×600@RGB565 的带宽没问题**：60fps 像素时钟约 37MHz，PSRAM 直刷可行；LVGL 合成走 ePicasso 硬件混合，M33 只提交脏区。
3. **SDK 风险**：HAL 层（`bf0_hal_lcdc_aux.c`）有 LCDC1 引用、57x CMSIS 有寄存器，但 **LVGL middleware 对 57x"同时驱动两块屏"的支持未验证**——这是 M1 阶段第一个要做掉的验证项，若不支持则副屏走 `lv_display` 轮刷降级。

## 三、58 脚预算（定制芯片版）

定制板相对模组有一个天然红利：**D7 型号内置 16MB NOR，可以砍掉模组上的 PY25Q128HA 外挂 Flash，收回 PA12–PA17 共 6 脚**。

| 功能 | 引脚数 | 明细 | 取舍 |
|---|---|---|---|
| LCDC0 DPI 16-bit | 20 | D0–15、CLK、DE、HSYNC、VSYNC | P0，不可减 |
| 背光 BL_PWM | 0 | **走 AW9523 的 PWM 输出** | 官方思路的延伸 |
| LCDC1 QSPI 副屏 | 8 | 专用脚 CS/CLK/DIO0–3/RSTB/TE | 可整组砍掉（-8） |
| WiFi AIC8800 | 6 | SDIO 1-bit（CLK/CMD/D0）+ UART TX/RX；唤醒 2 根走 AW9523 | 1-bit 固定 |
| 摄像头 DVP | 12 | D0–7、PCLK、XCLK、HSYNC、VSYNC；RST/PDN 走 AW9523 | P0（AI 看世界） |
| PDM 麦 | 3 | CLK + L + R（双麦为 AEC 前提） | 单麦可 -1 |
| 音频输出 | 2 | DAC1P/N（模拟固定脚）+ NS4150B（EN 走 AW9523） | 固定 |
| I2C0（触摸/扩展/传感器） | 2 | AW9523 + CTP + IMU 共享 | 固定 |
| USB | 2 | PA35/PA36 固定 | 固定 |
| TF 卡 | 0 | **去掉**（应用走 WiFi 分发；AI 玩伴场景无大文件需求） | 省 3–6 |
| 调试 UART + SWD | 4 | PA18/19 + PA43/44 | 固定 |
| 电源键 | 1 | PA33–PA42（WKUP 硬约束） | 固定 |
| 按键 | 2 | 音量/确认 | 可 -1 |
| 振动马达 | 1 | PWM | 可 -1 |
| **合计** | **59 → 微调后 ≤58** | | |

**结论：双屏 + WiFi + 摄像头 + 双麦的全功能配置约 59 脚，超 1 脚**，减法优先级（从上往下砍）：
1. 背光、唤醒、LED、各使能脚全部进 AW9523（已计入）
2. TF 卡去掉：-3
3. 副屏整组砍掉：-8（若 LVGL 双显示验证失败，这是天然退路）
4. 双麦换单麦：-1（**不建议**，AEC 与声源定位都依赖双麦）

## 四、外设推荐器件清单

| 外设 | 推荐 | 理由 |
|---|---|---|
| WiFi | **AIC8800D40LS-N**（与模组同款） | 双频 5G 可与 BT 共存，SDIO 1-bit 省 IO，官方已在用，风险最低 |
| 摄像头 | **OV2640**（2MP） | DevKit 三选一里唯一 **JPEG 直出**；芯片无 ISP 无编码器，OV5640/3660 的裸 RGB 流会吃满 PSRAM 带宽 |
| 音频 PA | NS4150B（DevKit 同款） | D 类、3W、免滤波，使能脚走 AW9523 |
| 麦克风 | 双 PDM MEMS | 芯片 2×PDM 控制器，远场 2–2.5m 规格（DevKit 注释） |
| IO 扩展 | **AW9523BTQR** | ¥2 / 16 路 / I2C / LED 调光，官方同款 |
| 触摸 | 电容触摸 I2C（GT911/CST 系列） | INT+RST，RST 走 AW9523 |
| IMU | I2C（LIS2DH/ST 系） | 中断脚务必落 **PA33–PA42**（仅这 10 脚可休眠唤醒） |
| 充电 | 外挂 Charger（如 ETA/SGM 系列） | **57G 无片上充电器**，这是硬件评估时确认的缺口 |
| 电源 | ETA3425 + ETA5055（DevKit 同款组合） | 官方已验证的 3.3V/1.8V 树 |

## 五、待验证清单（按阻塞程度排序）

- [ ] **LVGL 双显示（LCDC0 DPI + LCDC1 SPI）在 57x 上是否可用**——决定副屏去留，M1 第一项
- [ ] LCDC1 的 8 根专用脚在 QFN80 的具体落位（DS 管脚表，画原理图前确认）
- [ ] AIC8800D40LS 在 1-bit SDIO 下的实测吞吐（仅决定 WiFi 分发 .xzp 的速度预期）
- [ ] OV2640 DVP 8bit 的 PCLK 时序实测（DCMI 全走矩阵，之前结论：必须实测）
- [ ] AW9523 的 I2C 地址分配与触摸/IMU 无冲突
