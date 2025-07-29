/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** 原文注释: This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** 原文注释: SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 原文注释: SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef ENTRYDELEGATE_H
#define ENTRYDELEGATE_H

#include <QItemDelegate>

namespace KDGantt {
class ConstraintModel;
}

/**
 * @class EntryDelegate
 * @brief Gantt图表条目的委托类
 * 负责处理Gantt图表中条目的编辑事件、约束关系管理和只读状态设置
 * 继承自QItemDelegate，提供自定义的编辑功能
 */
class EntryDelegate : public QItemDelegate
{
public:
    /**
     * @brief 构造函数，初始化EntryDelegate
     * @param constraintModel 约束模型指针，用于管理条目间的依赖关系
     * @param parent 父对象指针
     */
    EntryDelegate(KDGantt::ConstraintModel *constraintModel, QObject *parent = 0);

    /**
     * @brief 重写editorEvent方法，处理编辑事件
     * @param event 事件对象
     * @param model 数据模型
     * @param option 视图项样式选项
     * @param index 模型索引
     * @return 如果事件被处理则返回true，否则返回false
     */
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    /**
     * @brief 添加两个条目之间的约束关系
     * @param index1 源条目索引
     * @param index2 目标条目索引
     */
    void addConstraint(const QModelIndex &index1, const QModelIndex &index2);
    /**
     * @brief 设置条目是否为只读状态
     * @param index 条目模型索引
     * @param readOnly 如果为true则设置为只读，否则可编辑
     */
    void setReadOnly(const QModelIndex &index, bool readOnly);

    /**
     * @brief 约束模型指针
     * 用于管理Gantt图表中条目之间的依赖关系
     */
    KDGantt::ConstraintModel *constraintModel;
};

#endif /* ENTRYDELEGATE_H */
