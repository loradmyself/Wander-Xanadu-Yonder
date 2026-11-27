# UI 框架（换碟唱机）

> 依据：`docs/ui-logic-prototype.html` v0.4 + 契约 `src/ui/theme.h`、`src/ui/slots.h`
> 落地代码：`src/ui/`（6 个 .c，约 2400 行）
> 固件：`main.bin` 681,704 B（ROM 7 MB 分区，余量充足）

## 一、分层

```
wxy_ui.c      编排层：导航状态机（轨道/内圈/应用）、泛层、计时与音乐运行时、线程
  ↑
wxy_pages.c   内容层：10 个场景（5 主场景 + 5 应用视图），全部绝对定位 452×364
  ↑
wxy_disc.c    唱机：10 张碟的身份、唱臂物理模型、轨道点、小 CD、碟仓、走带区
  ↑
wxy_kit.c     工具箱：控件工厂、动效、主题应用回调
  ↑
wxy_theme.c   主题引擎（theme.h 实现）      wxy_slots.c  图标槽位（slots.h 实现）
```

三条硬约定，代码里没有例外：

1. **页面不写色值**。所有颜色来自 `WXY_TOK_COLOR_*`，控件创建时用 `wxy_reg(obj, cb)` 登记，
   切主题时由主题引擎统一重刷。改主题 = 换一份 `const` 数据。
2. **不创建/销毁页面**。10 个场景启动时全部建好，之后只切显隐与透明度；
   任何一次点击都不触发 `malloc`（只有图标按槽位变化时才重建单个图标）。
3. **界面只认"碟"**。`wxy_ui_disc("timer")` 就能把界面推到计时器，AI Core 不需要知道界面结构。

## 二、碟与状态机

| 碟 | 编号 | 类型 | 说明 |
|---|---|---|---|
| chat / home / tools / set | 01–04 | 轨道 | 外圈，滑针切换，底部走带区 prev/next 循环 |
| theme | 04a | 内圈 | 设置 → 主题，"唱针往里一层"，父层虚化保留 |
| music / timer / weather / note / pal | M1/T1/W1/N1/C1 | 应用 | 换入后母轨虚化，返回时母碟旋回 |

```
          ┌───────── 轨道(外圈) ─────────┐
          │  chat ←→ home ←→ tools ←→ set │
          └───────────────┬───────────────┘
                    descend / return
                          ↓ ↑
                      theme(内圈)
   任意轨道 --open app--> 应用碟 --back--> 母轨
```

换碟动作（`wxy_tt_swap`，时长取 `WXY_TOK_MOTION_SWAP`=520ms）：
抬针 208ms → 旧碟右移 126px 淡出 → 换涂装（碟面/编号/名称/图标/走带文字）→ 新碟左移 78px 淡入 → 落针。

## 三、唱臂是真模型，不是贴图

支点固定在 (952, 96)，臂长 470，落点角度决定针尖坐标：

```
针尖 = 支点 + 470 × (cos θ, sin θ)
θ = 132°(外圈) … 152°(内圈)；音乐播放时随进度再推进 18°
```

`wxy_tt_progress(pct)` 同时推进唱针与盘面读纹光点，所以播放时"针在往里走"是算出来的。

## 四、主题与槽位

- 三套主题：`ink / pixel / cyber`，令牌值直接取自原型 CSS 变量；动效时长也进令牌
  （pixel 更快、cyber 更慢），页面对此无感。
- 切换流程：查表 → 换指针 → 遍历登记控件重刷。控件被删除时自动摘除登记项
  （挂 `LV_EVENT_DELETE`），所以运行时重建图标不会留下悬空指针。
- 图标：29 个槽位。有 FontAwesome 字形的用符号字形，没有的（心形、三张脸、钟、太阳）
  用 LVGL 图元拼，都是单色、都可染色。真 PNG 资源到位后只替换 `wxy_icon_create()`。

## 五、契约自检（2026-11-25 复查）

对 6 个 .c 做了一次"页面层有没有偷偷写色值"的扫描，结论与两处修正：

| 扫描项 | 结果 |
|---|---|
| `lv_obj_del(` LVGL 8 残留 | 0 处 |
| 页面层 `wxy_reg()` 登记数 | `pages 26 / disc 16 / ui 15`，每个改动颜色的控件都有回调 |
| 硬编码色值 | 3 类，全部收敛 |

修掉的两处：

