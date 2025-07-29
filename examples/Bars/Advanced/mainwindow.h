/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，展示高级柱状图的创建和配置
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
 * 该类继承自QWidget和Ui::MainWindow，用于展示高级柱状图的配置和交互
 * 包含图表对象、柱状图对象和数据模型，实现了多种柱状图属性的动态调整
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 初始化图表、柱状图和数据模型，设置信号槽连接
     */
    MainWindow(QWidget *parent = nullptr);

private slots:

    /**
     * @brief 柱状图类型下拉框选择变化槽函数
     * @param index 选中的索引
     * 根据选择的索引设置柱状图的类型
     */
    void on_barTypeCB_currentIndexChanged(int index);
    
    /**
     * @brief 柱状图方向下拉框选择变化槽函数
     * @param index 选中的索引
     * 根据选择的索引设置柱状图的方向（水平或垂直）
     */
    void on_barOrientationCB_currentIndexChanged(int index);
    
    /**
     * @brief 显示数值复选框状态变化槽函数
     * @param checked 是否选中
     * 设置是否在柱状图上显示数值
     */
    void on_paintValuesCB_toggled(bool checked);
    
    /**
     * @brief 3D效果复选框状态变化槽函数
     * @param checked 是否选中
     * 设置柱状图是否使用3D效果
     */
    void on_paintThreeDBarsCB_toggled(bool checked);
    
    /**
     * @brief 标记列复选框状态变化槽函数
     * @param checked 是否选中
     * 设置是否标记指定的列
     */
    void on_markColumnCB_toggled(bool checked);
    
    /**
     * @brief 标记列数值变化槽函数
     * @param i 列索引值
     * 设置要标记的列索引
     */
    void on_markColumnSB_valueChanged(int i);
    
    /**
     * @brief 3D深度复选框状态变化槽函数
     * @param checked 是否选中
     * 设置是否启用3D深度调整
     */
    void on_threeDDepthCB_toggled(bool checked);
    
    /**
     * @brief 深度数值变化槽函数
     * @param i 深度值
     * 设置3D柱状图的深度
     */
    void on_depthSB_valueChanged(int i);
    
    /**
     * @brief 宽度复选框状态变化槽函数
     * @param checked 是否选中
     * 设置是否启用宽度调整
     */
    void on_widthCB_toggled(bool checked);
    
    /**
     * @brief 宽度数值变化槽函数
     * @param i 宽度值
     * 设置柱状图的宽度
     */
    void on_widthSB_valueChanged(int i);
    
    /**
     * @brief 固定平面大小复选框状态变化槽函数
     * @param checked 是否选中
     * 设置是否固定坐标平面的大小
     */
    void on_fixPlaneSizeCB_toggled(bool checked);

private:
    KDChart::Chart *m_chart; // 图表对象，用于显示和管理图表元素
    KDChart::BarDiagram *m_bars; // 柱状图对象，负责绘制柱状图
    TableModel m_model; // 数据模型，存储图表数据
};

#endif /* MAINWINDOW_H */
