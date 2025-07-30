/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件定义了自定义时间轴类TimeAxis
// 继承自KDChart::CartesianAxis，用于在图表中显示时间相关的坐标轴

#ifndef TIMEAXIS_H
#define TIMEAXIS_H

// 笛卡尔坐标轴头文件
// TODO: Qt5.15.2升级 检查KDChartCartesianAxis API是否有变更
#include <KDChartCartesianAxis>

/**
 * @brief 自定义时间轴类
 * @details 继承自KDChart::CartesianAxis，用于在图表中显示时间相关的坐标轴
 *          提供了自定义标签显示的功能，可以将时间戳格式化为更友好的时间字符串
 */
class TimeAxis : public KDChart::CartesianAxis
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父图表对象
     * @details 初始化时间轴对象，并设置父图表
     * @note 时间轴将自动处理时间戳格式化为可读的日期时间字符串
     */
    explicit TimeAxis(KDChart::AbstractCartesianDiagram *parent);

    /**
     * @brief 自定义标签文本
     * @param label 原始标签文本
     * @return 格式化后的标签文本
     * @details 重写基类方法，将时间戳格式化为更友好的时间字符串
     *          时间戳单位为小时，需要乘以3600转换为秒
     * @todo C++17升级 考虑使用std::from_chars替代toDouble提高性能
     */
    const QString customizedLabel(const QString &label) const override;
};

#endif
