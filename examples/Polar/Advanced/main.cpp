/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QApplication>
// 包含Qt应用程序框架的头文件

#include "mainwindow.h"
// 包含自定义主窗口类的头文件

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 初始化Qt应用程序，创建并显示主窗口，启动事件循环
 */
int main(int argc, char **argv)
{
    // 创建Qt应用程序对象，管理应用程序的控制流和主要设置
    QApplication app(argc, argv);

    // 创建主窗口对象
    MainWindow mainWindow;
    // 显示主窗口
    mainWindow.show();

    // 进入应用程序事件循环，等待用户交互并处理事件
    return app.exec();
}
