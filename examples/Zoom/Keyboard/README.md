# Zoom/Keyboard 示例

## 概述

本示例展示了如何使用KDChart库实现通过键盘进行图表缩放和移动的功能。用户可以使用PageUp/PageDown键进行缩放，使用方向键进行平移操作。

## 功能说明

1. 创建可缩放的图表部件
2. 支持通过键盘进行缩放和移动操作
3. 展示多个数据集的图表
4. 演示图表标题、图例和操作提示的添加

## 代码结构

本目录包含以下文件：
- `CMakeLists.txt`: 构建配置文件
- `main.cpp`: 程序入口文件
- `mainwindow.cpp`: MainWindow类实现文件
- `mainwindow.h`: MainWindow类头文件
- `mainwindow.ui`: UI设计文件
- `zoomwidget.cpp`: ZoomWidget类实现文件
- `zoomwidget.h`: ZoomWidget类头文件

## 执行逻辑

```mermaid
sequenceDiagram
    participant main
    participant QApplication
    participant MainWindow
    participant ZoomWidget
    participant KDChart::Widget

    main->>QApplication: 初始化
    main->>MainWindow: 创建窗口对象
    MainWindow->>ZoomWidget: 创建缩放部件
    ZoomWidget->>KDChart::Widget: 继承并扩展功能
    main->>MainWindow: 设置数据集
    main->>KDChart::Widget: 配置图表属性
    main->>MainWindow: 显示窗口
    QApplication->>main: 运行事件循环
```

## 类关系图

```mermaid
classDiagram
    class QApplication
    class QWidget
    class KDChart::Widget
    class MainWindow
    class ZoomWidget
    class Ui::MainWindow

    QWidget <|-- KDChart::Widget
    QWidget <|-- MainWindow
    KDChart::Widget <|-- ZoomWidget
    Ui::MainWindow <|-- MainWindow

    MainWindow o-- ZoomWidget : contains
    MainWindow o-- KDChart::Widget : widget
```

## 关键代码解析

### 1. 主函数 (main.cpp)

- 初始化QApplication
- 创建MainWindow对象并设置大小
- 准备三个数据集并添加到图表
- 配置图表标题、图例和操作提示
- 显示窗口并启动事件循环

### 2. MainWindow类 (mainwindow.h/mainwindow.cpp)

- 继承自QWidget和Ui::MainWindow
- 包含一个KDChart::Widget类型的成员变量widget
- 构造函数中设置UI、创建图表布局和ZoomWidget对象
- 设置全局右边距

### 3. ZoomWidget类 (zoomwidget.h/zoomwidget.cpp)

- 继承自KDChart::Widget
- 重写鼠标按下事件、滚轮事件和键盘事件处理函数
- 实现findNewZoomCenter方法计算缩放中心
- 支持通过PageUp/PageDown键进行缩放，方向键进行平移

## 版本升级说明

### Qt5.15.2升级

1. **已标记的TODO项**:
   - 检查QApplication和QVector在新版本中的使用方式
   - 验证KDChart::Widget与Qt5.15.2的兼容性
   - 检查事件处理函数在新版本中的变化
   - 确认头文件包含和命名空间使用是否需要调整

2. **潜在影响**:
   - 事件处理机制可能发生变化
   - UI渲染和布局可能有细微调整
   - 需要确保KDChart库与Qt5.15.2兼容

### C++17升级

1. **已标记的TODO项**:
   - 考虑使用std::vector替代QVector以利用C++17特性
   - 使用结构化绑定和if constexpr优化代码结构
   - 检查C++17对Qt容器的影响
   - 优化算法和数据处理逻辑

2. **优化建议**:
   - 使用std::optional处理可能为空的返回值
   - 利用std::variant替代联合类型
   - 采用C++17的文件系统库处理路径

## 操作指南

- **缩放**：使用PageUp(放大)和PageDown(缩小)键
- **移动**：使用方向键(上、下、左、右)
- **鼠标操作**：点击鼠标可以设置缩放中心，滚轮可以缩放