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

#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartDatasetProxyModel>
#include <KDChartHeaderFooter>
#include <KDChartLineDiagram>
#include <KDChartPosition>
#include <KDChartTextAttributes>
#include <QComboBox>
#include <QLineEdit>
#include <QPen>

/**
 * @brief 页眉项类
 * 
 * 继承自QTreeWidgetItem，用于在树形视图中表示页眉/页脚项，
 * 存储对应的KDChart::HeaderFooter指针。
 */
class HeaderItem : public QTreeWidgetItem
{
public:
    /**
     * @brief 构造函数
     * @param header 页眉/页脚指针
     * @param parent 父树形视图
     */
    HeaderItem(KDChart::HeaderFooter *header, QTreeWidget *parent)
        : QTreeWidgetItem(parent)
        , m_header(header)
    {}

    /**
     * @brief 获取页眉/页脚指针
     * @return 页眉/页脚指针
     */
    KDChart::HeaderFooter *header() const
    {
        return m_header;
    }

private:
    KDChart::HeaderFooter *m_header; // 页眉/页脚指针
};

/**
 * @brief MainWindow构造函数
 * @param parent 父窗口部件
 * 
 * 初始化UI，创建图表，加载数据模型，并设置线图。
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
    m_lines->setModel(&m_model);
    m_chart->coordinatePlane()->replaceDiagram(m_lines);

    m_chart->update(); // 更新图表
}

/**
 * @brief 设置添加页眉对话框
 * @param dlg 对话框指针
 * @param conf 对话框UI配置
 * 
 * 配置添加页眉对话框的UI元素，设置位置下拉框的选项。
 */
void MainWindow::setupAddHeaderDialog(QDialog *dlg,
                                      Ui::AddHeaderDialog &conf) const
{
    conf.setupUi(dlg);
    conf.textED->setFocus(); // 设置文本编辑框为焦点

    // 注意：页眉/页脚位置可以是Center，但不能是Floating
    const QStringList labels = KDChart::Position::printableNames(KDChart::Position::IncludeCenter);
    const QList<QByteArray> names = KDChart::Position::names(KDChart::Position::IncludeCenter);

    // 填充位置下拉框
    for (int i = 0, end = qMin(labels.size(), names.size()); i != end; ++i)
        conf.positionCO->addItem(labels[i], names[i]);
}

/**
 * @brief 添加页眉按钮点击事件处理
 * 
 * 弹出添加页眉对话框，根据用户输入创建新的页眉/页脚，并添加到图表和树形视图中。
 */
void MainWindow::on_addHeaderPB_clicked()
{
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    setupAddHeaderDialog(&dlg, conf);
    conf.typeCO->setCurrentIndex(0); // 默认选择"页眉"
    conf.positionCO->setCurrentIndex(0);
    if (dlg.exec()) {
        auto *headerFooter = new KDChart::HeaderFooter(m_chart);
        m_chart->addHeaderFooter(headerFooter);
        headerFooter->setText(conf.textED->text());
        KDChart::TextAttributes attrs(headerFooter->textAttributes());
        attrs.setPen(QPen(Qt::red)); // 设置文本颜色为红色
        headerFooter->setTextAttributes(attrs);
        // 设置类型（页眉或页脚）
        headerFooter->setType(conf.typeCO->currentText() == "Header" ? KDChart::HeaderFooter::Header : KDChart::HeaderFooter::Footer);
        // 设置位置
        headerFooter->setPosition(
            KDChart::Position::fromName(conf.positionCO->itemData(
                                                           conf.positionCO->currentIndex())
                                            .toByteArray()));

        // 添加到树形视图
        auto *newItem = new HeaderItem(headerFooter, headersTV);
        newItem->setText(0, conf.textED->text());
        newItem->setText(1, headerFooter->type() == KDChart::HeaderFooter::Header ? tr("页眉") : tr("页脚"));
        newItem->setText(2, conf.positionCO->currentText());
        m_chart->update(); // 更新图表
    }
}

/**
 * @brief 编辑页眉按钮点击事件处理
 * 
 * 弹出编辑页眉对话框，根据用户输入更新选中的页眉/页脚，并更新树形视图。
 */
void MainWindow::on_editHeaderPB_clicked()
{
    if (headersTV->selectedItems().size() == 0)
        return;
    HeaderItem *item =
        static_cast<HeaderItem *>(headersTV->selectedItems().first());
    KDChart::HeaderFooter *headerFooter = item->header();
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    setupAddHeaderDialog(&dlg, conf);
    conf.textED->setText(headerFooter->text());
    // 设置当前类型
    conf.typeCO->setCurrentIndex(headerFooter->type() == KDChart::HeaderFooter::Header ? 0 : 1);
    // 设置当前位置
    conf.positionCO->setCurrentIndex(
        conf.positionCO->findText(headerFooter->position().printableName()));
    if (dlg.exec()) {
        headerFooter->setText(conf.textED->text());
        // 更新类型
        headerFooter->setType(conf.typeCO->currentText() == "Header" ? KDChart::HeaderFooter::Header : KDChart::HeaderFooter::Footer);
        // 更新位置
        headerFooter->setPosition(
            KDChart::Position::fromName(conf.positionCO->itemData(
                                                           conf.positionCO->currentIndex())
                                            .toByteArray()));
        // 更新树形视图项
        item->setText(0, conf.textED->text());
        item->setText(1, headerFooter->type() == KDChart::HeaderFooter::Header ? tr("页眉") : tr("页脚"));
        item->setText(2, conf.positionCO->currentText());
        m_chart->update(); // 更新图表
    }
}

/**
 * @brief 删除页眉按钮点击事件处理
 * 
 * 从图表和树形视图中删除选中的页眉/页脚项。
 */
void MainWindow::on_removeHeaderPB_clicked()
{
    if (headersTV->selectedItems().size() == 0)
        return;
    QList<QTreeWidgetItem *> items = headersTV->selectedItems();
    for (QList<QTreeWidgetItem *>::const_iterator it = items.begin(); it != items.end(); ++it) {
        KDChart::HeaderFooter *headerFooter = static_cast<HeaderItem *>((*it))->header();
#if 0
        // 注意：尽管页眉由图表拥有，你也可以直接删除它：
        // KD Chart会注意到这一点并调整其布局...
        delete headerFooter;
#else
        // ...但正确的方法是先获取它，使图表不再拥有它：
        m_chart->takeHeaderFooter(headerFooter);
        // ...然后删除它：
        delete headerFooter;
#endif
        delete (*it); // 删除树形视图项
    }
    m_chart->update(); // 更新图表
}

/**
 * @brief 页眉表格选择项改变事件处理
 * 
 * 根据选中项的数量启用或禁用编辑和删除按钮。
 */
void MainWindow::on_headersTV_itemSelectionChanged()
{
    removeHeaderPB->setEnabled(headersTV->selectedItems().count() > 0);
    editHeaderPB->setEnabled(headersTV->selectedItems().count() == 1);
}
