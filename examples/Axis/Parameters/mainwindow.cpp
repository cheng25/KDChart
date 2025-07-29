/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** 此文件是坐标轴参数配置示例的主窗口实现文件，包含MainWindow类的具体实现
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChartAbstractCoordinatePlane>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartLegend>
#include <KDChartLineAttributes>
#include <KDChartLineDiagram>
#include <KDChartMarkerAttributes>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口指针，默认为nullptr
 * 
 * 初始化主窗口UI，创建图表布局，加载数据模型，配置折线图和坐标轴。
 * 具体包括：设置UI界面、创建图表布局、加载CSV数据、配置折线图、
 * 设置四个坐标轴（底部、左侧、顶部、右侧）、配置坐标轴标题和属性、
 * 添加坐标轴到折线图、设置坐标轴单位后缀、配置图例以及初始化数据值属性。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置UI界面
    setupUi(this);

    // 创建图表布局并添加到图表框架中
    auto *chartLayout = new QHBoxLayout(chartFrame); // 创建水平布局
    m_chart = new Chart(); // 创建图表对象
    // 设置图表全局边距
    m_chart->setGlobalLeading(20, 20, 20, 20); // 设置图表上、右、下、左的边距
    chartLayout->addWidget(m_chart); // 将图表添加到布局中
    // 隐藏滚动条
    hSBar->setVisible(false);
    vSBar->setVisible(false);

    // 从CSV文件加载数据
    m_model.loadFromCSV(" :/data "); // 加载资源文件中的CSV数据

    // 设置折线图
    m_lines = new LineDiagram(); // 创建折线图对象
    m_lines->setModel(&m_model); // 设置折线图的数据模型
    // 创建四个坐标轴
    auto *xAxis = new CartesianAxis(m_lines); // 底部X轴
    auto *yAxis = new CartesianAxis(m_lines); // 左侧Y轴
    auto *axisTop = new CartesianAxis(m_lines); // 顶部X轴
    auto *axisRight = new CartesianAxis(m_lines); // 右侧Y轴
    // 设置坐标轴位置
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    axisTop->setPosition(KDChart::CartesianAxis::Top);
    axisRight->setPosition(KDChart::CartesianAxis::Right);

    // 设置坐标轴标题
    xAxis->setTitleText("底部横坐标轴");
    yAxis->setTitleText("左侧纵坐标轴");
    axisTop->setTitleText("顶部横坐标轴");
    axisRight->setTitleText("右侧纵坐标轴");
    // 设置顶部坐标轴标题文本属性（红色）
    TextAttributes taTop(xAxis->titleTextAttributes());
    taTop.setPen(QPen(Qt::red));
    axisTop->setTitleTextAttributes(taTop);
    // 设置右侧坐标轴标题文本属性（增大字体）
    TextAttributes taRight(xAxis->titleTextAttributes());
    Measure me(taRight.fontSize());
    me.setValue(me.value() * 1.5); // 字体大小增大1.5倍
    taRight.setFontSize(me);
    axisRight->setTitleTextAttributes(taRight);

    // 将坐标轴添加到折线图
    m_lines->addAxis(xAxis);
    m_lines->addAxis(yAxis);
    m_lines->addAxis(axisTop);
    m_lines->addAxis(axisRight);
    // 将折线图添加到图表坐标平面
    m_chart->coordinatePlane()->replaceDiagram(m_lines);

    // 设置坐标轴单位后缀
    m_lines->setUnitSuffix(QString::fromLatin1("s"), Qt::Horizontal); // 水平轴单位为"s"（秒）
    m_lines->setUnitSuffix(QString::fromLatin1("m"), Qt::Vertical); // 垂直轴单位为"m"（米）

    // 设置图例
    m_legend = new Legend(m_lines, m_chart); // 创建图例对象
    m_chart->addLegend(m_legend); // 将图例添加到图表

    // 初始化属性；这是必要的，因为我们需要先启用数据值属性，
    // 然后才能显示其中的部分（例如仅标记）。但如果启用数据值属性，
    // 即使我们只想设置标记，也会包含默认的数据值文本。
    // 因此我们启用DVA，然后单独禁用我们不需要的部分。
    on_paintValuesCB_toggled(false); // 初始时不显示数据值
    on_paintMarkersCB_toggled(false); // 初始时不显示标记
}

/**
 * @brief 处理线条类型下拉框变化事件
 * @param index 当前选中的索引
 * 
 * 根据选中的索引设置折线图的类型（普通、堆叠或百分比）。
 * 当用户选择不同的线条类型时，更新折线图的显示方式并刷新图表。
 */
void MainWindow::on_lineTypeCB_currentIndexChanged(int index)
{
    const QString text = lineTypeCB->itemText(index);
    if (text == "Normal")
        m_lines->setType(LineDiagram::Normal); // 普通折线图
    else if (text == "Stacked")
        m_lines->setType(LineDiagram::Stacked); // 堆叠折线图
    else if (text == "Percent")
        m_lines->setType(LineDiagram::Percent); // 百分比折线图
    else
        qWarning(" 不匹配任何类型");

    // 更新图表
    m_chart->update();
}

