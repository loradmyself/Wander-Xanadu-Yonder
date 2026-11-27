# hello · 动态应用样板

**当前状态：占位，不参与构建。**

这是第一个 `.xzp` 动态应用的预留位置，用来验证"应用与固件解耦"这条链路。
它还没有 C 代码，因为应用 ABI（`platform/include/xyz/app.h`、`tool.h`）尚未冻结——
先写代码再改 ABI，等于把样板写两遍。

## 启用顺序

1. 冻结 `platform/include/xyz/app.h`（应用入口、生命周期、页面挂载）
2. 冻结 `platform/include/xyz/tool.h`（工具声明与 `invoke` 契约）
3. 补 `tools/native_apps.py` + `package_app.py`（`.xzp` 打包与 ABI 符号校验）
4. 写 `hello.c`，在 `project/SConstruct` 里挂上应用包构建

## 清单字段说明

| 字段 | 含义 |
|---|---|
| `abi` | 应用 ABI 版本，装载器据此拒装不兼容的包 |
| `id` | 全局唯一，1–7 位小写字母/数字/下划线；工具名前缀必须等于它 |
| `version` | 应用自身版本，用于升级判断 |
| `tools` | 本应用向 AI 暴露的工具列表，空数组表示纯界面应用 |

`tools` 现在是空的。等 `tool.h` 冻结后，这里会长成：

```json
"tools": [
  { "name": "hello.greet", "schema": "tools/greet.json", "perm": [], "confirm": false }
]
```