1. **TTS 提示条前景写死** `0x17181A`，而底色走 `wxy_apply_accent_bg`。
   三套主题的 accent 明度差很大（ink `#E0B877` / pixel `#9BD465` / cyber `#F6C86A`），
   cyber 的 `ON_ACCENT` 恰好也等于 `0x17181A`，所以只在 ink/pixel 下露馅。
   改为 `WXY_TOK_COLOR_ON_ACCENT` + 登记 `wxy_apply_on_accent`（文本）；
   图标不是 label，`text_color` 对图元无效，单独给 `tts_icon_apply()` 调
   `wxy_icon_recolor()` 重染。
2. **碟身份色被复制到页面**：音乐页小碟的盘心、陪伴页的三张脸，
   原来各自抄了一份 `0xF6C86A` / `0xE2758F`。新增 `wxy_disc_hue(id)`，
   页面改成 `wxy_disc_hue("music")` / `("pal")`——碟表改了页面自动跟。

保持写死的一处：`wxy_disc.c` 的碟表本身。碟身份色是**故意不随主题变**的
（原型里也是固定 hue），所以它只允许出现在那张表里。

## 六、加一屏要改哪里

1. `wxy_discs[]` 加一行（id / 编号 / 名称 / 槽位 / 碟面主色 / 唱针角 / 类型）；
2. `wxy_pages.c` 写一个 `build_xxx(lv_obj_t *root)`，在 `wxy_pages_create()` 里
   `build_xxx(scene_begin("xxx"))`；
3. 需要被 AI/用户跳转，就调用 `wxy_ui_disc("xxx")`。

不需要碰唱机、走带区、主题引擎，也不需要写颜色。

## 七、已知偏差（相对原型，明确记账）

| 原型效果 | 当前实现 | 原因 / 后续 |
|---|---|---|
| `filter: blur()` 泛起 | 透明度 + 位移 | LVGL 无模糊，软件渲染也扛不住；观感接近 |
| 唱片整体旋转 | 读纹光点绕行 + 标签微动 | 560px 全盘旋转每帧重绘过大，需先测 `transform_rotation` 帧率 |
| 中文碟名 | 英文（Talk / Desk / Set …） | 固件只有 Montserrat；内置 CJK 字体只覆盖 53% 常用字。字库随 `.xzp` 资源包落地后，只改 `wxy_discs[]` 与字符串表 |
| 涟漪 / 呼吸 | 三根微光占位条 | 需要 RGBA 渐变绘制，收益低 |
| 碟心轴孔 | 未画 | 与碟面文字重叠 |

## 八、上板验收清单

串口（115200）应依次看到：

```
[WXY.THEME] theme base = ink
[WXY.TT]    disc -> chat
[WXY.PAGE]  scenes ready (10)
[WXY.UI]    ui ready (1024x600, 10 scenes), lv heap <used>/<2048> KB (used <p>%, frag <f>%), sys heap <u>/<t> KB
```

另外开头应有 `LV_MEM_RT: psram heap enabled [...), size=2048KB`——
如果没有这行而是 `using rt_malloc path`，说明 PSRAM 私有堆没生效，对象会挤回 191KB
系统堆，建 UI 建到一半就 OOM 断言（2026-11-26 上板实测死机就是这个）。

屏幕上：

1. 左侧黑胶盘 + 右上唱臂 + 5 个轨道点 + 左上角小 CD + 底部走带区；
2. 点轨道点 → 换碟（旧碟旋出、新碟旋入、唱针落下）；点小 CD → 碟仓展开，10 张碟可换；
3. 桌面点 Music → 换入 M1 碟，播放/暂停时读纹光点与唱针同步走；
4. 设置 → 主题与情绪 → 三套主题即时切换，页面不闪、不重建；
5. 对话页点"Hold to talk" → 柔光泛层；点天气卡 → 详情浮出盘面。

**内存观察点**：`ui ready` 里的 `lv heap` 数值。LVGL 已走 2MB PSRAM 私有堆
（`proj.conf`：`LV_RTTHREAD_USE_PSRAM_PRIVATE_HEAP=y`，2048KB），约 250 个控件
预计占 200~400KB，离上限很远。`sys heap` 是系统堆（线程/驱动用），与本框架无关。

**颜色深度**：保持默认 `LV_COLOR_DEPTH=16`。启动时那条
`framebuffer color depth(24) mismatch` 是**误报**——`lv_lcd_init()` 先用 GET_INFO
读到物理屏的 24bpp，随后就用 `RTGRAPHIC_CTRL_SET_BUF_FORMAT` 把 fb 设成 RGB565，
由显示驱动转到 RGB888 屏上（SiFli 官方例程也是这么跑的）。
不要为了消警告改成 24：绘制缓冲 `buf1_1` 在 **L1 内部 SRAM**，204KB→307KB 会把
系统堆从 191KB 挤到约 90KB，连 16KB 的 UI 线程都建不起来（2026-11-26 第二次上板实测）。
若后续真观察到花屏，再走 `FRAME_BUFFER_IN_PSRAM` 路线把绘制缓冲挪到 L2，而不是改深度。

