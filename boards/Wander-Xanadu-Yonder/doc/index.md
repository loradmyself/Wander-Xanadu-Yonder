# SF32LB57-LCD_N16R16_DPI

## SoC
SF32LB57GYD7N6

### MPI1
PSRAM 16MB

### MPI2
NOR Flash 16MB

## External Storage
### MPI3
None

## Display
DPI LCD, 4.3" 800x480 RGB (800x480-ZJY430-RGB, ST7265 timing, PCLK 30MHz ≈54Hz)
> 注：本板目录从 HDK 的 dpi-hdk_lb57eybbn6_n16_dpi 派生，board.conf 默认值
> 仍是那块的 7 寸 HTM-H070A20(1024x600)。本项目实物为 800x480 ZJY430，
> 由 project/sf32lb57-USB_Display_hcpu/proj.conf 覆盖。

## Touch
Capacitive touch panel, reset controlled by AW9523B

## Camera
GC032A (DVP)

## Other
AW9523B I/O expander on I2C1, RGB LED, ADC buttons
