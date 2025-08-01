﻿/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例程序，高级柱状图示例的入口文件
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "mainwindow.h"  // 包含主窗口类的头文件
#include <qapplication.h>  // 包含Qt应用程序类的头文件

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 程序入口点，创建应用程序对象和主窗口实例，并运行事件循环
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象，初始化Qt框架

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口

    return app.exec(); // 运行应用程序事件循环，处理用户输入和系统事件
}
