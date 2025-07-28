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

#include <QFileDialog>
#include <QtGui>

#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>

using namespace KDChart;

/**
 * @brief 主窗口构造函数
 * @param parent 父窗口指针
 * 
 * 初始化界面、图表和数据模型，设置图表属性和数据。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 初始化UI组件

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart(); // 创建图表对象
    m_chart->setGlobalLeading(20, 20, 20, 20); // 设置全局边距
    chartLayout->addWidget(m_chart); // 将图表添加到布局

    // 初始化数据模型
    const int rowCount = 8; // 行数
    const int columnCount = 3; // 列数
    m_model = new QStandardItemModel(rowCount, columnCount, this); // 创建数据模型
    m_model->setHeaderData(0, Qt::Horizontal, tr("Product A")); // 设置表头数据
    m_model->setHeaderData(1, Qt::Horizontal, tr("Product B"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Product C"));
    openFile(":/Charts/qtdata.cht"); // 从资源文件加载数据

    // 设置线图
    m_lines = new LineDiagram(); // 创建线图对象
    m_lines->setModel(m_model); // 设置数据模型

    // 添加坐标轴
    auto *xAxis = new CartesianAxis(m_lines); // X轴
    auto *yAxis = new CartesianAxis(m_lines); // Y轴
    xAxis->setPosition(KDChart::CartesianAxis::Bottom); // X轴位置：底部
    yAxis->setPosition(KDChart::CartesianAxis::Left); // Y轴位置：左侧
    m_lines->addAxis(xAxis); // 添加X轴
    m_lines->addAxis(yAxis); // 添加Y轴

    // 设置线条宽度
    for (int iColumn = 0; iColumn < columnCount; ++iColumn) {
        QPen linePen(m_lines->pen(iColumn));
        linePen.setWidth(3); // 设置线条宽度为3
        m_lines->setPen(iColumn, linePen);
    }

    // 将图表添加到坐标系
    m_chart->coordinatePlane()->replaceDiagram(m_lines);

    // 添加图例
    auto *legend = new Legend(m_lines, m_chart); // 创建图例
    legend->setPosition(Position::South); // 图例位置：底部
    legend->setAlignment(Qt::AlignCenter); // 对齐方式：居中
    legend->setShowLines(true); // 显示连接线
    legend->setTitleText(""); // 无标题
    legend->setOrientation(Qt::Horizontal); // 水平方向
    legend->addDiagram(m_lines); // 添加图表到图例
    m_chart->addLegend(legend); // 将图例添加到图表
}

/**
 * @brief 数据集1显示/隐藏槽函数
 * @param checked 是否显示
 * 
 * 根据复选框状态设置数据集1的显示/隐藏状态。
 */
void MainWindow::on_showDataset1CB_toggled(bool checked)
{
    setHidden(0, !checked); // 设置数据集0的隐藏状态
}

/**
 * @brief 数据集2显示/隐藏槽函数
 * @param checked 是否显示
 * 
 * 根据复选框状态设置数据集2的显示/隐藏状态。
 */
void MainWindow::on_showDataset2CB_toggled(bool checked)
{
    setHidden(1, !checked); // 设置数据集1的隐藏状态
}

/**
 * @brief 数据集3显示/隐藏槽函数
 * @param checked 是否显示
 * 
 * 根据复选框状态设置数据集3的显示/隐藏状态。
 */
void MainWindow::on_showDataset3CB_toggled(bool checked)
{
    setHidden(2, !checked); // 设置数据集2的隐藏状态
}

/**
 * @brief 设置数据集隐藏状态
 * @param dataset 数据集索引
 * @param hidden 是否隐藏
 * 
 * 设置指定数据集的隐藏状态并更新图表。
 */
void MainWindow::setHidden(int dataset, bool hidden)
{
    m_lines->setHidden(dataset, hidden); // 设置数据集隐藏状态
    m_chart->update(); // 更新图表
}

/**
 * @brief 打开文件并加载数据
 * @param path 文件路径
 * 
 * 从指定路径加载CSV格式的数据文件，并更新数据模型。
 */
void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("选择数据文件"),
                                                "", "*.cht"); // 打开文件对话框
    else
        fileName = path; // 使用指定路径

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
                    m_model->setData(m_model->index(row, 0, QModelIndex()), pieces.value(0)); // 设置数据
                    m_model->setData(m_model->index(row, 1, QModelIndex()), pieces.value(1));
                    m_model->setData(m_model->index(row, 2, QModelIndex()), pieces.value(2));
                    ++row;
                }
            } while (!line.isEmpty());

            file.close(); // 关闭文件
        }
    }
}
// TODO: Qt5.15.2升级 检查KDChart::Legend在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QFileDialog和QTextStream在Qt5.15.2中的行为变化
// TODO: C++17升级 考虑使用文件系统库(std::filesystem)替代QFile
// TODO: C++17升级 考虑使用结构化绑定优化数据处理
