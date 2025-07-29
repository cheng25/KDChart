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

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QListView>
#include <QSlider>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttListViewRowController>
#include <KDGanttView>

/**
 * @class MyStandardItem
 * @brief 自定义标准项类
 * 继承自QStandardItem，提供便捷的构造函数用于设置特定角色的数据
 * 用于在甘特图模型中存储和管理项目数据
 */
class MyStandardItem : public QStandardItem
{
public:
    /**
     * @brief 构造函数，使用指定角色设置数据
     * @param v 要设置的数据值
     * @param role 数据角色（默认为Qt::DisplayRole）
     */
    MyStandardItem(const QVariant &v, int role = Qt::DisplayRole)
        : QStandardItem()  // 调用父类QStandardItem的默认构造函数
    {
        setData(v, role);  // 设置指定角色的数据
    }
    /**
     * @brief 构造函数，设置显示角色的文本数据
     * @param v 要显示的文本字符串
     */
    MyStandardItem(const QString &v)
        : QStandardItem()  // 调用父类QStandardItem的默认构造函数
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色的文本数据
    }
};

/**
 * @class MyListView
 * @brief 自定义列表视图类
 * 继承自QListView，调整视口边距以预留标题空间
 * 用于在甘特图左侧显示任务列表
 */
class MyListView : public QListView
{
public:
    /**
     * @brief 构造函数，初始化自定义列表视图
     * @param parent 父窗口指针
     */
    MyListView(QWidget *parent = 0)
        : QListView(parent)  // 调用父类QListView的构造函数
    {
        // 为标题预留空间：字体高度*2 + 4倍边框宽度
        setViewportMargins(0, fontMetrics().height() * 2 + 4 * frameWidth(), 0, 0);
        // 禁用垂直滚动条
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // 设置垂直滚动模式为按像素滚动
        setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    }
};

/**
 * @class MyWidget
 * @brief 主窗口部件类
 * 包含Gantt图表视图、数据模型和交互控件，用于演示Gantt图表与列表视图的结合使用
 * 提供了任务数据的创建、约束关系的定义以及缩放功能
 */
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数，初始化主窗口部件
     */
    MyWidget()
        : QWidget(0)  // 调用父类QWidget的构造函数，无父窗口
    {
        qDebug() << "Building data";  // 调试输出：正在构建数据
        // 循环创建10个顶级项目
        for (int i = 0; i < 10; ++i) {
            // 创建多级项目
            QStandardItem *item = new MyStandardItem(QString::fromLatin1("Multi Item %1").arg(i));
            // 为多级项目添加子项
            for (int j = 0; j < 20; j += 3) {
                item->appendRow(QList<QStandardItem *>()
                                << new MyStandardItem(QString::fromLatin1("Item %1").arg(j))  // 项目名称
                                << new MyStandardItem(KDGantt::TypeTask)  // 项目类型为任务
                                << new MyStandardItem(QDateTime::currentDateTime().addDays(j), KDGantt::StartTimeRole)  // 开始时间
                                << new MyStandardItem(QDateTime::currentDateTime().addDays(j + 1 + i / 7), KDGantt::EndTimeRole)  // 结束时间
                                << new MyStandardItem(50));  // 完成度50%
            }
            // 添加事件类型子项
            item->appendRow(QList<QStandardItem *>()
                            << new MyStandardItem(QString::fromLatin1("Event"))  // 事件名称
                            << new MyStandardItem(KDGantt::TypeEvent)  // 项目类型为事件
                            << new MyStandardItem(QDateTime::currentDateTime(), KDGantt::StartTimeRole)  // 事件时间
                            << new MyStandardItem(QDateTime(), KDGantt::EndTimeRole)  // 无结束时间
                            << new MyStandardItem(QString()));  // 无完成度

            // 将多级项目添加到模型
            model.appendRow(QList<QStandardItem *>()
                            << item  // 项目名称
                            << new MyStandardItem(KDGantt::TypeMulti)  // 项目类型为多级
                            << new MyStandardItem(QString())  // 无开始时间
                            << new MyStandardItem(QString())  // 无结束时间
                            << new MyStandardItem(QString()));  // 无完成度
        }

        // 添加项目间的约束关系
        QModelIndex pidx = model.index(0, 0);  // 获取第一个顶级项目索引
        pidx = model.index(0, 0, pidx);  // 获取第一个子项目索引
        // 添加约束：项目0依赖项目1
        cmodel.addConstraint(KDGantt::Constraint(model.index(0, 0, pidx), model.index(1, 0, pidx)));
        // 添加约束：项目1依赖项目0
        cmodel.addConstraint(KDGantt::Constraint(model.index(1, 0, pidx), model.index(0, 0, pidx)));
        // 添加约束：项目1依赖项目10
        cmodel.addConstraint(KDGantt::Constraint(model.index(1, 0, pidx), model.index(10, 0, pidx)));
        // 添加约束：项目3依赖项目5
        cmodel.addConstraint(KDGantt::Constraint(model.index(3, 0, pidx), model.index(5, 0, pidx)));
        // 添加约束：项目7依赖项目4
        cmodel.addConstraint(KDGantt::Constraint(model.index(7, 0, pidx), model.index(4, 0, pidx)));

        qDebug() << "Creating view";  // 调试输出：正在创建视图
        slider.setOrientation(Qt::Horizontal);  // 设置滑块为水平方向
        slider.setRange(1, 1000);  // 设置滑块范围
        slider.setValue(100);  // 设置滑块初始值
        auto *l = new QVBoxLayout(this);  // 创建垂直布局
        l->addWidget(&view);  // 添加Gantt视图到布局
        l->addWidget(&slider);  // 添加滑块到布局

        // 配置日期时间网格
        grid.setStartDateTime(QDateTime::currentDateTime().addDays(-3));  // 设置网格开始时间为当前时间前3天
        grid.setDayWidth(100);  // 设置每天宽度为100像素
        grid.setFreeDays(QSet<Qt::DayOfWeek>() << Qt::Saturday << Qt::Sunday);  // 设置周末为自由日
        grid.setFreeDaysBrush(QBrush(Qt::red));  // 设置自由日背景色为红色

        // 创建自定义列表视图
        QListView *lv = new MyListView;
        view.setLeftView(lv);  // 设置Gantt视图的左侧列表视图
        // 设置行控制器
        view.setRowController(new KDGantt::ListViewRowController(lv, view.ganttProxyModel()));
        view.setGrid(&grid);  // 设置Gantt视图的网格
        view.setModel(&model);  // 设置Gantt视图的数据模型
        // view.setConstraintModel( &cmodel );  // 注释：未启用约束模型

        // 连接滑块值变化信号到缩放槽函数
        connect(&slider, &QSlider::valueChanged,
                this, &MyWidget::slotZoom);
    }

public slots:
    /**
     * @brief 缩放槽函数，调整网格的日宽度
     * @param z 缩放值，用于设置每天的宽度
     */
    void slotZoom(int z)
    {
        grid.setDayWidth(z);  // 设置网格的日宽度
    }

private:
    KDGantt::View view;  // Gantt图表视图
    KDGantt::DateTimeGrid grid;  // 日期时间网格
    QSlider slider;  // 缩放控制滑块
    QStandardItemModel model;  // 数据模型
    KDGantt::ConstraintModel cmodel;  // 约束模型
};

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建QApplication实例

    MyWidget w;  // 创建主窗口部件
    w.show();  // 显示窗口

    return app.exec();  // 运行应用程序事件循环
}

#include "main.moc"  // 包含元对象编译器生成的代码
