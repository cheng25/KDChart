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
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>
#include <QSlider>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGraphicsView>
#include <KDGanttView>

/**
 * @class MyStandardItem
 * @brief 自定义标准项类
 * 继承自QStandardItem，提供多种构造函数以支持不同类型数据的初始化
 */
class MyStandardItem : public QStandardItem
{
public:
    /**
     * @brief 构造函数，使用QVariant设置显示角色数据
     * @param v 要设置的数据值
     */
    MyStandardItem(const QVariant &v)
        : QStandardItem()  // 调用父类QStandardItem的默认构造函数
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色的数据
        // setFlags( flags() & ~Qt::ItemIsEditable );  // 原文注释：禁用编辑功能（当前未启用）
    }
    /**
     * @brief 构造函数，使用字符串设置显示角色数据
     * @param v 要显示的文本字符串
     */
    MyStandardItem(const QString &v)
        : QStandardItem()  // 调用父类QStandardItem的默认构造函数
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色的文本数据
        // setFlags( flags() & ~Qt::ItemIsEditable );  // 原文注释：禁用编辑功能（当前未启用）
    }
    /**
     * @brief 构造函数，使用日期时间和指定角色设置数据
     * @param dt 日期时间值
     * @param role 数据角色
     */
    MyStandardItem(const QDateTime &dt, int role)
        : QStandardItem()  // 调用父类QStandardItem的默认构造函数
    {
        setData(QVariant::fromValue(dt), role);  // 设置指定角色的日期时间数据
    }
};

