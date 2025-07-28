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

#include "derivedaddlegenddialog.h"
#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class LineDiagram;
}

/**
 * @brief 应用程序的主窗口类
 * 
 * 该类继承自QWidget，用于展示图例高级功能的示例。
 * 包含图表、数据模型和线图，并提供添加、编辑和删除图例的功能。
 */
class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化主窗口，创建图表、数据模型和线图，并设置UI。
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 添加图例按钮点击槽函数
     * 
     * 当用户点击"添加图例"按钮时触发，打开添加图例对话框。
     */
    void on_addLegendPB_clicked();

    /**
     * @brief 编辑图例按钮点击槽函数
     * 
     * 当用户点击"编辑图例"按钮时触发，打开编辑图例对话框。
     */
    void on_editLegendPB_clicked();

    /**
     * @brief 删除图例按钮点击槽函数
     * 
     * 当用户点击"删除图例"按钮时触发，删除选中的图例。
     */
    void on_removeLegendPB_clicked();

    /**
     * @brief 图例列表选择项变更槽函数
     * 
     * 当用户在图例列表中选择不同项时触发，更新编辑和删除按钮的状态。
     */
    void on_legendsTV_itemSelectionChanged();

private:
    /**
     * @brief 初始化添加图例对话框
     * @param conf 图例配置对话框
     * @param alignment 对齐方式
     * 
     * 设置添加图例对话框的初始配置，包括对齐方式等。
     */
    void initAddLegendDialog(DerivedAddLegendDialog &conf,
                             Qt::Alignment alignment) const;

    KDChart::Chart *m_chart; // 图表对象
    TableModel m_model; // 数据模型
    KDChart::LineDiagram *m_lines; // 线图对象
    QMap<Qt::Alignment, QString> alignmentMap; // 对齐方式映射表
};

#endif /* MAINWINDOW_H */
