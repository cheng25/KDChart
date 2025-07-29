/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，展示坐标轴标签配置的主入口文件
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "mainwindow.h"
#include <qapplication.h>

/**
 * @brief 应用程序主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 初始化Qt应用程序，创建并显示主窗口，启动事件循环
 * 核心功能：作为程序入口点，协调UI组件和图表功能的初始化
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 初始化Qt应用程序核心对象

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口界面

    // TODO: Qt5.15.2升级 验证QApplication构造函数在Qt5.15.2中的参数要求变更
    // TODO: C++17升级 使用[[maybe_unused]]标记未使用的命令行参数
    return app.exec(); // 启动应用程序事件循环
}
