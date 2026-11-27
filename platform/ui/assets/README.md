# WXY 图片资源目录契约

资源分两类：**框架 UI 资源**（全局共享，属于唱机外壳）和**应用资源**（属于某个应用，跟应用走）。
两类都用同一套 `ezip/` + `raw/` 分类规则，构建脚本统一扫描转换。

## 目录结构

```
platform/ui/assets/          框架 UI 资源（唱机外壳，全局共享，随 platform/ui 走）
├── ui/
│   ├── ezip/                框架大图 → 构建期转 eZip
│   │   ├── wxy_bg_full_1024x600.png    整屏背景
│   │   ├── wxy_vinyl_field_560.png     碟面
│   │   ├── wxy_vinyl_label_156.png     碟心标签
│   │   └── wxy_mini_cd_56.png          角落小 CD
│   └── raw/                 不转 eZip 的原图（存档/预览，不进固件）
└── icons/
    ├── ezip/                框架图标 → 构建期转 eZip（30 个，见 slots.h）
    └── raw/                 不转的图标（如 _preview.png）

apps/<app>/                  应用（参考 EPD 的 app.json 注册方式）
├── app.json                 应用清单（id/name/version/…）
└── assets/
    ├── ezip/                应用私有图 → 构建期自动转 eZip
    │   └── <app-id>_xxx.png
    └── raw/                 不转的原图

assets/                      全局素材（尚未接入构建：faces/ 表情、sounds/ 音效）
```

## 核心规则

1. **只有 `ezip/` 目录下的 PNG 会被转换**，`raw/` 下的不进固件。
2. **零配置收集**：构建脚本（platform/ui/SConscript）扫描 `platform/ui/assets/ui/ezip/`、
   `platform/ui/assets/icons/ezip/`、`apps/*/assets/ezip/` 下所有 PNG。
   加图/换图只要丢同名文件进来，不用改 SConscript。
3. **符号名 = 文件名**（去 `.png`）。C 侧直接 `extern const lv_image_dsc_t <文件名>` 引用。
4. **全局唯一**：所有应用的资源符号在同一个链接空间里。
   - 框架图用 `wxy_` 前缀（`wxy_ic_*` / `wxy_bg_*`）
   - 应用图必须用应用 id 前缀（`music_cover_48.png` / `weather_sunny_48.png`）
   - 撞名时构建脚本会直接报错并指出冲突的两个文件。
5. **生成物**在 `build_<board>/wxy_ui/*.tmp.c`，不落源码树，不入 git。

## 转换参数（已踩坑，勿改）

- `-rgb565`：屏幕物理 RGB888 但 LVGL 用 16 位色深，驱动层转换。
- `-winsize 2048`：LB57 硬件限制，**必须显式写**。漏掉上板报
  `EZIP_INT_STA_WIND_ERR_STA` → `wait_gpu_done timeout` 断言崩溃。
- **绝不能加 `-chip sf57x`**：强制调色板丢 alpha，图标透明区变黑块。
- 详见 `platform/ui/SConscript` 顶部注释。
