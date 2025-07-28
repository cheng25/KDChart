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

#include <KDChartChart>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartPosition>
#include <KDChartTextAttributes>
#include <QComboBox>
#include <QLineEdit>

/**
 * @brief 图例项辅助类
 * 
 * 继承自QTreeWidgetItem，用于在树形控件中存储图例对象的引用。
 */
class LegendItem : public QTreeWidgetItem
{
public:
    /**
     * @brief 构造函数
     * @param legend 图例对象
     * @param parent 父树形控件
     * 
     * 创建一个图例项，并关联到指定的图例对象。
     */
    LegendItem(KDChart::Legend *legend, QTreeWidget *parent)
        : QTreeWidgetItem(parent)
        , m_legend(legend)
    {}

    /**
     * @brief 获取关联的图例对象
     * @return 图例对象指针
     */
    KDChart::Legend *legend() const
    {
        return m_legend;
    }

private:
    KDChart::Legend *m_legend; // 关联的图例对象
};

/**
 * @brief 主窗口构造函数
 * @param parent 父窗口部件
 * 
 * 初始化主窗口UI，创建图表、数据模型和线图，并设置初始图例。
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this); // 设置UI

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new KDChart::Chart();
    chartLayout->addWidget(m_chart);

    m_model.loadFromCSV(":/data"); // 从CSV文件加载数据

    // 设置线图
    m_lines = new KDChart::LineDiagram();
    m_lines->setModel(&m_model); // 设置数据模型
    m_chart->coordinatePlane()->replaceDiagram(m_lines); // 替换图表的线图

    // 自定义线条样式
    QPen pen = QPen(m_lines->pen(0).color(), 2);
    pen.setStyle(Qt::DashLine);
    m_lines->setPen(0, pen);
    pen = QPen(m_lines->pen(2).color(), 2);
    pen.setStyle(Qt::DashDotDotLine);
    m_lines->setPen(2, pen);

    // 添加初始图例
    auto *legend = new KDChart::Legend(m_lines, m_chart);
    legend->setPosition(KDChart::Position::North); // 设置图例位置在顶部
    legend->setAlignment(Qt::AlignCenter); // 设置对齐方式为居中
    legend->setShowLines(false); // 不显示连接线
    legend->setTitleText(tr("图例")); // 设置图例标题
    legend->setOrientation(Qt::Vertical); // 设置图例方向为垂直
    m_chart->addLegend(legend);
    legend->show(); // 显示图例

    // 在树形控件中添加图例项
    auto *newItem = new LegendItem(legend, legendsTV);
    newItem->setText(0, tr("North"));
    newItem->setText(1, tr("no"));
    newItem->setText(2, tr("图例"));
    newItem->setText(3, tr("Vertical"));
    newItem->setText(4, tr("Center"));
    newItem->setText(5, tr("MarkersOnly"));

    // 初始化对齐方式映射表
    alignmentMap[Qt::AlignTop | Qt::AlignLeft] = tr("Top + Left");
    alignmentMap[Qt::AlignTop | Qt::AlignHCenter] = tr("Top + HCenter");
    alignmentMap[Qt::AlignTop | Qt::AlignRight] = tr("Top + Right");
    alignmentMap[Qt::AlignVCenter | Qt::AlignRight] = tr("VCenter + Right");
    alignmentMap[Qt::AlignBottom | Qt::AlignRight] = tr("Bottom + Right");
    alignmentMap[Qt::AlignBottom | Qt::AlignHCenter] = tr("Bottom + HCenter");
    alignmentMap[Qt::AlignBottom | Qt::AlignLeft] = tr("Bottom + Left");
    alignmentMap[Qt::AlignVCenter | Qt::AlignLeft] = tr("VCenter + Left");
    alignmentMap[Qt::AlignCenter] = tr("Center");

    m_chart->update(); // 更新图表
}

/**
 * @brief 初始化添加图例对话框
 * @param conf 图例配置对话框
 * @param alignment 对齐方式
 * 
 * 设置添加图例对话框的初始配置，包括位置、对齐方式等选项。
 */
