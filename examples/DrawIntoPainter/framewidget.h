/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** This file is part of the KD Chart library.
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** 许可协议: MIT
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>  // 包含QWidget类定义

namespace KDChart {
class Chart;  // 前向声明KDChart::Chart类
}

/**
 * @brief FrameWidget 框架窗口部件类
 * @details 用于将KD Chart图表绘制到自定义窗口部件中的容器类，提供了额外的绘制功能
 */
class FrameWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件指针，默认为nullptr
     * @param f 窗口标志，默认为空
     */
    explicit FrameWidget(QWidget *parent = nullptr, Qt::WindowFlags f = {});

    /**
     * @brief 重写的绘制事件处理函数
     * @param event 绘制事件对象
     * @details 负责在窗口部件上绘制图表
     */
    void paintEvent(QPaintEvent *) override;

    /**
     * @brief 设置要绘制的图表
     * @param chart KDChart::Chart指针
     * @details 将图表对象与窗口部件关联
     */
    void setChart(KDChart::Chart *chart);

private:
    KDChart::Chart *mChart = nullptr;  // 指向要绘制的图表对象
};

#endif /* FRAMEWIDGET_H */
