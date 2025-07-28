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
#include "projectmodel.h"

#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QModelIndex>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGlobal>
#include <KDGanttGraphicsView>
#include <KDGanttItemDelegate>
#include <KDGanttStyleOptionGanttItem>
#include <KDGanttView>

/**
 * @brief 项目类型组合框类
 * 
 * 自定义的组合框，用于选择甘特图项目类型
 */
class ItemTypeComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(KDGantt::ItemType itemType READ itemType WRITE setItemType)
public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    explicit ItemTypeComboBox(QWidget *parent = nullptr);

    /**
     * @brief 获取项目类型
     * 
     * @return 项目类型枚举值
     */
    KDGantt::ItemType itemType() const;

public slots:
    /**
     * @brief 设置项目类型
     * 
     * @param typ 项目类型枚举值
     */
    void setItemType(KDGantt::ItemType typ);
};

/**
 * @brief ItemTypeComboBox构造函数实现
 * 
 * @param parent 父窗口指针
 */
ItemTypeComboBox::ItemTypeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    // 添加项目类型选项
    addItem(tr("Task"), QVariant(KDGantt::TypeTask));
    addItem(tr("Event"), QVariant(KDGantt::TypeEvent));
    addItem(tr("Summary"), QVariant(KDGantt::TypeSummary));
}

/**
 * @brief 获取项目类型实现
 * 
 * @return 项目类型枚举值
 */
KDGantt::ItemType ItemTypeComboBox::itemType() const
{
    // 从当前索引获取数据并转换为项目类型
    return static_cast<KDGantt::ItemType>(itemData(currentIndex()).toInt());
}

/**
 * @brief 设置项目类型实现
 * 
 * @param typ 项目类型枚举值
 */
void ItemTypeComboBox::setItemType(KDGantt::ItemType typ)
{
    // 设置当前索引为类型减1（因为枚举从1开始）
    setCurrentIndex(typ - 1);
}

/**
 * @brief 自定义项目委托类
 * 
 * 用于自定义甘特图项目的显示和编辑
 */
class MyItemDelegate : public KDGantt::ItemDelegate
{
public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父对象指针
     */
    explicit MyItemDelegate(QObject *parent = nullptr);

    /**
     * @brief 创建编辑器
     * 
     * @param parent 父窗口
     * @param option 样式选项
     * @param idx 模型索引
     * @return 编辑器窗口部件
     */
    /*reimp*/ QWidget *createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &idx) const override;

    /**
     * @brief 设置编辑器数据
     * 
     * @param editor 编辑器窗口部件
     * @param index 模型索引
     */
    /*reimp*/ void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    /**
     * @brief 设置模型数据
     * 
     * @param editor 编辑器窗口部件
     * @param model 模型指针
     * @param index 模型索引
     */
    /*reimp*/ void setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const override;

protected:
    /**
     * @brief 绘制显示内容
     * 
     * @param painter 画家
     * @param option 样式选项
     * @param rect 绘制矩形
     * @param text 文本内容
     */
    /*reimp*/ void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                               const QRect &rect, const QString &text) const override;
};

/**
 * @brief MyItemDelegate构造函数实现
 * 
 * @param parent 父对象指针
 */
MyItemDelegate::MyItemDelegate(QObject *parent)
    : KDGantt::ItemDelegate(parent)
{
}

/**
 * @brief 创建编辑器实现
 * 
 * @param parent 父窗口
 * @param option 样式选项
 * @param idx 模型索引
 * @return 编辑器窗口部件
 */
QWidget *MyItemDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &idx) const
{
    qDebug() << "MyItemDelegate::createEditor(" << parent << idx << ")";
    // 如果索引有效且是第二列（类型列），创建自定义组合框
    if (idx.isValid() && idx.column() == 1)
        return new ItemTypeComboBox(parent);
    // 否则使用默认编辑器
    return ItemDelegate::createEditor(parent, option, idx);
}

/**
 * @brief 设置编辑器数据实现
 * 
 * @param editor 编辑器窗口部件
 * @param index 模型索引
 */
void MyItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ItemTypeComboBox *c;
    // 如果是自定义组合框
    if ((c = qobject_cast<ItemTypeComboBox *>(editor)) && index.isValid()) {
        // 设置项目类型
        c->setItemType(static_cast<KDGantt::ItemType>(index.data(Qt::EditRole).toInt()));
    } else {
        // 否则使用默认方法
        ItemDelegate::setEditorData(editor, index);
    }
}

/**
 * @brief 设置模型数据实现
 * 
 * @param editor 编辑器窗口部件
 * @param model 模型指针
 * @param index 模型索引
 */
void MyItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    ItemTypeComboBox *c;
    // 如果是自定义组合框
    if ((c = qobject_cast<ItemTypeComboBox *>(editor)) && index.isValid()) {
        // 设置模型数据
        model->setData(index, c->itemType());
    } else {
        // 否则使用默认方法
        ItemDelegate::setModelData(editor, model, index);
    }
}

/**
 * @brief 绘制显示内容实现
 * 
 * @param painter 画家
 * @param option 样式选项
 * @param rect 绘制矩形
 * @param text 文本内容
 */
void MyItemDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                                 const QRect &rect, const QString &text) const
{
    // 将文本转换为项目类型
    auto typ = static_cast<KDGantt::ItemType>(text.toInt());
    QString str;
    // 根据类型设置显示文本
    switch (typ) {
    case KDGantt::TypeTask:
        str = tr("Task");
        break;
    case KDGantt::TypeEvent:
        str = tr("Event");
        break;
    case KDGantt::TypeSummary:
        str = tr("Summary");
        break;
    default:
        str = tr("None");
        break;
    }
    // 使用基类方法绘制文本
    ItemDelegate::drawDisplay(painter, option, rect, str);
}

///////////////////////////////////////////////////////////////////////////////
// 提供自定义背景和前景
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief 日期时间网格类
 * 
 * 自定义的甘特图日期时间网格，提供自定义背景和前景绘制
 */
class DateTimeGrid : public KDGantt::DateTimeGrid
{
public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父对象指针
     */
    DateTimeGrid(QObject *parent = nullptr)
    {
        setParent(parent);
        setFreeDays(QSet<Qt::DayOfWeek>());
        setFreeDaysBrush(QBrush(Qt::NoBrush));
    }

    /**
     * @brief 析构函数
     */
    ~DateTimeGrid() override
    {
    }

    // virtual void paintUserDefinedHeader(QPainter* painter, const QRectF& headerRect, const QRectF& exposedRect, qreal offset, const KDGantt::DateTimeScaleFormatter* formatter, QWidget* widget = 0);

    /**
     * @brief 绘制背景
     * 
     * @param painter 画家
     * @param rect 绘制矩形
     */
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    /**
     * @brief 绘制前景
     * 
     * @param painter 画家
     * @param rect 绘制矩形
     */
    void drawForeground(QPainter *painter, const QRectF &rect) override;
};

/**
 * @brief 绘制背景实现
 * 
 * @param painter 画家
 * @param rect 绘制矩形
 */
void DateTimeGrid::drawBackground(QPainter *painter, const QRectF &rect)
{
    QLinearGradient grad;
    grad.setCoordinateMode(QGradient::ObjectBoundingMode);
    grad.setStart(0.5, 0.5);
    grad.setFinalStop(0.5, 0.0);
    grad.setSpread(QGradient::ReflectSpread);
    QColor currentColor = Qt::blue;
    // 创建渐变色
    for (qreal i = 0; i <= 1.0; i += 0.1) {
        currentColor = currentColor.lighter(100 + 20 * i);
        grad.setColorAt(i, currentColor);
    }
    QBrush brush(grad);

    // 计算当前日期前后两天的矩形
    QRectF r = computeRect(QDateTime::currentDateTime(),
                           QDateTime::currentDateTime().addDays(2),
                           rect);
    // 填充渐变背景
    painter->fillRect(r, brush);
}

/**
 * @brief 绘制前景实现
 * 
 * @param painter 画家
 * @param rect 绘制矩形
 */
void DateTimeGrid::drawForeground(QPainter *painter, const QRectF &rect)
{
    painter->save();

    // 计算当前日期前后两天的矩形
    QRectF r = computeRect(QDateTime::currentDateTime(),
                           QDateTime::currentDateTime().addDays(2),
                           rect);

    static QString text("Holiday");
    QFont font = painter->font();
    font.setPixelSize(r.width() / 5);

    QFontMetrics fm(font);
    int width = fm.horizontalAdvance(text);
    int height = fm.boundingRect(text).height();

    // 移动到矩形中心
    painter->translate(r.center());
    painter->translate(-width / 2, height / 2);
    painter->setFont(font);
    painter->drawText(0, 0, text);

    painter->restore();
}

