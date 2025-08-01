# SharedAbscissa 示例

## 示例概述

此示例展示了如何使用KD Chart库创建共享横坐标的多个图表。通过在一个图表窗口中显示两个独立的线图，并让它们共享同一个横坐标，演示了复杂数据可视化的实现方法。

## 功能说明

- 创建包含两个独立线图的图表窗口
- 实现两个线图共享同一个横坐标
- 为横坐标设置自定义刻度和注释
- 在不同的坐标平面中显示不同的数据系列
- 支持网格线与注释线的联动显示

## 代码执行逻辑

1. 程序入口点为`main`函数，创建QApplication实例
2. 创建MainWindow实例并显示
3. MainWindow构造函数中：
   - 设置UI界面
   - 创建图表对象和布局
   - 从CSV文件加载两个数据集
   - 创建两个线图并分别关联数据集
   - 创建共享的X轴和两个独立的Y轴
   - 配置坐标轴位置和属性
   - 将线图添加到不同的坐标平面
   - 启用网格线与注释线的联动
4. 启动应用程序事件循环

## 关键代码解析

### MainWindow类定义

```cpp
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    KDChart::Chart *m_chart;                      // 图表对象
    TableModel m_model;                           // 第一个数据模型
    TableModel m_model2;                          // 第二个数据模型
    KDChart::DatasetProxyModel *m_datasetProxy;   // 数据集代理模型
    KDChart::LineDiagram *m_lines;                // 第一个线图
    KDChart::LineDiagram *m_lines2;               // 第二个线图
    KDChart::CartesianCoordinatePlane *plane2;    // 第二个坐标平面
};
```

### 共享横坐标实现

```cpp
// 创建坐标轴
auto *xAxis = new CartesianAxis(m_lines);    // 共享的X轴
auto *yAxis = new CartesianAxis(m_lines);    // 第一个Y轴
auto *yAxis2 = new CartesianAxis(m_lines2);  // 第二个Y轴

// 为线图添加坐标轴
m_lines->addAxis(xAxis);  // 第一个线图添加X轴
m_lines->addAxis(yAxis);  // 第一个线图添加Y轴
m_lines2->addAxis(xAxis); // 第二个线图共享X轴
m_lines2->addAxis(yAxis2); // 第二个线图添加Y轴
```

### 自定义刻度和注释

```cpp
// 设置自定义刻度
QList<qreal> ticks;
ticks.append(5);
ticks.append(10);
ticks.append(15);
xAxis->setCustomTicks(ticks);

// 设置刻度注释
QMultiMap<qreal, QString> annotations;
annotations.insert(5, "Five");    // 5 → Five
annotations.insert(10, "Ten");    // 10 → Ten
annotations.insert(15, "Fifteen");  // 15 → Fifteen
xAxis->setAnnotations(annotations);
```

## Qt 5.15.2升级说明

- 已检查头文件包含和API使用情况
- 添加了相关TODO标记，提示检查以下内容：
  - KDChart API是否有变更
  - QApplication在新版本中的API变化

## C++17升级说明

- 建议使用std::make_unique管理动态内存（如chart、lines、axis等对象）
- 考虑使用结构化绑定优化数据访问
- 可使用constexpr优化常量表达式

## 使用说明

1. 编译并运行示例程序
2. 观察两个线图如何共享同一个横坐标
3. 注意横坐标上的自定义刻度和注释
4. 可以修改数据集或坐标轴属性来适应不同的可视化需求