/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <QtGui>

#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartDatasetProxyModel>
#include <KDChartHeaderFooter>
#include <KDChartTextAttributes>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTableView>

using namespace KDChart;

/**
 * @brief 主窗口构造函数
 * 
 * 初始化菜单、模型、视图和信号槽连接，设置窗口标题和大小。
 */
MainWindow::MainWindow()
{
    auto *fileMenu = new QMenu(tr("&File"), this); // 创建文件菜单
    QAction *openAction = fileMenu->addAction(tr("&Open...")); // 添加打开动作
    openAction->setShortcut(QKeySequence(tr("Ctrl+O"))); // 设置快捷键
    QAction *saveAction = fileMenu->addAction(tr("&Save As...")); // 添加保存动作
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S"))); // 设置快捷键
    QAction *quitAction = fileMenu->addAction(tr("E&xit")); // 添加退出动作
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q"))); // 设置快捷键

    setupModel(); // 设置数据模型
    initializeData(); // 初始化数据
    setupViews(); // 设置视图

    // 连接打开文件动作信号
    connect(openAction, &QAction::triggered, this, [this] {
        openFile();
    });
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile); // 连接保存文件信号
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit); // 连接退出信号

    // 连接选择变化信号
    connect(m_selectionModel, &QItemSelectionModel::selectionChanged,
            this, &MainWindow::selectionChanged);

    // 连接图表点击信号
    connect(m_diagramView, &BarDiagram::clicked, this, &MainWindow::itemClicked);

    menuBar()->addMenu(fileMenu); // 添加菜单到菜单栏
    statusBar(); // 显示状态栏

    setWindowTitle(tr("KD Chart used as item viewer together with a QTableView")); // 设置窗口标题
    resize(740, 480); // 设置窗口大小
}

/**
 * @brief 设置数据模型
 * 
 * 初始化QStandardItemModel，设置行数、列数和表头数据。
 */
void MainWindow::setupModel()
{
    const int rowCount = 8; // 行数
    const int columnCount = 3; // 列数
    m_model = new QStandardItemModel(rowCount, columnCount, this); // 创建数据模型
    m_model->setHeaderData(0, Qt::Horizontal, tr("Label")); // 设置表头数据
    m_model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Product A"));
}

/**
 * @brief 初始化数据
 * 
 * 从资源文件加载初始数据。
 */
void MainWindow::initializeData()
{
    openFile(":/Charts/qtdata.cht"); // 从资源文件加载数据
}

/**
 * @brief 设置视图
 * 
 * 初始化图表和表格视图，配置布局、代理模型和图表属性。
 */
void MainWindow::setupViews()
{
    m_chart = new Chart(); // 创建图表对象

    m_tableView = new QTableView; // 创建表格视图

    auto *splitter = new QSplitter(Qt::Vertical); // 创建垂直分割器
    splitter->addWidget(m_tableView); // 添加表格视图
    splitter->addWidget(m_chart); // 添加图表
    splitter->setStretchFactor(0, 0); // 设置拉伸因子
    splitter->setStretchFactor(1, 1);

    // 设置图表
    m_diagramView = new BarDiagram(); // 创建柱状图

    DatasetDescriptionVector columnConfig(m_model->columnCount() - 1);
    for (int iC = 1; iC <= columnConfig.size(); ++iC)
        columnConfig[iC - 1] = iC;

    qDebug() << "(" << m_model->rowCount() << "," << m_model->columnCount() << ")";

    auto *dproxy = new KDChart::DatasetProxyModel(this); // 创建数据集代理模型
    dproxy->setSourceModel(m_model); // 设置源模型
    dproxy->setDatasetColumnDescriptionVector(columnConfig); // 设置列配置
    m_diagramView->setModel(dproxy); // 设置图表模型

    auto *headerFooter = new KDChart::HeaderFooter(m_chart); // 创建页眉
    headerFooter->setText(tr("You can edit the table data, or select table cells with keyboard/mouse."));
    headerFooter->setType(HeaderFooter::Header); // 设置为页眉
    headerFooter->setPosition(Position::North); // 设置位置为顶部
    m_chart->addHeaderFooter(headerFooter); // 添加页眉到图表
    m_chart->coordinatePlane()->replaceDiagram(m_diagramView); // 替换图表

    m_tableView->setModel(m_model); // 设置表格模型

    m_selectionModel = new QItemSelectionModel(m_model); // 创建选择模型
    m_tableView->setSelectionModel(m_selectionModel); // 设置表格选择模型

    setCentralWidget(splitter); // 设置中央部件
}

/**
 * @brief 图表项点击槽函数
 * @param index 点击的项索引
 * 
 * 处理图表中项的点击事件，同步更新表格视图的选择状态。
 */
