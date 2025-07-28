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

#include <KDChartAbstractCoordinatePlane> // 包含抽象坐标平面类，用于图表的坐标系统
#include <KDChartBarDiagram>             // 包含柱状图类，用于显示柱状图数据
#include <KDChartChart>                  // 包含图表类，作为图表的容器
#include <KDChartDatasetProxyModel>      // 包含数据集代理模型类，用于处理数据集
#include <KDChartPosition>               // 包含位置类，用于指定图表元素的位置
#include <KDChartRelativePosition>       // 包含相对位置类，用于设置元素的相对位置
#include <KDChartTextAttributes>         // 包含文本属性类，用于设置文本的显示属性

#include <QDebug>                        // 包含调试输出类，用于输出调试信息
#include <QPainter>                      // 包含绘图类，用于绘制图形

using namespace KDChart;                 // 使用KDChart命名空间，简化代码

/**
 * @brief MainWindow::MainWindow 构造函数
 * @param parent 父窗口指针，默认为nullptr
 * @details 初始化主窗口UI，创建图表和数据集，设置初始配置。
 * 包括创建图表布局、加载数据、配置柱状图属性和设置初始UI状态。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);  // 初始化UI组件，由uic自动生成

    // 创建图表布局并添加图表控件
    auto *chartLayout = new QHBoxLayout(chartFrame);  // 创建水平布局
    m_chart = new Chart();  // 创建图表对象
    chartLayout->addWidget(m_chart);  // 将图表添加到布局

    m_model.loadFromCSV(" :/data ");  // 从CSV文件加载数据，路径为资源文件中的data

    // 设置图表
    m_bars = new BarDiagram();  // 创建柱状图对象
    m_bars->setModel(&m_model);  // 设置数据模型为m_model

    // 配置柱状图的画笔
    QPen pen(m_bars->pen());  // 获取当前画笔
    pen.setColor(Qt::darkGray);  // 设置颜色为深灰色
    pen.setWidth(1);             // 设置线宽为1像素
    m_bars->setPen(pen);  // 应用画笔设置
    m_chart->coordinatePlane()->replaceDiagram(m_bars);  // 替换图表的图示为柱状图
    m_chart->setGlobalLeadingTop(20);  // 设置图表顶部边距为20像素

    // 设置初始UI状态
    scopeCommonRB->setFocus(Qt::OtherFocusReason);  // 设置焦点
    scopeCommonRB->setChecked(true);  // 默认选中全局作用域
    paintValuesCB->setChecked(true);  // 默认显示数据值
}

/**
 * @brief MainWindow::on_scopeOneBarRB_toggled 切换单bar作用域
 * @param checked 是否选中
 * @details 当选择单bar作用域时，启用相关控件并更新界面。
 * 单bar作用域允许用户为单个柱状图元素设置数据值属性。
 */
void MainWindow::on_scopeOneBarRB_toggled(bool checked)
{
    if (checked) {
        scopeBarDatasetSB->setDisabled(false);  // 启用数据集索引控件
        scopeBarItemSB->setDisabled(false);     // 启用项索引控件
        scopeDatasetSB->setDisabled(true);      // 禁用数据集作用域控件
        populateWidgets();                      // 更新控件值，反映当前选中bar的属性
    }
}

/**
 * @brief MainWindow::on_scopeBarDatasetSB_valueChanged 更改bar数据集索引
 * @param i 数据集索引
 * @details 当数据集索引改变时，更新界面控件以反映新选中数据集的属性。
 * @note 参数i未直接使用，而是通过populateWidgets()函数间接获取当前索引值。
 */
void MainWindow::on_scopeBarDatasetSB_valueChanged(int i)
{
    Q_UNUSED(i)  // 标记参数i未使用
    populateWidgets();  // 更新控件值
}

/**
 * @brief MainWindow::on_scopeBarItemSB_valueChanged 更改bar项索引
 * @param i 项索引
 * @details 当项索引改变时，更新界面控件以反映新选中项的属性。
 * @note 参数i未直接使用，而是通过populateWidgets()函数间接获取当前索引值。
 */
void MainWindow::on_scopeBarItemSB_valueChanged(int i)
{
    Q_UNUSED(i)  // 标记参数i未使用
    populateWidgets();  // 更新控件值
}

/**
 * @brief MainWindow::on_scopeDatasetRB_toggled 切换数据集作用域
 * @param checked 是否选中
 * @details 当选择数据集作用域时，启用相关控件并更新界面。
 * 数据集作用域允许用户为整个数据集设置数据值属性。
 */
