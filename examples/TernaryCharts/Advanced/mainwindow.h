/****************************************************************************
**
** 此文件是KD Chart库的示例程序的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// 包含Qt对话框类的头文件
#include <QDialog>
// 包含Qt标准项模型类的头文件
#include <QStandardItemModel>

// 包含UI头文件
#include "ui_mainwindow.h"

// KDChart命名空间
namespace KDChart {

class Chart;
class TernaryLineDiagram;
class TernaryPointDiagram;
class TernaryCoordinatePlane;
}

/**
 * @brief 主窗口类，用于展示三元图表
 * 该类继承自QDialog和Ui::MainWindow，实现了三元图表的展示和交互功能
 */
class MainWindow : public QDialog,
                   private Ui::MainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 索引点击槽函数
     * @param index 被点击的模型索引
     */
    void indexClicked(const QModelIndex &index);

private:
    /**
     * @brief 设置数据模型
     */
    void setupModel();

    // 图表对象指针
    KDChart::Chart *m_chart = nullptr;
    // 三元点图表对象指针
    KDChart::TernaryPointDiagram *m_diagram = nullptr;
    // 三元坐标平面对象指针
    KDChart::TernaryCoordinatePlane *m_ternaryPlane = nullptr;

    // 标准项数据模型
    QStandardItemModel m_model;
};

// TODO: Qt5.15.2升级 检查KDChart的三元图表API是否有变更
// TODO: C++17升级 考虑使用std::unique_ptr管理动态内存

#endif
