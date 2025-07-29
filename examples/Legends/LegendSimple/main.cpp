/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <KDChartBarDiagram>
#include <KDChartLegend>
#include <KDChartPosition>
#include <KDChartWidget>
#include <QApplication>

using namespace KDChart;

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * 
 * 创建应用程序实例和图表窗口，配置柱状图数据，添加图例，并显示窗口。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    Widget widget; // 创建KDChart窗口部件
    widget.resize(600, 600); // 设置窗口大小

    QVector<qreal> vec0, vec1, vec2; // 数据向量

    // 填充数据
    vec0 << -5 << -4 << -3 << -2 << -1 << 0
         << 1 << 2 << 3 << 4 << 5;
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;
    vec2 << -125 << -64 << -27 << -8 << -1 << 0
         << 1 << 8 << 27 << 64 << 125;

    // 设置数据集
    widget.setDataset(0, vec0, "v0");
    widget.setDataset(1, vec1, "v1");
    widget.setDataset(2, vec2, "v2");
    widget.setType(Widget::Bar); // 设置图表类型为柱状图
    widget.addLegend(Position::North); // 在顶部添加图例
    widget.legend()->setOrientation(Qt::Horizontal); // 设置图例方向为水平
    widget.legend()->setTitleText("柱状图图例"); // 设置图例标题
    widget.legend()->setText(0, "向量 1"); // 设置第一个数据集的图例文本
    widget.legend()->setText(1, "向量 2"); // 设置第二个数据集的图例文本
    widget.legend()->setText(2, "向量 3"); // 设置第三个数据集的图例文本
    widget.legend()->setShowLines(true); // 显示图例中的连接线

    widget.show(); // 显示窗口

    return app.exec(); // 运行应用程序事件循环
}
