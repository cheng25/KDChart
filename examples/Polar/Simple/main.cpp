/****************************************************************************
**
** 此文件是KD Chart库的一部分。
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/
// TODO: Qt5.15.2升级 检查头文件包含是否需要调整

#include <KDChartPolarDiagram> // 极坐标图表类
#include <KDChartWidget>       // 图表窗口部件类
#include <QApplication>        // Qt应用程序类
// TODO: Qt5.15.2升级 检查KDChart API是否有变更

using namespace KDChart;

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 应用程序的入口点，创建应用程序对象、图表窗口并显示极坐标图表。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象

    Widget widget; // 创建图表窗口部件
    widget.resize(600, 600); // 设置窗口大小为600x600像素

    // 创建三个数据向量
    QVector<qreal> vec0, vec1, vec2;

    // 填充数据
    // vec0: 线性数据 -5到5
    vec0 << -5 << -4 << -3 << -2 << -1 << 0
         << 1 << 2 << 3 << 4 << 5;
    // vec1: 平方数据 (-5)^2到5^2
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;
    // vec2: 立方数据 (-5)^3到5^3
    vec2 << -125 << -64 << -27 << -8 << -1 << 0
         << 1 << 8 << 27 << 64 << 125;

    // 设置数据集
    widget.setDataset(0, vec0, "v0"); // 设置第一个数据集，名称为"v0"
    widget.setDataset(1, vec1, "v1"); // 设置第二个数据集，名称为"v1"
    widget.setDataset(2, vec2, "v2"); // 设置第三个数据集，名称为"v2"
    widget.setType(Widget::Polar); // 设置图表类型为极坐标
    widget.setGlobalLeading(5, 5, 5, 5); // 设置图表全局边距

    widget.show(); // 显示图表窗口

    return app.exec(); // 进入应用程序事件循环
}
