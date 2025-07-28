/****************************************************************************
**
** 此文件是KD Chart库的一部分。
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
 * @param parent 父窗口指针
 * 
 * 初始化主窗口UI，创建图表布局，加载数据模型，配置折线图和坐标轴
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置UI界面
    setupUi(this);

    // 创建图表布局并添加到图表框架中
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart();
    // 设置图表全局边距
    m_chart->setGlobalLeading(20, 20, 20, 20);
    chartLayout->addWidget(m_chart);
    // 隐藏滚动条
    hSBar->setVisible(false);
    vSBar->setVisible(false);

    // 从CSV文件加载数据
    m_model.loadFromCSV(":/data");

    // 设置折线图
    m_lines = new LineDiagram();
    m_lines->setModel(&m_model);
    // 创建四个坐标轴
    auto *xAxis = new CartesianAxis(m_lines);
    auto *yAxis = new CartesianAxis(m_lines);
    auto *axisTop = new CartesianAxis(m_lines);
    auto *axisRight = new CartesianAxis(m_lines);
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
    me.setValue(me.value() * 1.5);
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
    m_lines->setUnitSuffix(QString::fromLatin1("s"), Qt::Horizontal);
    m_lines->setUnitSuffix(QString::fromLatin1("m"), Qt::Vertical);

    // 设置图例
    m_legend = new Legend(m_lines, m_chart);
    m_chart->addLegend(m_legend);

    // 初始化属性；这是必要的，因为我们需要先启用数据值属性，
    // 然后才能显示其中的部分（例如仅标记）。但如果启用数据值属性，
    // 即使我们只想设置标记，也会包含默认的数据值文本。
    // 因此我们启用DVA，然后单独禁用我们不需要的部分。
    on_paintValuesCB_toggled(false);
    on_paintMarkersCB_toggled(false);
}

/**
 * @brief 处理线条类型下拉框变化事件
 * @param index 当前选中的索引
 * 
 * 根据选中的索引设置折线图的类型（普通、堆叠或百分比）
 */
void MainWindow::on_lineTypeCB_currentIndexChanged(int index)
{
    const QString text = lineTypeCB->itemText(index);
    if (text == "Normal")
        m_lines->setType(LineDiagram::Normal);
    else if (text == "Stacked")
        m_lines->setType(LineDiagram::Stacked);
    else if (text == "Percent")
        m_lines->setType(LineDiagram::Percent);
    else
        qWarning(" 不匹配任何类型");

    // 更新图表
    m_chart->update();
}

/**
 * @brief 处理图例显示复选框状态变化事件
 * @param checked 是否选中
 * 
 * 根据复选框状态设置图例的可见性
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
 * 根据复选框状态设置数据值的可见性，并配置文本属性
 */
void MainWindow::on_paintValuesCB_toggled(bool checked)
{
    // 我们在这里按列设置DataValueAttributes，
    // 因为我们希望文本以不同的颜色打印 - 根据各自数据集的颜色。
    const QFont font("Comic", 10);
    const int colCount = m_lines->model()->columnCount();
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {
        DataValueAttributes dva = m_lines->dataValueAttributes(iColumn);
        dva.setVisible(true);

        TextAttributes ta(dva.textAttributes());
        ta.setRotation(0);
        ta.setFont(font);
        ta.setPen(QPen(m_lines->brush(iColumn).color()));
        ta.setVisible(checked);

        dva.setTextAttributes(ta);
        m_lines->setDataValueAttributes(iColumn, dva);
    }
    m_chart->update();
}

