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
#include <KDChartHeaderFooter>
#include <KDChartPosition>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

/**
 * @brief 自定义图表窗口部件类
 * 
 * 该类继承自QWidget，用于展示如何配置和显示带有自定义页眉的柱状图。
 * 包含图表对象、数据模型以及设置页眉属性（文本、背景、边框）的功能。
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化数据模型、创建图表和柱状图、配置页眉属性，并设置布局。
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

        // 将图表添加到坐标平面
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 添加页眉并设置位置和文本
        auto *header = new HeaderFooter(&m_chart);
        header->setPosition(Position::North);
        header->setText("简单柱状图");
        m_chart.addHeaderFooter(header);

        // 配置页眉文本属性
        TextAttributes hta;
        hta.setPen(QPen(Qt::blue)); // 设置文本颜色为蓝色

        // 设置相对字体大小
        Measure m(35.0);
        m.setRelativeMode(header->autoReferenceArea(), KDChartEnums::MeasureOrientationMinimum);
        hta.setFontSize(m);
        // 设置最小字体大小
        m.setValue(3.0);
        m.setCalculationMode(KDChartEnums::MeasureCalculationModeAbsolute);
        hta.setMinimalFontSize(m);
        header->setTextAttributes(hta);

        // 配置页眉背景属性
        BackgroundAttributes hba;
        hba.setBrush(Qt::white); // 设置背景为白色
        hba.setVisible(true);
        header->setBackgroundAttributes(hba);

        // 配置页眉边框属性
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2)); // 设置边框为深灰色，宽度2
        hfa.setVisible(true);
        header->setFrameAttributes(hfa);

        // 设置布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        m_chart.setGlobalLeadingTop(10); // 设置图表顶部间距
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
