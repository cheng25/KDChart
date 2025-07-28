/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** This file is part of the KD Chart library.
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** 许可协议: MIT
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"
#include "framewidget.h"

#include <KDChartAbstractCoordinatePlane>
#include <KDChartBackgroundAttributes>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartLegend>
#include <KDChartLineAttributes>
#include <KDChartLineDiagram>
#include <KDChartMarkerAttributes>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>

#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <QPrinter>

using namespace KDChart;

/**
 * @brief drawIntoPixmap 将图表绘制到像素图中
 * @param size 像素图大小
 * @param chart 要绘制的图表
 * @return 包含图表的像素图
 * @details 创建指定大小的像素图，并在其上绘制图表
 */
static QPixmap drawIntoPixmap(const QSize &size, KDChart::Chart *chart)
{
    QPixmap pix(size);  // 创建指定大小的像素图
    pix.fill(Qt::white);  // 白色填充像素图
    QPainter painter(&pix);  // 创建画家对象
    chart->paint(&painter, QRect(0, 0, size.width(), size.height()));  // 绘制图表
    return pix;  // 返回像素图
}

// 定义宏以使用FrameWidget，它使用Chart::paint来绘制自身
// 如果不定义，则直接使用Chart部件
#define USE_FRAME_WIDGET 1

