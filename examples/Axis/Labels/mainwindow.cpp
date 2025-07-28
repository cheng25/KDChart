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
 * @brief MainWindow类的构造函数
 * @param parent 父窗口部件，默认为nullptr
 * 初始化UI组件，创建图表和数据模型，配置坐标轴标签、注释和自定义刻度等属性
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件

    // 创建图表布局并添加图表部件
    auto *chartLayout = new QHBoxLayout(m_chartFrame);
    m_chart = new KDChart::Chart;
    chartLayout->addWidget(m_chart);

    m_model.loadFromCSV(":/data"); // 从CSV文件加载数据

    // 设置图表
    m_lines = new KDChart::LineDiagram();
    m_lines->setModel(&m_model); // 设置数据模型

    m_xAxis = new KDChart::CartesianAxis(m_lines);
    KDChart::TextAttributes ta(m_xAxis->textAttributes());

    auto *yAxis = new AdjustedCartesianAxis(m_lines);
    yAxis->setBounds(3, 6); // 设置Y轴范围为3到6
    m_xAxis->setPosition(KDChart::CartesianAxis::Bottom); // 设置X轴位置为底部
    yAxis->setPosition(KDChart::CartesianAxis::Left); // 设置Y轴位置为左侧

// 设置以下为0，查看默认的横坐标标签（== X标题，从数据文件读取）
#if 1
    QStringList daysOfWeek;
    daysOfWeek << "Monday"
               << "Tuesday"
               << "Wednesday"
               << "Thursday"
               << "Friday"
               << "Saturday"
               << "Sunday";
    m_xAxis->setLabels(daysOfWeek); // 设置X轴标签为星期几

    // QStringList shortDays;
    // shortDays << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    // m_xAxis->setShortLabels( shortDays );
#endif

// 使用HTML绘制坐标轴标签中的文本
#if 0
    QStringList htmlStyles;
    htmlStyles << "<b>Bold</b>" << "<i>Italic</i>" << "<u>Underline</u>" << "<font color='red'>Red</font>";
    m_xAxis->setLabels( htmlStyles );
    ta.setTextDocument(new QTextDocument);
#endif

    // 自定义刻度示例
    QList<qreal> ticks;
    ticks.append(0.5);
    ticks.append(3.5);
    ticks.append(4.2);
    ticks.append(6.5);
    m_xAxis->setCustomTicks(ticks); // 设置X轴自定义刻度
    yAxis->setCustomTicks(ticks); // 设置Y轴自定义刻度

    // 将横坐标标签旋转-60度，避免标签重叠
    ta.setRotation(-60);

    m_xAxis->setTextAttributes(ta);
    m_lines->addAxis(m_xAxis); // 将X轴添加到图表
    m_lines->addAxis(yAxis); // 将Y轴添加到图表
    m_chart->coordinatePlane()->replaceDiagram(m_lines);
    // 设置图例
    m_xAxis->setCustomTickLength(11); // 设置自定义刻度长度为11像素
    yAxis->setCustomTickLength(11);
    m_legend = new KDChart::Legend(m_lines, m_chart);
    m_legend->setPosition(KDChart::Position::East); // 设置图例位置为东侧
    m_legend->setAlignment(Qt::AlignTop); // 设置图例对齐方式为顶部对齐
    m_chart->addLegend(m_legend);

    // 连接信号和槽
    connect(m_annotations, &QCheckBox::toggled, this, &MainWindow::annotationsToggled);
    connect(m_linesOnAnnotations, &QCheckBox::toggled, this, &MainWindow::gridLinesOnAnnotationsToggled);
}

/**
 * @brief 切换注释显示状态的槽函数
 * @param showAnnotations 是否显示注释
 * 当复选框状态改变时，更新X轴注释的显示
 */
void MainWindow::annotationsToggled(bool showAnnotations)
{
    QMultiMap<qreal, QString> annotations;
    if (showAnnotations) {
        // 在自定义位置设置自定义坐标轴注释
        annotations.insert(0.5, "Left");
        annotations.insert(3.5, "Center");
        annotations.insert(4.2, "Off Center");
        annotations.insert(6.5, "Right");
    }
    m_xAxis->setAnnotations(annotations); // 设置X轴注释
    m_chart->update(); // 更新图表
}

/**
 * @brief 切换注释上网格线显示状态的槽函数
 * @param onAnnotations 是否在注释上显示网格线
 * 当复选框状态改变时，更新注释上网格线的显示
 */
void MainWindow::gridLinesOnAnnotationsToggled(bool onAnnotations)
{
    // 在注释所在位置绘制网格线
    auto *plane = static_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane());
    KDChart::GridAttributes ga = plane->gridAttributes(Qt::Horizontal);
    ga.setLinesOnAnnotations(onAnnotations); // 设置是否在注释上显示网格线
    plane->setGridAttributes(Qt::Horizontal, ga);
    m_chart->update(); // 更新图表
}
