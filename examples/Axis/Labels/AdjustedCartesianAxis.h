/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的坐标轴标签调整示例的头文件，定义了AdjustedCartesianAxis类
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#ifndef ADJUSTED_CARTESIAN_AXIS_H
#define ADJUSTED_CARTESIAN_AXIS_H

#include <KDChartCartesianAxis>  // 包含KDChart笛卡尔坐标轴基类
#include <KDChartGlobal>         // 包含KDChart全局定义

/**
 * @brief 自定义笛卡尔坐标轴类
 * 继承自KDChart::CartesianAxis，用于演示坐标轴标签自定义和边界控制
 * 核心功能：提供坐标轴显示范围限制，支持自定义标签格式化
 * 
 * Qt5.15.2升级说明：
 * - 基类KDChart::CartesianAxis在Qt5.15.2中接口保持稳定
 * - Q_OBJECT宏及元对象系统功能完全兼容Qt5.15.2
 * 
 * C++17升级说明：
 * - 已使用override关键字明确重写基类方法
 * - 可考虑使用constexpr优化成员变量初始化
 */
// TODO: Qt5.15.2升级 检查KDChart::CartesianAxis在Qt5.15.2中的坐标系计算逻辑变更
class AdjustedCartesianAxis : public KDChart::CartesianAxis
{
    Q_OBJECT

    Q_DISABLE_COPY(AdjustedCartesianAxis)  // 禁用拷贝构造函数和赋值操作符

public:
    /**
     * @brief 构造函数
     * @param diagram 关联的图表对象，用于坐标计算和数据关联
     * @return 无返回值
     * @details 初始化坐标轴对象并绑定到指定图表，设置默认边界值为0.0
     */
    explicit AdjustedCartesianAxis(KDChart::AbstractCartesianDiagram *diagram = nullptr);

    /**
     * @brief 自定义坐标轴标签文本
     * @param label 原始标签文本（通常为数值字符串）
     * @return 格式化后的标签文本
     * @details 重写基类方法，可实现标签文本的自定义格式化，如添加单位、科学计数法转换等
     */
    const QString customizedLabel(const QString &label) const override;

    /**
     * @brief 设置坐标轴显示范围边界
     * @param lower 下边界值（包含）
     * @param upper 上边界值（包含）
     * @return 无返回值
     * @details 超出此范围的数据点将不会在坐标轴上显示
     */
    void setBounds(qreal lower, qreal upper)
    {
        m_lowerBound = lower;  // 设置下边界值
        m_upperBound = upper;  // 设置上边界值
    }
    
    /**
     * @brief 获取下边界值
     * @return 当前下边界值（qreal类型）
     * @details 用于外部查询当前坐标轴显示范围的下限
     */
    qreal lowerBound() const
    {
        return m_lowerBound;  // 返回存储的下边界值
    }
    
    /**
     * @brief 获取上边界值
     * @return 当前上边界值（qreal类型）
     * @details 用于外部查询当前坐标轴显示范围的上限
     */
    qreal upperBound() const
    {
        return m_upperBound;  // 返回存储的上边界值
    }

private:
    qreal m_lowerBound = 0.0;  // 坐标轴下边界值，默认为0.0
    qreal m_upperBound = 0.0;  // 坐标轴上边界值，默认为0.0
};

#endif // ADJUSTED_CARTESIAN_AXIS_H
