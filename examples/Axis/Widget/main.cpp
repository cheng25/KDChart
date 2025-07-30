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
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码，0表示成功
 * @details 创建Qt应用程序，配置图表窗口，设置坐标轴和数据显示
 * 核心功能：演示KDChart控件的基本使用，包括坐标轴配置、数据绑定和图例设置
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 初始化Qt应用程序核心对象

    Widget widget;                 // 创建图表窗口实例
    widget.resize(600, 600);       // 设置窗口大小为600x600像素

    QVector<QPair<qreal, qreal>> vec0;  // 坐标对向量(未使用)
    QVector<qreal> vec1, vec2;         // 数据向量

    // 为vec1填充抛物线形状数据
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;

    // 将数据集绑定到图表，并设置图例名称
    widget.setDataset(0, vec1, "vec1");

    // 添加图例并设置位置在顶部
    widget.addLegend(Position::North);

    // 配置坐标轴
    auto *xAxis = new CartesianAxis(widget.lineDiagram());  // 创建X轴对象
    auto *yAxis = new CartesianAxis(widget.lineDiagram());  // 创建Y轴对象
    xAxis->setPosition(CartesianAxis::Bottom);  // 设置X轴位置在底部
    yAxis->setPosition(CartesianAxis::Left);    // 设置Y轴位置在左侧
    xAxis->setTitleText("横轴(底部位置)");  // 设置X轴标题(中文)
    yAxis->setTitleText("纵轴(左侧位置)");    // 设置Y轴标题(中文)
    // 原英文标题: Abscissa bottom position
    // 原英文标题: Ordinate left position

    // 配置X轴标签
    QStringList daysOfWeek;  // 完整星期名称列表
    daysOfWeek << "Monday" << "Tuesday" << "Wednesday";
    QStringList shortDays;   // 简短星期名称列表
    shortDays << "Mon" << "Tue" << "Wed";

    xAxis->setLabels(daysOfWeek);    // 设置完整标签
    xAxis->setShortLabels(shortDays); // 设置简短标签

    widget.lineDiagram()->addAxis(xAxis);  // 将X轴添加到折线图
    widget.lineDiagram()->addAxis(yAxis);  // 将Y轴添加到折线图

    // 显示数据值
    DataValueAttributes a(widget.lineDiagram()->dataValueAttributes());
    a.setVisible(true);  // 设置数据值可见
    widget.lineDiagram()->setDataValueAttributes(a);

    // 设置全局边距，确保数据值显示在边界处
    widget.setGlobalLeading(20, 20, 20, 20);  // 上,右,下,左各20像素边距

    widget.show();  // 显示窗口

    // TODO: Qt5.15.2升级 检查CartesianAxis::setPosition在Qt5.15.2中的行为变更
    // TODO: C++17升级 使用std::unique_ptr管理xAxis和yAxis动态对象
    return app.exec();  // 启动应用程序事件循环
}
