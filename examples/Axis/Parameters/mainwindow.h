/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴参数配置示例的主窗口头文件，定义了MainWindow类的接口
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
class LineAttributes;
class CartesianAxis;
class Legend;
}

/**
 * @brief 主窗口类
 * 该类继承自QWidget和Ui::MainWindow，负责创建和管理图表界面，
 * 展示如何配置坐标轴和图表参数，如线条类型、图例、数据值显示、
 * 标记样式、透明度、缩放因子和滚动条等交互功能。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 初始化UI组件，创建图表对象，加载数据模型，并配置坐标轴和图表参数。
     * 具体包括：设置图表布局、初始化数据模型、创建折线图、配置坐标轴属性、
     * 设置图例以及连接UI控件的信号与槽函数。
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 线条类型下拉框索引变化的槽函数
     * @param index 当前选中的索引
     * 当下拉框选中不同的线条类型时，更新图表中线条的显示样式。
     */
    void on_lineTypeCB_currentIndexChanged(int index);
    /**
     * @brief 绘制图例复选框状态变化的槽函数
     * @param checked 是否选中
     * 当复选框状态改变时，控制图例的显示或隐藏。如果checked为true，则显示图例；
     * 否则隐藏图例。
     */
    void on_paintLegendCB_toggled(bool checked);
    /**
     * @brief 绘制数值复选框状态变化的槽函数
     * @param checked 是否选中
     * 当复选框状态改变时，控制图表中数据值的显示或隐藏。如果checked为true，则显示数据值；
     * 否则隐藏数据值。
     */
    void on_paintValuesCB_toggled(bool checked);
    /**
     * @brief 绘制标记复选框状态变化的槽函数
     * @param checked 是否选中
     * 当复选框状态改变时，控制图表中数据点标记的显示或隐藏。如果checked为true，则显示标记；
     * 否则隐藏标记。
     */
    void on_paintMarkersCB_toggled(bool checked);
    /**
     * @brief 标记样式下拉框索引变化的槽函数
     * @param index 当前选中的索引
     * 当下拉框选中不同的标记样式时，更新图表中数据点标记的显示样式。
     */
    void on_markersStyleCB_currentIndexChanged(int index);
    /**
     * @brief 标记宽度微调框值变化的槽函数
     * @param i 当前值
     * 当微调框的值变化时，更新图表中数据点标记的宽度。
     */
    void on_markersWidthSB_valueChanged(int i);
    /**
     * @brief 标记高度微调框值变化的槽函数
     * @param i 当前值
     * 当微调框的值变化时，更新图表中数据点标记的高度。
     */
    void on_markersHeightSB_valueChanged(int i);
    /**
     * @brief 显示区域复选框状态变化的槽函数
     * @param checked 是否选中
     * 当复选框状态改变时，控制图表中区域填充的显示或隐藏。如果checked为true，则显示区域填充；
     * 否则隐藏区域填充。
     */
    void on_displayAreasCB_toggled(bool checked);
    /**
     * @brief 透明度微调框值变化的槽函数
     * @param i 当前值
     * 当微调框的值变化时，更新图表中区域填充的透明度。值越大，透明度越高。
     */
    void on_transparencySB_valueChanged(int i);
    /**
     * @brief 缩放因子微调框值变化的槽函数
     * @param factor 当前缩放因子
     * 当微调框的值变化时，更新图表的缩放比例。factor大于1时放大图表，小于1时缩小图表。
     */
    void on_zoomFactorSB_valueChanged(double factor);
    /**
     * @brief 水平滚动条值变化的槽函数
     * @param value 当前值
     * 当滚动条的值变化时，水平平移图表的显示区域。
     */
    void on_hSBar_valueChanged(int value);
    /**
     * @brief 垂直滚动条值变化的槽函数
     * @param value 当前值
     */
    void on_vSBar_valueChanged(int value);

private:
    KDChart::Chart *m_chart; // 图表对象
    TableModel m_model; // 数据模型
    KDChart::DatasetProxyModel *m_datasetProxy; // 数据集代理模型
    KDChart::LineDiagram *m_lines; // 折线图对象
    KDChart::Legend *m_legend; // 图例对象
    // mutable KDChart::CartesianAxis xAxis;
    // mutable KDChart::CartesianAxis yAxis;
};

#endif /* MAINWINDOW_H */