/**
 * @brief MainWindow::MainWindow 构造函数
 * @param parent 父窗口部件指针
 * @details 初始化主窗口，创建图表、数据模型和UI元素
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);  // 初始化UI

    auto *chartLayout = new QHBoxLayout(chartFrame);  // 创建图表框架的水平布局
#ifdef USE_FRAME_WIDGET
    auto *chartFrameWidget = new FrameWidget();  // 创建FrameWidget对象
    chartLayout->addWidget(chartFrameWidget);  // 将FrameWidget添加到布局
#endif
    hSBar->setVisible(false);  // 隐藏水平滚动条
    vSBar->setVisible(false);  // 隐藏垂直滚动条

    m_model.loadFromCSV(":/data");  // 从CSV文件加载数据

    // 设置图表
    m_lines = new LineDiagram();  // 创建线图对象
    m_lines->setModel(&m_model);  // 设置数据模型

    auto *xAxis = new CartesianAxis(m_lines);  // 创建X轴
    auto *yAxis = new CartesianAxis(m_lines);  // 创建Y轴
    auto *axisTop = new CartesianAxis(m_lines);  // 创建顶部轴
    auto *axisRight = new CartesianAxis(m_lines);  // 创建右侧轴
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);  // 设置X轴位置为底部
    yAxis->setPosition(KDChart::CartesianAxis::Left);  // 设置Y轴位置为左侧
    axisTop->setPosition(KDChart::CartesianAxis::Top);  // 设置顶部轴位置
    axisRight->setPosition(KDChart::CartesianAxis::Right);  // 设置右侧轴位置

    m_lines->addAxis(xAxis);  // 为线图添加X轴
    m_lines->addAxis(yAxis);  // 为线图添加Y轴
    m_lines->addAxis(axisTop);  // 为线图添加顶部轴
    m_lines->addAxis(axisRight);  // 为线图添加右侧轴

    m_chart = new Chart();  // 创建图表对象
    // m_chart->setGlobalLeading(10,10,10,10); // 默认没有全局边距

#ifdef USE_FRAME_WIDGET
    chartFrameWidget->setChart(m_chart);  // 设置FrameWidget的图表
    // 确保在更改图表属性后重新绘制
    connect(m_chart, &KDChart::Chart::propertiesChanged,
            chartFrameWidget, QOverload<>::of(&FrameWidget::update));
#else
    chartLayout->addWidget(m_chart);  // 将图表添加到布局
#endif

    m_chart->coordinatePlane()->replaceDiagram(m_lines);  // 替换坐标平面的图表

    // 设置线条宽度
    for (int iColumn = 0; iColumn < m_lines->model()->columnCount(); ++iColumn) {
        QPen pen(m_lines->pen(iColumn));  // 获取当前线条的画笔
        pen.setWidth(4);  // 设置画笔宽度为4
        m_lines->setPen(iColumn, pen);  // 应用画笔设置
    }

    // 设置图表框架属性
    FrameAttributes faChart(m_chart->frameAttributes());
    faChart.setVisible(true);  // 显示框架
    faChart.setPen(QPen(QColor(0x60, 0x60, 0xb0), 8));  // 设置框架画笔
    m_chart->setFrameAttributes(faChart);  // 应用框架属性

    // 设置图表背景属性
    BackgroundAttributes baChart(m_chart->backgroundAttributes());
    baChart.setVisible(true);  // 显示背景
    baChart.setBrush(QColor(0xd0, 0xd0, 0xff));  // 设置背景画刷
    m_chart->setBackgroundAttributes(baChart);  // 应用背景属性

    // 设置图例
    m_legend = new Legend(m_lines, m_chart);  // 创建图例对象

    m_legend->setPosition(Position::South);  // 设置图例位置为底部
    m_legend->setAlignment(Qt::AlignRight);  // 设置图例对齐方式为右对齐
    m_legend->setShowLines(false);  // 不显示图例线条
    m_legend->setTitleText(tr("Legend"));  // 设置图例标题
    m_legend->setOrientation(Qt::Horizontal);  // 设置图例方向为水平

    // 设置图例框架和背景颜色
    const QColor legendColor(0xff, 0xe0, 0x80);  // 图例颜色
    FrameAttributes faLegend(m_legend->frameAttributes());
    faLegend.setVisible(true);  // 显示图例框架
    faLegend.setPen(QPen(legendColor, 1));  // 设置图例框架画笔
    m_legend->setFrameAttributes(faLegend);  // 应用图例框架属性

    BackgroundAttributes baLegend(m_legend->backgroundAttributes());
    baLegend.setVisible(true);  // 显示图例背景
    baLegend.setBrush(legendColor);  // 设置图例背景画刷
    m_legend->setBackgroundAttributes(baLegend);  // 应用图例背景属性

    m_chart->addLegend(m_legend);  // 将图例添加到图表

    // 绘制不同大小的相同图表
    QSize size1 = QSize(200, 200);  // 小图表大小
    QSize size2 = QSize(800, 800);  // 大图表大小
    m_pix1 = drawIntoPixmap(size1, m_chart);  // 绘制小图表
    m_pix2 = drawIntoPixmap(size2, m_chart);  // 绘制大图表
    m_pix2 = m_pix2.scaled(size1);  // 将大图表缩放到小图表大小

    // 创建并显示第一个小图表标签
    m_smallChart1 = new QLabel(this);
    m_smallChart1->setWindowTitle("200x200");
    m_smallChart1->setPixmap(m_pix1);
    m_smallChart1->setFixedSize(m_pix1.size());
    m_smallChart1->move(width() - m_pix1.width() * 2, height() / 2 - m_pix1.height() - 5);
    m_smallChart1->show();

    // 创建并显示第二个小图表标签
    m_smallChart2 = new QLabel(this);
    m_smallChart2->setWindowTitle("800x800 scaled down");
    m_smallChart2->setPixmap(m_pix2);
    m_smallChart2->setFixedSize(m_pix2.size());
    m_smallChart2->move(width() - m_pix2.width() * 2, height() / 2 + 5);
    m_smallChart2->show();

    // 更新图表框架属性
    faChart.setPen(QPen(QColor(0xb0, 0xb0, 0xff), 8));
    m_chart->setFrameAttributes(faChart);

    // 初始化属性
    // 这是必要的，因为我们需要先启用数据值属性，然后才能显示其中的部分（如仅显示标记）
    // 但如果我们启用了数据值属性，即使我们只想设置标记，也会包含默认的数据值文本
    // 所以我们启用DVA，然后单独禁用我们不想要的部分
    on_paintValuesCB_toggled(false);
    on_paintMarkersCB_toggled(false);
}

/**
 * @brief MainWindow::on_lineTypeCB_currentIndexChanged 线型组合框当前索引变化槽函数
 * @param index 选中的索引
 * @details 根据选中的线型更新图表的线条类型
 */
void MainWindow::on_lineTypeCB_currentIndexChanged(int index)
{
    const QString text = lineTypeCB->itemText(index);  // 获取选中的文本
    if (text == "Normal")
        m_lines->setType(LineDiagram::Normal);  // 设置为普通线型
    else if (text == "Stacked")
        m_lines->setType(LineDiagram::Stacked);  // 设置为堆叠线型
    else if (text == "Percent")
        m_lines->setType(LineDiagram::Percent);  // 设置为百分比线型
    else
        qWarning(" Does not match any type");  // 警告：不匹配任何类型
}

/**
 * @brief MainWindow::on_paintLegendCB_toggled 图例绘制复选框状态变化槽函数
 * @param checked 是否选中
 * @details 根据复选框状态显示或隐藏图例
 */