## 九、性能与字体（2026-11-26 上板）

**硬件加速是开着的**：反汇编 `lv_init()` 能看到 `bl lv_draw_epic_init`，
`lv_draw_epic.o` 已链入。EPIC 负责 FILL / BORDER / IMAGE，SW 单元做兜底。

**字体错乱的原因**：EPIC 的 LABEL 任务在画 Montserrat 的 A4/A8 字形遮罩时，
把字形位图错置/相互挤压（照片里 "Desk / Tools / Notes" 挤成一团）。
已给 SDK 打单点补丁，让 LABEL 任务回落到 SW 绘制单元：

```
SiFli-SDK/middleware/lvgl/lv_drivers_v9/sifli/lv_draw_epic.c
  evaluate() 里用 #if 0 包住 LV_DRAW_TASK_TYPE_LABEL 分支
```

补丁原件保存在 `docs/patches/sifli-epic-label-sw.patch`。
**执行 `git submodule update` 后补丁会丢**，进 SDK 目录重新
`git apply ../../docs/patches/sifli-epic-label-sw.patch` 即可。
不要在 `project/SConscript` 里用 `CPPDEFINES` 打开 SDK 的
`DEBUG_LV_DRAW_EPIC_ENABLED_FUNCTION` 调试门——实测会让 SCons 依赖计算卡死。

**帧率测量**：`proj.conf` 已开 `LV_USE_SYSMON` + `LV_USE_PERF_MONITOR_LOG_MODE`，
串口会周期性打印 FPS/CPU 占用。先拿静态首屏的基线数字，再判断要不要继续优化：
优先排查全屏 invalidate 的范围（碟 560px + 7 圈纹路），而不是盲目关加速。

### 帧率排查：`fps2` 数据与日志开关

SDK 内置 msh 命令 `fps2` 可开关 SiFli 自己的帧率统计（无需重烧），串口每 2 秒打：

```
fps X.X cpu X.X EPIC X.X LVGL:X irate:X, drate:X
```

2026-11-26 实测：`fps 0.5~1.8 / cpu 0.0 / EPIC 0.2~0.8 / LVGL:0~99`。
**CPU 与 GPU 都空闲、LVGL 线程却 99% 忙**——说明不是算力问题，而是
render 或 flush 里有一处在阻塞。

一个坑：`LV_USE_SYSMON + PERF_MONITOR_LOG_MODE` 开的 sysmon 日志用裸 `LV_LOG()`，
必须同时开 `CONFIG_LV_USE_LOG=y` 才有效，否则编译期变空操作（默认就是关的）。
SDK 的 `littlevgl2rtt.c` 已注册 `lv_rt_log` 回调走 ulog，不必开 `LV_LOG_PRINTF`。
打开后串口会自动刷（display 创建时启动 sysmon，每 300ms 一条）：

```
sysmon: N FPS (refr_cnt: X | redraw_cnt: Y), refr Nms (render N | flush N), CPU N%
```

这条的 `render / flush` 分解是定位关键：
- **render 大** → LABEL 走 SW 画的开销，考虑恢复部分 EPIC 绘制或减文字；
- **flush 大** → 刷屏链路（framebuffer `wait_line_valid` 每次最多等 1s 是嫌疑）。

### 帧率根因与修复（2026-11-26，sysmon 分解实测）

打开 `LV_USE_LOG` 后拿到 sysmon 分解，一锤定音：

```
sysmon: 1 FPS ... refr 551ms (render 540ms | flush 10ms), CPU 99   ← 转场动画期间
sysmon: 30 FPS ... refr 0ms (render 0ms | flush 0ms), CPU 0         ← 静止后
```

- **flush 只有 8~10ms**：刷屏链路、颜色转换、framebuffer 全部无问题（此前怀疑的
  `wait_line_valid` 1s 超时不成立）。
- **render 540ms**：动画期间每帧重绘整棵场景树（30+ 个走 SW 绘制的字形）。
- 静止后 `redraw_cnt=0 / render 0ms`：完全不重绘，所以"静态不卡、一动就卡"。

**根因**：转场动画对整棵场景树做 `opa 0→255 + y 9→0`，属性每帧变化强制全树重绘。
（注意：LVGL 9 普通 `opa` 不建离屏 layer，但只要属性在变，树就会被重画。）