void MainWindow::on_scopeDatasetRB_toggled(bool checked)
{
    if (checked) {
        scopeBarDatasetSB->setDisabled(true);  // 禁用bar数据集索引控件
        scopeBarItemSB->setDisabled(true);     // 禁用bar项索引控件
        scopeDatasetSB->setDisabled(false);    // 启用数据集索引控件
        populateWidgets();                     // 更新控件值，反映当前选中数据集的属性
    }
}

/**
 * @brief MainWindow::on_scopeDatasetSB_valueChanged 更改数据集索引
 * @param i 数据集索引
 * @details 当数据集索引改变时，更新界面控件以反映新选中数据集的属性。
 * @note 参数i未直接使用，而是通过populateWidgets()函数间接获取当前索引值。
 */
void MainWindow::on_scopeDatasetSB_valueChanged(int i)
{
    Q_UNUSED(i)  // 标记参数i未使用
    populateWidgets();  // 更新控件值
}

/**
 * @brief MainWindow::on_scopeCommonRB_toggled 切换全局作用域
 * @param checked 是否选中
 * @details 当选择全局作用域时，禁用相关控件并更新界面。
 * 全局作用域允许用户为整个图表设置数据值属性。
 */
void MainWindow::on_scopeCommonRB_toggled(bool checked)
{
    if (checked) {
        scopeBarDatasetSB->setDisabled(true);  // 禁用bar数据集索引控件
        scopeBarItemSB->setDisabled(true);     // 禁用bar项索引控件
        scopeDatasetSB->setDisabled(true);     // 禁用数据集索引控件
        populateWidgets();                     // 更新控件值，反映全局属性
    }
}

/**
 * @brief MainWindow::on_paintValuesCB_toggled 切换数据值显示
 * @param checked 是否显示
 * @details 控制是否在图表上显示数据值。
 * 当选中时，数据值将显示在图表上；取消选中时，数据值将隐藏。
 */
void MainWindow::on_paintValuesCB_toggled(bool checked)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    da.setVisible(checked);  // 设置数据值可见性
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_fontCombo_currentIndexChanged 更改字体
 * @param index 字体索引
 * @details 当选择不同字体时，更新数据值的字体。
 * 字体组合框包含系统中可用的字体列表，用户可以从中选择。
 */
void MainWindow::on_fontCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    TextAttributes ta(da.textAttributes());  // 获取文本属性
    const QString text = fontCombo->itemText(index);  // 获取选中的字体名称
    QFont font(text);  // 创建字体对象
    ta.setFont(font);  // 设置文本字体
    da.setTextAttributes(ta);  // 更新数据值属性的文本属性
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_relativeSizeSB_valueChanged 更改相对大小
 * @param i 相对大小百分比
 * @details 调整数据值文本的相对大小。
 * 相对大小是相对于图表默认字体大小的百分比，例如100表示默认大小，150表示1.5倍默认大小。
 */
void MainWindow::on_relativeSizeSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    TextAttributes ta(da.textAttributes());  // 获取文本属性
    ta.setFontSize(i);  // 设置字体大小
    da.setTextAttributes(ta);  // 更新数据值属性的文本属性
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_minimumSizeSB_valueChanged 更改最小大小
 * @param i 最小大小（像素）
 * @details 设置数据值文本的最小像素大小。
 * 即使相对大小设置得很小，文本也不会小于此像素值，确保文本的可读性。
 */
void MainWindow::on_minimumSizeSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    TextAttributes ta(da.textAttributes());  // 获取文本属性
    ta.setMinimalFontSize(i);  // 设置最小字体大小
    da.setTextAttributes(ta);  // 更新数据值属性的文本属性
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_rotationSB_valueChanged 更改旋转角度
 * @param i 旋转角度（度）
 * @details 调整数据值文本的旋转角度。
 * 角度范围为0到359度，0表示不旋转，90表示垂直向上，180表示上下颠倒，270表示垂直向下。
 */
void MainWindow::on_rotationSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    TextAttributes ta(da.textAttributes());  // 获取文本属性
    ta.setRotation(i);  // 设置文本旋转角度
    da.setTextAttributes(ta);  // 更新数据值属性的文本属性
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_posPosCombo_currentIndexChanged 更改正值位置
 * @param index 位置索引
 * @details 设置正值数据值的显示位置。
 * 位置选项包括顶部、底部、左侧、右侧、中心等，决定数据值相对于柱状图的位置。
 */
