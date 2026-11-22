/*
 * SPDX-FileCopyrightText: 2026 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bsp_board.h"
#include "aw9523.h"

#define EPD_GMODE_PIN           (54)         // GPIO_A54

#define EPD_VDD33_EN            (52)         // GPIO_A52
#define CTP_VDD33_EN            (53)         // GPIO_A53
#define DPI_VSYNC_PIN   (51)          // GPIO_A51

extern void BSP_GPIO_Set(int pin, int val, int is_porta);

void BSP_LCD_Reset(uint8_t high1_low0)
{
    aw9523_ensure_init();
    aw9523_set_pin(AW9523_PIN_LCD_RST, high1_low0);
}


void BSP_LCD_PowerDown(void)
{
    BSP_LCD_Reset(0);
}
static void QSPI_PIN_Set(int pad, pin_function func, int flags, int hcpu)
{
    HAL_PIN_Set(pad, func, flags, hcpu);
#ifdef BSP_LCDC_USING_DDR_QADSPI
    // Set the pin driver strength to 12mA
    HAL_PIN_SetDS(pad, PIN_DS_12MA);
#else
    // Set the pin driver strength to 8mA
    HAL_PIN_SetDS(pad, PIN_DS_8MA);
#endif /* BSP_LCDC_USING_DDR_QADSPI */
}

