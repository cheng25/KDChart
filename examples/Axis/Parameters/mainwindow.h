/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是坐标轴参数配置示例的主窗口头文件，定义了MainWindow类的接口
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

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
class LineAttributes;
class CartesianAxis;
class Legend;
}

/**
 * @brief 主窗口类
 * 继承自QWidget和Ui::MainWindow，负责创建和管理图表界面
 * 核心功能：配置坐标轴参数，处理用户交互，动态更新图表样式
 * 
 * Qt5.15.2升级说明：
 * - 检查QWidget及其派生类在Qt5.15.2中的行为变更
 * - 验证UI文件中使用的控件是否与Qt5.15.2兼容
 * 
 * C++17升级说明：
 * - 可使用std::unique_ptr管理动态创建的图表对象
 * - 考虑使用结构化绑定优化多返回值场景
 */
// TODO: Qt5.15.2升级 检查QWidget::setLayout在Qt5.15.2中的高DPI布局行为
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @return 无返回值
     * @details 初始化UI组件，创建图表对象，配置坐标轴属性，加载数据模型
     * 并设置信号槽连接，实现图表参数的动态调整
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 线条类型下拉框索引变化的槽函数
     * @param index 当前选中的索引
     * @return 无返回值
     * @details 根据选中的索引更新图表中线条的显示样式
     */
    void on_lineTypeCB_currentIndexChanged(int index);
    /**
     * @brief 绘制图例复选框状态变化的槽函数
     * @param checked 是否选中
     * @return 无返回值
     * @details 控制图例的显示或隐藏
     */
    void on_paintLegendCB_toggled(bool checked);
    /**
     * @brief 绘制数值复选框状态变化的槽函数
     * @param checked 是否选中
     * @return 无返回值
     * @details 控制图表中数据值的显示或隐藏
     */
    void on_paintValuesCB_toggled(bool checked);
    /**
     * @brief 绘制标记复选框状态变化的槽函数
     * @param checked 是否选中
     * @return 无返回值
     * @details 控制图表中数据点标记的显示或隐藏
     */
    void on_paintMarkersCB_toggled(bool checked);
    /**
     * @brief 标记样式下拉框索引变化的槽函数
     * @param index 当前选中的索引
     * @return 无返回值
     * @details 更新图表中数据点标记的显示样式
     */
    void on_markersStyleCB_currentIndexChanged(int index);
    /**
     * @brief 标记宽度微调框值变化的槽函数
     * @param i 当前值
     * @return 无返回值
     * @details 更新图表中数据点标记的宽度
     */
    void on_markersWidthSB_valueChanged(int i);
    /**
     * @brief 标记高度微调框值变化的槽函数
     * @param i 当前值
     * @return 无返回值
     * @details 更新图表中数据点标记的高度
     */
    void on_markersHeightSB_valueChanged(int i);
    /**
     * @brief 显示区域复选框状态变化的槽函数
     * @param checked 是否选中
     * @return 无返回值
     * @details 控制图表中区域填充的显示或隐藏
     */
    void on_displayAreasCB_toggled(bool checked);
    /**
     * @brief 透明度微调框值变化的槽函数
     * @param i 当前值
     * @return 无返回值
     * @details 更新图表中区域填充的透明度
     */
    void on_transparencySB_valueChanged(int i);
    /**
     * @brief 缩放因子微调框值变化的槽函数
     * @param factor 当前缩放因子
     * @return 无返回值
     * @details 更新图表的缩放比例
     */
    void on_zoomFactorSB_valueChanged(double factor);
    /**
     * @brief 水平滚动条值变化的槽函数
     * @param value 当前值
     * @return 无返回值
     * @details 水平平移图表的显示区域
     */
    void on_hSBar_valueChanged(int value);
    /**
     * @brief 垂直滚动条值变化的槽函数
     * @param value 当前值
     * @return 无返回值
     * @details 垂直平移图表的显示区域
     */
    void on_vSBar_valueChanged(int value);

private:
    KDChart::Chart* m_chart = nullptr;               // 图表对象，管理图表布局
    TableModel m_model;                              // 数据模型，存储图表数据
    KDChart::DatasetProxyModel* m_datasetProxy = nullptr; // 数据集代理模型，处理数据过滤和转换
    KDChart::LineDiagram* m_lines = nullptr;         // 折线图对象，负责数据可视化
    KDChart::Legend* m_legend = nullptr;             // 图例对象，显示数据集说明
};

#endif /* MAINWINDOW_H */
