/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的高级部件示例的主窗口头文件，定义了MainWindow类的接口
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

#include "ui_mainwindow.h"  // 包含UI设计文件生成的头文件

namespace KDChart {
class Widget;  // 前向声明KDChart::Widget类
}

/**
 * @brief 主窗口类
 * @details 继承自QWidget和Ui::MainWindow接口，是高级部件示例的核心容器类
 *          负责管理KDChart::Widget实例，并提供交互功能如更改图表类型、
 *          调整图表间距和添加数据集等
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @details 完成以下关键初始化工作：
     *          1. 调用UI初始化函数setupUi
     *          2. 创建KDChart::Widget图表部件实例
     *          3. 初始化数据集并添加到图表
     *          4. 连接信号与槽函数
     */
    MainWindow(QWidget *parent = nullptr);

    KDChart::Widget *widget;  // 图表部件实例指针，用于管理和操作图表

public slots:
    /**
     * @brief 更改图表类型的槽函数
     * @details 响应UI控件的信号，切换图表的显示类型（如折线图、柱状图等）
     */
    void changeType();

    /**
     * @brief 更改图表间距的槽函数
     * @param leading 新的间距值
     * @details 调整图表元素之间的间距，影响图表的整体布局
     */
    void changeLeading(int leading);

    /**
     * @brief 添加数据集的槽函数
     * @details 动态向图表中添加新的数据集，并更新图表显示
     */
    void addDataset();

private:
    int datasetCount = 3;  // 数据集计数器，初始值为3
};

// TODO: Qt5.15.2升级 验证KDChart::Widget在Qt5.15.2中的API兼容性
// TODO: Qt5.15.2升级 检查QWidget::setLayout()在高DPI环境下的布局行为
// TODO: C++17升级 使用std::unique_ptr管理widget动态对象
// TODO: C++17升级 考虑将datasetCount改为constexpr（若初始值固定）

#endif
