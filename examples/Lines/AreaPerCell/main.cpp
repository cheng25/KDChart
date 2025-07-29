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
#include <KDChartLineDiagram>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

/**
 * @brief 图表窗口部件类
 * 
 * 继承自QWidget，用于展示带有区域填充的线图示例。
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 创建一个ChartWidget实例，初始化数据模型和图表。
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型
        m_model.insertRows(0, 6, QModelIndex()); // 插入6行
        m_model.insertColumns(0, 1, QModelIndex()); // 插入1列

        // 填充数据
        m_model.setData(m_model.index(0, 0, QModelIndex()), 15);
        m_model.setData(m_model.index(1, 0, QModelIndex()), 11);
        m_model.setData(m_model.index(2, 0, QModelIndex()), 7);
        m_model.setData(m_model.index(3, 0, QModelIndex()), 3);
        m_model.setData(m_model.index(4, 0, QModelIndex()), -1);
        m_model.setData(m_model.index(5, 0, QModelIndex()), -5);

        // 创建线图
        auto *diagram = new LineDiagram;
        diagram->setModel(&m_model); // 设置数据模型

        // 替换图表的坐标系中的图表
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 为特定单元格绘制区域
        // 使用不同的画刷
        LineAttributes la3(diagram->lineAttributes(m_model.index(3, 0, QModelIndex())));
        la3.setDisplayArea(true); // 启用区域显示
        la3.setTransparency(150); // 设置透明度
        // 为索引1,3,4的单元格设置画刷和线条属性
        diagram->setBrush(m_model.index(1, 0, QModelIndex()), QBrush(Qt::green));
        diagram->setLineAttributes(m_model.index(1, 0, QModelIndex()), la3);
        diagram->setBrush(m_model.index(3, 0, QModelIndex()), QBrush(Qt::yellow));
        diagram->setLineAttributes(m_model.index(3, 0, QModelIndex()), la3);
        diagram->setBrush(m_model.index(4, 0, QModelIndex()), QBrush(Qt::red));
        diagram->setLineAttributes(m_model.index(4, 0, QModelIndex()), la3);

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
