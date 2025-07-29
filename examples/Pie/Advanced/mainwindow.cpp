/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的高级饼图示例的主窗口实现文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartPieAttributes>
#include <KDChartPieDiagram>
#include <KDChartThreeDPieAttributes>

#include <QDebug>
#include <QTimer>

using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口指针
 * 
 * 初始化主窗口，创建图表对象，加载数据模型，设置饼图参数。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart(); // 创建图表对象
    m_chart->setGlobalLeadingLeft(5); // 设置左侧全局边距
    m_chart->setGlobalLeadingRight(5); // 设置右侧全局边距
    chartLayout->addWidget(m_chart); // 将图表添加到布局

    m_model.loadFromCSV(":/data"); // 从CSV文件加载数据
    explodeDatasetSB->setMaximum(m_model.columnCount() - 1); // 设置爆炸数据集选择框的最大值

    // 设置图表
    auto *polarPlane = new PolarCoordinatePlane(m_chart); // 创建极坐标平面
    m_chart->replaceCoordinatePlane(polarPlane); // 替换图表的坐标平面
    m_pie = new PieDiagram(); // 创建饼图对象
    m_pie->setModel(&m_model); // 设置饼图的数据模型
    m_chart->coordinatePlane()->replaceDiagram(m_pie); // 替换坐标平面的图表

    m_timer = new QTimer(this); // 创建定时器
    // 连接定时器的超时信号到动画帧更新槽函数
    connect(m_timer, &QTimer::timeout, this, &MainWindow::slotNextFrame);
} // TODO: Qt5.15.2升级 检查QHBoxLayout在Qt5.15.2中的行为
// TODO: C++17升级 考虑使用std::make_unique管理动态内存

/**
 * @brief 响应起始位置数字输入框的变化
 * @param pos 新的起始位置值
 * 
 * 同步滑动条的值，并更新饼图的起始位置。
 */
void MainWindow::on_startPositionSB_valueChanged(double pos)
{
    const int intValue = static_cast<int>(pos);
    startPositionSL->blockSignals(true); // 阻止滑动条发出信号
    startPositionSL->setValue(intValue); // 设置滑动条的值
    startPositionSL->blockSignals(false); // 恢复滑动条的信号
    // 更新极坐标平面的起始位置
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos);
    m_chart->update(); // 更新图表
} // TODO: Qt5.15.2升级 验证PolarCoordinatePlane::setStartPosition在Qt5.15.2中的行为

/**
 * @brief 响应起始位置滑动条的变化
 * @param pos 新的起始位置值
 * 
 * 同步数字输入框的值，并更新饼图的起始位置。
 */
void MainWindow::on_startPositionSL_valueChanged(int pos)
{
    auto qrealValue = static_cast<qreal>(pos);
    startPositionSB->blockSignals(true); // 阻止数字输入框发出信号
    startPositionSB->setValue(qrealValue); // 设置数字输入框的值
    startPositionSB->blockSignals(false); // 恢复数字输入框的信号
    // 更新极坐标平面的起始位置
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos);
    m_chart->update(); // 更新图表
}

/**
 * @brief 响应爆炸效果提交按钮的点击
 * 
 * 应用当前设置的爆炸因子到指定的扇区。
 */
void MainWindow::on_explodeSubmitPB_clicked()
{
    // 设置指定扇区的爆炸因子
    setExplodeFactor(explodeDatasetSB->value(), explodeFactorSB->value());
    m_chart->update(); // 更新图表
}

/**
 * @brief 设置指定扇区的爆炸因子
 * @param column 扇区索引
 * @param value 爆炸因子值
 * 
 * 为指定的饼图扇区设置爆炸效果的强度。
 */
void MainWindow::setExplodeFactor(int column, qreal value)
{
    // 注意：
    // 这里使用按列获取属性的方法，它会自动回退到全局设置（甚至默认设置）
    PieAttributes attrs(m_pie->pieAttributes(column)); // 获取当前扇区的属性
    attrs.setExplodeFactor(value); // 设置爆炸因子
    m_pie->setPieAttributes(column, attrs); // 应用属性到扇区
    m_chart->update(); // 更新图表
} // TODO: Qt5.15.2升级 检查PieAttributes在Qt5.15.2中的API变化

/**
 * @brief 响应爆炸动画复选框的切换
 * @param toggle 是否启用动画
 * 
 * 根据复选框状态启动或停止爆炸动画。
 */
void MainWindow::on_animateExplosionCB_toggled(bool toggle)
{
    if (toggle)
        m_timer->start(100); // 启动定时器，每100毫秒触发一次
    else
        m_timer->stop(); // 停止定时器
} // TODO: Qt5.15.2升级 验证QTimer在Qt5.15.2中的行为

/**
 * @brief 动画下一帧的更新
 * 
 * 更新爆炸动画的状态，实现扇区的爆炸效果动画。
 */
void MainWindow::slotNextFrame()
{
    m_currentFactor += (1 * m_currentDirection); // 更新当前爆炸因子
    // 检查是否到达边界，反转动画方向
    if (m_currentFactor == 0 || m_currentFactor == 5)
        m_currentDirection = -m_currentDirection;

    if (m_currentFactor == 0) {
        setExplodeFactor(m_currentSlice, 0.0); // 重置当前扇区的爆炸因子
        m_currentSlice++; // 移动到下一个扇区
        // 检查是否到达最后一个扇区，重置到第一个
        if (m_currentSlice >= m_model.columnCount())
            m_currentSlice = 0;
    }

    // 设置当前扇区的爆炸因子
    setExplodeFactor(
        m_currentSlice,
        static_cast<qreal>(m_currentFactor) / 10.0);
    m_chart->update(); // 更新图表
} // TODO: C++17升级 考虑使用结构化绑定优化变量

/**
 * @brief 响应3D效果复选框的切换
 * @param toggle 是否启用3D效果
 * 
 * 根据复选框状态启用或禁用3D效果，并设置深度。
 */
void MainWindow::on_threeDGB_toggled(bool toggle)
{
    // 注意：
    // 这里使用全局获取属性的方法，它会自动回退到默认设置
    ThreeDPieAttributes attrs(m_pie->threeDPieAttributes()); // 获取当前3D属性
    attrs.setEnabled(toggle); // 设置3D效果启用状态
    attrs.setDepth(threeDFactorSB->value()); // 设置3D深度
    m_pie->setThreeDPieAttributes(attrs); // 应用3D属性
    m_chart->update(); // 更新图表
} // TODO: Qt5.15.2升级 检查ThreeDPieAttributes在Qt5.15.2中的API变化

/**
 * @brief 响应3D深度因子的变化
 * @param factor 新的深度因子值
 * 
 * 更新3D效果的深度。
 */
void MainWindow::on_threeDFactorSB_valueChanged(int factor)
{
    // 注意：
    // 这里使用全局获取属性的方法，它会自动回退到默认设置
    ThreeDPieAttributes attrs(m_pie->threeDPieAttributes()); // 获取当前3D属性
    attrs.setEnabled(threeDGB->isChecked()); // 设置3D效果启用状态
    attrs.setDepth(factor); // 设置3D深度
    m_pie->setThreeDPieAttributes(attrs); // 应用3D属性
    m_chart->update(); // 更新图表
}
