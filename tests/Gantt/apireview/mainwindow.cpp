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

#include "mainwindow.h"

#include "entrydelegate.h"
#include "entrydialog.h"
#include "ui_mainwindow.h"

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGraphicsView>
#include <KDGanttLegend>
#include <QAbstractItemView>
#include <QDebug>
#include <QHeaderView>
#include <QLabel>
#include <QPointer>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

/**
 * @class MainWindow
 * @brief 甘特图应用程序的主窗口类
 * 负责管理甘特图的模型、视图、交互操作和用户界面组件
 * 提供任务的创建、删除、编辑和甘特图的显示与操作功能
 */
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // 初始化UI界面

    initModel();       // 初始化数据模型
    initActions();     // 初始化动作和菜单
    initItemDelegate();// 初始化项委托
    initGrid();        // 初始化甘特图网格

    // 获取左侧树视图并设置列可见性
    auto *leftView = qobject_cast<QTreeView *>(ui->ganttView->leftView());
    Q_ASSERT(leftView);  // 确保左侧视图不为空
    leftView->setColumnHidden(1, true);  // 隐藏第1列
    leftView->setColumnHidden(2, true);  // 隐藏第2列
    leftView->setColumnHidden(3, true);  // 隐藏第3列
    leftView->setColumnHidden(4, true);  // 隐藏第4列
    leftView->setColumnHidden(5, true);  // 隐藏第5列
    leftView->header()->setStretchLastSection(true);  // 最后一列拉伸填充

    // 连接上下文菜单请求信号
    connect(ui->ganttView->leftView(), &QAbstractItemView::customContextMenuRequested,
            this, &MainWindow::showContextMenu);
    // 连接选择变化信号
    connect(ui->ganttView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::enableActions);

    // 连接甘特图视图点击信号
    connect(ui->ganttView->graphicsView(), &KDGantt::GraphicsView::clicked,
            this, &MainWindow::slotClicked);
    connect(ui->ganttView->graphicsView(), &KDGantt::GraphicsView::qrealClicked,
            this, &MainWindow::slotDoubleClicked);
}

/**
 * @brief 初始化数据模型
 * 创建任务数据模型和约束模型，并设置甘特图的模型和图例
 */
void MainWindow::initModel()
{
    // 创建标准项模型，6列数据
    model = new QStandardItemModel(0, 6, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Task"));  // 设置第0列标题
    ui->ganttView->setModel(model);  // 设置甘特图的数据模型

    // 创建并显示图例
    auto *l = new KDGantt::Legend;
    l->setWindowTitle(tr("Legend"));  // 设置图例窗口标题
    l->show();  // 显示图例
    l->setModel(model);  // 设置图例的数据模型

    // 创建约束模型并设置给甘特图
    constraintModel = new KDGantt::ConstraintModel(this);
    ui->ganttView->setConstraintModel(constraintModel);
}

/**
 * @brief 初始化动作和菜单
 * 创建应用程序的各种动作、菜单和快捷键，并连接信号与槽
 */
void MainWindow::initActions()
{
    // 创建新建条目动作
    newEntryAction = new QAction(tr("New entry"), this);
    newEntryAction->setShortcut(QKeySequence::New);  // 设置快捷键
    connect(newEntryAction, &QAction::triggered, this, &MainWindow::addNewEntry);  // 连接触发信号

    // 创建删除条目动作
    removeEntryAction = new QAction(tr("Remove entry"), this);
    removeEntryAction->setShortcut(QKeySequence::Delete);  // 设置快捷键
    connect(removeEntryAction, &QAction::triggered, this, &MainWindow::removeEntry);  // 连接触发信号

    // 创建演示条目动作
    demoAction = new QAction(tr("Demo entry"), this);
    connect(demoAction, &QAction::triggered, this, &MainWindow::addDemoEntry);  // 连接触发信号

    // 创建打印预览动作
    printAction = new QAction(tr("Print Preview..."), this);
    connect(printAction, &QAction::triggered, this, &MainWindow::printPreview);  // 连接触发信号

    // 创建放大动作
    zoomInAction = new QAction(tr("Zoom In"), this);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);  // 设置快捷键
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);  // 连接触发信号

    // 创建缩小动作
    zoomOutAction = new QAction(tr("Zoom Out"), this);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);  // 设置快捷键
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);  // 连接触发信号

    // 设置左侧视图的上下文菜单策略
    ui->ganttView->leftView()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ganttView->leftView()->addAction(newEntryAction);  // 添加新建动作
    ui->ganttView->leftView()->addAction(removeEntryAction);  // 添加删除动作

    // 创建条目菜单
    QMenu *entryMenu = menuBar()->addMenu(tr("Entry"));
    entryMenu->addAction(newEntryAction);  // 添加新建动作
    entryMenu->addAction(removeEntryAction);  // 添加删除动作
    entryMenu->addSeparator();  // 添加分隔线
    entryMenu->addAction(demoAction);  // 添加演示动作
    entryMenu->addSeparator();  // 添加分隔线
    entryMenu->addAction(printAction);  // 添加打印动作

    // 创建缩放菜单
    QMenu *zoomMenu = menuBar()->addMenu(tr("Zoom"));
    zoomMenu->addAction(zoomInAction);  // 添加放大动作
    zoomMenu->addAction(zoomOutAction);  // 添加缩小动作

    enableActions(QItemSelection());  // 初始化动作状态
}

