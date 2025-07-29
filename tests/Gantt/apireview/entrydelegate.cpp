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

#include "entrydelegate.h"

#include "entrydialog.h"

#include <KDGanttConstraintModel>
#include <KDGanttGlobal>
#include <QEvent>
#include <QModelIndex>
#include <QPointer>
#include <QStandardItemModel>

/**
 * @class EntryDelegate
 * @brief 用于Gantt图表条目的委托类
 * 负责处理Gantt图表中条目的编辑、约束管理和只读状态设置
 */
EntryDelegate::EntryDelegate(KDGantt::ConstraintModel *constraintModel, QObject *parent)
    : QItemDelegate(parent)  // 调用父类QItemDelegate的构造函数，设置父对象
{
    this->constraintModel = constraintModel;  // 初始化约束模型成员变量，保存约束模型指针
}

/**
 * @brief 处理编辑器事件，主要响应双击事件以打开编辑对话框
 * @param event 事件对象
 * @param model 数据模型
 * @param option 视图项选项
 * @param index 模型索引
 * @return 如果事件被处理则返回true，否则返回false
 */
bool EntryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // 如果不是鼠标双击事件，则不处理，返回false
    if (event->type() != QEvent::MouseButtonDblClick)
        return false;

    // 如果索引无效（如超出模型范围），则调用父类默认处理逻辑
    if (!index.isValid())
        return QItemDelegate::editorEvent(event, model, option, index);

    // 创建条目编辑对话框，指定数据模型为父对象以自动管理生命周期
    QPointer<EntryDialog> dialog = new EntryDialog(model);
    // 从模型索引初始化对话框数据，包括任务名称、类型、时间等信息
    dialog->initFrom(index, constraintModel);
    // 设置对话框标题为"编辑条目"
    dialog->setWindowTitle(tr("Edit Entry"));
    // 以模态方式显示对话框，等待用户操作
    dialog->exec();
    // 如果对话框已被销毁（如意外关闭），则返回false表示未处理
    if (!dialog)
        return false;

    // 获取当前行号和父索引，用于定位模型中的数据项
    int row = index.row();
    const QModelIndex parent = index.parent();
    // 更新模型中的名称数据（第0列）
    model->setData(model->index(row, 0, parent), dialog->name());
    // 更新模型中的类型数据（第1列）
    model->setData(model->index(row, 1, parent), dialog->type());
    // 如果不是摘要类型（摘要类型无具体时间），则更新开始和结束日期（第2、3列）
    if (dialog->type() != KDGantt::TypeSummary) {
        model->setData(model->index(row, 2, parent), dialog->startDate());
        model->setData(model->index(row, 3, parent), dialog->endDate());
    }
    // 更新完成度数据（第4列）
    model->setData(model->index(row, 4, parent), dialog->completion());
    // 更新图例数据（第5列）
    model->setData(model->index(row, 5, parent), dialog->legend());

    // 添加依赖约束关系，从对话框获取依赖项索引和当前项索引
    addConstraint(dialog->depends(), model->index(row, 0, parent));
    // 设置当前项的只读状态
    setReadOnly(model->index(row, 0, parent), dialog->readOnly());

    // 删除对话框，释放内存
    delete dialog;
    // 返回true表示事件已成功处理
    return true;
}

/**
 * @brief 设置模型项的只读状态
 * @param index 模型索引
 * @param readOnly 如果为true则设置为只读，否则设置为可编辑
 */
void EntryDelegate::setReadOnly(const QModelIndex &index, bool readOnly)
{
    // 获取当前行号和父索引
    int row = index.row();
    QModelIndex parent = index.parent();
    QStandardItem *item;
    // 将模型转换为QStandardItemModel类型，以便操作项的标志位
    const auto *model = qobject_cast<const QStandardItemModel *>(index.model());
    // 如果模型转换失败（不是QStandardItemModel），则返回不处理
    if (!model)
        return;

    // 设置第0列（名称列）的可编辑状态：如果只读则移除可编辑标志，否则添加
    item = model->itemFromIndex(model->index(row, 0, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第1列（类型列）的可编辑状态
    item = model->itemFromIndex(model->index(row, 1, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第2列（开始日期列）的可编辑状态
    item = model->itemFromIndex(model->index(row, 2, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第3列（结束日期列）的可编辑状态
    item = model->itemFromIndex(model->index(row, 3, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第4列（完成度列）的可编辑状态
    item = model->itemFromIndex(model->index(row, 4, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);
}

/**
 * @brief 添加两个索引之间的约束关系
 * @param index1 源索引（依赖项）
 * @param index2 目标索引（被依赖项）
 */
void EntryDelegate::addConstraint(const QModelIndex &index1, const QModelIndex &index2)
{
    // 如果任一索引无效（如未初始化或超出范围），则不添加约束
    if (!index1.isValid() || !index2.isValid())
        return;

    // 创建从index1到index2的约束关系
    KDGantt::Constraint c(index1, index2);
    // 将约束添加到约束模型中，用于甘特图的依赖关系显示
    constraintModel->addConstraint(c);
}
