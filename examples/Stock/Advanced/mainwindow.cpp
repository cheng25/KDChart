/****************************************************************************
**
** 此文件是KD Chart库的示例程序的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议: MIT
**
****************************************************************************/

// 包含主窗口头文件
#include "mainwindow.h"
// 包含KDChart笛卡尔坐标轴头文件
#include <KDChartCartesianAxis>
// 包含KDChart图表头文件
#include <KDChartChart>
// 包含KDChart图例头文件
#include <KDChartLegend>
// 包含Qt颜色对话框头文件
#include <QColorDialog>

// 使用KDChart命名空间
using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口指针
 * 初始化图表对象、数据模型，并设置UI和信号槽连接
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_chart(new Chart())
    , m_diagram(m_chart)
{
    // 设置UI
    setupUi(this);

    // 从CSV文件加载HLC数据
    m_HLCModel.loadFromCSV(":/HLC");
    // 从CSV文件加载OHLC数据
    m_OHLCModel.loadFromCSV(":/OHLC");

    // 设置图表类型为最高价-最低价-收盘价
    m_diagram.setType(StockDiagram::HighLowClose);
    // 设置图表数据模型为HLC模型
    m_diagram.setModel(&m_HLCModel);
    // 替换图表坐标系中的图表
    m_chart->coordinatePlane()->replaceDiagram(&m_diagram);
    // 创建图例
    auto *legend = new KDChart::Legend(&m_diagram, m_chart);
    // 添加图例到图表
    m_chart->addLegend(legend);

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    // 添加图表到布局
    chartLayout->addWidget(m_chart);

    // 创建左侧Y轴
    auto *leftAxis = new CartesianAxis(&m_diagram);
    // 创建底部X轴
    auto *bottomAxis = new CartesianAxis(&m_diagram);

    // 设置左侧Y轴位置
    leftAxis->setPosition(CartesianAxis::Left);

    // 获取底部X轴的文本属性
    TextAttributes attributes = bottomAxis->textAttributes();
    // 设置文本旋转90度
    attributes.setRotation(90);
    // 设置字体大小为绝对7.0
    attributes.setFontSize(Measure(7.0, KDChartEnums::MeasureCalculationModeAbsolute));
    // 应用文本属性到X轴
    bottomAxis->setTextAttributes(attributes);
    // 设置X轴位置
    bottomAxis->setPosition(CartesianAxis::Bottom);
    // 添加Y轴到图表
    m_diagram.addAxis(leftAxis);
    // 添加X轴到图表
    m_diagram.addAxis(bottomAxis);
    // 再次添加X轴（可能是冗余的）
    m_diagram.addAxis(bottomAxis);
    // 应用初始颜色
    applyColor(QColor("chartreuse"));
    // 连接颜色选择按钮的点击信号到chooseColor槽函数
    const bool connected = connect(colorChooser, &QPushButton::clicked, this, &MainWindow::chooseColor);
    // 确保连接成功
    Q_ASSERT(connected);
    // 避免未使用变量警告
    Q_UNUSED(connected);

    // 初始化股票图表的所有值为合理默认值
    initValues();
}

/**
 * @brief 选择颜色槽函数
 * 弹出颜色对话框，让用户选择图表颜色
 */
void MainWindow::chooseColor()
{
    // 弹出颜色对话框，初始颜色为当前图表画笔颜色
    applyColor(QColorDialog::getColor(m_diagram.brush().color(), this));
}

/**
 * @brief 应用颜色到图表
 * @param color 要应用的颜色
 * 设置图表的画笔和画刷颜色，并更新颜色选择按钮的颜色
 */
void MainWindow::applyColor(const QColor &color)
{
    // 检查颜色是否有效
    if (color.isValid()) {
        // 设置第一个数据集的画笔为颜色的深色调
        m_diagram.setPen(0, QPen(color.darker(130)));
        // 设置第一个数据集的画刷为颜色
        m_diagram.setBrush(0, QBrush(color));
        // 计算反色
        QColor inverse(255 - color.red(), 255 - color.green(), 255 - color.blue());
        // 设置第二个数据集的画笔为反色的深色调
        m_diagram.setPen(1, QPen(inverse.darker(130)));
        // 设置第二个数据集的画刷为反色
        m_diagram.setBrush(1, QBrush(inverse));
        // 获取颜色选择按钮的调色板
        QPalette pal = colorChooser->palette();
        // 设置按钮背景色
        pal.setBrush(QPalette::Button, QBrush(color));
        // 应用调色板到按钮
        colorChooser->setPalette(pal);
    }
}

