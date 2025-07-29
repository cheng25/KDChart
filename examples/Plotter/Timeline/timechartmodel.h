/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件定义了时间图表代理模型类TimeChartModel
// 继承自QSortFilterProxyModel，用于过滤和处理时间轴图表数据
// 提供了设置可见时间范围的功能，可以动态过滤时间序列数据

#ifndef TIMECHARTMODEL_H
#define TIMECHARTMODEL_H

// Qt日期时间处理头文件
// TODO: Qt5.15.2升级 检查QDateTime API是否有变更
#include <QDateTime>
// Qt对数据结构头文件
#include <QPair>
// Qt代理模型头文件
// TODO: Qt5.15.2升级 检查QSortFilterProxyModel API是否有变更
#include <QSortFilterProxyModel>

/**
 * @brief 时间图表代理模型类
 * @details 继承自QSortFilterProxyModel，用于在图表中显示时间范围过滤后的数据
 *          提供了设置可见时间范围的功能，可以过滤出指定时间范围内的数据
 *          支持动态更新可见范围，并自动触发数据重新过滤
 */
class TimeChartModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     * @details 初始化时间图表模型对象，设置默认时间范围为空
     */
    explicit TimeChartModel(QObject *parent = nullptr);

    /**
     * @brief 获取可见时间范围
     * @return 包含开始和结束时间的QPair
     * @details 返回当前设置的可见时间范围
     */
    QPair<QDateTime, QDateTime> visibleRange() const;

public Q_SLOTS:
    /**
     * @brief 设置可见时间范围
     * @param start 开始时间
     * @param end 结束时间
     * @details 设置图表可见的时间范围，过滤出该范围内的数据
     *          如果新范围与旧范围相同则不触发更新
     * @todo C++17升级 使用结构化绑定简化代码
     */
    void setVisibleRange(const QDateTime &start, const QDateTime &end);

    /**
     * @brief 设置可见开始时间
     * @param start 开始时间
     * @details 设置图表可见的开始时间，保持结束时间不变
     */
    void setVisibleStart(const QDateTime &start);

    /**
     * @brief 设置可见结束时间
     * @param end 结束时间
     * @details 设置图表可见的结束时间，保持开始时间不变
     */
    void setVisibleEnd(const QDateTime &end);

    /**
     * @brief 获取数据
     * @param index 数据索引
     * @param role 数据角色
     * @return 数据值
     * @details 重写基类方法，对偶数列的显示数据进行特殊处理
     *          将日期时间转换为从epoch开始的小时数
     * @todo C++17升级 使用std::chrono替代手动时间计算
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    /**
     * @brief 过滤行
     * @param source_row 源模型行号
     * @param source_parent 源模型父索引
     * @return 是否接受该行
     * @details 重写基类方法，根据时间范围过滤数据行
     *          如果日期在可见范围内或范围为空，则接受该行
     */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    /**
     * @brief 可见时间范围
     * @details 存储图表可见的时间范围，包含开始时间和结束时间
     *          默认为空范围，表示显示所有数据
     */
    QPair<QDateTime, QDateTime> range;
};

#endif
