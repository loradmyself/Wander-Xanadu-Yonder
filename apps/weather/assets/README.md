# weather 应用资源

## 目录约定

- `ezip/` — 构建期自动转 eZip（LVGL 9 + EPIC 硬件解码），**必须放这里**
- `raw/` — 不转换的原图（仅存档/预览用，不进固件）

构建脚本会扫描所有 `apps/*/assets/ezip/*.png` 并自动转换，无需改 SConscript。

## 命名契约（重要）

**所有 ezip/ 下的 PNG 文件名必须以应用 id 为前缀**：`weather_<name>.png`。

原因：eZip 转换后的 C 符号名 = PNG 文件名。全工程所有应用的资源符号在同一个链接空间里，
不加前缀会跨应用撞名（例如两个应用都有 `cloud_24.png` → `multiple definition`）。

示例：
- `weather_sunny_48.png` → 符号 `weather_sunny_48`
- `weather_cloud_24.png` → 符号 `weather_cloud_24`

## 参数注意

带透明通道的图标用 RGBA 源图即可，转换不加 `-chip sf57x`（会强制调色板丢 alpha）。
`-winsize 2048` 是 LB57 硬件限制，构建脚本统一处理。
