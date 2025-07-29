/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件实现了BubbleChart项目的MainWindow类
// 负责创建气泡图图表，初始化数据模型，设置气泡标记属性
// 支持3D效果切换和动态调整气泡大小
// 气泡图通过不同大小的圆形标记展示三个维度的数据
// X坐标、Y坐标和气泡大小分别代表不同的数据维度

// 主窗口头文件
#include "mainwindow.h"

// KD图表主类头文件
// TODO: Qt5.15.2升级 检查KDChartChart API是否有变更
#include <KDChartChart>
// 数据值属性类头文件
// TODO: Qt5.15.2升级 检查KDChartDataValueAttributes API是否有变更
#include <KDChartDataValueAttributes>
// 标记属性类头文件
// TODO: Qt5.15.2升级 检查KDChartMarkerAttributes API是否有变更
#include <KDChartMarkerAttributes>
// 绘图仪类头文件
// TODO: Qt5.15.2升级 检查KDChartPlotter API是否有变更
#include <KDChartPlotter>
// 文本属性类头文件
// TODO: Qt5.15.2升级 检查KDChartTextAttributes API是否有变更
#include <KDChartTextAttributes>

// 标准项数据模型头文件
// TODO: Qt5.15.2升级 检查QStandardItemModel API是否有变更
#include <QStandardItemModel>
// 定时器类头文件
#include <QTimer>

// 使用KDChart命名空间
using namespace KDChart;

// 气泡数量常量
static const int nBubbles = 7;
// 数据结构：用于存储气泡图数据
// 0: Y值
// 1: X值
// 2: 气泡大小
/**
 * @brief 气泡数据结构
 * @details 存储气泡图中每个数据点的X坐标、Y坐标和气泡大小
 */
struct DataType
{
    /**
     * @brief 构造函数
     * @param x_ X坐标
     * @param y_ Y坐标
     * @param size_ 气泡大小
     */
    DataType(qreal x_,
             qreal y_,
             qreal size_)
        : x(x_)  // X坐标
        , y(y_)  // Y坐标
        , size(size_)  // 气泡大小
    {}
    qreal x;      // X坐标
    qreal y;      // Y坐标
    qreal size;   // 气泡大小
};

// 气泡图示例数据
// TODO: C++17升级 考虑使用std::array替代C风格数组
// TODO: C++17升级 考虑使用结构化绑定优化数据访问
static const DataType bubblesData[nBubbles] = {
    DataType(0.5, 1.0, 100),  // 第一个气泡数据
    DataType(1.0, 0.5, 60),   // 第二个气泡数据
    DataType(1.6, 2.0, 28),   // 第三个气泡数据
    DataType(0.7, 0.3, 55),   // 第四个气泡数据
    DataType(1.3, 2.0, 95),   // 第五个气泡数据
    DataType(2.0, 1.0, 75),   // 第六个气泡数据
    DataType(1.4, 1.1, 85)    // 第七个气泡数据
};

// 自定义角色：用于存储气泡大小
#define ROLE_SIZE Qt::UserRole + 1

/**
 * @brief 构造函数
 * @param parent 父窗口部件，默认为nullptr
 * @details 初始化主窗口UI，创建图表和绘图仪
 * 设置坐标轴，连接信号槽，并初始化标记属性
 * TODO: Qt5.15.2升级 检查QWidget相关API是否有变更
 * TODO: Qt5.15.2升级 检查QHBoxLayout API是否有变更
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置UI控件
    setupUi(this);

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    // 创建图表对象
    m_chart = new Chart();
    // 添加图表到布局
    chartLayout->addWidget(m_chart);
    // 初始化数据模型
    initializeDataModel();
    // 创建绘图仪对象
    m_plotter = new Plotter();
    // 设置绘图仪的数据模型
    m_plotter->setModel(m_model);
    // 创建底部X轴
    auto *xAxis = new CartesianAxis(m_plotter);
    // 创建顶部X轴
    auto *xAxis2 = new CartesianAxis(m_plotter);
    // 创建左侧Y轴
    auto *yAxis = new CartesianAxis(m_plotter);
    // 创建右侧Y轴
    auto *yAxis2 = new CartesianAxis(m_plotter);
    // 设置底部X轴位置
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    // 设置顶部X轴位置
    xAxis2->setPosition(KDChart::CartesianAxis::Top);
    // 设置左侧Y轴位置
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    // 设置右侧Y轴位置
    yAxis2->setPosition(KDChart::CartesianAxis::Right);
    // 添加底部X轴到绘图仪
    m_plotter->addAxis(xAxis);
    // 添加顶部X轴到绘图仪
    m_plotter->addAxis(xAxis2);
    // 添加左侧Y轴到绘图仪
    m_plotter->addAxis(yAxis);
    // 添加右侧Y轴到绘图仪
    m_plotter->addAxis(yAxis2);
    // 连接3D效果复选框的toggled信号到setMarkerAttributes槽函数
    // TODO: Qt5.15.2升级 检查信号槽连接方式是否有变更
    connect(threeDEnabled, &QCheckBox::toggled, this, &MainWindow::setMarkerAttributes);

    // 替换图表的坐标系为绘图仪
    m_chart->coordinatePlane()->replaceDiagram(m_plotter);
    // 设置图表全局边距
    m_chart->setGlobalLeading(20, 20, 20, 20);

    // 设置标记属性
    setMarkerAttributes();
}

/**
 * @brief 初始化数据模型
 * @details 创建并填充数据模型，为气泡图提供X、Y坐标和气泡大小数据
 * 数据模型包含7行2列，分别存储X和Y坐标
 * 气泡大小通过自定义角色ROLE_SIZE存储
 * TODO: C++17升级 考虑使用范围for循环优化数据填充
 */
