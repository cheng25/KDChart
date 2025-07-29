/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** 中文注释：该文件演示了如何使用KDGantt::GraphicsView和自定义行控制器来创建甘特图视图
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有：2001年 Klarälvdalens Datakonsult AB（KDAB集团公司）<info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可协议：MIT
**
****************************************************************************/

#include <QApplication>      // 包含Qt应用程序类
#include <QModelIndex>       // 包含模型索引类
#include <QPointer>          // 包含智能指针类
#include <QPushButton>       // 包含按钮控件类
#include <QStandardItemModel>// 包含标准项模型类
#include <QDebug>            // 包含调试输出类

#include <stdlib.h>          // 包含标准库函数

#include <KDGanttAbstractRowController> // 包含甘特图抽象行控制器类
#include <KDGanttGraphicsView>         // 包含甘特图图形视图类
#include <KDGanttStyleOptionGanttItem> // 包含甘特图项样式选项类

/**
 * @class MyRowController
 * @brief 自定义行控制器类
 * 继承自KDGantt::AbstractRowController，用于管理甘特图中的行高、可见性和几何位置
 * 提供了对行布局的精确控制
 */
class MyRowController : public KDGantt::AbstractRowController
{
private:
    static const int ROW_HEIGHT; // 行高常量，定义甘特图中每行的高度
    QPointer<QAbstractItemModel> m_model; // 指向数据模型的指针，用于获取行数等信息

public:
    /**
     * @brief 默认构造函数
     * 初始化行控制器
     */
    MyRowController()
    {
    }

    /**
     * @brief 设置数据模型
     * @param model 要关联的数据模型指针
     */
    void setModel(QAbstractItemModel *model)
    {
        m_model = model; // 将成员变量m_model指向传入的模型
    }

    /**
     * @brief 重写获取表头高度的方法
     * @return 表头高度，固定为40像素
     */
    /*reimp*/ int headerHeight() const override
    {
        return 40; // 返回表头高度为40像素
    }

    /**
     * @brief 重写检查行可见性的方法
     * @param idx 行对应的模型索引
     * @return 始终返回true，表示所有行均可见
     */
    /*reimp*/ bool isRowVisible(const QModelIndex &) const override
    {
        return true; // 所有行均可见
    }

    /**
     * @brief 重写检查行展开状态的方法
     * @param idx 行对应的模型索引
     * @return 始终返回false，表示所有行均未展开
     */
    /*reimp*/ bool isRowExpanded(const QModelIndex &) const override
    {
        return false; // 所有行均未展开
    }

    /**
     * @brief 重写获取行几何信息的方法
     * @param idx 行对应的模型索引
     * @return 行的几何信息，包括起始位置和高度
     */
    /*reimp*/ KDGantt::Span rowGeometry(const QModelIndex &idx) const override
    {
        qDebug() << "rowGeometry(" << idx.row() << ")"; // 调试输出当前行索引
        // 返回指定行的几何信息：起始高度为行索引*行高，高度为ROW_HEIGHT
        return KDGantt::Span(idx.row() * ROW_HEIGHT, ROW_HEIGHT);
    }

    /**
     * @brief 重写获取最大项高度的方法
     * @return 最大项高度，为行高的一半
     */
    /*reimp*/ int maximumItemHeight() const override
    {
        return ROW_HEIGHT / 2; // 最大项高度为行高的一半
    }

    /**
     * @brief 重写获取总行高的方法
     * @return 总行高，等于行数乘以行高
     */
    /*reimp*/ int totalHeight() const override
    {
        // 如果模型为空返回0，否则返回总行数*行高
        return m_model.isNull() ? 0 : m_model->rowCount() * ROW_HEIGHT;
    }

    /**
     * @brief 重写根据高度获取索引的方法
     * @param height 高度值
     * @return 对应高度的模型索引
     */
    /*reimp*/ QModelIndex indexAt(int height) const override
    {
        // 根据高度计算对应的行索引，返回模型中该索引
        return m_model->index(height / ROW_HEIGHT, 0);
    }

    /**
     * @brief 重写获取下方索引的方法
     * @param idx 当前索引
     * @return 当前索引下方一行的索引
     */
    /*reimp*/ QModelIndex indexBelow(const QModelIndex &idx) const override
    {
        if (!idx.isValid()) // 如果索引无效，返回空索引
            return QModelIndex();
        // 返回当前索引下方一行的索引
        return idx.model()->index(idx.row() + 1, idx.column(), idx.parent());
    }

