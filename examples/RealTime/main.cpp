/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例文件，展示实时数据更新图表的实现。
** 该示例通过定时器定期更新图表数据，模拟实时数据可视化场景。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/
// TODO: Qt5.15.2升级 检查头文件包含是否需要调整

// KD Chart柱状图类
#include <KDChartBarDiagram>
// KD Chart核心图表类
#include <KDChartChart>
// Qt图形界面库
#include <QtGui>
// Qt应用程序框架
#include <QApplication>
// TODO: Qt5.15.2升级 检查KDChart API是否有变更
// TODO: Qt5.15.2升级 验证QtGui模块在新版本中的兼容性

/**
 * @brief 图表窗口部件类
 * @details 继承自QWidget，用于展示实时更新的柱状图
 *          该类创建一个包含柱状图的窗口，并通过定时器定期更新图表数据
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件指针
     * @details 初始化图表窗口，创建数据模型，设置柱状图，并启动定时器定期更新数据
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 插入2行数据
        m_model.insertRows(0, 2, QModelIndex());
        // 插入3列数据
        m_model.insertColumns(0, 3, QModelIndex());
        // 初始化数据模型
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row + 1 * column));
            }
        }

        // 创建柱状图对象
        auto *diagram = new KDChart::BarDiagram;
        // 设置图表数据模型
        diagram->setModel(&m_model);

        // 将图表添加到坐标平面
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 创建垂直布局
        auto *l = new QVBoxLayout(this);
        // 将图表添加到布局
        l->addWidget(&m_chart);
        // 设置窗口布局
        setLayout(l);
        // 创建定时器
        m_timer = new QTimer(this);
        // 连接定时器超时信号到更新槽函数
        connect(m_timer, &QTimer::timeout,
                this, &ChartWidget::slotTimeout);
        // 启动定时器，间隔200毫秒
        m_timer->start(200);
    }

private slots:
    /**
     * @brief 定时器超时槽函数
     * @details 定期更新图表数据，模拟实时数据变化
     *          这里更新第一行第二列的数据，使其在1-24之间循环变化
     */
    void slotTimeout()
    {
        // 获取要更新的数据索引
        QModelIndex index = m_model.index(0, 1, QModelIndex());
        // 计算新值，使其在1-24之间循环
        qreal value = (m_model.data(index).toInt() % 24) + 1;
        // 更新数据模型
        m_model.setData(index, value);
    }

private:
    KDChart::Chart m_chart;          // 图表对象
    QStandardItemModel m_model;      // 数据模型
    QTimer *m_timer;                 // 定时器，用于定期更新数据
}; // TODO: C++17升级 考虑使用final修饰符
// TODO: Qt5.15.2升级 检查QTimer和QStandardItemModel在新版本中的API变化

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 创建Qt应用程序实例，初始化图表窗口并显示，启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    // 创建Qt应用程序实例
    QApplication app(argc, argv);

    // 创建图表窗口实例
    ChartWidget w;
    // 显示图表窗口
    w.show();

    // 启动应用程序事件循环
    return app.exec();
} // TODO: Qt5.15.2升级 检查QApplication在新版本中的API变化

#include "main.moc"
