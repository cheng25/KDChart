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

#include <QDialog>
#include <QMap>

#include "ui_addheaderdialog.h"
#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
}

/**
 * @brief 主窗口类
 * 
 * 该类继承自QWidget，用于展示高级页眉和页脚管理功能。
 * 允许用户添加、编辑和删除图表的页眉和页脚，并提供相应的界面控件。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化图表、数据模型、连接线图，并设置UI控件。
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_addHeaderPB_clicked();      // 添加页眉按钮点击事件
    void on_editHeaderPB_clicked();     // 编辑页眉按钮点击事件
    void on_removeHeaderPB_clicked();   // 删除页眉按钮点击事件
    void on_headersTV_itemSelectionChanged(); // 页眉表格选择项改变事件

private:
    /**
     * @brief 设置添加页眉对话框
     * @param dlg 对话框指针
     * @param conf 对话框UI配置
     * 
     * 配置添加页眉对话框的初始状态和属性。
     */
    void setupAddHeaderDialog(QDialog *dlg,
                              Ui::AddHeaderDialog &conf) const;

    KDChart::Chart *m_chart;               // 图表对象
    TableModel m_model;                    // 数据模型
    KDChart::DatasetProxyModel *m_datasetProxy; // 数据集代理模型
    KDChart::LineDiagram *m_lines;         // 线图对象
};

#endif /* MAINWINDOW_H */
