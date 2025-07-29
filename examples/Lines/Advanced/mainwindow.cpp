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

#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>

#include <QMouseEvent>
#include <QTimer>

using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口部件
 * 
 * 初始化UI，创建图表和数据模型，设置坐标轴，配置定时器和事件过滤器。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI控件

    m_curColumn = -1; // 初始化当前列索引
    m_curOpacity = 0; // 初始化当前透明度

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart(); // 创建图表对象
    chartLayout->addWidget(m_chart); // 将图表添加到布局

    m_model.loadFromCSV(":/data"); // 从CSV文件加载数据

    // 设置线图
    m_lines = new LineDiagram(this); // 创建线图对象
    m_lines->setModel(&m_model); // 设置数据模型

    // 创建坐标轴
    auto *xAxis = new CartesianAxis(m_lines);
    auto *yAxis = new CartesianAxis(m_lines);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom); // 设置X轴位置
    yAxis->setPosition(KDChart::CartesianAxis::Left); // 设置Y轴位置
    m_lines->addAxis(xAxis); // 添加X轴
    m_lines->addAxis(yAxis); // 添加Y轴

    m_chart->coordinatePlane()->replaceDiagram(m_lines); // 将线图添加到图表
    m_chart->setGlobalLeading(20, 20, 20, 20); // 设置全局边距

    // 初始化定时器
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::slot_timerFired); // 连接定时器信号到槽函数
    timer->start(30); // 启动定时器，间隔30毫秒

    // 更改图表内的光标样式
    m_chart->setCursor(Qt::IBeamCursor);

    // 为图表安装事件过滤器
    m_chart->installEventFilter(this);
}

/**
 * @brief 事件过滤器
 * @param target 目标对象
 * @param event 事件
 * @return 如果事件被处理则返回true，否则返回false
 * 
 * 捕获图表的鼠标移动事件，获取鼠标位置并显示对应的数据值。
 */
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_chart) { // 检查目标是否为图表
        if (event->type() == QEvent::MouseMove) { // 检查事件类型是否为鼠标移动
            // 当鼠标移动到数据点上时，获取该数据点并打印数据值
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            QPointF pos = mouseEvent->pos(); // 获取鼠标位置
            QModelIndex index = m_lines->indexAt(pos.toPoint()); // 获取鼠标位置对应的数据索引
            if (index.isValid()) { // 检查索引是否有效
                qDebug() << "Mouse position" << pos << "Data:" << m_model.data(index).toDouble();
            }
        }
    }
    return QWidget::eventFilter(target, event); // 调用父类的事件过滤器
}

/**
 * @brief 线图类型下拉框选择变化槽函数
 * @param index 选中的索引
 * 
 * 根据选择的索引切换线图类型（普通、堆叠、百分比）。
 */
void MainWindow::on_lineTypeCB_currentIndexChanged(int index)
{
    const QString text = lineTypeCB->itemText(index); // 获取选中项的文本
    if (text == "Normal")
        m_lines->setType(LineDiagram::Normal); // 设置为普通线图
    else if (text == "Stacked")
        m_lines->setType(LineDiagram::Stacked); // 设置为堆叠线图
    else if (text == "Percent")
        m_lines->setType(LineDiagram::Percent); // 设置为百分比线图
    else
        qWarning(" Does not match any type"); // 警告：没有匹配的类型

    m_chart->update(); // 更新图表
}

/**
 * @brief 数据值显示复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否显示数据值。
 */
void MainWindow::on_paintValuesCB_toggled(bool checked)
{
    const int colCount = m_lines->model()->columnCount(m_lines->rootIndex()); // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) { // 遍历所有列
        DataValueAttributes a(m_lines->dataValueAttributes(iColumn)); // 获取数据值属性
        QBrush brush(m_lines->brush(iColumn)); // 获取画笔
        TextAttributes ta(a.textAttributes()); // 获取文本属性
        ta.setRotation(0); // 设置旋转角度
        ta.setFont(QFont("Comic", 10)); // 设置字体
        ta.setPen(QPen(brush.color())); // 设置笔颜色

        if (checked)
            ta.setVisible(true); // 显示文本
        else
            ta.setVisible(false); // 隐藏文本
        a.setVisible(true); // 启用数据值属性
        a.setTextAttributes(ta); // 设置文本属性
        m_lines->setDataValueAttributes(iColumn, a); // 设置数据值属性
    }
    m_chart->update(); // 更新图表
}

/**
 * @brief 平滑线条滑块值变化槽函数
 * @param value 平滑度值
 * 
 * 调整线条的平滑度。
 */
void MainWindow::on_smoothLinesSDR_valueChanged(int value)
{
    m_lines->setLineTension(value / 10.0); // 设置线条张力（平滑度）
    m_chart->update(); // 更新图表
}