/**
 * @brief 初始化股票图表的值
 * 设置3D柱状图属性的默认值，并同步UI控件状态
 */
void MainWindow::initValues()
{
    // 获取图表的3D柱状图属性
    m_threeDBarAttributes = m_diagram.threeDBarAttributes();
    // 设置深度为10.0
    m_threeDBarAttributes.setDepth(10.0);
    // 不使用阴影颜色
    m_threeDBarAttributes.setUseShadowColors(false);
    // 设置3D属性复选框状态
    threeDProperties->setChecked(m_threeDBarAttributes.isEnabled());
    // 设置透视角度滑块值
    perspectiveAngle->setValue(m_threeDBarAttributes.angle());
    // 设置透视深度滑块值
    perspectiveDepth->setValue(( int )m_threeDBarAttributes.depth());
    // 设置阴影颜色复选框状态
    useShadowColors->setChecked(m_threeDBarAttributes.useShadowColors());
    // 应用3D属性到图表
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
}

/**
 * @brief 3D属性切换槽函数
 * @param checked 是否启用3D效果
 * 更新图表的3D效果状态
 */
void MainWindow::on_threeDProperties_toggled(bool checked)
{
    // 设置3D属性启用状态
    m_threeDBarAttributes.setEnabled(checked);
    // 应用3D属性到图表
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
    // 更新图表
    m_chart->update();
}

/**
 * @brief 透视角度值变化槽函数
 * @param value 透视角度值
 * 更新图表的透视角度
 */
void MainWindow::on_perspectiveAngle_valueChanged(int value)
{
    // 设置透视角度
    m_threeDBarAttributes.setAngle(value);
    // 应用3D属性到图表
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
    // 更新图表
    m_chart->update();
}

/**
 * @brief 透视深度值变化槽函数
 * @param value 透视深度值
 * 更新图表的透视深度
 */
void MainWindow::on_perspectiveDepth_valueChanged(int value)
{
    // 设置透视深度
    m_threeDBarAttributes.setDepth(value);
    // 应用3D属性到图表
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
    // 更新图表
    m_chart->update();
}

/**
 * @brief 阴影颜色切换槽函数
 * @param checked 是否使用阴影颜色
 * 更新图表的阴影颜色状态
 */
void MainWindow::on_useShadowColors_toggled(bool checked)
{
    // 设置是否使用阴影颜色
    m_threeDBarAttributes.setUseShadowColors(checked);
    // 应用3D属性到图表
    m_diagram.setThreeDBarAttributes(m_threeDBarAttributes);
    // 更新图表
    m_chart->update();
}

/**
 * @brief 股票类型下拉框变化槽函数
 * @param index 选中项的索引
 * 根据选中的股票类型更新图表类型和数据模型
 */
void MainWindow::on_stockTypeCB_currentIndexChanged(int index)
{
    // 临时解决方案：避免设置新模型时图表消失
    m_chart->coordinatePlane()->takeDiagram(&m_diagram);

    // 获取选中项的文本
    const QString text = stockTypeCB->itemText(index);
    // 根据文本设置图表类型和模型
    if (text == "High-Low-Close") {
        // 设置为最高价-最低价-收盘价类型
        m_diagram.setType(StockDiagram::HighLowClose);
        // 设置HLC数据模型
        m_diagram.setModel(&m_HLCModel);
    } else if (text == "Open-High-Low-Close") {
        // 设置为开盘价-最高价-最低价-收盘价类型
        m_diagram.setType(StockDiagram::OpenHighLowClose);
        // 设置OHLC数据模型
        m_diagram.setModel(&m_OHLCModel);
    } else if (text == "Candlestick") {
        // 设置为蜡烛图类型
        m_diagram.setType(StockDiagram::Candlestick);
        // 设置OHLC数据模型
        m_diagram.setModel(&m_OHLCModel);
    }

    // 将图表重新添加到坐标系
    m_chart->coordinatePlane()->replaceDiagram(&m_diagram);
}

// TODO: Qt5.15.2升级 检查KDChart API是否有变更
// TODO: Qt5.15.2升级 检查QColorDialog API是否有变更
// TODO: C++17升级 考虑使用std::make_unique管理动态内存
// TODO: C++17升级 考虑使用结构化绑定优化代码
// TODO: C++17升级 考虑使用if constexpr优化条件判断
