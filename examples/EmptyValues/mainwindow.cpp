/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** This file is part of the KD Chart library.
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** 许可协议: MIT
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"  // 包含MainWindow类定义

#include <KDChartAbstractCoordinatePlane>  // 包含抽象坐标平面类
#include <KDChartBarDiagram>  // 包含柱状图类
#include <KDChartCartesianAxis>  // 包含笛卡尔坐标轴类
#include <KDChartChart>  // 包含图表类
// TODO: Qt5.15.2升级 检查KDChart库头文件是否与Qt5.15.2兼容
// C++17 兼容性: 头文件包含方式符合C++17标准

#include <QDebug>  // 包含调试输出类
#include <QHBoxLayout>  // 包含水平布局类
#include <QPen>  // 包含画笔类
// TODO: Qt5.15.2升级 考虑使用#include <QHBoxLayout>替代旧的头文件形式
// C++17 兼容性: Qt头文件与C++17兼容

using namespace KDChart;  // 使用KDChart命名空间，简化代码编写
// C++17 兼容性: using namespace指令符合C++17标准

/**
 * @brief MainWindow::MainWindow 构造函数
 * @param parent 父窗口部件指针，默认为nullptr
 * @return 无返回值
 * @details 初始化主窗口，创建图表布局，加载包含空值的数据，设置柱状图和坐标轴。
 * 此构造函数演示了如何处理包含空值的数据并在图表中正确显示。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)  // 调用基类QWidget的构造函数
{
    // C++17 兼容性: 使用auto关键字自动推导类型，符合C++17标准
    auto *chartLayout = new QHBoxLayout(this);  // 创建水平布局，并设置父部件为此窗口
    m_chart = new Chart();  // 创建图表对象
    chartLayout->addWidget(m_chart);  // 将图表添加到布局中

    m_model.loadFromCSV(" :/data ");  // 从CSV文件加载数据，文件路径为资源文件中的data
    // TODO: Qt5.15.2升级 检查QStringLiteral是否可用于优化字符串处理

    // 设置图表
    m_bars = new BarDiagram();  // 创建柱状图对象
    m_bars->setModel(&m_model);  // 设置数据模型，关联到m_model
    m_bars->addAxis(new CartesianAxis(m_bars));  // 为柱状图添加笛卡尔坐标轴

    m_chart->coordinatePlane()->replaceDiagram(m_bars);  // 替换坐标平面的图表为柱状图

    // C++17 兼容性: dynamic_cast在C++17中仍受支持
    auto *plane = dynamic_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane());  // 获取笛卡尔坐标平面
    Q_ASSERT(plane);  // 断言平面不为空，如果为空则程序终止
    // 模型中的值都是零，所以手动设置平面的大小为非零值
    plane->setVerticalRange(QPair<qreal, qreal>(-2.0, 2.0));  // 设置垂直范围为-2.0到2.0
    // C++17 兼容性: QPair在Qt5.15.2中与C++17兼容
}
