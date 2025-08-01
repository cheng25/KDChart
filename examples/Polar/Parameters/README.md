# 极坐标图表参数示例

## 示例概述

这是KD Chart库中的极坐标图表参数示例，展示了如何创建和自定义极坐标图表，包括设置背景、数据点标记、数据值显示等高级功能。

## 类功能说明

### ChartWidget类

ChartWidget类是该示例的主窗口，负责初始化图表组件、配置图表属性和显示极坐标图表数据。主要功能包括：

- 初始化数据模型并填充示例数据
- 创建和配置极坐标平面
- 创建和配置极坐标图表
- 自定义图表背景、数据点标记和数据值显示
- 设置UI布局

## 代码执行逻辑

1. 应用程序启动时，执行`main`函数：
   - 创建`QApplication`对象
   - 创建`ChartWidget`对象并显示
   - 进入应用程序事件循环

2. `ChartWidget`构造函数执行以下操作：
   - 初始化数据模型并填充示例数据
   - 创建极坐标平面并替换默认的笛卡尔平面
   - 创建极坐标图表并设置数据模型
   - 配置图表背景属性
   - 配置数据值属性（标记样式、大小、位置等）
   - 为特殊单元格设置不同的标记样式
   - 设置图表缩放因子
   - 创建并设置UI布局

## 使用说明

1. 运行示例程序后，将显示一个极坐标图表窗口
2. 图表中展示了示例数据，其中(1,2)单元格的标记样式与其他单元格不同，更大且为圆形

## 关键代码解析

- 数据模型初始化：创建`QStandardItemModel`并填充示例数据
- 极坐标平面配置：创建`PolarCoordinatePlane`并设置背景属性
- 图表配置：创建`PolarDiagram`并设置数据模型
- 数据值属性配置：设置标记样式、大小、位置和文本属性
- 特殊单元格自定义：为特定单元格设置不同的标记样式
- 缩放因子设置：调整图表的缩放比例以获得更好的显示效果

## Qt 5.15.2升级说明

本示例代码在Qt 5.15.2下运行需要注意以下几点：
- 考虑使用`QtWidgets`代替`QtGui`，因为Qt5.15中部分API已移动
- 检查`PolarDiagram` API是否有变更

## C++17兼容性

本示例代码兼容C++17标准，无需特别修改。