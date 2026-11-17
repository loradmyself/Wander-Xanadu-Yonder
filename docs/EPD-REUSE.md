# EPD-Platform 可复用资产盘点

> 对象：`F:\SiFli\SiFli-EPD-Platform`
> 目的：确定 WXY 平台哪些能力可以继承、哪些必须自建
> 结论日期：2026-11-17

## 一句话结论

**EPD-Platform 不是"类似"，它已经把 WXY 需要的基础设施做完了大半**：服务线程模型、LLM 客户端（三协议）、ASR/TTS 三供应商链路、双核卸载、动态应用装载。

**唯一完全缺失的是「工具执行层」——而这恰好是 WXY 的核心。** 这是一个非常好的起点分布：脏活累活别人做完了，差异化留给自建。

## 一、已有资产（按复用价值排序）

| 资产 | 位置 | 规模 | 具备能力 | 复用度 |
| --- | --- | --- | --- | --- |
| **LLM 客户端** | `src/platform/epd_ai.h` + `src/services/ai/` | 4814 行 | 三种协议（Chat Completions / Responses / Messages）、流式事件、工具调用分片、usage 统计、预测量、取消、20 个错误码 | ★★★★★ |
| **语音链路** | `src/services/speech/` | 16 文件，约 200 KB | 阿里/豆包/腾讯三家 provider、WebSocket + TLS、音频采集/播放背压、ASR 打断 TTS | ★★★★★ |
| **服务线程模型** | `src/services/app_service.h/.c` | — | 服务定义、可取消等待、worker→UI 桥、休眠协调、内存回收保护 | ★★★★★ |
| **应用装载** | `src/ui/platform/app_sap.c`、`app_installer.c`、`app_package.c` | 约 8 万字节 | `.sap` 解析、安装/卸载、签名与完整性、崩溃恢复 | ★★★★ |
| **双核计算卸载** | `src/platform/epd_compute.h/.c` | — | HCPU→ACPU 组件调度、防饿死、取消、会话复用 | ★★★ |
| **内存压力管理** | `src/platform/epd_memory.c`、`app_memory_pressure.c` | — | 分级回收、可回收服务标记、回收租约 | ★★★ |
| **Markdown 与会话 UI** | `modules/ai_demo/src/chat_markdown.c`、`chat_speech.c` | 约 640 行 | 流式文本按块渲染、长文分段朗读、朗读暂停/续读 | ★★★ |

## 二、值得学的具体设计（都是踩坑换来的）

1. **worker → UI 不是无界队列，是"请求跑一步"**
   `epd_service_ui_request(service, delay_ms)` 让 worker 请求 UI 线程在 N 毫秒后执行一次 `ui_process()` 回调。回调必须是有界、非阻塞、可协作的一步。比无界消息队列安全得多——不会因为生产者快而堆爆内存。

2. **跨线程只传值，不传指针**
   `epd_service_publish_summary()` 让 worker 发布一份**副本**，UI 读快照，`revision` 只在内容变化时递增。彻底消除"UI 正在画、worker 把对象释放了"这类野指针问题。

3. **`app_service_start_auxiliary`：辅助 worker 共享所属应用的授权与存储生命周期**
   一个应用可以有多个 worker，但它们随主服务一起停，模块代码引用计数不会被提前释放。这解决了动态应用最容易出的"卸载时还有线程在跑"问题。

4. **音频输入的显式 ack 背压**
   `speech_audio_input_ack(bytes)` 只在网络**真正接受**这些字节后才调用；未 ack 的部分计入"2 秒未发送音频预算"。超预算就暂停采集而不是丢帧。这是嵌入式语音最正确的做法。

5. **音频输出把"重试"写进契约**
   `speech_audio_output_write()` 返回 0 表示背压，文档明确要求**用同一个指针、同一个长度重试**，游标只能按正数结果前进。这个约束避免了"跳帧"和"重复播放半句"。

6. **工具参数分片的正确处理**（`ai_protocol.c: tool_arguments()`）
   - 分片累积，带 `EPD_AI_ARGUMENT_LIMIT` 上限
   - 处理服务端重发全量参数的 `full` 分支，先校验前缀一致再跳过已收部分
   - **只在 `end` 时才 JSON 解析**，且校验顶层必须是 object
   - 解析成功才置 `tool_done` 并发 `TOOL_END`
   
   这正是"流式下 tool_call arguments 分片拼接"的标准答案。

7. **发请求前先量体积**
   `epd_ai_request_measure(..., reserve_encoded_bytes, ...)` 用真实编码器离线算出请求体大小，并为**尚未执行的工具结果**预留空间。判断 `encoded_size + reserve <= 128KB` 才发。这是防止"聊到第 20 轮突然 OOM"的关键机制。

