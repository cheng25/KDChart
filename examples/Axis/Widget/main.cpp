/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** This file is part of the KD Chart library.
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** 许可协议: MIT
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartCartesianAxis>    // 包含笛卡尔坐标轴类
#include <KDChartDataValueAttributes> // 包含数据值属性类
#include <KDChartLineDiagram>     // 包含折线图类
#include <KDChartWidget>          // 包含KDChart控件类
#include <QApplication>           // 包含Qt应用程序类

using namespace KDChart;          // 使用KDChart命名空间

/**
 * @brief main 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 返回应用程序执行状态，0表示成功
 * @details 程序入口点，创建应用程序对象和图表窗口实例，配置图表数据和坐标轴，然后启动事件循环
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建Qt应用程序对象

    Widget widget;                 // 创建图表窗口实例
    widget.resize(600, 600);       // 设置窗口大小为600x600像素

    QVector<QPair<qreal, qreal>> vec0;  // 定义坐标对向量(未使用)
    QVector<qreal> vec1, vec2;         // 定义数据向量

    // 为vec1填充数据，形成一个抛物线形状
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;

    // 分配数据集并设置图例项文本
    // assign your datasets
    // while setting the legend
    // items text
    widget.setDataset(0, vec1, "vec1");

    // 添加图例并设置位置在顶部
    widget.addLegend(Position::North);

    // 配置坐标轴
    auto *xAxis = new CartesianAxis(widget.lineDiagram());  // 创建X轴
    auto *yAxis = new CartesianAxis(widget.lineDiagram());  // 创建Y轴
    xAxis->setPosition(CartesianAxis::Bottom);  // 设置X轴位置在底部
    yAxis->setPosition(CartesianAxis::Left);    // 设置Y轴位置在左侧
    xAxis->setTitleText("Abscissa bottom position");  // 设置X轴标题(英文)
    yAxis->setTitleText("Ordinate left position");    // 设置Y轴标题(英文)

    // 配置X轴标签
    // 无需重写标签，它会迭代直到所有标签都被写入
    // configure Xaxis labels
    // no need to re-write labels it iterates until all
    // labels are written
    QStringList daysOfWeek;  // 完整星期名称列表
    daysOfWeek << "Monday"
               << "Tuesday"
               << "Wednesday";
    QStringList shortDays;   // 简短星期名称列表
    shortDays << "Mon"
              << "Tue"
              << "Wed";

    // 设置用户配置的X轴标签
    xAxis->setLabels(daysOfWeek);    // 设置完整标签
    xAxis->setShortLabels(shortDays); // 设置简短标签

    widget.lineDiagram()->addAxis(xAxis);  // 将X轴添加到折线图
    widget.lineDiagram()->addAxis(yAxis);  // 将Y轴添加到折线图

    // 显示数据值
    DataValueAttributes a(widget.lineDiagram()->dataValueAttributes());
    a.setVisible(true);  // 设置数据值可见
    widget.lineDiagram()->setDataValueAttributes(a);

    // 确保数据值显示在边界处
    widget.setGlobalLeading(20, 20, 20, 20);  // 设置全局边距(上,右,下,左)为20像素

    widget.show();  // 显示窗口

    return app.exec();  // 启动应用程序事件循环
}
