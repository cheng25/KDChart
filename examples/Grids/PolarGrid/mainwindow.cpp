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

#include <KDChartAbstractCoordinatePlane>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartGridAttributes>
#include <KDChartLegend>
#include <KDChartMarkerAttributes>
#include <KDChartPolarDiagram>
#include <KDChartTextAttributes>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

/**
 * @brief 构造函数
 * @param parent 父窗口部件
 * 
 * 初始化主窗口，设置UI组件，实例化KD Chart类，配置图表布局，并填充数据模型。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件

    // 实例化KD Chart类
    initKDChartClasses();

    // 将KDChart::Chart插入Qt的布局中
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart->setGlobalLeading(2, 2, 2, 2); // 设置全局边距
    chartLayout->addWidget(m_chart);

    // 连接KD Chart类
    wireUpKDChartClasses();

    // 初始化ItemModel并填充一些数据
    m_model.insertRows(0, 40);  // 插入40行
    m_model.insertColumns(0, 5); // 插入5列
    setItemModelData();
}

/**
 * @brief 初始化KDChart相关类
 * 
 * 创建图表、极坐标图和极坐标平面对象。
 */
void MainWindow::initKDChartClasses()
{
    m_chart = new Chart();              // 创建图表对象
    m_diagram = new PolarDiagram();     // 创建极坐标图对象
    m_polarPlane = new PolarCoordinatePlane(); // 创建极坐标平面对象
}

/**
 * @brief 连接KDChart相关类
 * 
 * 将极坐标平面设置到图表中，将数据模型设置到极坐标图中，并将极坐标图添加到坐标平面。
 */
void MainWindow::wireUpKDChartClasses()
{
    m_chart->replaceCoordinatePlane(m_polarPlane); // 替换图表的坐标平面
    // 注意：在将图表添加到坐标平面之前，我们需要为图表设置一个有效的数据模型
    m_diagram->setModel(&m_model); // 设置数据模型
    m_chart->coordinatePlane()->replaceDiagram(m_diagram); // 替换坐标平面的图表
}

/**
 * @brief 设置数据模型数据
 * 
 * 手动填充数据模型的单元格。
 */
void MainWindow::setItemModelData()
{
    // 这里我们不从资源文件读取数据，而是手动填充单元格
    int value = 0;
    for (int column = 0; column < m_model.columnCount(); ++column) {
        for (int row = 0; row < m_model.rowCount(); ++row) {
            QModelIndex index = m_model.index(row, column);
            m_model.setData(index, QVariant(value++));
        }
    }
}

/**
 * @brief 环形网格起始位置微调框值改变槽函数
 * @param pos 新的起始位置值
 * 
 * 同步滑动条的值，并更新极坐标平面的起始位置。
 */
void MainWindow::on_startPositionSB_valueChanged(double pos)
{
    const int intValue = static_cast<int>(pos);
    startPositionSL->blockSignals(true); // 阻塞滑动条的信号
    startPositionSL->setValue(intValue); // 设置滑动条的值
    startPositionSL->blockSignals(false); // 解除信号阻塞
    // 注意：这里我们使用全局getter方法，它会自动回退以返回默认设置
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos);
    update(); // 更新窗口
}

/**
 * @brief 环形网格起始位置滑动条值改变槽函数
 * @param pos 新的起始位置值
 * 
 * 同步微调框的值，并更新极坐标平面的起始位置。
 */
void MainWindow::on_startPositionSL_valueChanged(int pos)
{
    auto qrealValue = static_cast<qreal>(pos);
    startPositionSB->blockSignals(true); // 阻塞微调框的信号
    startPositionSB->setValue(qrealValue); // 设置微调框的值
    startPositionSB->blockSignals(false); // 解除信号阻塞
    // 注意：这里我们使用全局getter方法，它会自动回退以返回默认设置
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos);
    update(); // 更新窗口
}

/**
 * @brief 环形网格复选框状态改变槽函数
 * @param toggle 复选框状态
 * 
 * 设置环形网格的可见性。
 */
void MainWindow::on_circularGridCB_toggled(bool toggle)
{
    GridAttributes attrs(m_polarPlane->gridAttributes(true)); // 获取环形网格属性
    attrs.setGridVisible(toggle); // 设置网格可见性
    m_polarPlane->setGridAttributes(true, attrs); // 应用网格属性
    update(); // 更新窗口
}

/**
 * @brief 径向网格复选框状态改变槽函数
 * @param toggle 复选框状态
 * 
 * 设置径向网格的可见性。
 */
void MainWindow::on_sagittalGridCB_toggled(bool toggle)
{
    GridAttributes attrs(m_polarPlane->gridAttributes(false)); // 获取径向网格属性
    attrs.setGridVisible(toggle); // 设置网格可见性
    m_polarPlane->setGridAttributes(false, attrs); // 应用网格属性
    update(); // 更新窗口
}

/* 计划在未来版本中实现:
void MainWindow::on_circularAxisCB_toggled( bool toggle )
{
    Q_UNUSED( toggle );
    update();
}
void MainWindow::on_sagittalAxisCB_toggled( bool toggle )
{
    Q_UNUSED( toggle );
    update();
}
*/
