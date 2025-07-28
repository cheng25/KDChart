/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
**
** 许可证：MIT
**
****************************************************************************/

#include "mainwindow.h"

#include "entrydelegate.h"  // 包含自定义项委托头文件
#include "entrydialog.h"    // 包含任务编辑对话框头文件
#include "ui_mainwindow.h"  // 包含UI头文件

#include <algorithm>  // 包含C++算法库

#include <KDGanttConstraintModel>  // 包含KDGantt约束模型
#include <KDGanttDateTimeGrid>     // 包含KDGantt日期时间网格
#include <KDGanttGraphicsView>     // 包含KDGantt图形视图
#include <KDGanttLegend>           // 包含KDGantt图例
#include <QAbstractItemView>       // 包含Qt抽象项视图
#include <QCloseEvent>             // 包含Qt关闭事件
#include <QDebug>                  // 包含Qt调试工具
#include <QHeaderView>             // 包含Qt表头视图
#include <QPointer>                // 包含Qt智能指针
#include <QScrollBar>              // 包含Qt滚动条
#include <QStandardItemModel>      // 包含Qt标准项模型
#include <QTreeView>               // 包含Qt树视图

/**
 * @brief 自定义标准项类
 * 
 * 继承自QStandardItem，用于在模型中存储数据
 */
class MyStandardItem : public QStandardItem
{
public:
    /**
     * @brief 构造函数，接受QVariant类型数据
     * 
     * @param v 要存储的数据
     */
    MyStandardItem(const QVariant &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色数据
    }

    /**
     * @brief 构造函数，接受QString类型数据
     * 
     * @param v 要存储的字符串
     */
    MyStandardItem(const QString &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);  // 设置显示角色数据
    }
};

/**
 * @brief MainWindow类的构造函数
 * 
 * @param parent 父窗口指针
 * @param flags 窗口标志
 */
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , ui(new Ui::MainWindow)  // 创建UI对象
{
    ui->setupUi(this);  // 初始化UI

    initModel();        // 初始化数据模型
    initActions();      // 初始化动作
    initItemDelegate(); // 初始化项委托
    initGrid();         // 初始化网格

    // 获取左侧树视图并设置列隐藏
    auto *leftView = qobject_cast<QTreeView *>(ui->ganttView->leftView());
    Q_ASSERT(leftView);  // 断言确保leftView不为空
    leftView->setColumnHidden(1, true);  // 隐藏第2列
    leftView->setColumnHidden(2, true);  // 隐藏第3列
    leftView->setColumnHidden(3, true);  // 隐藏第4列
    leftView->setColumnHidden(4, true);  // 隐藏第5列
    leftView->setColumnHidden(5, true);  // 隐藏第6列
    leftView->header()->setStretchLastSection(true);  // 最后一列自适应宽度

    // 连接信号槽
    connect(ui->ganttView->leftView(), &QAbstractItemView::customContextMenuRequested,
            this, &MainWindow::showContextMenu);  // 自定义上下文菜单请求
    connect(ui->ganttView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::enableActions);  // 选择变更时启用动作
}

/**
 * @brief MainWindow类的析构函数
 */
MainWindow::~MainWindow()
{
    // 析构函数为空，UI对象由Qt自动管理
}

/**
 * @brief 关闭事件处理函数
 * 
 * 重写父类的关闭事件处理，用于在窗口关闭时清理图例对象
 * 
 * @param event 关闭事件对象
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    delete smallLegend;    // 删除小型图例
    delete detailedLegend; // 删除详细图例
    event->accept();       // 接受关闭事件
}

/**
 * @brief 初始化数据模型
 * 
 * 创建并配置任务数据模型、图例模型和约束模型
 */
void MainWindow::initModel()
{
    // 创建标准项模型，6列
    model = new QStandardItemModel(0, 6, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Tree View of Entries"));  // 设置表头
    ui->ganttView->setModel(model);  // 设置甘特图视图的模型

    // 创建图例模型
    auto *lmodel = new QStandardItemModel;
    lmodel->appendRow(QList<QStandardItem *>()
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(KDGantt::TypeEvent)
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QString::fromLatin1("Event")));  // 事件类型
    lmodel->appendRow(QList<QStandardItem *>()
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(KDGantt::TypeTask)
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QString::fromLatin1("Task")));  // 任务类型
    lmodel->appendRow(QList<QStandardItem *>()
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(KDGantt::TypeSummary)
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QVariant())
                      << new MyStandardItem(QString::fromLatin1("Summary")));  // 摘要类型

    // 创建小型图例
    smallLegend = new KDGantt::Legend();
    smallLegend->setWindowTitle(tr("Legend"));  // 设置窗口标题
    smallLegend->show();  // 显示图例
    smallLegend->setModel(lmodel);  // 设置图例模型

    // 创建详细图例
    detailedLegend = new KDGantt::Legend();
    detailedLegend->setWindowTitle(tr("List"));  // 设置窗口标题
    detailedLegend->show();  // 显示图例
    detailedLegend->setModel(model);  // 设置图例模型

    // 创建约束模型
    constraintModel = new KDGantt::ConstraintModel(this);
    ui->ganttView->setConstraintModel(constraintModel);  // 设置甘特图视图的约束模型
}

