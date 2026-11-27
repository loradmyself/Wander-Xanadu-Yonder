# Wander-Xanadu-Yonder 项目长期约定

## 构建（Git Bash，务必照抄）
```bash
cd project
export SIFLI_SDK=F:/Wander-Xanadu-Yonder/SiFli-SDK
export RTT_CC=gcc
export RTT_EXEC_PATH=C:/Users/YBL/.sifli/tools/arm-none-eabi-gcc/14.2.1/bin
export PATH="/c/Users/YBL/.sifli/tools/arm-none-eabi-gcc/14.2.1/bin:$PATH"   # 必须 /c/ 形式
export PYTHONPATH="$SIFLI_SDK/tools/build;$SIFLI_SDK/tools/build/default"      # 必须分号
scons --board=Wander-Xanadu-Yonder --board_search_path=F:/Wander-Xanadu-Yonder/boards -j8
```
- PATH 写成 `C:/...` 会被 MSYS 转成 `C;F:\...`，裸名 `arm-none-eabi-gcc` 找不到，报成 `link_copy.lds 系统找不到指定的文件`。
- `reg.exe 被安全策略拦截` 是 SDK 探测 Keil/IAR 注册表，噪音，忽略。
- 产物：`project/build_Wander-Xanadu-Yonder_hcpu/main.bin`。

## 代码约定
- UI 页面层不写色值，一律 `WXY_TOK_COLOR_*` + `wxy_reg(obj, cb)`；碟身份色只出现在 `wxy_discs[]`，页面用 `wxy_disc_hue(id)` 取。
- 场景启动时全建好，只切显隐/透明度，点击不 malloc。
- 界面只认"碟"：`wxy_ui_disc("timer")` / `wxy_ui_app("music")`，AI Core 不需要知道界面结构。
- 文案暂用英文：内置 CJK 字库只覆盖约 47% 常用字，等 `.xzp` 字库资源落地再换。

## 内存与颜色（2026-11-26 上板实测定案）
- LVGL 走 2MB PSRAM 私有堆：proj.conf `LV_RTTHREAD_USE_PSRAM_PRIVATE_HEAP=y` + 2048KB；系统堆仅 191KB，LVGL 挤在里面必 OOM（断言 lv_obj_remove_flag 295）。
- 屏 htm_h070a20 物理 RGB888，但 `LV_COLOR_DEPTH` 保持默认 **16**（驱动 `SET_BUF_FORMAT` 转 RGB565）。切 24 会让 L1 SRAM 里的绘制缓冲 +102KB、系统堆缩到 ~90KB，UI 线程建不起来。若真花屏，用 `FRAME_BUFFER_IN_PSRAM` 把绘制缓冲挪到 L2，不要改深度。
- 烧录后串口应见 `LV_MEM_RT: psram heap enabled ... size=2048KB`。

## LVGL 渲染 / 帧率（2026-11-26 上板定案）
- **LB57 全核是 Cortex-M33**（register.h `__CM33_REV`），没有 M55/Helium。严禁
  `-march=armv8.1-m.main+mve`（M33 执行 MVE 即 fault，表现为只输出 SFBL 无 banner）。
  building.py 的 mtune/march 保持 SDK 默认 cortex-m33/armv8-m.main+dsp+fp+cdecp1。
  改 CPU 架构前先查 CMSIS register.h。render ~530ms 纯 C 是 M33 真实水平，优化靠减绘制量。
- **图片渲染走真硬件 ezip（用户定案），EPIC draw unit 已重新启用**。ezip 只有 EPIC
  绘制路径能解（lv_draw_epic_img.c EPIC_INPUT_EZIP），SW blend 不认识 LV_IMAGE_FLAGS_EZIP。
  - 转换：`ezip.exe -convert x.png -rgb565 -cfile 2 -section ROM3_IMG -winsize 2048
    -lvgl_version 9`（winsize 2048 是 LB57 硬件限制）。产物在 src/ui/icons_ezip/。
  - 接入：wxy_icon_ezip.c 查表 + wxy_slots.c 的 wxy_icon_create 图片优先；recolor 对
    lv_image no-op（v9 widget 无 recolor，EPIC recolor 仅 draw dsc 层）。
  - 链接：`.ROM3_IMG_EZIP*` 段已加进 Templates/gcc/hcpu/link.lds 的 **.text 段**。
    放 .rom2(ROM2) 会产生第二个 load region → main.bin 变目录 → ptab 打包断言失败；
    放 .text 主镜像保持单 bin。资源若不显示：main.bin 目录残留要先 rm 掉。
  - SDK 补丁（EPIC LABEL→SW 防字形错乱 + lds 资源段）：`git submodule update` 后重新
    apply `docs/patches/sifli-lvgl9-epic-label-and-ezip-lds.patch`。
- EPIC 500ms 等待 bug 的历史：曾表现为 cpu 0% + LVGL 线程 99% 忙（阻塞等信号量），
  全量 SW 时换碟 render ~530ms。现在 EPIC 重新启用走硬件 ezip，上板需重新验证 500ms
  是否复现（若复现，ezip 图片绘制也会卡，届时再排 drv_epic_single 的完成信号）。
- 判读方法：先 `CONFIG_LV_USE_LOG=y` 或 msh 敲 `fps2`。看 render/flush 分解 + CPU 匹配：
  render 大且 CPU 低 = 等（bug/信号量），render 大且 CPU 高 = 算（绘制量）。
