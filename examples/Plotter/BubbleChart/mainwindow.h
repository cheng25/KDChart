/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件定义了BubbleChart项目的主窗口类
// 该类负责创建和管理气泡图图表，包括数据模型初始化和标记属性设置
// 气泡图是一种特殊类型的散点图，其中数据点的大小表示第三个维度的值
// 本类展示了如何使用KD Chart库创建和配置气泡图，并处理相关交互

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// UI文件头，由uic工具根据mainwindow.ui生成
// TODO: Qt5.15.2升级 检查ui_mainwindow.h是否需要适配新版本
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
// 前向声明QStandardItemModel类，用于数据存储
class QStandardItemModel;
QT_END_NAMESPACE

namespace KDChart {
// 前向声明KDChart::Chart类，图表主类
class Chart;
// 前向声明KDChart::Plotter类，绘图仪类
class Plotter;
}

/**
 * @brief 主窗口类
 * @details 继承自QWidget和Ui::MainWindow，负责管理气泡图的显示和交互
 * 包含图表、绘图仪和数据模型的初始化与配置
 * 提供数据加载和标记样式设置功能
 * TODO: Qt5.15.2升级 检查QWidget相关API是否有变更
 * TODO: C++17升级 考虑使用override关键字明确标识重写的方法
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * @details 初始化主窗口，创建图表和绘图仪
     * 设置布局和连接信号槽
     * 初始化数据模型并配置标记属性
     * TODO: Qt5.15.2升级 检查构造函数中使用的API是否需要适配
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 初始化数据模型
     * @details 填充数据模型，为气泡图提供数据
     * 生成X轴、Y轴和气泡大小的数据
     * TODO: Qt5.15.2升级 检查QStandardItemModel API是否有变更
     * TODO: C++17升级 考虑使用结构化绑定和范围for循环优化数据生成
     */
    void initializeDataModel();

    /**
     * @brief 设置标记属性
     * @details 配置气泡图中标记的样式和大小
     * 设置标记的形状、边框和填充颜色等属性
     * TODO: Qt5.15.2升级 检查KDChart::MarkerAttributes API是否有变更
     */
    void setMarkerAttributes();

private:
    // 数据模型，存储气泡图的数据，包含X轴、Y轴和气泡大小数据
    // 每行代表一个数据点，列0: X值，列1: Y值，列2: 气泡大小值
    QStandardItemModel *m_model = nullptr;
    // 图表对象，用于显示绘图仪和坐标轴
    KDChart::Chart *m_chart;
    // 绘图仪对象，用于绘制气泡图
    KDChart::Plotter *m_plotter;
};

#endif /* MAINWINDOW_H */
