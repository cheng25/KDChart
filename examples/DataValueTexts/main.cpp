/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** This file is part of the KD Chart library.
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** 许可协议: MIT
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"  // 包含主窗口类定义
#include <qapplication.h>  // 包含Qt应用程序类

/**
 * @brief main 程序主入口
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * @details 创建Qt应用程序对象，实例化主窗口并显示，启动事件循环。
 * 这是整个应用程序的入口点，负责初始化Qt框架并启动用户界面。
 * 
 * Qt5.15.2升级说明：
 * - QApplication构造函数在Qt5.15.2中保持兼容
 * - exec()方法在Qt5.15.2中保持兼容
 * 
 * C++17升级说明：
 * - 此代码与C++17标准完全兼容
 * - 可以考虑使用std::clamp等C++17新特性优化代码（如需要）
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建Qt应用程序对象，初始化Qt框架

    MainWindow mainWindow;  // 实例化主窗口对象
    mainWindow.show();      // 显示主窗口

    return app.exec();  // 启动应用程序事件循环，等待用户交互
}