void MainWindow::initAddLegendDialog(DerivedAddLegendDialog &conf,
                                     Qt::Alignment alignment) const
{
    conf.titleTextED->setFocus(); // 设置标题文本编辑框为焦点

    // 注意: 图例位置可以是浮动的，但不能是Center
    const QStringList labels = KDChart::Position::printableNames(KDChart::Position::IncludeFloating);
    const QList<QByteArray> names = KDChart::Position::names(KDChart::Position::IncludeFloating);

    // 添加位置选项
    for (int i = 0, end = qMin(labels.size(), names.size()); i != end; ++i)
        conf.positionCO->addItem(labels[i], names[i]);

    // 添加对齐方式选项
    QMap<Qt::Alignment, QString>::const_iterator it = alignmentMap.constBegin();
    while (it != alignmentMap.constEnd()) {
        conf.alignmentCO->addItem(it.value());
        ++it;
    }
    // 设置当前对齐方式
    const int idx = conf.alignmentCO->findText(alignmentMap[alignment]);
    if (idx > -1)
        conf.alignmentCO->setCurrentIndex(idx);
}

/**
 * @brief 添加图例按钮点击槽函数
 * 
 * 打开添加图例对话框，根据用户输入创建新的图例并添加到图表中。
 */
void MainWindow::on_addLegendPB_clicked()
{
    DerivedAddLegendDialog conf; // 创建添加图例对话框
    initAddLegendDialog(conf, Qt::AlignCenter); // 初始化对话框
    if (conf.exec() == QDialog::Accepted) { // 如果用户点击了确定按钮
        auto *legend = new KDChart::Legend(m_lines, m_chart); // 创建新的图例
        m_chart->addLegend(legend); // 添加图例到图表
        // 设置图例位置
        legend->setPosition(
            KDChart::Position::fromName(conf.positionCO->itemData(conf.positionCO->currentIndex()).toByteArray()));
        // 获取对齐方式
        Qt::Alignment alignment = Qt::AlignCenter;
        const QString selectedAlignment(conf.alignmentCO->currentText());
        QMap<Qt::Alignment, QString>::const_iterator i = alignmentMap.constBegin();
        while (i != alignmentMap.constEnd()) {
            if (i.value() == selectedAlignment) {
                alignment = i.key();
                break;
            }
            ++i;
        }
        legend->setAlignment(alignment); // 设置对齐方式
        legend->setShowLines(conf.showLinesCB->isChecked()); // 设置是否显示连接线
        legend->setTitleText(conf.titleTextED->text()); // 设置标题文本
        // 设置方向
        legend->setOrientation((conf.orientationCO->currentIndex() == 0) ? Qt::Vertical : Qt::Horizontal);

        // 设置图例样式
        switch (conf.styleCO->currentIndex()) {
        case 0:
            legend->setLegendStyle(KDChart::Legend::MarkersOnly);
            break;
        case 1:
            legend->setLegendStyle(KDChart::Legend::LinesOnly);
            break;
        case 2:
            legend->setLegendStyle(KDChart::Legend::MarkersAndLines);
            break;
        default:
            legend->setLegendStyle(KDChart::Legend::MarkersOnly);
            break;
        }

        // 在树形控件中添加新的图例项
        auto *newItem = new LegendItem(legend, legendsTV);
        newItem->setText(0, conf.positionCO->currentText());
        newItem->setText(1, conf.showLinesCB->isChecked() ? tr("yes") : tr("no"));
        newItem->setText(2, conf.titleTextED->text());
        newItem->setText(3, conf.orientationCO->currentText());
        newItem->setText(4, selectedAlignment);
        newItem->setText(5, conf.styleCO->currentText());
        m_chart->update(); // 更新图表
    }
}

/**
 * @brief 编辑图例按钮点击槽函数
 * 
 * 打开编辑图例对话框，根据用户输入更新选中的图例属性。
 */