/**
 * @brief 初始化动作
 * 
 * 创建并配置菜单和工具栏动作
 */
void MainWindow::initActions()
{
    // 创建新建条目动作
    newEntryAction = new QAction(tr("New entry"), this);
    newEntryAction->setShortcut(QKeySequence::New);  // 设置快捷键
    connect(newEntryAction, &QAction::triggered, this, &MainWindow::addNewEntry);  // 连接信号槽

    // 创建删除条目动作
    removeEntryAction = new QAction(tr("Remove entry"), this);
    removeEntryAction->setShortcut(QKeySequence::Delete);  // 设置快捷键
    connect(removeEntryAction, &QAction::triggered, this, &MainWindow::removeEntry);  // 连接信号槽

    // 创建放大动作
    zoomInAction = new QAction(tr("Zoom In"), this);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);  // 设置快捷键
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);  // 连接信号槽

    // 创建缩小动作
    zoomOutAction = new QAction(tr("Zoom Out"), this);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);  // 设置快捷键
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);  // 连接信号槽

    // 创建适配动作
    zoomFitAction = new QAction(tr("Zoom to Fit"), this);
    connect(zoomFitAction, &QAction::triggered, this, &MainWindow::zoomFit);  // 连接信号槽

    // 设置左侧视图的上下文菜单策略
    ui->ganttView->leftView()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ganttView->leftView()->addAction(newEntryAction);  // 添加动作到视图
    ui->ganttView->leftView()->addAction(removeEntryAction);  // 添加动作到视图

    // 创建菜单栏
    QMenu *entryMenu = menuBar()->addMenu(tr("Entry"));  // 条目菜单
    entryMenu->addAction(newEntryAction);
    entryMenu->addAction(removeEntryAction);

    QMenu *zoomMenu = menuBar()->addMenu(tr("Zoom"));  // 缩放菜单
    zoomMenu->addAction(zoomInAction);
    zoomMenu->addAction(zoomOutAction);
    zoomMenu->addAction(zoomFitAction);

    QMenu *scaleMenu = menuBar()->addMenu(tr("Scale"));  // 刻度菜单
    scaleMenu->addAction(tr("Auto"), this, &MainWindow::scaleAuto);  // 自动刻度
    scaleMenu->addAction(tr("Hour"), this, &MainWindow::scaleHour);  // 小时刻度
    scaleMenu->addAction(tr("Day"), this, &MainWindow::scaleDay);    // 天刻度
    scaleMenu->addAction(tr("Week"), this, &MainWindow::scaleWeek);  // 周刻度
    scaleMenu->addAction(tr("Month"), this, &MainWindow::scaleMonth);  // 月刻度

    // 初始化动作状态
    enableActions(QItemSelection());
}

/**
 * @brief 初始化项委托
 * 
 * 设置自定义项委托用于编辑任务
 */
void MainWindow::initItemDelegate()
{
    auto *delegate = new EntryDelegate(constraintModel, this);  // 创建自定义委托
    ui->ganttView->leftView()->setItemDelegate(delegate);  // 设置委托
}

/**
 * @brief 初始化网格
 * 
 * 配置甘特图的日期时间网格
 */
void MainWindow::initGrid()
{
    grid = new KDGantt::DateTimeGrid();  // 创建日期时间网格
    grid->setDayWidth(70);  // 设置天宽度
    ui->ganttView->setGrid(grid);  // 设置甘特图视图的网格
}

/**
 * @brief 显示上下文菜单
 * 
 * 在指定位置显示上下文菜单
 * 
 * @param pos 鼠标位置
 */
void MainWindow::showContextMenu(const QPoint &pos)
{
    // 如果鼠标位置没有有效的索引，清除选择
    if (!ui->ganttView->leftView()->indexAt(pos).isValid())
        ui->ganttView->selectionModel()->clearSelection();

    QMenu menu(ui->ganttView->leftView());  // 创建菜单
    menu.addAction(newEntryAction);  // 添加动作
    menu.addAction(removeEntryAction);  // 添加动作
    menu.exec(ui->ganttView->leftView()->viewport()->mapToGlobal(pos));  // 显示菜单
}

