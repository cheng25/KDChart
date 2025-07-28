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

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

namespace KDChart {
class Chart;
class LineDiagram;
}

/**
 * @brief 主窗口类
 * 
 * 继承自QWidget和Ui::MainWindow，实现了一个带有图表和数据隐藏功能的界面。
 * 提供了控制三个数据集显示/隐藏的功能，并能够从文件加载数据。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     * 
     * 初始化界面、图表和数据模型，设置信号槽连接。
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 数据集1显示/隐藏槽函数
     * @param checked 是否显示数据集1
     * 
     * 响应显示/隐藏数据集1的复选框状态变化。
     */
    void on_showDataset1CB_toggled(bool checked);
    /**
     * @brief 数据集2显示/隐藏槽函数
     * @param checked 是否显示数据集2
     * 
     * 响应显示/隐藏数据集2的复选框状态变化。
     */
    void on_showDataset2CB_toggled(bool checked);
    /**
     * @brief 数据集3显示/隐藏槽函数
     * @param checked 是否显示数据集3
     * 
     * 响应显示/隐藏数据集3的复选框状态变化。
     */
    void on_showDataset3CB_toggled(bool checked);
    /**
     * @brief 设置数据集隐藏状态槽函数
     * @param dataset 数据集索引
     * @param hidden 是否隐藏
     * 
     * 设置指定数据集的隐藏状态并更新图表。
     */
    void setHidden(int dataset, bool hidden);
    /**
     * @brief 打开文件槽函数
     * @param path 文件路径
     * 
     * 从指定路径加载数据文件并更新数据模型。
     */
    void openFile(const QString &path);

private:
    KDChart::Chart *m_chart; // 图表对象
    KDChart::LineDiagram *m_lines; // 线图对象
    QAbstractItemModel *m_model; // 数据模型对象
};

#endif /* MAINWINDOW_H */
// TODO: Qt5.15.2升级 检查KDChart::Chart和KDChart::LineDiagram在Qt5.15.2中的兼容性
// TODO: Qt5.15.2升级 验证QAbstractItemModel的使用是否符合Qt5.15.2规范
// TODO: C++17升级 考虑使用std::optional优化参数传递
