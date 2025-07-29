/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChartAbstractCoordinatePlane>
#include <KDChartCartesianAxis>
#include <KDChartChart>
#include <KDChartLegend>
#include <KDChartLineDiagram>

#include <QDebug>
#include <QHBoxLayout>
#include <QPen>
#include <QStandardItemModel>

using namespace KDChart;

/**
 * @brief 空数据模型类
 * 
 * 继承自QAbstractItemModel，实现了一个没有任何数据的模型。
 * 用于演示KDChart如何处理没有数据的情况。
 */
class EmptyModel : public QAbstractItemModel
{
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，默认为nullptr
     * 
     * 创建一个空数据模型实例。
     */
    EmptyModel(QObject *parent = nullptr)
        : QAbstractItemModel(parent)
    {
        // qDebug() << "EmptyModel::EmptyModel()";
    }

    /**
     * @brief 析构函数
     * 
     * 销毁空数据模型实例。
     */
    ~EmptyModel() override
    {
        // qDebug() << "EmptyModel::~EmptyModel()";
    }

    /**
     * @brief 获取列数
     * @param parent 父索引，默认为QModelIndex()
     * @return 列数，固定返回0
     * 
     * 重写QAbstractItemModel的方法，返回0表示没有列。
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::columnCount(...)";
        return 0; // 返回0列
    }

    /**
     * @brief 获取行数
     * @param parent 父索引，默认为QModelIndex()
     * @return 行数，固定返回0
     * 
     * 重写QAbstractItemModel的方法，返回0表示没有行。
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::rowCount(...)";
        return 0; // 返回0行
    }

    // NOTE: 以下方法不会被KD Chart调用，
    //       因为模型返回的列数和行数都是0。
    /**
     * @brief 获取数据
     * @param index 索引
     * @param role 数据角色，默认为Qt::DisplayRole
     * @return 数据，这里不会被调用
     * 
     * 重写QAbstractItemModel的方法，但由于模型为空，此方法不会被调用。
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        Q_UNUSED(role);
        qDebug() << "EmptyModel::data(" << index.row() << index.column() << ")";
        Q_ASSERT_X(false, "EmptyModel::data", "We should not end here...");
        return QVariant();
    }

    /**
     * @brief 创建索引
     * @param row 行号
     * @param column 列号
     * @param parent 父索引，默认为QModelIndex()
     * @return 索引，返回无效索引
     * 
     * 重写QAbstractItemModel的方法，返回无效索引。
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(row);
        Q_UNUSED(column);
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::index(" << row << column << ")";
        return QModelIndex(); // 返回无效索引
    }

    /**
     * @brief 获取父索引
     * @param parent 索引
     * @return 父索引，返回无效索引
     * 
     * 重写QAbstractItemModel的方法，返回无效索引。
     */
    QModelIndex parent(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::parent(...)";
        return QModelIndex(); // 返回无效索引
    }
}; // TODO: C++17升级 考虑使用final修饰符标记最终类

/**
 * @brief 主窗口构造函数
 * @param parent 父窗口部件，默认为nullptr
 * 
 * 初始化主窗口，创建图表布局和空数据模型，设置图表和坐标轴。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 创建水平布局
    auto *chartLayout = new QHBoxLayout(this);
    // 创建图表对象
    m_chart = new Chart();
    // 设置图表全局边距
    m_chart->setGlobalLeading(5, 5, 5, 5);
    // 将图表添加到布局
    chartLayout->addWidget(m_chart);

    // 创建空数据模型
    m_model = new EmptyModel(this); // 模型完全不包含任何数据

    // 设置图表
    m_bars = new LineDiagram();
    m_bars->setModel(m_model); // 关联空数据模型
    // 创建X轴
    auto *xAxis = new CartesianAxis(m_bars);
    // 创建Y轴
    auto *yAxis = new CartesianAxis(m_bars);
    // 设置X轴位置在底部
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    // 设置Y轴位置在左侧
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    // 设置X轴标题
    xAxis->setTitleText("Abscissa axis at the bottom");
    // 设置Y轴标题
    yAxis->setTitleText("Ordinate axis at the left side");
    // 添加X轴到图表
    m_bars->addAxis(xAxis);
    // 添加Y轴到图表
    m_bars->addAxis(yAxis);

    // 替换坐标系中的图表
    m_chart->coordinatePlane()->replaceDiagram(m_bars);

    // 创建图例
    auto *legend = new Legend(m_bars, m_chart);
    // 设置图例位置在底部
    legend->setPosition(Position::South);
    // 设置图例对齐方式为居中
    legend->setAlignment(Qt::AlignCenter);
    // 设置图例显示连接线
    legend->setShowLines(true);
    // 设置图例标题
    legend->setTitleText("This is the legend - showing no data either");
    // 设置图例方向为水平
    legend->setOrientation(Qt::Horizontal);
    // 添加图表到图例
    legend->addDiagram(m_bars);
    // 将图例添加到图表
    m_chart->addLegend(legend);
} // TODO: Qt5.15.2升级 检查KDChart::Chart和相关类在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QHBoxLayout在Qt5.15.2中的布局行为
// TODO: C++17升级 考虑使用结构化绑定优化变量声明
// TODO: C++17升级 考虑使用std::make_unique管理动态内存
