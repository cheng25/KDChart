/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的饼图示例文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/

#include <KDChartPieDiagram>
#include <KDChartPolarCoordinatePlane>
#include <KDChartWidget>
#include <QApplication>
#include <QPen>

using namespace KDChart;

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建一个简单的饼图示例，展示KDChart库的基本使用方法。
 * 该示例创建了三个数据集，并将它们显示为饼图，同时演示了如何自定义饼图的外观。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    Widget widget; // 创建KDChart控件实例
    widget.resize(600, 600); // 设置控件大小
    widget.setGlobalLeading(5, 5, 5, 5); // 设置全局边距

    QVector<qreal> vec0, vec1, vec2; // 定义三个数据向量

    // 初始化数据向量
    vec0 << 1 << -4 << -3 << -2 << -1 << 0
         << 1 << 2 << 3 << 4 << 5;
    vec1 << 2 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;
    vec2 << 3 << -64 << -27 << -8 << -1 << 0
         << 1 << 8 << 27 << 64 << 125;

    // 添加数据集到图表控件
    widget.setDataset(0, vec0, "v0");
    widget.setDataset(1, vec1, "v1");
    widget.setDataset(2, vec2, "v2");
    widget.setType(Widget::Pie); // 设置图表类型为饼图

    // 配置饼图的画笔样式
    QPen piePen;
    piePen.setWidth(3); // 设置线宽
    piePen.setColor(Qt::white); // 设置颜色
    widget.pieDiagram()->setPen(2, piePen); // 为第三个扇区设置画笔
    // 设置极坐标平面的起始位置（90度对应顶部）
    (( PolarCoordinatePlane * )widget.coordinatePlane())->setStartPosition(90);

    widget.show(); // 显示图表控件

    return app.exec(); // 运行应用程序事件循环
} // TODO: Qt5.15.2升级 检查KDChart::Widget在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QPen在Qt5.15.2中的行为变化
// TODO: C++17升级 考虑使用结构化绑定优化变量声明
// TODO: C++17升级 考虑使用std::optional处理可能的空指针