    /**
     * @brief 重写获取上方索引的方法
     * @param idx 当前索引
     * @return 当前索引上方一行的索引
     */
    /*reimp*/ QModelIndex indexAbove(const QModelIndex &idx) const override
    {
        if (!idx.isValid()) // 如果索引无效，返回空索引
            return QModelIndex();
        // 返回当前索引上方一行的索引
        return idx.model()->index(idx.row() - 1, idx.column(), idx.parent());
    }
};

const int MyRowController::ROW_HEIGHT = 30; // 初始化行高为30像素

/**
 * @class MyStandardItemModel
 * @brief 自定义标准项模型类
 * 继承自QStandardItemModel，用于自定义数据设置行为
 */
class MyStandardItemModel : public QStandardItemModel
{
public:
    /**
     * @brief 重写设置数据的方法
     * @param idx 数据项的模型索引
     * @param value 要设置的数据值
     * @param role 数据角色
     * @return 设置是否成功
     */
    /*reimp*/ bool setData(const QModelIndex &idx, const QVariant &value, int role) override
    {
        // qDebug() << "MyStandardItemModel::setData("<<idx<<","<<value<<","<<role<<")"; // 调试输出设置数据信息
        // 调用父类实现设置数据
        return QStandardItemModel::setData(idx, value, role);
    }
};

/**
 * @brief 生成随机对齐方式
 * @return 随机生成的对齐方式（上对齐、下对齐或居中对齐）
 */
static Qt::Alignment random_alignment()
{
    // 生成0-2之间的随机数
    int r = ( int )(3.0 * rand() / (RAND_MAX + 1.0));
    switch (r) {
    case 0:
        return Qt::AlignTop; // 上对齐
    case 1:
        return Qt::AlignBottom; // 下对齐
    case 2:
        return Qt::AlignCenter; // 居中对齐
    }
    return Qt::Alignment(); // 默认返回空对齐方式
}

/**
 * @brief 生成随机文本位置
 * @return 随机生成的文本位置（左、右或居中）
 */
static KDGantt::StyleOptionGanttItem::Position random_position()
{
    // 生成0-2之间的随机数
    int r = ( int )(3.0 * rand() / (RAND_MAX + 1.0));
    switch (r) {
    case 0:
        return KDGantt::StyleOptionGanttItem::Left; // 左对齐
    case 1:
        return KDGantt::StyleOptionGanttItem::Right; // 右对齐
    case 2:
    default:
        return KDGantt::StyleOptionGanttItem::Center; // 居中对齐
    }
}

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象

    auto *view = new KDGantt::GraphicsView; // 创建甘特图视图对象
    auto *rowController = new MyRowController; // 创建自定义行控制器对象
    view->setReadOnly(true); // 设置视图为只读模式
    view->setRowController(rowController); // 为视图设置自定义行控制器
    view->show(); // 显示视图

    MyStandardItemModel model; // 创建自定义标准项模型
    // 循环创建10行数据
    for (int i = 0; i < 10; ++i) {
        QList<QStandardItem *> items; // 存储一行中的所有项
        // 循环创建10列数据
        for (int j = 0; j < 10; ++j) {
            auto *item = new QStandardItem; // 创建标准项
            item->setText(QString::fromLatin1("Item %1").arg(i)); // 设置文本为"Item i"
            item->setData(KDGantt::TypeTask, KDGantt::ItemTypeRole); // 设置项类型为任务
            // 设置开始时间：当前时间加上2*j天再加上100000*i毫秒
            item->setData(QDateTime::currentDateTime().addDays(2 * j).addMSecs(100000 * i), KDGantt::StartTimeRole);
            // 设置结束时间：当前时间加上2*j+1天再加上100000*i毫秒
            item->setData(QDateTime::currentDateTime().addDays(2 * j + 1).addMSecs(100000 * i), KDGantt::EndTimeRole);
            // 设置文本对齐方式为随机生成的对齐方式
            item->setData(QVariant::fromValue<int>(random_alignment()), Qt::TextAlignmentRole);
            // 设置文本位置为随机生成的位置
            item->setData(QVariant::fromValue<int>(random_position()), KDGantt::TextPositionRole);
            // 移除项的可选择标志
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            items.push_back(item); // 将项添加到行列表中
        }
        model.appendRow(items); // 将行添加到模型中
    }
    qDebug() << &model; // 调试输出模型地址
    rowController->setModel(&model); // 为行控制器设置模型
    view->setModel(&model); // 为视图设置模型
#if 0
    QPushButton* pb = new QPushButton( QObject::tr( "Reset" ) ); // 创建重置按钮
    QObject::connect( pb, &QPushButton::clicked,
                      &model, &MyStandardItemModel::modelReset ); // 连接按钮点击信号到模型重置槽函数
    pb->show(); // 显示按钮
#endif
    return app.exec(); // 运行应用程序事件循环
    // return 0;
}