/**
 * @brief 根据选择启用动作
 * 
 * 根据当前选中的项启用或禁用相关操作
 * 
 * @param selected 当前选中的项
 */
void MainWindow::enableActions(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty()) {  // 没有选中项
        newEntryAction->setEnabled(true);  // 启用新建动作
        removeEntryAction->setEnabled(false);  // 禁用删除动作
        return;
    }

    QModelIndex selectedIndex = selected.indexes()[0];  // 获取第一个选中的索引

    // 如果选中的是事件或任务类型
    if (model->data(model->index(selectedIndex.row(), 1)) == KDGantt::TypeEvent || 
        model->data(model->index(selectedIndex.row(), 1)) == KDGantt::TypeTask) {
        newEntryAction->setEnabled(false);  // 禁用新建动作
        removeEntryAction->setEnabled(true);  // 启用删除动作
        return;
    }

    // 其他情况，启用所有动作
    newEntryAction->setEnabled(true);
    removeEntryAction->setEnabled(true);
}

/**
 * @brief 添加新任务条目
 * 
 * 弹出任务编辑对话框，添加新的任务到模型中
 */
void MainWindow::addNewEntry()
{
    QPointer<EntryDialog> dialog = new EntryDialog(model);  // 创建对话框
    dialog->setWindowTitle(tr("New Entry"));  // 设置窗口标题
    if (dialog->exec() == QDialog::Rejected || !dialog) {  // 如果对话框被拒绝或为空
        delete dialog;
        return;
    }

    // 获取选中的索引
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    const QModelIndex parent = selectedIndexes.value(0);  // 获取父索引

    // 插入新行
    if (!model->insertRow(model->rowCount(parent), parent))
        return;

    int row = model->rowCount(parent) - 1;  // 获取新行的行号
    if (row == 0 && parent.isValid())
        model->insertColumns(model->columnCount(parent), 5, parent);  // 插入列

    // 设置新行的数据
    model->setData(model->index(row, 0, parent), dialog->name());  // 名称
    model->setData(model->index(row, 1, parent), dialog->type());  // 类型
    if (dialog->type() != KDGantt::TypeSummary) {  // 如果不是摘要类型
        model->setData(model->index(row, 2, parent), dialog->startDate(), KDGantt::StartTimeRole);  // 开始时间
        model->setData(model->index(row, 3, parent), dialog->endDate(), KDGantt::EndTimeRole);  // 结束时间
    }
    model->setData(model->index(row, 4, parent), dialog->completion());  // 完成度
    const QString legend(dialog->legend());
    if (!legend.isEmpty())
        model->setData(model->index(row, 5, parent), legend);  // 图例

    // 添加约束
    addConstraint(dialog->depends(), model->index(row, 0, parent));
    // 设置只读状态
    setReadOnly(model->index(row, 0, parent), dialog->readOnly());

    delete dialog;  // 删除对话框
}

/**
 * @brief 设置项的只读状态
 * 
 * @param index 模型索引
 * @param readOnly 是否只读
 */
