/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
**
** 许可证：MIT
**
****************************************************************************/

#include "entrydialog.h"
#include "ui_entrydialog.h"

#include <KDGanttConstraintModel>  // 包含KDGantt约束模型头文件
#include <KDGanttGlobal>          // 包含KDGantt全局定义
#include <QAbstractItemModel>     // 包含Qt抽象项模型
#include <QDateTime>              // 包含Qt日期时间类
#include <QPersistentModelIndex>  // 包含Qt持久化模型索引

// 注册QPersistentModelIndex为元类型，以便在信号槽中传递
Q_DECLARE_METATYPE(QPersistentModelIndex)

/**
 * @brief EntryDialog类的构造函数
 * 
 * @param model 指向数据模型的指针，用于获取任务数据
 * @param parent 父窗口指针
 * @param f 窗口标志
 */
EntryDialog::EntryDialog(const QAbstractItemModel *model, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)              // 调用父类QDialog的构造函数
    , indexList(QList<QPersistentModelIndex>())  // 初始化索引列表
    , ui(new Ui::EntryDialog)         // 创建UI对象
{
    ui->setupUi(this);  // 初始化UI
    this->model = model;  // 保存模型指针
    init();  // 调用初始化函数
}

/**
 * @brief 初始化对话框
 * 
 * 设置UI控件的初始状态，填充任务类型和依赖项下拉列表，
 * 连接信号槽，并设置默认值。
 */
void EntryDialog::init()
{
    // 添加任务类型到下拉列表
    ui->type->addItem(tr("Event"), KDGantt::TypeEvent);    // 事件类型
    ui->type->addItem(tr("Task"), KDGantt::TypeTask);      // 任务类型
    ui->type->addItem(tr("Summary"), KDGantt::TypeSummary);// 摘要类型
    ui->type->addItem(tr("Multi"), KDGantt::TypeMulti);    // 多任务类型

    // 遍历模型，添加所有任务项到依赖下拉列表
    for (int row = 0; row < model->rowCount(); ++row)
        addDependItem(model, model->index(row, 0));

    // 连接信号槽
    connect(ui->startDate, &QDateTimeEdit::dateTimeChanged,
            this, &EntryDialog::updateEndDate);  // 开始日期改变时更新结束日期
    connect(ui->readOnly, &QCheckBox::toggled, this, &EntryDialog::disableEditing);  // 只读复选框状态改变时禁用编辑
    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EntryDialog::typeChanged);  // 任务类型改变时更新界面

    // 设置默认开始日期为当前日期时间
    ui->startDate->setDateTime(QDateTime::currentDateTime());
    // 触发类型改变事件，设置初始界面状态
    typeChanged(0);
}

/**
 * @brief 从模型索引初始化对话框数据
 * 
 * @param index 模型中的索引，用于获取特定任务的数据
 * @param constraintModel 约束模型，用于处理任务依赖关系
 */
void EntryDialog::initFrom(const QModelIndex &index, const KDGantt::ConstraintModel *constraintModel)
{
    int row = index.row();  // 获取行号
    const QModelIndex parent = index.parent();  // 获取父索引

    // 填充任务名称
    ui->name->setText(model->data(model->index(row, 0, parent)).toString());
    // 填充图例文本
    ui->legend->setText(model->data(model->index(row, 5, parent)).toString());
    // 设置任务类型
    int idx = ui->type->findData(model->data(model->index(row, 1, parent)).toInt());
    ui->type->setCurrentIndex(idx);
    // 设置开始日期
    ui->startDate->setDateTime(model->data(model->index(row, 2, parent), KDGantt::StartTimeRole).toDateTime());
    // 设置结束日期
    ui->endDate->setDateTime(model->data(model->index(row, 3, parent), KDGantt::EndTimeRole).toDateTime());
    // 设置完成度
    ui->completion->setValue(model->data(model->index(row, 4, parent)).toInt());
    // 设置只读状态
    ui->readOnly->setChecked(!(model->flags(model->index(row, 0, parent)) & Qt::ItemIsEditable));

    // 获取约束模型中与当前索引相关的约束
    QList<KDGantt::Constraint> constraints = constraintModel->constraintsForIndex(model->index(row, 0, parent));
    if (constraints.isEmpty())
        return;

    QModelIndex constraintIndex;
    // 查找当前任务依赖的前置任务
    for (int i = 0; i < constraints.size(); ++i) {
        KDGantt::Constraint constraint = constraints[i];
        if (constraint.endIndex() == index) {
            constraintIndex = constraint.startIndex();
            break;
        }
    }

    if (!constraintIndex.isValid())
        return;

    // 设置依赖下拉列表的当前项
    ui->depends->setCurrentIndex(indexList.indexOf(constraintIndex) + 1);
}

