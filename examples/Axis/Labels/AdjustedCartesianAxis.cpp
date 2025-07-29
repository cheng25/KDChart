/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "AdjustedCartesianAxis.h"

/**
 * @brief 构造函数实现
 * @param diagram 关联的图表对象
 * @return 无返回值
 * @details 初始化自定义坐标轴对象，设置默认边界值为0.0
 */
AdjustedCartesianAxis::AdjustedCartesianAxis(KDChart::AbstractCartesianDiagram *diagram)
    : CartesianAxis(diagram), m_lowerBound(0.0), m_upperBound(0.0)
{
    // 初始化基类构造函数
    // 设置坐标轴默认属性
    setAllowOverlappingDataLabels(false);
}

/**
 * @brief 自定义标签文本实现
 * @param label 原始标签文本
 * @return 格式化后的标签文本
 * @details 根据设置的边界值过滤标签，超出范围的标签返回空字符串
 */
const QString AdjustedCartesianAxis::customizedLabel(const QString &label) const
{
    // 将字符串标签转换为数值
    bool ok = false;
    const qreal value = label.toDouble(&ok);
    
    // 如果转换失败或值超出边界范围，则返回空字符串
    if (!ok || value < m_lowerBound || value > m_upperBound)
        return QString();
    
    // TODO: C++17升级 使用std::string_view优化字符串处理
    // 返回原始标签文本
    return label;
}