void MainWindow::initializeDataModel()
{
    // 创建数据模型，7行2列
    m_model = new QStandardItemModel(nBubbles, 2);
    // 设置表头
    m_model->setHeaderData(0, Qt::Horizontal, tr("Some Bubbles"));
    // 遍历所有气泡数据
    for (int i = 0; i < nBubbles; ++i) {
        // X坐标索引
        const QModelIndex indexX = m_model->index(i, 0);
        // Y坐标索引
        const QModelIndex indexY = m_model->index(i, 1);

        // 设置X坐标
        m_model->setData(indexX, QVariant(bubblesData[i].x), Qt::DisplayRole);

        // 设置Y坐标
        m_model->setData(indexY, QVariant(bubblesData[i].y), Qt::DisplayRole);

        // 设置气泡大小
        m_model->setData(indexY, bubblesData[i].size, ROLE_SIZE);
    }
}

/**
 * @brief 设置标记属性
 * @details 配置气泡图中每个标记的样式、大小和3D效果
 * 根据ROLE_SIZE角色设置每个气泡的大小
 * 根据threeDEnabled复选框状态设置是否启用3D效果
 * TODO: Qt5.15.2升级 检查KDChart::DataValueAttributes相关API是否有变更
 * TODO: C++17升级 考虑使用结构化绑定和范围for循环优化代码
 */
void MainWindow::setMarkerAttributes()
{
    // 禁用连接线
    m_plotter->setPen(Qt::NoPen);

    // 遍历所有气泡
    for (int iRow = 0; iRow < nBubbles; ++iRow) {
        // X坐标索引
        const QModelIndex indexX = m_plotter->model()->index(iRow, 0);
        // Y坐标索引
        const QModelIndex indexY = m_plotter->model()->index(iRow, 1);
        // 获取数据值属性
        DataValueAttributes dva(m_plotter->dataValueAttributes(indexX));
        // 设置可见
        dva.setVisible(true);

        // 获取文本属性
        TextAttributes ta(dva.textAttributes());
        // 设置文本不可见
        ta.setVisible(false);

        // 获取标记属性
        MarkerAttributes ma(dva.markerAttributes());
        // 设置标记可见
        ma.setVisible(true);
        // 设置标记样式为圆形
        ma.setMarkerStyle(MarkerAttributes::MarkerCircle);
        // 设置是否启用3D效果
        ma.setThreeD(threeDEnabled->isChecked());

        // 设置大小
        // 获取气泡大小
        const qreal d = m_model->data(indexY, ROLE_SIZE).toReal();
        // 设置标记大小
        ma.setMarkerSize(QSizeF(d, d));

        // 获取位置
        RelativePosition pos(dva.positivePosition());
        // 设置对齐方式
        pos.setAlignment(Qt::AlignCenter);
        // 设置水平边距
        pos.setHorizontalPadding(0);

        // 设置位置
        dva.setPositivePosition(pos);
        // 设置标记属性
        dva.setMarkerAttributes(ma);
        // 设置文本属性
        dva.setTextAttributes(ta);

        // 注意：KDChart::Plotter查看X单元格的数据值属性
        // Y单元格的属性会被忽略
        // 设置数据值属性
        m_plotter->setDataValueAttributes(indexX, dva);
    }
    // 更新图表
    m_chart->update();
}
