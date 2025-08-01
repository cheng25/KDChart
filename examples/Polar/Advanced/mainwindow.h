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
// 包含UI文件生成的头文件
#include <QStandardItemModel>
// 包含Qt标准项模型头文件

namespace KDChart {
class Chart;
class DatasetProxyModel;
class PolarDiagram;
class PolarCoordinatePlane;
} // 前向声明KDChart命名空间中的类，减少编译依赖

/**
 * @brief 主窗口类
 * @details 该类是极坐标图表高级示例的主窗口，负责初始化图表、处理用户交互
 * 和显示极坐标图表数据。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // 圆形网格的起始位置
    /**
     * @brief 处理起始位置微调框值变化
     * @param pos 新的起始位置值
     */
    void on_startPositionSB_valueChanged(double pos);
    /**
     * @brief 处理起始位置滑动条值变化
     * @param pos 新的起始位置值
     */
    void on_startPositionSL_valueChanged(int pos);

    // 显示网格
    /**
     * @brief 处理圆形网格复选框状态变化
     * @param toggle 是否启用圆形网格
     */
    void on_circularGridCB_toggled(bool toggle);
    /**
     * @brief 处理径向网格复选框状态变化
     * @param toggle 是否启用径向网格
     */
    void on_sagittalGridCB_toggled(bool toggle);

private:
    /**
     * @brief 初始化KDChart相关类
     * @details 创建图表、极坐标平面和极坐标图表对象
     */
    void initKDChartClasses();
    /**
     * @brief 连接KDChart相关类
     * @details 设置图表和数据模型之间的关联
     */
    void wireUpKDChartClasses();
    /**
     * @brief 设置数据模型数据
     * @details 为图表数据模型填充示例数据
     */
    void setItemModelData();

    // 这次我们不使用特殊的TableModel，而是使用Qt自带的标准模型
    QStandardItemModel m_model; // 数据模型，存储图表显示的数据

    KDChart::Chart *m_chart; // 图表对象，用于管理图表显示
    KDChart::PolarDiagram *m_diagram; // 极坐标图表对象
    KDChart::PolarCoordinatePlane *m_polarPlane; // 极坐标坐标平面

    int m_currentFactor = 0; // 当前因子
    int m_currentDirection = 1; // 当前方向
    int m_currentSlice = 0; // 当前切片
};

#endif /* MAINWINDOW_H */