/**
 * @brief MainWindow构造函数实现
 * 
 * @param parent 父窗口指针
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_model(new ProjectModel(this))  // 创建项目模型
    , m_view(new KDGantt::View)  // 创建甘特图视图
{
    m_view->setModel(m_model);  // 设置视图模型
    m_view->setSelectionModel(new QItemSelectionModel(m_model));  // 设置选择模型

    // slotToolsNewItem();
    // 为左侧视图的第二列设置自定义委托
    m_view->leftView()->setItemDelegateForColumn(1, new MyItemDelegate(this));
    m_view->leftView()->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // 设置水平滚动条策略
    m_view->graphicsView()->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // 设置图形视图滚动条策略

    m_view->setGrid(new DateTimeGrid(this));  // 设置自定义网格

    setCentralWidget(m_view);  // 设置中心部件

    QMenuBar *mb = menuBar();  // 获取菜单栏

    auto *fileMenu = new QMenu(tr("&File"));  // 创建文件菜单

#ifndef QT_NO_PRINTER
    // 添加保存PDF和打印动作
    fileMenu->addAction(tr("&Save as PDF..."), this, &MainWindow::slotFileSavePdf);
    fileMenu->addAction(tr("&Print..."), this, &MainWindow::slotFilePrint);
#endif

    fileMenu->addSeparator();  // 添加分隔线
    fileMenu->addAction(tr("&Quit"), this, &MainWindow::slotFileQuit);  // 添加退出动作

    mb->addMenu(fileMenu);  // 添加文件菜单到菜单栏

    auto *toolsMenu = new QMenu(tr("&Tools"));  // 创建工具菜单

    // 添加新建项目、追加项目动作
    toolsMenu->addAction(tr("&New Item"), this, &MainWindow::slotToolsNewItem);
    toolsMenu->addAction(tr("&Add Item"), this, &MainWindow::slotToolsAppendItem);
    toolsMenu->addSeparator();  // 添加分隔线
    QMenu *alignMenu = toolsMenu->addMenu(tr("Ali&gn"));  // 创建对齐子菜单
    // 添加对齐动作
    alignMenu->addAction(tr("&Left"), this, &MainWindow::slotAlignLeft);
    alignMenu->addAction(tr("&Center"), this, &MainWindow::slotAlignCenter);
    alignMenu->addAction(tr("&Right"), this, &MainWindow::slotAlignRight);
    alignMenu->addAction(tr("&Hidden"), this, &MainWindow::slotAlignHidden);
    toolsMenu->addSeparator();  // 添加分隔线
    // 添加折叠和展开动作
    toolsMenu->addAction(tr("&Collapse All"), this, &MainWindow::slotCollapseAll);
    toolsMenu->addAction(tr("&Expand All"), this, &MainWindow::slotExpandAll);

    mb->addMenu(toolsMenu);  // 添加工具菜单到菜单栏
}

/**
 * @brief SavePdfDialog构造函数实现
 * 
 * @param parent 父窗口指针
 */
SavePdfDialog::SavePdfDialog(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);  // 设置为模态对话框
    setWindowTitle(tr("Save as PDF"));  // 设置窗口标题
    auto *l = new QVBoxLayout(this);  // 创建垂直布局
    setLayout(l);  // 设置布局

    auto *fileLayout = new QHBoxLayout(this);  // 创建水平布局
    l->addLayout(fileLayout);  // 添加到主布局
    auto *fileLabel = new QLabel(tr("File:"), this);  // 创建文件标签
    fileLayout->addWidget(fileLabel);  // 添加到水平布局
    m_fileEdit = new QLineEdit(this);  // 创建文件编辑框
    fileLabel->setBuddy(m_fileEdit);  // 设置伙伴控件
    // 设置默认文件路径
    m_fileEdit->setText(QFileInfo(QDir::homePath(), "gantt.pdf").absoluteFilePath());
    fileLayout->addWidget(m_fileEdit);  // 添加到水平布局
    auto *m_fileButton = new QPushButton("...", this);  // 创建文件按钮
    // 连接按钮点击信号到槽函数
    connect(m_fileButton, &QPushButton::clicked, this, &SavePdfDialog::fileButtonClicked);
    fileLayout->addWidget(m_fileButton);  // 添加到水平布局

    m_rowLabels = new QCheckBox(tr("Row Header"), this);  // 创建行标签复选框
    m_rowLabels->setChecked(true);  // 默认为选中
    l->addWidget(m_rowLabels);  // 添加到主布局

    m_columnLabels = new QCheckBox(tr("Column Header"), this);  // 创建列标签复选框
    m_columnLabels->setChecked(true);  // 默认为选中
    l->addWidget(m_columnLabels);  // 添加到主布局

    auto *btnBox = new QDialogButtonBox(this);  // 创建按钮盒
    btnBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);  // 设置标准按钮
    // 连接接受和拒绝信号
    connect(btnBox, &QDialogButtonBox::accepted, this, &SavePdfDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &SavePdfDialog::reject);
    l->addWidget(btnBox);  // 添加到主布局

    resize(QSize(400, 100).expandedTo(minimumSizeHint()));  // 调整大小
}

/**
 * @brief 文件按钮点击槽函数实现
 */
void SavePdfDialog::fileButtonClicked()
{
    // 打开文件保存对话框
    const QString file = QFileDialog::getSaveFileName(this, tr("Choose PDF File..."), QString(), tr("PDF files (*.pdf)"));
    if (!file.isEmpty())
        m_fileEdit->setText(file);  // 设置文件路径
}

