/****************************************************************************
**
** 此文件是KD Chart库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议：MIT
**
****************************************************************************/

#include "mainwindow.h"  // 包含主窗口类定义
#include <qapplication.h>  // 包含Qt应用程序类

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * @details 创建Qt应用程序实例，初始化主窗口并显示，启动应用程序事件循环
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
} // TODO: Qt5.15.2升级 检查QApplication在新版本中的API变化
