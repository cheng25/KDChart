/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件演示了如何使用KD Chart库创建基于时间轴的图表应用
// 展示了如何使用时间戳-值数据点和代理模型来定义绘制的测量数据"窗口"
// 应用包含一个动画按钮，可以切换数据的动态更新

// Qt应用程序类头文件
// TODO: Qt5.15.2升级 检查QApplication API是否有变更
#include <QApplication>
// Qt按钮类头文件
// TODO: Qt5.15.2升级 检查QPushButton API是否有变更
#include <QPushButton>
// Qt分割器类头文件
// TODO: Qt5.15.2升级 检查QSplitter API是否有变更
#include <QSplitter>
// Qt标准数据模型头文件
// TODO: Qt5.15.2升级 检查QStandardItemModel API是否有变更
#include <QStandardItemModel>
// Qt计时器类头文件
// TODO: Qt5.15.2升级 检查QTimer API是否有变更
#include <QTimer>
// Qt树视图类头文件
// TODO: Qt5.15.2升级 检查QTreeView API是否有变更
#include <QTreeView>
// Qt窗口部件类头文件
// TODO: Qt5.15.2升级 检查QWidget API是否有变更
#include <QWidget>

// 数学函数头文件
// TODO: C++17升级 考虑使用std::cmath替代cmath
#include <math.h>

// 图表组件头文件
// TODO: Qt5.15.2升级 检查Chart API是否有变更
#include <KDChartChart>
// 绘图器图表头文件
// TODO: Qt5.15.2升级 检查Plotter API是否有变更
#include <KDChartPlotter>

// 自定义时间轴头文件
#include "timeaxis.h"
// 自定义时间图表模型头文件
#include "timechartmodel.h"