8. **结构体尾部加字段不破 ABI**
   ```c
   #define EPD_AI_REQUEST_V1_SIZE offsetof(epd_ai_request_t, web_search)
   /* Full-width tail keeps this field outside the padding of older requests. */
   ```
   新字段用 64 位宽放在尾部，老应用按 V1_SIZE 填，平台按 struct_size 判断。C 语言里做 ABI 演进的正确姿势。

9. **ASR 打断 TTS 的完整状态机**
   `speech_tts_suspend()` / `speech_tts_quiet()` / `speech_tts_restore()` / `speech_tts_resume_begin()` 一整套：用户开口时把正在朗读的 TTS 暂停并记住播放位置（session + intent），识别结束后恢复到原位置。这是"可打断"体验的真实实现，不是简单 stop。

10. **明确拒绝隐式重试**
    `epd_ai_perform` 注释：`Failed POSTs are never retried.` 重试策略上移到调用方。避免了"超时导致同一句话被扣两次费"。

## 三、致命缺口：工具没有人执行

证据（全平台检索结果）：

| 位置 | 现象 |
| --- | --- |
| `modules/ai_demo/src/demo.c` 的 `on_event()` | 只处理 `EPD_AI_EVENT_TEXT` 与 `SEARCH_*`，`TOOL_START/ARGUMENTS/END` 直接 `return true` 忽略 |
| `modules/ai_demo/src/demo.c:756` | `result.tool_count` 只用于一处条件判断，没有任何分发 |
| `modules/*/app.json` | **无 `tools` 字段**，应用无法声明自己提供哪些工具 |
| 全仓检索 `tool_registry` / `tool_dispatch` | **无匹配** |

**含义：协议解析是完整的，但注册表、执行器、权限、二次确认、超时全部没有。** 也就是说模型能"说出"要调什么工具，但没有任何代码去执行它。

这部分 100% 是 WXY 要自己写的，也正是差异化所在。

## 四、拿来就要改的限制项

| 项 | 现值 | 问题 | 建议 |
| --- | --- | --- | --- |
| `EPD_AI_MAX_TOOLS` | 8 | 工具箱平台远远不够 | 扩到 32；但要重算 RAM——每个工具的 `parameters_json` 是常驻开销，按平均 1.5 KB 计，32 个约 48 KB |
| `EPD_AI_MAX_PROFILES` | 8 | 够用 | 保留 |
| `app.json` | 无 `tools` | 应用无法声明工具 | 扩成清单 v2，加 `tools[]` |
| `EPD_APP_ABI` | 1 | 加 tools 必须升版本 | 升到 2，装载器拒装旧包 |
| 包后缀 | `.sap` | 与 WXY 命名冲突 | 改 `.xzp` |
| 服务归属 | 绑 `epd_service_t *owner` | 平台服务必须挂在某个 service 上 | 沿用——这个约束是对的，别绕 |
| 音频路由 | `speech_media.c` 按板级写死 | 换硬件要改 | 抽象成路由表 |

## 五、路线修正

原计划里 **P0（骨架 2–3 周）与 P1（语音闭环 3–4 周）的很大一部分可以省掉**：

| 原阶段 | 原内容 | 修正后 |
| --- | --- | --- |
| P0 | fork 骨架、剥阅读域、改名、跑通桌面 | 保留，但从"搭"变成"剥离 + 改名"，工作量约减半 |
| P1 | 流式 LLM 客户端、会话管理、STT/TTS 接入 | **几乎全免**——`ai_*` 与 `speech_*` 已有；只剩"接线 + 适配本板音频路由" |
| P2 | 工具注册表 + 调度器 + 3 个内置工具 | **不变，且获得更多时间**——这是唯一真正的空白，应该加大投入 |
| P3 | 人格、情绪、记忆、主动交互 | 不变 |

省下的时间建议全部投到 P2/P3。**协议层不是壁垒，工具生态和人格才是。**

## 六、差异化判断

EPD-Platform 的产品形态是"阅读器 + AI 问答"：AI 是一个**功能入口**，用户主要还是在读书。

WXY 的产品形态是"AI 玩伴 + 工具平台"：AI 是**主体**，应用是它的手脚。

这个差别决定了三件事不能照抄：

1. **不能沿用 `ai_demo` 的交互模型**。它是"提问—等待—全文显示"的单轮问答；玩伴需要常驻会话、可打断、有等待态表情、有主动发起。
2. **不能把工具硬编码在应用里**。EPD 的做法（books/notes 各自在自己代码里调 `epd_ai_perform`）意味着"AI 能力 = 应用自己实现"。WXY 必须反过来：应用只声明工具，平台统一调度。
3. **不需要墨水屏的刷新预算**。EPD 里大量设计（局部刷新预算、状态文本省刷新）是为电子墨水服务的，换 LCD 后可以简化，但要小心别把"省刷新"和"省电"的逻辑一起删掉——省电逻辑仍需要。

## 七、许可证

所有源码文件头部为 `SPDX-License-Identifier: Apache-2.0`。整仓 `LICENSE` 文件需确认；内部使用无碍，对外分发需保留版权声明与许可文本。