/**
 * @class MyWidget
 * @brief 主窗口部件类
 * 包含Gantt图表视图、数据模型、打印功能和交互控件，用于演示Gantt图表的高级视图功能
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
        // proxyModel.setSourceModel( &model );  // 原文注释：未启用代理模型
        // 循环创建20个顶级项目
        for (int h = 0; h < 20; ++h) {
            QStandardItem *topitem = new MyStandardItem(QString::fromLatin1("Top Item %1").arg(h));  // 创建顶级项目
            // 为每个顶级项目添加20个子项目
            for (int i = 0; i < 20; ++i) {
                QStandardItem *item = new MyStandardItem(QString::fromLatin1("Multi Item %1").arg(i));  // 创建多级项目
                // 为多级项目添加子任务项
                for (int j = 0; j < 30; j += 3) {
                    item->appendRow(QList<QStandardItem *>()
                                    << new MyStandardItem(QString::fromLatin1("Item %1").arg(j))  // 任务名称
                                    << new MyStandardItem(KDGantt::TypeTask)  // 任务类型
                                    << new MyStandardItem(QDateTime::currentDateTime().addDays(j), KDGantt::StartTimeRole)  // 开始时间
                                    << new MyStandardItem(QDateTime::currentDateTime().addDays(j + 1 + i / 7), KDGantt::EndTimeRole)  // 结束时间
                                    << new MyStandardItem(50));  // 完成度50%
                }
                // 添加事件类型子项
                item->appendRow(QList<QStandardItem *>()
                                << new MyStandardItem(QString::fromLatin1("Event"))  // 事件名称
                                << new MyStandardItem(KDGantt::TypeEvent)  // 事件类型
                                << new MyStandardItem(QDateTime::currentDateTime(), KDGantt::StartTimeRole)  // 事件时间
                                << new MyStandardItem(QDateTime(), KDGantt::EndTimeRole)  // 无结束时间
                                << new MyStandardItem(QString()));  // 无完成度

                // 将多级项目添加到顶级项目
                topitem->appendRow(QList<QStandardItem *>()
                                   << item  // 项目名称
                                   << new MyStandardItem(KDGantt::TypeMulti)  // 多级类型
                                   << new MyStandardItem(QString())  // 无开始时间
                                   << new MyStandardItem(QString())  // 无结束时间
                                   << new MyStandardItem(QString()));  // 无完成度
            }
            // 将顶级项目添加到模型
            model.appendRow(QList<QStandardItem *>()
                            << topitem  // 项目名称
                            << new MyStandardItem(KDGantt::TypeMulti /*Summary*/)  // 多级摘要类型
                            << new MyStandardItem(QString())  // 无开始时间
                            << new MyStandardItem(QString())  // 无结束时间
                            << new MyStandardItem(QString()));  // 无完成度
        }
        // 添加无数据项目
        model.appendRow(QList<QStandardItem *>()
                        << new MyStandardItem(QString::fromLatin1("No data")));  // 无数据项名称

        // 添加项目间的约束关系
        // cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 0, 3 ), proxyModel.index( 10, 3 ) ) );  // 原文注释：未启用代理模型约束
        // cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 10, 3 ), proxyModel.index( 5, 3 ) ) );  // 原文注释：未启用代理模型约束
        QModelIndex pidx = model.index(0, 0);  // 获取第一个顶级项目索引
        pidx = model.index(0, 0, pidx);  // 获取第一个子项目索引
        cmodel.addConstraint(KDGantt::Constraint(model.index(0, 0, pidx), model.index(1, 0, pidx)));  // 项目0依赖项目1
        cmodel.addConstraint(KDGantt::Constraint(model.index(1, 0, pidx), model.index(0, 0, pidx)));  // 项目1依赖项目0
        cmodel.addConstraint(KDGantt::Constraint(model.index(1, 0, pidx), model.index(10, 0, pidx)));  // 项目1依赖项目10
        cmodel.addConstraint(KDGantt::Constraint(model.index(3, 0, pidx), model.index(5, 0, pidx)));  // 项目3依赖项目5
        cmodel.addConstraint(KDGantt::Constraint(model.index(7, 0, pidx), model.index(4, 0, pidx)));  // 项目7依赖项目4

        qDebug() << "Creating view";  // 调试输出：正在创建视图
        slider.setOrientation(Qt::Horizontal);  // 设置滑块为水平方向
        slider.setRange(1, 10000);  // 设置滑块范围
        slider.setValue(100);  // 设置滑块初始值
        auto *l = new QVBoxLayout(this);  // 创建垂直布局
        l->addWidget(&view);  // 添加Gantt视图到布局
        l->addWidget(&slider);  // 添加滑块到布局
        grid.setStartDateTime(QDateTime::currentDateTime().addDays(-3));  // 设置网格开始时间为当前时间前3天
        grid.setDayWidth(100);  // 设置每天宽度为100像素
        // grid.setNoInformationBrush( Qt::NoBrush );  // 原文注释：未设置无信息区域画笔
        view.setGrid(&grid);  // 设置Gantt视图的网格
        view.setModel(&model);  // 设置Gantt视图的数据模型
        // view.setConstraintModel( &cmodel );  // 原文注释：未启用约束模型
        // 连接滑块值变化信号到缩放槽函数
        connect(&slider, &QSlider::valueChanged,
                this, &MyWidget::slotZoom);

        auto *pb1 = new QPushButton(tr("Print Preview..."));  // 创建打印预览按钮
        auto *pb2 = new QPushButton(tr("Print..."));  // 创建打印按钮
        l->addWidget(pb1);  // 添加打印预览按钮到布局
        l->addWidget(pb2);  // 添加打印按钮到布局
        connect(pb1, &QPushButton::clicked, this, &MyWidget::slotPrintPreview);  // 连接打印预览按钮点击信号
        connect(pb2, &QPushButton::clicked, this, &MyWidget::slotPrint);  // 连接打印按钮点击信号

        view.graphicsView()->setHeaderContextMenuPolicy(Qt::CustomContextMenu);  // 设置头部上下文菜单策略
        // 连接头部上下文菜单请求信号
        connect(view.graphicsView(), &KDGantt::GraphicsView::headerContextMenuRequested,
                this, &MyWidget::slotHeaderMenu);
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

    /**
     * @brief 打印预览槽函数，生成预览图像并显示
     */
    void slotPrintPreview()
    {
        QPixmap pix(1000, 200);  // 创建1000x200像素的 pixmap
        pix.fill(Qt::white);  // 白色填充背景
        QPainter painter(&pix);  // 创建画家对象
        view.print(&painter, pix.rect());  // 将视图内容绘制到 pixmap
        painter.end();  // 结束绘制
        auto *label = new QLabel;  // 创建标签用于显示预览
        label->setPixmap(pix);  // 设置标签的 pixmap
        label->show();  // 显示预览窗口
    }
    /**
     * @brief 打印槽函数，打开打印对话框并执行打印
     */
    void slotPrint()
    {
        QPrinter printer(QPrinter::HighResolution);  // 创建高分辨率打印机对象
        QPrintDialog pd(&printer, this);  // 创建打印对话框
        if (pd.exec() == QDialog::Accepted) {  // 如果用户确认打印
            /*
            printer.setResolution( 600 );  // 原文注释：设置分辨率为600 DPI
            QPainter p( &printer );  // 原文注释：创建打印机画家
            QFont f;  // 原文注释：创建字体
            f.setPixelSize( 9 );  // 原文注释：设置字体像素大小
            p.setFont( f );  // 原文注释：设置字体
            view.print( &p );  // 原文注释：打印视图内容
            */
            // 获取视图场景的可见区域
            QRectF r = view.graphicsView()->mapToScene(view.graphicsView()->viewport()->rect()).boundingRect();
            view.print(&printer, r.left(), r.right());  // 打印指定区域
        }
    }
    /**
     * @brief 头部菜单槽函数，显示上下文菜单
     * @param pt 菜单显示位置
     */
    void slotHeaderMenu(const QPoint &pt)
    {
        QMenu menu;  // 创建菜单
        menu.addAction(tr("This"));  // 添加菜单项
        menu.addAction(tr("is"));  // 添加菜单项
        menu.addAction(tr("just"));  // 添加菜单项
        menu.addAction(tr("a"));  // 添加菜单项
        menu.addAction(tr("test"));  // 添加菜单项
        menu.exec(pt);  // 在指定位置显示菜单
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
    QPixmapCache::setCacheLimit(30 * 1024);  // 设置像素图缓存限制为30MB

    MyWidget w;  // 创建主窗口部件
    w.show();  // 显示窗口

    return app.exec();  // 运行应用程序事件循环
}

#include "main.moc"  // 包含元对象编译器生成的代码
