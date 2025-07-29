/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** 原文注释: This file is part of the KD Chart library.
** 中文注释: 该文件演示了当动态添加行和列数据时，KD Chart图表如何实时更新显示
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** 原文注释: SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 原文注释: SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartBarDiagram>
#include <KDChartCartesianAxis>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartLegend>
#include <QApplication>
#include <QPushButton>
#include <QtGui>

/**
 * @class ChartWidget
 * @brief 延迟数据测试的主窗口部件类
 * 用于演示动态添加行和列数据时KD Chart图表的更新效果
 * 继承自QWidget，包含图表显示和数据操作按钮
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数，初始化图表窗口
     * @param parent 父窗口指针
     */
    explicit ChartWidget(QWidget *parent = 0)
        : QWidget(parent)  // 调用父类QWidget的构造函数
    {
        // 初始化数据模型，插入0行
        m_model.insertRows(0, 0, QModelIndex());
        // 创建柱状图对象
        auto *diagram = new KDChart::BarDiagram;
        // 设置图表的数据模型
        diagram->setModel(&m_model);

        // 创建图例，并关联到图表
        auto *legend = new KDChart::Legend(diagram, diagram);
        // 将图例添加到图表
        m_chart.addLegend(legend);

        // 以下代码目前无法工作，但期望实现多图表关联图例的功能
#if 0
    KDChart::Legend* legend = new KDChart::Legend;
    legend->addDiagram(diagram1);
    legend->addDiagram(diagram2);
    ...
    m_chart.addLegend(legend);
#endif

        // 创建横轴（X轴）
        auto *abcissa = new KDChart::CartesianAxis(diagram);
        // 设置横轴位置在底部
        abcissa->setPosition(KDChart::CartesianAxis::Bottom);
        // 创建纵轴（Y轴）
        auto *ordinate = new KDChart::CartesianAxis(diagram);
        // 设置纵轴位置在左侧
        ordinate->setPosition(KDChart::CartesianAxis::Left);
        // 为图表添加横轴
        diagram->addAxis(abcissa);
        // 为图表添加纵轴
        diagram->addAxis(ordinate);

        // 注意：如果在添加轴之前执行此操作，轴将不会显示
        // 替换坐标平面中的图表
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 设置按钮文本
        m_rowbutton.setText(tr("Add rows"));
        m_colbutton.setText(tr("Add columns"));
        // 连接添加行按钮的点击信号到addRows槽函数
        connect(&m_rowbutton, &QPushButton::clicked,
                this, &ChartWidget::addRows);
        // 连接添加列按钮的点击信号到addCols槽函数
        connect(&m_colbutton, &QPushButton::clicked,
                this, &ChartWidget::addCols);

        // 创建垂直布局
        auto *l = new QVBoxLayout(this);
        // 添加图表到布局
        l->addWidget(&m_chart);
        // 添加添加行按钮到布局
        l->addWidget(&m_rowbutton);
        // 添加添加列按钮到布局
        l->addWidget(&m_colbutton);

        // 设置窗口布局
        setLayout(l);
    }

private slots:

    /**
     * @brief 添加一行数据到模型
     * 在模型末尾插入一行，并为每个列设置初始数据
     */
    void addRows()
    {
        // 在当前行计数位置插入一行
        m_model.insertRows(m_model.rowCount(), 1);
        // 为新行的每个列设置数据
        for (int i = 0; i < m_model.columnCount(); ++i) {
            m_model.setData(m_model.index(m_model.rowCount() - 1, i), i);
        }
    }
    /**
     * @brief 添加一列数据到模型
     * 在模型末尾插入一列，并为每个行设置初始数据
     */
    void addCols()
    {
        // 在当前列计数位置插入一列
        m_model.insertColumns(m_model.columnCount(), 1);
        // 为新列的每个行设置数据
        for (int i = 0; i < m_model.rowCount(); ++i) {
            m_model.setData(m_model.index(i, m_model.columnCount() - 1), i);
        }
    }

private:
    /**
     * @brief KDChart图表对象
     * 用于显示柱状图和图例
     */
    KDChart::Chart m_chart;
    /**
     * @brief 添加行按钮
     * 点击时触发addRows槽函数
     */
    QPushButton m_rowbutton;
    /**
     * @brief 添加列按钮
     * 点击时触发addCols槽函数
     */
    QPushButton m_colbutton;
    /**
     * @brief 标准项数据模型
     * 存储图表显示的数据
     */
    QStandardItemModel m_model;
};

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char **argv)
{
    // 创建QApplication实例
    QApplication app(argc, argv);

    // 创建ChartWidget实例
    ChartWidget w;
    // 显示窗口
    w.show();

    // 运行应用程序事件循环
    return app.exec();
}

// 包含元对象编译器生成的代码
#include "main.moc"
