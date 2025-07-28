/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <KDChartAbstractCartesianDiagram>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBackgroundAttributes>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartGridAttributes>
#include <KDChartHeaderFooter>
#include <KDChartLineDiagram>
#include <KDChartPosition>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

/**
 * @brief 自定义图表窗口部件类
 * 
 * 该类继承自QWidget，用于展示一个带有自定义网格配置的折线图。
 * 包含图表的初始化、数据模型设置、坐标轴配置、网格属性设置等功能。
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化图表窗口，设置数据模型，创建折线图，配置坐标轴、标题和网格属性。
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型，插入6行4列数据
        m_model.insertRows(0, 6, QModelIndex());
        m_model.insertColumns(0, 4, QModelIndex());
        for (int row = 0; row < 6; ++row) {
            for (int column = 0; column < 4; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row * 0.5 + column));
            }
        }

        // 创建折线图并设置数据模型
        auto *diagram = new LineDiagram;
        diagram->setModel(&m_model);

        // 创建并配置坐标轴
        auto *xAxis = new CartesianAxis(diagram);
        auto *yAxis = new CartesianAxis(diagram);
        xAxis->setPosition(KDChart::CartesianAxis::Bottom); // X轴位于底部
        yAxis->setPosition(KDChart::CartesianAxis::Left);   // Y轴位于左侧
        diagram->addAxis(xAxis);
        diagram->addAxis(yAxis);

        // 将图表添加到坐标平面
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        /* 配置标题 */

        // 添加标题并设置位置和文本
        auto *header = new HeaderFooter(&m_chart);
        header->setPosition(Position::North); // 标题位于顶部
        header->setText("配置了网格的折线图");
        m_chart.addHeaderFooter(header);

        // 配置标题文本属性
        TextAttributes hta;
        hta.setPen(QPen(Qt::red)); // 文本颜色为红色

        // 设置相对字体大小
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
        hba.setBrush(Qt::white); // 背景为白色
        hba.setVisible(true);    // 显示背景
        header->setBackgroundAttributes(hba);

        // 配置标题边框
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2)); // 边框为深灰色，宽度2
        hfa.setPadding(2);                         // 内边距2
        hfa.setVisible(true);                      // 显示边框
        header->setFrameAttributes(hfa);

        // 将抽象坐标平面转换为笛卡尔坐标平面
        auto *plane = static_cast<CartesianCoordinatePlane *>(diagram->coordinatePlane());

        /* 配置网格步长和画笔 */

        // 垂直网格
        GridAttributes gv(plane->gridAttributes(Qt::Vertical));

        // 配置主网格画笔
        QPen gridPen(Qt::gray);
        gridPen.setWidth(2);
        gv.setGridPen(gridPen);

        // 配置子网格画笔
        QPen subGridPen(Qt::darkGray);
        subGridPen.setStyle(Qt::DotLine); // 虚线样式
        gv.setSubGridPen(subGridPen);

        // 显示蓝色零线
        gv.setZeroLinePen(QPen(Qt::blue));

        // 设置网格步长和子步长宽度
        gv.setGridStepWidth(1.0);
        gv.setGridSubStepWidth(0.5);
        gv.setGridVisible(true);   // 显示主网格
        gv.setSubGridVisible(true); // 显示子网格

        // 水平网格
        GridAttributes gh = plane->gridAttributes(Qt::Horizontal);
        gh.setGridPen(gridPen);
        gh.setGridStepWidth(0.5);
        gh.setSubGridPen(subGridPen);
        gh.setGridSubStepWidth(0.1);

        // 应用网格属性
        plane->setGridAttributes(Qt::Vertical, gv);
        plane->setGridAttributes(Qt::Horizontal, gh);

        // 数据值显示和位置
        const int colCount = diagram->model()->columnCount(diagram->rootIndex());
        for (int iColumn = 0; iColumn < colCount; ++iColumn) {
            DataValueAttributes a(diagram->dataValueAttributes(iColumn));
            RelativePosition pos(a.position(true));
            pos.setAlignment(Qt::AlignRight);
            a.setPositivePosition(pos);
            QBrush brush(diagram->brush(iColumn));
            TextAttributes ta(a.textAttributes());
            ta.setRotation(0);
            ta.setFont(QFont("Comic", 10));
            ta.setPen(QPen(brush.color()));
            ta.setVisible(true);
            a.setVisible(true);
            a.setTextAttributes(ta);
            diagram->setDataValueAttributes(iColumn, a);
        }

        // 设置布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        m_chart.setGlobalLeadingRight(20);
        setLayout(l);
    }

private:
    Chart m_chart;              // 图表对象
    QStandardItemModel m_model; // 数据模型
    QPixmap pixmap;             //  pixmap对象，未使用
};

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * 
 * 创建应用程序实例和图表窗口，显示窗口并运行应用程序事件循环。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
