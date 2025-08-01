/****************************************************************************
**
** 此文件是KD Chart库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议：MIT
**
****************************************************************************/

#include <QtGui>                   // 包含Qt图形界面组件

#include <QApplication>            // 包含Qt应用程序类

#include <KDChartBackgroundAttributes> // 包含图表背景属性类
#include <KDChartBarDiagram>       // 包含柱状图类
#include <KDChartChart>            // 包含图表类
#include <KDChartFrameAttributes>  // 包含边框属性类
#include <KDChartHeaderFooter>     // 包含页眉页脚类
#include <KDChartPosition>         // 包含位置类
#include <QSqlError>               // 包含SQL错误类
#include <QSqlField>               // 包含SQL字段类
#include <QSqlQuery>               // 包含SQL查询类
#include <QSqlRecord>              // 包含SQL记录类
#include <QSqlTableModel>          // 包含SQL表格模型类

#include <QPixmap>                 // 包含像素图类

using namespace KDChart;

/**
 * @brief 转置代理模型类
 * @details 继承自QAbstractProxyModel，用于交换数据模型中的行和列
 *          实现了行列转置功能，使得可以以不同方式展示数据
 */
class TransposeProxyModel : public QAbstractProxyModel
{
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit TransposeProxyModel(QObject *parent = nullptr)
        : QAbstractProxyModel(parent)
    {
    }

    /**
     * @brief 析构函数
     */
    ~TransposeProxyModel() override
    {
    }

    /**
     * @brief 将源模型索引映射到代理模型索引
     * @param sourceIndex 源模型索引
     * @return 对应的代理模型索引
     * @details 交换行和列，实现转置
     */
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override
    {
        return index(sourceIndex.column(), sourceIndex.row());
    }

    /**
     * @brief 将代理模型索引映射到源模型索引
     * @param proxyIndex 代理模型索引
     * @return 对应的源模型索引
     * @details 交换行和列，实现转置
     */
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override
    {
        return sourceModel()->index(proxyIndex.column(), proxyIndex.row());
    }

    /**
     * @brief 创建代理模型索引
     * @param r 行号
     * @param c 列号
     * @param ind 父索引
     * @return 创建的索引
     */
    QModelIndex index(int r, int c, const QModelIndex &ind = QModelIndex()) const override
    {
        Q_UNUSED(ind)
        return createIndex(r, c);
    }

    /**
     * @brief 获取父索引
     * @param 子索引
     * @return 父索引
     */
    QModelIndex parent(const QModelIndex &) const override
    {
        return QModelIndex();
    }

    /**
     * @brief 获取行数
     * @param 父索引
     * @return 行数
     * @details 返回源模型的列数（因为转置了）
     */
    int rowCount(const QModelIndex &) const override
    {
        return sourceModel()->columnCount();
    }

    /**
     * @brief 获取列数
     * @param 父索引
     * @return 列数
     * @details 返回源模型的行数（因为转置了）
     */
    int columnCount(const QModelIndex &) const override
    {
        return sourceModel()->rowCount();
    }

    /**
     * @brief 获取数据
     * @param ind 索引
     * @param role 数据角色
     * @return 数据值
     */
    QVariant data(const QModelIndex &ind, int role) const override
    {
        return sourceModel()->data(mapToSource(ind), role);
    }
}; // TODO: C++17升级 考虑使用override和final修饰符

/**
 * @brief 图表窗口类
 * @details 继承自QWidget，用于展示从SQL数据库获取的数据
 *          该类创建一个SQLite数据库，添加测试数据，并使用KD Chart显示数据
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件指针
     * @details 初始化图表窗口，创建SQLite数据库，添加测试数据，设置柱状图
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 添加SQLite数据库
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName(":memory:"); // 使用内存数据库而非文件
        // db.setUserName("");
        // db.setPassword("");
        bool ok = db.open();
        Q_ASSERT(ok);
        Q_UNUSED(ok) // 避免release模式下的警告

        // 创建表
        QSqlQuery createTableQuery = db.exec("CREATE TABLE IF NOT EXISTS MyTable (col1 INT NOT NULL PRIMARY KEY, col2 INT);");
        Q_ASSERT(!createTableQuery.lastError().isValid());

        // 创建SQL表格模型
        m_model = new QSqlTableModel(this, db);
        m_model->setTable("MyTable");
        m_model->setEditStrategy(QSqlTableModel::OnRowChange);
        m_model->setSort(0, Qt::AscendingOrder);

        // 选择数据
        ok = m_model->select();
        Q_ASSERT(ok);

        // 设置表头数据
        m_model->setHeaderData(0, Qt::Horizontal, tr("Column 1"));
        m_model->setHeaderData(1, Qt::Horizontal, tr("Column 2"));

        // 添加测试数据
        for (int row = 0; row < 3; ++row) {
            QSqlRecord rec;
            for (int column = 0; column < 2; ++column) {
                QSqlField field(column == 0 ? "col1" : "col2", QVariant::Int);
                field.setValue(row + 1 * column);
                rec.append(field);
            }
            ok = m_model->insertRecord(-1, rec);
            Q_ASSERT(ok);
        }

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

        // 配置坐标平面边框属性
        FrameAttributes pfa;
        pfa.setPen(QPen(QBrush(Qt::blue), 2));
        pfa.setVisible(true);
        diagram->coordinatePlane()->setFrameAttributes(pfa);

        // 配置页眉边框属性
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2));
        hfa.setPadding(2);
        hfa.setVisible(true);
        header->setFrameAttributes(hfa);

        // 创建垂直布局
        auto *l = new QVBoxLayout(this);
        // 将图表添加到布局
        l->addWidget(&m_chart);
        // 设置窗口布局
        setLayout(l);
    }

private:
    Chart m_chart;                 // 图表对象
    QSqlTableModel *m_model = nullptr; // SQL表格模型
}; // TODO: Qt5.15.2升级 检查QSqlTableModel和QSqlDatabase在新版本中的API变化

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 创建Qt应用程序实例，初始化图表窗口并显示，启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    // 创建Qt应用程序实例
    QApplication app(argc, argv);

    // 创建图表窗口实例
    ChartWidget w;
    // 显示图表窗口
    w.show();

    // 启动应用程序事件循环
    return app.exec();
} // TODO: Qt5.15.2升级 检查QApplication在新版本中的API变化

#include "main.moc"