/**
 * @brief 初始化项委托
 * 设置自定义的条目委托，用于处理任务条目的编辑和约束
 */
void MainWindow::initItemDelegate()
{
    // 创建条目委托，传入约束模型
    auto *delegate = new EntryDelegate(constraintModel, this);
    ui->ganttView->leftView()->setItemDelegate(delegate);  // 设置左侧视图的项委托
}

/**
 * @brief 初始化甘特图网格
 * 创建日期时间网格并设置给甘特图视图
 */
void MainWindow::initGrid()
{
    grid = new KDGantt::DateTimeGrid();  // 创建日期时间网格
    grid->setDayWidth(dayWidth);  // 设置日宽度
    ui->ganttView->setGrid(grid);  // 设置甘特图的网格
}

/**
 * @brief 显示上下文菜单
 * 在指定位置显示上下文菜单，提供条目操作选项
 * @param pos 上下文菜单的显示位置（视图坐标）
 */
void MainWindow::showContextMenu(const QPoint &pos)
{
    // 如果点击位置没有有效索引，清除选择
    if (!ui->ganttView->leftView()->indexAt(pos).isValid())
        ui->ganttView->selectionModel()->clearSelection();

    // 创建并显示上下文菜单
    QMenu menu(ui->ganttView->leftView());
    menu.addAction(newEntryAction);  // 添加新建动作
    menu.addAction(removeEntryAction);  // 添加删除动作
    menu.exec(ui->ganttView->leftView()->viewport()->mapToGlobal(pos));  // 在全局坐标显示菜单
}

/**
 * @brief 根据选择状态启用或禁用动作
 * 根据当前选择的条目类型更新动作的可用性
 * @param selected 当前选择的模型索引
 */
void MainWindow::enableActions(const QItemSelection &selected)
{
    // 如果没有选择任何索引
    if (selected.indexes().isEmpty()) {
        newEntryAction->setEnabled(true);  // 启用新建动作
        removeEntryAction->setEnabled(false);  // 禁用删除动作
        return;
    }

    QModelIndex selectedIndex = selected.indexes()[0];  // 获取第一个选择的索引

    // 检查选择的条目类型
    if (model->data(model->index(selectedIndex.row(), 1)) == KDGantt::TypeEvent || 
        model->data(model->index(selectedIndex.row(), 1)) == KDGantt::TypeTask) {
        newEntryAction->setEnabled(false);  // 禁用新建动作
        removeEntryAction->setEnabled(true);  // 启用删除动作
        return;
    }

    // 默认启用状态
    newEntryAction->setEnabled(true);
    removeEntryAction->setEnabled(true);
}

