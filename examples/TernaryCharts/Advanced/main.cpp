/****************************************************************************
**
** 此文件是KD Chart库的示例程序的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议: MIT
**
****************************************************************************/

// 包含Qt应用程序类的头文件
#include <QApplication>

// 包含主窗口类的头文件
#include "mainwindow.h"

/**
 * @brief 程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数字符串数组
 * @return 程序退出码
 */
int main(int argc, char **argv)
{
    // 创建Qt应用程序实例
    QApplication app(argc, argv);

    // 创建主窗口实例
    MainWindow mainWindow;
    // 显示主窗口
    mainWindow.show();

    // 启动应用程序事件循环
    return app.exec();

    // TODO: Qt5.15.2升级 检查QApplication API是否有变更
    // TODO: C++17升级 考虑使用结构化绑定或其他C++17特性优化代码
}