void MainWindow::setReadOnly(const QModelIndex &index, bool readOnly)
{
    int row = index.row();  // 行号
    const QModelIndex parent = index.parent();  // 父索引
    QStandardItem *item;

    // 设置第0列的只读状态
    item = model->itemFromIndex(model->index(row, 0, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第1列的只读状态
    item = model->itemFromIndex(model->index(row, 1, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第2列的只读状态
    item = model->itemFromIndex(model->index(row, 2, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第3列的只读状态
    item = model->itemFromIndex(model->index(row, 3, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);

    // 设置第4列的只读状态
    item = model->itemFromIndex(model->index(row, 4, parent));
    item->setFlags(readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable);
}

/**
 * @brief 添加任务约束
 * 
 * 在两个任务之间添加依赖约束
 * 
 * @param index1 前置任务索引
 * @param index2 后续任务索引
 */
void MainWindow::addConstraint(const QModelIndex &index1, const QModelIndex &index2)
{
    if (!index1.isValid() || !index2.isValid())  // 检查索引是否有效
        return;

    KDGantt::Constraint c(index1, index2);  // 创建约束
    ui->ganttView->constraintModel()->addConstraint(c);  // 添加约束到模型
}

/**
 * @brief 删除选中的任务条目
 * 
 * 从模型中删除当前选中的任务条目
 */
void MainWindow::removeEntry()
{
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();  // 获取选中的索引
    QModelIndex index = selectedIndexes.value(0);  // 获取第一个索引

    if (!index.isValid())  // 检查索引是否有效
        return;

    model->removeRow(index.row(), index.parent());  // 从模型中删除行
}

/**
 * @brief 放大甘特图
 */
void MainWindow::zoomIn()
{
    qreal dayWidth = grid->dayWidth() + 10;  // 增加天宽度
    if (dayWidth > 400)  // 如果天宽度超过400，切换到小时刻度
        grid->setScale(KDGantt::DateTimeGrid::ScaleHour);

    grid->setDayWidth(dayWidth);  // 设置新的天宽度
}

/**
 * @brief 缩小甘特图
 */
void MainWindow::zoomOut()
{
    qreal dayWidth = grid->dayWidth() - 10;  // 减少天宽度
    if (dayWidth < 10)  // 确保天宽度不小于10
        dayWidth = 10;

    if (dayWidth <= 400)  // 如果天宽度小于等于400，切换到天刻度
        grid->setScale(KDGantt::DateTimeGrid::ScaleDay);

    grid->setDayWidth(dayWidth);  // 设置新的天宽度
}

/**
 * @brief 适配甘特图到视图
 */
void MainWindow::zoomFit()
{
    const QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();  // 获取选中的索引

    if (selectedIndexes.isEmpty()) {  // 如果没有选中项，返回
        return;
    }

    KDGantt::Span span;  // 跨度
    for (QModelIndex idx : selectedIndexes) {  // 遍历所有选中的索引
        const KDGantt::Span s = grid->mapToChart(grid->model()->index(idx.row(), 0));  // 映射到图表坐标
        if (span.isValid()) {
            span = span.expandedTo(s);  // 扩展跨度
        } else {
            span = s;  // 设置初始跨度
        }
    }

    span.setLength(span.length() + 20);  // 增加长度
    span.setStart(span.start() - 10);  // 减少起始位置

    qDebug() << selectedIndexes << span;  // 调试输出

    const qreal view_width = ui->ganttView->graphicsView()->viewport()->width();  // 视图宽度
    const QDateTime start = grid->mapFromChart(span.start()).value<QDateTime>();  // 起始时间
    const QDateTime end = grid->mapFromChart(span.end()).value<QDateTime>();  // 结束时间

    qreal delta = start.date().daysTo(end.date());  // 天数差
    delta += start.time().msecsTo(end.time()) / (1000. * 24. * 60. * 60.);  // 加上时间差

    qDebug() << view_width << "/" << delta;  // 调试输出
    grid->setDayWidth(view_width / (std::max(( qreal )1., delta)));  // 设置天宽度
    qDebug() << "daywidth set to" << grid->dayWidth();  // 调试输出
    qDebug() << "start scroll to" << grid->mapToChart(start);  // 调试输出
    ui->ganttView->graphicsView()->horizontalScrollBar()->setValue(grid->mapToChart(start));  // 设置水平滚动条位置
}

/**
 * @brief 设置自动刻度
 */
void MainWindow::scaleAuto()
{
    auto *grid = static_cast<KDGantt::DateTimeGrid *>(ui->ganttView->grid());  // 获取网格
    grid->setScale(KDGantt::DateTimeGrid::ScaleAuto);  // 设置自动刻度
}

/**
 * @brief 设置小时刻度
 */
void MainWindow::scaleHour()
{
    auto *grid = static_cast<KDGantt::DateTimeGrid *>(ui->ganttView->grid());  // 获取网格
    grid->setScale(KDGantt::DateTimeGrid::ScaleHour);  // 设置小时刻度
}

/**
 * @brief 设置天刻度
 */
void MainWindow::scaleDay()
{
    auto *grid = static_cast<KDGantt::DateTimeGrid *>(ui->ganttView->grid());  // 获取网格
    grid->setScale(KDGantt::DateTimeGrid::ScaleDay);  // 设置天刻度
}

/**
 * @brief 设置周刻度
 */
void MainWindow::scaleWeek()
{
    auto *grid = static_cast<KDGantt::DateTimeGrid *>(ui->ganttView->grid());  // 获取网格
    grid->setScale(KDGantt::DateTimeGrid::ScaleWeek);  // 设置周刻度
}

/**
 * @brief 设置月刻度
 */
void MainWindow::scaleMonth()
{
    auto *grid = static_cast<KDGantt::DateTimeGrid *>(ui->ganttView->grid());  // 获取网格
    grid->setScale(KDGantt::DateTimeGrid::ScaleMonth);  // 设置月刻度
}