/**
 * @brief 数据点居中复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制数据点是否居中显示。
 */
void MainWindow::on_centerDataPointsCB_toggled(bool checked)
{
    m_lines->setCenterDataPoints(checked); // 设置数据点是否居中
    m_chart->update(); // 更新图表
}

/**
 * @brief 区域动画复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否启用区域动画效果。
 */
void MainWindow::on_animateAreasCB_toggled(bool checked)
{
    if (checked) { // 如果启用动画
        highlightAreaCB->setCheckState(Qt::Unchecked); // 取消选中高亮区域复选框
        m_curRow = 0; // 重置当前行索引
        m_curColumn = 0; // 重置当前列索引
    } else { // 如果禁用动画
        m_curColumn = -1; // 重置当前列索引
    }
    highlightAreaCB->setEnabled(!checked); // 设置高亮区域复选框是否可用
    highlightAreaSB->setEnabled(!checked); // 设置高亮区域滑块是否可用

    // 取消所有之前高亮的列
    const int rowCount = m_lines->model()->rowCount(); // 获取行数
    const int colCount = m_lines->model()->columnCount(); // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) { // 遍历所有列
        setHighlightArea(-1, iColumn, 127, false, false); // 取消列高亮
        for (int iRow = 0; iRow < rowCount; ++iRow) // 遍历所有行
            setHighlightArea(iRow, iColumn, 127, false, false); // 取消单元格高亮
    }
    m_chart->update(); // 更新图表
    m_curOpacity = 0; // 重置当前透明度
}

/**
 * @brief 定时器触发槽函数
 * 
 * 用于实现区域动画效果，定期更新高亮区域。
 */
void MainWindow::slot_timerFired()
{
    if (m_curColumn < 0) { // 如果当前列索引无效
        return; // 退出函数
    }
    m_curOpacity += 8; // 增加透明度
    if (m_curOpacity > 255) { // 如果透明度超过255
        setHighlightArea(m_curRow, m_curColumn, 127, false, false); // 取消当前区域高亮
        m_curOpacity = 5; // 重置透明度
        ++m_curRow; // 移动到下一行
        if (m_curRow >= m_lines->model()->rowCount(m_lines->rootIndex())) { // 如果到达最后一行
            m_curRow = 0; // 重置行索引
            ++m_curColumn; // 移动到下一列
            if (m_curColumn >= m_lines->model()->columnCount(m_lines->rootIndex())) // 如果到达最后一列
                m_curColumn = 0; // 重置列索引
        }
    }
    setHighlightArea(m_curRow, m_curColumn, m_curOpacity, true, true); // 设置当前区域高亮
}

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
void MainWindow::setHighlightArea(int row, int column, int opacity,
                                  bool checked, bool doUpdate)
{
    if (row < 0) { // 如果行索引无效（高亮整列）
        // 高亮整个数据集
        LineAttributes la = m_lines->lineAttributes(column); // 获取线条属性
        if (checked) { // 如果启用高亮
            la.setDisplayArea(true); // 显示区域
            la.setTransparency(opacity); // 设置透明度
        } else { // 如果禁用高亮
            la.setDisplayArea(false); // 隐藏区域
        }
        m_lines->setLineAttributes(column, la); // 设置线条属性
    } else { // 如果行索引有效（高亮特定单元格）
        // 高亮两个线段
        if (row) { // 如果不是第一行
            QModelIndex cellIndex(m_lines->model()->index(row - 1, column,
                                                          m_lines->rootIndex())); // 获取上一行单元格索引
            if (checked) { // 如果启用高亮
                LineAttributes la(m_lines->lineAttributes(cellIndex)); // 获取线条属性
                la.setDisplayArea(true); // 显示区域
                la.setTransparency(255 - opacity); // 设置透明度
                // 为该单元格设置特定的线条属性
                m_lines->setLineAttributes(cellIndex, la);
            } else { // 如果禁用高亮
                // 移除该单元格的特定线条属性
                m_lines->resetLineAttributes(cellIndex);
            }
        }
        if (row < m_lines->model()->rowCount(m_lines->rootIndex())) { // 如果不是最后一行
            QModelIndex cellIndex(m_lines->model()->index(row, column, m_lines->rootIndex())); // 获取当前单元格索引
            if (checked) { // 如果启用高亮
                LineAttributes la(m_lines->lineAttributes(cellIndex)); // 获取线条属性
                la.setDisplayArea(true); // 显示区域
                la.setTransparency(opacity); // 设置透明度
                // 为该单元格设置特定的线条属性
                m_lines->setLineAttributes(cellIndex, la);
            } else { // 如果禁用高亮
                // 移除该单元格的特定线条属性
                m_lines->resetLineAttributes(cellIndex);
            }
        }
    }
    if (doUpdate) // 如果需要更新图表
        m_chart->update(); // 更新图表
}

