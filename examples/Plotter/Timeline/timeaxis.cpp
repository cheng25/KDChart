/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件实现了自定义时间轴类TimeAxis的方法
// 包含构造函数和自定义标签格式化方法

#include "timeaxis.h"

// Qt日期时间处理头文件
// TODO: Qt5.15.2升级 检查QDateTime API是否有变更
#include <QtCore/QDateTime>

/**
 * @brief TimeAxis类构造函数
 * @param parent 父图表对象
 * @details 初始化时间轴对象，调用基类构造函数
 * @note 构造函数体为空，仅进行初始化
 */
TimeAxis::TimeAxis(KDChart::AbstractCartesianDiagram *parent)
    : KDChart::CartesianAxis(parent)
{
    // 故意留白，仅调用基类构造函数
}

/**
 * @brief 自定义标签文本
 * @param label 原始标签文本（时间戳）
 * @return 格式化后的日期字符串
 * @details 将时间戳转换为QDateTime对象，然后格式化为日期字符串
 *          时间戳单位为小时，需要乘以3600转换为秒
 * @todo C++17升级 使用std::from_chars替代toDouble提高性能
 */
const QString TimeAxis::customizedLabel(const QString &label) const
{
    // 这里我们可以自由地将值格式化为任何我们想要的形式。例如，
    // 我们也可以返回科学计数法，如下所示：
    // const int precision = 2;
    // return QString::number(label.toReal(), 'E', precision);

    // 将时间值格式化为美观的字符串表示
    // TODO: Qt5.15.2升级 检查fromSecsSinceEpoch是否有替代方法
    const QDateTime dateTime = QDateTime::fromSecsSinceEpoch(label.toDouble() * 3600.0);
    // 返回日期字符串
    return dateTime.date().toString();
}