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
 * @brief MainWindow类构造函数
 * @param parent 父窗口指针，默认为nullptr
 * @details 该构造函数是坐标轴配置示例的核心实现，完成以下关键任务：
 *          1. 初始化UI界面组件和布局管理器
 *          2. 创建KDChart图表框架和数据模型
 *          3. 配置柱状图显示属性
 *          4. 创建并定制多位置坐标轴（上、下、左、右）
 *          5. 设置坐标轴标题、文本样式和刻度属性
 *          6. 将所有组件整合到主窗口布局中
 *          本示例重点展示了坐标轴的高级定制功能，包括多坐标轴共存、
 *          样式差异化配置和动态属性调整等核心特性。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件，加载.ui文件定义的界面布局

    // 创建图表布局并添加图表部件
    auto *chartLayout = new QHBoxLayout(chartFrame);  // 创建水平布局管理器，用于图表框架
    m_chart = new Chart();                            // 实例化KDChart核心图表对象
    m_chart->setGlobalLeading(10, 10, 10, 10);        // 设置图表四周留白（上、右、下、左），单位：像素
    chartLayout->addWidget(m_chart);                  // 将图表部件添加到布局管理器
    hSBar->setVisible(false);                         // 隐藏水平滚动条（示例中图表尺寸固定）
    vSBar->setVisible(false);                         // 隐藏垂直滚动条（示例中图表尺寸固定）

    m_model.loadFromCSV(":/data"); // 从资源文件加载CSV格式数据到数据模型

    // 设置图表
    m_lines = new BarDiagram();      // 创建柱状图绘制组件
    m_lines->setModel(&m_model);     // 将数据模型关联到柱状图组件

    // 创建并定位坐标轴
    auto *topAxis = new CartesianAxis(m_lines);    // 创建顶部X轴，关联柱状图数据
    auto *leftAxis = new CartesianAxis(m_lines);    // 创建左侧Y轴，关联柱状图数据
    RulerAttributes rulerAttr = topAxis->rulerAttributes(); // 获取坐标轴标尺属性对象
    rulerAttr.setTickMarkPen(0.9999999, QPen(Qt::red)); // 设置主刻度线样式（宽度：0.9999999px，红色）
    rulerAttr.setTickMarkPen(2.0, QPen(Qt::green)); // 设置次刻度线样式（宽度：2px，绿色）
    rulerAttr.setTickMarkPen(3.0, QPen(Qt::blue));  // 设置第三刻度线样式（宽度：3px，蓝色）
    rulerAttr.setShowMinorTickMarks(true);          // 启用次刻度线显示
    // rulerAttr.setShowMajorTickMarks(false);      // 禁用主刻度线显示（当前为注释状态）
    topAxis->setRulerAttributes(rulerAttr);         // 应用坐标轴标尺属性配置
    auto *rightAxis = new CartesianAxis(m_lines);   // 创建右侧Y轴，关联柱状图数据
    auto *bottomAxis = new CartesianAxis(m_lines);  // 创建底部X轴，关联柱状图数据
    topAxis->setPosition(CartesianAxis::Top);       // 设置坐标轴显示位置：顶部
    leftAxis->setPosition(CartesianAxis::Left);     // 设置坐标轴显示位置：左侧
    rightAxis->setPosition(CartesianAxis::Right);   // 设置坐标轴显示位置：右侧
    bottomAxis->setPosition(CartesianAxis::Bottom); // 设置坐标轴显示位置：底部

// 设置标签和刻度之间的边距为零（当前为注释状态）
#if 0
    RulerAttributes ra = bottomAxis->rulerAttributes();
    ra.setLabelMargin( 0 );  // 设置标签与刻度线之间的边距为0像素
    bottomAxis->setRulerAttributes( ra );
#endif

// 为底部坐标轴显示红色边框（当前为注释状态）
#if 0
    FrameAttributes fa( bottomAxis->frameAttributes() );
    fa.setPen( QPen( QBrush( QColor( "#ff0000" ) ), 1.0 ) ); // 设置边框为1px红色实线
    fa.setVisible( true );  // 启用边框显示
    bottomAxis->setFrameAttributes( fa );