void MainWindow::on_posPosCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.positivePosition());  // 获取正值的相对位置
    const QString text = posPosCombo->itemText(index);  // 获取选中的位置名称
    relPos.setReferencePosition(Position::fromName(qPrintable(text)));  // 设置参考位置
    da.setPositivePosition(relPos);  // 更新正值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_posAlignCombo_currentIndexChanged 更改正值对齐方式
 * @param index 对齐方式索引
 * @details 设置正值数据值的对齐方式。
 * 对齐方式选项包括左对齐、右对齐、居中对齐等，决定数据值文本在其位置上的对齐方式。
 */
void MainWindow::on_posAlignCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.positivePosition());  // 获取正值的相对位置
    const QString text = posAlignCombo->itemText(index);  // 获取选中的对齐方式名称
    relPos.setAlignment(alignmentFromScreeName(text));  // 设置对齐方式
    da.setPositivePosition(relPos);  // 更新正值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_posPadHoriSB_valueChanged 更改正值水平填充
 * @param i 水平填充像素
 * @details 调整正值数据值的水平填充距离。
 * 填充距离是指数据值文本与柱状图之间的水平间距，增加间距可以避免文本与柱状图重叠。
 */
void MainWindow::on_posPadHoriSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.positivePosition());  // 获取正值的相对位置
    Measure pad(relPos.horizontalPadding());  // 获取当前水平填充
    pad.setValue(i);  // 设置新的水平填充值
    relPos.setHorizontalPadding(pad);  // 更新水平填充
    da.setPositivePosition(relPos);  // 更新正值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_posPadVertSB_valueChanged 更改正值垂直填充
 * @param i 垂直填充像素
 * @details 调整正值数据值的垂直填充距离。
 * 填充距离是指数据值文本与柱状图之间的垂直间距，增加间距可以避免文本与柱状图重叠。
 */
void MainWindow::on_posPadVertSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.positivePosition());  // 获取正值的相对位置
    Measure pad(relPos.verticalPadding());  // 获取当前垂直填充
    pad.setValue(i);  // 设置新的垂直填充值
    relPos.setVerticalPadding(pad);  // 更新垂直填充
    da.setPositivePosition(relPos);  // 更新正值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_negPosCombo_currentIndexChanged 更改负值位置
 * @param index 位置索引
 * @details 设置负值数据值的显示位置。
 * 位置选项包括顶部、底部、左侧、右侧、中心等，决定数据值相对于柱状图的位置。
 */
void MainWindow::on_negPosCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.negativePosition());  // 获取负值的相对位置
    const QString text = negPosCombo->itemText(index);  // 获取选中的位置名称
    relPos.setReferencePosition(Position::fromName(qPrintable(text)));  // 设置参考位置
    da.setNegativePosition(relPos);  // 更新负值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_negAlignCombo_currentIndexChanged 更改负值对齐方式
 * @param index 对齐方式索引
 * @details 设置负值数据值的对齐方式。
 * 对齐方式选项包括左对齐、右对齐、居中对齐等，决定数据值文本在其位置上的对齐方式。
 */
void MainWindow::on_negAlignCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.negativePosition());  // 获取负值的相对位置
    const QString text = negAlignCombo->itemText(index);  // 获取选中的对齐方式名称
    relPos.setAlignment(alignmentFromScreeName(text));  // 设置对齐方式
    da.setNegativePosition(relPos);  // 更新负值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_negPadHoriSB_valueChanged 更改负值水平填充
 * @param i 水平填充像素
 * @details 调整负值数据值的水平填充距离。
 * 填充距离是指数据值文本与柱状图之间的水平间距，增加间距可以避免文本与柱状图重叠。
 */
void MainWindow::on_negPadHoriSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.negativePosition());  // 获取负值的相对位置
    Measure pad(relPos.horizontalPadding());  // 获取当前水平填充
    pad.setValue(i);  // 设置新的水平填充值
    relPos.setHorizontalPadding(pad);  // 更新水平填充
    da.setNegativePosition(relPos);  // 更新负值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_negPadVertSB_valueChanged 更改负值垂直填充
 * @param i 垂直填充像素
 * @details 调整负值数据值的垂直填充距离。
 * 填充距离是指数据值文本与柱状图之间的垂直间距，增加间距可以避免文本与柱状图重叠。
 */
void MainWindow::on_negPadVertSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    RelativePosition relPos(da.negativePosition());  // 获取负值的相对位置
    Measure pad(relPos.verticalPadding());  // 获取当前垂直填充
    pad.setValue(i);  // 设置新的垂直填充值
    relPos.setVerticalPadding(pad);  // 更新垂直填充
    da.setNegativePosition(relPos);  // 更新负值位置
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_labelLE_textEdited 更改数据标签
 * @param text 标签文本
 * @details 设置数据值的显示标签。
 * 标签文本会显示在数据值旁边，用于提供额外信息或说明。
 */
