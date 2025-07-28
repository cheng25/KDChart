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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"  // 包含UI设计文件
#include <TableModel.h>  // 包含表格模型类

#include <QPixmap>  // 包含像素图类

namespace KDChart {
class Chart;  // 前向声明KDChart::Chart类
class DatasetProxyModel;  // 前向声明KDChart::DatasetProxyModel类
class LineDiagram;  // 前向声明KDChart::LineDiagram类
class Legend;  // 前向声明KDChart::Legend类
}

QT_BEGIN_NAMESPACE
class QLabel;  // 前向声明QLabel类
QT_END_NAMESPACE

/**
 * @brief MainWindow 主窗口类
 * @details 该类是应用程序的主窗口，负责创建和管理图表、处理用户交互以及显示图表的不同视图
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow 构造函数
     * @param parent 父窗口部件指针
     * @details 初始化主窗口，创建图表和数据模型
     */
    MainWindow(QWidget *parent = nullptr);

protected:
    /**
     * @brief resizeEvent 重写的窗口大小调整事件处理函数
     * @param event 大小调整事件对象
     * @details 当窗口大小改变时，调整两个小图表的大小
     */
    /* reimp */ void resizeEvent(QResizeEvent *) override;

private slots:
    /**
     * @brief on_lineTypeCB_currentIndexChanged 线型组合框当前索引变化槽函数
     * @param index 选中的索引
     * @details 处理线型选择的变化，更新图表中的线条样式
     */
    void on_lineTypeCB_currentIndexChanged(int index);

    /**
     * @brief on_paintLegendCB_toggled 图例绘制复选框状态变化槽函数
     * @param checked 是否选中
     * @details 切换图例的显示/隐藏状态
     */
    void on_paintLegendCB_toggled(bool checked);

    /**
     * @brief on_paintValuesCB_toggled 数据值绘制复选框状态变化槽函数
     * @param checked 是否选中
     * @details 切换数据值的显示/隐藏状态
     */
    void on_paintValuesCB_toggled(bool checked);

    /**
     * @brief on_paintMarkersCB_toggled 标记绘制复选框状态变化槽函数
     * @param checked 是否选中
     * @details 切换数据点标记的显示/隐藏状态
     */
    void on_paintMarkersCB_toggled(bool checked);

    /**
     * @brief on_markersStyleCB_currentIndexChanged 标记样式组合框当前索引变化槽函数
     * @param index 选中的索引
     * @details 处理标记样式选择的变化，更新数据点标记的样式
     */
    void on_markersStyleCB_currentIndexChanged(int index);

    /**
     * @brief on_markersWidthSB_valueChanged 标记宽度微调框值变化槽函数
     * @param i 宽度值
     * @details 处理标记宽度的变化，更新数据点标记的宽度
     */
    void on_markersWidthSB_valueChanged(int i);

    /**
     * @brief on_markersHeightSB_valueChanged 标记高度微调框值变化槽函数
     * @param i 高度值
     * @details 处理标记高度的变化，更新数据点标记的高度
     */
    void on_markersHeightSB_valueChanged(int i);

    /**
     * @brief on_displayAreasCB_toggled 区域显示复选框状态变化槽函数
     * @param checked 是否选中
     * @details 切换区域填充的显示/隐藏状态
     */
    void on_displayAreasCB_toggled(bool checked);

    /**
     * @brief on_transparencySB_valueChanged 透明度微调框值变化槽函数
     * @param i 透明度值
     * @details 处理透明度的变化，更新图表元素的透明度
     */
    void on_transparencySB_valueChanged(int i);

    /**
     * @brief on_zoomFactorSB_valueChanged 缩放因子微调框值变化槽函数
     * @param factor 缩放因子
     * @details 处理缩放因子的变化，更新图表的缩放级别
     */
    void on_zoomFactorSB_valueChanged(double factor);

    /**
     * @brief on_hSBar_valueChanged 水平滚动条值变化槽函数
     * @param value 滚动条值
     * @details 处理水平滚动条的变化，调整图表的水平位置
     */
    void on_hSBar_valueChanged(int value);

    /**
     * @brief on_vSBar_valueChanged 垂直滚动条值变化槽函数
     * @param value 滚动条值
     * @details 处理垂直滚动条的变化，调整图表的垂直位置
     */
    void on_vSBar_valueChanged(int value);

    /**
     * @brief on_savePB_clicked 保存按钮点击槽函数
     * @details 处理保存按钮的点击事件，将图表保存为图片
     */
    void on_savePB_clicked();

    /**
     * @brief on_savePDF_clicked 保存PDF按钮点击槽函数
     * @details 处理保存PDF按钮的点击事件，将图表保存为PDF文件
     */
    void on_savePDF_clicked();

private:
    TableModel m_model;  // 表格数据模型
    KDChart::DatasetProxyModel *m_datasetProxy;  // 数据集代理模型
    KDChart::Legend *m_legend;  // 图例对象

    /**
     * @brief paintMarkers 绘制数据点标记
     * @param checked 是否绘制标记
     * @param printSize 打印大小
     * @details 根据参数设置绘制或隐藏数据点标记，并设置标记的大小
     */
    void paintMarkers(bool checked, const QSize &printSize);

protected:
    KDChart::Chart *m_chart;  // 图表对象
    KDChart::LineDiagram *m_lines;  // 线图对象
    QPixmap m_pix1, m_pix2;  // 用于显示小图表的像素图
    QLabel *m_smallChart1;  // 第一个小图表的标签
    QLabel *m_smallChart2;  // 第二个小图表的标签
};

#endif /* MAINWINDOW_H */
