/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <KDChartBackgroundAttributes>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartLegend>
#include <KDChartPosition>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

/**
 * @brief 自定义图表窗口部件类
 * 
 * 该类继承自QWidget，用于展示带有自定义图例属性的柱状图。
 * 包含图表、数据模型和自定义的图例配置。
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化数据模型，创建图表和图例，并配置图例的各种属性。
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型
        m_model.insertRows(0, 2, QModelIndex());
        m_model.insertColumns(0, 3, QModelIndex());
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row + 1 * column));
            }
        }

        // 创建柱状图并设置模型
        auto *diagram = new BarDiagram;
        diagram->setModel(&m_model);

        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 添加图例并进行基本设置
        auto *legend = new Legend(diagram, &m_chart);
        legend->setPosition(Position::North); // 设置图例位置在顶部
        legend->setAlignment(Qt::AlignCenter); // 设置图例对齐方式为居中
        legend->setShowLines(true); // 显示图例中的连接线
        legend->setSpacing(5); // 设置图例项间距
        legend->setTitleText(tr("图例")); // 设置图例标题
        legend->setOrientation(Qt::Horizontal); // 设置图例方向为水平
        m_chart.addLegend(legend);

        // 配置图例项的标记样式
        MarkerAttributes lma(legend->markerAttributes(0));
        lma.setMarkerStyle(MarkerAttributes::MarkerDiamond); // 设置标记样式为菱形
        legend->setMarkerAttributes(0, lma);
        lma.setMarkerStyle(MarkerAttributes::MarkerCircle); // 设置标记样式为圆形
        legend->setMarkerAttributes(1, lma);

        // 配置图例标题和标签文本
        legend->setTitleText("柱状图");
        legend->setText(0, "系列 1");
        legend->setText(1, "系列 2");
        legend->setText(2, "系列 3");

        // 调整图例项的字体
        TextAttributes lta(legend->textAttributes());
        lta.setPen(QPen(Qt::darkGray)); // 设置文本颜色
        Measure me(lta.fontSize());
        me.setValue(me.value() * 1.5);
        lta.setFontSize(Measure(9, KDChartEnums::MeasureCalculationModeAbsolute)); // 设置字体大小
        legend->setTextAttributes(lta);

        // 调整图例标题的字体
        lta = legend->titleTextAttributes();
        lta.setPen(QPen(Qt::darkGray)); // 设置标题颜色
        me = lta.fontSize();
        me.setValue(me.value() * 1.5);
        lta.setFontSize(me); // 设置标题字体大小
        legend->setTitleTextAttributes(lta);

        // 配置标记的边框笔
        QPen markerPen;
        markerPen.setColor(Qt::darkGray); // 设置边框颜色
        markerPen.setWidth(2); // 设置边框宽度
        // 为每个图例项设置边框笔
        for (uint i = 0; i < 3; i++)
            legend->setPen(i, markerPen);

        // 为图例添加背景
        BackgroundAttributes ba;
        ba.setBrush(Qt::white); // 设置背景颜色
        ba.setVisible(true); // 显示背景
        legend->setBackgroundAttributes(ba);

        // 为图例添加边框
        FrameAttributes fa;
        fa.setPen(markerPen); // 设置边框笔
        fa.setVisible(true); // 显示边框
        legend->setFrameAttributes(fa);

        // 设置布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        m_chart.setGlobalLeadingTop(10); // 设置图表顶部边距
        setLayout(l);
    }

private:
    Chart m_chart; // 图表对象
    QStandardItemModel m_model; // 数据模型
};

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * 
 * 创建应用程序实例和ChartWidget窗口，并显示窗口。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    ChartWidget w; // 创建自定义图表窗口
    w.show(); // 显示窗口

    return app.exec(); // 运行应用程序事件循环
}

#include "main.moc"
