/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件实现了时间图表代理模型类TimeChartModel的方法
// 包含构造函数、可见范围管理、数据处理和过滤方法

#include "timechartmodel.h"

/**
 * @brief TimeChartModel类构造函数
 * @param parent 父对象
 * @details 初始化时间图表模型对象，调用基类构造函数
 *          设置默认可见范围为空（显示所有数据）
 */
TimeChartModel::TimeChartModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    // 构造函数体为空，仅调用基类构造函数
    // 初始化时range默认为空范围
}

/**
 * @brief 获取可见时间范围
 * @return 包含开始和结束时间的QPair
 * @details 返回当前设置的可见时间范围
 *          如果开始时间或结束时间为空，表示不限制该方向的范围
 */
QPair<QDateTime, QDateTime> TimeChartModel::visibleRange() const
{
    return range;
}

/**
 * @brief 设置可见时间范围
 * @param start 开始时间
 * @param end 结束时间
 * @details 设置图表可见的时间范围，如果新范围与旧范围相同则不更新
 *          设置后会使过滤器失效，触发重新过滤
 * @todo C++17升级 使用结构化绑定简化代码
 */
void TimeChartModel::setVisibleRange(const QDateTime &start, const QDateTime &end)
{
    const QPair<QDateTime, QDateTime> r = qMakePair(start, end);
    if (r == range)
        return;

    range = r;
    // 使过滤器失效，触发重新过滤
    // TODO: Qt5.15.2升级 检查invalidateFilter是否有变更
    invalidateFilter();
}

/**
 * @brief 设置可见开始时间
 * @param start 开始时间
 * @details 设置图表可见的开始时间，保持结束时间不变
 */
void TimeChartModel::setVisibleStart(const QDateTime &start)
{
    setVisibleRange(start, range.second);
}

/**
 * @brief 设置可见结束时间
 * @param end 结束时间
 * @details 设置图表可见的结束时间，保持开始时间不变
 */
void TimeChartModel::setVisibleEnd(const QDateTime &end)
{
    setVisibleRange(range.first, end);
}

/**
 * @brief 获取数据
 * @param index 数据索引
 * @param role 数据角色
 * @return 数据值
 * @details 重写基类方法，对偶数列的显示数据进行特殊处理
 *          将日期时间转换为从epoch开始的小时数
 * @todo C++17升级 使用std::chrono替代手动时间计算
 */
QVariant TimeChartModel::data(const QModelIndex &index, int role) const
{
    // 调用基类方法获取数据
    const QVariant v = QSortFilterProxyModel::data(index, role);
    // 检查是否为偶数列且为显示角色
    if (index.column() % 2 != 0 || role != Qt::DisplayRole)
        return v;
    else {
        // 将日期时间转换为从epoch开始的小时数
        // TODO: Qt5.15.2升级 检查QDateTime::fromSecsSinceEpoch是否有变更
        return QDateTime::fromSecsSinceEpoch(0).secsTo(v.toDateTime()) / 3600.0;
    }
}

/**
 * @brief 过滤行
 * @param source_row 源模型行号
 * @param source_parent 源模型父索引
 * @return 是否接受该行
 * @details 重写基类方法，根据时间范围过滤数据行
 *          如果日期在可见范围内或范围为空，则接受该行
 *          空范围表示不限制该方向的数据
 */
bool TimeChartModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // 获取源模型中的索引
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    // 获取日期时间数据
    const QDateTime date = index.data().toDateTime();
    // 检查是否在可见范围内
    return (date >= range.first || range.first.isNull()) && (date <= range.second || range.second.isNull());
}
