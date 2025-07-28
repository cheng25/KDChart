/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartLineDiagram>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

/**
 * @brief 图表窗口部件类
 * 
 * 继承自QWidget，用于展示带有自定义参数的线图示例。
 * 示例中演示了如何配置数据值属性、线条样式等高级功能。
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 创建一个ChartWidget实例，初始化数据模型和图表，并配置各种参数。
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型
        m_model.insertRows(0, 5, QModelIndex()); // 插入5行
        m_model.insertColumns(0, 5, QModelIndex()); // 插入5列

        // 填充数据
        qreal increase = 1.15;
        for (int i = 0; i < 5; ++i) {
            increase += 0.06;
            for (int j = 0; j < 5; ++j) {
                m_model.setData(m_model.index(i, j, QModelIndex()),
                                (increase + i) * j * (1.0 + 0.1 * (rand() % 10)));
            }
        }

        // 创建线图
        auto *diagram = new LineDiagram;
        diagram->setModel(&m_model); // 设置数据模型

        // 显示数据值
        // 1 - 获取相关属性
        DataValueAttributes dva(diagram->dataValueAttributes());

        // 2 - 配置数据值文本的字体和颜色
        TextAttributes ta(dva.textAttributes());
        dva.setDecimalDigits(2); // 设置小数位数为2

        // 为数据值标签添加前缀/后缀
        // dva.setPrefix( "* " );
        dva.setSuffix(" Ohm"); // 添加单位

        // 旋转文本（如果需要）
        // ta.setRotation( 0 );

        // 3 - 设置文本属性
        ta.setFont(QFont("Comic")); // 设置字体
        ta.setPen(QPen(QColor(Qt::darkGreen))); // 设置笔颜色
        ta.setVisible(true); // 显示文本

        // 设置字体大小
        Measure me(ta.fontSize());
        me.setValue(me.value() * 0.25); // 缩小字体
        ta.setFontSize(me);

        // 4 - 将文本属性分配给数据值属性
        dva.setTextAttributes(ta);
        dva.setVisible(true); // 启用数据值显示

        // 5 - 将数据值属性分配给图表
        diagram->setDataValueAttributes(dva);

        // 为所有数据集设置粗线条宽度
        for (int i = 0; i < m_model.rowCount(); ++i) {
            QPen pen(diagram->pen(i));
            pen.setWidth(17); // 设置线宽
            diagram->setPen(i, pen);
        }

        // 以不同方式绘制线条的一个部分
        // 1 - 获取数据集的笔并更改其样式
        // 这样可以保持线条的原始颜色
        QPen linePen(diagram->pen(1));
        linePen.setColor(Qt::yellow); // 更改颜色
        linePen.setWidth(7); // 更改线宽
        linePen.setStyle(Qt::DashLine); // 更改线条样式为虚线

        // 2 - 为线条中的一个部分更改笔并分配给图表
        diagram->setPen(m_model.index(1, 1, QModelIndex()), linePen);

        // 3 - 分配给图表
        m_chart.coordinatePlane()->replaceDiagram(diagram);
        m_chart.setGlobalLeadingRight(50); // 设置全局右侧边距

        // 设置布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart); // 添加图表到布局
        setLayout(l); // 设置窗口部件的布局
    }

private:
    Chart m_chart; // 图表对象
    QStandardItemModel m_model; // 数据模型
};

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建并显示ChartWidget窗口部件。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    ChartWidget w; // 创建图表窗口部件
    w.show(); // 显示窗口部件

    return app.exec(); // 运行应用程序事件循环
}

#include "main.moc"
