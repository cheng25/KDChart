/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件演示了如何使用KDChart库创建一个高级绘图图表应用
// 展示了三个不同数据集的图形，并自定义了它们的样式、标记和线条属性
// 还演示了如何配置图表的框架、背景、网格、坐标轴和图例等高级特性
// 数据集1: y = i^2 (绿色环形标记，无线条)
// 数据集2: y = i^2 - i (红色菱形标记，无线条)
// 数据集3: y = i^2 + i (黄色圆形标记，蓝色线条)

// Qt图形界面类头文件
// TODO: Qt5.15.2升级 检查是否需要替换为QtWidgets头文件
#include <QtGui>

// 背景属性头文件
// TODO: Qt5.15.2升级 检查BackgroundAttributes API是否有变更
#include <KDChartBackgroundAttributes>
// 图表组件头文件
// TODO: Qt5.15.2升级 检查Chart API是否有变更
#include <KDChartChart>
// 数据值属性头文件
// TODO: Qt5.15.2升级 检查DataValueAttributes API是否有变更
#include <KDChartDataValueAttributes>
// 框架属性头文件
// TODO: Qt5.15.2升级 检查FrameAttributes API是否有变更
#include <KDChartFrameAttributes>
// 网格属性头文件
// TODO: Qt5.15.2升级 检查GridAttributes API是否有变更
#include <KDChartGridAttributes>
// 图例头文件
// TODO: Qt5.15.2升级 检查Legend API是否有变更
#include <KDChartLegend>
// 标记属性头文件
// TODO: Qt5.15.2升级 检查MarkerAttributes API是否有变更
#include <KDChartMarkerAttributes>
// 绘图器图表头文件
// TODO: Qt5.15.2升级 检查Plotter API是否有变更
#include <KDChartPlotter>
// 文本属性头文件
// TODO: Qt5.15.2升级 检查TextAttributes API是否有变更
#include <KDChartTextAttributes>

// Qt应用程序类头文件
// TODO: Qt5.15.2升级 检查QApplication API是否有变更
#include <QApplication>
// 标准数据模型头文件
// TODO: Qt5.15.2升级 检查QStandardItemModel API是否有变更
#include <QStandardItemModel>

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出码
 * @details 初始化Qt应用程序，创建高级绘图图表，展示三个数据集的图形，并自定义图表的各种属性
 *          包括框架、背景、网格、坐标轴和图例等高级特性
 * @todo C++17升级 考虑使用结构化绑定和范围for循环简化代码
 */
