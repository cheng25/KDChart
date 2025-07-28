/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，展示简单柱状图的创建和使用
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
#include <QApplication>
#include <QtGui>
#include <QtSvg/QSvgGenerator>

/**
 * @brief 自定义图表窗口部件类
 * 该类继承自QWidget，用于展示如何创建和显示简单的柱状图
 * 包含图表对象和数据模型，实现了基本的柱状图绘制功能
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 初始化数据模型，创建柱状图，并设置布局
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型，插入2行3列
        m_model.insertRows(0, 2, QModelIndex());
        m_model.insertColumns(0, 3, QModelIndex());
        // 填充模型数据
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row + 1 * column));
                /*
                // 显示工具提示:
                m_model.setData(index,
                QString("<table>"
                    "<tr><td>行</td><td>列</td><td>值</td></tr>"
                    "<tr><th>%1</th><th>%2</th><th>%3</th></tr></table>"
                ).arg(row).arg(column).arg(row+1 * column), Qt::ToolTipRole );
                */
            }
        }

        /*
    {
    // 为一个数据项显示注释:
        const int row = 0;
        const int column = 2;
        const QModelIndex index = m_model.index(row, column, QModelIndex());
        m_model.setData(
            index,
            QString("值 %1/%2: %3")
                    .arg( row )
                    .arg( column )
                    .arg( m_model.data( index ).toInt() ),
            KDChart::CommentRole );
    }
    */

        // 创建柱状图
        auto *diagram = new KDChart::BarDiagram;
        // 设置图表数据模型
        diagram->setModel(&m_model);
        // 设置柱状图的边框笔样式
        diagram->setPen(QPen(Qt::black, 0));

        // 将图表添加到坐标平面
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 设置布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        setLayout(l);

        /*
    // 将图表渲染为SVG
    QSvgGenerator generator;
    generator.setFileName("/home/kdab/chart.svg");
    generator.setSize(QSize(300, 300));
    generator.setViewBox(QRect(0, 0, 300, 300));
    generator.setTitle(tr("SVG图表"));
    QPainter painter;
    painter.begin(&generator);
    painter.setRenderHint(QPainter::Antialiasing);
    m_chart.paint(&painter, generator.viewBox());
    painter.end();
    */
    }

private:
    KDChart::Chart m_chart; // 图表对象，用于显示柱状图
    QStandardItemModel m_model; // 数据模型，存储图表显示的数据
};

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 程序入口点，创建应用程序对象和图表窗口部件，并运行事件循环
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ChartWidget w; // 创建图表窗口部件
    w.show(); // 显示窗口

    return app.exec(); // 运行应用程序事件循环
}

#include "main.moc"
