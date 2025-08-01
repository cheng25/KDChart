/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴配置示例的主窗口头文件，定义了MainWindow类的接口
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001年KDAB集团公司(Klarälvdalens Datakonsult AB)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT开源许可
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"  // 包含UI设计文件生成的头文件
#include <TableModel.h>     // 包含表格数据模型类头文件

namespace KDChart {
class Chart;      // 前向声明KDChart::Chart类（图表核心类）
class BarDiagram; // 前向声明KDChart::BarDiagram类（柱状图类）
}

/**
 * @brief 主窗口类
 * @details 继承自QWidget和Ui::MainWindow接口，是坐标轴配置示例的核心容器类
 *          负责整合图表组件、数据模型和UI交互元素，实现坐标轴的多样化配置演示
 *          支持坐标轴位置调整、标题设置、刻度样式自定义等核心功能。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @details 初始化UI，创建图表对象，设置数据模型，并配置坐标轴属性。
     */
    MainWindow(QWidget *parent = nullptr);

private:
    KDChart::Chart *m_chart;      // 图表核心对象，管理坐标系和图表元素的顶层容器
    TableModel m_model;           // 表格数据模型，存储从CSV文件加载的柱状图数据
    KDChart::BarDiagram *m_lines; // 柱状图绘制组件，负责数据可视化呈现
}; // TODO: C++17升级 考虑使用final修饰符防止继承

// TODO: Qt5.15.2升级 检查KDChart::Chart在Qt5.15.2中的坐标系管理API变化
// TODO: Qt5.15.2升级 验证QWidget::setLayout()在高DPI环境下的布局行为
// TODO: C++17升级 使用std::unique_ptr管理m_chart和m_lines动态对象
// TODO: C++17升级 考虑将m_model改为constexpr初始化（若数据固定）

#endif /* MAINWINDOW_H */
