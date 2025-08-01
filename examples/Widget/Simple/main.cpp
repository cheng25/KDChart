/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的简单部件示例，展示如何使用KDChart::Widget快速创建图表
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include <KDChartWidget>    // 包含KDChart图表部件类
#include <QApplication>     // 包含Qt应用程序类

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return int 应用程序退出代码
 * @details 作为Widget简单示例的入口点，负责完成以下核心任务：
 *          1. 初始化Qt应用程序框架
 *          2. 创建KDChart::Widget图表部件实例
 *          3. 准备三个数据集（线性、二次方和三次方）
 *          4. 将数据集添加到图表部件
 *          5. 显示图表部件
 *          6. 启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    // 初始化Qt应用程序核心对象
    QApplication app(argc, argv);

    // 创建KDChart图表部件实例
    KDChart::Widget widget;
    // 设置图表部件尺寸为600x600像素
    widget.resize(600, 600);

    // 准备三个数据向量，分别存储线性、二次方和三次方数据
    QVector<qreal> vec0, vec1, vec2;

    // 线性数据：-5到5的连续整数
    vec0 << -5 << -4 << -3 << -2 << -1 << 0
         << 1 << 2 << 3 << 4 << 5;
    // 二次方数据：对应线性数据的平方值
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;
    // 三次方数据：对应线性数据的立方值
    vec2 << -125 << -64 << -27 << -8 << -1 << 0
         << 1 << 8 << 27 << 64 << 125;

    // 添加线性数据集到图表，名称为"Linear"
    widget.setDataset(0, vec0, "Linear");
    // 添加二次方数据集到图表，名称为"Quadratic"
    widget.setDataset(1, vec1, "Quadratic");
    // 添加三次方数据集到图表，名称为"Cubic"
    widget.setDataset(2, vec2, "Cubic");

    // 显示图表部件及其所有子组件
    widget.show();

    // 启动应用程序事件循环
    return app.exec();
}

// TODO: Qt5.15.2升级 验证KDChart::Widget在Qt5.15.2中的API兼容性
// TODO: Qt5.15.2升级 检查QApplication构造函数在Qt5.15.2中的参数要求
// TODO: C++17升级 考虑使用std::array替代QVector以提高性能
// TODO: C++17升级 可使用结构化绑定优化变量声明
