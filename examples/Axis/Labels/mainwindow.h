/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴标签配置示例的主窗口头文件，定义了MainWindow类的接口
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
class DatasetProxyModel;
class LineDiagram;
class CartesianAxis;
class Legend;
}

/**
 * @brief 主窗口类
 * 该类继承自QWidget和Ui::MainWindow，负责创建和管理图表界面，
 * 展示如何配置坐标轴标签、注释和自定义刻度等高级功能
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 初始化UI组件，创建图表对象，加载数据模型，并配置坐标轴标签和注释
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 切换注释显示状态的槽函数
     * @param checked 是否显示注释
     * 当复选框状态改变时，更新X轴注释的显示
     */
    void annotationsToggled(bool);
    /**
     * @brief 切换注释上网格线显示状态的槽函数
     * @param checked 是否在注释上显示网格线
     * 当复选框状态改变时，更新注释上网格线的显示
     */
    void gridLinesOnAnnotationsToggled(bool);

private:
    KDChart::Chart *m_chart; // 图表对象，用于显示和管理图表
    KDChart::CartesianAxis *m_xAxis; // X坐标轴对象，用于配置X轴属性
    TableModel m_model; // 数据模型，存储和管理图表数据
    KDChart::DatasetProxyModel *m_datasetProxy; // 数据集代理模型
    KDChart::LineDiagram *m_lines; // 折线图对象，用于绘制折线图
    KDChart::Legend *m_legend; // 图例对象，用于显示图表图例
};

#endif /* MAINWINDOW_H */