void MainWindow::on_paintLegendCB_toggled(bool checked)
{
    KDChart::Legend *legend = m_chart->legend();  // 获取图表的图例
    if (checked != (legend != nullptr)) {
        if (checked)
            m_chart->addLegend(m_legend);  // 添加图例
        else
            m_chart->takeLegend(legend);  // 移除图例
    }
}

/**
 * @brief MainWindow::on_paintValuesCB_toggled 数据值绘制复选框状态变化槽函数
 * @param checked 是否选中
 * @details 根据复选框状态显示或隐藏数据值
 */
void MainWindow::on_paintValuesCB_toggled(bool checked)
{
    const int colCount = m_lines->model()->columnCount();  // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {
        DataValueAttributes a = m_lines->dataValueAttributes(iColumn);  // 获取数据值属性
        a.setVisible(true);  // 设置数据值属性可见

        TextAttributes ta = a.textAttributes();  // 获取文本属性
        ta.setRotation(0);  // 设置旋转角度为0
        ta.setFont(QFont("Comic", 10));  // 设置字体
        ta.setPen(m_lines->brush(iColumn).color());  // 设置画笔颜色
        ta.setVisible(checked);  // 根据复选框状态设置文本可见性

        a.setTextAttributes(ta);  // 设置文本属性
        m_lines->setDataValueAttributes(iColumn, a);  // 应用数据值属性
    }
}

/**
 * @brief MainWindow::on_paintMarkersCB_toggled 标记绘制复选框状态变化槽函数
 * @param checked 是否选中
 * @details 根据复选框状态显示或隐藏数据点标记
 */
void MainWindow::on_paintMarkersCB_toggled(bool checked)
{
    paintMarkers(checked, QSize());  // 调用绘制标记函数
}

/**
 * @brief MainWindow::on_markersStyleCB_currentIndexChanged 标记样式组合框当前索引变化槽函数
 * @param index 选中的索引
 * @details 根据选中的标记样式更新数据点标记
 */
void MainWindow::on_markersStyleCB_currentIndexChanged(int index)
{
    Q_UNUSED(index);  // 未使用的参数
    on_paintMarkersCB_toggled(paintMarkersCB->isChecked());  // 刷新标记显示
}

/**
 * @brief MainWindow::on_markersWidthSB_valueChanged 标记宽度微调框值变化槽函数
 * @param i 宽度值
 * @details 根据宽度值更新标记的大小
 */
void MainWindow::on_markersWidthSB_valueChanged(int i)
{
    Q_UNUSED(i);  // 未使用的参数
    markersHeightSB->setValue(markersWidthSB->value());  // 同步高度值
    on_paintMarkersCB_toggled(paintMarkersCB->isChecked());  // 刷新标记显示
}

/**
 * @brief MainWindow::on_markersHeightSB_valueChanged 标记高度微调框值变化槽函数
 * @param i 高度值
 * @details 根据高度值更新标记的大小
 */
void MainWindow::on_markersHeightSB_valueChanged(int i)
{
    Q_UNUSED(i);  // 未使用的参数
    markersWidthSB->setValue(markersHeightSB->value());  // 同步宽度值
    on_paintMarkersCB_toggled(paintMarkersCB->isChecked());  // 刷新标记显示
}

/**
 * @brief MainWindow::on_displayAreasCB_toggled 区域显示复选框状态变化槽函数
 * @param checked 是否选中
 * @details 根据复选框状态显示或隐藏区域填充
 */
void MainWindow::on_displayAreasCB_toggled(bool checked)
{
    const int colCount = m_lines->model()->columnCount();  // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {
        LineAttributes la(m_lines->lineAttributes(iColumn));  // 获取线条属性
        la.setDisplayArea(checked);  // 设置区域显示状态
        if (checked)
            la.setTransparency(transparencySB->value());  // 设置透明度
        m_lines->setLineAttributes(iColumn, la);  // 应用线条属性
    }
}

/**
 * @brief MainWindow::on_transparencySB_valueChanged 透明度微调框值变化槽函数
 * @param alpha 透明度值
 * @details 根据透明度值更新区域填充的透明度
 */
void MainWindow::on_transparencySB_valueChanged(int alpha)
{
    Q_UNUSED(alpha);  // 未使用的参数
    if (!displayAreasCB->isChecked())
        displayAreasCB->setChecked(true);  // 如果区域未显示，则显示区域
    else
        on_displayAreasCB_toggled(true);  // 刷新区域显示
}

