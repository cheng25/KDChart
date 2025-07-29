/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件包含BigDataset应用程序的入口点
// 该应用程序演示了如何使用KD Chart库处理和可视化大型数据集
// 负责初始化QApplication对象，创建并显示主窗口部件MainWidget
// 是程序的启动和控制中心

// 主窗口部件头文件
// TODO: Qt5.15.2升级 检查MainWidget类是否需要适配Qt5.15.2的API变更
#include "MainWidget.h"
// Qt应用程序类头文件
// TODO: Qt5.15.2升级 检查是否需要添加其他Qt模块头文件
#include <QApplication>

/**
 * @brief 应用程序入口点
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 初始化Qt应用程序，创建MainWidget实例并显示
 *          启动应用程序的事件循环，处理用户交互和系统事件
 * @todo C++17升级 考虑使用std::optional处理可能的初始化错误
 */
int main(int argc, char *argv[])
{
    // TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
    QApplication app(argc, argv);  // 初始化Qt应用程序
    MainWidget mainWidget;  // 创建主窗口部件实例
    mainWidget.show();  // 显示主窗口部件
    // TODO: Qt5.15.2升级 检查QApplication::exec()返回值处理是否需要变更
    return app.exec();  // 启动应用程序事件循环
}
