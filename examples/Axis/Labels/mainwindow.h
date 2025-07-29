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
 * 负责创建和管理图表界面，配置坐标轴属性，处理用户交互
 * 核心功能：展示自定义坐标轴效果，提供标签显示切换功能
 * 
 * Qt5.15.2升级说明：
 * - 检查QWidget及其派生类在Qt5.15.2中的行为变更
 * - 验证UI文件(mainwindow.ui)中使用的控件是否与Qt5.15.2兼容
 * 
 * C++17升级说明：
 * - 可使用std::unique_ptr管理动态创建的图表对象
 * - 考虑使用结构化绑定优化多返回值场景
 */
// TODO: Qt5.15.2升级 检查QWidget::setLayout在Qt5.15.2中的高DPI布局行为
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @return 无返回值
     * @details 初始化UI组件，创建图表对象，配置坐标轴属性
     */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     * @return 无返回值
     * @details 释放动态分配的资源，包括图表对象和数据模型
     */
    ~MainWindow() override;

private slots:
    /**
     * @brief 切换注释显示状态
     * @param checked 复选框状态
     * @return 无返回值
     * @details 根据复选框状态显示或隐藏坐标轴注释
     */
    void on_checkBoxAnnotations_toggled(bool checked);

private:
    KDChart::Chart* m_chart = nullptr;          // 图表对象，负责管理图表组件
    AdjustedCartesianAxis* m_axis = nullptr;    // 自定义坐标轴实例
    TableModel m_model;                         // 数据模型，存储图表展示数据
    KDChart::BarDiagram* m_diagram = nullptr;   // 柱状图对象，负责数据可视化
};

#endif /* MAINWINDOW_H */
