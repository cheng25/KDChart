/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <QApplication>

#include <KDChartChart>
#include <KDChartLeveyJenningsAxis>
#include <KDChartLeveyJenningsCoordinatePlane>
#include <KDChartLeveyJenningsDiagram>
#include <KDChartLeveyJenningsGridAttributes>

#include <QDateTime>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>

/**
 * @brief 选择动画器类
 * 
 * 继承自QObject，用于在表格视图中自动切换选中项，实现动画效果。
 */
class SelectionAnimator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param view 表格视图对象
     * 
     * 创建一个选择动画器实例，并关联到指定的表格视图。
     */
    SelectionAnimator(QAbstractItemView *view)
        : QObject(view)
        , view(view)
    {
        auto *const t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &SelectionAnimator::animate);
        t->start(1000); // 每秒触发一次动画
    }

protected Q_SLOTS:
    /**
     * @brief 动画槽函数
     * 
     * 切换表格视图中的选中项，实现自动选择的动画效果。
     */
    void animate()
    {
        // 计算下一行的索引，循环选择
        const int row = (view->selectionModel()->currentIndex().row() + 1) % view->model()->rowCount();
        view->selectionModel()->setCurrentIndex(view->model()->index(row, 0), QItemSelectionModel::ClearAndSelect);
    }

private:
    QAbstractItemView *const view; // 关联的表格视图对象
};