/**
 * @brief 保存PDF文件槽函数实现
 */
void MainWindow::slotFileSavePdf()
{
#ifndef QT_NO_PRINTER
    SavePdfDialog dialog(this);  // 创建保存PDF对话框
    if (dialog.exec() != QDialog::Accepted)  // 执行对话框
        return;

    const QString file = dialog.m_fileEdit->text();  // 获取文件路径
    if (file.isEmpty())
        return;

    const bool drawRowLabels = dialog.m_rowLabels->isChecked();  // 获取行标签选项
    const bool drawColumnLabels = dialog.m_columnLabels->isChecked();  // 获取列标签选项

    QPrinter printer(QPrinter::HighResolution);  // 创建打印机
    printer.setPageOrientation(QPageLayout::Landscape);  // 设置为横向
    printer.setColorMode(QPrinter::Color);  // 设置为彩色
    printer.setPageMargins({0.2, 0.2, 0.2, 0.2}, QPageLayout::Point);  // 设置页边距
    printer.setOutputFormat(QPrinter::PdfFormat);  // 设置输出格式为PDF
    printer.setOutputFileName(file);  // 设置输出文件名
    m_view->print(&printer, drawRowLabels, drawColumnLabels);  // 打印视图
#endif
}

/**
 * @brief 打印槽函数实现
 */
void MainWindow::slotFilePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);  // 创建打印机
    printer.setPageOrientation(QPageLayout::Landscape);  // 设置为横向
    printer.setColorMode(QPrinter::Color);  // 设置为彩色
    QPrintDialog dialog(&printer, this);  // 创建打印对话框
    if (dialog.exec() != QDialog::Accepted)  // 执行对话框
        return;
    m_view->print(&printer);  // 打印视图
#endif
}

/**
 * @brief 退出槽函数实现
 */
void MainWindow::slotFileQuit()
{
    // TODO
    QApplication::instance()->quit();  // 退出应用程序
}

/**
 * @brief 新建项目项槽函数实现
 */
void MainWindow::slotToolsNewItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid()) {
        qDebug() << "MainWindow::slotToolsNewItem" << idx;
        // 在当前索引位置插入新行
        m_model->insertRows(0, 1, m_model->index(idx.row(), 0, idx.parent()));
    } else {
        // 在根索引位置插入新行
        m_model->insertRows(0, 1, m_view->rootIndex());
    }
}

/**
 * @brief 追加项目项槽函数实现
 */
void MainWindow::slotToolsAppendItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid()) {
        qDebug() << "MainWindow::slotToolsAppendItem" << idx;
        // 在当前索引后追加新行
        m_model->insertRows(m_model->rowCount(idx), 1, m_model->index(idx.row(), 0, idx.parent()));
    } else {
        // 在根索引后追加新行
        m_model->insertRows(m_model->rowCount(m_view->rootIndex()), 1, m_view->rootIndex());
    }
}

/**
 * @brief 全部折叠槽函数实现
 */
void MainWindow::slotCollapseAll()
{
    // 不要使用树视图的collapseAll/expandAll方法，而是使用视图提供的方法，因为它会负责更新所有内容
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid())
        m_view->collapseAll();  // 折叠所有项
}

/**
 * @brief 全部展开槽函数实现
 */
void MainWindow::slotExpandAll()
{
    // 不要使用树视图的collapseAll/expandAll方法，而是使用视图提供的方法，因为它会负责更新所有内容
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid())
        m_view->expandAll();  // 展开所有项
}

/**
 * @brief 左对齐槽函数实现
 */
void MainWindow::slotAlignLeft()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid()) {
        // 设置文本左对齐
        m_model->setData(idx, KDGantt::StyleOptionGanttItem::Left, KDGantt::TextPositionRole);
    }
}

/**
 * @brief 居中对齐槽函数实现
 */
void MainWindow::slotAlignCenter()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid()) {
        // 设置文本居中对齐
        m_model->setData(idx, KDGantt::StyleOptionGanttItem::Center, KDGantt::TextPositionRole);
    }
}

/**
 * @brief 右对齐槽函数实现
 */
void MainWindow::slotAlignRight()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid()) {
        // 设置文本右对齐
        m_model->setData(idx, KDGantt::StyleOptionGanttItem::Right, KDGantt::TextPositionRole);
    }
}

/**
 * @brief 隐藏对齐槽函数实现
 */
void MainWindow::slotAlignHidden()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();  // 获取当前选择索引
    if (idx.isValid()) {
        // 设置文本隐藏
        m_model->setData(idx, KDGantt::StyleOptionGanttItem::Hidden, KDGantt::TextPositionRole);
    }
}

#include "mainwindow.moc"  // 包含元对象编译文件