/**
 * @brief 添加新条目
 * 显示新建条目对话框，根据用户输入添加新任务到模型
 */
void MainWindow::addNewEntry()
{
    QPointer<EntryDialog> dialog = new EntryDialog(model);  // 创建条目对话框
    dialog->setWindowTitle(tr("New Entry"));  // 设置对话框标题
    // 如果对话框被拒绝或为空，删除对话框并返回
    if (dialog->exec() == QDialog::Rejected || !dialog) {
        delete dialog;
        return;
    }

    // 获取当前选择的索引
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    const QModelIndex parent = selectedIndexes.value(0);  // 父索引

    // 插入新行
    if (!model->insertRow(model->rowCount(parent), parent))
        return;

    int row = model->rowCount(parent) - 1;  // 获取新行索引
    // 如果是父索引的第一个子项，插入5列
    if (row == 0 && parent.isValid())
        model->insertColumns(model->columnCount(parent), 5, parent);

    // 设置新条目的数据
    model->setData(model->index(row, 0, parent), dialog->name());  // 设置名称
    model->setData(model->index(row, 1, parent), dialog->type());  // 设置类型
    // 如果不是摘要类型，设置开始和结束时间
    if (dialog->type() != KDGantt::TypeSummary) {
        model->setData(model->index(row, 2, parent), dialog->startDate(), KDGantt::StartTimeRole);
        model->setData(model->index(row, 3, parent), dialog->endDate(), KDGantt::EndTimeRole);
    }
    model->setData(model->index(row, 4, parent), dialog->completion());  // 设置完成度
    model->setData(model->index(row, 5, parent), dialog->legend());  // 设置图例

    // 添加依赖约束
    addConstraint(dialog->depends(), model->index(row, 0, parent));
    // 设置只读属性
    setReadOnly(model->index(row, 0, parent), dialog->readOnly());

    delete dialog;  // 删除对话框
}

/**
 * @brief 设置条目只读属性
 * 设置指定索引的条目是否为只读状态
 * @param index 条目的模型索引
 * @param readOnly 是否只读
 */
void MainWindow::setReadOnly(const QModelIndex &index, bool readOnly)
{
    int row = index.row();  // 获取行号
    const QModelIndex parent = index.parent();  // 获取父索引
    QStandardItem *item;

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

void MainWindow::addConstraint(const QModelIndex &index1, const QModelIndex &index2)
{
    if (!index1.isValid() || !index2.isValid())
        return;

    KDGantt::Constraint c(index1, index2);
    ui->ganttView->constraintModel()->addConstraint(c);
}

void MainWindow::addConstraint(const QStandardItem *item1, const QStandardItem *item2)
{
    addConstraint(model->indexFromItem(item1), model->indexFromItem(item2));
}

void MainWindow::removeEntry()
{
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    QModelIndex index = selectedIndexes.value(0);

    if (!index.isValid())
        return;

    model->removeRow(index.row(), index.parent());
}

class MyStandardItem : public QStandardItem
{
public:
    MyStandardItem(const QVariant &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);
    }
    MyStandardItem(const QString &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);
    }
    MyStandardItem(const QDateTime &dt, int role)
        : QStandardItem()
    {
        setData(QVariant::fromValue(dt), role);
    }
};

