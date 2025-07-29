/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件定义了BigDataset应用程序的MainWidget类
// 该类是应用程序的主窗口部件，负责管理UI控件、绘图仪和数据模型
// 提供了函数类型切换（如正弦、余弦、线性等函数）和动态添加数据点的功能
// 支持处理和可视化大型数据集，展示KD Chart库的性能和灵活性

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

// 数据模型头文件
// TODO: Qt5.15.2升级 检查Model类是否需要适配Qt5.15.2的API变更
#include "Model.h"
// UI控件头文件
// TODO: Qt5.15.2升级 检查ui_Controls.h中使用的Qt API是否有变更
#include "ui_Controls.h"
// QWidget基类头文件
// TODO: Qt5.15.2升级 检查QWidget头文件是否需要替换或添加其他头文件
#include <QWidget>

namespace KDChart {
// 前向声明KDChart::Plotter类，用于绘制图表
class Plotter;
};
// 前向声明Model类，用于存储图表数据
class Model;

QT_BEGIN_NAMESPACE
// 前向声明QPushButton类，用于按钮控件
class QPushButton;
// 前向声明QRadioButton类，用于单选按钮控件
class QRadioButton;
QT_END_NAMESPACE

/**
 * @brief 主窗口部件类
 * @details 继承自QWidget，负责管理应用程序的UI和逻辑
 * 包含绘图仪、数据模型和各种UI控件
 * 支持函数类型切换和动态添加数据点
 * 能够处理和可视化大型数据集
 * TODO: Qt5.15.2升级 检查QWidget相关API是否有变更
 * TODO: C++17升级 考虑使用override关键字明确标记重写函数
 */
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @details 初始化MainWidget类的实例
     * 创建UI控件、绘图仪和数据模型
     * 设置信号槽连接
     * 初始化图表和数据
     * TODO: Qt5.15.2升级 检查构造函数中使用的API是否需要适配
     */
    MainWidget();

private slots:
    /**
     * @brief 函数类型切换槽函数
     * @param checked 是否选中
     * @details 当用户切换函数类型单选按钮时调用
     * 根据选中的函数类型更新图表数据
     * 支持正弦、余弦、线性等多种函数类型
     * TODO: Qt5.15.2升级 检查信号槽连接方式是否有变更
     */
    void functionToggled(bool checked);

    /**
     * @brief 添加数据点按钮点击槽函数
     * @details 当用户点击添加数据点按钮时调用
     * 向数据模型中添加新的数据点
     * 支持添加100、1000或10000个数据点
     * TODO: Qt5.15.2升级 检查按钮点击事件处理是否有变更
     */
    void addPointsButtonClicked();

private:
    // 控件容器，用于放置UI控件（如按钮、单选框等）
    QWidget *m_controlsContainer;
    // UI控件对象，通过ui_Controls.h生成，包含所有界面元素
    Ui::Controls m_controls;
    // 函数类型选择单选按钮向量，存储不同函数类型的单选按钮
    QVector<QRadioButton *> m_functionSelector;
    // 添加数据点按钮向量，存储不同添加数据点数量的按钮
    QVector<QPushButton *> m_addPointsButtons;

    // 绘图仪对象，用于绘制图表，展示数据集的可视化效果
    KDChart::Plotter *m_plotter;
    // 数据模型对象，存储图表数据，支持高效处理大型数据集
    Model m_model;
};

#endif
