# API 参考

- 分钟等级: 高级

本文档提供了 KDChart 库 API 的全面参考,帮助开发者了解可用的类、方法和它们的用法模式。KDChart 是一个功能强大且灵活的基于 Qt 的图表库,支持各种图表类型和广泛的定制选项。

KDChart 遵循一个结构良好的面向对象设计模式,其中图表的创建和操作是通过具有特定职责的类的层次结构来完成的。下图显示了库的整体架构:

来源:KDChartWidget.h,KDChartChart.h

## 核心组件

### KDChart::Widget

KDChart::Widget 类提供了一种最简单的方法来创建图表,而无需使用模型/视图架构。这是初学者的推荐入门点。

#### 关键方法:

| 方法 | 描述 |
| --- | --- |
| setDataset(int column, const QVector<qreal> &data, const QString &title = QString()) | 为列设置 Y 值数据 |
| setDataset(int column, const QVector<QPair<qreal, qreal>> &data, const QString &title = QString()) | 为列设置 X,Y 值对 |
| setDataCell(int row, int column, qreal data) | 在指定位置设置单个 Y 值 |
| setDataCell(int row, int column, QPair<qreal, qreal> data) | 在指定位置设置单个 X,Y 值对 |
| resetData() | 清除所有数据 |
| setType(ChartType chartType, SubType subType = Normal) | 设置图表类型和子类型 |
| setSubType(SubType subType) | 仅设置图表的子类型 |

#### 图表类型和子类型:

CPP

```
enum ChartType { NoType, Bar, Line, Plot, Pie, Ring, Polar }; enum SubType { Normal, Stacked, Percent, Rows };
```

#### 访问特定图表类型:

| 方法 | 描述 |
| --- | --- |
| barDiagram() | 如果适用,则返回图表作为 BarDiagram |
| lineDiagram() | 如果适用,则返回图表作为 LineDiagram |
| plotter() | 如果适用,则返回图表作为 Plotter |
| pieDiagram() | 如果适用,则返回图表作为 PieDiagram |
| ringDiagram() | 如果适用,则返回图表作为 RingDiagram |
| polarDiagram() | 如果适用,则返回图表作为 PolarDiagram |

来源:KDChartWidget.h

### KDChart::Chart

KDChart::Chart 类是模型/视图兼容的图表小部件。它比 KDChart::Widget 更灵活,但需要更多的设置代码。

#### 关键方法:

| 方法 | 描述 |
| --- | --- |
| coordinatePlane() | 返回坐标平面 |
| addCoordinatePlane(AbstractCoordinatePlane\* plane) | 添加坐标平面 |
| takeCoordinatePlane(AbstractCoordinatePlane\* plane) | 移除坐标平面,但不删除它 |
| replaceCoordinatePlane(AbstractCoordinatePlane\* plane, AbstractCoordinatePlane\* oldPlane = nullptr) | 替换坐标平面 |

来源:KDChartChart.h

## 图表类

### AbstractDiagram

AbstractDiagram 类是所有图表类型的基类。它为所有图表提供公共功能。

#### 关键方法:

| 方法 | 描述 |
| --- | --- |
| setModel(QAbstractItemModel\* model) | 设置数据模型 |
| model() | 返回数据模型 |
| setAttributesModel(AttributesModel\* model) | 设置属性模型 |
| coordinatePlane() | 返回坐标平面 |
| setPen(const QPen& pen) | 设置用于绘制的笔 |
| setBrush(const QBrush& brush) | 设置用于填充的画笔 |

来源:KDChartAbstractDiagram.h

### 笛卡尔图表

#### AbstractCartesianDiagram

所有使用笛卡尔坐标系统的图表的基类。

#### BarDiagram

将数据表示为垂直或水平条。

CPP

```
// 创建一个简单的条形图 KDChart::Widget widget; widget.setType(KDChart::Widget::Bar); QVector<qreal> data = {1.2, 2.4, 3.6, 4.8, 6.0}; widget.setDataset(0, data, "示例数据");
```

#### LineDiagram

将数据表示为连接点的线。

CPP

```
// 创建一个折线图 KDChart::Widget widget; widget.setType(KDChart::Widget::Line); QVector<qreal> data = {1.2, 2.4, 3.6, 4.8, 6.0}; widget.setDataset(0, data, "示例数据");
```

#### Plotter

具有额外功能的更高级的折线图,用于散点图。

CPP

```
// 创建一个散点图 KDChart::Widget widget; widget.setType(KDChart::Widget::Plot); // 使用 X-Y 对 QVector<QPair<qreal, qreal>> data; data << qMakePair(1.0, 2.5) << qMakePair(2.0, 3.5) << qMakePair(3.0, 4.5); widget.setDataset(0, data, "示例数据");
```

