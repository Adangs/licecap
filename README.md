# LICEcap

LICEcap 是一个轻量的屏幕录制工具，可以在 Windows 和 macOS 上录制指定屏幕区域，并输出动画 GIF。项目还支持 LICE 原生 LCF 格式。本仓库基于 LICEcap 1.32 源码，并新增了简体中文运行时界面本地化。

## 功能

- 拖动并调整捕获窗口，先确定要录制的屏幕区域。
- 录制为 `.gif` 或 `.lcf`。
- 可设置最高帧率（默认 8 FPS）、GIF 循环次数和自动停止时间。
- 捕获选项包括标题帧、经过时间、鼠标按键、大字体、Shift+Space 暂停和 GIF 透明度。
- 支持录制期间插入文字帧。
- 简体中文界面通过系统语言自动启用：Windows 使用用户界面语言，macOS 使用首选语言；两者都无法识别时回退读取 `LC_ALL`、`LC_MESSAGES` 或 `LANG`。非 `zh`/`cmn` 环境保持英文界面。

## 仓库结构

| 目录/文件 | 说明 |
| --- | --- |
| `licecap/` | LICEcap 应用源码、Windows 工程、macOS Xcode 工程和打包脚本 |
| `licecap/licecap_ui.cpp` | 主界面与捕获逻辑，内部文案以英文为基准 |
| `licecap/licecap_ui_zh.cpp` | 简体中文翻译表和运行时本地化实现 |
| `WDL/` | Cockos WDL、SWELL、LICE、zlib、giflib、libpng 等第三方与支撑代码 |
| `licecap/license.txt` | LICEcap 使用的 GPL-2.0-or-later 许可证 |
| `licecap/whatsnew.txt` | 上游版本历史 |

## 构建准备

本仓库已包含构建所需的 WDL 子树，不需要单独下载第三方库。

### macOS

要求 macOS SDK 与 Xcode。工程声明 `MACOSX_DEPLOYMENT_TARGET = 10.7`，构建目标为当前 Xcode 支持的标准 64 位架构。

```sh
xcodebuild -project licecap/licecap.xcodeproj -configuration Release build
```

产物通常位于 `licecap/build/Release/licecap.app`。

### Windows

使用 Visual Studio 打开 `licecap/licecap/licecap.sln`。工程配置为 Win32 与 x64，工具集为 `v120`（对应 Visual Studio 2013）。如果你的 Visual Studio 版本较新，需要在项目属性中重定向平台工具集后再编译。

也可以用 MSBuild：

```powershell
msbuild licecap\licecap\licecap.vcxproj /p:Configuration=Release /p:Platform=x64
```

## 使用

1. 启动 `licecap`，拖动半透明捕获窗口覆盖目标区域。
2. 调整窗口输入框中的宽度和高度，必要时设置最高帧率。
3. 选择 **Record...**，指定 `.gif` 或 `.lcf` 输出文件。
4. 再次点击按钮暂停或继续；点击 **Stop** 结束录制。
5. 暂停时选择 **Insert...**，可以插入文字帧。

Shift+Space 暂停功能需要在保存对话框中启用；启用后，Windows 上还会提供 `Ctrl+Alt+P` 作为暂停快捷键。

## 开发说明

- 修改英文界面文案时，同步检查 `licecap/licecap_ui_zh.cpp` 的翻译表。翻译函数按完整字符串精确匹配，未命中的字符串会继续显示英文。
- Windows 工程的字符集为 MultiByte；中文模块在设置控件文本时显式转换为 UTF-16，以减少乱码风险。
- `requires_wdl.txt` 是上游遗留的依赖提示。当前仓库已直接包含 WDL 子树，所以按本 README 构建时不需要按该文件重新布置依赖。

## 许可证

LICEcap 源码遵循 [GNU General Public License v2 or later](licecap/license.txt)。`WDL` 及其包含的第三方代码各有独立许可与版权声明，重新分发前请检查对应目录下的许可文件。
