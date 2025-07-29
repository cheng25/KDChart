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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <KDChartDataValueAttributes> // 包含数据值属性类，用于配置图表中数据值的显示方式
#include <TableModel.h>              // 包含表格模型类，用于提供图表数据

namespace KDChart {
class Chart;      // 前向声明Chart类，避免不必要的头文件包含
class BarDiagram; // 前向声明BarDiagram类，避免不必要的头文件包含
}

/**
 * @brief MainWindow 主窗口类
 * @details 该类继承自QWidget和Ui::MainWindow，用于展示和配置图表的数据值文本属性。
 * 提供了丰富的界面控件来调整数据值的显示方式、字体、位置、对齐方式等。
 * 用户可以通过界面交互实时预览数据值文本的显示效果，并应用不同的作用域（单bar、数据集或全局）。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * @details 初始化主窗口，创建图表和数据集，设置UI控件，加载示例数据并初始化图表显示。
     * 连接UI控件的信号与相应的槽函数，以便响应用户交互。
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief on_scopeOneBarRB_toggled 切换单bar作用域
     * @param checked 是否选中
     * @details 当选择"单bar"作用域时触发，设置数据值属性仅应用于单个bar。
     * 此时会启用bar数据集索引和bar项索引的输入框，允许用户指定具体的bar。
     */
    void on_scopeOneBarRB_toggled(bool checked);

    /**
     * @brief on_scopeBarDatasetSB_valueChanged 更改bar数据集索引
     * @param i 数据集索引
     * @details 当调整bar数据集的索引值时触发，更新当前选中的数据集。
     * 索引值范围通常从0开始，对应图表中的不同数据集系列。
     */
    void on_scopeBarDatasetSB_valueChanged(int i);

    /**
     * @brief on_scopeBarItemSB_valueChanged 更改bar项索引
     * @param i 项索引
     * @details 当调整bar项的索引值时触发，更新当前选中的bar项。
     * 索引值范围通常从0开始，对应数据集中的不同数据点。
     */
    void on_scopeBarItemSB_valueChanged(int i);

    /**
     * @brief on_scopeDatasetRB_toggled 切换数据集作用域
     * @param checked 是否选中
     * @details 当选择"数据集"作用域时触发，设置数据值属性应用于整个数据集。
     * 此时会启用数据集索引输入框，允许用户指定具体的数据集。
     */
    void on_scopeDatasetRB_toggled(bool checked);

    /**
     * @brief on_scopeDatasetSB_valueChanged 更改数据集索引
     * @param i 数据集索引
     * @details 当调整数据集的索引值时触发，更新当前选中的数据集。
     * 索引值范围通常从0开始，对应图表中的不同数据集系列。
     */
    void on_scopeDatasetSB_valueChanged(int i);

    /**
     * @brief on_scopeCommonRB_toggled 切换全局作用域
     * @param checked 是否选中
     * @details 当选择"全局"作用域时触发，设置数据值属性应用于整个图表。
     * 此时会禁用所有索引输入框，因为全局设置不需要指定具体的数据集或bar。
     */
    void on_scopeCommonRB_toggled(bool checked);

    /**
     * @brief on_paintValuesCB_toggled 切换数据值显示状态
     * @param checked 是否显示数据值
     * @details 控制是否在图表上显示数据值。选中时显示数据值，取消选中时隐藏数据值。
     */
    void on_paintValuesCB_toggled(bool checked);

    /**
     * @brief on_fontCombo_currentIndexChanged 更改字体
     * @param index 字体索引
     * @details 当选择不同的字体时触发，更新数据值的字体。
     * 字体组合框中包含了系统中可用的字体列表。
     */
    void on_fontCombo_currentIndexChanged(int index);

    /**
     * @brief on_relativeSizeSB_valueChanged 更改相对大小
     * @param i 相对大小百分比
     * @details 调整数据值文本相对于默认大小的百分比。
     * 例如，设置为150表示文本大小为默认大小的150%。
     */
    void on_relativeSizeSB_valueChanged(int i);

    /**
     * @brief on_minimumSizeSB_valueChanged 更改最小大小
     * @param i 最小大小（像素）
     * @details 设置数据值文本的最小像素大小。
     * 即使相对大小设置得很小，文本也不会小于此像素值。
     */
    void on_minimumSizeSB_valueChanged(int i);

    /**
     * @brief on_rotationSB_valueChanged 更改旋转角度
     * @param i 旋转角度（度）
     * @details 调整数据值文本的旋转角度。
     * 角度范围通常为0到359度，0表示不旋转，90表示垂直向上，180表示上下颠倒，270表示垂直向下。
     */
    void on_rotationSB_valueChanged(int i);

    /**
     * @brief on_posPosCombo_currentIndexChanged 更改正值位置
     * @param index 位置索引
     * @details 设置正值数据值的显示位置。
     * 位置选项通常包括：顶部、底部、左侧、右侧、中心等。
     */
    void on_posPosCombo_currentIndexChanged(int index);

    /**
     * @brief on_posAlignCombo_currentIndexChanged 更改正值对齐方式
     * @param index 对齐方式索引
     * @details 设置正值数据值的对齐方式。
     * 对齐方式选项通常包括：左对齐、右对齐、居中对齐等。
     */
    void on_posAlignCombo_currentIndexChanged(int index);

    /**
     * @brief on_posPadHoriSB_valueChanged 更改正值水平填充
     * @param i 水平填充像素
     * @details 调整正值数据值的水平填充距离。
     * 填充距离是指数据值文本与bar之间的水平间距。
     */
    void on_posPadHoriSB_valueChanged(int i);

    /**
     * @brief on_posPadVertSB_valueChanged 更改正值垂直填充
     * @param i 垂直填充像素
     * @details 调整正值数据值的垂直填充距离。
     * 填充距离是指数据值文本与bar之间的垂直间距。
     */
    void on_posPadVertSB_valueChanged(int i);

    /**
     * @brief on_negPosCombo_currentIndexChanged 更改负值位置
     * @param index 位置索引
     * @details 设置负值数据值的显示位置。
     * 位置选项通常包括：顶部、底部、左侧、右侧、中心等。
     */
    void on_negPosCombo_currentIndexChanged(int index);

    /**
     * @brief on_negAlignCombo_currentIndexChanged 更改负值对齐方式
     * @param index 对齐方式索引
     * @details 设置负值数据值的对齐方式。
     * 对齐方式选项通常包括：左对齐、右对齐、居中对齐等。
     */
    void on_negAlignCombo_currentIndexChanged(int index);

    /**
     * @brief on_negPadHoriSB_valueChanged 更改负值水平填充
     * @param i 水平填充像素
     * @details 调整负值数据值的水平填充距离。
     * 填充距离是指数据值文本与bar之间的水平间距。
     */
    void on_negPadHoriSB_valueChanged(int i);

    /**
     * @brief on_negPadVertSB_valueChanged 更改负值垂直填充
     * @param i 垂直填充像素
     * @details 调整负值数据值的垂直填充距离。
     * 填充距离是指数据值文本与bar之间的垂直间距。
     */
    void on_negPadVertSB_valueChanged(int i);

    /**
     * @brief on_labelLE_textEdited 编辑标签文本
     * @param text 标签文本
     * @details 当标签文本编辑框内容改变时触发，更新数据值的标签文本。
     * 标签文本会显示在数据值旁边，用于提供额外信息。
     */
    void on_labelLE_textEdited(const QString &text);

    /**
     * @brief on_prefixLE_textEdited 编辑前缀文本
     * @param text 前缀文本
     * @details 当前缀文本编辑框内容改变时触发，更新数据值的前缀文本。
     * 前缀文本会显示在数据值之前，例如货币符号"$"。
     */
    void on_prefixLE_textEdited(const QString &text);

    /**
     * @brief on_suffixLE_textEdited 编辑后缀文本
     * @param text 后缀文本
     * @details 当后缀文本编辑框内容改变时触发，更新数据值的后缀文本。
     * 后缀文本会显示在数据值之后，例如单位"%"。
     */
    void on_suffixLE_textEdited(const QString &text);

