# 专业图表(Levey-Jennings, 三元图)

- 分钟等级: 高级

KDChart 提供了超出标准柱状图、折线图和饼图的特殊图表类型。本指南重点介绍两种强大的特殊图表类型:用于质量控制可视化的 Levey-Jennings 图表 和用于三组分组成数据的 三元图表。这些图表类型提供了特定领域的可视化,解决了特定的数据展示挑战。

## Levey-Jennings 图表

Levey-Jennings 图表是广泛应用于实验室质量控制中的专业控制图表,用于跟踪分析过程随时间的变化。它们帮助识别测量数据中的偏移、趋势和异常值,对临床实验室和制造质量控制至关重要。

### 关键概念

Levey-Jennings 图表跟踪测量值相对于以下几点的变化:

* 预期均值(目标值)
* 标准差带(通常为 ±1SD、±2SD 和 ±3SD)
* 时间序列数据点,可以标记为通过或未通过质量控制
* 特殊事件,如设备更换或校准

### 创建基本的 Levey-Jennings 图表

以下是使用 KDChart 创建简单 Levey-Jennings 图表的方法:

```cpp
// 创建图表和图表对象 auto *chart = new KDChart::Chart(); auto *diagram = new KDChart::LeveyJenningsDiagram(); // 设置数据模型 diagram->setModel(&model); // 配置质量控制参数 diagram->setExpectedMeanValue(200); // 目标值 diagram->setExpectedStandardDeviation(20); // 允许的变差 // 创建和设置专业坐标系 auto *plane = new KDChart::LeveyJenningsCoordinatePlane(); chart->replaceCoordinatePlane(plane); plane->replaceDiagram(diagram);
```

来源:main.cpp#L160-L171

### 数据模型结构

KDChart 中的 Levey-Jennings 图表期望模型中包含特定数据:

| 列 | 内容 | 描述 |
| --- | --- | --- |
| 0 | 批次 ID | 标识样本批次 |
| 1 | 测量值 | 实际测量值 |
| 2 | 通过/失败标志 | 指示质量控制状态的布尔值 |
| 3 | 日期/时间 | 测量的时间戳 |
| 4 | 均值(可选) | 自定义均值参考 |
| 5 | 标准差(可选) | 自定义标准差参考 |

每一行代表一个单独的测量点。以下示例展示了如何为 Levey-Jennings 图表填充模型:

```cpp
// 创建一个包含 14 行(数据点)和 6 列的模型 QStandardItemModel model(14, 6); // 设置表头 model.setHeaderData(0, Qt::Horizontal, QObject::tr("批次")); model.setHeaderData(1, Qt::Horizontal, QObject::tr("值")); model.setHeaderData(2, Qt::Horizontal, QObject::tr("通过")); model.setHeaderData(3, Qt::Horizontal, QObject::tr("日期/时间")); // 设置测量点的数据 model.setData(model.index(0, 0), 1); // 批次 1 model.setData(model.index(0, 1), 210); // 值 model.setData(model.index(0, 2), true); // 质控通过 model.setData(model.index(0, 3), QDateTime::fromString("2007-07-06T09:00:00", Qt::ISODate));
```

来源:main.cpp#L54-L112

### Levey-Jennings 图表的专用功能

#### 1. 添加专用轴

```cpp
// 左轴(显示测量值) auto *axis = new KDChart::LeveyJenningsAxis(diagram); axis->setPosition(KDChart::CartesianAxis::Left); diagram->addAxis(axis); // 右轴(显示计算值,如标准差) auto *axis2 = new KDChart::LeveyJenningsAxis(diagram); axis2->setPosition(KDChart::CartesianAxis::Right); axis2->setType(KDChart::LeveyJenningsGridAttributes::Calculated); diagram->addAxis(axis2); // 底轴(显示时间) auto *axis3 = new KDChart::LeveyJenningsAxis(diagram); axis3->setPosition(KDChart::CartesianAxis::Bottom); diagram->addAxis(axis3);
```

来源:main.cpp#L221-L233

#### 2. 标记特殊事件

Levey-Jennings 图表允许标记可能影响测量的重要事件:

```cpp
// 标记流体包更换 diagram->setFluidicsPackChanges( QVector<QDateTime>() << QDateTime::fromString("2007-07-11T15:00:00", Qt::ISODate)); // 标记传感器更换 diagram->setSensorChanges( QVector<QDateTime>() << QDateTime::fromString("2007-07-10T11:00:00", Qt::ISODate));
```

来源:main.cpp#L212-L215

要表示缺失值,只需在模型中不设置该值,但仍提供时间戳。

## 三元图表

三元图表(也称为三角形图或单纯形图)显示总和为常数值的三个变量的比例。它们常用于:

* 材料科学中的相图
* 地质学中的土壤组成
* 化学中的三组分混合物
* 博弈论中的混合策略

### 关键概念

