/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，实现高级柱状图的交互和配置功能
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChartAbstractCoordinatePlane>
#include <KDChartBackgroundAttributes>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartDatasetProxyModel>
#include <KDChartTextAttributes>
#include <KDChartThreeDBarAttributes>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

/**
 * @brief MainWindow类构造函数
 * @param parent 父窗口部件，默认为nullptr
 * 初始化UI界面，创建图表对象，加载数据模型，配置柱状图的基本属性
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI界面，由Qt Designer生成的函数

    // 创建图表布局并添加图表控件
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart();
    chartLayout->addWidget(m_chart);

    // 从CSV文件加载数据
    m_model.loadFromCSV(":/data");

    // 设置柱状图
    m_bars = new BarDiagram();
    m_bars->setModel(&m_model); // 关联数据模型

    // 定义柱状图边框的自定义颜色
    QPen pen(m_bars->pen());
    pen.setColor(Qt::black);
    pen.setWidth(0);
    m_bars->setPen(pen);
    m_chart->coordinatePlane()->replaceDiagram(m_bars); // 将柱状图添加到图表

    // 定义柱状图的自定义颜色
    QList<QColor> bcolor;
    bcolor.append(Qt::darkGreen);
    bcolor.append(Qt::green);
    bcolor.append(Qt::darkRed);
    bcolor.append(Qt::red);
    for (int row = 0; row < m_model.columnCount(); ++row) {
        m_bars->setBrush(row, QBrush(bcolor[row]));
    }

    // 配置坐标平面的背景
    BackgroundAttributes pba;
    // pba.setBrush( QBrush(QColor(0x20,0x20,0x60)) );
    pba.setVisible(true);
    m_chart->coordinatePlane()->setBackgroundAttributes(pba);

    m_chart->setGlobalLeadingTop(20); // 设置图表顶部的全局边距
}

/**
 * @brief 柱状图类型下拉框选择变化槽函数
 * @param index 选中的索引
 * 根据选择的索引设置柱状图的类型（普通、堆叠、百分比）
 */
void MainWindow::on_barTypeCB_currentIndexChanged(int index)
{
    const QString text = barTypeCB->itemText(index);
    if (text == "Normal")
        m_bars->setType(BarDiagram::Normal); // 普通柱状图
    else if (text == "Stacked")
        m_bars->setType(BarDiagram::Stacked); // 堆叠柱状图
    else if (text == "Percent")
        m_bars->setType(BarDiagram::Percent); // 百分比柱状图
    else
        qWarning(" Does not match any type");

    m_chart->update(); // 更新图表
}

/**
 * @brief 柱状图方向下拉框选择变化槽函数
 * @param index 选中的索引
 * 根据选择的索引设置柱状图的方向（垂直或水平）
 */
void MainWindow::on_barOrientationCB_currentIndexChanged(int index)
{
    const QString text = barOrientationCB->itemText(index);
    if (text == "Vertical")
        m_bars->setOrientation(Qt::Vertical); // 垂直方向
    else if (text == "Horizontal")
        m_bars->setOrientation(Qt::Horizontal); // 水平方向
    else
        qWarning(" Does not match any orientation");

    m_chart->update(); // 更新图表
}

/**
 * @brief 显示数值复选框状态变化槽函数
 * @param checked 是否选中
 * 设置是否在柱状图上显示数值，并配置数值的字体和颜色
 */
void MainWindow::on_paintValuesCB_toggled(bool checked)
{
    Q_UNUSED(checked);
    // 在这里我们按列设置数据值属性，因为我们希望文本以不同的颜色显示
    // 颜色与各自数据集的颜色一致
    const QFont font(QFont("Comic", 10));
    const int colCount = m_bars->model()->columnCount();
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {
        QBrush brush(m_bars->brush(iColumn));
        DataValueAttributes a(m_bars->dataValueAttributes(iColumn));
        TextAttributes ta(a.textAttributes());
        ta.setRotation(0);
        ta.setFont(font);
        ta.setPen(QPen(brush.color()));
        if (checked)
            ta.setVisible(true);
        else
            ta.setVisible(false);

        a.setTextAttributes(ta);
        a.setVisible(true);
        m_bars->setDataValueAttributes(iColumn, a);
    }

    m_chart->update(); // 更新图表
}

