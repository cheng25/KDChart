/****************************************************************************
**
** 此文件是KD Chart库甘特图API审查示例的主程序入口
** 原文注释: This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** 原文注释: SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 原文注释: SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QApplication>  // 包含Qt应用程序类

#include "mainwindow.h"  // 包含主窗口类定义

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // 创建Qt应用程序实例

    MainWindow mainWin;  // 创建主窗口实例
    mainWin.show();  // 显示主窗口

    return app.exec();  // 运行应用程序事件循环
}