/**
 * @brief MainWindow::on_zoomFactorSB_valueChanged 缩放因子微调框值变化槽函数
 * @param factor 缩放因子
 * @details 根据缩放因子更新图表的缩放级别
 */
void MainWindow::on_zoomFactorSB_valueChanged(double factor)
{
    const bool isZoomedIn = factor > 1.0f;  // 判断是否放大
    hSBar->setVisible(isZoomedIn);  // 根据缩放状态显示/隐藏水平滚动条
    vSBar->setVisible(isZoomedIn);  // 根据缩放状态显示/隐藏垂直滚动条
    if (!isZoomedIn) {
        hSBar->setValue(500);  // 重置水平滚动条
        vSBar->setValue(500);  // 重置垂直滚动条
    }
    m_chart->coordinatePlane()->setZoomFactorX(factor);  // 设置X轴缩放因子
    m_chart->coordinatePlane()->setZoomFactorY(factor);  // 设置Y轴缩放因子
}

/**
 * @brief MainWindow::on_hSBar_valueChanged 水平滚动条值变化槽函数
 * @param hPos 水平位置
 * @details 根据水平滚动条位置更新图表的缩放中心
 */
void MainWindow::on_hSBar_valueChanged(int hPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hPos / 1000.0, vSBar->value() / 1000.0));  // 设置缩放中心
    m_chart->update();  // 更新图表
}

/**
 * @brief MainWindow::on_vSBar_valueChanged 垂直滚动条值变化槽函数
 * @param vPos 垂直位置
 * @details 根据垂直滚动条位置更新图表的缩放中心
 */
void MainWindow::on_vSBar_valueChanged(int vPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hSBar->value() / 1000.0, vPos / 1000.0));  // 设置缩放中心
}

/**
 * @brief MainWindow::paintMarkers 绘制数据点标记
 * @param checked 是否绘制标记
 * @param printSize 打印大小
 * @details 根据参数设置绘制或隐藏数据点标记，并设置标记的大小和样式
 * // TODO: Qt5.15.2升级 检查MarkerAttributes是否有API变更
 */
void MainWindow::paintMarkers(bool checked, const QSize &printSize)
{
    MarkerAttributes::MarkerStylesMap map;  // 创建标记样式映射
    map.insert(0, MarkerAttributes::MarkerSquare);  // 平方标记
    map.insert(1, MarkerAttributes::MarkerCircle);  // 圆形标记
    map.insert(2, MarkerAttributes::MarkerRing);  // 环形标记
    map.insert(3, MarkerAttributes::MarkerCross);  // 十字标记
    map.insert(4, MarkerAttributes::MarkerDiamond);  // 菱形标记

    // 接下来：指定列/单元格特定的属性！
    const int colCount = m_lines->model()->columnCount();  // 获取列数
    for (int iColumn = 0; iColumn < colCount; ++iColumn) {

        DataValueAttributes dva = m_lines->dataValueAttributes(iColumn);  // 获取数据值属性
        dva.setVisible(true);  // 设置数据值属性可见
        MarkerAttributes ma(dva.markerAttributes());  // 获取标记属性

        // 根据选中的标记样式设置标记
        switch (markersStyleCB->currentIndex()) {
        case 0:
            ma.setMarkerStyle(MarkerAttributes::MarkerSquare);  // 平方标记
            break;
        case 1:
            // 列特定的属性
            ma.setMarkerStyle(map.value(iColumn));  // 使用映射的标记样式
            break;
        case 2:
            ma.setMarkerStyle(MarkerAttributes::MarkerCircle);  // 圆形标记
            break;
        case 3:
            ma.setMarkerStyle(MarkerAttributes::MarkerDiamond);  // 菱形标记
            break;
        case 4:
            ma.setMarkerStyle(MarkerAttributes::Marker1Pixel);  // 1像素标记
            break;
        case 5:
            ma.setMarkerStyle(MarkerAttributes::Marker4Pixels);  // 4像素标记
            break;
        case 6:
            ma.setMarkerStyle(MarkerAttributes::MarkerRing);  // 环形标记
            break;
        case 7:
            ma.setMarkerStyle(MarkerAttributes::MarkerCross);  // 十字标记
            break;
        case 8:
            ma.setMarkerStyle(MarkerAttributes::MarkerFastCross);  // 快速十字标记
            break;
        default:
            Q_ASSERT(false);  // 断言失败
        }
        ma.setVisible(checked);  // 根据复选框状态设置标记可见性

        // 计算缩放因子
        qreal factorWidth = printSize.isValid() ? (printSize.width() / m_chart->rect().width()) : 1.0f;
        qreal factorHeight = printSize.isValid() ? (printSize.height() / m_chart->rect().height()) : 1.0f;
        ma.setMarkerSize(QSize(markersWidthSB->value() * factorWidth, markersHeightSB->value() * factorHeight));  // 设置标记大小

        dva.setMarkerAttributes(ma);  // 设置标记属性
        m_lines->setDataValueAttributes(iColumn, dva);  // 应用数据值属性

        // 为特定值设置特殊标记
        DataValueAttributes yellowAttributes(dva);  // 复制数据值属性
        MarkerAttributes yellowMarker(yellowAttributes.markerAttributes());  // 获取标记属性
        yellowMarker.setMarkerColor(Qt::yellow);  // 设置标记颜色为黄色
        yellowAttributes.setMarkerAttributes(yellowMarker);  // 应用标记属性

        const int rowCount = m_lines->model()->rowCount();  // 获取行数
        for (int j = 0; j < rowCount; ++j) {
            QModelIndex index = m_lines->model()->index(j, iColumn, QModelIndex());  // 获取模型索引
            QBrush brush = m_lines->model()->headerData(iColumn, Qt::Vertical, DatasetBrushRole).value<QBrush>();  // 获取画笔
            qreal value = m_lines->model()->data(index).toReal();  // 获取数据值
            /* 为特定值设置特定颜色的标记 */
            if (value == 13) {
                m_lines->setDataValueAttributes(index, yellowAttributes);  // 应用黄色标记
            }
        }
    }
}

