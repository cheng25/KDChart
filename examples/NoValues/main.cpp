/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的示例文件。
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
 * 创建应用程序实例和主窗口，展示无数据值的图表示例。
 * 此示例演示了KDChart如何处理没有数据的情况，仍然能够显示图表框架、坐标轴和图例。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口
    // 设置窗口标题
    mainWindow.setWindowTitle(QObject::tr("You can have a chart without having any data at all."));

    return app.exec(); // 运行应用程序事件循环
} // TODO: Qt5.15.2升级 检查QApplication在Qt5.15.2中的行为变化
// TODO: Qt5.15.2升级 验证QObject::tr在Qt5.15.2中的国际化行为
// TODO: C++17升级 考虑使用constexpr优化常量表达式
