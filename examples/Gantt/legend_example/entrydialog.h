/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
**
** 许可证：MIT
**
****************************************************************************/

#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDateTime>      // 包含Qt日期时间类
#include <QDialog>        // 包含Qt对话框基类
#include <QModelIndex>    // 包含Qt模型索引类

QT_BEGIN_NAMESPACE
class QAbstractItemModel;  // 前向声明Qt抽象项模型类
namespace Ui {
class EntryDialog;        // 前向声明UI命名空间中的EntryDialog类
} // namespace Ui
QT_END_NAMESPACE

namespace KDGantt {
class ConstraintModel;    // 前向声明KDGantt命名空间中的约束模型类
} // namespace KDGantt

/**
 * @brief 任务条目编辑对话框类
 * 
 * 该类提供了一个对话框界面，用于编辑甘特图中的任务条目信息，
 * 包括任务名称、类型、开始/结束日期、完成度、依赖关系等。
 */
class EntryDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * @param model 指向数据模型的指针，用于获取任务数据
     * @param parent 父窗口指针
     * @param f 窗口标志
     */
    explicit EntryDialog(const QAbstractItemModel *model, QWidget *parent = nullptr, Qt::WindowFlags f = {});

    /**
     * @brief 从模型索引初始化对话框数据
     * 
     * @param index 模型中的索引，用于获取特定任务的数据
     * @param constraintModel 约束模型，用于处理任务依赖关系
     */
    void initFrom(const QModelIndex &index, const KDGantt::ConstraintModel *constraintModel);

    /**
     * @brief 获取任务名称
     * 
     * @return QString 任务名称
     */
    QString name() const;

    /**
     * @brief 获取任务类型
     * 
     * @return int 任务类型标识符
     */
    int type() const;

    /**
     * @brief 获取任务开始日期
     * 
     * @return QDateTime 任务开始的日期时间
     */
    QDateTime startDate() const;

    /**
     * @brief 获取任务结束日期
     * 
     * @return QDateTime 任务结束的日期时间
     */
    QDateTime endDate() const;

    /**
     * @brief 获取任务完成度
     * 
     * @return int 完成度百分比 (0-100)
     */
    int completion() const;

    /**
     * @brief 检查任务是否为只读
     * 
     * @return bool 如果任务为只读则返回true，否则返回false
     */
    bool readOnly() const;

    /**
     * @brief 获取依赖的任务索引
     * 
     * @return QModelIndex 依赖任务的模型索引
     */
    QModelIndex depends() const;

    /**
     * @brief 获取图例文本
     * 
     * @return QString 图例描述文本
     */
    QString legend() const;

private slots:
    /**
     * @brief 更新结束日期
     * 
     * 当开始日期改变时，自动更新结束日期
     * @param startDate 新的开始日期
     */
    void updateEndDate(const QDateTime &startDate);

    /**
     * @brief 禁用编辑功能
     * 
     * @param disable 如果为true则禁用编辑，否则启用编辑
     */
    void disableEditing(bool disable);

    /**
     * @brief 任务类型改变槽函数
     * 
     * 当任务类型改变时被调用，用于更新界面其他部分
     * @param index 新选中的类型索引
     */
    void typeChanged(int index);

private:
    /**
     * @brief 初始化对话框
     * 
     * 设置UI控件的初始状态和连接信号槽
     */
    void init();

    /**
     * @brief 添加依赖项
     * 
     * 向依赖下拉列表中添加任务项
     * @param model 数据模型
     * @param index 模型索引
     * @param indent 缩进级别，用于显示层级关系
     */
    void addDependItem(const QAbstractItemModel *model, const QModelIndex &index, int indent = 0);

    QList<QPersistentModelIndex> indexList;  // 持久化模型索引列表，用于存储任务项
    const QAbstractItemModel *model;         // 指向数据模型的指针
    Ui::EntryDialog *ui;                     // UI界面指针
}; // class EntryDialog

#endif /* ENTRYDIALOG_H */
