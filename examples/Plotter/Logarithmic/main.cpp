/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件是Logarithmic项目的主文件
// 展示了如何使用KD Chart库创建具有对数坐标的图表
// 包含三个数据集：100*sin(x)、x和x^3，演示了对数坐标的特性

#include <KDChartChart>                 // 图表主类
#include <KDChartDataValueAttributes>   // 数据值属性类
#include <KDChartFrameAttributes>       // 框架属性类
#include <KDChartLineDiagram>           // 线图类
#include <QtGui>                        // Qt图形界面相关类
// TODO: Qt5.15.2升级 检查QtGui头文件是否需要替换为QtWidgets

#include <KDChartCartesianAxis>         // 笛卡尔坐标轴类
#include <KDChartCartesianCoordinatePlane>  // 笛卡尔坐标平面类
#include <KDChartLegend>                // 图例类
#include <KDChartPlotter>               // 绘图仪类
#include <QStandardItemModel>           // 标准项数据模型类

#include <QApplication>                 // Qt应用程序类

using namespace KDChart;  // 使用KDChart命名空间

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数字符串数组
 * @return 应用程序退出代码
 * @details 创建QApplication实例，初始化图表和数据模型
 * 配置对数坐标，添加数据集，并显示图表
 * TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
 */
int main(int argc, char **argv)
{
    // TODO: Qt5.15.2升级 检查QApplication初始化是否需要适配新版本
    QApplication app(argc, argv);  // 创建Qt应用程序实例

    const int points = 420;  // 数据点数量
    const double xMin = 0;   // X轴最小值
    const double xMax = 20;  // X轴最大值
    const double step = (xMax - xMin) / (points - 1);  // 步长

    QStandardItemModel model(points, 6);  // 创建数据模型，420行6列

    double x = xMin;  // 初始X值
    // TODO: C++17升级 考虑使用结构化绑定简化循环
    // TODO: C++17升级 考虑使用std::generate_n填充数据
    for (int n = 0; n < points; ++n, x += step) {
        QModelIndex index = model.index(n, 0);  // 获取第一个X坐标索引
        model.setData(index, x);  // 设置第一个数据集的X坐标
        index = model.index(n, 1);  // 获取第一个Y坐标索引
        model.setData(index, sin(x) * 100.0);  // 设置第一个数据集的Y坐标 (100*sin(x))

        index = model.index(n, 2);  // 获取第二个X坐标索引
        model.setData(index, x);  // 设置第二个数据集的X坐标
        index = model.index(n, 3);  // 获取第二个Y坐标索引
        model.setData(index, x);  // 设置第二个数据集的Y坐标 (x)

        index = model.index(n, 4);  // 获取第三个X坐标索引
        model.setData(index, x);  // 设置第三个数据集的X坐标
        index = model.index(n, 5);  // 获取第三个Y坐标索引
        model.setData(index, x * x * x);  // 设置第三个数据集的Y坐标 (x^3)
    }

    model.setHeaderData(0, Qt::Horizontal, QString::fromLatin1("100 * sin(x)"));  // 设置第一个数据集标题
    model.setHeaderData(2, Qt::Horizontal, QString::fromLatin1("x"));  // 设置第二个数据集标题
    model.setHeaderData(4, Qt::Horizontal, QString::fromLatin1("x^3"));  // 设置第三个数据集标题

    auto *chart = new KDChart::Chart();  // 创建图表对象
    // TODO: Qt5.15.2升级 检查Chart构造函数是否有变更

    KDChart::AbstractCartesianDiagram *diagram = new KDChart::Plotter;  // 创建绘图仪
    diagram->setModel(&model);  // 设置数据模型
    chart->coordinatePlane()->replaceDiagram(diagram);  // 替换图表的坐标系为绘图仪

    auto *xAxis = new KDChart::CartesianAxis(diagram);  // 创建X轴
    auto *yAxis = new KDChart::CartesianAxis(diagram);  // 创建Y轴
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);  // 设置X轴位置为底部
    yAxis->setPosition(KDChart::CartesianAxis::Left);  // 设置Y轴位置为左侧
    diagram->addAxis(xAxis);  // 添加X轴到图表
    diagram->addAxis(yAxis);  // 添加Y轴到图表

    auto *legend = new KDChart::Legend(diagram, chart);  // 创建图例
    KDChart::FrameAttributes legendAtt = legend->frameAttributes();  // 获取图例框架属性
    legendAtt.setCornerRadius(9);  // 设置边框圆角半径
    legend->setFrameAttributes(legendAtt);  // 应用框架属性
    legend->setPosition(KDChart::Position::East);  // 设置图例位置为右侧
    legend->setAlignment(Qt::AlignCenter);  // 设置图例对齐方式为居中
    legend->setTitleText("Legend");  // 设置图例标题
    chart->addLegend(legend);  // 添加图例到图表

    auto *cart_plane = dynamic_cast<KDChart::CartesianCoordinatePlane *>(chart->coordinatePlane());  // 获取笛卡尔坐标平面
    Q_ASSERT(cart_plane);  // 断言检查，确保cart_plane不为空

    // 设置X轴和Y轴为对数坐标模式
    cart_plane->setAxesCalcModeX(KDChart::AbstractCoordinatePlane::Logarithmic);  // X轴对数模式
    // TODO: Qt5.15.2升级 检查setAxesCalcModeX方法是否有变更
    cart_plane->setAxesCalcModeY(KDChart::AbstractCoordinatePlane::Logarithmic);  // Y轴对数模式
    // TODO: Qt5.15.2升级 检查setAxesCalcModeY方法是否有变更

    // 设置垂直范围从0.005到1000 - 使用对数轴实际会得到1到100
    // cart_plane->setVerticalRange(QPair<qreal,qreal>( 0.005, 1000 ) );

    // 设置水平范围从0.001到100
    cart_plane->setHorizontalRange(QPair<qreal, qreal>(0.001, 100));  // 设置X轴范围
    // TODO: Qt5.15.2升级 检查setHorizontalRange方法参数是否有变更

    chart->show();  // 显示图表

    int ret = app.exec();  // 运行应用程序事件循环

    delete chart;  // 释放图表对象内存
    // TODO: C++17升级 考虑使用智能指针管理内存

    return ret;  // 返回退出代码
}
