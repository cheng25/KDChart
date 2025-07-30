/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

/**
 * @brief 主窗口类实现文件
 * 本文件包含MainWindow类的实现，展示了如何在KD Chart库中创建叠加的图表平面
 * 核心功能是演示如何在同一个图表中添加多个坐标平面并在其上绘制不同的数据
 */
#include "mainwindow.h"

#include <KDChartAbstractCoordinatePlane>
#include <KDChartChart>
#include <KDChartGridAttributes>
#include <KDChartLineDiagram>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

/**
 * @brief 构造函数实现
 * @param parent 父窗口部件，默认为nullptr
 * @return 无返回值
 * @details 初始化主窗口UI，创建图表对象和数据模型，配置图表属性和数据
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 设置UI组件

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart(); // 创建图表对象
    chartLayout->addWidget(m_chart);

    // 从CSV文件加载数据
    m_model.loadFromCSV(":/data");  // 加载第一个数据集
    m_model2.loadFromCSV(":/data2"); // 加载第二个数据集

    // 设置第一个折线图
    m_lines = new LineDiagram();
    m_lines->setModel(&m_model); // 设置数据模型

    // 设置第二个折线图
    m_lines2 = new LineDiagram();
    m_lines2->setModel(&m_model2); // 设置数据模型

    // 创建第二个坐标平面
    // 我们称之为"plane2"是为了记住它是除了默认平面之外额外添加的
    // TODO: Qt5.15.2升级 检查KDChart::CartesianCoordinatePlane在Qt5.15.2中的兼容性
    plane2 = new CartesianCoordinatePlane(m_chart);

    // 设置参考坐标平面
    plane2->setReferenceCoordinatePlane(m_chart->coordinatePlane());

    // 创建坐标轴
    auto *xAxis = new CartesianAxis(m_lines2);   // 第二个图表的X轴
    auto *yAxis = new CartesianAxis(m_lines);    // 第一个图表的Y轴
    auto *yAxis2 = new CartesianAxis(m_lines2);  // 第二个图表的Y轴

    // 设置坐标轴位置
    xAxis->setPosition(KDChart::CartesianAxis::Top);    // X轴在顶部
    yAxis->setPosition(KDChart::CartesianAxis::Left);    // Y轴在左侧
    yAxis2->setPosition(KDChart::CartesianAxis::Left);   // 第二个Y轴在左侧

    // 添加坐标轴到图表
    m_lines->addAxis(yAxis);

    m_lines2->addAxis(xAxis);
    m_lines2->addAxis(yAxis2);

    // 将图表添加到坐标平面
    m_chart->coordinatePlane()->replaceDiagram(m_lines);
    plane2->replaceDiagram(m_lines2);
    m_chart->addCoordinatePlane(plane2 /*, 1*/);
    m_chart->setGlobalLeading(20, 20, 20, 20); // 设置全局边距

    init(); // 调用初始化方法
}

/**
 * @brief 初始化方法实现
 * @return 无返回值
 * @details 设置初始网格状态，默认为不显示第二个平面的网格
 */
void MainWindow::init()
{
    on_displayGrid2CB_toggled(false); // 初始时不显示第二个平面的网格
}

/**
 * @brief 切换第一个网格显示状态的槽函数实现
 * @param checked 是否显示网格
 * @return 无返回值
 * @details 响应UI中displayGrid1CB复选框的状态变化，控制第一个平面的网格显示
 */
void MainWindow::on_displayGrid1CB_toggled(bool checked)
{
    // 获取第一个坐标平面
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane());

    // 禁用或启用平面的网格
    GridAttributes gv(plane->gridAttributes(Qt::Vertical));
    gv.setGridVisible(checked);
    plane->setGridAttributes(Qt::Vertical, gv);
    plane->setGridAttributes(Qt::Horizontal, gv);
}

/**
 * @brief 切换第二个网格显示状态的槽函数实现
 * @param checked 是否显示网格
 * @return 无返回值
 * @details 响应UI中displayGrid2CB复选框的状态变化，控制第二个平面的网格显示
 */
void MainWindow::on_displayGrid2CB_toggled(bool checked)
{
    // 禁用或启用plane2的网格
    GridAttributes gv(plane2->gridAttributes(Qt::Vertical));
    gv.setGridVisible(checked);
    plane2->setGridAttributes(Qt::Vertical, gv);
    plane2->setGridAttributes(Qt::Horizontal, gv);
}
