/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是甘特图图例示例的一部分，定义了EntryDelegate类
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#ifndef ENTRYDELEGATE_H
#define ENTRYDELEGATE_H

#include <QItemDelegate>  // 包含QItemDelegate类定义
// TODO: Qt5.15.2升级 检查QItemDelegate在Qt5.15.2中的变化
// C++17 兼容性: 头文件包含方式符合C++17标准

namespace KDGantt {
class ConstraintModel;  // 前向声明KDGantt::ConstraintModel类
}

/**
 * @brief EntryDelegate 类
 * @details 该类继承自QItemDelegate，用于自定义甘特图中条目的显示和编辑
 * 主要功能包括：
 * - 处理编辑器事件
 * - 添加约束关系
 * - 设置项目的只读状态
 */
class EntryDelegate : public QItemDelegate
{
public:
    /**
     * @brief 构造函数
     * @param constraintModel 约束模型指针
     * @param parent 父对象指针，默认为nullptr
     * @details 初始化EntryDelegate对象，保存约束模型指针
     */
    explicit EntryDelegate(KDGantt::ConstraintModel *constraintModel, QObject *parent = nullptr);

    /**
     * @brief 处理编辑器事件
     * @param event 事件对象
     * @param model 数据模型
     * @param option 样式选项
     * @param index 模型索引
     * @return 如果事件被处理则返回true，否则返回false
     * @details 重写基类方法，处理鼠标事件等，实现自定义编辑行为
     * C++17 兼容性: 使用override关键字明确表示重写基类方法，符合C++17标准
     */
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    /**
     * @brief 添加约束
     * @param index1 第一个项目的索引
     * @param index2 第二个项目的索引
     * @details 在两个项目之间添加约束关系
     */
    void addConstraint(const QModelIndex &index1, const QModelIndex &index2);

    /**
     * @brief 设置只读状态
     * @param index 项目索引
     * @param readOnly 是否只读
     * @details 设置指定项目是否为只读状态
     */
    void setReadOnly(const QModelIndex &index, bool readOnly);

    KDGantt::ConstraintModel *constraintModel;  // 约束模型指针，用于管理甘特图中的依赖关系
};  // C++17 兼容性: 类定义末尾的分号符合C++17标准

#endif /* ENTRYDELEGATE_H */  // 条件编译结束，防止头文件重复包含