/**
 * @brief 添加依赖项到下拉列表
 * 
 * 递归添加模型中的所有任务项到依赖下拉列表，支持层级显示
 * 
 * @param model 数据模型
 * @param index 模型索引
 * @param indent 缩进级别，用于显示层级关系
 */
void EntryDialog::addDependItem(const QAbstractItemModel *model, const QModelIndex &index, int indent)
{
    indexList << QPersistentModelIndex(index);  // 保存持久化索引
    // 创建带有缩进的显示文本
    QString str = QString("%1%2").arg(QString().fill(' ', indent)).arg(model->data(index).toString());
    ui->depends->addItem(str);  // 添加到下拉列表

    // 递归添加子项
    for (int row = 0; row < model->rowCount(index); ++row)
        addDependItem(model, model->index(row, 0, index), indent + 2);
}

/**
 * @brief 获取任务名称
 * 
 * @return QString 任务名称
 */
QString EntryDialog::name() const
{
    return ui->name->text();
}

/**
 * @brief 获取任务类型
 * 
 * @return int 任务类型标识符
 */
int EntryDialog::type() const
{
    return ui->type->itemData(ui->type->currentIndex()).toInt();
}

/**
 * @brief 获取任务开始日期
 * 
 * @return QDateTime 任务开始的日期时间
 */
QDateTime EntryDialog::startDate() const
{
    return ui->startDate->dateTime();
}

/**
 * @brief 获取任务结束日期
 * 
 * @return QDateTime 任务结束的日期时间
 */
QDateTime EntryDialog::endDate() const
{
    return ui->endDate->dateTime();
}

/**
 * @brief 获取任务完成度
 * 
 * @return int 完成度百分比 (0-100)
 */
int EntryDialog::completion() const
{
    return ui->completion->value();
}

/**
 * @brief 更新结束日期
 * 
 * 当开始日期改变时，自动更新结束日期的最小值
 * 
 * @param startDate 新的开始日期
 */
void EntryDialog::updateEndDate(const QDateTime &startDate)
{
    ui->endDate->setMinimumDate(startDate.date());  // 设置结束日期的最小日期
    ui->endDate->setMinimumTime(startDate.time());  // 设置结束日期的最小时间
}

/**
 * @brief 检查任务是否为只读
 * 
 * @return bool 如果任务为只读则返回true，否则返回false
 */
bool EntryDialog::readOnly() const
{
    return ui->readOnly->isChecked();
}

/**
 * @brief 获取依赖的任务索引
 * 
 * @return QModelIndex 依赖任务的模型索引，如果没有依赖则返回无效索引
 */
QModelIndex EntryDialog::depends() const
{
    if (ui->depends->currentIndex() == 0)
        return QModelIndex();  // 没有依赖

    QPersistentModelIndex index = indexList[ui->depends->currentIndex() - 1];
    if (index.isValid())
        return index;  // 返回有效的依赖索引

    return QModelIndex();  // 无效索引
}

/**
 * @brief 获取图例文本
 * 
 * @return QString 图例描述文本
 */
QString EntryDialog::legend() const
{
    return ui->legend->text();
}

/**
 * @brief 禁用编辑功能
 * 
 * 根据参数禁用或启用所有编辑控件
 * 
 * @param disable 如果为true则禁用编辑，否则启用编辑
 */
void EntryDialog::disableEditing(bool disable)
{
    ui->name->setEnabled(!disable);       // 禁用/启用名称编辑
    ui->type->setEnabled(!disable);       // 禁用/启用类型选择
    ui->completion->setEnabled(!disable); // 禁用/启用完成度调整
    ui->startDate->setEnabled(!disable);  // 禁用/启用开始日期编辑
    ui->endDate->setEnabled(!disable);    // 禁用/启用结束日期编辑
    ui->depends->setEnabled(!disable);    // 禁用/启用依赖选择
}

/**
 * @brief 任务类型改变槽函数
 * 
 * 当任务类型改变时被调用，用于更新界面其他部分
 * 
 * @param index 新选中的类型索引
 */
void EntryDialog::typeChanged(int index)
{
    if (!index) {  // 如果是事件类型
        ui->label_EndDate->hide();  // 隐藏结束日期标签
        ui->endDate->hide();        // 隐藏结束日期控件
    } else {  // 其他类型
        ui->label_EndDate->show();  // 显示结束日期标签
        ui->endDate->show();        // 显示结束日期控件
    }
}
