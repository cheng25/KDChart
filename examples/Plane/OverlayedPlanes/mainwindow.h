/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
class LineAttributes;
class CartesianAxis;
class CartesianCoordinatePlane;
}

/**
 * @brief 主窗口类
 * 该类继承自QWidget并实现了Ui::MainWindow接口，用于展示如何在KD Chart库中创建叠加的图表平面
 * 本示例主要演示了如何在同一个图表中添加多个坐标平面并在其上绘制不同的数据
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * @return 无返回值
     * @details 初始化主窗口，设置UI组件，并调用init()方法初始化图表
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 切换第一个网格显示状态的槽函数
     * @param checked 是否显示网格
     * @return 无返回值
     * @details 响应UI中displayGrid1CB复选框的状态变化，控制第一个平面的网格显示
     */
    void on_displayGrid1CB_toggled(bool checked);

    /**
     * @brief 切换第二个网格显示状态的槽函数
     * @param checked 是否显示网格
     * @return 无返回值
     * @details 响应UI中displayGrid2CB复选框的状态变化，控制第二个平面的网格显示
     */
    void on_displayGrid2CB_toggled(bool checked);

private:
    /**
     * @brief 初始化方法
     * @return 无返回值
     * @details 创建图表、数据模型和坐标平面，配置图表属性和数据
     */
    void init();

    KDChart::Chart *m_chart;                   // 图表对象，用于显示和管理图表及其所有元素
    TableModel m_model;                        // 第一个数据模型，存储第一条数据集
    TableModel m_model2;                       // 第二个数据模型，存储第二条数据集
    KDChart::DatasetProxyModel *m_datasetProxy; // 数据集代理模型，用于处理数据显示
    KDChart::LineDiagram *m_lines;             // 第一个折线图对象，用于在第一个平面上显示数据
    KDChart::LineDiagram *m_lines2;            // 第二个折线图对象，用于在第二个平面上显示数据
    // TODO: Qt5.15.2升级 检查KDChart::CartesianCoordinatePlane在Qt5.15.2中的兼容性
    KDChart::CartesianCoordinatePlane *plane2; // 第二个坐标平面，用于叠加显示不同的数据
};

#endif /* MAINWINDOW_H */
