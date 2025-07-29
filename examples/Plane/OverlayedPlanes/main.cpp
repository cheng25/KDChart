/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

/**
 * @file main.cpp
 * @brief 程序入口文件
 * 本文件包含程序的主入口点main函数，负责初始化应用程序并创建主窗口
 */
#include "mainwindow.h"
#include <qapplication.h>

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 初始化Qt应用程序，创建并显示MainWindow窗口，启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    // TODO: Qt5.15.2升级 验证QApplication构造函数参数要求变更
    // TODO: C++17升级 使用constexpr优化常量表达式
    QApplication app(argc, argv);

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show();     // 显示主窗口

    return app.exec(); // 运行应用程序事件循环
}
