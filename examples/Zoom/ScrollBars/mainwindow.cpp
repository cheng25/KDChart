/****************************************************************************
**
** 此文件是KD Chart图表库的一部分。
** 本文件实现了带滚动条缩放功能的示例主窗口类。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001年KDAB集团公司(Klarälvdalens Datakonsult AB)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT开源许可
**
****************************************************************************/

#include "mainwindow.h"            // 包含主窗口类头文件

#include <KDChartAbstractCoordinatePlane> // 包含抽象坐标平面类
#include <KDChartChart>                // 包含图表类
#include <KDChartLegend>               // 包含图例类
#include <KDChartLineDiagram>          // 包含折线图类

#include <QDebug>                      // 包含Qt调试输出功能
#include <QPainter>                    // 包含Qt绘图功能

using namespace KDChart; // 使用KDChart命名空间

/**
 * @brief 构造函数
 * @param parent 父窗口指针
 * @details 初始化UI，创建图表布局，设置数据模型和图表属性。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart(); // 创建图表对象
    m_chart->setGlobalLeadingTop(10);  // 设置顶部全局边距
    m_chart->setGlobalLeadingRight(10); // 设置右侧全局边距
    chartLayout->addWidget(m_chart);   // 将图表添加到布局
    hSBar->setVisible(false);          // 隐藏水平滚动条
    vSBar->setVisible(false);          // 隐藏垂直滚动条

    m_model.loadFromCSV(":/data"); // 从CSV文件加载数据

    // 设置图表
    m_lines = new LineDiagram();  // 创建折线图对象
    m_lines->setModel(&m_model);  // 设置图表数据模型

    // 创建坐标轴
    auto *xAxis = new CartesianAxis(m_lines);  // 创建X轴
    auto *yAxis = new CartesianAxis(m_lines);  // 创建Y轴
    xAxis->setPosition(KDChart::CartesianAxis::Bottom); // 设置X轴位置在底部
    yAxis->setPosition(KDChart::CartesianAxis::Left);   // 设置Y轴位置在左侧

    xAxis->setTitleText("Abscissa axis at the bottom"); // 设置X轴标题
    yAxis->setTitleText("Ordinate axis at the left side"); // 设置Y轴标题

    m_lines->addAxis(xAxis);  // 将X轴添加到图表
    m_lines->addAxis(yAxis);  // 将Y轴添加到图表
    m_chart->coordinatePlane()->replaceDiagram(m_lines); // 替换图表的坐标平面

    // 连接图表属性变化信号到应用缩放参数槽函数
    connect(m_chart, &Chart::propertiesChanged, this, &MainWindow::applyNewZoomParameters);

    // 设置图例
    m_legend = new Legend(m_lines, m_chart);  // 创建图例对象
    m_chart->addLegend(m_legend);             // 将图例添加到图表
    m_legend->setPosition(KDChartEnums::PositionEast); // 设置图例位置在东侧
    m_legend->setAlignment(Qt::AlignTop);     // 设置图例对齐方式为顶部对齐
}

/**
 * @brief 缩放因子滑动条值变化槽函数
 * @param factor 新的缩放因子
 * @details 根据缩放因子显示或隐藏滚动条，并设置图表的缩放因子。
 */
void MainWindow::on_zoomFactorSB_valueChanged(double factor)
{
    if (factor > 1) {
        hSBar->setVisible(true);  // 显示水平滚动条
        vSBar->setVisible(true);  // 显示垂直滚动条
    } else {
        hSBar->setValue(500);     // 重置水平滚动条值
        vSBar->setValue(500);     // 重置垂直滚动条值
        hSBar->setVisible(false); // 隐藏水平滚动条
        vSBar->setVisible(false); // 隐藏垂直滚动条
    }
    m_chart->coordinatePlane()->setZoomFactorX(factor); // 设置X轴缩放因子
    m_chart->coordinatePlane()->setZoomFactorY(factor); // 设置Y轴缩放因子
    m_chart->update(); // 更新图表

/**
 * @brief 调整网格复选框状态变化槽函数
 * @param checked 是否选中
 * @details 设置是否根据缩放自动调整网格。
 */
void MainWindow::on_adjustGridCB_toggled(bool checked)
{
    static_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane())
        ->setAutoAdjustGridToZoom(checked); // 设置自动调整网格
    m_chart->update(); // 更新图表
}

/**
 * @brief 橡皮筋缩放复选框状态变化槽函数
 * @param checked 是否选中
 * @details 设置是否启用橡皮筋缩放功能。
 */
void MainWindow::on_rubberBandZoomCB_toggled(bool checked)
{
    m_chart->coordinatePlane()->setRubberBandZoomingEnabled(checked); // 设置橡皮筋缩放
}

/**
 * @brief 水平滚动条值变化槽函数
 * @param hPos 新的滚动条值
 * @details 根据水平滚动条值更新缩放中心。
 */
void MainWindow::on_hSBar_valueChanged(int hPos)
{
    // 设置缩放中心
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hPos / 1000.0, vSBar->value() / 1000.0));
    m_chart->update(); // 更新图表
}

/**
 * @brief 垂直滚动条值变化槽函数
 * @param vPos 新的滚动条值
 * @details 根据垂直滚动条值更新缩放中心。
 */
void MainWindow::on_vSBar_valueChanged(int vPos)
{
    // 设置缩放中心
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hSBar->value() / 1000.0, vPos / 1000.0));
    m_chart->update(); // 更新图表
}

/**
 * @brief 应用新的缩放参数
 * @details 更新滚动条和缩放因子滑动条的值，以反映图表当前的缩放状态。
 */
void MainWindow::applyNewZoomParameters()
{
    hSBar->blockSignals(true);  // 阻塞水平滚动条信号
    vSBar->blockSignals(true);  // 阻塞垂直滚动条信号

    // 更新水平滚动条值
    hSBar->setValue(qRound(m_chart->coordinatePlane()->zoomCenter().x() * 1000));
    // 更新垂直滚动条值
    vSBar->setValue(qRound(m_chart->coordinatePlane()->zoomCenter().y() * 1000));
    // 更新缩放因子滑动条值
    zoomFactorSB->setValue(m_chart->coordinatePlane()->zoomFactorX());

    hSBar->blockSignals(false); // 解除水平滚动条信号阻塞
    vSBar->blockSignals(false); // 解除垂直滚动条信号阻塞

// TODO: Qt5.15.2升级 检查QWidget信号槽连接方式在新版本中的变化
// TODO: Qt5.15.2升级 验证KDChart::Chart与Qt5.15.2的兼容性
// TODO: C++17升级 使用std::optional处理可能为空的指针
// TODO: C++17升级 利用结构化绑定优化坐标计算