/**
 * @brief MainWindow::on_savePB_clicked 保存按钮点击槽函数
 * @details 将图表保存为PNG图片文件
 */
void MainWindow::on_savePB_clicked()
{
    const QString file = QFileDialog::getSaveFileName(this, tr("Choose PNG File..."));  // 获取保存文件名
    if (file.isEmpty())
        return;  // 如果文件名为空，返回
    qDebug() << "Painting into PNG";
    QPixmap qpix(600, 600);  // 创建600x600的像素图
    QPainter painter(&qpix);  // 创建画家对象
    painter.setBrush(Qt::white);  // 设置白色画刷
    painter.fillRect(0, 0, 600, 600, Qt::white);  // 填充白色背景
    m_chart->paint(&painter, QRect(100, 100, 400, 400));  // 绘制图表
    painter.end();  // 结束绘制
    qpix.save(file, "PNG");  // 保存为PNG文件
    qDebug() << "Painting into PNG - done";
}

/**
 * @brief MainWindow::on_savePDF_clicked 保存PDF按钮点击槽函数
 * @details 将图表保存为PDF文件
 */
void MainWindow::on_savePDF_clicked()
{
    const QString file = QFileDialog::getSaveFileName(this, tr("Choose PDF File..."));  // 获取保存文件名
    if (file.isEmpty())
        return;  // 如果文件名为空，返回
    qDebug() << "Painting into PDF";
    QPrinter printer(QPrinter::HighResolution);  // 创建打印机对象
    printer.setPageOrientation(QPageLayout::Landscape);  // 设置页面方向为横向
    printer.setOutputFormat(QPrinter::PdfFormat);  // 设置输出格式为PDF
    printer.setOutputFileName(file);  // 设置输出文件名
    paintMarkers(paintMarkersCB->isChecked(), printer.pageLayout().paintRectPixels(printer.resolution()).size());  // 绘制标记
    QPainter painter;  // 创建画家对象
    painter.begin(&printer);  // 开始绘制到打印机
    m_chart->paint(&painter, printer.pageLayout().paintRectPixels(printer.resolution()));  // 绘制图表
    painter.end();  // 结束绘制
    paintMarkers(paintMarkersCB->isChecked(), m_chart->geometry().size());  // 绘制标记
    qDebug() << "Painting into PDF - done";
}

/**
 * @brief MainWindow::resizeEvent 重写的窗口大小调整事件处理函数
 * @param event 大小调整事件对象
 * @details 当窗口大小改变时，调整两个小图表的位置
 */
void MainWindow::resizeEvent(QResizeEvent *)
{
    m_smallChart1->move(width() - m_pix1.width() * 2, height() / 2 - m_pix1.height() - 5);  // 移动第一个小图表
    m_smallChart2->move(width() - m_pix2.width() * 2, height() / 2 + 5);  // 移动第二个小图表
}
