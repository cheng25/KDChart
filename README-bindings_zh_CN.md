# KDChart 和 Python 绑定

以下是构建 KDChart Python 绑定的说明。

从 2.7.0 版本开始，KD Chart 可以构建 Python3 绑定。

当前不支持的功能：

- 调试版本构建
- 针对 Qt 调试库的构建
- 静态构建
- Python2 绑定
- 仅部分 32 位平台受支持。请参阅 <https://wiki.qt.io/Qt_for_Python>

此外，目前没有计划支持 qmake 构建系统。

## 先决条件

您需要准备以下内容：

- 支持 C++14 的编译器（Qt6 构建需要支持 C++17）
- Python3.6 或更高版本
- Qt5 5.15 或更高版本
- Qt6 6.2 或更高版本
- Qt 项目提供的 QtForPython。

## 为 Qt5 安装 PySide2

确保您已安装 PySide2、shiboken2 和 shiboken2-generator。
目前，您无法从 PyPi 获取 shiboken2-generator，因为其 wheel 包不在 PyPi 上。
若要使用 wheel 包，请执行以下操作：
```bash
% pip3 install \
    --index-url=http://download.qt.io/official_releases/QtForPython/ \
    --trusted-host download.qt.io \
    shiboken2 pyside2 shiboken2_generator
```

更多信息请访问 <https://doc.qt.io/qtforpython/shiboken2/gettingstarted.html>

之后运行：
```bash
pip3 list | grep PySide
```

注意：版本 *必须* 与您计划用于构建 KD Chart 的 Qt 版本相匹配。

## 为 Qt6 安装 PySide6

遵循 [上一章节](#Install PySide2 for Qt5) 的说明，
只是用 pip3 安装 `shiboken6 pyside6 shiboken6_generator`。

## 构建 KDChart Python 绑定

通过传递 `-DKDChart_PYTHON_BINDINGS=True' 选项来告诉 CMake 构建绑定。
然后像往常一样运行 `cmake --build`。

绑定将安装到 `CMAKE_INSTALL_PREFIX`，这可能需要设置
`PYTHONPATH` 环境变量指向该路径以运行应用程序。

例如，如果您在 linux 上安装到默认位置，您将执行以下操作：

```bash
  export PYTHONPATH=/usr/local/KDAB/KDChart-3.0.0/lib64/python3.10/site-packages
```

或者，通过传递（例如）`-DKDChart_BINDINGS_INSTALL_PREFIX=/usr/lib/python3.8/site-packages` 选项来配置绑定安装位置。
并根据需要调整 PYTHONPATH。

要运行 KDChart python 示例：

```bash
   export PYTHONPATH=/usr/local/KDAB/KDChart-3.0.0/lib64/python3.10/site-packages # adapt as needed
   cd python/examples/view
   python3 main.py
```

### 构建问题

- 如果您看到错误消息“无法定位 Clang 的内置包含目录”
  那么首先确保您已安装 llvm。 如果您仍然有问题，请尝试设置环境变量 `LLVM_INSTALL_DIR` 指向您的 llvm 安装。

  示例：

```bash
    export LLVM_INSTALL_DIR=/usr/local/opt/llvm-11
    set "LLVM_INSTALL_DIR=C:\Program Files\LLVM" #Windows
```

- 当您构建示例时，可能会遇到从 shiboken2_generator 加载共享库的错误。

  尝试：

```bash
    export LD_LIBRARY_PATH=/usr/local/lib/python/dist-packages/PySide2/Qt/lib #linux
    export DYLD_LIBRARY_PATH=/usr/local/lib/python/dist-packages/PySide2/Qt/lib #Mac
    (Adjust to wherever your PySide is installed)
```

- 在 Windows 上，QtForPython 提供的 `libclang.dll` 与 MSVC2019 不兼容。
  要解决此问题，请将 llvm 提供的 `libclang.dll` 复制到 shiboken2 中，如下所示：

```bash
    cd C:\Python37\Lib\site-packages\shiboken2_generator
    copy libclang.dll libclang.dll.save
    copy "C:\Program Files\llvm\bin\libclang.dll" libclang.dll
    (Python3 installation in C:\Python37 and llvm in c:\Program Files\llvm. adjust as needed)
```
