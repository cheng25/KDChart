/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴标签配置示例的主窗口实现文件，包含MainWindow类的具体实现
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "mainwindow.h"
#include "AdjustedCartesianAxis.h"

#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartGridAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>

#include <QDebug>
#include <QLinearGradient>
#include <QTextDocument>

/**
 * @brief MainWindow类的构造函数实现
 * @param parent 父窗口指针，默认为nullptr
 * @return 无返回值
 * @details 初始化UI组件，创建图表布局，配置坐标轴属性，加载数据模型并设置信号槽连接
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件，根据.ui文件创建界面元素

    // 创建图表布局并添加图表部件
    auto *chartLayout = new QHBoxLayout(m_chartFrame); // 创建水平布局管理器
    m_chart = new KDChart::Chart; // 创建图表对象
    chartLayout->addWidget(m_chart); // 将图表添加到布局中

    m_model.loadFromCSV(":/data"); // 从CSV资源文件加载数据

    // 设置图表数据和坐标轴
    m_lines = new KDChart::LineDiagram(); // 创建折线图对象
    m_lines->setModel(&m_model); // 将数据模型绑定到折线图

    m_xAxis = new KDChart::CartesianAxis(m_lines); // 创建X坐标轴对象
    KDChart::TextAttributes ta(m_xAxis->textAttributes()); // 获取X轴文本属性

    auto *yAxis = new AdjustedCartesianAxis(m_lines); // 创建自定义Y坐标轴对象
    yAxis->setBounds(3, 6); // 设置Y轴显示范围为3到6
    m_xAxis->setPosition(KDChart::CartesianAxis::Bottom); // 设置X轴位置为底部
    yAxis->setPosition(KDChart::CartesianAxis::Left); // 设置Y轴位置为左侧

    // 设置X轴标签为星期几
    QStringList daysOfWeek;
    daysOfWeek << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
    m_xAxis->setLabels(daysOfWeek); // 应用X轴标签文本

    // 设置X轴文本旋转-60度避免重叠
    ta.setRotation(-60);
    m_xAxis->setTextAttributes(ta);

    m_lines->addAxis(m_xAxis); // 将X轴添加到图表
    m_lines->addAxis(yAxis); // 将Y轴添加到图表
    m_chart->coordinatePlane()->replaceDiagram(m_lines);

    // 配置自定义刻度样式
    m_xAxis->setCustomTickLength(11); // 设置自定义刻度长度为11像素
    yAxis->setCustomTickLength(11);

    // 设置图例
    m_legend = new KDChart::Legend(m_lines, m_chart); // 创建图例对象
    m_legend->setPosition(KDChart::Position::East); // 设置图例位置为东侧
    m_legend->setAlignment(Qt::AlignTop); // 设置图例顶部对齐
    m_chart->addLegend(m_legend); // 将图例添加到图表

    // 连接信号和槽
    connect(m_annotations, &QCheckBox::toggled, this, &MainWindow::annotationsToggled);
    connect(m_linesOnAnnotations, &QCheckBox::toggled, this, &MainWindow::gridLinesOnAnnotationsToggled);
}

/**
 * @brief 切换注释显示状态的槽函数实现
 * @param showAnnotations 是否显示注释
 * @return 无返回值
 * @details 根据复选框状态在指定位置显示或隐藏坐标轴注释文本
 */
void MainWindow::annotationsToggled(bool showAnnotations)
{
    QMultiMap<qreal, QString> annotations; // 存储注释位置和文本的映射
    if (showAnnotations) {
        // 在自定义刻度位置添加注释文本
        annotations.insert(0.5, "左侧注释");
        annotations.insert(3.5, "中心注释");
        annotations.insert(4.2, "偏心注释");
        annotations.insert(6.5, "右侧注释");
    }
    m_xAxis->setAnnotations(annotations); // 应用注释配置
    m_chart->update(); // 更新图表显示
}

/**
 * @brief 切换注释上网格线显示状态的槽函数实现
 * @param onAnnotations 是否在注释位置显示网格线
 * @return 无返回值
 * @details 根据复选框状态控制注释位置的网格线显示
 */
void MainWindow::gridLinesOnAnnotationsToggled(bool onAnnotations)
{
    // 获取坐标平面并配置网格属性
    auto *plane = static_cast<KDChart::CartesianCoordinatePlane*>(m_chart->coordinatePlane());
    KDChart::GridAttributes ga = plane->gridAttributes(Qt::Horizontal); // 获取水平网格属性
    ga.setLinesOnAnnotations(onAnnotations); // 设置是否在注释位置显示网格线
    plane->setGridAttributes(Qt::Horizontal, ga); // 应用网格属性配置
    m_chart->update(); // 更新图表显示
}