#endif

    // 设置坐标轴标题
    topAxis->setTitleText("Abscissa Top configured size and color"); // 顶部坐标轴标题文本
    leftAxis->setTitleText("left Ordinate: fonts configured");      // 左侧坐标轴标题文本
    rightAxis->setTitleText("right Ordinate: default settings");   // 右侧坐标轴标题文本
    bottomAxis->setTitleText("Abscissa Bottom");                    // 底部坐标轴标题文本

    // 配置标题文本属性
    TextAttributes taTop(topAxis->titleTextAttributes()); // 获取顶部坐标轴标题文本属性
    taTop.setPen(QPen(Qt::red));                           // 设置标题文本颜色为红色
    topAxis->setTitleTextAttributes(taTop);                // 应用顶部坐标轴标题文本属性

    TextAttributes taLeft(leftAxis->titleTextAttributes()); // 获取左侧坐标轴标题文本属性
    taLeft.setRotation(180);                                // 设置标题文本旋转180度（垂直显示）
    Measure me(taLeft.fontSize());                          // 获取当前字体大小度量对象
    me.setValue(me.value() * 0.8);                          // 将字体大小缩小为原来的80%
    taLeft.setFontSize(me);

// 隐藏左侧坐标轴标题（当前为注释状态）
#if 0
    taLeft.setVisible( false );
#endif
    leftAxis->setTitleTextAttributes(taLeft);               // 应用左侧坐标轴标题文本属性

    TextAttributes taBottom(bottomAxis->titleTextAttributes());
    taBottom.setPen(QPen(Qt::blue)); // 设置底部坐标轴标题文本颜色为蓝色
    bottomAxis->setTitleTextAttributes(taBottom);           // 应用底部坐标轴标题文本属性

    // 配置标签文本属性
    TextAttributes taLabels(topAxis->textAttributes());
    taLabels.setPen(QPen(Qt::darkGreen)); // 设置坐标轴标签文本颜色为深绿色
    taLabels.setRotation(90); // 设置标签文本旋转90度（垂直显示）
    topAxis->setTextAttributes(taLabels);
    leftAxis->setTextAttributes(taLabels);
    bottomAxis->setTextAttributes(taLabels);

// 配置坐标轴标签内容（当前为启用状态）
#if 1
    // 配置顶部坐标轴标签
    QStringList daysOfWeek;
    daysOfWeek << "M O N D A Y" << "Tuesday" << "Wednesday" << "Thursday" << "Friday";
    topAxis->setLabels(daysOfWeek); // 设置完整标签文本

    QStringList shortDays;
    shortDays << "MON" << "Tue" << "Wed" << "Thu" << "Fri";
    topAxis->setShortLabels(shortDays); // 设置缩短标签文本（空间不足时显示）

    // 配置底部坐标轴标签
    QStringList bottomLabels;
    bottomLabels << "Team A" << "Team B" << "Team C";
    bottomAxis->setLabels(bottomLabels); // 设置完整标签文本

    QStringList shortBottomLabels;
    shortBottomLabels << "A" << "B";
    bottomAxis->setShortLabels(shortBottomLabels); // 设置缩短标签文本
#endif

    // 添加坐标轴到图表
    m_lines->addAxis(topAxis);
    m_lines->addAxis(leftAxis);
    m_lines->addAxis(rightAxis);
    m_lines->addAxis(bottomAxis);

    // 将图表分配给坐标平面
    m_chart->coordinatePlane()->replaceDiagram(m_lines);
}

// TODO: Qt5.15.2升级 检查KDChart::CartesianAxis在Qt5.15.2中的坐标系计算逻辑变化
// TODO: Qt5.15.2升级 验证QHBoxLayout在高DPI环境下的布局计算精度
// TODO: Qt5.15.2升级 检查TextAttributes类的字体渲染API是否有变更
// TODO: C++17升级 使用std::unique_ptr管理所有动态创建的图表组件对象
// TODO: C++17升级 使用结构化绑定优化多变量属性配置代码
// TODO: C++17升级 考虑使用std::filesystem::path处理CSV文件路径
// TODO: C++17升级 可使用if constexpr优化条件编译代码块