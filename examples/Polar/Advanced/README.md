# 极坐标图表高级示例

## 示例概述

这是KD Chart库中的极坐标图表高级示例，展示了如何创建和自定义极坐标图表，包括调整起始位置、显示/隐藏网格等功能。

## 类功能说明

### MainWindow类

MainWindow类是该示例的主窗口，负责初始化图表组件、处理用户交互和显示极坐标图表数据。主要功能包括：

- 初始化UI组件和KD Chart相关类
- 设置图表数据模型
- 处理用户交互事件（如调整起始位置、显示/隐藏网格）
- 更新图表显示

## 代码执行逻辑

1. 应用程序启动时，执行`main`函数：
   - 创建`QApplication`对象
   - 创建`MainWindow`对象并显示
   - 进入应用程序事件循环

2. `MainWindow`构造函数执行以下操作：
   - 调用`setupUi`初始化UI组件
   - 调用`initKDChartClasses`创建KD Chart相关对象
   - 将图表添加到UI布局中
   - 调用`wireUpKDChartClasses`连接图表组件
   - 初始化数据模型并填充数据

3. 用户交互处理：
   - 调整起始位置微调框或滑动条时，更新极坐标平面的起始位置
   - 勾选/取消勾选网格复选框时，显示/隐藏相应的网格

## 使用说明

1. 运行示例程序后，将显示一个极坐标图表窗口
2. 使用"起始位置"微调框或滑动条可以调整极坐标图表的起始角度
3. 勾选"圆形网格"复选框可以显示/隐藏圆形网格
4. 勾选"径向网格"复选框可以显示/隐藏径向网格

## 关键代码解析

- `initKDChartClasses`: 初始化图表、极坐标图表和极坐标坐标平面对象
- `wireUpKDChartClasses`: 连接图表组件，设置数据模型
- `setItemModelData`: 填充数据模型
- `on_startPositionSB_valueChanged`和`on_startPositionSL_valueChanged`: 处理起始位置调整
- `on_circularGridCB_toggled`和`on_sagittalGridCB_toggled`: 处理网格显示/隐藏

## Qt 5.15.2升级说明

本示例代码在Qt 5.15.2下运行正常，无需特别修改。

## C++17兼容性

本示例代码兼容C++17标准，无需特别修改。