# Wander · Xanadu · Yonder

**雪夜舟 / WXY** — 一个把 AI 玩伴做成平台的嵌入式工程。

> Not all those who wander are lost.
> — J.R.R. Tolkien, *The Fellowship of the Ring*

平台化的 AI 玩伴：动态应用不再只是"应用"，每个应用同时向 AI 暴露一组**工具**，用户可以直接点开用，AI 也可以替你调用。二者共用同一份代码、同一个安装包。

| 层 | 中文 | 英文 | 缩写 |
| --- | --- | --- | --- |
| 主名（整机 / 品牌） | 雪夜舟 | Wander · Xanadu · Yonder | WXY |
| 系统与交互运行时 | 无弦音 | Wander · Xanadu · Yonder | WXY |

- **雪夜舟**：典出《世说新语·任诞》王子猷雪夜访戴，「乘兴而行，兴尽而返，何必见戴」。陪伴的价值在过程，不在达成目的。
- **Xanadu**：柯勒律治《忽必烈汗》「In Xanadu did Kubla Khan / A stately pleasure-dome decree」—— 灵感降临之地。
- **无弦音**：《宋书·陶潜传》「畜素琴一张，无弦，每有酒适，辄抚弄以寄其意」—— 不必你把话说完整，它已经懂了。

命名的完整决策过程见 [docs/NAMING.md](docs/NAMING.md)。平台功能规格与优先级见 [docs/FEATURES.md](docs/FEATURES.md)。

## 当前状态

工程骨架阶段（P0）。尚未接入 SiFli-SDK 与板级配置，构建命令暂不可用。

| 阶段 | 目标 | 状态 |
| --- | --- | --- |
| P0 | 仓库、目录骨架、ABI 头 | 进行中 |
| P1 | 语音闭环（VAD / STT / LLM / TTS） | 未开始 |
| P2 | 工具层（注册表 / 调度器 / Schema 校验） | 未开始 |
| P3 | 玩伴层（人格 / 情绪 / 记忆） | 未开始 |
| P4 | 生态（第三方工具 / 侧载 / OTA） | 未开始 |

## 架构

```
┌──────────────────────────────────────────────────────┐
│ 交互层  语音前端 · 角色形象 · 对话界面 · 工具卡片        │
├──────────────────────────────────────────────────────┤
│ AI Core 会话上下文 · 工具注册表 · 调度与超时 · 人格记忆  │
├──────────────────────────────────────────────────────┤
│ 动态应用 .xzp   每个应用 = 页面 + 工具集                │
├──────────────────────────────────────────────────────┤
│ 平台服务  网络 / 存储 / 音频 / 电源 / Web 门户          │
├──────────────────────────────────────────────────────┤
│ RT-Thread + SiFli SDK · LVGL9 · HCPU 交互 + ACPU 计算  │
└──────────────────────────────────────────────────────┘
```

核心契约是**工具**，不是 UI 入口。应用通过 `app.json` 声明工具与参数 Schema，AI 侧靠注册表发现并调用。

## 目录结构

```
.
├─ boards/                 板级适配（引脚、屏、供电）
├─ platform/               HCPU 平台层
│  ├─ ai/                  会话、工具注册表、调度器、LLM 客户端
│  ├─ services/            网络 / 存储 / 音频 / 电源 / 门户
│  ├─ ui/                  桌面 / 对话 / 角色形象 / 设置
│  └─ include/xyz/         对外 ABI，动态应用只允许包含这里
├─ apps/                   动态应用源码，构建产物为 .xzp
├─ compute/                ACPU 计算组件（解码 / 排版 / 渲染）
├─ assets/                 图标、提示音、表情、网页
├─ tools/                  构建准备、资源转换、应用打包
├─ project/                顶层构建、分区表、预装镜像
├─ docs/                   设计文档与决策记录
└─ tests/                  上机用例与压测脚本
```

## 命名规则

| 项 | 取值 |
| --- | --- |
| 仓库 | `Wander-Xanadu-Yonder` |
| C 前缀 | `xyz_`（对外 ABI）/ `wxy_`（内部 UI 与会话框架） |
| 应用包后缀 | `.xzp` |
| 日志 tag | `WXY` |
| 板级构建目标 | `wxy_<board>_hcpu` |
| 应用 ID | 1–7 位小写字母、数字、下划线，不加前缀（如 `timer`） |
| 工具名 | `应用ID.动作`（如 `timer.create`） |
| ABI 版本宏 | `XYZ_ABI_VERSION`，改 ABI 必须升版本并重打全部 `.xzp` |
| 分支 | `main`（可编译）/ `dev` / `feat-*` / `fix-*` |
| 版本 tag | `v0.1.0` 语义化版本 |

## 构建

待接入 SDK 后补全。规划中的流程：

```powershell
. .\SiFli-SDK\export.ps1
python .\tools\prepare_build.py
python -m SCons -C project --board=wxy_<board>_hcpu -j8
python -m SCons -C apps APPS=timer -j8
```

## 许可证

待定（候选 Apache-2.0，与 SiFli 生态保持一致）。第三方 SDK、字库、图标各自保留原许可证。
