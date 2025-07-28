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
#include <TableModel.h>

namespace KDChart {
class Chart;
class LineDiagram;
}

/**
 * @brief 高级线图示例主窗口类
 * 
 * 继承自QWidget和Ui::MainWindow，用于展示高级线图功能，包括：
 * - 线图类型切换（普通、堆叠、百分比）
 * - 数据值显示控制
 * - 数据点居中控制
 * - 3D模式切换
 * - 区域动画效果
 * - 区域高亮显示
 * - 平滑线条控制
 * - 坐标轴反转
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化主窗口，创建图表和数据模型，设置UI控件。
     */
    MainWindow(QWidget *parent = nullptr);

private:
    /**
     * @brief 事件过滤器
     * @param target 目标对象
     * @param event 事件
     * @return 如果事件被处理则返回true，否则返回false
     * 
     * 用于捕获图表部件的鼠标移动事件，以跟踪鼠标位置。
     */
    bool eventFilter(QObject *target, QEvent *event) override;

private slots:
    /**
     * @brief 线图类型下拉框选择变化槽函数
     * @param index 选中的索引
     * 
     * 根据选择的索引切换线图类型（普通、堆叠、百分比）。
     */
    void on_lineTypeCB_currentIndexChanged(int index);

    /**
     * @brief 数据值显示复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否显示数据值。
     */
    void on_paintValuesCB_toggled(bool checked);

    /**
     * @brief 数据点居中复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制数据点是否居中显示。
     */
    void on_centerDataPointsCB_toggled(bool checked);

    /**
     * @brief 3D模式复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否启用3D模式。
     */
    void on_threeDModeCB_toggled(bool checked);

    /**
     * @brief 深度滑块值变化槽函数
     * @param i 深度值
     * 
     * 调整3D模式下的深度。
     */
    void on_depthSB_valueChanged(int i);

    /**
     * @brief 区域动画复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否启用区域动画效果。
     */
    void on_animateAreasCB_toggled(bool checked);

    /**
     * @brief 区域高亮复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否启用区域高亮显示。
     */
    void on_highlightAreaCB_toggled(bool checked);

    /**
     * @brief 区域高亮滑块值变化槽函数
     * @param i 区域索引
     * 
     * 调整要高亮显示的区域索引。
     */
    void on_highlightAreaSB_valueChanged(int i);

    /**
     * @brief 平滑线条滑块值变化槽函数
     * @param value 平滑度值
     * 
     * 调整线条的平滑度。
     */
    void on_smoothLinesSDR_valueChanged(int value);

    /**
     * @brief 设置高亮区域
     * @param row 行索引
     * @param column 列索引
     * @param opacity 透明度
     * @param checked 是否启用
     * @param doUpdate 是否更新图表
     * 
     * 设置指定区域的高亮显示效果。
     */
    void setHighlightArea(int row, int column, int opacity,
                          bool checked, bool doUpdate);

    /**
     * @brief 跟踪区域复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否启用区域跟踪。
     */
    void on_trackAreasCB_toggled(bool checked);

    /**
     * @brief 跟踪区域滑块值变化槽函数
     * @param i 区域索引
     * 
     * 调整要跟踪的区域索引。
     */
    void on_trackAreasSB_valueChanged(int i);

    /**
     * @brief 设置跟踪区域
     * @param column 列索引
     * @param checked 是否启用
     * @param doUpdate 是否更新图表
     * 
     * 设置指定区域的跟踪效果。
     */
    void setTrackedArea(int column, bool checked, bool doUpdate);

    /**
     * @brief 定时器触发槽函数
     * 
     * 用于实现区域动画效果。
     */
    void slot_timerFired();

    /**
     * @brief 水平反转复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否反转水平坐标轴。
     */
    void on_reverseHorizontalCB_toggled(bool checked);

    /**
     * @brief 垂直反转复选框状态变化槽函数
     * @param checked 是否选中
     * 
     * 控制是否反转垂直坐标轴。
     */
    void on_reverseVerticalCB_toggled(bool checked);

private:
    KDChart::Chart *m_chart; // 图表对象
    KDChart::LineDiagram *m_lines; // 线图对象
    TableModel m_model; // 数据模型
    int m_curRow; // 当前行索引（用于动画）
    int m_curColumn; // 当前列索引（用于动画）
    int m_curOpacity; // 当前透明度（用于动画）
};

#endif /* MAINWINDOW_H */