来源:KDChartAbstractCartesianDiagram.h,KDChartBarDiagram.h,KDChartLineDiagram.h,KDChartPlotter.h

### 极坐标图表

#### AbstractPolarDiagram

所有使用极坐标系统的图表的基类。

#### PieDiagram

将数据表示为饼图的切片。

CPP

```
// 创建一个饼图 KDChart::Widget widget; widget.setType(KDChart::Widget::Pie); QVector<qreal> data = {15.0, 25.0, 35.0, 25.0}; widget.setDataset(0, data, "示例数据");
```

#### RingDiagram

将数据表示为带扇区的同心圆环。

CPP

```
// 创建一个环形图 KDChart::Widget widget; widget.setType(KDChart::Widget::Ring); QVector<qreal> data = {15.0, 25.0, 35.0, 25.0}; widget.setDataset(0, data, "示例数据");
```

#### PolarDiagram

在极坐标系统中表示数据。

CPP

```
// 创建一个极坐标图 KDChart::Widget widget; widget.setType(KDChart::Widget::Polar); QVector<qreal> data = {1.2, 2.4, 3.6, 4.8, 6.0, 4.8, 3.6, 2.4}; widget.setDataset(0, data, "示例数据");
```

来源:KDChartAbstractPolarDiagram.h,KDChartPieDiagram.h,KDChartRingDiagram.h,KDChartPolarDiagram.h

### 三元图表

#### AbstractTernaryDiagram

三元坐标系统(在三角形上表示 3 变量数据)中图表的基类。

#### TernaryLineDiagram

将三元数据表示为线条。

#### TernaryPointDiagram

将三元数据表示为点。

CPP

```
// 使用三元图表需要使用模型/视图方法进行更多设置 KDChart::Chart chart; KDChart::TernaryCoordinatePlane* plane = new KDChart::TernaryCoordinatePlane(&chart); chart.addCoordinatePlane(plane); KDChart::TernaryPointDiagram* diagram = new KDChart::TernaryPointDiagram(); diagram->setModel(model); // 需要使用适当的数据设置模型 plane->addDiagram(diagram);
```

来源:KDChartAbstractTernaryDiagram.h,KDChartTernaryLineDiagram.h,KDChartTernaryPointDiagram.h

### 特殊图表

#### LeveyJenningsDiagram

用于实验室环境中的质量控制图表的专用图表。

#### StockDiagram

表示股票市场数据(开盘价、最高价、最低价、收盘价)。

CPP

```
// 创建一个股票图表 KDChart::Chart chart; KDChart::CartesianCoordinatePlane* plane = new KDChart::CartesianCoordinatePlane(&chart); chart.addCoordinatePlane(plane); KDChart::StockDiagram* diagram = new KDChart::StockDiagram(); diagram->setModel(model); // 包含 OHLC 数据的模型 plane->addDiagram(diagram);
```

来源:KDChartLeveyJenningsDiagram.h,KDChartStockDiagram.h

## 坐标平面

### AbstractCoordinatePlane

所有坐标平面的基类。

### CartesianCoordinatePlane

提供笛卡尔坐标系。

CPP

```
// 创建笛卡尔坐标平面并添加自定义设置 KDChart::Chart chart; KDChart::CartesianCoordinatePlane* plane = new KDChart::CartesianCoordinatePlane(&chart); chart.addCoordinatePlane(plane); // 配置网格 KDChart::CartesianGrid* grid = dynamic_cast<KDChart::CartesianGrid*>(plane->grid()); grid->setGranularitySequence(QList<qreal>() << 1.0 << 2.0 << 5.0);
```

### PolarCoordinatePlane

提供极坐标系。

### RadarCoordinatePlane

提供雷达(星形)坐标系。

### TernaryCoordinatePlane

提供三元坐标系。

来源:KDChartAbstractCoordinatePlane.h,KDChartCartesianCoordinatePlane.h,KDChartPolarCoordinatePlane.h,KDChartRadarCoordinatePlane.h,KDChartTernaryCoordinatePlane.h

## 轴和网格

### AbstractAxis

所有轴的基类。

### CartesianAxis

表示笛卡尔坐标系中的轴。

CPP

```
// 配置轴 KDChart::CartesianAxis* xAxis = new KDChart::CartesianAxis(diagram); xAxis->setPosition(KDChart::CartesianAxis::Bottom); xAxis->setTitleText("X 轴"); diagram->addAxis(xAxis); KDChart::CartesianAxis* yAxis = new KDChart::CartesianAxis(diagram); yAxis->setPosition(KDChart::CartesianAxis::Left); yAxis->setTitleText("Y 轴"); diagram->addAxis(yAxis);
```

