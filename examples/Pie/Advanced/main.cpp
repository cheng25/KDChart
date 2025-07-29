/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的高级饼图示例的应用程序入口文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/

#include "mainwindow.h"
#include <qapplication.h>

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建应用程序实例和主窗口，启动高级饼图示例。
 * 该示例演示了KDChart库的高级饼图功能，包括扇区爆炸效果、动画和3D视图。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口

    return app.exec(); // 运行应用程序事件循环
} // TODO: Qt5.15.2升级 检查QApplication在Qt5.15.2中的行为变化
// TODO: C++17升级 考虑使用constexpr优化常量表达式