/**
 * @brief 处理图例显示复选框状态变化事件
 * @param checked 是否选中
 * 
 * 根据复选框状态设置图例的可见性。如果checked为true，则显示图例；
 * 否则隐藏图例，并刷新图表。
 */
void MainWindow::on_paintLegendCB_toggled(bool checked)
{
    m_legend->setVisible(checked);
    m_chart->update();
}

/**
 * @brief 处理数值显示复选框状态变化事件
 * @param checked 是否选中
 * 
 * 根据复选框状态设置数据值的可见性，并配置文本属性。
 * 为每列数据设置不同颜色的文本，并根据复选框状态控制文本的显示或隐藏。
 */
void MainWindow::on_paintValuesCB_toggled(bool checked)
{
    // 我们在这里按列设置DataValueAttributes，
    // 因为我们希望文本以不同的颜色打印 - 根据各自数据集的颜色。
    const QFont font("Comic", 10); // 设置字体为Comic，大小为10
    const int colCount = m_lines->model()->columnCount(); // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {
        DataValueAttributes dva = m_lines->dataValueAttributes(iColumn);
        dva.setVisible(true);

        TextAttributes ta(dva.textAttributes());
        ta.setRotation(0); // 文本不旋转
        ta.setFont(font); // 设置字体
        ta.setPen(QPen(m_lines->brush(iColumn).color())); // 设置文本颜色与数据集颜色一致
        ta.setVisible(checked); // 根据复选框状态设置文本可见性

        dva.setTextAttributes(ta);
        m_lines->setDataValueAttributes(iColumn, dva);
    }
    m_chart->update(); // 刷新图表
}

/**
 * @brief 处理标记显示复选框状态变化事件
 * @param checked 是否选中
 * 
 * 根据复选框状态设置标记的可见性，并配置标记样式和大小。
 * 支持为不同列设置不同的标记样式，并为特定值设置特定颜色的标记。
 */
void MainWindow::on_paintMarkersCB_toggled(bool checked)
{
    // 创建一个包含不同标记类型的映射
    MarkerAttributes::MarkerStylesMap map;
    map.insert(0, MarkerAttributes::MarkerSquare);
    map.insert(1, MarkerAttributes::MarkerCircle);
    map.insert(2, MarkerAttributes::MarkerRing);
    map.insert(3, MarkerAttributes::MarkerCross);
    map.insert(4, MarkerAttributes::MarkerDiamond);

    // 接下来：指定列/单元格特定的属性！
    const int rowCount = m_lines->model()->rowCount(); // 获取行数
    const int colCount = m_lines->model()->columnCount(); // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {

        DataValueAttributes dva = m_lines->dataValueAttributes(iColumn);
        dva.setVisible(true);
        MarkerAttributes ma(dva.markerAttributes());

        switch (markersStyleCB->currentIndex()) {
        case 0:
            ma.setMarkerStyle(MarkerAttributes::MarkerSquare); // 方形标记
            break;
        case 1:
            // 列特定的属性
            ma.setMarkerStyle(map.value(iColumn)); // 根据列索引设置不同的标记样式
            break;
        case 2:
            ma.setMarkerStyle(MarkerAttributes::MarkerCircle); // 圆形标记
            break;
        case 3:
            ma.setMarkerStyle(MarkerAttributes::MarkerDiamond); // 菱形标记
            break;
        case 4:
            ma.setMarkerStyle(MarkerAttributes::Marker1Pixel); // 1像素标记
            break;
        case 5:
            ma.setMarkerStyle(MarkerAttributes::Marker4Pixels); // 4像素标记
            break;
        case 6:
            ma.setMarkerStyle(MarkerAttributes::MarkerRing); // 环形标记
            break;
        case 7:
            ma.setMarkerStyle(MarkerAttributes::MarkerCross); // 十字标记
            break;
        case 8:
            ma.setMarkerStyle(MarkerAttributes::MarkerFastCross); // 快速十字标记
            break;
        default:
            Q_ASSERT(false);
        }
        ma.setVisible(checked); // 根据复选框状态设置标记可见性

        ma.setMarkerSize(QSize(markersWidthSB->value(), markersHeightSB->value())); // 设置标记大小
        dva.setMarkerAttributes(ma);
        m_lines->setDataValueAttributes(iColumn, dva);

        // 为某些值指定单元格特定的属性！
        for (int j = 0; j < rowCount; ++j) {
            const QModelIndex index(m_lines->model()->index(j, iColumn, QModelIndex()));
            const qreal value = m_lines->model()->data(index).toReal();
            /* 为特定值设置特定颜色 - 标记 */
            if (value == 8) { // 如果值等于8
                // 获取单元格特定属性
                // 或回退到列设置
                // 或回退到全局设置：
                DataValueAttributes yellowAttributes(m_lines->dataValueAttributes(index));
                MarkerAttributes yellowMarker(yellowAttributes.markerAttributes());
                yellowMarker.setMarkerColor(Qt::yellow); // 设置标记颜色为黄色
                yellowMarker.setMarkerSize(QSize(markersWidthSB->value(), markersHeightSB->value()));
                yellowMarker.setVisible(checked);
                yellowAttributes.setMarkerAttributes(yellowMarker);
                yellowAttributes.setVisible(checked);
                // cell specific attributes:
                m_lines->setDataValueAttributes(index, yellowAttributes);
            }
        }
    }

    m_chart->update(); // 刷新图表
}

