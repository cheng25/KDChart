/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"
// 包含主窗口类头文件

#include <KDChartAbstractCoordinatePlane>
// 包含KDChart抽象坐标平面类头文件
#include <KDChartChart>
// 包含KDChart图表类头文件
#include <KDChartDataValueAttributes>
// 包含KDChart数据值属性类头文件
#include <KDChartGridAttributes>
// 包含KDChart网格属性类头文件
#include <KDChartLegend>
// 包含KDChart图例类头文件
#include <KDChartMarkerAttributes>
// 包含KDChart标记属性类头文件
#include <KDChartPolarDiagram>
// 包含KDChart极坐标图表类头文件
#include <KDChartTextAttributes>
// 包含KDChart文本属性类头文件

#include <QDebug>
// 包含Qt调试输出类头文件
#include <QPainter>
// 包含Qt绘图类头文件

using namespace KDChart;

/**
 * @brief 主窗口构造函数
 * @param parent 父窗口指针
 * @details 初始化UI，创建并配置图表组件，设置数据模型
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件

    // 实例化KD Chart相关类
    initKDChartClasses();

    // 将KDChart::Chart插入到Qt的布局中
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart->setGlobalLeadingLeft(5); // 设置图表左侧外边距为5像素
    m_chart->setGlobalLeadingRight(5); // 设置图表右侧外边距为5像素
    chartLayout->addWidget(m_chart); // 将图表添加到布局中

    // 连接KD Chart相关类
    wireUpKDChartClasses();

    // 初始化ItemModel并填充一些数据
    m_model.insertRows(0, 40); // 插入40行数据
    m_model.insertColumns(0, 5); // 插入5列数据
    setItemModelData(); // 设置数据模型内容
}

/**
 * @brief 初始化KDChart相关类
 * @details 创建图表、极坐标图表和极坐标坐标平面对象
 */
void MainWindow::initKDChartClasses()
{
    m_chart = new Chart(); // 创建图表对象
    m_diagram = new PolarDiagram(); // 创建极坐标图表对象
    m_polarPlane = new PolarCoordinatePlane(); // 创建极坐标坐标平面对象
}

/**
 * @brief 连接KDChart相关类
 * @details 将极坐标坐标平面设置到图表中，为图表设置数据模型，并将图表添加到坐标平面
 */
void MainWindow::wireUpKDChartClasses()
{
    m_chart->replaceCoordinatePlane(m_polarPlane); // 替换图表的坐标平面为极坐标平面
    // 注意：在将图表添加到坐标平面之前，我们需要为图表设置一个有效的数据模型
    m_diagram->setModel(&m_model); // 为极坐标图表设置数据模型
    m_chart->coordinatePlane()->replaceDiagram(m_diagram); // 将极坐标图表添加到坐标平面
}

/**
 * @brief 设置数据模型数据
 * @details 手动填充数据模型的单元格，而不是从资源文件读取数据
 */
void MainWindow::setItemModelData()
{
    // 这次我们不从资源文件读取数据，而是手动填充单元格
    int value = 0; // 初始值
    for (int column = 0; column < m_model.columnCount(); ++column) { // 遍历所有列
        for (int row = 0; row < m_model.rowCount(); ++row) { // 遍历所有行
            QModelIndex index = m_model.index(row, column); // 获取单元格索引
            m_model.setData(index, QVariant(value++)); // 设置单元格数据，并递增value
        }
    }
}

/**
 * @brief 处理起始位置微调框值变化
 * @param pos 新的起始位置值
 * @details 当用户更改起始位置微调框的值时，同步更新滑动条，并设置极坐标平面的起始位置
 */
void MainWindow::on_startPositionSB_valueChanged(double pos)
{
    const int intValue = static_cast<int>(pos); // 将double值转换为int
    startPositionSL->blockSignals(true); // 阻止滑动条发送信号，避免循环更新
    startPositionSL->setValue(intValue); // 更新滑动条值
    startPositionSL->blockSignals(false); // 重新启用滑动条信号
    // 注意：我们在这里使用全局getter方法，它会自动回退以返回默认设置
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos); // 设置极坐标平面的起始位置
    update(); // 更新窗口
}

/**
 * @brief 处理起始位置滑动条值变化
 * @param pos 新的起始位置值
 * @details 当用户更改起始位置滑动条的值时，同步更新微调框，并设置极坐标平面的起始位置
 */
void MainWindow::on_startPositionSL_valueChanged(int pos)
{
    auto qrealValue = static_cast<qreal>(pos); // 将int值转换为qreal
    startPositionSB->blockSignals(true); // 阻止微调框发送信号，避免循环更新
    startPositionSB->setValue(qrealValue); // 更新微调框值
    startPositionSB->blockSignals(false); // 重新启用微调框信号
    // 注意：我们在这里使用全局getter方法，它会自动回退以返回默认设置
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos); // 设置极坐标平面的起始位置
    update(); // 更新窗口
}

/**
 * @brief 处理圆形网格复选框状态变化
 * @param toggle 是否启用圆形网格
 * @details 当用户更改圆形网格复选框的状态时，更新极坐标平面的圆形网格显示
 */
void MainWindow::on_circularGridCB_toggled(bool toggle)
{
    GridAttributes attrs(m_polarPlane->gridAttributes(true)); // 获取圆形网格属性
    attrs.setGridVisible(toggle); // 设置网格可见性
    m_polarPlane->setGridAttributes(true, attrs); // 更新极坐标平面的圆形网格属性
    update(); // 更新窗口
}

/**
 * @brief 处理径向网格复选框状态变化
 * @param toggle 是否启用径向网格
 * @details 当用户更改径向网格复选框的状态时，更新极坐标平面的径向网格显示
 */
void MainWindow::on_sagittalGridCB_toggled(bool toggle)
{
    GridAttributes attrs(m_polarPlane->gridAttributes(false)); // 获取径向网格属性
    attrs.setGridVisible(toggle); // 设置网格可见性
    m_polarPlane->setGridAttributes(false, attrs); // 更新极坐标平面的径向网格属性
    update(); // 更新窗口
}
