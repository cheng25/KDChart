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

#include "entrydialog.h"

#include "ui_entrydialog.h"

#include <KDGanttConstraintModel>
#include <KDGanttGlobal>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QPersistentModelIndex>

// 声明QPersistentModelIndex为元类型，以便在信号槽中传递
Q_DECLARE_METATYPE(QPersistentModelIndex)

/**
 * @class EntryDialog
 * @brief Gantt图表条目的编辑对话框类
 * 用于编辑Gantt图表中的条目信息，包括名称、类型、日期、依赖关系、完成度和只读状态等
 */

/**
 * @brief 构造函数，初始化对话框UI和成员变量
 * @param model 数据模型指针，用于获取和设置条目数据
 * @param parent 父窗口指针，用于窗口管理
 * @param f 窗口标志，控制窗口样式和行为
 */
EntryDialog::EntryDialog(const QAbstractItemModel *model, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)  // 调用QDialog构造函数，设置父窗口和窗口标志
    , indexList(QList<QPersistentModelIndex>())  // 初始化持久模型索引列表，用于存储依赖项索引
    , ui(new Ui::EntryDialog)  // 创建UI对象，用于访问对话框控件
{
    ui->setupUi(this);  // 初始化UI界面，加载.ui文件中定义的控件布局
    this->model = model;  // 保存数据模型指针，用于后续数据读写
    init();  // 调用初始化函数，设置控件初始状态和信号槽连接
}

/**
 * @brief 初始化对话框界面组件和信号槽连接
 * 包括填充类型下拉框、添加依赖项选项、设置信号槽连接和初始日期
 */
void EntryDialog::init()
{
    // 向下拉框添加条目类型选项：事件、任务、摘要、多任务，关联对应KDGantt类型值
    ui->type->addItem(tr("Event"), KDGantt::TypeEvent);
    ui->type->addItem(tr("Task"), KDGantt::TypeTask);
    ui->type->addItem(tr("Summary"), KDGantt::TypeSummary);
    ui->type->addItem(tr("Multi"), KDGantt::TypeMulti);

    // 遍历数据模型所有行，为每行添加依赖项选项到下拉框
    for (int row = 0; row < model->rowCount(); ++row)
        addDependItem(model, model->index(row, 0));  // 添加指定索引的条目作为依赖项选项

    // 连接开始日期时间变化信号到更新结束日期槽函数：确保结束日期不早于开始日期
    connect(ui->startDate, &QDateTimeEdit::dateTimeChanged,
            this, &EntryDialog::updateEndDate);
    // 连接只读复选框状态变化信号到禁用编辑槽函数：控制是否允许编辑其他控件
    connect(ui->readOnly, &QCheckBox::toggled, this, &EntryDialog::disableEditing);

    // 设置开始日期时间编辑框的初始值为当前日期时间
    ui->startDate->setDateTime(QDateTime::currentDateTime());
}

/**
 * @brief 从模型索引加载数据并初始化对话框控件
 * @param index 模型索引，指向要编辑的条目数据
 * @param constraintModel 约束模型指针，用于获取条目间的依赖关系
 */
void EntryDialog::initFrom(const QModelIndex &index, const KDGantt::ConstraintModel *constraintModel)
{
    // 获取当前条目的行号和父索引，用于定位模型中的数据列
    int row = index.row();
    const QModelIndex parent = index.parent();

    // 设置名称编辑框文本：从模型第0列（名称列）获取数据
    ui->name->setText(model->data(model->index(row, 0, parent)).toString());
    // 设置图例编辑框文本：从模型第5列（图例列）获取数据
    ui->legend->setText(model->data(model->index(row, 5, parent)).toString());
    // 查找并设置类型下拉框选中项：根据模型第1列（类型列）的值匹配下拉框选项
    int idx = ui->type->findData(model->data(model->index(row, 1, parent)).toInt());
    ui->type->setCurrentIndex(idx);
    // 设置开始日期时间：从模型第2列（开始日期列）获取数据
    ui->startDate->setDateTime(model->data(model->index(row, 2, parent)).toDateTime());
    // 设置结束日期时间：从模型第3列（结束日期列）获取数据
    ui->endDate->setDateTime(model->data(model->index(row, 3, parent)).toDateTime());
    // 设置完成度滑块值：从模型第4列（完成度列）获取数据并转换为整数
    ui->completion->setValue(model->data(model->index(row, 4, parent)).toInt());
    // 设置只读复选框状态：根据模型项的可编辑标志判断，不可编辑则勾选
    ui->readOnly->setChecked(!(model->flags(model->index(row, 0, parent)) & Qt::ItemIsEditable));

    // 获取当前条目在约束模型中的所有约束关系
    QList<KDGantt::Constraint> constraints = constraintModel->constraintsForIndex(model->index(row, 0, parent));
    if (constraints.isEmpty())  // 如果没有约束关系，直接返回
        return;

    // 查找当前条目作为结束点的约束（即依赖项）
    QModelIndex constraintIndex;
    for (int i = 0; i < constraints.size(); ++i) {
        KDGantt::Constraint constraint = constraints[i];
        if (constraint.endIndex() == index) {  // 找到以当前条目为结束点的约束
            constraintIndex = constraint.startIndex();  // 获取约束的起始点（依赖项）索引
            break;
        }
    }

    // 如果找到有效依赖项索引，在依赖下拉框中选择对应项
    if (!constraintIndex.isValid())
        return;

    // 在依赖下拉框中选择依赖项：indexList中存储所有可选依赖项，+1是因为第0项为"无依赖"
    ui->depends->setCurrentIndex(indexList.indexOf(constraintIndex) + 1);
}