private:
    /**
     * @brief currentIndex 获取当前索引
     * @return 当前选中的模型索引
     * @details 根据当前选择的作用域（单bar、数据集或全局），返回对应的模型索引。
     * 对于全局作用域，返回无效索引。
     */
    const QModelIndex currentIndex() const;

    /**
     * @brief attributes 获取数据值属性
     * @return 当前的数据值属性
     * @details 根据界面控件的当前设置，构建并返回数据值属性对象。
     * 该对象包含了数据值的显示方式、字体、位置、对齐方式等信息。
     */
    const KDChart::DataValueAttributes attributes() const;

    /**
     * @brief positionToScreenName 位置转换为显示名称
     * @param pos KDChart位置枚举
     * @return 对应的显示名称
     * @details 将KDChart::Position枚举值转换为界面显示的名称。
     * 例如，将KDChart::Position::Top转换为"顶部"。
     */
    const char *positionToScreenName(const KDChart::Position &pos) const;

    /**
     * @brief alignmentFromScreeName 从显示名称获取对齐方式
     * @param name 显示名称
     * @return 对应的Qt对齐方式
     * @details 根据界面显示的名称返回对应的Qt::Alignment值。
     * 例如，将"居中对齐"转换为Qt::AlignCenter。
     */
    const Qt::Alignment alignmentFromScreeName(const QString &name) const;

    /**
     * @brief alignmentToScreenName 对齐方式转换为显示名称
     * @param align Qt对齐方式
     * @return 对应的显示名称
     * @details 将Qt::Alignment值转换为界面显示的名称。
     * 例如，将Qt::AlignCenter转换为"居中对齐"。
     */
    const QString alignmentToScreenName(const Qt::Alignment &align) const;

    /**
     * @brief populateWidgets 填充控件
     * @details 初始化界面控件，设置默认值和选项。
     * 例如，填充字体组合框、位置组合框、对齐方式组合框等。
     */
    void populateWidgets();

    /**
     * @brief setAttributes 设置数据值属性
     * @param da 要设置的数据值属性
     * @details 应用指定的数据值属性到图表。
     * 根据当前选择的作用域，将属性应用到单bar、数据集或整个图表。
     */
    void setAttributes(const KDChart::DataValueAttributes &da);

    KDChart::Chart *m_chart;      // 图表对象，用于显示和管理图表
    KDChart::BarDiagram *m_bars;  // 柱状图对象，用于显示柱状图数据
    TableModel m_model;           // 表格数据模型，提供图表所需的数据
};

#endif /* MAINWINDOW_H */
