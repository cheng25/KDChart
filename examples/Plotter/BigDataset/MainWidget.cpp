/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件实现了BigDataset应用程序的MainWidget类的所有方法
// 该类负责创建和管理应用程序的主窗口界面
// 包括UI控件、图表、绘图仪和数据模型的交互逻辑
// 支持函数类型切换（正弦、余弦、线性等）和动态添加大量数据点功能
// 展示了KD Chart库处理大型数据集的性能和灵活性

// 主窗口部件头文件
// TODO: Qt5.15.2升级 检查MainWidget.h中使用的Qt API是否有变更
#include "MainWidget.h"

// KD图表抽象图表私有头文件
// TODO: Qt5.15.2升级 检查KDChartAbstractDiagram_p.h是否有API变更
#include "KDChartAbstractDiagram_p.h"
// KD图表主类头文件
// TODO: Qt5.15.2升级 检查KDChartChart类API是否有变更
#include "KDChartChart.h"
// KD绘图仪类头文件
// TODO: Qt5.15.2升级 检查KDChartPlotter类API是否有变更
#include "KDChartPlotter.h"

// 水平布局头文件
// TODO: Qt5.15.2升级 检查QHBoxLayout相关API是否有变更
#include <QHBoxLayout>

// 数学函数头文件
#include <cmath>

/**
 * @brief 构造函数
 * @details 初始化MainWidget类的实例
 * 创建UI控件、图表、绘图仪，并设置它们之间的关联
 * 配置坐标轴和数据模型
 * 设置信号槽连接
 * 初始化用户界面和图表显示
 * TODO: Qt5.15.2升级 检查QWidget相关API是否有变更
 * TODO: C++17升级 考虑使用结构化绑定简化变量声明
 */
MainWidget::MainWidget()
    : m_controlsContainer(new QWidget(this))
{
    // 设置图表和UI布局

    // 创建水平布局作为主布局
    auto *topLayout = new QHBoxLayout(this);
    // 设置UI控件
    m_controls.setupUi(m_controlsContainer);
    // 添加控件容器到主布局
    topLayout->addWidget(m_controlsContainer);

    // 创建图表对象
    auto *chart = new KDChart::Chart;
    // 添加图表到主布局
    topLayout->addWidget(chart);

    // 创建绘图仪对象
    m_plotter = new KDChart::Plotter;
    // 设置绘图仪的数据模型
    m_plotter->setModel(&m_model);
    // TODO: Qt5.15.2升级 检查KDChartAbstractDiagram::Private::get方法是否有变更
    // 启用绘制时间输出，用于性能分析
    KDChart::AbstractDiagram::Private::get(m_plotter)->doDumpPaintTime = true;
    // 替换图表的坐标系为绘图仪
    chart->coordinatePlane()->replaceDiagram(m_plotter);

    // 将图表的坐标系转换为笛卡尔坐标系
    auto *cPlane = qobject_cast<KDChart::CartesianCoordinatePlane *>(chart->coordinatePlane());
    // 确保坐标系转换成功
    Q_ASSERT(cPlane);
    // 设置垂直范围为-2到2
    cPlane->setVerticalRange(QPair<qreal, qreal>(-2, 2));

    // 配置X轴
    // 创建X轴
    auto *xAxis = new KDChart::CartesianAxis(m_plotter);
    // 设置X轴位置为底部
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    // 设置X轴标题
    xAxis->setTitleText("X");
    // 添加X轴到绘图仪
    m_plotter->addAxis(xAxis);

    // 配置Y轴
    // 创建Y轴
    auto *yAxis = new KDChart::CartesianAxis(m_plotter);
    // 设置Y轴位置为左侧
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    // 设置Y轴标题
    yAxis->setTitleText("Y");
    // 添加Y轴到绘图仪
    m_plotter->addAxis(yAxis);

    // 设置信号槽连接

    // 向量中元素的索引必须与Model::Function枚举值对应
    m_functionSelector << m_controls.sineRadio << m_controls.triangleRadio << m_controls.squareRadio
                       << m_controls.noiseRadio << m_controls.oneDivSineRadio
                       << m_controls.sineOneDivRadio;
    // 遍历所有函数选择单选按钮
    for (QRadioButton *r : std::as_const(m_functionSelector)) {
        // 连接单选按钮的toggled信号到槽函数
        // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
        connect(r, &QRadioButton::toggled, this, &MainWidget::functionToggled);
    }

    // 连接运行按钮的toggled信号到模型的setRunning槽函数
    // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
    connect(m_controls.runButton, &QRadioButton::toggled,
            &m_model, &Model::setRunning);

    // 顺序很重要，索引对应添加的数据点数量
    m_addPointsButtons << m_controls.add1kButton << m_controls.add10kButton << m_controls.add100kButton;
    // 遍历所有添加数据点按钮
    for (QPushButton *b : std::as_const(m_addPointsButtons)) {
        // 连接添加数据点按钮的clicked信号到槽函数
        // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
        connect(b, &QPushButton::clicked, this, &MainWidget::addPointsButtonClicked);
    }
}

/**
 * @brief 函数类型切换槽函数
 * @param checked 是否选中
 * @details 当用户切换函数类型单选按钮时调用
 * 设置数据模型的函数类型
 * 只有当单选按钮被选中时才执行
 * 根据选中的单选按钮索引确定对应的函数类型
 * TODO: Qt5.15.2升级 检查QRadioButton::toggled信号是否有变更
 * TODO: C++17升级 考虑使用std::optional处理可能的空指针
 */
void MainWidget::functionToggled(bool checked)
{
    // 如果未选中，直接返回
    if (!checked) {
        return;
    }
    // 获取触发信号的单选按钮
    QRadioButton *senderButton = qobject_cast<QRadioButton *>(sender());
    // 获取单选按钮在向量中的索引
    int idx = m_functionSelector.indexOf(senderButton);
    // 确保索引有效
    Q_ASSERT(idx >= 0);
    // 设置数据模型的函数类型
    m_model.setFunction(static_cast<Model::Function>(idx));
}

/**
 * @brief 添加数据点按钮点击槽函数
 * @details 当用户点击添加数据点按钮时调用
 * 根据按钮索引添加对应数量的数据点
 * 按钮索引0对应添加1000个点，索引1对应添加10000个点，索引2对应添加100000个点
 * 使用pow函数计算添加的数据点数量
 * TODO: C++17升级 使用std::pow替代pow，并考虑使用constexpr优化
 * TODO: C++17升级 考虑使用std::as_const来避免不必要的拷贝
 * TODO: Qt5.15.2升级 检查QPushButton::clicked信号是否有变更
 */
void MainWidget::addPointsButtonClicked()
{
    // 获取触发信号的按钮
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    // 获取按钮在向量中的索引
    int idx = m_addPointsButtons.indexOf(senderButton);
    // 确保索引有效
    Q_ASSERT(idx >= 0);
    // 添加数据点，数量为10^(idx+3)，即索引0添加1000点，索引1添加10000点，索引2添加100000点
    // TODO: C++17升级 使用std::pow替代pow
    m_model.appendPoints(pow(qreal(10), qreal(idx + 3)));
}
