/****************************************************************************
**
** 此文件是KD Chart库的缩放示例程序中的主窗口实现文件
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB, 一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
** 此文件实现了MainWindow类，包括UI设置和图表部件的创建
**
****************************************************************************/

// 包含主窗口头文件
#include "mainwindow.h"
// 包含缩放窗口部件头文件
#include "zoomwidget.h"
// 包含KDChart库的Widget类头文件
#include <KDChartWidget>
// 包含Qt调试类头文件
#include <QDebug>

// 使用KDChart命名空间以简化代码
using namespace KDChart;

/**
 * @brief MainWindow类构造函数
 * @param parent 父窗口指针，默认为nullptr
 * @details 初始化主窗口UI，创建图表布局和ZoomWidget部件
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置UI界面（由Qt Designer生成）
    setupUi(this);

    // 创建图表区域的水平布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    // 创建ZoomWidget实例，作为图表部件的容器
    widget = new ZoomWidget(chartFrame);
    // 设置图表部件的全局右边距为10像素
    widget->setGlobalLeadingRight(10);
    // 将图表部件添加到布局中
    chartLayout->addWidget(widget);
}

// TODO: Qt5.15.2升级 检查QHBoxLayout的API是否有变更
// TODO: Qt5.15.2升级 确认ZoomWidget类的实现是否需要调整以适应新Qt版本
// TODO: C++17升级 考虑使用std::make_unique管理动态分配的对象
// TODO: C++17升级 可以使用结构化绑定简化代码
