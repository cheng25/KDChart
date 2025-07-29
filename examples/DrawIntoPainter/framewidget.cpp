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

#include "framewidget.h"  // 包含FrameWidget类定义

#include <KDChartChart>  // 包含KDChart::Chart类定义

#include <QDebug>  // 包含调试输出类
#include <QPainter>  // 包含绘图类

/**
 * @brief FrameWidget::FrameWidget 构造函数
 * @param parent 父窗口部件指针
 * @param f 窗口标志
 * @details 初始化FrameWidget对象
 */
FrameWidget::FrameWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    // 此构造函数故意留空
}

/**
 * @brief FrameWidget::setChart 设置要绘制的图表
 * @param chart KDChart::Chart指针
 * @details 将图表对象与窗口部件关联，并建立属性更改信号的连接
 */
void FrameWidget::setChart(KDChart::Chart *chart)
{
    mChart = chart;  // 保存图表指针
    // 建立信号槽连接，当图表属性更改时更新窗口部件
    // 这是必要的，因为图表无法自动调度其他对象（此对象）调用其自定义绘制方法
    connect(mChart, &KDChart::Chart::propertiesChanged, this, QOverload<>::of(&FrameWidget::update));
}

/**
 * @brief FrameWidget::paintEvent 重写的绘制事件处理函数
 * @param e 绘制事件对象
 * @details 负责在窗口部件上绘制图表和额外的装饰元素
 */
void FrameWidget::paintEvent(QPaintEvent *e)
{
    if (!mChart) {
        // 如果没有图表，调用基类的绘制事件处理函数
        QWidget::paintEvent(e);
    } else {
        QPainter painter(this);  // 创建画家对象

        const int wid = 64;  // 装饰椭圆的宽度和高度
        const QRect r(rect());  // 获取窗口部件的客户区域
        const QPen oldPen(painter.pen());  // 保存原始画笔
        const QBrush oldBrush(painter.brush());  // 保存原始画刷

        // 绘制图表下方的装饰元素
        painter.setPen(QPen(Qt::NoPen));  // 不使用边框
        painter.setBrush(QBrush(QColor(0xd0, 0xd0, 0xff)));  // 设置蓝色画刷
        // 在四个角落绘制蓝色椭圆
        painter.drawEllipse(r.left(), r.top(), wid, wid);
        painter.drawEllipse(r.left() + r.width() - wid - 1, r.top(), wid, wid);
        painter.drawEllipse(r.left(), r.top() + r.height() - wid - 1, wid, wid);
        painter.drawEllipse(r.left() + r.width() - wid - 1, r.top() + r.height() - wid - 1, wid, wid);

        // 恢复原始画笔和画刷
        painter.setBrush(oldBrush);
        painter.setPen(oldPen);

        // 绘制图表，调整位置使其位于装饰元素内部
        mChart->paint(&painter, QRect(r.left() + wid / 2, r.top() + wid / 2, r.width() - wid, r.height() - wid));

        // 绘制图表上方的装饰元素
        painter.setPen(QPen(Qt::NoPen));  // 不使用边框
        painter.setBrush(QBrush(QColor(0xd0, 0xff, 0xff)));  // 设置青色画刷
        const int wid2 = 40;  // 内部装饰椭圆的宽度和高度
        const int gap = (wid - wid2) / 2;  // 计算边距
        // 在四个角落绘制青色椭圆
        painter.drawEllipse(r.left() + gap, r.top() + gap, wid2, wid2);
        painter.drawEllipse(r.left() + r.width() - wid + gap - 1, r.top() + gap, wid2, wid2);
        painter.drawEllipse(r.left() + gap, r.top() + r.height() - wid + gap - 1, wid2, wid2);
        painter.drawEllipse(r.left() + r.width() - wid + gap - 1, r.top() + r.height() - wid + gap - 1, wid2, wid2);

        // 恢复原始画笔和画刷
        painter.setBrush(oldBrush);
        painter.setPen(oldPen);
    }
}