### AbstractGrid

所有网格的基类。

### CartesianGrid

表示笛卡尔坐标系中的网格。

### PolarGrid

表示极坐标系中的网格。

来源:KDChartAbstractAxis.h,KDChartCartesianAxis.h,KDChartAbstractGrid.h,KDChartCartesianGrid.h,KDChartPolarGrid.h

## 样式和外观

### 属性类

KDChart 提供了各种属性类来定制图表元素的外观:

| 属性类 | 描述 |
| --- | --- |
| BackgroundAttributes | 控制背景外观 |
| DataValueAttributes | 控制数据值的显示方式 |
| FrameAttributes | 控制框架的外观 |
| GridAttributes | 控制网格的外观 |
| LineAttributes | 控制线条的外观 |
| MarkerAttributes | 控制数据点标记的外观 |
| TextAttributes | 控制文本的外观 |
| ThreeDBarAttributes | 控制 3D 条的外观 |
| ThreeDLineAttributes | 控制 3D 线的外观 |
| ThreeDPieAttributes | 控制 3D 饼图切片的外观 |

CPP

```
// 样式化条形图 KDChart::BarDiagram* barDiagram = widget.barDiagram(); if (barDiagram) { // 设置条形属性 KDChart::BarAttributes ba = barDiagram->barAttributes(); ba.setGapBetweenBars(0.2); ba.setGapBetweenGroups(0.5); barDiagram->setBarAttributes(ba); // 使其 3D KDChart::ThreeDBarAttributes tda = barDiagram->threeDBarAttributes(); tda.setEnabled(true); tda.setDepth(10); tda.setAngle(15); barDiagram->setThreeDBarAttributes(tda); }
```

来源:KDChartBackgroundAttributes.h,KDChartBarAttributes.h,KDChartThreeDBarAttributes.h

## 标题、页脚和图例

### HeaderFooter

表示图表中的标题或页脚。

CPP

```
// 添加标题和页脚 widget.addHeaderFooter("我的图表标题", KDChart::HeaderFooter::Header, KDChart::Position::North); widget.addHeaderFooter("数据来源:示例公司", KDChart::HeaderFooter::Footer, KDChart::Position::South);
```

### Legend

表示图表中的图例。

CPP

```
// 添加图例 widget.addLegend(KDChart::Position::East);
```

来源:KDChartHeaderFooter.h,KDChartLegend.h

## 数据处理

### AttributesModel

存储各个数据点的属性。

CPP

```
// 特定数据点的自定义属性 KDChart::BarDiagram* barDiagram = widget.barDiagram(); if (barDiagram) { // 突出显示第二个条 QModelIndex index = barDiagram->model()->index(1, 0); KDChart::DataValueAttributes dva = barDiagram->dataValueAttributes(index); KDChart::TextAttributes ta = dva.textAttributes(); ta.setVisible(true); dva.setTextAttributes(ta); dva.setVisible(true); barDiagram->setDataValueAttributes(index, dva); }
```

### DatasetProxyModel

允许从模型中选择特定的数据集。

来源:KDChartAttributesModel.h,KDChartDatasetProxyModel.h

## KDGantt 模块

KDGantt 模块提供用于创建甘特图的类。

### KDGantt::View

用于显示甘特图的主类。

CPP

```
// 基本甘特图设置 KDGantt::View view; QStandardItemModel model; // 使用适当的列设置模型 // ... view.setModel(&model);
```

### KDGantt::GraphicsView

KDGantt::View 使用的图形视图。

### KDGantt::AbstractGrid

甘特图网格的基类。

### KDGantt::DateTimeGrid

甘特图的时间基础网格。

来源:kdganttview.h,kdganttgraphicsview.h,kdganttabstractgrid.h,kdganttdatetimegrid.h

## Python 绑定

KDChart 通过 PyKDChart 模块提供 Python 绑定。

PYTHON

```
# Python 示例 from PyKDChart import KDChart widget = KDChart.Widget() widget.setType(KDChart.Widget.Bar) # 设置数据和自定义类似于 C++ 方法
```

来源:PyKDChart

## WebAssembly 支持

KDChart 可以编译为 WebAssembly,允许它在网页浏览器中使用。

此 API 参考提供了 KDChart 主要组件的概述。有关特定类和方法的更多详细信息,请参阅源代码中的头文件或库附带的全面文档。