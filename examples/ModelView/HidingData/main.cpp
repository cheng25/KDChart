/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
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
 * 创建应用程序实例和主窗口，显示主窗口并启动事件循环。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口

    return app.exec(); // 运行应用程序事件循环
}
// TODO: Qt5.15.2升级 检查QApplication在Qt5.15.2中的行为是否有变化
// TODO: C++17升级 考虑使用constexpr优化常量定义
