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
 * @brief 自定义的笛卡尔坐标轴类
 * 继承自KDChart::CartesianAxis，用于演示如何自定义坐标轴标签
 * 提供了边界设置功能，可以限制坐标轴的显示范围
 * 
 * Qt5.15.2升级说明：
 * - 基类KDChart::CartesianAxis在Qt5.15.2中保持兼容
 * - Q_OBJECT宏和相关Qt元对象系统功能在Qt5.15.2中正常工作
 * 
 * C++17升级说明：
 * - 此代码与C++17标准完全兼容
 * - 可考虑使用C++17的内联变量初始化特性优化成员变量定义
 */
class AdjustedCartesianAxis : public KDChart::CartesianAxis
{
    Q_OBJECT

    Q_DISABLE_COPY(AdjustedCartesianAxis)  // 禁用拷贝构造函数和赋值操作符

public:
    /**
     * @brief 构造函数
     * @param diagram 关联的图表对象，默认为nullptr
     * @return 无返回值
     * @details 初始化自定义坐标轴对象，并关联到指定的图表
     */
    explicit AdjustedCartesianAxis(KDChart::AbstractCartesianDiagram *diagram = nullptr);

    /**
     * @brief 自定义坐标轴标签
     * @param label 原始标签文本
     * @return 调整后的标签文本
     * @details 重写基类方法，用于自定义坐标轴标签的显示内容
     */
    const QString customizedLabel(const QString &label) const override;

    /**
     * @brief 设置坐标轴边界
     * @param lower 下边界值
     * @param upper 上边界值
     * @return 无返回值
     * @details 设置坐标轴的显示范围，超出此范围的数据将不会显示
     */
    void setBounds(qreal lower, qreal upper)
    {
        m_lowerBound = lower;
        m_upperBound = upper;
    }
    
    /**
     * @brief 获取下边界值
     * @return 下边界值
     * @details 返回当前设置的坐标轴下边界
     */
    qreal lowerBound() const
    {
        return m_lowerBound;
    }
    
    /**
     * @brief 获取上边界值
     * @return 上边界值
     * @details 返回当前设置的坐标轴上边界
     */
    qreal upperBound() const
    {
        return m_upperBound;
    }

private:
    qreal m_lowerBound = 0.0;  // 坐标轴下边界值
    qreal m_upperBound = 0.0;  // 坐标轴上边界值
};

#endif // ADJUSTED_CARTESIAN_AXIS_H
