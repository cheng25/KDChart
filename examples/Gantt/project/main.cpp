/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
** 英文原文：This file is part of the KD Chart library.
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
** 英文原文：SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** 许可证：MIT
** 英文原文：SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QApplication>  // 包含Qt应用程序类

#include "mainwindow.h"  // 包含主窗口头文件

/**
 * @brief 应用程序入口函数
 * 
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return int 应用程序退出代码
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建Qt应用程序对象

    MainWindow mw;  // 实例化主窗口
    mw.resize(800, 600);  // 设置窗口大小为800x600
    mw.show();  // 显示主窗口

    return app.exec();  // 启动应用程序事件循环
}
