/****************************************************************************
**
** 此文件是KD Chart库自定义约束示例的主程序
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
#include <QDateTime>          // 包含日期时间类
#include <QDebug>             // 包含调试输出类
#include <QLabel>             // 包含标签控件类
#include <QMenu>              // 包含菜单类
#include <QPainter>           // 包含绘图类
#include <QPixmap>            // 包含像素图类
#include <QPixmapCache>       // 包含像素图缓存类
#include <QSlider>            // 包含滑块控件类
#include <QStandardItemModel> // 包含标准项模型类
#include <QVBoxLayout>        // 包含垂直布局类

#include <KDGanttConstraintModel> // 包含甘特图约束模型类
#include <KDGanttDateTimeGrid>   // 包含日期时间网格类
#include <KDGanttGraphicsView>   // 包含甘特图图形视图类
#include <KDGanttView>           // 包含甘特图视图类

/**
 * @class MyGraphicsView
 * @brief 自定义甘特图图形视图类
 * 继承自KDGantt::GraphicsView，重写添加约束方法以支持自定义约束样式
 * 根据Shift键修饰符区分硬约束和软约束，并设置不同的线条样式
 */
class MyGraphicsView : public KDGantt::GraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    MyGraphicsView(QWidget *parent = 0)
        : KDGantt::GraphicsView(parent)  // 调用父类构造函数
    {
    }

    /**
     * @brief 重写添加约束方法
     * 根据源索引、目标索引和键盘修饰符创建或移除约束
     * @param from 源任务索引
     * @param to 目标任务索引
     * @param modifiers 键盘修饰符（Shift键用于区分硬约束/软约束）
     */
    /*reimp*/ void addConstraint(const QModelIndex &from,
                                 const QModelIndex &to,
                                 Qt::KeyboardModifiers modifiers) override
    {
        qDebug() << "MyGraphicsView::addConstraint()";  // 调试输出约束添加信息
        if (isReadOnly())  // 如果视图为只读模式，不添加约束
            return;
        KDGantt::ConstraintModel *cmodel = constraintModel();  // 获取约束模型
        // 创建约束：Shift键按下创建硬约束，否则创建软约束
        KDGantt::Constraint c(from, to, (modifiers & Qt::ShiftModifier) ? KDGantt::Constraint::TypeHard : KDGantt::Constraint::TypeSoft);
        // 设置有效约束的画笔样式：绿色3像素虚线
        c.setData(KDGantt::Constraint::ValidConstraintPen, QPen(QColor(Qt::green), 3, Qt::DashLine));
        // 设置无效约束的画笔样式：蓝色3像素虚线
        c.setData(KDGantt::Constraint::InvalidConstraintPen, QPen(QColor(Qt::blue), 3, Qt::DashLine));
        // 如果约束已存在则移除，否则添加
        if (cmodel->hasConstraint(c))
            cmodel->removeConstraint(c);
        else
            cmodel->addConstraint(c);
    }
};

/**
 * @class MyStandardItem
 * @brief 自定义标准项类
 * 继承自QStandardItem，提供多种构造函数以支持不同数据类型
 * 用于甘特图模型中的任务数据项
 */
class MyStandardItem : public QStandardItem
{
public:
    /**
     * @brief 构造函数，接受QVariant类型数据
     * @param v 要存储的数据
     */
    MyStandardItem(const QVariant &v)
        : QStandardItem()  // 调用父类构造函数
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色数据
    }
    /**
     * @brief 构造函数，接受字符串类型数据
     * @param v 要存储的字符串
     */
    MyStandardItem(const QString &v)
        : QStandardItem()  // 调用父类构造函数
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色数据
    }
    /**
     * @brief 构造函数，接受日期时间和数据角色
     * @param dt 日期时间数据
     * @param role 数据角色
     */
    MyStandardItem(const QDateTime &dt, int role)
        : QStandardItem()  // 调用父类构造函数
    {
        setData(QVariant::fromValue(dt), role);  // 设置指定角色的日期时间数据
    }
};

/**
 * @class MyWidget
 * @brief 主窗口部件类
 * 负责创建甘特图视图、模型、网格和滑块控件，设置UI布局
 * 提供甘特图的缩放功能和示例数据
 */
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * 初始化甘特图视图、模型、网格和滑块，设置布局和信号槽连接
     */
    MyWidget()
        : QWidget(0)  // 调用父类构造函数，设置无父窗口
    {
        view.setGraphicsView(new MyGraphicsView);  // 设置自定义图形视图

        qDebug() << "Building data";  // 调试输出构建数据信息
        // 循环创建2个任务项
        for (int h = 0; h < 2; ++h) {
            QList<QStandardItem *> items;
            items
                << new MyStandardItem(QString::fromLatin1("Item %1").arg(h))  // 任务名称
                << new MyStandardItem(KDGantt::TypeTask)  // 任务类型
                << new MyStandardItem(QDateTime::currentDateTime().addDays(h), KDGantt::StartTimeRole)  // 开始时间
                << new MyStandardItem(QDateTime::currentDateTime().addDays(h + 1), KDGantt::EndTimeRole)  // 结束时间
                << new MyStandardItem(50);  // 完成百分比
            model.appendRow(items);  // 添加任务项到模型
        }

        qDebug() << "Creating view";  // 调试输出创建视图信息
        slider.setOrientation(Qt::Horizontal);  // 设置滑块为水平方向
        slider.setRange(1, 10000);  // 设置滑块范围
        slider.setValue(100);  // 设置初始值
        auto *l = new QVBoxLayout(this);  // 创建垂直布局
        l->addWidget(&view);  // 添加甘特图视图到布局
        l->addWidget(&slider);  // 添加滑块到布局
        grid.setStartDateTime(QDateTime::currentDateTime().addDays(-3));  // 设置网格开始日期（当前时间减3天）
        grid.setDayWidth(100);  // 设置日宽度
        view.setGrid(&grid);  // 设置甘特图网格
        view.setModel(&model);  // 设置甘特图模型

        // 连接滑块值变化信号到缩放槽函数
        connect(&slider, &QSlider::valueChanged,
                this, &MyWidget::slotZoom);
    }

public slots:
    /**
     * @brief 缩放槽函数
     * 根据滑块值调整甘特图网格的日宽度
     * @param z 滑块值（日宽度）
     */
    void slotZoom(int z)
    {
        grid.setDayWidth(z);  // 设置网格日宽度
    }

private:
    KDGantt::View view;  // 甘特图视图
    KDGantt::DateTimeGrid grid;  // 日期时间网格
    QSlider slider;  // 缩放滑块
    QStandardItemModel model;  // 标准项数据模型
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
    // QPixmapCache::setCacheLimit( 30*1024 );  // 设置像素图缓存限制（注释掉）

    MyWidget w;  // 创建主窗口部件
    w.show();  // 显示主窗口部件

    return app.exec();  // 运行应用程序事件循环
}

#include "main.moc"  // 包含元对象编译器生成的代码
