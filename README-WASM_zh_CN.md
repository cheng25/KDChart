# KDChart with WebAssembly

## 演示

一个演示版本可在 <https://demos.kdab.com/wasm/kdchart> 获得。

## 构建 KDChart 的提示

- 如果您还没有，请访问 <https://doc.qt.io/qt-5/wasm.html>

- 打开一个适合 WASM 开发的终端（正确的 Qt 和工具链在 PATH 中，等等）

- KDChart 可以用以下方式构建：

```bash
    cmake \
      -DCMAKE_TOOLCHAIN_FILE=/usr/local/emsdk-1.39.8/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
      -DCMAKE_FIND_ROOT_PATH=~/Qt/5.15.1/wasm_32/ -DCMAKE_BUILD_TYPE=Release`
    (Adapt the paths to your own situation)
```

## 构建您自己的应用程序的提示

- 链接到 KDChart（libkdchart.a，或类似的）
