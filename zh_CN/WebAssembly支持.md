# WebAssembly 支持

- 分钟等级: 进阶

WebAssembly (WASM) 允许你在不使用插件的情况下,在网页浏览器中运行高性能应用程序。KDChart 提供了对 WebAssembly 的支持,使你能够创建直接在浏览器中运行的交互式图表和可视化效果,性能接近原生。

KDChart 通过 Emscripten 工具链支持 WebAssembly 编译,允许你在网页应用中充分利用 KDChart 的可视化能力。这意味着你可以为网页部署创建与桌面应用相同的丰富、交互式图表,保持跨平台的一致外观和体验。

来源:README-WASM.md

## WebAssembly 演示

在开始开发之前,你可以查看官方的 KDChart WebAssembly 演示,了解其可能性:

https://demos.kdab.com/wasm/kdchart

该演示展示了 KDChart 在浏览器环境中的能力,并为你的 WebAssembly 应用提供了参考。

来源:README-WASM.md#L3-L5

## 前置条件

要使用 KDChart 和 WebAssembly,你需要:

1. Qt WebAssembly 开发环境 - 按照官方 Qt WebAssembly 设置指南 https://doc.qt.io/qt-5/wasm.html 准备你的环境
2. Emscripten SDK - 将 C++ 编译为 WebAssembly 的工具链
3. CMake - 用于构建过程
4. KDChart 源代码 - 你将为其编译 WebAssembly 的库

来源:README-WASM.md#L7-L11

## 构建 KDChart for WebAssembly

### 设置你的环境

首先,确保你的终端已正确设置为 WebAssembly 开发:

1. 确保正确的 Qt WebAssembly 构建在你的 PATH 中
2. 确保已激活环境中的 Emscripten 工具链
3. 验证 CMake 能找到 Qt 和 Emscripten

来源:README-WASM.md#L11-L12

### 构建过程

要为 WebAssembly 构建 KDChart:

BASH

```
cmake \ -DCMAKE_TOOLCHAIN_FILE=/usr/local/emsdk-1.39.8/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \ -DCMAKE_FIND_ROOT_PATH=~/Qt/5.15.1/wasm_32/ \ -DCMAKE_BUILD_TYPE=Release
```

记得根据你的系统配置调整路径:

* 更新 Emscripten 工具链路径以匹配你的 Emscripten 安装
* 修改 Qt 路径以指向你的 Qt WebAssembly 构建位置

运行 CMake 后,使用以下命令构建库:

BASH

```
make
```

此过程将生成一个静态库文件 (libkdchart.a),你可以将其链接到你的 WebAssembly 应用中。

来源:README-WASM.md#L13-L20

## 创建你自己的 KDChart WebAssembly 应用

### 项目设置

要创建一个新的使用 KDChart 的 Qt 应用:

1. 创建一个带有图表功能的标准 Qt 项目
2. 配置你的项目以使用 KDChart 库
3. 设置 WebAssembly 特定的构建设置

### 链接到 KDChart

在你的 CMake 项目中:

CMAKE

```
# 查找 KDChart 库 find_package(KDChart REQUIRED) # 将你的应用与 KDChart 链接 target_link_libraries(YourApp PRIVATE KDChart::kdchart)
```

对于 QMake 项目,在你的 .pro 文件中包含 KDChart pri 文件:

QMAKE

```
include(/path/to/kdchart.pri)
```

来源:README-WASM.md#L22-L24, CMakeLists.txt#L228-L232

### 构建你的应用

要为 WebAssembly 构建你的应用:

BASH

```
mkdir build-wasm && cd build-wasm cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/emscripten/cmake/Modules/Platform/Emscripten.cmake \ -DCMAKE_PREFIX_PATH=/path/to/qt/wasm_32 \ -DCMAKE_BUILD_TYPE=Release \ .. make
```

## WebAssembly 优化技巧

在开发 KDChart WebAssembly 应用时,考虑以下优化策略:

1. 最小化图表复杂性 - 仅使用必要的图表元素以保持文件大小较小
2. 优化数据加载 - 尽可能异步加载数据
3. 内存管理 - 注意内存使用,因为 WebAssembly 在浏览器的内存空间中运行
4. 文件大小 - 使用带有优化功能的发布构建以最小化 WebAssembly 二进制文件大小
5. 响应式设计 - 确保你的图表能适应不同屏幕尺寸以兼容移动设备

## 部署 WebAssembly 应用

构建完你的 WebAssembly 应用后,你将会有几个文件需要部署:

1. HTML 文件 - 你应用的入口点
2. JS 文件 - 由 Emscripten 生成的 JavaScript 粘合代码
3. WASM 文件 - 包含你的应用和 KDChart 的 WebAssembly 二进制文件
4. 数据文件 - 你的应用需要的任何额外资源

将这些文件上传到一个正确服务 .wasm MIME 类型(application/wasm)的网页服务器。现代网页服务器通常支持这一点,但如果你使用的是旧服务器,可能需要添加 MIME 类型。

## 浏览器兼容性

WebAssembly 在所有现代浏览器中都得到支持,包括:

* Google Chrome(版本 57+)
* Firefox(版本 52+)
* Safari(版本 11+)
* Edge(版本 16+)

然而,一些旧浏览器可能不支持 WebAssembly 或对某些功能的支持有限。始终在不同浏览器和设备上测试你的应用以确保兼容性。

## 调试 WebAssembly 应用

调试 WebAssembly 应用可能具有挑战性。以下是一些技巧:

1. 使用浏览器的开发者工具调试 JavaScript 交互
2. 在构建过程中启用 Emscripten 的调试信息以获得更好的堆栈跟踪
3. 添加日志语句以跟踪应用流程
4. 对于 Qt WebAssembly 应用,Qt 的调试输出将定向到浏览器的控制台

## 结论

KDChart 对 WebAssembly 的支持为在网页上部署丰富、交互式的数据可视化开辟了新的可能性。通过遵循本指南,你可以构建、优化和部署直接在浏览器中运行的 KDChart 应用,为用户提供强大的可视化工具,无论他们使用何种平台。

记得查看 官方 KDChart WebAssembly 演示 以获取灵感和参考实现细节,以便在开发你自己的 WebAssembly 应用时参考。