void MainWindow::on_editLegendPB_clicked()
{
    if (legendsTV->selectedItems().size() == 0) // 如果没有选中项
        return;
    LegendItem *item = static_cast<LegendItem *>(legendsTV->selectedItems().first()); // 获取选中的图例项
    KDChart::Legend *legend = item->legend(); // 获取关联的图例对象
    DerivedAddLegendDialog conf; // 创建编辑图例对话框
    initAddLegendDialog(conf, legend->alignment()); // 初始化对话框
    conf.showLinesCB->setChecked(legend->showLines()); // 设置显示连接线复选框状态
    conf.titleTextED->setText(legend->titleText()); // 设置标题文本
    legend->setLegendSymbolAlignment(Qt::AlignBottom); // 设置图例符号对齐方式

    // 设置当前位置
    conf.positionCO->setCurrentIndex(
        conf.positionCO->findText(legend->position().printableName()));
    // 设置当前方向
    conf.orientationCO->setCurrentIndex((legend->orientation() == Qt::Vertical) ? 0 : 1);
    // 设置当前样式
    conf.styleCO->setCurrentIndex(legend->legendStyle());

    if (conf.exec() == QDialog::Accepted) { // 如果用户点击了确定按钮
        // 设置新的位置
        legend->setPosition(
            KDChart::Position::fromName(conf.positionCO->itemData(conf.positionCO->currentIndex()).toByteArray()));
        // 获取新的对齐方式
        Qt::Alignment alignment = Qt::AlignCenter;
        const QString selectedAlignment(conf.alignmentCO->currentText());
        QMap<Qt::Alignment, QString>::const_iterator i = alignmentMap.constBegin();
        while (i != alignmentMap.constEnd()) {
            if (i.value() == selectedAlignment) {
                alignment = i.key();
                break;
            }
            ++i;
        }
        legend->setAlignment(alignment); // 设置新的对齐方式
        legend->setShowLines(conf.showLinesCB->isChecked()); // 设置是否显示连接线
        legend->setTitleText(conf.titleTextED->text()); // 设置新的标题文本
        // 设置新的方向
        legend->setOrientation((conf.orientationCO->currentIndex() == 0) ? Qt::Vertical : Qt::Horizontal);

        // 设置新的图例样式
        switch (conf.styleCO->currentIndex()) {
        case 0:
            legend->setLegendStyle(KDChart::Legend::MarkersOnly);
            break;
        case 1:
            legend->setLegendStyle(KDChart::Legend::LinesOnly);
            break;
        case 2:
            legend->setLegendStyle(KDChart::Legend::MarkersAndLines);
            break;
        default:
            legend->setLegendStyle(KDChart::Legend::MarkersOnly);
            break;
        }

        // 更新树形控件中的图例项信息
        item->setText(0, conf.positionCO->currentText());
        item->setText(1, conf.showLinesCB->isChecked() ? tr("yes") : tr("no"));
        item->setText(2, conf.titleTextED->text());
        item->setText(3, conf.orientationCO->currentText());
        item->setText(4, selectedAlignment);
        item->setText(5, conf.styleCO->currentText());
        m_chart->update(); // 更新图表
    }
}

/**
 * @brief 删除图例按钮点击槽函数
 * 
 * 删除选中的图例对象，并从树形控件中移除对应的项。
 */
void MainWindow::on_removeLegendPB_clicked()
{
    if (legendsTV->selectedItems().size() == 0) // 如果没有选中项
        return;
    QList<QTreeWidgetItem *> items = legendsTV->selectedItems(); // 获取所有选中项
    for (QList<QTreeWidgetItem *>::const_iterator it = items.begin();
         it != items.end(); ++it) { // 遍历选中项
        KDChart::Legend *legend = static_cast<LegendItem *>((*it))->legend(); // 获取关联的图例对象
        // 正确的方法是先从图表中移除图例
        m_chart->takeLegend(legend);
        // 然后删除图例对象
        delete legend;
        delete (*it); // 删除树形控件项
    }
    m_chart->update(); // 更新图表
}

/**
 * @brief 图例列表选择项变更槽函数
 * 
 * 根据选中项的数量更新删除图例按钮的启用状态。
 */
void MainWindow::on_legendsTV_itemSelectionChanged()
{
    // 只有当选中一个项时才启用删除按钮
    removeLegendPB->setEnabled(legendsTV->selectedItems().count() == 1);
}