int main(int argc, char *argv[])
{
    // TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
    // 创建Qt应用程序实例
    QApplication a(argc, argv);
    // 创建图表对象
    KDChart::Chart chart;

    // 创建数据模型
    QStandardItemModel model;

    // 设置数据模型的行列数
    model.setRowCount(100);      // 100行数据
    model.setColumnCount(6);     // 6列数据（3个数据集，每个数据集2列：X值和Y值）

    // 填充数据模型
    // TODO: C++17升级 考虑使用std::generate_n填充数据
    // TODO: C++17升级 考虑使用结构化绑定简化循环
    for (int i = 0; i < 100; ++i) {
        // 数据集1: y = i^2
        qreal t = i;                    // x值
        qreal v = qreal(i) * i;         // y值 = i²
        QModelIndex index = model.index(i, 0);
        model.setData(index, QVariant(t)); // 设置X值
        index = model.index(i, 1);
        model.setData(index, QVariant(v)); // 设置Y值

        // 数据集2: y = i^2 - i
        t = i * 2;                      // x值 = 2i
        v = qreal(i) * i - i;           // y值 = i² - i
        index = model.index(i, 2);
        model.setData(index, QVariant(t)); // 设置X值
        index = model.index(i, 3);
        model.setData(index, QVariant(v)); // 设置Y值

        // 数据集3: y = i^2 + i
        t = qreal(i) / 2;               // x值 = i/2
        v = qreal(i) * i + i;           // y值 = i² + i
        index = model.index(i, 4);
        model.setData(index, QVariant(t)); // 设置X值
        index = model.index(i, 5);
        model.setData(index, QVariant(v)); // 设置Y值
    }

    // 设置列标题
    model.setHeaderData(0, Qt::Horizontal, "Dataset 1");
    model.setHeaderData(2, Qt::Horizontal, "Dataset 2");
    model.setHeaderData(4, Qt::Horizontal, "Dataset 3");

    // 配置图表框架
    KDChart::FrameAttributes fm = chart.frameAttributes();
    fm.setVisible(true);               // 显示框架
    fm.setPen(QPen(Qt::black));        // 框架颜色为黑色
    chart.setFrameAttributes(fm);
    chart.setGlobalLeading(10, 0, 10, 10);  // 设置图表边距（上，右，下，左）

    // 配置图表背景
    KDChart::BackgroundAttributes chart_bg;
    chart_bg.setBrush(Qt::white);      // 背景颜色为白色
    chart_bg.setVisible(true);         // 显示背景
    chart.setBackgroundAttributes(chart_bg);

    // 配置坐标平面
    KDChart::AbstractCoordinatePlane *plane1 = chart.coordinatePlane();
    plane1->setRubberBandZoomingEnabled(true);  // 启用橡皮筋缩放功能

    // TODO: Qt5.15.2升级 检查Chart和Plotter相关API是否有变更
    // 创建绘图器图表
    auto *plotter = new KDChart::Plotter;
    plotter->setHidden(0, true);       // 隐藏第一个数据集的线条
    plotter->setAntiAliasing(false);   // 禁用抗锯齿
    plotter->setModel(&model);         // 设置数据模型
    plane1->replaceDiagram(plotter);   // 将图表添加到坐标平面

    // 自定义网格
    auto *cp1 = static_cast<KDChart::CartesianCoordinatePlane *>(plane1);
    KDChart::GridAttributes gv = cp1->gridAttributes(Qt::Vertical);
    QPen gridPen(QColor(200, 100, 100));
    gridPen.setStyle(Qt::DashLine);    // 主网格线样式为虚线
    gv.setGridPen(gridPen);
    gridPen.setStyle(Qt::DotLine);     // 子网格线样式为点线
    gridPen.setColor(QColor(255, 155, 155));
    gv.setSubGridPen(gridPen);
    cp1->setGridAttributes(Qt::Vertical, gv);

    // 配置X轴
    auto *xAxis = new KDChart::CartesianAxis(plotter);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);  // X轴在底部
    xAxis->setTitleText("X-Title");    // X轴标题

    // 配置X轴标题文本属性
    KDChart::TextAttributes att = xAxis->titleTextAttributes();
    QFont f = att.font();
    f.setBold(true);                   // 粗体
    att.setFont(f);
    att.setAutoShrink(true);           // 自动缩小以适应
    att.setFontSize(KDChart::Measure(16));  // 字体大小16
    xAxis->setTitleTextAttributes(att);

    // 配置Y轴
    auto *y1Axis = new KDChart::CartesianAxis(plotter);
    y1Axis->setPosition(KDChart::CartesianAxis::Left);    // Y轴在左侧
    y1Axis->setTitleText("Y-Title");    // Y轴标题

    // 配置Y轴标题文本属性
    att = y1Axis->titleTextAttributes();
    f = att.font();
    f.setBold(true);                   // 粗体
    att.setFont(f);
    att.setAutoShrink(true);           // 自动缩小以适应
    att.setFontSize(KDChart::Measure(16));  // 字体大小16
    y1Axis->setTitleTextAttributes(att);

    // 将坐标轴添加到绘图器
    plotter->addAxis(xAxis);
    plotter->addAxis(y1Axis);

    // 创建图例
    auto *legend = new KDChart::Legend(plotter, &chart);
    chart.addLegend(legend);

    // 配置图例文本属性
    att = legend->textAttributes();
    f = att.font();
    f.setBold(false);                  // 非粗体
    att.setFont(f);
    att.setAutoShrink(true);           // 自动缩小以适应
    att.setFontSize(KDChart::Measure(16));  // 字体大小16
    legend->setTextAttributes(att);

    // 配置图例位置和标题
    legend->setPosition(KDChart::Position::East);  // 图例在右侧
    legend->setAlignment(Qt::AlignCenter);        // 图例居中对齐
    legend->setTitleText("Curves");               // 图例标题

    // 配置图例标题文本属性
    att = legend->titleTextAttributes();
    f = att.font();
    f.setBold(true);                   // 粗体
    att.setFont(f);
    att.setAutoShrink(true);           // 自动缩小以适应
    att.setFontSize(KDChart::Measure(16));  // 字体大小16
    legend->setTitleTextAttributes(att);

    // 配置图例背景
    KDChart::BackgroundAttributes legend_bg;
    legend_bg.setBrush(Qt::white);      // 背景颜色为白色
    legend_bg.setVisible(true);         // 显示背景
    legend->setBackgroundAttributes(legend_bg);

    // 配置数据值属性
    KDChart::DataValueAttributes attr = plotter->dataValueAttributes();
    KDChart::TextAttributes tattr = attr.textAttributes();
    tattr.setRotation(0);              // 文本旋转角度为0
    attr.setTextAttributes(tattr);
    plotter->setDataValueAttributes(attr);

    // 自定义标记属性

    // 数据集1: 绿色，环形标记，无线条
    QColor SERIES_1_OUTLINE = QColor(0, 128, 0);  // 绿色
    attr = plotter->dataValueAttributes(0);
    KDChart::MarkerAttributes mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_1_OUTLINE);       // 标记颜色
    mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerRing);  // 环形标记
    mattr.setMarkerSize(QSizeF(6.0, 6.0));                       // 标记大小
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(0, attr);
    plotter->setPen(0, Qt::NoPen);  // 无线条

    // 数据集2: 菱形标记，(黑色轮廓，红色内部)，无线条
    QColor SERIES_2_INSIDE = QColor(255, 100, 100);  // 红色
    attr = plotter->dataValueAttributes(1);
    mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_2_INSIDE);          // 标记颜色
    mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerDiamond);  // 菱形标记
    mattr.setMarkerSize(QSizeF(8.0, 8.0));                          // 标记大小
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(1, attr);
    plotter->setPen(1, Qt::NoPen);  // 无线条

    // 数据集3: 圆形标记，(绿色轮廓，黄色内部)，蓝色线条
    QColor SERIES_3_INSIDE = QColor("yellow");    // 黄色
    QColor SERIES_3_LINE = QColor("navy");       // 深蓝色
    attr = plotter->dataValueAttributes(2);
    mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_3_INSIDE);         // 标记颜色
    mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerCircle);  // 圆形标记
    mattr.setMarkerSize(QSizeF(8.0, 8.0));                          // 标记大小
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(2, attr);
    plotter->setPen(2, QPen(SERIES_3_LINE));  // 蓝色线条

    // 显示图表
    chart.show();

    // 运行应用程序事件循环
    // TODO: Qt5.15.2升级 检查QApplication::exec()返回值处理是否需要变更
    return a.exec();
}