/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <TableModel.h>

namespace KDChart {
class Chart;          // 图表类
class BarDiagram;     // 柱状图类
class DatasetProxyModel; // 数据集代理模型类
}

QT_BEGIN_NAMESPACE
class QAbstractItemView;  // 抽象项视图
class QStandardItemModel; // 标准项模型
class QTableView;         // 表格视图
class QItemSelectionModel; // 项选择模型
QT_END_NAMESPACE

/**
 * @brief 主窗口类
 * 
 * 继承自QMainWindow，实现了将KD Chart图表与QTableView结合使用的功能，
 * 支持数据的可视化展示、编辑和选择交互。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 初始化界面、模型和视图，设置菜单和连接信号槽。
     */
    MainWindow();

protected slots:
    /**
     * @brief 选择变化槽函数
     * @param selected 选中的项
     * @param deselected 取消选中的项
     * 
     * 处理表格视图中的选择变化，更新图表中对应项的显示样式。
     */
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private slots:
    /**
     * @brief 初始化数据
     * 
     * 从资源文件加载初始数据。
     */
    void initializeData();

    /**
     * @brief 打开文件
     * @param path 文件路径，默认为空
     * 
     * 打开指定路径的数据文件，加载数据到模型中。
     */
    void openFile(const QString &path = QString());

    /**
     * @brief 保存文件
     * 
     * 将当前数据保存到文件。
     */
    void saveFile();

    /**
     * @brief 项点击槽函数
     * @param index 点击的项索引
     * 
     * 处理图表中项的点击事件，同步更新表格视图的选择状态。
     */
    void itemClicked(const QModelIndex &index);

private:
    /**
     * @brief 设置模型
     * 
     * 初始化数据模型，设置表头信息。
     */
    void setupModel();

    /**
     * @brief 设置视图
     * 
     * 初始化图表和表格视图，配置布局和代理模型。
     */
    void setupViews();

    KDChart::Chart *m_chart;                  // 图表对象
    KDChart::DatasetProxyModel *m_datasetProxy; // 数据集代理模型

    QStandardItemModel *m_model;              // 数据模型
    KDChart::BarDiagram *m_diagramView;       // 柱状图视图
    QTableView *m_tableView;                  // 表格视图
    QItemSelectionModel *m_selectionModel;    // 选择模型
};

#endif
// TODO: Qt5.15.2升级 检查KDChart相关类在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QItemSelectionModel使用规范
// TODO: C++17升级 考虑使用std::optional优化可能为空的值
