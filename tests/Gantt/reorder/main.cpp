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

#include <QApplication>       // 包含Qt应用程序类
#include <QModelIndex>        // 包含Qt模型索引类
#include <QStandardItemModel> // 包含Qt标准项模型类
#include <QTimer>             // 包含Qt定时器类

#include <cassert>            // 包含断言宏

#include <KDGanttConstraintModel> // 包含Gantt约束模型类
#include <KDGanttView>           // 包含Gantt视图类

/**
 * @class MyTaskModel
 * @brief 自定义任务模型类
 * 继承自QAbstractTableModel，实现支持行移动功能的Gantt任务数据模型
 * 用于演示任务重排序时Gantt图表的更新行为
 */
class MyTaskModel : public QAbstractTableModel
{
    /**
     * @struct Task
     * @brief 任务数据结构
     * 存储单个Gantt任务的标题和时间信息
     */
    struct Task
    {
        QString title;    // 任务标题
        QDateTime start;  // 任务开始时间
        QDateTime end;    // 任务结束时间
    };

    QList<Task> m_tasks;  // 任务列表，存储所有任务数据

public:
    /**
     * @brief 构造函数，初始化任务模型并添加示例任务
     * @param parent 父对象指针
     */
    MyTaskModel(QObject *parent = 0)
        : QAbstractTableModel(parent)  // 调用父类构造函数
    {
        Task task;  // 创建临时任务对象

        // 添加第一个任务
        task.title = tr("Item 1");  // 设置任务标题
        task.start = QDateTime::currentDateTime();  // 设置开始时间为当前时间
        task.end = task.start.addDays(1);  // 设置结束时间为开始时间加1天
        m_tasks.push_back(task);  // 添加任务到列表

        // 添加第二个任务
        task.title = tr("Item 2");  // 设置任务标题
        task.start = QDateTime::currentDateTime().addDays(2);  // 设置开始时间为当前时间加2天
        task.end = task.start.addDays(1);  // 设置结束时间为开始时间加1天
        m_tasks.push_back(task);  // 添加任务到列表

        // 添加第三个任务
        task.title = tr("Item 3");  // 设置任务标题
        task.start = QDateTime::currentDateTime().addDays(4);  // 设置开始时间为当前时间加4天
        task.end = task.start.addDays(1);  // 设置结束时间为开始时间加1天
        m_tasks.push_back(task);  // 添加任务到列表

        // 添加第四个任务
        task.title = tr("Item 4");  // 设置任务标题
        task.start = QDateTime::currentDateTime().addDays(6);  // 设置开始时间为当前时间加6天
        task.end = task.start.addDays(1);  // 设置结束时间为开始时间加1天
        m_tasks.push_back(task);  // 添加任务到列表
    }

    /**
     * @brief 获取模型中的行数
     * @param index 模型索引（未使用）
     * @return 任务数量
     */
    /*reimp*/ int rowCount(const QModelIndex &index = QModelIndex()) const override
    {
        return index.isValid() ? 0 : m_tasks.size();  // 有效索引返回0，否则返回任务数量
    }

    /**
     * @brief 获取模型中的列数
     * @param index 模型索引（未使用）
     * @return 固定返回4列（标题、类型、开始时间、结束时间）
     */
    /*reimp*/ int columnCount(const QModelIndex &index = QModelIndex()) const override
    {
        return index.isValid() ? 0 : 4;  // 有效索引返回0，否则返回4列
    }

    /**
     * @brief 获取指定索引和角色的数据
     * @param index 模型索引
     * @param role 数据角色
     * @return 对应的数据值
     */
    /*reimp*/ QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        // 检查索引是否有效
        if (index.isValid() && index.row() < rowCount() && index.column() < columnCount()) {
            switch (index.column()) {
            case 0:  // 第一列：任务标题
                return (role == Qt::DisplayRole)
                    ? QVariant::fromValue(m_tasks[index.row()].title)
                    : QVariant();
            case 1:  // 第二列：任务类型
                return (role == Qt::DisplayRole)
                    ? QVariant::fromValue((int)KDGantt::TypeTask)
                    : QVariant();
            case 2:  // 第三列：开始时间
                return (role == KDGantt::StartTimeRole || role == Qt::DisplayRole)
                    ? QVariant::fromValue(m_tasks[index.row()].start)
                    : QVariant();
            case 3:  // 第四列：结束时间
                return (role == KDGantt::EndTimeRole || role == Qt::DisplayRole)
                    ? QVariant::fromValue(m_tasks[index.row()].end)
                    : QVariant();
            }
        }
        return QVariant();  // 返回无效数据
    }

    /**
     * @brief 移动行位置
     * 实现模型中行的移动功能，用于演示甘特图中任务重排序
     * @param from 源行索引
     * @param to 目标行索引
     */
    void moveRow(int from, int to)
    {
        if (from == to)  // 如果源索引等于目标索引，无需移动
            return;
        if (from >= m_tasks.size() || to >= m_tasks.size() + 1)  // 检查索引是否越界
            return;

        // 开始移动行
        if (beginMoveRows(QModelIndex(), from, from, QModelIndex(), to)) {
            m_tasks.move(from, to);  // 在任务列表中移动任务
            endMoveRows();  // 结束移动行
        } else {
            assert(0);  // 移动失败时触发断言
        }
    }
};

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建Qt应用程序实例

    auto *view = new KDGantt::View;  // 创建Gantt视图

    MyTaskModel model;  // 创建自定义任务模型

    KDGantt::ConstraintModel constraints;  // 创建约束模型
    // 添加任务间约束关系
    constraints.addConstraint(KDGantt::Constraint(model.index(0, 0), model.index(1, 0)));  // 任务0依赖任务1
    constraints.addConstraint(KDGantt::Constraint(model.index(0, 0), model.index(2, 0)));  // 任务0依赖任务2
    constraints.addConstraint(KDGantt::Constraint(model.index(1, 0), model.index(3, 0)));  // 任务1依赖任务3

    view->setModel(&model);  // 设置Gantt视图的数据模型
    view->setConstraintModel(&constraints);  // 设置Gantt视图的约束模型
    view->show();  // 显示Gantt视图

    // 5秒后执行行移动操作
    QTimer::singleShot(5000, &app, [&model] {  // 创建单次定时器，5秒后执行lambda函数
        /* 5秒后，将第1行移动到位置0 */
        qDebug() << "Moving row" << 1 << "to" << 0;  // 调试输出移动信息
        model.moveRow(1, 0);  // 移动行
    });

    return app.exec();  // 运行应用程序事件循环
}
