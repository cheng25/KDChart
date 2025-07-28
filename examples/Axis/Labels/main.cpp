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
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 程序入口点，创建应用程序对象和主窗口实例，并启动事件循环。
 * 本示例主要展示如何配置KD Chart库中的坐标轴标签、注释和自定义刻度等功能。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口

    return app.exec(); // 运行应用程序事件循环
}
