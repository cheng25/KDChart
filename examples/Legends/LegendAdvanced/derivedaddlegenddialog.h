/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#ifndef DERIVEDADDLEGENDDIALOG_H
#define DERIVEDADDLEGENDDIALOG_H

#include "ui_addlegenddialog.h"

/**
 * @brief 派生的添加图例对话框类
 * 
 * 继承自QDialog和Ui::AddLegendDialog，用于显示和处理添加图例的用户界面。
 * 该对话框允许用户配置图例的位置、对齐方式、标题、方向和样式等属性。
 */
class DerivedAddLegendDialog : public QDialog, public Ui::AddLegendDialog
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 创建一个添加图例对话框实例。
     */
    DerivedAddLegendDialog(QWidget *parent = nullptr);
};

#endif // DERIVEDADDLEGENDDIALOG_H
