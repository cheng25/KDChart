/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴配置示例的主窗口实现文件，包含MainWindow类的具体实现
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartRulerAttributes>
#include <KDChartTextAttributes>

using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口部件，默认为nullptr
 * 初始化UI组件，创建图表和数据模型，配置坐标轴的位置、标题、标签和刻度线等属性
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件

    // 创建图表布局并添加图表部件
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart();
    m_chart->setGlobalLeading(10, 10, 10, 10); // 设置图表全局边距（上、右、下、左）
    chartLayout->addWidget(m_chart);
    hSBar->setVisible(false); // 隐藏水平滚动条
    vSBar->setVisible(false); // 隐藏垂直滚动条

    m_model.loadFromCSV(":/data"); // 从CSV文件加载数据

    // 设置图表
    m_lines = new BarDiagram();
    m_lines->setModel(&m_model); // 设置数据模型

    // 创建并定位坐标轴
    auto *topAxis = new CartesianAxis(m_lines);
    auto *leftAxis = new CartesianAxis(m_lines);
    RulerAttributes rulerAttr = topAxis->rulerAttributes();
    rulerAttr.setTickMarkPen(0.9999999, QPen(Qt::red)); // 设置主刻度线颜色为红色
    rulerAttr.setTickMarkPen(2.0, QPen(Qt::green)); // 设置次刻度线颜色为绿色
    rulerAttr.setTickMarkPen(3.0, QPen(Qt::blue)); // 设置第三刻度线颜色为蓝色
    rulerAttr.setShowMinorTickMarks(true); // 显示次刻度线
    // rulerAttr.setShowMajorTickMarks(false);
    topAxis->setRulerAttributes(rulerAttr);
    auto *rightAxis = new CartesianAxis(m_lines);
    auto *bottomAxis = new CartesianAxis(m_lines);
    topAxis->setPosition(CartesianAxis::Top); // 设置坐标轴位置为顶部
    leftAxis->setPosition(CartesianAxis::Left); // 设置坐标轴位置为左侧
    rightAxis->setPosition(CartesianAxis::Right); // 设置坐标轴位置为右侧
    bottomAxis->setPosition(CartesianAxis::Bottom); // 设置坐标轴位置为底部

// 设置标签和刻度之间的边距为零
#if 0
    RulerAttributes ra = bottomAxis->rulerAttributes();
    ra.setLabelMargin( 0 );
    bottomAxis->setRulerAttributes( ra );
#endif

// 为底部坐标轴显示红色边框
#if 0
    FrameAttributes fa( bottomAxis->frameAttributes() );
    fa.setPen( QPen( QBrush( QColor( "#ff0000" ) ), 1.0 ) );
    fa.setVisible( true );
    bottomAxis->setFrameAttributes( fa );
#endif

    // 设置坐标轴标题
    topAxis->setTitleText("Abscissa Top configured size and color");
    leftAxis->setTitleText("left Ordinate: fonts configured");
    rightAxis->setTitleText("right Ordinate: default settings");
    bottomAxis->setTitleText("Abscissa Bottom");

    // 配置标题文本属性
    TextAttributes taTop(topAxis->titleTextAttributes());
    taTop.setPen(QPen(Qt::red)); // 设置顶部坐标轴标题颜色为红色
    topAxis->setTitleTextAttributes(taTop);

    TextAttributes taLeft(leftAxis->titleTextAttributes());
    taLeft.setRotation(180); // 设置左侧坐标轴标题旋转180度
    Measure me(taLeft.fontSize());
    me.setValue(me.value() * 0.8); // 设置字体大小为原来的80%
    taLeft.setFontSize(me);

// 设置以下为1，隐藏左侧坐标轴标题
//  - 无论是否设置了标题文本
#if 0
    taLeft.setVisible( false );
#endif
    leftAxis->setTitleTextAttributes(taLeft);

    TextAttributes taBottom(bottomAxis->titleTextAttributes());
    taBottom.setPen(QPen(Qt::blue)); // 设置底部坐标轴标题颜色为蓝色
    bottomAxis->setTitleTextAttributes(taBottom);

    // 配置标签文本属性
    TextAttributes taLabels(topAxis->textAttributes());
    taLabels.setPen(QPen(Qt::darkGreen)); // 设置标签颜色为深绿色
    taLabels.setRotation(90); // 设置标签旋转90度
    topAxis->setTextAttributes(taLabels);
    leftAxis->setTextAttributes(taLabels);
    bottomAxis->setTextAttributes(taLabels);

// 设置以下为0，查看默认的横坐标标签
// (== X标题，从数据文件读取)
#if 1
    // 配置标签及其缩短版本
    QStringList daysOfWeek;
    daysOfWeek << "M O N D A Y"
               << "Tuesday"
               << "Wednesday"
               << "Thursday"
               << "Friday";
    topAxis->setLabels(daysOfWeek);

    QStringList shortDays;
    shortDays << "MON"
              << "Tue"
              << "Wed"
              << "Thu"
              << "Fri";
    topAxis->setShortLabels(shortDays);

    QStringList bottomLabels;
    bottomLabels << "Team A"
                 << "Team B"
                 << "Team C";
    bottomAxis->setLabels(bottomLabels);

    QStringList shortBottomLabels;
    shortBottomLabels << "A"
                      << "B";
    bottomAxis->setShortLabels(shortBottomLabels);
#endif

    // 添加坐标轴到图表
    m_lines->addAxis(topAxis);
    m_lines->addAxis(leftAxis);
    m_lines->addAxis(rightAxis);
    m_lines->addAxis(bottomAxis);

    // 将图表分配给坐标平面
    m_chart->coordinatePlane()->replaceDiagram(m_lines);
}
