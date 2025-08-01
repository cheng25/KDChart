/****************************************************************************
**
** 此文件是KD Chart图表库的一部分。
** 本示例演示了图表的缩放功能，特别是通过键盘进行缩放和移动操作。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001年KDAB集团公司(Klarälvdalens Datakonsult AB)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT开源许可
**
****************************************************************************/

#include <KDChartWidget> // 包含KDChart图表部件类
#include <QApplication>  // 包含Qt应用程序类

#include <QDebug>        // 包含Qt调试输出功能
#include <QVector>       // 包含Qt动态数组类

#include "mainwindow.h"  // 包含自定义主窗口类头文件

using namespace KDChart; // 使用KDChart命名空间

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * @details 此函数初始化应用程序，创建主窗口，并设置图表数据和属性。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 初始化Qt应用程序

    MainWindow window; // 创建主窗口对象

    window.resize(800, 600); // 设置窗口大小为800x600像素

    QVector<qreal> vec0; // 创建第一个数据集向量
    vec0.append(-5);     // 添加数据点: -5
    vec0.append(-4);     // 添加数据点: -4
    vec0.append(-3);     // 添加数据点: -3
    vec0.append(-2);     // 添加数据点: -2
    vec0.append(-1);     // 添加数据点: -1
    vec0.append(0);      // 添加数据点: 0
    vec0.append(1);      // 添加数据点: 1
    vec0.append(2);      // 添加数据点: 2
    vec0.append(3);      // 添加数据点: 3
    vec0.append(4);      // 添加数据点: 4
    vec0.append(5);      // 添加数据点: 5
    window.widget->setDataset(0, vec0); // 将第一个数据集设置到图表部件中

    QVector<qreal> vec1; // 创建第二个数据集向量
    vec1.append(25);     // 添加数据点: 25
    vec1.append(16);     // 添加数据点: 16
    vec1.append(9);      // 添加数据点: 9
    vec1.append(4);      // 添加数据点: 4
    vec1.append(1);      // 添加数据点: 1
    vec1.append(0);      // 添加数据点: 0
    vec1.append(1);      // 添加数据点: 1
    vec1.append(4);      // 添加数据点: 4
    vec1.append(9);      // 添加数据点: 9
    vec1.append(16);     // 添加数据点: 16
    vec1.append(25);     // 添加数据点: 25
    window.widget->setDataset(1, vec1); // 将第二个数据集设置到图表部件中

    QVector<qreal> vec2; // 创建第三个数据集向量
    vec2.append(-125);   // 添加数据点: -125
    vec2.append(-64);    // 添加数据点: -64
    vec2.append(-27);    // 添加数据点: -27
    vec2.append(-8);     // 添加数据点: -8
    vec2.append(-1);     // 添加数据点: -1
    vec2.append(0);      // 添加数据点: 0
    vec2.append(1);      // 添加数据点: 1
    vec2.append(8);      // 添加数据点: 8
    vec2.append(27);     // 添加数据点: 27
    vec2.append(64);     // 添加数据点: 64
    vec2.append(125);    // 添加数据点: 125
    window.widget->setDataset(2, vec2); // 将第三个数据集设置到图表部件中

    // 添加标题到图表顶部
    window.widget->addHeaderFooter("Zoom Example",
                                   HeaderFooter::Header, Position::North);
    // 添加缩放操作提示到图表上方
    window.widget->addHeaderFooter("zoom in/out: PageDn / PageUp",
                                   HeaderFooter::Footer, Position::North);
    // 添加平移操作提示到图表下方
    window.widget->addHeaderFooter("pan around: Left / Right / Up / Down",
                                   HeaderFooter::Footer, Position::South);

    // 在图表右侧添加图例
    window.widget->addLegend(Position::East);

    // window.widget->setType( Widget::Polar ); // 注释: 设置图表类型为极坐标图

    window.show(); // 显示主窗口

    return app.exec(); // 运行应用程序事件循环

// TODO: Qt5.15.2升级 检查QApplication和QVector在新版本中的使用方式
// TODO: Qt5.15.2升级 验证KDChart::Widget与Qt5.15.2的兼容性
// TODO: C++17升级 考虑使用std::vector替代QVector以利用C++17特性
// TODO: C++17升级 使用结构化绑定和if constexpr优化代码结构
}
