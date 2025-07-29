/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <TableModel.h>

namespace KDChart {
class Chart;
class BarDiagram;
class LineDiagram;
}

/**
 * @brief 主窗口类
 * 
 * 继承自QWidget，用于展示无数据值的图表示例。
 * 该类演示了如何在KDChart中处理和显示没有数据值的图表。
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 
     * 创建主窗口实例，初始化图表和数据模型。
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * 
     * 清理主窗口资源。
     */
    ~MainWindow() override
    {
    }

private:
    KDChart::Chart *m_chart; // 图表对象
    KDChart::LineDiagram *m_bars; // 折线图对象
    QAbstractItemModel *m_model; // 数据模型对象
}; // TODO: Qt5.15.2升级 检查MainWindow类在Qt5.15.2中的兼容性
// TODO: C++17升级 考虑使用智能指针管理成员变量

#endif /* MAINWINDOW_H */
