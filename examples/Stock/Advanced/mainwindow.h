/****************************************************************************
**
** 此文件是KD Chart库的示例程序的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// 包含UI头文件
#include "ui_mainwindow.h"
// 包含KDChart图表类头文件
#include <KDChartChart>
// 包含KDChart股票图表类头文件
#include <KDChartStockDiagram>
// 包含KDChart 3D柱状图属性类头文件
#include <KDChartThreeDBarAttributes>
// 包含表格模型头文件
#include <TableModel.h>

/**
 * @brief 主窗口类，用于展示股票图表
 * 该类继承自QWidget和Ui::MainWindow，实现了股票图表的展示和交互功能
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 3D属性切换槽函数
     * @param checked 是否启用3D效果
     */
    void on_threeDProperties_toggled(bool checked);

    /**
     * @brief 透视深度值变化槽函数
     * @param value 透视深度值
     */
    void on_perspectiveDepth_valueChanged(int value);

    /**
     * @brief 透视角度值变化槽函数
     * @param value 透视角度值
     */
    void on_perspectiveAngle_valueChanged(int value);

    /**
     * @brief 阴影颜色切换槽函数
     * @param checked 是否使用阴影颜色
     */
    void on_useShadowColors_toggled(bool checked);

    /**
     * @brief 股票类型下拉框变化槽函数
     * @param index 选中项的索引
     */
    void on_stockTypeCB_currentIndexChanged(int index);

    /**
     * @brief 选择颜色槽函数
     */
    void chooseColor();

private:
    /**
     * @brief 初始化股票图表的值
     */
    void initValues();

    /**
     * @brief 应用颜色到图表
     * @param color 要应用的颜色
     */
    void applyColor(const QColor &color);

    // 图表对象指针
    KDChart::Chart *m_chart;
    // HLC(最高价-最低价-收盘价)数据模型
    TableModel m_HLCModel;
    // OHLC(开盘价-最高价-最低价-收盘价)数据模型
    TableModel m_OHLCModel;

    // 股票图表对象
    KDChart::StockDiagram m_diagram;
    // 3D柱状图属性对象
    KDChart::ThreeDBarAttributes m_threeDBarAttributes;
};

#endif // MAINWINDOW_H
