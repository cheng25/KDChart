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

#include <KDChartAbstractCoordinatePlane>  // 包含抽象坐标平面类
#include <KDChartChart>                    // 包含图表类
#include <KDChartGridAttributes>           // 包含网格属性类
#include <KDChartLineDiagram>              // 包含线图类

#include <QDebug>                          // 包含调试输出类
#include <QPainter>                        // 包含绘图类

using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口部件指针
 * @details 初始化主窗口，创建图表对象、数据模型和线图，设置共享横坐标
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置UI界面
    setupUi(this);

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    // 创建图表对象
    m_chart = new Chart();
    // 将图表添加到布局
    chartLayout->addWidget(m_chart);

    // 使用新的布局系统
    m_chart->setUseNewLayoutSystem(true);

    // 从CSV文件加载数据
    m_model.loadFromCSV(" :/data");
    m_model2.loadFromCSV(" :/data2");

    // 设置第一个线图
    m_lines = new LineDiagram();
    m_lines->setModel(&m_model);

    // 设置第二个线图
    m_lines2 = new LineDiagram();
    m_lines2->setModel(&m_model2);

    // 获取默认的坐标平面
    auto *plane1 = static_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane());

    // 创建第二个坐标平面
    plane2 = new CartesianCoordinatePlane(m_chart);

    // 创建坐标轴
    auto *xAxis = new CartesianAxis(m_lines);    // 共享的X轴
    auto *yAxis = new CartesianAxis(m_lines);    // 第一个Y轴
    auto *yAxis2 = new CartesianAxis(m_lines2);  // 第二个Y轴

    // 设置自定义刻度
    QList<qreal> ticks;
    ticks.append(5);
    ticks.append(10);
    ticks.append(15);
    xAxis->setCustomTicks(ticks);

    // 设置刻度注释
    QMultiMap<qreal, QString> annotations;
    annotations.insert(5, "Five");    // 5 → Five
    annotations.insert(10, "Ten");    // 10 → Ten
    annotations.insert(15, "Fifteen");  // 15 → Fifteen
    xAxis->setAnnotations(annotations);

    // 设置坐标轴位置
    xAxis->setPosition(KDChart::CartesianAxis::Top);    // X轴在顶部
    yAxis->setPosition(KDChart::CartesianAxis::Left);   // 第一个Y轴在左侧
    yAxis2->setPosition(KDChart::CartesianAxis::Right);  // 第二个Y轴在右侧

    // 为线图添加坐标轴
    m_lines->addAxis(xAxis);  // 第一个线图添加X轴
    m_lines->addAxis(yAxis);  // 第一个线图添加Y轴
    m_lines2->addAxis(xAxis); // 第二个线图共享X轴
    m_lines2->addAxis(yAxis2); // 第二个线图添加Y轴

    // 设置第一个坐标平面的图表
    m_chart->coordinatePlane()->replaceDiagram(m_lines);
    // 设置图表边距
    m_chart->setGlobalLeading(20, 20, 20, 20);
    // 设置第二个坐标平面的图表
    plane2->replaceDiagram(m_lines2);
    // 将第二个坐标平面添加到图表
    m_chart->addCoordinatePlane(plane2);

    // 启用两个平面中的注释
    KDChart::GridAttributes grid1 = plane1->gridAttributes(Qt::Horizontal);
    grid1.setLinesOnAnnotations(true);
    plane1->setGridAttributes(Qt::Horizontal, grid1);
    KDChart::GridAttributes grid2 = plane2->gridAttributes(Qt::Horizontal);
    grid2.setLinesOnAnnotations(true);
    plane2->setGridAttributes(Qt::Horizontal, grid2);
    // 更新图表
    m_chart->update();
} // TODO: Qt5.15.2升级 检查KDChart API是否有变更
// TODO: C++17升级 考虑使用智能指针管理动态内存
