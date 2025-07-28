/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是甘特图图例示例的一部分，实现了EntryDelegate类的功能
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "entrydelegate.h"  // 包含EntryDelegate类定义
#include "entrydialog.h"    // 包含EntryDialog类定义

#include <KDGanttConstraintModel>  // 包含约束模型类
#include <KDGanttGlobal>           // 包含KDGantt全局定义
#include <QEvent>                  // 包含事件类
#include <QModelIndex>             // 包含模型索引类
#include <QPointer>                // 包含QPointer类
#include <QStandardItemModel>      // 包含标准项模型类
// TODO: Qt5.15.2升级 检查KDGantt库头文件是否与Qt5.15.2兼容
// C++17 兼容性: 头文件包含方式符合C++17标准

/**
 * @brief EntryDelegate::EntryDelegate 构造函数
 * @param constraintModel 约束模型指针
 * @param parent 父对象指针，默认为nullptr
 * @return 无返回值
 * @details 初始化EntryDelegate对象，保存约束模型指针
 */
EntryDelegate::EntryDelegate(KDGantt::ConstraintModel *constraintModel, QObject *parent)
    : QItemDelegate(parent)  // 调用基类QItemDelegate的构造函数
{
    this->constraintModel = constraintModel;  // 保存约束模型指针
}

/**
 * @brief EntryDelegate::editorEvent 处理编辑器事件
 * @param event 事件对象
 * @param model 数据模型
 * @param option 样式选项
 * @param index 模型索引
 * @return 如果事件被处理则返回true，否则返回false
 * @details 重写基类方法，处理鼠标双击事件，显示编辑对话框，并更新模型数据
 */
bool EntryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // 只处理鼠标双击事件
    if (event->type() != QEvent::MouseButtonDblClick)
        return false;

    // 检查索引是否有效
    if (!index.isValid())
        return QItemDelegate::editorEvent(event, model, option, index);

    // 创建编辑对话框
    // C++17 兼容性: 使用QPointer管理对象生命周期，符合C++17内存管理最佳实践
    QPointer<EntryDialog> dialog = new EntryDialog(model);
    dialog->initFrom(index, constraintModel);  // 从模型索引初始化对话框
    dialog->setWindowTitle(tr("Edit Entry"));  // 设置对话框标题
    dialog->exec();  // 显示模态对话框
    if (!dialog)  // 检查对话框是否已被删除
        return false;

    // 更新模型数据
    int row = index.row();  // 获取行号
    const QModelIndex parent = index.parent();  // 获取父索引
    model->setData(model->index(row, 0, parent), dialog->name());  // 设置名称
    model->setData(model->index(row, 1, parent), dialog->type());  // 设置类型
    // 如果不是摘要类型，则设置开始和结束时间
    if (dialog->type() != KDGantt::TypeSummary) {
        model->setData(model->index(row, 2, parent), dialog->startDate(), KDGantt::StartTimeRole);  // 设置开始时间
        model->setData(model->index(row, 3, parent), dialog->endDate(), KDGantt::EndTimeRole);      // 设置结束时间
    }
    model->setData(model->index(row, 4, parent), dialog->completion());  // 设置完成度
    model->setData(model->index(row, 5, parent), dialog->legend());      // 设置图例

    // 添加约束关系
    addConstraint(dialog->depends(), model->index(row, 0, parent));
    // 设置只读状态
    setReadOnly(model->index(row, 0, parent), dialog->readOnly());

    delete dialog;  // 删除对话框
    return true;  // 表示事件已被处理
}

/**
 * @brief EntryDelegate::setReadOnly 设置项目的只读状态
 * @param index 项目索引
 * @param readOnly 是否只读
 * @return 无返回值
 * @details 设置指定项目的只读状态，通过修改项目的标志位实现
 */
void EntryDelegate::setReadOnly(const QModelIndex &index, bool readOnly)
{
    int row = index.row();  // 获取行号
    QModelIndex parent = index.parent();  // 获取父索引
    QStandardItem *item;  // 标准项指针
    // C++17 兼容性: 使用qobject_cast进行类型转换，符合C++17标准
    const auto *model = qobject_cast<const QStandardItemModel *>(index.model());
    if (!model)  // 检查模型是否有效
        return;

    // 设置每一列的只读状态
    item = model->itemFromIndex(model->index(row, 0, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    item = model->itemFromIndex(model->index(row, 1, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    item = model->itemFromIndex(model->index(row, 2, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    item = model->itemFromIndex(model->index(row, 3, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    item = model->itemFromIndex(model->index(row, 4, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);
}

/**
 * @brief EntryDelegate::addConstraint 添加约束关系
 * @param index1 第一个项目的索引
 * @param index2 第二个项目的索引
 * @return 无返回值
 * @details 在两个项目之间添加约束关系，只有当两个索引都有效时才添加
 */
void EntryDelegate::addConstraint(const QModelIndex &index1, const QModelIndex &index2)
{
    // 检查两个索引是否都有效
    if (!index1.isValid() || !index2.isValid())
        return;

    // 创建约束并添加到约束模型
    KDGantt::Constraint c(index1, index2);
    constraintModel->addConstraint(c);
    // TODO: Qt5.15.2升级 检查KDGantt::ConstraintModel::addConstraint在Qt5.15.2中的变化
}
