/****************************************************************************
**
** 此文件是KD Chart图表库的一部分。
** 本文件定义了带滚动条缩放功能的示例主窗口类。
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

#include "ui_mainwindow.h" // 包含UI设计头文件
#include <TableModel.h>    // 包含表格数据模型头文件

namespace KDChart {
class Chart;       // 前向声明Chart类
class LineDiagram; // 前向声明LineDiagram类
class Legend;      // 前向声明Legend类
}

/**
 * @brief 主窗口类
 * @details 此类实现了带滚动条缩放功能的图表窗口，支持设置缩放因子、调整网格等功能。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 缩放因子滑动条值变化槽函数
     * @param factor 新的缩放因子
     */
    void on_zoomFactorSB_valueChanged(double factor);
    /**
     * @brief 调整网格复选框状态变化槽函数
     * @param checked 是否选中
     */
    void on_adjustGridCB_toggled(bool checked);
    /**
     * @brief 橡皮筋缩放复选框状态变化槽函数
     * @param checked 是否选中
     */
    void on_rubberBandZoomCB_toggled(bool checked);
    /**
     * @brief 水平滚动条值变化槽函数
     * @param value 新的滚动条值
     */
    void on_hSBar_valueChanged(int value);
    /**
     * @brief 垂直滚动条值变化槽函数
     * @param value 新的滚动条值
     */
    void on_vSBar_valueChanged(int value);

    /**
     * @brief 应用新的缩放参数
     */
    void applyNewZoomParameters();

private:
    KDChart::Chart *m_chart;       // 图表对象指针
    TableModel m_model;            // 表格数据模型
    KDChart::LineDiagram *m_lines; // 折线图对象指针
    KDChart::Legend *m_legend;     // 图例对象指针

// TODO: Qt5.15.2升级 检查QWidget和Q_OBJECT宏在新版本中的使用
// TODO: Qt5.15.2升级 验证KDChart::Chart与Qt5.15.2的兼容性
// TODO: C++17升级 考虑使用智能指针管理动态内存
// TODO: C++17升级 使用结构化绑定优化信号槽连接
};

#endif /* MAINWINDOW_H */
