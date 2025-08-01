/****************************************************************************
**
** 此文件是KD Chart库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议：MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"  // 包含UI表单生成的头文件
#include <TableModel.h>      // 包含表格数据模型

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
class LineAttributes;
class CartesianAxis;
class CartesianCoordinatePlane;
}

/**
 * @brief 主窗口类
 * @details 继承自QWidget和Ui::MainWindow，用于展示共享横坐标的多个图表
 *          该示例演示了如何在一个图表窗口中显示多个图表，并共享同一个横坐标
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件指针
     * @details 初始化主窗口，创建图表对象、数据模型和线图，设置共享横坐标
     */
    MainWindow(QWidget *parent = nullptr);

private:
    KDChart::Chart *m_chart;                      // 图表对象
    TableModel m_model;                           // 第一个数据模型
    TableModel m_model2;                          // 第二个数据模型
    KDChart::DatasetProxyModel *m_datasetProxy;   // 数据集代理模型
    KDChart::LineDiagram *m_lines;                // 第一个线图
    KDChart::LineDiagram *m_lines2;               // 第二个线图
    KDChart::CartesianCoordinatePlane *plane2;    // 第二个坐标平面
    // mutable KDChart::CartesianAxis xAxis;
    // mutable KDChart::CartesianAxis yAxis;
};

#endif /* MAINWINDOW_H */
