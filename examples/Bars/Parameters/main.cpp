/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，展示柱状图参数配置功能
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

/**
 * @brief 自定义图表窗口部件类
 * 该类继承自QWidget，用于展示柱状图参数配置功能
 * 包括固定宽度、间距配置、数值显示和边框样式等
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 初始化数据模型，创建柱状图，并配置柱状图参数
     */
    explicit ChartWidget(QWidget *parent = nullptr);

private:
    Chart m_chart; // 图表对象，用于显示柱状图
    QStandardItemModel m_model; // 数据模型，存储图表显示的数据
};

/**
 * @brief ChartWidget类构造函数实现
 * @param parent 父窗口部件
 * 初始化数据模型，创建柱状图，配置柱状图参数，并设置布局
 */
ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    // 初始化数据模型，插入2行3列
    m_model.insertRows(0, 2, QModelIndex());
    m_model.insertColumns(0, 3, QModelIndex());
    // 填充模型数据
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            QModelIndex index = m_model.index(row, column, QModelIndex());
            m_model.setData(index, row * 3 + column);
        }
    }

    // 创建柱状图
    auto *diagram = new KDChart::BarDiagram;
    diagram->setModel(&m_model); // 关联数据模型

    // 配置柱状图属性
    BarAttributes ba(diagram->barAttributes());
    // 设置柱状图固定宽度并启用
    ba.setFixedBarWidth(140);
    ba.setUseFixedBarWidth(true);
    // 配置值之间和块之间的间距
    ba.setGroupGapFactor(0.50);
    ba.setBarGapFactor(0.125);

    // 应用柱状图属性
    diagram->setBarAttributes(ba);

    // 配置数据值显示属性
    DataValueAttributes dva(diagram->dataValueAttributes());
    TextAttributes ta(dva.textAttributes());
    // 旋转文本（如果需要）
    // ta.setRotation( 0 );
    ta.setFont(QFont("Comic", 9));
    ta.setPen(QPen(QColor(Qt::darkGreen)));
    ta.setVisible(true);
    dva.setTextAttributes(ta);
    dva.setVisible(true);
    diagram->setDataValueAttributes(dva);

    // 配置柱状图边框样式
    QPen linePen;
    linePen.setColor(Qt::magenta);
    linePen.setWidth(4);
    linePen.setStyle(Qt::DotLine);
    // 只为一个数据集设置边框
    // 如果要为所有柱状图设置边框，调用 setPen( myPen );
    diagram->setPen(1, linePen);

    // 将柱状图添加到图表
    m_chart.coordinatePlane()->replaceDiagram(diagram);
    m_chart.setGlobalLeadingTop(40); // 设置图表顶部边距

    // 设置布局
    auto *l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
}

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 程序入口点，创建应用程序对象和图表窗口部件，并运行事件循环
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象，初始化Qt框架

    ChartWidget w; // 创建图表窗口部件
    w.show(); // 显示窗口

    return app.exec(); // 运行应用程序事件循环，处理用户输入和系统事件
}

#include "main.moc"
