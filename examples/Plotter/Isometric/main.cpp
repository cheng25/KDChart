/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件是Isometric项目的主文件
// 展示了如何使用KD Chart库创建具有等距缩放功能的图表
// 包含图表初始化、数据模型设置、坐标轴配置和图例定制等功能

#include <QtGui>  // Qt图形界面相关类
// TODO: Qt5.15.2升级 检查QtGui头文件是否需要替换为QtWidgets

#include <KDChartBackgroundAttributes>  // 背景属性类
#include <KDChartChart>                 // 图表主类
#include <KDChartDataValueAttributes>   // 数据值属性类
#include <KDChartFrameAttributes>       // 框架属性类
#include <KDChartGridAttributes>        // 网格属性类
#include <KDChartLegend>                // 图例类
#include <KDChartMarkerAttributes>      // 标记属性类
#include <KDChartPlotter>               // 绘图仪类
#include <KDChartTextAttributes>        // 文本属性类

#include <QApplication>  // Qt应用程序类
#include <QStandardItemModel>  // 标准项数据模型类

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数字符串数组
 * @return 应用程序退出代码
 * @details 创建QApplication实例，初始化图表，设置数据模型
 * 配置图表属性，启用等距缩放，并显示图表
 * TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
 */
int main(int argc, char *argv[])
{
    // TODO: Qt5.15.2升级 检查QApplication初始化是否需要适配新版本
    QApplication a(argc, argv);  // 创建Qt应用程序实例
    KDChart::Chart chart;  // 创建图表对象

    QStandardItemModel model;  // 创建数据模型

    const double offset = 600;  // 数据偏移量
    model.setRowCount(100);  // 设置数据模型行数
    model.setColumnCount(2);  // 设置数据模型列数（1个数据集）
    // TODO: C++17升级 考虑使用结构化绑定简化循环
    // TODO: C++17升级 考虑使用std::generate_n填充数据
    for (int i = 0; i < 100; ++i) {
        double t = i + offset;  // X坐标值
        double v = i + offset;  // Y坐标值
        QModelIndex index = model.index(i, 0);  // 获取X坐标索引
        model.setData(index, t);  // 设置X坐标数据
        index = model.index(i, 1);  // 获取Y坐标索引
        model.setData(index, v);  // 设置Y坐标数据
    }

    model.setHeaderData(0, Qt::Horizontal, "Dataset 1");  // 设置表头数据

    // 图表通用布局设置
    KDChart::FrameAttributes fm = chart.frameAttributes();  // 获取图表框架属性
    fm.setVisible(true);  // 设置框架可见
    fm.setPen(QPen(Qt::black));  // 设置框架边框颜色
    chart.setFrameAttributes(fm);  // 应用框架属性
    chart.setGlobalLeading(10, 0, 10, 10);  // 设置图表全局边距

    KDChart::BackgroundAttributes chart_bg;  // 图表背景属性
    chart_bg.setBrush(Qt::white);  // 设置背景颜色为白色
    chart_bg.setVisible(true);  // 设置背景可见
    chart.setBackgroundAttributes(chart_bg);  // 应用背景属性

    // 坐标平面设置
    KDChart::AbstractCoordinatePlane *plane1 = chart.coordinatePlane();  // 获取坐标平面
    plane1->setRubberBandZoomingEnabled(true);  // 启用橡皮筋缩放功能

    // 创建笛卡尔图表
    auto *plotter = new KDChart::Plotter;  // 创建绘图仪对象
    // TODO: Qt5.15.2升级 检查Plotter构造函数是否有变更
    plotter->setAntiAliasing(false);  // 禁用抗锯齿
    plotter->setModel(&model);  // 设置数据模型
    plane1->replaceDiagram(plotter);  // 替换图表的坐标系为绘图仪

    // 自定义网格
    auto *cp1 = static_cast<KDChart::CartesianCoordinatePlane *>(plane1);  // 转换为笛卡尔坐标平面
    KDChart::GridAttributes gv = cp1->gridAttributes(Qt::Vertical);  // 获取垂直网格属性
    QPen gridPen(QColor(200, 100, 100));  // 创建网格线画笔
    gridPen.setStyle(Qt::DashLine);  // 设置网格线样式为虚线
    gv.setGridPen(gridPen);  // 设置网格线画笔
    gridPen.setStyle(Qt::DotLine);  // 设置子网格线样式为点线
    gridPen.setColor(QColor(255, 155, 155));  // 设置子网格线颜色
    gv.setSubGridPen(gridPen);  // 设置子网格线画笔
    cp1->setGridAttributes(Qt::Vertical, gv);  // 应用垂直网格属性

    // 启用等距缩放
    cp1->setIsometricScaling(true);  // 设置等距缩放
    // TODO: Qt5.15.2升级 检查isometricScaling相关API是否有变更

    // 坐标轴设置
    auto *xAxis = new KDChart::CartesianAxis(plotter);  // 创建X轴
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);  // 设置X轴位置为底部
    xAxis->setTitleText("X-Title");  // 设置X轴标题

    KDChart::TextAttributes att = xAxis->titleTextAttributes();  // 获取X轴标题文本属性
    QFont f = att.font();  // 获取字体
    f.setBold(true);  // 设置粗体
    att.setFont(f);  // 设置字体
    att.setAutoShrink(true);  // 启用自动缩小
    att.setFontSize(KDChart::Measure(16));  // 设置字体大小
    xAxis->setTitleTextAttributes(att);  // 应用标题文本属性

    auto *y1Axis = new KDChart::CartesianAxis(plotter);  // 创建Y轴
    y1Axis->setPosition(KDChart::CartesianAxis::Left);  // 设置Y轴位置为左侧
    y1Axis->setTitleText("Y-Title");  // 设置Y轴标题

    att = y1Axis->titleTextAttributes();  // 获取Y轴标题文本属性
    f = att.font();  // 获取字体
    f.setBold(true);  // 设置粗体
    att.setFont(f);  // 设置字体
    att.setAutoShrink(true);  // 启用自动缩小
    att.setFontSize(KDChart::Measure(16));  // 设置字体大小
    y1Axis->setTitleTextAttributes(att);  // 应用标题文本属性

    // 添加坐标轴到绘图仪
    plotter->addAxis(xAxis);  // 添加X轴
    plotter->addAxis(y1Axis);  // 添加Y轴

    // 创建图例
    auto *legend = new KDChart::Legend(plotter, &chart);  // 创建图例
    chart.addLegend(legend);  // 添加图例到图表
    att = legend->textAttributes();  // 获取图例文本属性
    f = att.font();  // 获取字体
    f.setBold(false);  // 取消粗体
    att.setFont(f);  // 设置字体
    att.setAutoShrink(true);  // 启用自动缩小
    legend->setTextAttributes(att);  // 应用文本属性

    legend->setPosition(KDChart::Position::East);  // 设置图例位置为右侧
    legend->setAlignment(Qt::AlignCenter);  // 设置图例对齐方式为居中
    legend->setTitleText("Curves");  // 设置图例标题
    att = legend->titleTextAttributes();  // 获取图例标题文本属性
    f = att.font();  // 获取字体
    f.setBold(true);  // 设置粗体
    att.setFont(f);  // 设置字体
    att.setAutoShrink(true);  // 启用自动缩小
    att.setFontSize(KDChart::Measure(16));  // 设置字体大小
    legend->setTitleTextAttributes(att);  // 应用标题文本属性

    KDChart::BackgroundAttributes legend_bg;  // 图例背景属性
    legend_bg.setBrush(Qt::white);  // 设置背景颜色为白色
    legend_bg.setVisible(true);  // 设置背景可见
    legend->setBackgroundAttributes(legend_bg);  // 应用背景属性

    KDChart::DataValueAttributes attr = plotter->dataValueAttributes();  // 获取数据值属性
    KDChart::TextAttributes tattr = attr.textAttributes();  // 获取文本属性
    // TODO: Qt5.15.2升级 检查Measure构造函数参数是否有变更
    tattr.setFontSize(KDChart::Measure(12, KDChartEnums::MeasureCalculationModeAbsolute));  // 设置字体大小
    tattr.setRotation(0);  // 设置旋转角度
    attr.setTextAttributes(tattr);  // 设置文本属性
    plotter->setDataValueAttributes(attr);  // 应用数据值属性

    // 自定义标记属性

    // 数据集1：绿色，MarkerRing，无线条
    QColor SERIES_1_OUTLINE = QColor(0, 128, 0);  // 数据集1的轮廓颜色
    attr = plotter->dataValueAttributes(0);  // 获取数据集1的数据值属性
    KDChart::MarkerAttributes mattr = attr.markerAttributes();  // 获取标记属性
    mattr.setMarkerColor(SERIES_1_OUTLINE);  // 设置标记颜色
    //        mattr.setMarkerStyle( KDChart::MarkerAttributes::MarkerRing );  // 设置标记样式为环形
    mattr.setMarkerSize(QSizeF(6.0, 6.0));  // 设置标记大小
    mattr.setVisible(true);  // 设置标记可见
    attr.setMarkerAttributes(mattr);  // 设置标记属性
    attr.setVisible(true);  // 设置数据值可见
    plotter->setDataValueAttributes(0, attr);  // 应用数据值属性
    plotter->setPen(0, Qt::NoPen);  // 设置数据集1的线条为无

    chart.show();  // 显示图表
    return a.exec();  // 运行应用程序事件循环
}
