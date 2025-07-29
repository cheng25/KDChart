/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件演示了如何使用KDChart库创建一个简单的绘图图表应用
// 展示了三个函数的图表：100 * sin(x)、x和x^3
// 可以通过定义PLOTTED_POINTS宏来切换是显示点还是线
// TODO: Qt5.15.2升级 检查KDChart库相关API是否有变更
// TODO: C++17升级 考虑使用std::optional处理可能的初始化错误
// TODO: C++17升级 考虑使用std::cmath替代cmath
// TODO: C++17升级 考虑使用结构化绑定简化数据模型操作

// 笛卡尔坐标轴头文件
// TODO: Qt5.15.2升级 检查KDChartCartesianAxis API是否有变更
#include <KDChartCartesianAxis>
// 笛卡尔坐标平面头文件
// TODO: Qt5.15.2升级 检查KDChartCartesianCoordinatePlane API是否有变更
#include <KDChartCartesianCoordinatePlane>
// 图表组件头文件
// TODO: Qt5.15.2升级 检查KDChartChart API是否有变更
#include <KDChartChart>
// 数据值属性头文件
// TODO: Qt5.15.2升级 检查KDChartDataValueAttributes API是否有变更
#include <KDChartDataValueAttributes>
// 图例头文件
// TODO: Qt5.15.2升级 检查KDChartLegend API是否有变更
#include <KDChartLegend>
// 折线图头文件
// TODO: Qt5.15.2升级 检查KDChartLineDiagram API是否有变更
#include <KDChartLineDiagram>
// 标记属性头文件
// TODO: Qt5.15.2升级 检查KDChartMarkerAttributes API是否有变更
#include <KDChartMarkerAttributes>
// 绘图器图表头文件
// TODO: Qt5.15.2升级 检查KDChartPlotter API是否有变更
#include <KDChartPlotter>
// 文本属性头文件
// TODO: Qt5.15.2升级 检查KDChartTextAttributes API是否有变更
#include <KDChartTextAttributes>
// Qt应用程序类头文件
// TODO: Qt5.15.2升级 检查QApplication API是否有变更
#include <QApplication>
// 标准数据模型头文件
// TODO: Qt5.15.2升级 检查QStandardItemModel API是否有变更
#include <QStandardItemModel>

// 数学函数头文件
#include <cmath>

// 定义π常量
// TODO: C++17升级 使用constexpr优化常量定义
#define PI 3.141592653589793

// 启用下一行以显示点而不是线
// #define PLOTTED_POINTS

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出码
 * @details 初始化Qt应用程序，创建图表，并显示三个函数的图形：100 * sin(x)、x和x^3
 * TODO: C++17升级 考虑使用std::optional处理可能的初始化错误
 * TODO: C++17升级 考虑使用结构化绑定简化变量声明
 */
int main(int argc, char **argv)
{
    // TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
    // 创建Qt应用程序实例
    QApplication app(argc, argv);

#if defined PLOTTED_POINTS
    // 如果显示点，则使用60个数据点
    const int points = 60;
#else
    // 如果显示线，则使用1000个数据点
    const int points = 1000;
#endif
    // x轴最小值
    const qreal xMin = -2 * PI;
    // x轴最大值
    const qreal xMax = 2 * PI;
    // 步长
    const qreal step = (xMax - xMin) / (points - 1);

    // 创建数据模型，存储points行，6列数据
    // TODO: Qt5.15.2升级 检查QStandardItemModel构造函数是否有变更
    QStandardItemModel model(points, 6);

    // 填充数据模型
    double x = xMin;  // 初始x值
    // TODO: C++17升级 考虑使用结构化绑定简化循环
    for (int n = 0; n < points; ++n, x += step) {
        // 第一列：x值（用于100*sin(x)的x坐标）
        QModelIndex index = model.index(n, 0);
        model.setData(index, x);
        // 第二列：100*sin(x)值（y坐标）
        index = model.index(n, 1);
        model.setData(index, sin(x) * 100.0);

        // 第三列：x值（用于x的x坐标）
        index = model.index(n, 2);
        model.setData(index, x);
        // 第四列：x值（用于x的y坐标）
        index = model.index(n, 3);
        model.setData(index, x);

        // 第五列：x值（用于x^3的x坐标）
        index = model.index(n, 4);
        model.setData(index, x);
        // 第六列：x^3值（y坐标）
        index = model.index(n, 5);
        model.setData(index, x * x * x);
    }

    // 设置列标题
    model.setHeaderData(0, Qt::Horizontal, QString::fromLatin1("100 * sin(x)"));
    model.setHeaderData(2, Qt::Horizontal, QString::fromLatin1("x"));
    model.setHeaderData(4, Qt::Horizontal, QString::fromLatin1("x^3"));

    // TODO: Qt5.15.2升级 检查Chart构造函数是否有变更
    // 创建图表对象
    auto *chart = new KDChart::Chart();

    // 创建绘图器图表
    KDChart::AbstractCartesianDiagram *diagram = new KDChart::Plotter;
    diagram->setModel(&model);  // 设置图表数据模型
    chart->coordinatePlane()->replaceDiagram(diagram);  // 将图表添加到坐标平面

#if defined PLOTTED_POINTS
    // 如果显示点，则设置线条为无，只显示标记点
    diagram->setPen(QPen(Qt::NoPen));
    const int colCount = model.columnCount(diagram->rootIndex());
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {
        const QPen markerPen(diagram->brush(iColumn).color());
        KDChart::DataValueAttributes dva(diagram->dataValueAttributes(iColumn));
        KDChart::TextAttributes ta(dva.textAttributes());
        KDChart::MarkerAttributes ma(dva.markerAttributes());
        ma.setPen(markerPen);
        ma.setMarkerStyle(KDChart::MarkerAttributes::MarkerCircle);  // 设置标记样式为圆形
        ma.setMarkerSize(QSize(3, 3));  // 设置标记大小

        dva.setVisible(true);
        ta.setVisible(false);
        ma.setVisible(true);
        dva.setTextAttributes(ta);
        dva.setMarkerAttributes(ma);
        diagram->setDataValueAttributes(iColumn, dva);
    }
#endif

    // 创建X轴和Y轴
    auto *xAxis = new KDChart::CartesianAxis(diagram);
    auto *yAxis = new KDChart::CartesianAxis(diagram);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);  // X轴在底部
    yAxis->setPosition(KDChart::CartesianAxis::Left);    // Y轴在左侧
    diagram->addAxis(xAxis);  // 将X轴添加到图表
    diagram->addAxis(yAxis);  // 将Y轴添加到图表

    // 创建图例
    auto *legend = new KDChart::Legend(diagram, chart);
    legend->setPosition(KDChart::Position::East);  // 图例在右侧
    legend->setAlignment(Qt::AlignCenter);  // 图例居中对齐
    legend->setTitleText("Legend");  // 图例标题
    chart->addLegend(legend);  // 将图例添加到图表

    // 显示图表
    chart->show();

    // 运行应用程序事件循环
    // TODO: Qt5.15.2升级 检查QApplication::exec()返回值处理是否需要变更
    return app.exec();
}
