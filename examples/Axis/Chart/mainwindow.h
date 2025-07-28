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

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class BarDiagram;
}

/**
 * @brief 主窗口类
 * 该类继承自QWidget和Ui::MainWindow，负责创建和管理图表界面，
 * 展示如何配置坐标轴的各种属性，如位置、标题、标签、刻度线等
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 初始化UI组件，创建图表对象，加载数据模型，并配置坐标轴属性
     */
    MainWindow(QWidget *parent = nullptr);

private:
    KDChart::Chart *m_chart; // 图表对象，用于显示和管理图表
    TableModel m_model; // 数据模型，存储和管理图表数据
    KDChart::BarDiagram *m_lines; // 柱状图对象，用于绘制柱状图
};

#endif /* MAINWINDOW_H */