void MainWindow::on_labelLE_textEdited(const QString &text)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    da.setDataLabel(text.isEmpty() ? QString() : text);  // 设置数据标签，为空时使用默认值
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_prefixLE_textEdited 更改前缀
 * @param text 前缀文本
 * @details 设置数据值的前缀文本。
 * 前缀文本会显示在数据值之前，例如货币符号"$"或单位缩写。
 */
void MainWindow::on_prefixLE_textEdited(const QString &text)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    da.setPrefix(text.isEmpty() ? QString() : text);  // 设置前缀，为空时使用默认值
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::on_suffixLE_textEdited 更改后缀
 * @param text 后缀文本
 * @details 设置数据值的后缀文本。
 * 后缀文本会显示在数据值之后，例如百分比符号"%"或单位名称。
 */
void MainWindow::on_suffixLE_textEdited(const QString &text)
{
    DataValueAttributes da(attributes());  // 获取当前数据值属性
    da.setSuffix(text.isEmpty() ? QString() : text);  // 设置后缀，为空时使用默认值
    setAttributes(da);  // 应用属性更改

    m_chart->update();  // 更新图表以显示更改
}

/**
 * @brief MainWindow::currentIndex 获取当前索引
 * @return 当前选中的模型索引
 * @details 根据当前选择的作用域和索引值，返回对应的模型索引。
 * 对于单bar作用域，返回选中的bar对应的模型索引；对于其他作用域，返回无效索引。
 */
const QModelIndex MainWindow::currentIndex() const
{
    const int dataset = scopeBarDatasetSB->value();  // 获取数据集索引
    const int item = scopeBarItemSB->value();        // 获取项索引
    return m_bars->model()->index(item, dataset, QModelIndex());  // 返回模型索引
}

/**
 * @brief MainWindow::attributes 获取数据值属性
 * @return 当前数据值属性
 * @details 根据当前选择的作用域，返回对应的数据集属性。
 * 支持单bar、数据集和全局三种作用域。
 */
const KDChart::DataValueAttributes MainWindow::attributes() const
{
    if (scopeOneBarRB->isChecked()) {
        // 返回单个bar的属性
        return m_bars->dataValueAttributes(currentIndex());
    }
    if (scopeDatasetRB->isChecked()) {
        // 返回数据集的属性
        return m_bars->dataValueAttributes(scopeDatasetSB->value());
    }
    // 返回全局属性
    return m_bars->dataValueAttributes();
}

/**
 * @brief MainWindow::setAttributes 设置数据值属性
 * @param da 数据值属性
 * @details 根据当前选择的作用域，设置对应的数据集属性。
 * 支持单bar、数据集和全局三种作用域。
 */
void MainWindow::setAttributes(const KDChart::DataValueAttributes &da)
{
    if (scopeOneBarRB->isChecked())
        m_bars->setDataValueAttributes(currentIndex(), da);  // 设置单个bar的属性
    else if (scopeDatasetRB->isChecked())
        m_bars->setDataValueAttributes(scopeDatasetSB->value(), da);  // 设置数据集的属性
    else
        m_bars->setDataValueAttributes(da);  // 设置全局属性
}

/**
 * @brief MainWindow::positionToScreenName 位置转屏幕名称
 * @param pos 位置对象
 * @return 屏幕显示名称
 * @details 将位置对象转换为适合在界面上显示的名称。
 * 对于未知位置，返回默认值"( Default Value )"。
 */
const char *MainWindow::positionToScreenName(const Position &pos) const
{
    static const char *defaultPositionName = "( Default Value )";
    if (pos.isUnknown())
        return defaultPositionName;  // 未知位置返回默认值
    return pos.name();  // 返回位置名称
}

/**
 * @brief MainWindow::alignmentFromScreeName 屏幕名称转对齐方式
 * @param name 屏幕显示名称
 * @return 对齐方式
 * @details 将界面上显示的对齐方式名称转换为Qt的对齐方式。
 * 支持多种对齐方式，如居中、左下、底部、右下等。
 */
