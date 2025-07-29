/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartWidget>
#include <QApplication>

using namespace KDChart;

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建一个简单的线图示例，展示如何使用KD Chart库绘制多条线并配置其属性。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    Widget widget; // 创建KD Chart控件
    widget.resize(600, 600); // 设置控件大小

    QVector<qreal> vec0, vec1, vec2; // 数据向量

    // 填充数据
    vec0 << 5 << 1 << 3 << 4 << 1;
    vec1 << 3 << 6 << 2 << 4 << 8;
    vec2 << 0 << 7 << 1 << 2 << 1;

    // 设置数据集
    widget.setDataset(0, vec0, "vec0");
    widget.setDataset(1, vec1, "vec1");
    widget.setDataset(2, vec2, "vec2");
    widget.setSubType(Widget::Percent); // 设置图表子类型为百分比

    // 配置画笔和线条
    // 为第1列设置点线
    QPen pen;
    QBrush brush;
    pen.setWidth(3); // 设置线宽
    pen.setStyle(Qt::DotLine); // 设置线条样式为点线
    pen.setColor(Qt::yellow); // 设置线条颜色
    brush.setColor(Qt::yellow); // 设置填充颜色
    // 调用图表并设置新的画笔和画刷
    widget.lineDiagram()->setPen(1, pen);
    widget.lineDiagram()->setBrush(1, brush);

    // 设置图例
    auto *legend = new Legend(&widget);
    legend->setPosition(Position::East); // 设置图例位置在东侧
    legend->setAlignment(Qt::AlignCenter); // 设置对齐方式为居中
    legend->setOrientation(Qt::Vertical); // 设置图例方向为垂直
    legend->setLegendStyle(Legend::LinesOnly); // 设置图例样式为仅显示线条
    legend->setShowLines(true); // 显示线条
    widget.addLegend(legend); // 将图例添加到图表

    // 设置属性
    // 显示区域
    LineAttributes la(widget.lineDiagram()->lineAttributes());
    la.setDisplayArea(true); // 启用区域显示
    la.setTransparency(25); // 设置透明度
    widget.lineDiagram()->setLineAttributes(la); // 应用线条属性

    widget.show(); // 显示图表控件

    return app.exec(); // 运行应用程序事件循环
}
