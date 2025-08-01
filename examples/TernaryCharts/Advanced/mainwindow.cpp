/****************************************************************************
**
** 此文件是KD Chart库的示例程序的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可协议: MIT
**
****************************************************************************/

// 包含Qt标签类的头文件
#include <QLabel>
// 包含Qt字符串类的头文件
#include <QString>
// 包含Qt文本流类的头文件
#include <QTextStream>

// 包含KDChart图表类的头文件
#include <KDChartChart>
// 包含KDChart三元坐标轴类的头文件
#include <KDChartTernaryAxis>
// 包含KDChart三元坐标平面类的头文件
#include <KDChartTernaryCoordinatePlane>
// 包含KDChart三元线图表类的头文件
#include <KDChartTernaryLineDiagram>
// 包含KDChart三元点图表类的头文件
#include <KDChartTernaryPointDiagram>

// 包含主窗口头文件
#include "mainwindow.h"

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口指针
 * 初始化图表对象、三元坐标平面和数据模型，并设置UI和信号槽连接
 */
MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    // 设置UI
    setupUi(this);
    // 创建图表对象
    m_chart = new KDChart::Chart;
    // 用三元坐标平面替换默认的笛卡尔坐标平面
    m_ternaryPlane = new KDChart::TernaryCoordinatePlane;
    m_chart->replaceCoordinatePlane(m_ternaryPlane);
    // 创建三元点图表对象
    m_diagram = new KDChart::TernaryPointDiagram;
    // 用三元点图表替换默认图表
    m_ternaryPlane->replaceDiagram(m_diagram);

    // 添加三个三元坐标轴，并设置位置
    auto *axisA = new KDChart::TernaryAxis(m_diagram);
    axisA->setPosition(KDChartEnums::PositionSouth); // 南位置
    auto *axisB = new KDChart::TernaryAxis(m_diagram);
    axisB->setPosition(KDChartEnums::PositionWest);  // 西位置
    auto *axisC = new KDChart::TernaryAxis(m_diagram);
    axisC->setPosition(KDChartEnums::PositionEast);  // 东位置

    // 创建图表布局
    auto *chartLayout = new QHBoxLayout(frame);
    // 添加图表到布局
    chartLayout->addWidget(m_chart);

    // 设置数据模型
    setupModel();
    // 设置图表的数据模型
    m_diagram->setModel(&m_model);
    // 连接图表的点击信号到indexClicked槽函数
    connect(m_diagram, &KDChart::TernaryPointDiagram::clicked, 
            this, &MainWindow::indexClicked);
}

/**
 * @brief 设置数据模型
 * 初始化模型并填充测试数据，生成5行3列的测试数据
 */
void MainWindow::setupModel()
{
    // 插入5行
    m_model.insertRows(0, 5);
    // 插入3列
    m_model.insertColumns(0, 3);

    // 初始每列的份额均等
    const float column0Share = 1.0f / 3.0f;
    const float column1Share = 1.0f / 3.0f;
    const float column2Share = 1.0f / 3.0f;

    // 为每行生成数据
    for (int row = 0; row < m_model.rowCount(); ++row) {
        // 计算X轴偏移量
        const qreal SkewX = column0Share * (1 - 1.0 / (5 * row * row * row + 1));
        // Y轴偏移量等于X轴偏移量
        const qreal SkewY = SkewX;
        QModelIndex index;

        // 设置第一列数据
        index = m_model.index(row, 0);
        m_model.setData(index, QVariant(column0Share - SkewX));
        // 设置第二列数据
        index = m_model.index(row, 1);
        m_model.setData(index, QVariant(column1Share + SkewX - SkewY));
        // 设置第三列数据
        index = m_model.index(row, 2);
        m_model.setData(index, QVariant(column2Share + SkewY));
    }
}

/**
 * @brief 索引点击槽函数
 * @param index 被点击的模型索引
 * 处理图表上点的点击事件，显示点击的行列信息
 */
void MainWindow::indexClicked(const QModelIndex &index)
{
    // 输出调试信息
    qDebug() << "MainWindow::indexClicked:" << index;
    // 检查索引是否有效
    if (index.isValid()) {
        // 创建字符串
        QString text;
        QTextStream stream(&text);
        // 格式化行列信息
        stream << "Row: " << index.row() << " - Column: " << index.column();
        // 在标签上显示信息
        label->setText(text);
    }
}

// TODO: Qt5.15.2升级 检查KDChart::TernaryPointDiagram API是否有变更
// TODO: Qt5.15.2升级 检查KDChart::TernaryCoordinatePlane API是否有变更
// TODO: C++17升级 考虑使用std::make_unique管理动态内存
// TODO: C++17升级 考虑使用结构化绑定优化数据访问
// TODO: C++17升级 考虑使用if constexpr优化条件判断
