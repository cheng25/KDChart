/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例文件，展示Levey-Jennings图表的实现。
** Levey-Jennings图表是一种特殊类型的控制图，广泛应用于临床实验室质量控制。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/
// TODO: Qt5.15.2升级 检查头文件包含是否需要调整

// Qt应用程序框架头文件
#include <QApplication>
// KD Chart核心图表类
#include <KDChartChart>
// Levey-Jennings图表专用坐标轴
#include <KDChartLeveyJenningsAxis>
// Levey-Jennings图表专用坐标平面
#include <KDChartLeveyJenningsCoordinatePlane>
// Levey-Jennings图表类
#include <KDChartLeveyJenningsDiagram>
// Levey-Jennings图表网格属性
#include <KDChartLeveyJenningsGridAttributes>
// 日期时间处理
#include <QDateTime>
// 分割器布局
#include <QSplitter>
// 标准数据模型
#include <QStandardItemModel>
// 表格视图
#include <QTableView>
// 定时器
#include <QTimer>
// TODO: Qt5.15.2升级 检查KDChart API是否有变更
// TODO: Qt5.15.2升级 验证所有包含的头文件在新版本中的兼容性

/**
 * @brief 选择动画器类
 * @details 继承自QObject，用于在表格视图中自动切换选中项，实现数据高亮动画效果
 *          主要应用于Levey-Jennings图表与表格数据的联动展示，帮助用户直观理解数据对应关系
 */
class SelectionAnimator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param view 表格视图对象指针，动画器将控制该视图的选中项
     * @details 创建选择动画器实例，初始化定时器并设置1秒间隔触发动画
     */
    SelectionAnimator(QAbstractItemView *view)
        : QObject(view) // 调用父类构造函数并设置父对象
        , view(view) // 初始化表格视图指针
    {
        // 创建定时器对象并设置父对象为当前动画器
        auto *const t = new QTimer(this);
        // 连接定时器timeout信号到animate槽函数
        connect(t, &QTimer::timeout, this, &SelectionAnimator::animate);
        t->start(1000); // 设置定时器间隔为1000毫秒（1秒）
    }

protected Q_SLOTS:
    /**
     * @brief 动画槽函数
     * @details 实现表格行的循环选中效果，每次选中下一行数据，到达末尾后回到起始行
     *          通过selectionModel实现表格与图表的数据联动
     */
    void animate()
    {
        // 获取当前选中行索引
        int currentRow = view->selectionModel()->currentIndex().row();
        // 计算下一行索引，使用模运算实现循环
        const int row = (currentRow + 1) % view->model()->rowCount();
        // 获取下一行的模型索引
        QModelIndex newIndex = view->model()->index(row, 0);
        // 设置当前选中项，清除原有选择并选择新行
        view->selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::ClearAndSelect);
    }

private:
    QAbstractItemView *const view; // 关联的表格视图对象指针，用于操作选中状态
}; // TODO: C++17升级 考虑使用final修饰符
// TODO: Qt5.15.2升级 检查QAbstractItemView和QTimer在新版本中的API变化

#include "main.moc"

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 创建Levey-Jennings图表示例，展示实验室质量控制数据的时间序列可视化
 *          Levey-Jennings图表常用于临床实验室，监控检测结果的稳定性和质量控制状态
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    QStandardItemModel model(14, 6); // 创建数据模型，14行6列（14个数据点，6个属性）

    // 设置表头数据（批次、数值、合格状态、日期时间、平均值、标准差）
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
    model.setData(model.index(9, 3), QDateTime::fromString("2007-10T21:00:00", Qt::ISODate));

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

    auto *chart = new KDChart::Chart(); // 创建图表容器对象

    auto *diagram = new KDChart::LeveyJenningsDiagram; // 创建Levey-Jennings图表对象
    diagram->setModel(&model); // 设置数据模型
    diagram->setExpectedMeanValue(200); // 设置预期平均值（质量控制目标值）
    diagram->setExpectedStandardDeviation(20); // 设置预期标准差（质量控制允许波动范围）
    auto *plane = new KDChart::LeveyJenningsCoordinatePlane; // 创建Levey-Jennings专用坐标系
    chart->replaceCoordinatePlane(plane); // 替换图表的默认坐标系
    plane->replaceDiagram(diagram); // 将图表对象关联到坐标系

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

    // 添加左侧坐标轴（显示测量值）
    auto *axis = new KDChart::LeveyJenningsAxis(diagram);
    axis->setPosition(KDChart::CartesianAxis::Left);
    diagram->addAxis(axis);

    // 添加右侧坐标轴（显示计算值）
    auto *axis2 = new KDChart::LeveyJenningsAxis(diagram);
    axis2->setPosition(KDChart::CartesianAxis::Right);
    axis2->setType(KDChart::LeveyJenningsGridAttributes::Calculated);
    diagram->addAxis(axis2);

    // 添加底部坐标轴（显示时间）
    KDChart::CartesianAxis *axis3 = new KDChart::LeveyJenningsAxis(diagram);
    axis3->setPosition(KDChart::CartesianAxis::Bottom);
    diagram->addAxis(axis3);

    auto *tv = new QTableView; // 创建表格视图用于数据展示
    tv->setModel(&model); // 设置数据模型
    tv->setSelectionModel(diagram->selectionModel()); // 共享选择模型，实现图表与表格联动选择

    auto *splitter = new QSplitter; // 创建分割器布局
    splitter->addWidget(tv); // 添加表格视图
    splitter->addWidget(chart); // 添加图表

    splitter->show(); // 显示主窗口

    new SelectionAnimator(tv); // 创建选择动画器实例，实现自动选中效果

    return app.exec(); // 运行应用程序事件循环
} // TODO: Qt5.15.2升级 检查LeveyJenningsDiagram在Qt5.15.2中的API兼容性
// TODO: Qt5.15.2升级 验证QDateTime::fromString()在新版本中的行为变化
// TODO: C++17升级 使用std::make_unique管理动态内存（如chart、diagram等对象）
// TODO: C++17升级 使用结构化绑定优化数据模型设置代码
// TODO: C++17升级 使用std::optional处理缺失的数值（如第3行数据）
