/****************************************************************************
**
** 此文件是KD Chart图表库的一部分。
** 本文件是带滚动条缩放功能示例的程序入口文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001年KDAB集团公司(Klarälvdalens Datakonsult AB)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT开源许可
**
****************************************************************************/

#include "mainwindow.h"  // 包含主窗口类头文件
#include <qapplication.h> // 包含Qt应用程序类

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * @details 此函数初始化应用程序，创建主窗口并显示。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 初始化Qt应用程序

    MainWindow mainWindow; // 创建主窗口对象
    mainWindow.show();     // 显示主窗口

    return app.exec(); // 运行应用程序事件循环

// TODO: Qt5.15.2升级 检查QApplication在新版本中的使用方式
// TODO: Qt5.15.2升级 验证MainWindow与Qt5.15.2的兼容性
// TODO: C++17升级 考虑使用结构化绑定优化代码
// TODO: C++17升级 检查对C++17标准库的依赖
}
