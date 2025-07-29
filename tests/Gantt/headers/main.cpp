/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** 中文注释：该文件演示了如何自定义甘特图的日期时间刻度格式化器和表头视图
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有：2001年 Klarälvdalens Datakonsult AB（KDAB集团公司）<info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可协议：MIT
**
****************************************************************************/

#include <QApplication>
#include <QHeaderView>
#include <QPainter>
#include <QStandardItemModel>
#include <QTreeView>

#include <KDGanttDateTimeGrid>
#include <KDGanttView>

using namespace KDGantt;

/* 自定义格式化器，每10分钟显示一个标签。
 */
// 自定义日期时间刻度格式化器类，用于控制甘特图时间轴的显示格式
// 该类继承自DateTimeScaleFormatter，重写了时间范围计算和文本格式化方法
class MyDateTimeScaleFormatter : public DateTimeScaleFormatter
{
public:
    // 构造函数
    MyDateTimeScaleFormatter();

    /*reimp*/ QDateTime nextRangeBegin(const QDateTime &datetime) const override;
    /*reimp*/ QDateTime currentRangeBegin(const QDateTime &datetime) const override;

    /*reimp*/ QString text(const QDateTime &dt) const override;
};

// 构造函数：初始化父类为小时刻度，格式为"hh"
MyDateTimeScaleFormatter::MyDateTimeScaleFormatter()
    : DateTimeScaleFormatter(Hour, "hh")
{
}

// 获取下一个时间范围的起始时间
// 参数：datetime - 当前时间
// 返回：当前时间范围起始时间加上10分钟
QDateTime MyDateTimeScaleFormatter::nextRangeBegin(const QDateTime &datetime) const
{
    // 计算下一个时间范围的开始，在当前范围基础上增加10分钟
    return currentRangeBegin(datetime).addSecs(60 * 10); // 60秒*10=10分钟
}

// 获取当前时间范围的起始时间
// 参数：datetime - 当前时间
// 返回：调整到最近的10分钟整点的时间
QDateTime MyDateTimeScaleFormatter::currentRangeBegin(const QDateTime &datetime) const
{
    QDateTime dt(datetime);
    // 将分钟数调整为10的倍数，秒和毫秒设为0
    dt.setTime(QTime(dt.time().hour(), (dt.time().minute() / 10) * 10, 0, 0));
    return dt;
}

// 将日期时间格式化为显示文本
// 参数：dt - 要格式化的日期时间
// 返回：格式化后的字符串，格式为":分钟\nXX"
QString MyDateTimeScaleFormatter::text(const QDateTime &dt) const
{
    // 格式化时间为":分钟"格式，并在下方显示"XX"
    return QObject::tr(":%1\nXX").arg(dt.time().toString("mm")); // 使用冒号+分钟格式，并换行显示XX
}

/* 自定义表头视图，比标准视图更高
 * 以便在图形视图一侧容纳更多行
 */
// 自定义表头视图类，用于增加甘特图左侧表头的高度
// 继承自QHeaderView，重写sizeHint方法以提供更大的高度
class MyHeaderView : public QHeaderView
{
public:
    // 构造函数
    // 参数：parent - 父部件指针
    explicit MyHeaderView(QWidget *parent = 0)
        : QHeaderView(Qt::Horizontal, parent) // 初始化水平方向的表头
    {
    }

    /*reimp*/ QSize sizeHint() const override
    {
        QSize s = QHeaderView::sizeHint(); // 获取父类的尺寸提示
        s.rheight() *= 3; // 将高度增加到原来的3倍
        return s; // 返回调整后的尺寸
    }
};

// 主函数，应用程序入口
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象

    QStandardItemModel model(1, 1); // 创建标准项模型，初始1行1列
    model.setHeaderData(0, Qt::Horizontal, QObject::tr("Task")); // 设置水平表头数据为"Task"

    /*** 具有一些替代表头标签的视图 ***/
    View view1; // 创建第一个甘特图视图
    DateTimeGrid grid1; // 创建日期时间网格
    // 设置自定义上部刻度：年刻度，格式"yyyy"，提示文本"In the year %1."，左对齐
    grid1.setUserDefinedUpperScale(new DateTimeScaleFormatter(DateTimeScaleFormatter::Year,
                                                              QString::fromLatin1("yyyy"),
                                                              QString::fromLatin1("In the year %1."),
                                                              Qt::AlignLeft));
    // 设置自定义下部刻度：月刻度，格式"MMMM"，提示文本"In the month %1."，右对齐
    grid1.setUserDefinedLowerScale(new DateTimeScaleFormatter(DateTimeScaleFormatter::Month,
                                                              QString::fromLatin1("MMMM"),
                                                              QString::fromLatin1("In the month %1."),
                                                              Qt::AlignRight));
    grid1.setScale(DateTimeGrid::ScaleUserDefined); // 设置网格刻度为用户自定义模式
    grid1.setDayWidth(6.); // 设置每天宽度为6.0像素
    view1.setGrid(&grid1); // 为视图1设置网格
    view1.setModel(&model); // 为视图1设置数据模型
    view1.show(); // 显示视图1

    /*** 每10分钟带有表头和垂直网格线的视图 */
    View view2; // 创建第二个甘特图视图
    auto *tw = qobject_cast<QTreeView *>(view2.leftView()); // 获取视图2的左侧树视图
    if (tw)
        tw->setHeader(new MyHeaderView); // 如果获取成功，设置自定义表头视图
    DateTimeGrid grid2; // 创建第二个日期时间网格
    grid2.setDayWidth(5000); // 设置每天宽度为5000像素（放大时间刻度）
    // 设置自定义上部刻度：小时刻度，格式"hh"
    grid2.setUserDefinedUpperScale(new DateTimeScaleFormatter(DateTimeScaleFormatter::Hour, QString::fromLatin1("hh")));
    grid2.setUserDefinedLowerScale(new MyDateTimeScaleFormatter); // 设置自定义下部刻度为MyDateTimeScaleFormatter
    grid2.setScale(DateTimeGrid::ScaleUserDefined); // 设置网格刻度为用户自定义模式
    view2.setGrid(&grid2); // 为视图2设置网格
    view2.setModel(&model); // 为视图2设置数据模型
    view2.show(); // 显示视图2

    return app.exec(); // 运行应用程序事件循环
}
