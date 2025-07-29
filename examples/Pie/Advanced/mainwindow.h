/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的高级饼图示例的主窗口头文件。
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

#include "ui_mainwindow.h"
#include <TableModel.h>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE
namespace KDChart {
class Chart;
class PieDiagram;
}

/**
 * @brief 高级饼图示例的主窗口类
 * 
 * 该类实现了一个具有高级功能的饼图演示窗口，包括：
 * - 可调节的起始位置
 * - 扇区爆炸效果及动画
 * - 3D视图切换和深度调节
 * - 交互式控制界面
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
    // 起始位置控制
    void on_startPositionSB_valueChanged(double pos); /**< 数字输入框改变起始位置 */
    void on_startPositionSL_valueChanged(int pos);    /**< 滑动条改变起始位置 */

    // 爆炸效果控制
    void on_explodeSubmitPB_clicked();                /**< 提交爆炸因子按钮点击事件 */
    void on_animateExplosionCB_toggled(bool toggle);  /**< 爆炸动画复选框切换事件 */
    void setExplodeFactor(int column, qreal value);   /**< 设置指定扇区的爆炸因子 */

    // 动画控制
    void slotNextFrame();                             /**< 动画下一帧 */

    // 3D效果控制
    void on_threeDGB_toggled(bool toggle);            /**< 3D效果复选框切换事件 */
    void on_threeDFactorSB_valueChanged(int factor);  /**< 3D深度因子改变事件 */

private:
    KDChart::Chart *m_chart;              /**< 图表对象指针 */
    TableModel m_model;                   /**< 数据模型对象 */
    KDChart::PieDiagram *m_pie;           /**< 饼图对象指针 */
    QTimer *m_timer;                      /**< 动画定时器指针 */

    int m_currentFactor = 0;              /**< 当前爆炸因子 */
    int m_currentDirection = 1;           /**< 当前动画方向 (1:增加, -1:减少) */
    int m_currentSlice = 0;               /**< 当前动画扇区索引 */
}; // TODO: Qt5.15.2升级 检查MainWindow类在Qt5.15.2中的兼容性
// TODO: C++17升级 考虑使用智能指针管理成员变量

#endif /* MAINWINDOW_H */