**修复**：场景瞬切（opa COVER、y=0），改为动画一个独立的纯色覆盖层 `s_fade`
（BG 色矩形，盖在读碟窗上）的 opa：淡出=新场景淡入，淡入=旧场景淡出。
动画期间只重绘这一个矩形（EPIC FILL，<1ms），下面的树静止不重绘。
`wxy_fade(s_fade, COVER, TRANSP, ENTER, ...)` 复用现成辅助函数。

**遗留**：`wxy_ui_back/descend/return` 里对母轨 root 的 76→255 虚化恢复动画
仍是整树重绘，本次未改。若这些操作仍卡，同样思路改为覆盖层或改瞬时设置。

## 十、帧率最终结论（2026-11-26 晚，修正第八/九节的中间判断）

### 数据（上板 sysmon，打开 `CONFIG_LV_USE_LOG=y` 后）

```
转场/换碟: refr 551ms (render 540ms | flush 10ms), CPU 99, redraw_cnt 1
静止:      30 FPS, render 0ms | flush 0ms, redraw_cnt 0
fps2:      fps 1.3  cpu 0.0  EPIC 0.6  LVGL:99
```

### 关键推理：540ms 是「等」，不是「算」

`cpu_get_usage = 0%` 而 LVGL 线程 99% 忙，是**线程阻塞在等信号量**的特征；
纯软件逐像素绘制一定会把 CPU 吃满，不可能 0%。所以前面两节把卡顿归因于
「LABEL 走 SW 画字慢」「动画重绘整棵树」「覆盖层/离屏合成」**全部错误**——
这也解释了为什么改成覆盖层淡入后 render 仍是 534~589ms、一点没降。

### 根因

`rtconfig.h` 是 `CONFIG_LV_OS_NONE 1`（`LV_USE_OS=0`），EPIC 是**同步** draw unit：
`lv_draw_epic.c` 的 `execute_drawing()` 在每个 draw task 开头(397)与结尾(444)
各调一次 `drv_epic_wait_done()`，即 `drv_gpu_check_done(DRV_EPIC_TIMEOUT_MS)`，
而 `DRV_EPIC_TIMEOUT_MS = 500`。GPU 完成信号不到位时 `wait_gpu_done()` 每轮
`rt_sem_take` 等满 500ms 再 `gpu_reset()` 重试，正好对应 render ≈540ms。

旁证三条：EPIC 占用 <1%（GPU 根本没在加速）；EPIC 画 Montserrat A4/A8 字形
会错乱（第九节）；覆盖层方案减少重绘次数却完全无效——因为慢的是**每次渲染的
固定等待**，与重绘次数、重绘面积都无关。

### 修复：整体禁用 EPIC draw unit，全量走 SW

```
SiFli-SDK/middleware/lvgl/lv_drivers_v9/sifli/lv_draw_epic.c
  #define WXY_DISABLE_EPIC_DRAW_UNIT 1
  lv_draw_epic_init() 不再调用 lv_draw_create_unit()
```

安全性已逐项确认：
- `lv_draw_sw_init()` 在 `lv_init.c:239` 注册，早于 EPIC 的 267 —— 禁用后
  `unit_cnt=1`，SW 接管全部 draw task，不会黑屏；
- `drv_gpu_open()` 在 `rtos/.../drivers/drv_lcd.c` 另有 3 处调用，刷屏走
  `drv_lcd_fb_write_send`，不依赖 EPIC draw unit（实测 flush 8~10ms 也印证）；
- 想恢复硬件绘制，把宏改成 0 重编即可。

**统一补丁**：`docs/patches/sifli-lvgl9-sw-only.patch`（含 LABEL 回落 + unit 禁用
两处），取代旧的 `sifli-epic-label-sw.patch`。执行过 `git submodule update` 后
补丁会丢，进 SDK 目录 `git apply ../../docs/patches/sifli-lvgl9-sw-only.patch`。

**编译验证**：`main.bin` 703,368 B；`nm -S` 显示 `lv_draw_epic_init` 仅 2 字节
`bx lr`，`lv_draw_epic_fill/border/label/execute_drawing` 全部被链接器 GC（符号数 0）。

### 复测判读

- `render` 降到几十 ms、CPU 升到 60~99% → 修好了，剩下的是纯绘制量；
  若还嫌不够顺，再削 560px 碟的 7 圈纹路 / 3 层 glow / 文字数量。
- 仍 ≈540ms 且 CPU 仍 0% → 还有别的等待点（`lv_drivers_v9/lv_gpu.c:263` 的
  `drv_epic_wait_done()` 是下一个嫌疑），把 sysmon 行发我继续查。
