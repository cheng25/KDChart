# Zoom/ScrollBars 示例

## 概述

本示例展示了如何使用KDChart库实现带滚动条的图表缩放功能。用户可以通过滑动条调整缩放因子，并使用水平和垂直滚动条控制图表的可视区域。

## 功能说明

1. 创建可缩放的折线图图表
2. 支持通过滑动条调整缩放因子
3. 提供水平和垂直滚动条控制图表视图
4. 支持自动调整网格到缩放比例
5. 支持橡皮筋缩放功能
6. 展示图表标题、坐标轴和图例

## 代码结构

本目录包含以下文件：
- `CMakeLists.txt`: 构建配置文件
- `ZoomScrollBars.qrc`: 资源文件
- `main.cpp`: 程序入口文件
- `mainwindow.cpp`: MainWindow类实现文件
- `mainwindow.h`: MainWindow类头文件
- `mainwindow.ui`: UI设计文件

## 执行逻辑

```mermaid
sequenceDiagram
    participant main
    participant QApplication
    participant MainWindow
    participant KDChart::Chart
    participant KDChart::LineDiagram

    main->>QApplication: 初始化
    main->>MainWindow: 创建窗口对象
    MainWindow->>MainWindow: 初始化UI
    MainWindow->>KDChart::Chart: 创建图表对象
    MainWindow->>KDChart::LineDiagram: 创建折线图对象
    MainWindow->>KDChart::LineDiagram: 设置数据模型
    MainWindow->>KDChart::Chart: 添加坐标轴和图例
    main->>MainWindow: 显示窗口
    QApplication->>main: 运行事件循环
```

## 类关系图

```mermaid
classDiagram
    class QApplication
    class QWidget
    class Ui::MainWindow
    class KDChart::Chart
    class KDChart::LineDiagram
    class KDChart::Legend
    class KDChart::CartesianAxis
    class TableModel
    class MainWindow

    QWidget <|-- MainWindow
    Ui::MainWindow <|-- MainWindow
    MainWindow o-- KDChart::Chart : m_chart
    MainWindow o-- TableModel : m_model
    MainWindow o-- KDChart::LineDiagram : m_lines
    MainWindow o-- KDChart::Legend : m_legend
    KDChart::LineDiagram <-- KDChart::CartesianAxis : axes
```

## 关键代码解析

### 1. 主函数 (main.cpp)

- 初始化QApplication
- 创建MainWindow对象并显示
- 启动应用程序事件循环

### 2. MainWindow类 (mainwindow.h/mainwindow.cpp)

- 继承自QWidget和Ui::MainWindow
- 包含图表、数据模型、折线图和图例等成员变量
- 构造函数中初始化UI、创建图表布局、设置数据模型和图表属性
- 实现缩放因子调整、网格调整、滚动条控制等槽函数
- 实现应用新缩放参数的方法

### 3. 信号槽连接

- 连接图表属性变化信号到应用缩放参数槽函数
- 连接滑动条和复选框信号到相应的处理槽函数

## 版本升级说明

### Qt5.15.2升级

1. **已标记的TODO项**:
   - 检查QWidget和Q_OBJECT宏在新版本中的使用
   - 验证KDChart::Chart与Qt5.15.2的兼容性
   - 检查QApplication在新版本中的使用方式
   - 检查QWidget信号槽连接方式在新版本中的变化

2. **潜在影响**:
   - UI渲染和布局可能有细微调整
   - 信号槽连接机制可能发生变化
   - 需要确保KDChart库与Qt5.15.2兼容

### C++17升级

1. **已标记的TODO项**:
   - 考虑使用智能指针管理动态内存
   - 使用结构化绑定优化信号槽连接
   - 使用std::optional处理可能为空的指针
   - 利用结构化绑定优化坐标计算
   - 考虑使用结构化绑定优化代码
   - 检查对C++17标准库的依赖

2. **优化建议**:
   - 使用std::variant替代联合类型
   - 采用C++17的文件系统库处理路径
   - 使用std::any处理不同类型的数据

## 操作指南

- **缩放**：使用缩放因子滑动条调整缩放比例
- **移动**：使用水平和垂直滚动条移动图表视图
- **网格调整**：勾选"Adjust Grid"复选框启用网格自动调整
- **橡皮筋缩放**：勾选"Rubber Band Zoom"复选框启用橡皮筋缩放功能，然后在图表上拖动选择区域