/**
 * @brief 图表窗口部件类
 * @details 封装了KD Chart图表和相关UI组件，实现基于时间轴的数据可视化
 *          包含一个动画按钮、树视图和图表显示区域
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * @details 初始化UI组件，创建图表和数据模型，设置信号槽连接
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 创建水平分割器
        auto *splitter = new QSplitter(this);
        // 创建水平布局
        auto *l = new QHBoxLayout(this);
        setLayout(l);
        l->addWidget(splitter);

        // 创建左侧窗口部件
        auto *leftWidget = new QWidget(splitter);
        auto *leftLayout = new QVBoxLayout(leftWidget);
        leftWidget->setLayout(leftLayout);

        // 创建动画按钮
        auto *button = new QPushButton("Animate", leftWidget);
        leftLayout->addWidget(button);
        button->setCheckable(true);
        // 连接按钮状态切换信号到槽函数
        // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
        connect(button, &QPushButton::toggled, this, &ChartWidget::buttonToggled);

        // 创建树视图
        auto *tv = new QTreeView(leftWidget);
        leftLayout->addWidget(tv);

        // 创建图表
        m_chart = new KDChart::Chart(splitter);

        // 创建数据模型 (365行，2列)
        m_model = new QStandardItemModel(365, 2, this);
        // 填充数据模型 (2010年全年每天的数据)
        // TODO: C++17升级 考虑使用结构化绑定简化循环
        for (int i = 0; i < 365; ++i) {
            const QDateTime dt = QDateTime(QDate(2010, 1, 1), QTime());
            m_model->setData(m_model->index(i, 0), dt.addDays(i));  // 设置时间戳
            m_model->setData(m_model->index(i, 1), sin(i / 10.0) * 10);  // 设置Y值 (正弦函数)
        }

        // 创建时间图表代理模型
        auto *proxy = new TimeChartModel(this);
        proxy->setSourceModel(m_model);
        // 设置可见范围 (2010年2月1日 至 2010年3月31日)
        proxy->setVisibleRange(QDateTime(QDate(2010, 2, 1), QTime()),
                               QDateTime(QDate(2010, 3, 31), QTime()));

        // 创建绘图器
        auto *plotter = new KDChart::Plotter;
        m_chart->coordinatePlane()->replaceDiagram(plotter);

        // 设置树视图的数据模型
        tv->setModel(proxy);
        tv->show();

        // 创建时间轴
        auto *axis = new TimeAxis(plotter);
        axis->setPosition(TimeAxis::Bottom);  // 设置时间轴在底部
        plotter->addAxis(axis);

        // 设置绘图器的数据模型
        plotter->setModel(proxy);

        // 获取笛卡尔坐标平面
        auto coordinatePlane = qobject_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane());
        // 连接代理模型的行插入信号到坐标平面的调整范围槽函数
        // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
        connect(proxy, &TimeChartModel::rowsInserted,
                coordinatePlane, &KDChart::CartesianCoordinatePlane::adjustRangesToData);
        // 连接代理模型的行移除信号到坐标平面的调整范围槽函数
        // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
        connect(proxy, &TimeChartModel::rowsRemoved,
                coordinatePlane, &KDChart::CartesianCoordinatePlane::adjustRangesToData);

        // 更新可见范围 (2010年3月15日 至 2010年5月18日)
        proxy->setVisibleRange(QDateTime(QDate(2010, 3, 15), QTime()),
                               QDateTime(QDate(2010, 5, 18), QTime()));
        // 调整坐标范围以适应数据
        coordinatePlane->adjustRangesToData();

        // 创建计时器
        m_timer = new QTimer(this);
        // 连接计时器超时信号到槽函数
        // TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
        connect(m_timer, &QTimer::timeout, this, &ChartWidget::slotTimeout);
    }
private slots:
    /**
     * @brief 计时器超时槽函数
     * @details 更新数据模型中的数据，实现动画效果
     *          使用了一个技巧来阻止QAbstractItemModel为每次setData调用发出dataChanged信号
     *          而是收集所有更改的索引，最后一次发出信号，以提高性能
     */
    void slotTimeout()
    {
        // 阻止模型发出信号
        m_model->blockSignals(true);
        QModelIndexList indexes;

        QVariant v1, v2;
        // 更新所有数据点
        // TODO: C++17升级 考虑使用std::generate_n替代循环
        for (int i = 0; i < 365; ++i) {
            QModelIndex idx = m_model->index(i, 1);
            indexes.append(idx);
            // 更新Y值 (随时间变化的正弦函数)
            // TODO: C++17升级 使用std::sin替代sin
            m_model->setData(idx, sin(i / 10.0 + m_counter) * 10);
        }

        // 取消阻止信号
        m_model->blockSignals(false);
        // 如果有更改的索引，手动发出dataChanged信号
        if (!indexes.isEmpty()) {
            m_model->metaObject()->invokeMethod(m_model, "dataChanged", Qt::DirectConnection, Q_ARG(QModelIndex, indexes.first()), Q_ARG(QModelIndex, indexes.last()));
        }

        // 增加计数器，用于下一次更新
        m_counter += 0.02;
    }

    /**
     * @brief 按钮状态切换槽函数
     * @param checked 按钮是否被选中
     * @details 根据按钮状态启动或停止计时器
     */
    void buttonToggled(bool checked)
    {
        if (checked)
            m_timer->start(200);  // 启动计时器，每200毫秒更新一次
        else
            m_timer->stop();  // 停止计时器
    }

private:
    KDChart::Chart *m_chart;          // 图表对象
    QStandardItemModel *m_model;      // 数据模型
    QTimer *m_timer;                  // 计时器
    qreal m_counter = 0;              // 计数器，用于动画效果
};

/**
 * @brief 程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出码
 * @details 初始化Qt应用程序，创建ChartWidget实例并显示
 * @todo C++17升级 考虑使用std::optional处理可能的初始化错误
 */
int main(int argc, char *argv[])
{
    // TODO: Qt5.15.2升级 验证QApplication构造函数参数要求是否有变更
    QApplication app(argc, argv);

    ChartWidget w;
    w.show();

    // TODO: Qt5.15.2升级 检查QApplication::exec()返回值处理是否需要变更
    return app.exec();
}

#include "main.moc"
