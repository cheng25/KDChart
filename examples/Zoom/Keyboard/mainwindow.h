/****************************************************************************
**
** 此文件是KD Chart库的缩放示例程序中的主窗口头文件
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB, 一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
** 此文件定义了MainWindow类，用于展示如何使用键盘进行图表缩放功能
**
****************************************************************************/

// 防止头文件重复包含
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// 包含UI头文件，由Qt Designer生成
#include "ui_mainwindow.h"

// 声明KDChart命名空间中的Widget类
namespace KDChart {
class Widget;
}

/**
 * @brief 主窗口类
 * @details 此类继承自QWidget和Ui::MainWindow，用于创建应用程序的主窗口界面
 *          包含图表部件和键盘缩放功能
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @details 初始化主窗口，创建图表部件并设置UI
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief 图表部件指针
     * @details 用于显示和操作图表的主要部件
     */
    KDChart::Widget *widget;

private:
};

#endif

// TODO: Qt5.15.2升级 检查MainWindow类的继承方式是否需要调整
// TODO: Qt5.15.2升级 确认KDChart::Widget类的API是否有变更
// TODO: C++17升级 考虑使用智能指针管理widget成员变量
// TODO: C++17升级 可以使用constexpr优化编译时常量
