/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的高级部件示例的主窗口实现文件，包含MainWindow类的具体实现
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有: 2001 Klarälvdalens Datakonsult AB (KDAB集团公司)
**
** SPDX-License-Identifier: MIT
** 许可协议: MIT
**
****************************************************************************/

#include "mainwindow.h"         // 包含主窗口类头文件

#include <KDChartAbstractDiagram>  // 包含抽象图表类
#include <KDChartWidget>           // 包含图表部件类
#include <QDebug>                  // 包含调试输出类
#include <QMessageBox>             // 包含消息框类

using namespace KDChart;

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口指针，默认为nullptr
 * @details 完成以下关键初始化工作：
 *          1. 调用UI初始化函数setupUi
 *          2. 创建图表布局并添加KDChart::Widget实例
 *          3. 设置初始图表类型为折线图
 *          4. 连接UI控件信号与对应的槽函数
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 初始化UI组件
    setupUi(this);

    // 创建图表布局并添加图表部件
    auto *chartLayout = new QHBoxLayout(chartFrame);
    widget = new Widget(chartFrame);
    chartLayout->addWidget(widget);

    // 设置初始图表类型为折线图（索引为1）
    typeSelector->setCurrentIndex(1); // 初始为LineDiagram

    // 连接类型选择下拉框的信号与槽函数
    connect(typeSelector, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::changeType);

    // 连接添加数据集按钮的信号与槽函数
    connect(btnAddDataset, &QPushButton::clicked, this, &MainWindow::addDataset);

    // 连接间距选择器的信号与槽函数
    connect(leadingSelector, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::changeLeading);
}

/**
 * @brief 更改图表类型的槽函数
 * @details 根据下拉框选择的文本，设置图表的显示类型
 *          支持的类型包括：柱状图、折线图、饼图、极坐标图
 */
void MainWindow::changeType()
{
    // 获取当前选择的图表类型文本
    QString text = typeSelector->currentText();
    
    // 根据选择的文本设置图表类型
    if (text == "Widget::Bar")
        widget->setType(Widget::Bar);       // 设置为柱状图
    else if (text == "Widget::Line")
        widget->setType(Widget::Line);      // 设置为折线图
    else if (text == "Widget::Pie")
        widget->setType(Widget::Pie);       // 设置为饼图
    else if (text == "Widget::Polar")
        widget->setType(Widget::Polar);     // 设置为极坐标图
    else
        widget->setType(Widget::NoType);    // 无类型
}

/**
 * @brief 更改图表间距的槽函数
 * @param leading 新的间距值
 * @details 设置图表的全局间距，影响图表元素之间的空白区域
 *          这里将上下左右间距设置为相同的值
 */
void MainWindow::changeLeading(int leading)
{
    // 设置图表的全局间距（上、右、下、左）
    widget->setGlobalLeading(leading, leading, leading, leading);
}

/**
 * @brief 添加数据集的槽函数
 * @details 从行编辑框中获取用户输入的数据，格式为分号分隔的数字
 *          验证数据数量是否与模型匹配，若匹配则添加新数据集
 */
void MainWindow::addDataset()
{
    // 从行编辑框中获取文本并按分号分割
    const QStringList parts = lineAddDataset->text().split(';');
    bool ok;
    QVector<qreal> vec;
    
    // 将分割后的字符串转换为实数并添加到向量中
    for (const QString &str : parts) {
        const qreal val = str.toDouble(&ok);
        if (ok)
            vec.append(val);
    }
    
    // 获取数据模型的行数
    const int rows = widget->diagram()->model()->rowCount();
    
    // 检查数据数量是否匹配
    if (vec.count() != rows) {
        // 数量不匹配时显示警告消息
        QMessageBox::warning(this, "输入的值数量错误!",
                             QString("您输入了 %1 个值，<br>但数据模型需要 %2 个值。"<br><br>注意: 使用 <b>;</b> 分隔值!")
                                 .arg(vec.count(), rows));
    } else {
        // 数量匹配时添加新数据集
        widget->setDataset(datasetCount++, vec, "用户数据");
    }
}

// TODO: Qt5.15.2升级 验证KDChart::Widget::setType在Qt5.15.2中的API变化
// TODO: Qt5.15.2升级 检查KDChart::Widget::setGlobalLeading在Qt5.15.2中的行为
// TODO: Qt5.15.2升级 确认QMessageBox::warning在Qt5.15.2中的参数要求
// TODO: C++17升级 考虑使用std::vector替代QVector以提高性能
// TODO: C++17升级 可使用结构化绑定优化循环变量声明
// TODO: C++17升级 使用std::optional处理可能的转换失败