/**
 * @brief 处理标记显示复选框状态变化事件
 * @param checked 是否选中
 * 
 * 根据复选框状态设置标记的可见性，并配置标记样式和大小
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
    const int rowCount = m_lines->model()->rowCount();
    const int colCount = m_lines->model()->columnCount();
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {

        DataValueAttributes dva = m_lines->dataValueAttributes(iColumn);
        dva.setVisible(true);
        MarkerAttributes ma(dva.markerAttributes());

        switch (markersStyleCB->currentIndex()) {
        case 0:
            ma.setMarkerStyle(MarkerAttributes::MarkerSquare);
            break;
        case 1:
            // 列特定的属性
            ma.setMarkerStyle(map.value(iColumn));
            break;
        case 2:
            ma.setMarkerStyle(MarkerAttributes::MarkerCircle);
            break;
        case 3:
            ma.setMarkerStyle(MarkerAttributes::MarkerDiamond);
            break;
        case 4:
            ma.setMarkerStyle(MarkerAttributes::Marker1Pixel);
            break;
        case 5:
            ma.setMarkerStyle(MarkerAttributes::Marker4Pixels);
            break;
        case 6:
            ma.setMarkerStyle(MarkerAttributes::MarkerRing);
            break;
        case 7:
            ma.setMarkerStyle(MarkerAttributes::MarkerCross);
            break;
        case 8:
            ma.setMarkerStyle(MarkerAttributes::MarkerFastCross);
            break;
        default:
            Q_ASSERT(false);
        }
        ma.setVisible(checked);

        ma.setMarkerSize(QSize(markersWidthSB->value(), markersHeightSB->value()));
        dva.setMarkerAttributes(ma);
        m_lines->setDataValueAttributes(iColumn, dva);

        // 为某些值指定单元格特定的属性！
        for (int j = 0; j < rowCount; ++j) {
            const QModelIndex index(m_lines->model()->index(j, iColumn, QModelIndex()));
            const qreal value = m_lines->model()->data(index).toReal();
            /* 为特定值设置特定颜色 - 标记 */
            if (value == 8) {
                // 获取单元格特定属性
                // 或回退到列设置
                // 或回退到全局设置：
                DataValueAttributes yellowAttributes(m_lines->dataValueAttributes(index));
                MarkerAttributes yellowMarker(yellowAttributes.markerAttributes());
                yellowMarker.setMarkerColor(Qt::yellow);
                yellowMarker.setMarkerSize(QSize(markersWidthSB->value(), markersHeightSB->value()));
                yellowMarker.setVisible(checked);
                yellowAttributes.setMarkerAttributes(yellowMarker);
                yellowAttributes.setVisible(checked);
                // cell specific attributes:
                m_lines->setDataValueAttributes(index, yellowAttributes);
            }
        }
    }

    m_chart->update();
}

void MainWindow::on_markersStyleCB_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    on_paintMarkersCB_toggled(paintMarkersCB->isChecked());
}

void MainWindow::on_markersWidthSB_valueChanged(int i)
{
    Q_UNUSED(i);
    markersHeightSB->setValue(markersWidthSB->value());
    if (paintMarkersCB->isChecked())
        on_paintMarkersCB_toggled(true);
}

void MainWindow::on_markersHeightSB_valueChanged(int /*i*/)
{
    markersWidthSB->setValue(markersHeightSB->value());
    if (paintMarkersCB->isChecked())
        on_paintMarkersCB_toggled(true);
}

void MainWindow::on_displayAreasCB_toggled(bool checked)
{
    LineAttributes la(m_lines->lineAttributes());
    la.setDisplayArea(checked);
    if (checked) {
        la.setTransparency(transparencySB->value());
    }
    m_lines->setLineAttributes(la);
    m_chart->update();
}

void MainWindow::on_transparencySB_valueChanged(int alpha)
{
    LineAttributes la(m_lines->lineAttributes());
    la.setTransparency(alpha);
    m_lines->setLineAttributes(la);
    on_displayAreasCB_toggled(true);
}

void MainWindow::on_zoomFactorSB_valueChanged(double factor)
{
    const bool isZoomedIn = factor > 1;
    hSBar->setVisible(isZoomedIn);
    vSBar->setVisible(isZoomedIn);
    if (!isZoomedIn) {
        hSBar->setValue(500);
        vSBar->setValue(500);
    }
    m_chart->coordinatePlane()->setZoomFactorX(factor);
    m_chart->coordinatePlane()->setZoomFactorY(factor);
    m_chart->update();
}

void MainWindow::on_hSBar_valueChanged(int hPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hPos / 1000.0, vSBar->value() / 1000.0));
    m_chart->update();
}

void MainWindow::on_vSBar_valueChanged(int vPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hSBar->value() / 1000.0, vPos / 1000.0));
    m_chart->update();
}
