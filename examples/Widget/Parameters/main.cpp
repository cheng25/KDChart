/****************************************************************************
**
** 此文件是KD Chart库的示例程序
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB, 一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
** 此文件演示了如何使用KDChart::Widget类创建带有参数配置的图表，包括3D效果、堆叠柱状图等
**
****************************************************************************/

// 包含KDChart库的柱状图类头文件
#include <KDChartBarDiagram>
// 包含KDChart库的3D柱状图属性类头文件
#include <KDChartThreeDBarAttributes>
// 包含KDChart库的Widget类头文件
#include <KDChartWidget>
// 包含Qt应用程序类头文件
#include <QApplication>
// 包含Qt画笔类头文件，用于设置线条样式
#include <QPen>

// 使用KDChart命名空间以简化代码
using namespace KDChart;

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 此函数负责初始化应用程序、创建图表部件、配置图表属性并显示窗口
 */
int main(int argc, char **argv)
{
    // 创建QApplication实例，初始化Qt应用程序
    QApplication app(argc, argv);

    // 创建图表部件实例
    Widget widget;
    // 设置图表部件大小为600x600像素
    widget.resize(600, 600);

    // 创建两个数据集向量
    QVector<qreal> vec0, vec1;

    // 初始化第一个数据集 (线性数据)
    vec0 << 5 << 4 << 3 << 2 << 1 << 0
         << 1 << 2 << 3 << 4 << 5;
    // 初始化第二个数据集 (二次方数据)
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;

    // 添加第一个数据集到图表，索引为0，名称为"vec0"
    widget.setDataset(0, vec0, "vec0");
    // 添加第二个数据集到图表，索引为1，名称为"vec1"
    widget.setDataset(1, vec1, "vec1");
    // 设置图表类型为堆叠柱状图
    widget.setType(Widget::Bar, Widget::Stacked);

    // 配置画笔，用于绘制柱状图的边框
    QPen pen;
    // 设置画笔宽度为2像素
    pen.setWidth(2);
    // 设置画笔颜色为深灰色
    pen.setColor(Qt::darkGray);
    // 获取柱状图对象并设置画笔
    widget.barDiagram()->setPen(pen);

    // 设置3D柱状图属性
    ThreeDBarAttributes td(widget.barDiagram()->threeDBarAttributes());
    // 设置3D深度为15
    td.setDepth(15);
    // 启用3D效果
    td.setEnabled(true);
    // 应用3D属性到柱状图
    widget.barDiagram()->setThreeDBarAttributes(td);
    // 设置图表顶部边距为20像素
    widget.setGlobalLeadingTop(20);
    // 显示图表部件
    widget.show();

    // 启动应用程序的事件循环并返回退出代码
    return app.exec();
}

// TODO: Qt5.15.2升级 检查KDChart::Widget类的API是否有变更
// TODO: Qt5.15.2升级 检查ThreeDBarAttributes相关API是否有变更
// TODO: C++17升级 考虑使用std::vector替代QVector
// TODO: C++17升级 可以使用结构化绑定简化代码
