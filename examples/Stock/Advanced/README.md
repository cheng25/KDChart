# Stock Advanced 示例

## 示例概述

此示例展示了如何使用KD Chart库创建高级股票图表。支持多种股票图表类型（最高价-最低价-收盘价、开盘价-最高价-最低价-收盘价和蜡烛图），并提供了3D效果、颜色定制等高级功能。

## 功能说明

- 支持三种股票图表类型：High-Low-Close、Open-High-Low-Close和Candlestick
- 从CSV文件加载股票数据
- 提供3D效果设置（透视角度、深度、阴影颜色）
- 自定义图表颜色
- 支持图例显示
- 支持坐标轴文本旋转和字体大小调整

## 代码执行逻辑

1. 程序入口点为`main`函数，创建QApplication实例
2. 创建MainWindow实例并显示
3. MainWindow构造函数中：
   - 初始化UI
   - 从CSV文件加载HLC和OHLC数据
   - 创建图表和股票图表对象
   - 设置图表类型为High-Low-Close
   - 创建图例并添加到图表
   - 设置坐标轴属性
   - 连接颜色选择按钮信号槽
   - 初始化图表默认值
4. 响应UI交互：
   - 3D属性切换
   - 透视角度和深度调整
   - 阴影颜色切换
   - 股票类型选择
   - 颜色选择
5. 启动应用程序事件循环

## 关键代码解析

### 主窗口构造函数

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_chart(new Chart())
    , m_diagram(m_chart)
{
    setupUi(this);

    m_HLCModel.loadFromCSV(":/HLC");
    m_OHLCModel.loadFromCSV(":/OHLC");

    m_diagram.setType(StockDiagram::HighLowClose);
    m_diagram.setModel(&m_HLCModel);
    m_chart->coordinatePlane()->replaceDiagram(&m_diagram);
    auto *legend = new KDChart::Legend(&m_diagram, m_chart);
    m_chart->addLegend(legend);

    // ... 其他初始化代码 ...
}
```

### 股票类型切换

```cpp
void MainWindow::on_stockTypeCB_currentIndexChanged(int index)
{
    m_chart->coordinatePlane()->takeDiagram(&m_diagram);

    const QString text = stockTypeCB->itemText(index);
    if (text == "High-Low-Close") {
        m_diagram.setType(StockDiagram::HighLowClose);
        m_diagram.setModel(&m_HLCModel);
    } else if (text == "Open-High-Low-Close") {
        m_diagram.setType(StockDiagram::OpenHighLowClose);
        m_diagram.setModel(&m_OHLCModel);
    } else if (text == "Candlestick") {
        m_diagram.setType(StockDiagram::Candlestick);
        m_diagram.setModel(&m_OHLCModel);
    }

    m_chart->coordinatePlane()->replaceDiagram(&m_diagram);
}
```

### 3D属性设置

```cpp
void MainWindow::on_threeDProperties_toggled(bool checked)
{
    m_threeDBarAttributes.setEnabled(checked);
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
    m_chart->update();
}

void MainWindow::on_perspectiveAngle_valueChanged(int value)
{
    m_threeDBarAttributes.setAngle(value);
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
    m_chart->update();
}
```

### 颜色定制

```cpp
void MainWindow::applyColor(const QColor &color)
{
    if (color.isValid()) {
        m_diagram.setPen(0, QPen(color.darker(130)));
        m_diagram.setBrush(0, QBrush(color));
        QColor inverse(255 - color.red(), 255 - color.green(), 255 - color.blue());
        m_diagram.setPen(1, QPen(inverse.darker(130)));
        m_diagram.setBrush(1, QBrush(inverse));
        // ... 更新按钮颜色 ...
    }
}
```

## Qt 5.15.2升级说明

- 已检查头文件包含和API使用情况
- 添加了相关TODO标记，提示检查以下内容：
  - KDChart API是否有变更
  - QColorDialog API是否有变更

## C++17升级说明

- 建议使用std::make_unique管理动态内存（如m_chart、legend等对象）
- 考虑使用结构化绑定优化数据访问
- 可使用if constexpr优化条件判断（如在on_stockTypeCB_currentIndexChanged函数中）
- 考虑使用override和final修饰符优化虚函数声明

## 使用说明

1. 编译并运行示例程序
2. 从下拉菜单选择不同的股票图表类型
3. 使用复选框和滑块调整3D效果
4. 点击颜色按钮自定义图表颜色
5. 观察图表的变化