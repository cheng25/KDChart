/****************************************************************************
**
** 此文件是KD Chart库的示例程序的一部分。
** 它定义了坐标轴参数配置示例的主窗口类接口，包括图表创建、坐标轴配置
** 和用户交互处理等核心功能。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// 包含UI表单生成的头文件
#include "ui_mainwindow.h"
// 包含表格数据模型头文件
#include <TableModel.h>

// KDChart命名空间，包含所有图表相关类
namespace KDChart {
class Chart;           // 图表主容器
class DatasetProxyModel; // 数据集代理模型
class LineDiagram;     // 折线图组件
class LineAttributes;  // 线条属性
class CartesianAxis;   // 笛卡尔坐标轴
class Legend;          // 图例组件
}

/**
 * @brief 主窗口类
 * @details 该类继承自QWidget和Ui::MainWindow，是坐标轴参数配置示例的核心类。
 *          主要负责：
 *          - 创建和配置图表组件
 *          - 初始化和管理数据模型
 *          - 处理用户交互事件
 *          - 动态更新图表样式和坐标轴参数
 *          - 展示不同配置下的图表效果
 *          本类通过Qt的信号槽机制实现UI控件与图表数据的联动更新。
 * 
 * Qt5.15.2升级说明：
 * - 需检查QWidget及其派生类在Qt5.15.2中的行为变更
 * - 验证UI文件中使用的控件是否与Qt5.15.2兼容
 * - 关注CartesianAxis类在Qt5.15.2中的API变化
 * 
 * C++17升级说明：
 * - 可使用std::unique_ptr管理动态创建的图表对象，避免内存泄漏
 * - 考虑使用结构化绑定优化多返回值场景
 * - 可使用if constexpr简化条件编译代码
 */
// TODO: Qt5.15.2升级 检查QWidget::setLayout在Qt5.15.2中的高DPI布局行为
// TODO: Qt5.15.2升级 验证KDChart::CartesianAxis的API在Qt5.15.2中是否有变更
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT // Qt元对象宏，启用信号槽机制

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @details 初始化UI组件，创建图表对象，配置坐标轴属性，加载数据模型
     *          并设置信号槽连接，实现图表参数的动态调整。具体职责包括：
     *          1. 调用setupUi()初始化UI表单
     *          2. 创建KDChart::Chart对象作为图表容器
     *          3. 初始化数据模型并加载示例数据
     *          4. 创建折线图组件并绑定数据
     *          5. 配置坐标轴参数（刻度、标签、标题等）
     *          6. 设置信号槽连接，响应用户交互
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 线条类型下拉框索引变化的槽函数
     * @param index 当前选中的索引
     * @details 根据选中的索引更新图表中线条的显示样式，如实线、虚线、点线等
     *          索引与线条类型的对应关系由UI设计决定
     */
    void on_lineTypeCB_currentIndexChanged(int index);
    /**
     * @brief 绘制图例复选框状态变化的槽函数
     * @param checked 是否选中
     * @details 控制图例的显示或隐藏。当选中时，在图表中显示图例；
     *          未选中时，隐藏图例
     */
    void on_paintLegendCB_toggled(bool checked);
    /**
     * @brief 绘制数值复选框状态变化的槽函数
     * @param checked 是否选中
     * @details 控制图表中数据值标签的显示或隐藏。当选中时，在每个数据点
     *          上方显示对应的数值；未选中时，隐藏这些数值
     */
    void on_paintValuesCB_toggled(bool checked);
    /**
     * @brief 绘制标记复选框状态变化的槽函数
     * @param checked 是否选中
     * @details 控制图表中数据点标记的显示或隐藏。当选中时，在每个数据点
     *          处显示标记；未选中时，仅显示线条
     */
    void on_paintMarkersCB_toggled(bool checked);
    /**
     * @brief 标记样式下拉框索引变化的槽函数
     * @param index 当前选中的索引
     * @details 更新图表中数据点标记的显示样式，如圆形、方形、三角形等
     *          索引与标记样式的对应关系由UI设计决定
     */
    void on_markersStyleCB_currentIndexChanged(int index);
    /**
     * @brief 标记宽度微调框值变化的槽函数
     * @param i 当前值
     * @details 更新图表中数据点标记的宽度。值越大，标记越宽
     */
    void on_markersWidthSB_valueChanged(int i);
    /**
     * @brief 标记高度微调框值变化的槽函数
     * @param i 当前值
     * @details 更新图表中数据点标记的高度。值越大，标记越高
     */
    void on_markersHeightSB_valueChanged(int i);
    /**
     * @brief 显示区域复选框状态变化的槽函数
     * @param checked 是否选中
     * @details 控制图表中线条下方区域填充的显示或隐藏。当选中时，线条与
     *          X轴之间的区域将被填充；未选中时，仅显示线条
     */
    void on_displayAreasCB_toggled(bool checked);
    /**
     * @brief 透明度微调框值变化的槽函数
     * @param i 当前值
     * @details 更新图表中区域填充的透明度。值越小，透明度越高（越透明）；
     *          值越大，透明度越低（越不透明）
     */
    void on_transparencySB_valueChanged(int i);
    /**
     * @brief 缩放因子微调框值变化的槽函数
     * @param factor 当前缩放因子
     * @details 更新图表的缩放比例。factor大于1时放大图表，小于1时缩小图表
     */
    void on_zoomFactorSB_valueChanged(double factor);
    /**
     * @brief 水平滚动条值变化的槽函数
     * @param value 当前值
     * @details 水平平移图表的显示区域。通过调整滚动条位置，可以查看图表
     *          的不同水平部分
     */
    void on_hSBar_valueChanged(int value);
    /**
     * @brief 垂直滚动条值变化的槽函数
     * @param value 当前值
     * @details 垂直平移图表的显示区域。通过调整滚动条位置，可以查看图表
     *          的不同垂直部分
     */
    void on_vSBar_valueChanged(int value);

private:
    KDChart::Chart* m_chart = nullptr;               // 图表对象，管理图表布局、坐标轴和图表类型
    TableModel m_model;                              // 数据模型，存储图表显示的数据
    KDChart::DatasetProxyModel* m_datasetProxy = nullptr; // 数据集代理模型，处理数据过滤和转换
    KDChart::LineDiagram* m_lines = nullptr;         // 折线图对象，负责数据的折线图可视化
    KDChart::Legend* m_legend = nullptr;             // 图例对象，显示数据集的名称和样式说明
};

#endif /* MAINWINDOW_H */

// TODO: C++17升级 使用std::unique_ptr替换原始指针管理动态资源
// TODO: C++17升级 考虑使用std::optional优化可能为空的返回值
// TODO: Qt5.15.2升级 检查QObject::connect的新语法在Qt5.15.2中的使用