void MainWindow::itemClicked(const QModelIndex &index)
{
    QItemSelectionModel::SelectionFlags command = QItemSelectionModel::Clear | QItemSelectionModel::Select;
    if (QApplication::keyboardModifiers() & Qt::ControlModifier)
        command = QItemSelectionModel::Toggle;
    m_selectionModel->setCurrentIndex(static_cast<const QAbstractProxyModel *>(index.model())
                                          ->mapToSource(index),
                                      command);
}

/**
 * @brief 选择变化槽函数
 * @param selected 选中的项
 * @param deselected 取消选中的项
 * 
 * 处理表格视图中的选择变化，更新图表中对应项的显示样式。
 */
void MainWindow::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (deselected != selected) {

        // 首先设置未选中的条形图使用不可见的画笔，然后设置选中的使用深蓝色画笔
        const QItemSelection *pItemSelection = &deselected;
        QPen pen(Qt::NoPen);

        for (int iItemSelection = 0; iItemSelection < 2; ++iItemSelection) {
            for (int i = 0; i < pItemSelection->size(); ++i) {
                QItemSelectionRange range(pItemSelection->at(i));
                for (int iRow = range.topLeft().row(); iRow <= range.bottomRight().row(); ++iRow) {
                    for (int iColumn = range.topLeft().column(); iColumn <= range.bottomRight().column(); ++iColumn) {
                        // 忽略第一列：这只是表格视图中显示的标签文本
                        if (iColumn)
                            // 启用（或禁用）此条形图周围的线条
                            m_diagramView->setPen(m_diagramView->model()
                                                      ->index(iRow, iColumn - 1, m_diagramView->rootIndex()),
                                                  pen);
                    }
                }
            }
            pItemSelection = &selected;
            pen.setColor(Qt::darkBlue);
            pen.setStyle(Qt::DashLine);
            pen.setWidth(2);
        }

        m_chart->update(); // 更新图表
    }
}

/**
 * @brief 打开文件
 * @param path 文件路径，默认为空
 * 
 * 打开指定路径的数据文件，加载数据到模型中。
 */
void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"), "", "*.cht");
    else
        fileName = path;

    if (!fileName.isEmpty()) {
        QFile file(fileName); // 创建文件对象

        if (file.open(QFile::ReadOnly | QFile::Text)) { // 以只读文本模式打开文件
            QTextStream stream(&file); // 创建文本流
            QString line;

            m_model->removeRows(0, m_model->rowCount(QModelIndex()), QModelIndex()); // 清空现有数据

            int row = 0;
            do {
                line = stream.readLine(); // 读取一行
                if (!line.isEmpty()) {

                    m_model->insertRows(row, 1, QModelIndex()); // 插入新行

                    QStringList pieces = line.split(',', Qt::SkipEmptyParts); // 分割逗号分隔的值
                    m_model->setData(m_model->index(row, 0, QModelIndex()),
                                     pieces.value(0));
                    m_model->setData(m_model->index(row, 1, QModelIndex()),
                                     pieces.value(1));
                    m_model->setData(m_model->index(row, 2, QModelIndex()),
                                     pieces.value(2));
                    m_model->setData(m_model->index(row, 0, QModelIndex()),
                                     QColor(pieces.value(3)), Qt::DecorationRole);
                    ++row;
                }
            } while (!line.isEmpty());

            file.close(); // 关闭文件
            statusBar()->showMessage(tr("Loaded %1").arg(fileName), 2000); // 显示加载成功消息
        }
    }
}

/**
 * @brief 保存文件
 * 
 * 将当前数据保存到文件。
 */
void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as"), "", "*.cht"); // 获取保存文件名

    if (!fileName.isEmpty()) {
        QFile file(fileName); // 创建文件对象
        QTextStream stream(&file); // 创建文本流

        if (file.open(QFile::WriteOnly | QFile::Text)) { // 以只写文本模式打开文件
            for (int row = 0; row < m_model->rowCount(QModelIndex()); ++row) { // 遍历所有行

                QStringList pieces; // 存储数据片段

                pieces.append(m_model->data(m_model->index(row, 0, QModelIndex()),
                                            Qt::DisplayRole)
                                  .toString());
                pieces.append(m_model->data(m_model->index(row, 1, QModelIndex()),
                                            Qt::DisplayRole)
                                  .toString());
                pieces.append(m_model->data(m_model->index(row, 2, QModelIndex()),
                                            Qt::DisplayRole)
                                  .toString());
                pieces.append(m_model->data(m_model->index(row, 0, QModelIndex()),
                                            Qt::DecorationRole)
                                  .toString());

                stream << pieces.join(",") << "\n"; // 写入数据
            }
        }

        file.close(); // 关闭文件
        statusBar()->showMessage(tr("Saved %1").arg(fileName), 2000); // 显示保存成功消息
    }
}
// TODO: Qt5.15.2升级 检查KDChart::HeaderFooter在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QFileDialog和QTextStream在Qt5.15.2中的行为变化
// TODO: C++17升级 考虑使用文件系统库(std::filesystem)替代QFile
// TODO: C++17升级 考虑使用结构化绑定优化数据处理