/**
 * @brief 处理标记样式下拉框变化事件
 * @param index 当前选中的索引
 * 
 * 当标记样式下拉框选中不同的样式时，更新标记的显示样式。
 * 该函数会调用on_paintMarkersCB_toggled函数来应用新的样式。
 */
void MainWindow::on_markersStyleCB_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    on_paintMarkersCB_toggled(paintMarkersCB->isChecked());
}

/**
 * @brief 处理标记宽度微调框值变化事件
 * @param i 当前值
 * 
 * 当标记宽度微调框的值变化时，同步更新标记高度微调框的值，
 * 并更新标记的大小。
 */
void MainWindow::on_markersWidthSB_valueChanged(int i)
{
    Q_UNUSED(i);
    markersHeightSB->setValue(markersWidthSB->value()); // 同步宽度和高度
    if (paintMarkersCB->isChecked())
        on_paintMarkersCB_toggled(true); // 如果标记可见，则更新标记
}

/**
 * @brief 处理标记高度微调框值变化事件
 * @param i 当前值
 * 
 * 当标记高度微调框的值变化时，同步更新标记宽度微调框的值，
 * 并更新标记的大小。
 */
void MainWindow::on_markersHeightSB_valueChanged(int /*i*/)
{
    markersWidthSB->setValue(markersHeightSB->value()); // 同步宽度和高度
    if (paintMarkersCB->isChecked())
        on_paintMarkersCB_toggled(true); // 如果标记可见，则更新标记
}

/**
 * @brief 处理显示区域复选框状态变化事件
 * @param checked 是否选中
 * 
 * 根据复选框状态设置折线图的区域填充显示。如果checked为true，
 * 则显示区域填充，并设置透明度；否则隐藏区域填充。
 */
void MainWindow::on_displayAreasCB_toggled(bool checked)
{
    LineAttributes la(m_lines->lineAttributes());
    la.setDisplayArea(checked); // 设置是否显示区域填充
    if (checked) {
        la.setTransparency(transparencySB->value()); // 设置透明度
    }
    m_lines->setLineAttributes(la);
    m_chart->update(); // 刷新图表
}

/**
 * @brief 处理透明度微调框值变化事件
 * @param alpha 当前透明度值
 * 
 * 当透明度微调框的值变化时，更新区域填充的透明度，
 * 并确保区域填充处于显示状态。
 */
void MainWindow::on_transparencySB_valueChanged(int alpha)
{
    LineAttributes la(m_lines->lineAttributes());
    la.setTransparency(alpha); // 设置透明度
    m_lines->setLineAttributes(la);
    on_displayAreasCB_toggled(true); // 确保区域填充处于显示状态
}

/**
 * @brief 处理缩放因子微调框值变化事件
 * @param factor 当前缩放因子
 * 
 * 当缩放因子微调框的值变化时，更新图表的缩放比例。
 * 当factor大于1时放大图表，并显示滚动条；当factor等于1时恢复原始大小，
 * 并隐藏滚动条。
 */
void MainWindow::on_zoomFactorSB_valueChanged(double factor)
{
    const bool isZoomedIn = factor > 1;
    hSBar->setVisible(isZoomedIn); // 根据缩放状态显示或隐藏水平滚动条
    vSBar->setVisible(isZoomedIn); // 根据缩放状态显示或隐藏垂直滚动条
    if (!isZoomedIn) {
        hSBar->setValue(500); // 重置水平滚动条位置
        vSBar->setValue(500); // 重置垂直滚动条位置
    }
    m_chart->coordinatePlane()->setZoomFactorX(factor); // 设置X轴缩放因子
    m_chart->coordinatePlane()->setZoomFactorY(factor); // 设置Y轴缩放因子
    m_chart->update(); // 刷新图表
}

/**
 * @brief 处理水平滚动条值变化事件
 * @param hPos 当前水平位置
 * 
 * 当水平滚动条的值变化时，更新图表的缩放中心，实现水平平移图表。
 */
void MainWindow::on_hSBar_valueChanged(int hPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hPos / 1000.0, vSBar->value() / 1000.0));
    m_chart->update(); // 刷新图表
}

/**
 * @brief 处理垂直滚动条值变化事件
 * @param vPos 当前垂直位置
 * 
 * 当垂直滚动条的值变化时，更新图表的缩放中心，实现垂直平移图表。
 */
void MainWindow::on_vSBar_valueChanged(int vPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hSBar->value() / 1000.0, vPos / 1000.0));
    m_chart->update(); // 刷新图表
}
