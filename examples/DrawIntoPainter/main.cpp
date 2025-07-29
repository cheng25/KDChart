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

#include "mainwindow.h"  // 包含MainWindow类定义
#include <qapplication.h>  // 包含QApplication类定义
// TODO: Qt5.15.2升级 考虑使用#include <QApplication>替代#include <qapplication.h>

/**
 * @brief main 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出码
 * @details 初始化Qt应用程序，创建并显示主窗口，启动应用程序事件循环
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建Qt应用程序对象

    MainWindow mainWindow;  // 创建主窗口实例
    mainWindow.show();  // 显示主窗口

    return app.exec();  // 启动应用程序事件循环
}
