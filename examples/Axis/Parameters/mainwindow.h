/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴参数配置示例的主窗口头文件
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
 * 该类继承自QWidget，负责创建和管理图表界面，展示如何配置坐标轴和图表参数
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 线条类型下拉框索引变化的槽函数
     * @param index 当前选中的索引
     */
    void on_lineTypeCB_currentIndexChanged(int index);
    /**
     * @brief 绘制图例复选框状态变化的槽函数
     * @param checked 是否选中
     */
    void on_paintLegendCB_toggled(bool checked);
    /**
     * @brief 绘制数值复选框状态变化的槽函数
     * @param checked 是否选中
     */
    void on_paintValuesCB_toggled(bool checked);
    /**
     * @brief 绘制标记复选框状态变化的槽函数
     * @param checked 是否选中
     */
    void on_paintMarkersCB_toggled(bool checked);
    /**
     * @brief 标记样式下拉框索引变化的槽函数
     * @param index 当前选中的索引
     */
    void on_markersStyleCB_currentIndexChanged(int index);
    /**
     * @brief 标记宽度微调框值变化的槽函数
     * @param i 当前值
     */
    void on_markersWidthSB_valueChanged(int i);
    /**
     * @brief 标记高度微调框值变化的槽函数
     * @param i 当前值
     */
    void on_markersHeightSB_valueChanged(int i);
    /**
     * @brief 显示区域复选框状态变化的槽函数
     * @param checked 是否选中
     */
    void on_displayAreasCB_toggled(bool checked);
    /**
     * @brief 透明度微调框值变化的槽函数
     * @param i 当前值
     */
    void on_transparencySB_valueChanged(int i);
    /**
     * @brief 缩放因子微调框值变化的槽函数
     * @param factor 当前缩放因子
     */
    void on_zoomFactorSB_valueChanged(double factor);
    /**
     * @brief 水平滚动条值变化的槽函数
     * @param value 当前值
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