/**
 * @brief 区域高亮复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否启用区域高亮显示。
 */
void MainWindow::on_highlightAreaCB_toggled(bool checked)
{
    setHighlightArea(-1, highlightAreaSB->value(), 127, checked, true); // 设置高亮区域
}

/**
 * @brief 区域高亮滑块值变化槽函数
 * @param i 区域索引
 * 
 * 调整要高亮显示的区域索引。
 */
void MainWindow::on_highlightAreaSB_valueChanged(int i)
{
    Q_UNUSED(i); // 未使用的参数
    if (highlightAreaCB->isChecked()) // 如果高亮区域复选框已选中
        on_highlightAreaCB_toggled(true); // 触发高亮区域复选框槽函数
    else
        on_highlightAreaCB_toggled(false); // 触发高亮区域复选框槽函数
}

/**
 * @brief 3D模式复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否启用3D模式。
 */
void MainWindow::on_threeDModeCB_toggled(bool checked)
{
    ThreeDLineAttributes td(m_lines->threeDLineAttributes()); // 获取3D线条属性
    td.setDepth(depthSB->value()); // 设置深度
    if (checked)
        td.setEnabled(true); // 启用3D模式
    else
        td.setEnabled(false); // 禁用3D模式

    m_lines->setThreeDLineAttributes(td); // 设置3D线条属性

    m_chart->update(); // 更新图表
}

/**
 * @brief 深度滑块值变化槽函数
 * @param i 深度值
 * 
 * 调整3D模式下的深度。
 */
void MainWindow::on_depthSB_valueChanged(int i)
{
    Q_UNUSED(i); // 未使用的参数
    if (threeDModeCB->isChecked()) // 如果3D模式复选框已选中
        on_threeDModeCB_toggled(true); // 触发3D模式复选框槽函数
}

/**
 * @brief 跟踪区域复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否启用区域跟踪。
 */
void MainWindow::on_trackAreasCB_toggled(bool checked)
{
    setTrackedArea(trackAreasSB->value(), checked, true); // 设置跟踪区域
}

/**
 * @brief 跟踪区域滑块值变化槽函数
 * @param i 区域索引
 * 
 * 调整要跟踪的区域索引。
 */
void MainWindow::on_trackAreasSB_valueChanged(int i)
{
    Q_UNUSED(i); // 未使用的参数
    on_trackAreasCB_toggled(trackAreasCB->isChecked()); // 触发跟踪区域复选框槽函数
}

/**
 * @brief 设置跟踪区域
 * @param column 列索引
 * @param checked 是否启用
 * @param doUpdate 是否更新图表
 * 
 * 设置指定区域的跟踪效果。
 */
void MainWindow::setTrackedArea(int column, bool checked, bool doUpdate)
{
    const int rowCount = m_model.rowCount(m_lines->rootIndex()); // 获取行数
    const int columnCount = m_model.columnCount(m_lines->rootIndex()); // 获取列数
    for (int i = 0; i < rowCount; ++i) { // 遍历所有行
        for (int j = 0; j < columnCount; ++j) { // 遍历所有列
            QModelIndex cellIndex(m_model.index(i, j,
                                                m_lines->rootIndex())); // 获取单元格索引
            ValueTrackerAttributes va(m_lines->valueTrackerAttributes(cellIndex)); // 获取值跟踪属性
            va.setEnabled(checked && j == column); // 设置是否启用跟踪
            va.setAreaBrush(QColor(255, 255, 0, 50)); // 设置区域画刷
            va.setOrientations(Qt::Horizontal | Qt::Vertical); // 设置方向
            QColor color = Qt::cyan; // 设置颜色
            color.setAlphaF(0.5); // 设置透明度
            va.setAreaBrush(color); // 设置区域画刷
            m_lines->setValueTrackerAttributes(cellIndex, va); // 设置值跟踪属性
        }
    }
    if (doUpdate) // 如果需要更新图表
        m_chart->update(); // 更新图表
}

/**
 * @brief 水平反转复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否反转水平坐标轴。
 */
void MainWindow::on_reverseHorizontalCB_toggled(bool checked)
{
    static_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane())
        ->setHorizontalRangeReversed(checked); // 设置水平坐标轴是否反转
}

/**
 * @brief 垂直反转复选框状态变化槽函数
 * @param checked 是否选中
 * 
 * 控制是否反转垂直坐标轴。
 */
void MainWindow::on_reverseVerticalCB_toggled(bool checked)
{
    static_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane())
        ->setVerticalRangeReversed(checked); // 设置垂直坐标轴是否反转
}
