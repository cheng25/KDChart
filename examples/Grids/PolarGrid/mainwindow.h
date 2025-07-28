/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QStandardItemModel>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class PolarDiagram;
class PolarCoordinatePlane;
}

/**
 * @brief 主窗口类
 * 
 * 该类继承自QWidget和Ui::MainWindow，用于展示极坐标网格配置的示例。
 * 包含图表的初始化、数据模型设置、网格属性配置等功能。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化主窗口，设置UI组件和图表相关类。
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    // 环形网格的起始位置
    void on_startPositionSB_valueChanged(double pos);
    void on_startPositionSL_valueChanged(int pos);

    // 显示网格
    void on_circularGridCB_toggled(bool toggle);
    void on_sagittalGridCB_toggled(bool toggle);

    /* 计划在未来版本中实现:
    // 显示轴
    void on_circularAxisCB_toggled( bool toggle );
    void on_sagittalAxisCB_toggled( bool toggle );
    */

private:
    /**
     * @brief 初始化KDChart相关类
     * 
     * 创建并配置图表、极坐标图和极坐标平面。
     */
    void initKDChartClasses();

    /**
     * @brief 连接KDChart相关类
     * 
     * 建立图表组件之间的连接关系。
     */
    void wireUpKDChartClasses();

    /**
     * @brief 设置数据模型数据
     * 
     * 为数据模型填充示例数据。
     */
    void setItemModelData();

    // 这里我们不使用特殊的TableModel，而是使用Qt自带的标准模型
    QStandardItemModel m_model;          // 标准数据模型

    KDChart::Chart *m_chart;              // 图表对象
    KDChart::PolarDiagram *m_diagram;     // 极坐标图对象
    KDChart::PolarCoordinatePlane *m_polarPlane; // 极坐标平面对象

    int m_currentFactor = 0;              // 当前因子
    int m_currentDirection = 1;           // 当前方向
    int m_currentSlice = 0;               // 当前切片
};

#endif /* MAINWINDOW_H */
