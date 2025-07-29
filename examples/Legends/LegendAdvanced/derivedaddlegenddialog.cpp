/****************************************************************************
**
** 此文件是KD Chart库的示例文件。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/

#include "derivedaddlegenddialog.h"

#include <KDChartPosition>

/**
 * @brief 构造函数
 * @param parent 父窗口部件
 * 
 * 创建一个添加图例对话框实例，并设置其UI。
 */
DerivedAddLegendDialog::DerivedAddLegendDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); // 设置UI
}