#include "main.moc"

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建一个Levey-Jennings图表示例，展示质量控制数据的可视化。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    QStandardItemModel model(14, 6); // 创建数据模型，14行6列

    // 设置表头数据
    model.setHeaderData(0, Qt::Horizontal, QObject::tr("批次"));
    model.setHeaderData(1, Qt::Horizontal, QObject::tr("数值"));
    model.setHeaderData(2, Qt::Horizontal, QObject::tr("合格"));
    model.setHeaderData(3, Qt::Horizontal, QObject::tr("日期/时间"));
    model.setHeaderData(4, Qt::Horizontal, QObject::tr("平均值"));
    model.setHeaderData(5, Qt::Horizontal, QObject::tr("标准差"));

    // 批次1数据
    model.setData(model.index(0, 0), 1); // 批次号
    model.setData(model.index(0, 1), 210); // 数值
    model.setData(model.index(0, 2), true); // QC值合格/不合格
    model.setData(model.index(0, 3), QDateTime::fromString("2007-07-06T09:00:00", Qt::ISODate));

    model.setData(model.index(1, 0), 1);
    model.setData(model.index(1, 1), 9.5);
    model.setData(model.index(1, 2), true);
    model.setData(model.index(1, 3), QDateTime::fromString("2007-07-06T21:00:00", Qt::ISODate));
    model.setData(model.index(1, 4), 7.5); // 平均值
    model.setData(model.index(1, 5), 1.0); // 标准差

    model.setData(model.index(2, 0), 1);
    model.setData(model.index(2, 1), 200);
    model.setData(model.index(2, 2), true);
    model.setData(model.index(2, 3), QDateTime::fromString("2007-07-07T09:00:00", Qt::ISODate));

    // 此值应缺失（但批次号仍然需要）
    model.setData(model.index(3, 0), 1);
    model.setData(model.index(3, 3), QDateTime::fromString("2007-07-07T21:00:00", Qt::ISODate));

    model.setData(model.index(4, 0), 1);
    model.setData(model.index(4, 1), 180);
    model.setData(model.index(4, 2), true);
    model.setData(model.index(4, 3), QDateTime::fromString("2007-07-08T09:00:00", Qt::ISODate));

    // 批次2数据
    model.setData(model.index(5, 0), 2);
    model.setData(model.index(5, 1), 210);
    model.setData(model.index(5, 2), true);
    model.setData(model.index(5, 3), QDateTime::fromString("2007-07-08T21:00:00", Qt::ISODate));

    model.setData(model.index(6, 0), 2);
    model.setData(model.index(6, 1), 195);
    model.setData(model.index(6, 2), true);
    model.setData(model.index(6, 3), QDateTime::fromString("2007-07-09T09:00:00", Qt::ISODate));

    // 此值不合格
    model.setData(model.index(7, 0), 2);
    model.setData(model.index(7, 1), 200);
    model.setData(model.index(7, 2), false);
    model.setData(model.index(7, 3), QDateTime::fromString("2007-07-09T21:00:00", Qt::ISODate));

    model.setData(model.index(8, 0), 2);
    model.setData(model.index(8, 1), 210);
    model.setData(model.index(8, 2), true);
    model.setData(model.index(8, 3), QDateTime::fromString("2007-07-10T09:00:00", Qt::ISODate));

    model.setData(model.index(9, 0), 2);
    model.setData(model.index(9, 1), 180);
    model.setData(model.index(9, 2), true);
    model.setData(model.index(9, 3), QDateTime::fromString("2007-07-10T21:00:00", Qt::ISODate));

    // 此值完全超出范围，因此会被截断
    model.setData(model.index(10, 0), 2);
    model.setData(model.index(10, 1), 290);
    model.setData(model.index(10, 2), true);
    model.setData(model.index(10, 3), QDateTime::fromString("2007-07-11T09:00:00", Qt::ISODate));

    // 此值再次合格
    model.setData(model.index(11, 0), 2);
    model.setData(model.index(11, 1), 210);
    model.setData(model.index(11, 2), true);
    model.setData(model.index(11, 3), QDateTime::fromString("2007-07-11T21:00:00", Qt::ISODate));

    model.setData(model.index(12, 0), 2);
    model.setData(model.index(12, 1), 205);
    model.setData(model.index(12, 2), true);
    model.setData(model.index(12, 3), QDateTime::fromString("2007-07-12T09:00:00", Qt::ISODate));

    model.setData(model.index(13, 0), 2);
    model.setData(model.index(13, 1), 204);
    model.setData(model.index(13, 2), true);
    model.setData(model.index(13, 3), QDateTime::fromString("2007-07-12T21:00:00", Qt::ISODate));

    auto *chart = new KDChart::Chart(); // 创建图表对象

    auto *diagram = new KDChart::LeveyJenningsDiagram; // 创建Levey-Jennings图表
    diagram->setModel(&model); // 设置数据模型
    diagram->setExpectedMeanValue(200); // 设置预期平均值
    diagram->setExpectedStandardDeviation(20); // 设置预期标准差
    auto *plane = new KDChart::LeveyJenningsCoordinatePlane; // 创建坐标系
    chart->replaceCoordinatePlane(plane); // 替换图表的坐标系
    plane->replaceDiagram(diagram); // 替换坐标系的图表

    /*diagram->setLotChangedSymbolPosition( Qt::AlignBottom );
    diagram->setSensorChangedSymbolPosition( Qt::AlignTop );
    diagram->setFluidicsPackChangedSymbolPosition( Qt::AlignTop );*/

    // diagram->setScanLinePen( QPen( Qt::green ) );
    // diagram->setSymbol( KDChart::LeveyJenningsDiagram::NotOkDataPoint,
    //                     diagram->symbol( KDChart::LeveyJenningsDiagram::OkDataPoint ) );

    // 设置流体包更换时间点
    diagram->setFluidicsPackChanges(QVector<QDateTime>() << QDateTime::fromString("2007-07-11T15:00:00", Qt::ISODate));
    // 设置传感器更换时间点
    diagram->setSensorChanges(QVector<QDateTime>() << QDateTime::fromString("2007-07-10T11:00:00", Qt::ISODate));

    // 添加左侧坐标轴
    auto *axis = new KDChart::LeveyJenningsAxis(diagram);
    axis->setPosition(KDChart::CartesianAxis::Left);
    diagram->addAxis(axis);

    // 添加右侧坐标轴
    auto *axis2 = new KDChart::LeveyJenningsAxis(diagram);
    axis2->setPosition(KDChart::CartesianAxis::Right);
    axis2->setType(KDChart::LeveyJenningsGridAttributes::Calculated);
    diagram->addAxis(axis2);

    // 添加底部坐标轴
    KDChart::CartesianAxis *axis3 = new KDChart::LeveyJenningsAxis(diagram);
    axis3->setPosition(KDChart::CartesianAxis::Bottom);
    diagram->addAxis(axis3);

    auto *tv = new QTableView; // 创建表格视图
    tv->setModel(&model); // 设置数据模型
    tv->setSelectionModel(diagram->selectionModel()); // 共享选择模型

    auto *splitter = new QSplitter; // 创建分割器
    splitter->addWidget(tv); // 添加表格视图
    splitter->addWidget(chart); // 添加图表

    splitter->show(); // 显示分割器

    new SelectionAnimator(tv); // 创建选择动画器

    return app.exec(); // 运行应用程序事件循环
}
// TODO: Qt5.15.2升级 检查KDChart::LeveyJennings相关类在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QDateTime::fromString在Qt5.15.2中的行为是否有变化
// TODO: C++17升级 考虑使用结构化绑定优化数据模型的设置
// TODO: C++17升级 考虑使用if constexpr优化条件判断逻辑
