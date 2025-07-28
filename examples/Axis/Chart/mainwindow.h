/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴配置示例的主窗口头文件，定义了MainWindow类的接口
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"  // 包含UI设计文件生成的头文件
#include <TableModel.h>     // 包含数据模型类头文件

namespace KDChart {
class Chart;      // 前向声明KDChart::Chart类
class BarDiagram; // 前向声明KDChart::BarDiagram类
}

/**
 * @brief 主窗口类
 * 该类继承自QWidget和Ui::MainWindow，负责创建和管理图表界面，
 * 展示如何配置坐标轴的各种属性，如位置、标题、标签、刻度线等。
 * 本示例主要演示了坐标轴的配置方法和效果。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * @return 无返回值
     * @details 初始化UI组件，创建图表对象，加载数据模型，
     * 配置坐标轴属性，并将图表添加到UI布局中。
     * 构造函数是类的入口点，负责完成所有初始化工作。
     */
    MainWindow(QWidget *parent = nullptr);

private:
    KDChart::Chart *m_chart;      // 图表对象，用于显示和管理图表及其所有元素
    TableModel m_model;           // 数据模型，存储和管理图表要显示的数据
    KDChart::BarDiagram *m_lines; // 柱状图对象，用于绘制和配置柱状图
};

// TODO: Qt5.15.2升级 检查KDChart相关类在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QWidget使用规范
// TODO: C++17升级 考虑使用std::optional优化可能为空的指针成员

#endif /* MAINWINDOW_H */
