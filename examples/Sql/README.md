# Sql 示例

## 示例概述

此示例展示了如何使用KD Chart库结合SQL数据库创建图表。通过SQLite内存数据库存储数据，使用QSqlTableModel访问数据，并通过转置代理模型实现数据的行列交换，最终使用柱状图展示数据。

## 功能说明

- 创建SQLite内存数据库
- 定义数据表结构并插入测试数据
- 使用QSqlTableModel管理数据库数据
- 实现行列转置代理模型
- 使用KD Chart的BarDiagram展示数据
- 自定义图表边框和页眉属性

## 代码执行逻辑

1. 程序入口点为`main`函数，创建QApplication实例
2. 创建ChartWidget实例并显示
3. ChartWidget构造函数中：
   - 创建SQLite内存数据库
   - 创建数据表MyTable
   - 初始化QSqlTableModel并设置表结构
   - 插入测试数据
   - 创建BarDiagram柱状图
   - 创建TransposeProxyModel转置代理模型
   - 设置图表的页眉和边框属性
   - 配置并显示图表
4. 启动应用程序事件循环

## 关键代码解析

### TransposeProxyModel类

```cpp
class TransposeProxyModel : public QAbstractProxyModel
{
public:
    explicit TransposeProxyModel(QObject *parent = nullptr);
    ~TransposeProxyModel() override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex index(int r, int c, const QModelIndex &ind = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &) const override;
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &ind, int role) const override;
};
```

### 数据库操作

```cpp
// 创建SQLite数据库
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
db.setHostName("localhost");
db.setDatabaseName(":memory:"); // 使用内存数据库
bool ok = db.open();

// 创建表
QSqlQuery createTableQuery = db.exec("CREATE TABLE IF NOT EXISTS MyTable (col1 INT NOT NULL PRIMARY KEY, col2 INT);");

// 初始化数据模型
m_model = new QSqlTableModel(this, db);
m_model->setTable("MyTable");
m_model->setEditStrategy(QSqlTableModel::OnRowChange);
m_model->setSort(0, Qt::AscendingOrder);

// 插入测试数据
for (int row = 0; row < 3; ++row) {
    QSqlRecord rec;
    for (int column = 0; column < 2; ++column) {
        QSqlField field(column == 0 ? "col1" : "col2", QVariant::Int);
        field.setValue(row + 1 * column);
        rec.append(field);
    }
    ok = m_model->insertRecord(-1, rec);
}
```

### 图表设置

```cpp
// 创建柱状图
auto *diagram = new BarDiagram;

// 创建转置代理模型
auto *proxymodel = new TransposeProxyModel(this);
proxymodel->setSourceModel(m_model);
diagram->setModel(proxymodel);

// 设置图表的图表
m_chart.coordinatePlane()->replaceDiagram(diagram);

// 添加页眉
auto *header = new HeaderFooter(&m_chart);
header->setPosition(Position::North);
header->setText("简单柱状图");
m_chart.addHeaderFooter(header);
```

## Qt 5.15.2升级说明

- 已检查头文件包含和API使用情况
- 添加了相关TODO标记，提示检查以下内容：
  - QSqlTableModel和QSqlDatabase在新版本中的API变化
  - QApplication在新版本中的API变化

## C++17升级说明

- 建议使用std::make_unique管理动态内存（如diagram、proxymodel等对象）
- 考虑使用override和final修饰符优化虚函数声明
- 可使用结构化绑定优化数据访问

## 使用说明

1. 编译并运行示例程序
2. 观察基于SQL数据的柱状图展示
3. 可以修改数据库结构或数据来观察图表变化
4. 可以调整转置代理模型的实现来改变数据展示方式