void MainWindow::addDemoEntry()
{
    QStandardItem *softwareRelease = new MyStandardItem(tr("Software Release"));
    QStandardItem *codeFreeze = new MyStandardItem(tr("Code Freeze"));
    codeFreeze->setData(KDGantt::TextPositionRole, KDGantt::StyleOptionGanttItem::Right);
    QStandardItem *packaging = new MyStandardItem(tr("Packaging"));
    QStandardItem *upload = new MyStandardItem(tr("Upload"));
    QStandardItem *testing = new MyStandardItem(tr("Testing"));
    QStandardItem *updateDocumentation = new MyStandardItem(tr("Update Documentation"));

    QDateTime now = QDateTime::currentDateTime();

    softwareRelease->appendRow(QList<QStandardItem *>()
                               << codeFreeze << new MyStandardItem(KDGantt::TypeEvent)
                               << new MyStandardItem(now, KDGantt::StartTimeRole));
    softwareRelease->appendRow(QList<QStandardItem *>()
                               << packaging << new MyStandardItem(KDGantt::TypeTask)
                               << new MyStandardItem(now.addDays(5), KDGantt::StartTimeRole)
                               << new MyStandardItem(now.addDays(10), KDGantt::EndTimeRole));
    softwareRelease->appendRow(QList<QStandardItem *>()
                               << upload << new MyStandardItem(KDGantt::TypeTask)
                               << new MyStandardItem(now.addDays(10).addSecs(2 * 60 * 60), KDGantt::StartTimeRole)
                               << new MyStandardItem(now.addDays(11), KDGantt::EndTimeRole));
    softwareRelease->appendRow(QList<QStandardItem *>()
                               << testing << new MyStandardItem(KDGantt::TypeTask)
                               << new MyStandardItem(now.addSecs(3 * 60 * 60), KDGantt::StartTimeRole)
                               << new MyStandardItem(now.addDays(5), KDGantt::EndTimeRole));
    softwareRelease->appendRow(QList<QStandardItem *>()
                               << updateDocumentation << new MyStandardItem(KDGantt::TypeTask)
                               << new MyStandardItem(now.addSecs(3 * 60 * 60), KDGantt::StartTimeRole)
                               << new MyStandardItem(now.addDays(3), KDGantt::EndTimeRole));

    model->appendRow(QList<QStandardItem *>()
                     << softwareRelease << new MyStandardItem(KDGantt::TypeSummary));

    addConstraint(codeFreeze, packaging);
    addConstraint(codeFreeze, testing);
    addConstraint(codeFreeze, updateDocumentation);
    addConstraint(packaging, upload);
    addConstraint(testing, packaging);
    addConstraint(updateDocumentation, packaging);
}

void MainWindow::zoomIn()
{
    dayWidth += 10;
    if (dayWidth > 400)
        grid->setScale(KDGantt::DateTimeGrid::ScaleHour);

    grid->setDayWidth(dayWidth);
}

void MainWindow::zoomOut()
{
    dayWidth -= 10;
    if (dayWidth < 10)
        dayWidth = 10;

    if (dayWidth <= 400)
        grid->setScale(KDGantt::DateTimeGrid::ScaleDay);

    grid->setDayWidth(dayWidth);
}

void MainWindow::printPreview()
{
    auto *preview = new QLabel(this, Qt::Window);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setScaledContents(true);
    preview->setWindowTitle(tr("Print Preview"));
    QPixmap pix(1000, 300);
    pix.fill(Qt::white);
    {
        QPainter p(&pix);
        p.setRenderHints(QPainter::Antialiasing);
        ui->ganttView->print(&p, pix.rect());
    }
    preview->setPixmap(pix);
    preview->show();
}

void MainWindow::slotClicked(const QModelIndex &index)
{
    statusBar()->showMessage(tr("(%1,%2,_,%4) clicked")
                                 .arg(index.row())
                                 .arg(index.column())
                                 .arg(( quint64 )index.model()));
}
void MainWindow::slotDoubleClicked(const QModelIndex &index)
{
    statusBar()->showMessage(tr("(%1,%2,_,%4) qreal clicked")
                                 .arg(index.row())
                                 .arg(index.column())
                                 .arg(( quint64 )index.model()));
}
