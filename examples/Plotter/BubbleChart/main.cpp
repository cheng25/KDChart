/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件是BubbleChart项目的入口文件
// 负责创建应用程序实例和主窗口，并启动事件循环
// 气泡图示例展示了如何使用KD Chart库创建气泡图图表
// 支持数据点的可视化展示，其中气泡的大小代表第三个维度的数据

// 主窗口类头文件
#include "mainwindow.h"
// Qt应用程序类头文件
// TODO: Qt5.15.2升级 检查是否需要添加其他Qt模块头文件
#include <qapplication.h>

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数字符串数组
 * @return 应用程序退出代码
 * @details 创建QApplication实例和MainWindow主窗口
 * 显示主窗口并运行应用程序事件循环
 * TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
 * TODO: C++17升级 考虑使用std::optional处理可能的错误情况
 */
int main(int argc, char **argv)
{
    // TODO: Qt5.15.2升级 检查QApplication初始化是否需要适配新版本
    // 创建Qt应用程序实例
    QApplication app(argc, argv);

    // 创建主窗口实例
    MainWindow mainWindow;
    // 显示主窗口
    mainWindow.show();

    // 运行应用程序事件循环
    // TODO: Qt5.15.2升级 检查QApplication::exec()返回值处理是否需要变更
    return app.exec();
}
