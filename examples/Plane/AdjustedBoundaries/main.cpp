/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartAbstractCartesianDiagram>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBackgroundAttributes>
#include <KDChartBarDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartGridAttributes>
#include <KDChartHeaderFooter>
#include <KDChartPosition>
#include <QtGui>

#include <QApplication>

using namespace KDChart;

/**
 * @brief 自定义图表窗口部件类
 * 该类继承自QWidget，用于展示如何设置KD Chart图表的平面边界调整
 * 本示例主要演示了如何自定义图表的坐标轴、数据值属性、网格和标题等视觉属性
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     * @return 无返回值
     * @details 初始化数据模型，创建柱状图，配置图表坐标轴、数据值属性、网格和标题等
     * 核心功能是展示如何调整图表平面的垂直范围边界
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
                m_model.setData(index, QVariant(row + 10 * column));
            }
        }

        // 创建柱状图
        auto *diagram = new BarDiagram;
        diagram->setModel(&m_model);

        /*
         * 坐标轴设置
         */

        // 创建X轴和Y轴
        auto *xAxis = new CartesianAxis(diagram);
        auto *yAxis = new CartesianAxis(diagram);
        xAxis->setPosition(KDChart::CartesianAxis::Bottom); // 设置X轴在底部
        yAxis->setPosition(KDChart::CartesianAxis::Left);   // 设置Y轴在左侧
        diagram->addAxis(xAxis);
        diagram->addAxis(yAxis);

        /*
         * 坐标轴标签设置
         */

        // 设置以下为0，可以查看默认的横坐标标签
        // (== X轴标题，从数据文件读取)
#if 1
        QStringList months;
        months << "January"
               << "February";
        xAxis->setLabels(months); // 设置X轴标签
        QStringList shortMonths;
        shortMonths << "Jan"
                    << "Feb";
        xAxis->setShortLabels(shortMonths); // 设置X轴短标签
#endif

        /*
         * 数据值设置
         */

        // 我们在此按列设置数据值属性，
        // 因为我们希望文本以不同的颜色打印 -
        // 根据各自数据集的颜色
        const QFont font(QFont("Comic", 10));
        const int colCount = diagram->model()->columnCount();
        for (int iColumn = 0; iColumn < colCount; ++iColumn) {
            QBrush brush(diagram->brush(iColumn));
            DataValueAttributes a(diagram->dataValueAttributes(iColumn));
            TextAttributes ta(a.textAttributes());
            ta.setRotation(0); // 设置文本旋转角度
            ta.setFont(font);  // 设置字体
            ta.setPen(QPen(brush.color())); // 设置文本颜色
            ta.setVisible(true); // 显示文本
            a.setTextAttributes(ta);
            a.setSuffix("%"); // 设置后缀为百分比
            a.setVisible(true); // 显示数据值
            diagram->setDataValueAttributes(iColumn, a);
        }

        /*
         * 垂直范围设置
         */

        // m_chart.coordinatePlane() 返回一个抽象平面
        // 我们需要强制转换以便能够设置自己的边界
        // TODO: Qt5.15.2升级 检查KDChart::CartesianCoordinatePlane在Qt5.15.2中的兼容性
        auto *plane1 = static_cast<CartesianCoordinatePlane *>(m_chart.coordinatePlane());

        plane1->setVerticalRange(QPair<qreal, qreal>(0.0, 100.0)); // 设置垂直范围为0.0到100.0
        plane1->replaceDiagram(diagram);

        /*
         * 标题设置
         */

        // 添加一个标题并设置它
        auto *header = new HeaderFooter(&m_chart);
        header->setPosition(Position::North); // 设置标题位置在北部
        header->setText("A Bar Chart with Adjusted Vertical Range"); // 设置标题文本
        m_chart.addHeaderFooter(header);

        // 配置标题文本属性
        TextAttributes hta;
        hta.setPen(QPen(Qt::red)); // 设置文本颜色为红色

        // 让标题随窗口一起调整大小
        // 即相对大小
        Measure m(35.0);
        m.setRelativeMode(header->autoReferenceArea(),
                          KDChartEnums::MeasureOrientationMinimum);
        hta.setFontSize(m);
        // 设置最小字体大小
        m.setValue(3.0);
        m.setCalculationMode(KDChartEnums::MeasureCalculationModeAbsolute);
        hta.setMinimalFontSize(m);
        header->setTextAttributes(hta);

        // 配置标题背景
        BackgroundAttributes hba;
        hba.setBrush(Qt::white); // 设置背景颜色为白色
        hba.setVisible(true);    // 显示背景
        header->setBackgroundAttributes(hba);

        // 配置标题边框属性
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2)); // 设置边框颜色为深灰色，宽度为2
        hfa.setPadding(2);                         // 设置内边距为2
        hfa.setVisible(true);                      // 显示边框
        header->setFrameAttributes(hfa);

        /*
         * 网格设置
         */

        // 获取网格属性
        // 显示网格和子网格
        GridAttributes ga(plane1->gridAttributes(Qt::Vertical));

        // 配置子网格画笔
        QPen subGridPen(Qt::darkGray);
        subGridPen.setStyle(Qt::DotLine); // 设置为点线
        ga.setSubGridPen(subGridPen);

        // 更改步长和子步长宽度
        // 以适应边界
        ga.setGridStepWidth(10.0);  // 主网格步长为10.0
        ga.setGridSubStepWidth(5.0); // 子网格步长为5.0
        ga.setGridVisible(true);    // 显示主网格
        ga.setSubGridVisible(true); // 显示子网格

        // 将网格属性应用到平面
        plane1->setGridAttributes(Qt::Vertical, ga);

        // 设置布局
        auto *l = new QVBoxLayout(this);
        m_chart.setGlobalLeadingTop(5); // 设置全局顶部边距为5
        l->addWidget(&m_chart);
        setLayout(l);
    }

private:
    Chart m_chart;               // 图表对象，用于显示和管理图表及其所有元素
    QStandardItemModel m_model;  // 数据模型，存储和管理图表要显示的数据
    QPixmap pixmap;              // 背景图片（本示例未使用）
};

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 初始化Qt应用程序，创建并显示ChartWidget窗口部件，启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    // TODO: C++17升级 使用constexpr优化常量表达式
    QApplication app(argc, argv);

    ChartWidget w; // 创建图表窗口部件
    w.show();      // 显示窗口

    return app.exec(); // 运行应用程序事件循环
}

#include "main.moc"