/**
 * @brief 递归添加依赖项到下拉框，支持层级显示
 * @param model 数据模型指针，用于获取依赖项名称
 * @param index 模型索引，指向当前要添加的依赖项
 * @param indent 缩进空格数，用于层级显示不同级别的依赖项
 */
void EntryDialog::addDependItem(const QAbstractItemModel *model, const QModelIndex &index, int indent)
{
    // 将当前索引添加到持久索引列表，用于后续查找依赖项
    indexList << QPersistentModelIndex(index);
    // 创建带缩进的显示文本：缩进空格数*indent + 依赖项名称，实现层级显示
    QString str = QString("%1%2").arg(QString().fill(' ', indent)).arg(model->data(index).toString());
    ui->depends->addItem(str);  // 将格式化后的文本添加到依赖下拉框

    // 递归添加所有子项作为依赖项，缩进增加2个空格以区分层级
    for (int row = 0; row < model->rowCount(index); ++row)
        addDependItem(model, model->index(row, 0, index), indent + 2);
}

/**
 * @brief 获取名称编辑框中的文本
 * @return 条目标题名称字符串
 */
QString EntryDialog::name() const
{
    return ui->name->text();  // 返回名称编辑框的当前文本
}

/**
 * @brief 获取类型下拉框中选中项的值
 * @return 条目类型值（KDGantt::TypeEvent/TypeTask等枚举值）
 */
int EntryDialog::type() const
{
    // 返回下拉框当前选中项的关联数据（即KDGantt类型值）
    return ui->type->itemData(ui->type->currentIndex()).toInt();
}

/**
 * @brief 获取开始日期时间编辑框中的值
 * @return 条目开始日期时间QDateTime对象
 */
QDateTime EntryDialog::startDate() const
{
    return ui->startDate->dateTime();  // 返回开始日期时间编辑框的当前值
}

/**
 * @brief 获取结束日期时间编辑框中的值
 * @return 条目结束日期时间QDateTime对象
 */
QDateTime EntryDialog::endDate() const
{
    return ui->endDate->dateTime();  // 返回结束日期时间编辑框的当前值
}

/**
 * @brief 获取完成度滑块的值
 * @return 条目完成度百分比（0-100整数）
 */
int EntryDialog::completion() const
{
    return ui->completion->value();  // 返回完成度滑块的当前值
}

/**
 * @brief 更新结束日期的最小值为开始日期，确保结束日期不早于开始日期
 * @param startDate 开始日期时间，作为结束日期的最小值
 */
void EntryDialog::updateEndDate(const QDateTime &startDate)
{
    ui->endDate->setMinimumDate(startDate.date());  // 设置结束日期的最小日期为开始日期的日期
    ui->endDate->setMinimumTime(startDate.time());  // 设置结束日期的最小时间为开始日期的时间
}

/**
 * @brief 检查只读复选框的状态
 * @return 如果复选框被勾选则返回true（只读），否则返回false（可编辑）
 */
bool EntryDialog::readOnly() const
{
    return ui->readOnly->isChecked();  // 返回只读复选框的勾选状态
}

/**
 * @brief 获取依赖下拉框中选中的依赖项索引
 * @return 选中的依赖项模型索引，若未选择则返回无效索引
 */
QModelIndex EntryDialog::depends() const
{
    // 如果选中第0项（"无依赖"），返回无效索引
    if (ui->depends->currentIndex() <= 0)
        return QModelIndex();
    // 返回indexList中对应索引的依赖项模型索引（currentIndex()-1是因为第0项为"无依赖"）
    return indexList.at(ui->depends->currentIndex() - 1);
}

/**
 * @brief 获取图例编辑框中的文本
 * @return 条目图例说明字符串
 */
QString EntryDialog::legend() const
{
    return ui->legend->text();  // 返回图例编辑框的当前文本
}

/**
 * @brief 根据只读状态禁用或启用编辑控件
 * @param readOnly 如果为true则禁用编辑控件，否则启用
 */
void EntryDialog::disableEditing(bool readOnly)
{
    // 设置名称编辑框的可编辑状态
    ui->name->setEnabled(!readOnly);
    // 设置类型下拉框的可编辑状态
    ui->type->setEnabled(!readOnly);
    // 设置开始日期时间编辑框的可编辑状态
    ui->startDate->setEnabled(!readOnly);
    // 设置结束日期时间编辑框的可编辑状态
    ui->endDate->setEnabled(!readOnly);
    // 设置完成度滑块的可编辑状态
    ui->completion->setEnabled(!readOnly);
    // 设置依赖下拉框的可编辑状态
    ui->depends->setEnabled(!readOnly);
    // 设置图例编辑框的可编辑状态
    ui->legend->setEnabled(!readOnly);
}

/**
 * @brief 析构函数，释放UI对象资源
 */
EntryDialog::~EntryDialog()
{
    delete ui;  // 删除UI对象，释放动态分配的控件资源
}
