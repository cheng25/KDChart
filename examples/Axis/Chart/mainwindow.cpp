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

#include "mainwindow.h"  // 包含主窗口头文件
#include <KDChartBarDiagram>    // 包含柱状图类头文件
#include <KDChartChart>         // 包含图表类头文件
#include <KDChartFrameAttributes> // 包含框架属性类头文件
#include <KDChartRulerAttributes> // 包含坐标轴属性类头文件
#include <KDChartTextAttributes>  // 包含文本属性类头文件

using namespace KDChart;  // 使用KDChart命名空间

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口部件，默认为nullptr
 * @return 无返回值
 * @details 初始化UI组件，创建图表和数据模型，配置坐标轴的位置、标题、标签和刻度线等属性。
 * 本构造函数是示例的核心，展示了如何自定义坐标轴的各种视觉属性。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件，根据ui文件创建界面元素

    // 创建图表布局并添加图表部件
    auto *chartLayout = new QHBoxLayout(chartFrame);  // 创建水平布局
    m_chart = new Chart();                            // 创建图表对象
    m_chart->setGlobalLeading(10, 10, 10, 10);        // 设置图表全局边距（上、右、下、左）
    chartLayout->addWidget(m_chart);                  // 将图表添加到布局
    hSBar->setVisible(false);                         // 隐藏水平滚动条
    vSBar->setVisible(false);                         // 隐藏垂直滚动条

    m_model.loadFromCSV(" :/data "); // 从CSV文件加载数据（注意这里有个空格，可能是代码中的笔误）

    // 设置图表
    m_lines = new BarDiagram();      // 创建柱状图对象
    m_lines->setModel(&m_model);     // 设置数据模型

    // 创建并定位坐标轴
    auto *topAxis = new CartesianAxis(m_lines);    // 创建顶部坐标轴
    auto *leftAxis = new CartesianAxis(m_lines);    // 创建左侧坐标轴
    RulerAttributes rulerAttr = topAxis->rulerAttributes(); // 获取坐标轴属性
    rulerAttr.setTickMarkPen(0.9999999, QPen(Qt::red)); // 设置主刻度线颜色为红色
    rulerAttr.setTickMarkPen(2.0, QPen(Qt::green)); // 设置次刻度线颜色为绿色
    rulerAttr.setTickMarkPen(3.0, QPen(Qt::blue));  // 设置第三刻度线颜色为蓝色
    rulerAttr.setShowMinorTickMarks(true);          // 显示次刻度线
    // rulerAttr.setShowMajorTickMarks(false);      // 注释掉的代码：隐藏主刻度线
    topAxis->setRulerAttributes(rulerAttr);         // 应用坐标轴属性
    auto *rightAxis = new CartesianAxis(m_lines);   // 创建右侧坐标轴
    auto *bottomAxis = new CartesianAxis(m_lines);  // 创建底部坐标轴
    topAxis->setPosition(CartesianAxis::Top);       // 设置坐标轴位置为顶部
    leftAxis->setPosition(CartesianAxis::Left);     // 设置坐标轴位置为左侧
    rightAxis->setPosition(CartesianAxis::Right);   // 设置坐标轴位置为右侧
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
    topAxis->setTitleText("Abscissa Top configured size and color"); // 顶部坐标轴标题
    leftAxis->setTitleText("left Ordinate: fonts configured");      // 左侧坐标轴标题
    rightAxis->setTitleText("right Ordinate: default settings");   // 右侧坐标轴标题
    bottomAxis->setTitleText("Abscissa Bottom");                    // 底部坐标轴标题

    // 配置标题文本属性
    TextAttributes taTop(topAxis->titleTextAttributes()); // 获取顶部坐标轴标题文本属性
    taTop.setPen(QPen(Qt::red));                           // 设置顶部坐标轴标题颜色为红色
    topAxis->setTitleTextAttributes(taTop);                // 应用文本属性

    TextAttributes taLeft(leftAxis->titleTextAttributes()); // 获取左侧坐标轴标题文本属性
    taLeft.setRotation(180);                                // 设置左侧坐标轴标题旋转180度
    Measure me(taLeft.fontSize());                          // 获取字体大小
    me.setValue(me.value() * 0.8);                          // 设置字体大小为原来的80%
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

// TODO: Qt5.15.2升级 检查KDChart::CartesianAxis在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QHBoxLayout和相关UI组件行为变化
// TODO: C++17升级 考虑使用std::filesystem替代QFile进行文件操作
// TODO: C++17升级 考虑使用结构化绑定优化多变量赋值