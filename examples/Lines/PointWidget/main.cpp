/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include <KDChartDataValueAttributes>
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
 * 创建一个点图示例，展示如何使用KD Chart库绘制数据点并配置其属性。
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
    vec2 << 2 << 7 << 1 << 2 << 1;

    // 设置数据集
    widget.setDataset(0, vec0, "vec0");
    widget.setDataset(1, vec1, "vec1");
    widget.setDataset(2, vec2, "vec2");

    // 不显示线条，只显示数据点
    widget.lineDiagram()->setPen(Qt::NoPen);

    // 配置数据值属性
    DataValueAttributes dva(widget.lineDiagram()->dataValueAttributes());
    MarkerAttributes ma(dva.markerAttributes());
    TextAttributes ta(dva.textAttributes());
    ma.setVisible(true); // 显示标记
    // 显示或不显示值
    ta.setVisible(false); // 不显示文本值
    dva.setTextAttributes(ta);
    dva.setMarkerAttributes(ma);
    dva.setVisible(true); // 启用数据值属性

    widget.lineDiagram()->setDataValueAttributes(dva); // 应用数据值属性
    widget.setGlobalLeading(10, 10, 10, 10); // 设置全局边距

    widget.show(); // 显示图表控件

    return app.exec(); // 运行应用程序事件循环
}