const Qt::Alignment MainWindow::alignmentFromScreeName(const QString &name) const
{
    if (name == "Center")
        return Qt::AlignCenter;  // 居中对齐
    if (name == "BottomLeft")
        return Qt::AlignLeft | Qt::AlignBottom;  // 左下对齐
    if (name == "Bottom")
        return Qt::AlignHCenter | Qt::AlignBottom;  // 底部居中对齐
    if (name == "BottomRight")
        return Qt::AlignRight | Qt::AlignBottom;  // 右下对齐
    if (name == "Right")
        return Qt::AlignRight | Qt::AlignVCenter;  // 右侧居中对齐
    if (name == "TopRight")
        return Qt::AlignRight | Qt::AlignTop;  // 右上对齐
    if (name == "Top")
        return Qt::AlignHCenter | Qt::AlignTop;  // 顶部居中对齐
    if (name == "TopLeft")
        return Qt::AlignLeft | Qt::AlignTop;  // 左上对齐
    if (name == "Left")
        return Qt::AlignLeft | Qt::AlignVCenter;  // 左侧居中对齐
    return Qt::AlignCenter;  // 默认居中对齐
}

/**
 * @brief MainWindow::alignmentToScreenName 对齐方式转屏幕名称
 * @param align 对齐方式
 * @return 屏幕显示名称
 * @details 将Qt的对齐方式转换为适合在界面上显示的名称。
 * 支持多种对齐方式，如居中、左下、底部、右下等。
 */
const QString MainWindow::alignmentToScreenName(const Qt::Alignment &align) const
{
    if (align == Qt::AlignCenter)
        return "Center";  // 居中对齐
    if (align == (Qt::AlignLeft | Qt::AlignBottom))
        return "BottomLeft";  // 左下对齐
    if (align == (Qt::AlignHCenter | Qt::AlignBottom))
        return "Bottom";  // 底部居中对齐
    if (align == (Qt::AlignRight | Qt::AlignBottom))
        return "BottomRight";  // 右下对齐
    if (align == (Qt::AlignRight | Qt::AlignVCenter))
        return "Right";  // 右侧居中对齐
    if (align == (Qt::AlignRight | Qt::AlignTop))
        return "TopRight";  // 右上对齐
    if (align == (Qt::AlignHCenter | Qt::AlignTop))
        return "Top";  // 顶部居中对齐
    if (align == (Qt::AlignLeft | Qt::AlignTop))
        return "TopLeft";  // 左上对齐
    if (align == (Qt::AlignLeft | Qt::AlignVCenter))
        return "Left";  // 左侧居中对齐
    return "Center";  // 默认居中对齐
}

/**
 * @brief MainWindow::populateWidgets 填充控件
 * @details 根据当前数据值属性更新界面控件的值。
 * 确保界面控件的状态与图表的数据值属性保持一致。
 */
void MainWindow::populateWidgets()
{
    const DataValueAttributes da(attributes());  // 获取当前数据值属性
    const TextAttributes ta(da.textAttributes());  // 获取文本属性
    const RelativePosition posPos(da.positivePosition());  // 获取正值的相对位置
    const RelativePosition negPos(da.negativePosition());  // 获取负值的相对位置

    paintValuesCB->setChecked(da.isVisible() && ta.isVisible());  // 设置显示数据值复选框
    fontCombo->setCurrentFont(ta.font());  // 设置字体下拉框
    relativeSizeSB->setValue(static_cast<int>(ta.fontSize().value()));  // 设置相对大小
    minimumSizeSB->setValue(static_cast<int>(ta.minimalFontSize().value()));  // 设置最小大小
    rotationSB->setValue(static_cast<int>(ta.rotation()));  // 设置旋转角度

    posPosCombo->setCurrentIndex(posPosCombo->findText(
        positionToScreenName(posPos.referencePosition())));  // 设置正值位置
    posAlignCombo->setCurrentIndex(posAlignCombo->findText(
        alignmentToScreenName(posPos.alignment())));  // 设置正值对齐方式
    posPadHoriSB->setValue(static_cast<int>(posPos.horizontalPadding().value()));  // 设置正值水平填充
    posPadVertSB->setValue(static_cast<int>(posPos.verticalPadding().value()));  // 设置正值垂直填充

    negPosCombo->setCurrentIndex(negPosCombo->findText(positionToScreenName(
        negPos.referencePosition())));  // 设置负值位置
    negAlignCombo->setCurrentIndex(negAlignCombo->findText(
        alignmentToScreenName(negPos.alignment())));  // 设置负值对齐方式
    negPadHoriSB->setValue(static_cast<int>(negPos.horizontalPadding().value()));  // 设置负值水平填充
    negPadVertSB->setValue(static_cast<int>(negPos.verticalPadding().value()));  // 设置负值垂直填充

    labelLE->setText(da.dataLabel());  // 设置标签文本
    prefixLE->setText(da.prefix());  // 设置前缀文本
    suffixLE->setText(da.suffix());  // 设置后缀文本
}