void BSP_LCD_PowerUp(void)
{
#if defined(BSP_USING_DCMI)
    /*
     * The panel and the DVP camera share PA32/PA33/PA43/PA44/PA45 on this
     * devkit, so only one of them can own those pads. Power the camera down
     * before the LCD takes them over.
     */
    BSP_CAMERA_PowerDown();
#endif /* BSP_USING_DCMI */

#ifdef BSP_LCDC_USING_DPI
    /* R channel */
    HAL_PIN_Set(PAD_PA24, LCDC1_DPI_R0, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA25, LCDC1_DPI_R1, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA26, LCDC1_DPI_R2, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA27, LCDC1_DPI_R3, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA28, LCDC1_DPI_R4, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA29, LCDC1_DPI_R5, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA30, LCDC1_DPI_R6, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA31, LCDC1_DPI_R7, PIN_NOPULL, 1);

    /* G channel */
    HAL_PIN_Set(PAD_PA32, LCDC1_DPI_G0, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA33, LCDC1_DPI_G1, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA37, LCDC1_DPI_G2, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA38, LCDC1_DPI_G3, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA39, LCDC1_DPI_G4, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA40, LCDC1_DPI_G5, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA41, LCDC1_DPI_G6, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA42, LCDC1_DPI_G7, PIN_NOPULL, 1);

    /* B channel */
    HAL_PIN_Set(PAD_PA43, LCDC1_DPI_B0, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA44, LCDC1_DPI_B1, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA45, LCDC1_DPI_B2, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA52, LCDC1_DPI_B3, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA53, LCDC1_DPI_B4, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA54, LCDC1_DPI_B5, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA46, LCDC1_DPI_B6, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA47, LCDC1_DPI_B7, PIN_NOPULL, 1);

    /* Timing control */
    HAL_PIN_Set(PAD_PA48, LCDC1_DPI_CLK,   PIN_NOPULL, 1);
    // Set the clk pin driver strength to 8mA
    HAL_PIN_SetDS(PAD_PA48, PIN_DS_8MA);
    HAL_PIN_Set(PAD_PA49, LCDC1_DPI_HSYNC, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA51, LCDC1_DPI_VSYNC, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA50, LCDC1_DPI_DE,    PIN_NOPULL, 1);

    /* Backlight
     *
     * 注意：HDK 原板（dpi-hdk_lb57eybbn6_n16_dpi）的背光 PWM 在 PA11
     * (GPTIM2_CH2)，但本板的 PA11 经 R6(0R) 接的是 WS2812B 的 DIN，
     * 两者不可能共用一根线。这行如果保留，LCD 初始化（晚于 BSP_PIN_Init）
     * 会把 PA11 重新复用成 GPTIM2_CH2，pwmt1 CH4 送出的 WS2812 位流就到不了
     * pin，灯会一直停在开机锁存的白色。
     *
     * 本板背光不需要软件 PWM（恒亮），所以这里不再占用 PA11；PA11 由
     * bsp_pinmux.c 的 BSP_PIN_Init() 复用为 GPTIM1_CH4 供 WS2812 使用。
     * 若换回 HDK 原板，必须恢复下面这行。 */
    // HAL_PIN_Set(PAD_PA11, GPTIM2_CH2, PIN_NOPULL, 1);   // LCDC1_BL_PWM_CTRL, LCD backlight PWM (pwmt2 CH2)

#elif defined(BSP_LCDC_USING_DBI)
#elif defined(BSP_LCDC_USING_DIRECT_EPD)

#ifdef EPD_TPS_GOOD_PIN
#endif

#elif defined(BSP_LCDC_USING_QADSPI)
    /*
     * QSPI routing of the sf32lb57-lcd devkit with the 57 QSPI ribbon.
     *
     * DIO0..DIO2 use the pads shared with the DPI connector, while DIO3, CLK,
     * CS and TE are routed on the pads of the dedicated 57 QSPI ribbon:
     *   DIO0 PA39   DIO1 PA38   DIO2 PA37   DIO3 PA30
     *   CLK  PA31 (12mA)       CS   PA29   TE   PA28
     * The panel reset is not a SoC pad, it is driven by the AW9523 (P1_7).
     */
    QSPI_PIN_Set(PAD_PA39, LCDC1_MATRIX_SPI_DIO0, PIN_NOPULL, 1);
    QSPI_PIN_Set(PAD_PA38, LCDC1_MATRIX_SPI_DIO1, PIN_NOPULL, 1);
    QSPI_PIN_Set(PAD_PA37, LCDC1_MATRIX_SPI_DIO2, PIN_NOPULL, 1);
    QSPI_PIN_Set(PAD_PA30, LCDC1_MATRIX_SPI_DIO3, PIN_NOPULL, 1);
    QSPI_PIN_Set(PAD_PA31, LCDC1_MATRIX_SPI_CLK,  PIN_NOPULL, 1);
    HAL_PIN_SetDS(PAD_PA31, PIN_DS_12MA);
    HAL_PIN_Set(PAD_PA29, LCDC1_MATRIX_SPI_CS,    PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA28, LCDC1_MATRIX_SPI_TE,    PIN_NOPULL, 1);

    HAL_PIN_Set(PAD_PA45, GPTIM2_CH2, PIN_NOPULL, 1);   // LCDC1_BL_PWM_CTRL, LCD backlight PWM (pwmt2 CH2)

#endif /* BSP_LCDC_USING_QADSPI */

    HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_LCDC, RCC_CLK_LCDC_SYSCLK);

    /* Release the panel reset. On this board it is driven by the AW9523
       (P1_7) and drv_lcd never calls BSP_LCD_Reset() on its own, so without
       this the panel stays in reset and never answers the ID read. */
    BSP_LCD_Reset(1);
}

void BSP_LCD_GMODE_Set(uint8_t high1_low0)
{
    BSP_GPIO_Set(EPD_GMODE_PIN, high1_low0, 1);
}


void BSP_TP_PowerUp(void)
{
    aw9523_ensure_init();
    aw9523_set_pin(AW9523_PIN_CTP_RST, 1);

    //TP irq pin
    HAL_PIN_Set(PAD_PA00 + TOUCH_IRQ_PIN, GPIO_A0 + TOUCH_IRQ_PIN, PIN_PULLUP, 1);
}

void BSP_TP_PowerDown(void)
{
    aw9523_ensure_init();
    aw9523_set_pin(AW9523_PIN_CTP_RST, 0);
    //TP irq pin
    HAL_PIN_Set(PAD_PA00 + TOUCH_IRQ_PIN, GPIO_A0 + TOUCH_IRQ_PIN, PIN_NOPULL, 1);
}

void BSP_TP_Reset(uint8_t high1_low0)
{
    aw9523_ensure_init();
    aw9523_set_pin(AW9523_PIN_CTP_RST, high1_low0);
}

uint32_t BSP_GET_DPI_AUX_VSYNC_PIN(void)
{
    return DPI_VSYNC_PIN;
}