在三元图表中:

* 三角形的每个角代表一个组分的 100%
* 三角形内的任何点代表三个组分的独特混合
* 三个组分的值通常总和为 1.0(或 100%)

### 创建基本的三元图表

以下是使用 KDChart 创建简单三元图表的方法:

```cpp
// 创建图表对象 auto *chart = new KDChart::Chart(); // 创建和设置三元坐标系 auto *ternaryPlane = new KDChart::TernaryCoordinatePlane(); chart->replaceCoordinatePlane(ternaryPlane); // 创建三元点图表 auto *diagram = new KDChart::TernaryPointDiagram(); ternaryPlane->replaceDiagram(diagram); // 添加围绕三角形的三个轴 auto *axisA = new KDChart::TernaryAxis(diagram); axisA->setPosition(KDChartEnums::PositionSouth); // 底部 auto *axisB = new KDChart::TernaryAxis(diagram); axisB->setPosition(KDChartEnums::PositionWest); // 左侧 auto *axisC = new KDChart::TernaryAxis(diagram); axisC->setPosition(KDChartEnums::PositionEast); // 右侧 // 设置数据模型 diagram->setModel(&model);
```

来源:mainwindow.cpp#L34-L56

### 三元图表的数据模型结构

三元图表需要特定的数据结构:

* 每一行代表三元图中的一个数据点
* 每个点需要恰好 3 列,代表三个组分的比例
* 每行的值通常(但不一定)总和为 1.0 或 100%

以下是为三元图表设置数据模型的示例:

```cpp
// 初始化模型,包含 5 行和 3 列 m_model.insertRows(0, 5); m_model.insertColumns(0, 3); // 初始等分布(每个组分 1/3) const float column0Share = 1.0f / 3.0f; const float column1Share = 1.0f / 3.0f; const float column2Share = 1.0f / 3.0f; // 为每行生成数据,略有变化 for (int row = 0; row < m_model.rowCount(); ++row) { // 计算偏移因子以产生变化 const qreal skewX = column0Share * (1 - 1.0 / (5 * row * row * row + 1)); const qreal skewY = skewX; // 设置组分 A 的数据 QModelIndex index = m_model.index(row, 0); m_model.setData(index, QVariant(column0Share - skewX)); // 设置组分 B 的数据 index = m_model.index(row, 1); m_model.setData(index, QVariant(column1Share + skewX - skewY)); // 设置组分 C 的数据 index = m_model.index(row, 2); m_model.setData(index, QVariant(column2Share + skewY)); }
```

来源:mainwindow.cpp#L75-L104

### 三元图表变体

KDChart 提供两种类型的三元图表:

1. TernaryPointDiagram:在三元坐标系中显示单个点,适合离散数据点。
2. TernaryLineDiagram:用线条连接数据点,用于显示三元空间中的趋势或边界。

```cpp
connect(m_diagram, &KDChart::TernaryPointDiagram::clicked, this, &YourClass::handlePointClicked);
```

## 特殊图表类型比较

| 特性 | Levey-Jennings 图表 | 三元图表 |
| --- | --- | --- |
| 主要用途 | 质量控制监测随时间变化 | 三组分的组成 |
| 数据结构 | 带有质控标记的时间序列 | 三种比例组分 |
| 关键类 | LeveyJenningsDiagram | TernaryPointDiagram / TernaryLineDiagram |
| 坐标系 | LeveyJenningsCoordinatePlane | TernaryCoordinatePlane |
| 轴类型 | LeveyJenningsAxis | TernaryAxis |
| 常见行业 | 医学实验室,制造 | 材料科学,化学,地质学 |

## 最佳实践

### 对于 Levey-Jennings 图表

1. 始终设置预期均值和标准差值,为您的质量控制数据提供适当的上下文:

   ```cpp
   diagram->setExpectedMeanValue(200); diagram->setExpectedStandardDeviation(20);
   ```
2. 包含批次信息,以跟踪潜在的批次相关问题。
3. 使用提供的 API 标记设备更换,以帮助将测量偏移与维护事件相关联。

### 对于三元图表

1. 确保您的三个组分总和为一个常数值(通常为 1.0 或 100%)对于每个数据点。
2. 考虑使用 TernaryPointDiagram 用于离散样本,使用 TernaryLineDiagram 显示边界或趋势。
3. 为所有三个轴添加有意义的标签,帮助读者理解每个角代表什么。

## 结论

像 Levey-Jennings 和三元图表这样的特殊图表,使您能够有效地可视化特定领域的数据。虽然它们需要比标准图表更专业的知识,但它们为特定应用提供了强大的洞察力。KDChart 使得实现这些特殊可视化变得简单,帮助您以最小的努力构建行业标准的图表。

对于更复杂的自定义,请参考样式和外观和轴和网格文档部分,这些部分提供了如何进一步自定义这些特殊图表类型的详细信息。