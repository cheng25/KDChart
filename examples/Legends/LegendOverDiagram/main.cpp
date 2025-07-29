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
 * 创建应用程序实例和图表窗口，配置柱状图数据，添加并精确定位图例，并显示窗口。
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

    // 创建图例
    auto *legend = new Legend(widget.diagram(), &widget);
    legend->setOrientation(Qt::Horizontal); // 设置图例方向为水平
    legend->setTitleText("柱状图图例"); // 设置图例标题
    legend->setText(0, "向量 1"); // 设置第一个数据集的图例文本
    legend->setText(1, "向量 2"); // 设置第二个数据集的图例文本
    legend->setText(2, "向量 3"); // 设置第三个数据集的图例文本
    legend->setShowLines(true); // 显示图例中的连接线

    // 以下代码将图例精确对齐到图表坐标平面的右上角
    //
    // 注意: 这里不使用 Legend::setAlignment，因为当使用 setRelativePosition 时，
    //       对齐是在 KDChart::RelativePosition 而不是 KDChart::Legend 上设置的
    KDChart::RelativePosition relativePosition; // 创建相对位置对象
    relativePosition.setReferenceArea(widget.coordinatePlane()); // 设置参考区域为图表坐标平面
    relativePosition.setReferencePosition(Position::NorthEast); // 设置参考位置为右上角
    relativePosition.setAlignment(Qt::AlignTop | Qt::AlignRight); // 设置对齐方式为右上对齐
    relativePosition.setHorizontalPadding(KDChart::Measure(-1.0, KDChartEnums::MeasureCalculationModeAbsolute)); // 设置水平边距
    relativePosition.setVerticalPadding(KDChart::Measure(1.0, KDChartEnums::MeasureCalculationModeAbsolute)); // 设置垂直边距
    legend->setFloatingPosition(relativePosition); // 设置图例的浮动位置

    widget.addLegend(legend); // 将图例添加到图表

    widget.show(); // 显示窗口

    return app.exec(); // 运行应用程序事件循环
}
