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
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出代码
 * 
 * 创建应用程序实例和主窗口，显示窗口并运行应用程序事件循环。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    MainWindow mainWindow; // 创建主窗口实例
    mainWindow.show(); // 显示主窗口

    return app.exec(); // 运行应用程序事件循环
}
