/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，展示如何设置图表的背景、边框、标题和文本属性等
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include <KDChartBackgroundAttributes>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartHeaderFooter>
#include <KDChartPosition>
#include <QApplication>
#include <QtGui>

#include <QPixmap>

using namespace KDChart;

/**
 * @brief 自定义图表窗口部件类
 * 该类继承自QWidget，用于展示如何设置KD Chart图表的背景、边框、标题和文本属性等
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * 
     * 初始化数据模型，创建柱状图，配置图表背景、边框、标题和文本属性等
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
            }
        }

        // 创建背景图片
        auto *pixmap = new QPixmap("background.png");
        // 创建柱状图
        auto *diagram = new BarDiagram;
        // 设置图表数据模型
        diagram->setModel(&m_model);

        // 将图表添加到坐标平面
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 添加表头并设置
        auto *header = new HeaderFooter(&m_chart);
        header->setPosition(Position::North); // 设置表头位置在北部
        header->setText("A Simple Bar Chart"); // 设置表头文本
        m_chart.addHeaderFooter(header);

        // 配置表头文本属性
        TextAttributes hta;
        hta.setPen(QPen(Qt::blue)); // 设置文本颜色为蓝色

        // 设置字体大小为相对模式
        Measure m(35.0);
        m.setRelativeMode(header->autoReferenceArea(),
                          KDChartEnums::MeasureOrientationMinimum);
        hta.setFontSize(m);
        // 设置最小字体大小
        m.setValue(3.0);
        m.setCalculationMode(KDChartEnums::MeasureCalculationModeAbsolute);
        hta.setMinimalFontSize(m);
        header->setTextAttributes(hta);

        // 配置坐标平面的背景属性
        BackgroundAttributes pba;
        pba.setPixmap(*pixmap); // 设置背景图片
        pba.setPixmapMode(BackgroundAttributes::BackgroundPixmapModeStretched); // 设置图片拉伸模式
        pba.setVisible(true); // 显示背景
        diagram->coordinatePlane()->setBackgroundAttributes(pba);

        // 配置表头的背景属性
        BackgroundAttributes hba;
        hba.setBrush(Qt::white); // 设置背景颜色为白色
        hba.setVisible(true); // 显示背景
        header->setBackgroundAttributes(hba);

        // 配置坐标平面的边框属性
        FrameAttributes pfa;
        pfa.setPen(QPen(QBrush(Qt::blue), 2)); // 设置边框颜色为蓝色，宽度为2
        pfa.setVisible(true); // 显示边框
        diagram->coordinatePlane()->setFrameAttributes(pfa);

        // 配置表头的边框属性
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2)); // 设置边框颜色为深灰色，宽度为2
        hfa.setPadding(2); // 设置内边距为2
        hfa.setVisible(true); // 显示边框
        header->setFrameAttributes(hfa);

        // 设置布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        setLayout(l);
    }

private:
    Chart m_chart; // 图表对象
    QStandardItemModel m_model; // 数据模型
    QPixmap pixmap; // 背景图片
};

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 初始化Qt应用程序，创建并显示ChartWidget窗口部件，启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ChartWidget w; // 创建图表窗口部件
    w.show(); // 显示窗口

    return app.exec(); // 运行应用程序事件循环
}

#include "main.moc"