/**
 * @brief 3D效果复选框状态变化槽函数
 * @param checked 是否选中
 * 设置柱状图是否使用3D效果，并配置3D深度
 */
void MainWindow::on_paintThreeDBarsCB_toggled(bool checked)
{
    ThreeDBarAttributes td(m_bars->threeDBarAttributes());
    qreal defaultDepth = td.depth();
    if (checked) {
        td.setEnabled(true);
        if (threeDDepthCB->isChecked())
            td.setDepth(depthSB->value());
        else
            td.setDepth(defaultDepth);
    } else {
        td.setEnabled(false);
    }
    m_bars->setThreeDBarAttributes(td);
    m_chart->update(); // 更新图表
}

/**
 * @brief 标记列复选框状态变化槽函数
 * @param checked 是否选中
 * 设置是否标记指定的列，并配置标记的样式
 */
void MainWindow::on_markColumnCB_toggled(bool checked)
{
    const int column = markColumnSB->value();
    QPen pen(m_bars->pen(column));
    if (checked) {
        pen.setColor(Qt::yellow);
        pen.setStyle(Qt::DashLine);
        pen.setWidth(3);
    } else {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
    }
    m_bars->setPen(column, pen);
    m_chart->update(); // 更新图表
}

/**
 * @brief 深度数值变化槽函数
 * @param i 深度值
 * 当3D深度复选框和3D效果复选框都选中时，更新3D柱状图的深度
 */
void MainWindow::on_depthSB_valueChanged(int i)
{
    Q_UNUSED(i);

    if (threeDDepthCB->isChecked() && paintThreeDBarsCB->isChecked())
        on_paintThreeDBarsCB_toggled(true);
}

/**
 * @brief 3D深度复选框状态变化槽函数
 * @param checked 是否选中
 * 当3D效果复选框选中时，更新3D柱状图的深度设置
 */
void MainWindow::on_threeDDepthCB_toggled(bool checked)
{
    Q_UNUSED(checked);

    if (paintThreeDBarsCB->isChecked())
        on_paintThreeDBarsCB_toggled(true);
}

/**
 * @brief 标记列数值变化槽函数
 * @param i 列索引值
 * 当列索引变化时，更新标记列复选框的状态
 */
void MainWindow::on_markColumnSB_valueChanged(int i)
{
    QPen pen(m_bars->pen(i));
    markColumnCB->setChecked(pen.color() == Qt::yellow);
}

/**
 * @brief 宽度数值变化槽函数
 * @param value 宽度值
 * 当宽度复选框选中时，设置柱状图的固定宽度
 */
void MainWindow::on_widthSB_valueChanged(int value)
{
    if (widthCB->isChecked()) {
        BarAttributes ba(m_bars->barAttributes());
        ba.setFixedBarWidth(value);
        ba.setUseFixedBarWidth(true);
        m_bars->setBarAttributes(ba);
    }
    m_chart->update(); // 更新图表
}

/**
 * @brief 宽度复选框状态变化槽函数
 * @param checked 是否选中
 * 设置是否使用固定的柱状图宽度
 */
void MainWindow::on_widthCB_toggled(bool checked)
{
    if (checked) {
        on_widthSB_valueChanged(widthSB->value());
    } else {
        BarAttributes ba(m_bars->barAttributes());
        ba.setUseFixedBarWidth(false);
        m_bars->setBarAttributes(ba);
        m_chart->update(); // 更新图表
    }
}

/**
 * @brief 固定平面大小复选框状态变化槽函数
 * @param checked 是否选中
 * 设置是否固定坐标平面的大小，并在取消选中时重置缩放设置
 */
void MainWindow::on_fixPlaneSizeCB_toggled(bool checked)
{
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane());
    if (plane == nullptr)
        return;
    plane->setFixedDataCoordinateSpaceRelation(checked);
    // 小调整：
    // 当取消选中关系调整复选框时，将缩放设置重置为初始值
    if (!checked) {
        m_chart->coordinatePlane()->setZoomFactorX(1.0);
        m_chart->coordinatePlane()->setZoomFactorY(1.0);
        m_chart->coordinatePlane()->setZoomCenter(QPointF(0.5, 0.5));
    }
    m_chart->update(); // 更新图表
}
