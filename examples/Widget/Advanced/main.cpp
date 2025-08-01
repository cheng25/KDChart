/****************************************************************************
**
** 此文件是KD Chart库的示例程序入口文件
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB, 一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
** 此文件演示了如何使用KDChart::Widget类创建高级图表部件，包括添加数据集、页眉页脚和图例
**
****************************************************************************/

// 包含KDChart库的Widget类头文件，用于创建图表部件
#include <KDChartWidget>
// 包含Qt应用程序类头文件，用于初始化应用程序
#include <QApplication>

// 包含Qt调试类头文件，用于输出调试信息
#include <QDebug>
// 包含Qt向量类头文件，用于存储数据
#include <QVector>

// 包含自定义主窗口类头文件
#include "mainwindow.h"

// 使用KDChart命名空间以简化代码
using namespace KDChart;

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 此函数负责初始化应用程序、创建主窗口、准备示例数据并显示窗口
 */
int main(int argc, char **argv)
{
    // 创建QApplication实例，初始化Qt应用程序
    QApplication app(argc, argv);

    // 创建主窗口实例
    MainWindow window;

    // 设置主窗口大小为800x600像素
    window.resize(800, 600);

    // 创建线性函数数据集
    QVector<qreal> vec0;
    vec0.append(-5);
    vec0.append(-4);
    vec0.append(-3);
    vec0.append(-2);
    vec0.append(-1);
    vec0.append(0);
    vec0.append(1);
    vec0.append(2);
    vec0.append(3);
    vec0.append(4);
    vec0.append(5);
    // 将线性数据集添加到图表部件，索引为0，名称为"Linear"
    window.widget->setDataset(0, vec0, "Linear");

    // 创建二次方函数数据集 (y = x²)
    QVector<qreal> vec1;
    vec1.append(25);  // (-5)²
    vec1.append(16);  // (-4)²
    vec1.append(9);   // (-3)²
    vec1.append(4);   // (-2)²
    vec1.append(1);   // (-1)²
    vec1.append(0);   // 0²
    vec1.append(1);   // 1²
    vec1.append(4);   // 2²
    vec1.append(9);   // 3²
    vec1.append(16);  // 4²
    vec1.append(25);  // 5²
    // 将二次方数据集添加到图表部件，索引为1，名称为"Quadratic"
    window.widget->setDataset(1, vec1, "Quadratic");

    // 创建三次方函数数据集 (y = x³)
    QVector<qreal> vec2;
    vec2.append(-125);  // (-5)³
    vec2.append(-64);   // (-4)³
    vec2.append(-27);   // (-3)³
    vec2.append(-8);    // (-2)³
    vec2.append(-1);    // (-1)³
    vec2.append(0);     // 0³
    vec2.append(1);     // 1³
    vec2.append(8);     // 2³
    vec2.append(27);    // 3³
    vec2.append(64);    // 4³
    vec2.append(125);   // 5³
    // 将三次方数据集添加到图表部件，索引为2，名称为"Cubic"
    window.widget->setDataset(2, vec2, "Cubic");

    // 向图表部件添加页眉
    window.widget->addHeaderFooter("Header West", HeaderFooter::Header, Position::West);    // 西部页眉
    window.widget->addHeaderFooter("Header Center", HeaderFooter::Header, Position::Center);  // 中部页眉
    window.widget->addHeaderFooter("Header East", HeaderFooter::Header, Position::East);    // 东部页眉

    // 向图表部件添加页脚
    window.widget->addHeaderFooter("Footer West", HeaderFooter::Footer, Position::West);    // 西部页脚
    window.widget->addHeaderFooter("Footer Center", HeaderFooter::Footer, Position::Center);  // 中部页脚
    window.widget->addHeaderFooter("Footer East", HeaderFooter::Footer, Position::East);    // 东部页脚

    // 在图表东部添加图例
    window.widget->addLegend(Position::East);

    // 显示主窗口
    window.show();

    // 启动应用程序的事件循环并返回退出代码
    return app.exec();
}

// TODO: Qt5.15.2升级 检查KDChart::Widget类的API是否有变更
// TODO: Qt5.15.2升级 检查HeaderFooter相关API是否有变更
// TODO: C++17升级 考虑使用std::vector替代QVector
// TODO: C++17升级 考虑使用结构化绑定简化代码
