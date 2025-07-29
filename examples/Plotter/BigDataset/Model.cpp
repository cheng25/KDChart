/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件实现了BigDataset应用程序的Model类的所有方法
// 该类继承自QAbstractItemModel，用于为KD Chart库提供大数据集的数据支持
// 支持多种函数类型的数据生成（正弦函数、三角波函数、方波函数等）
// 可以动态添加数据点，也可以通过定时器自动添加数据点
// 优化了大数据集的处理性能，展示KD Chart库的高效渲染能力
// TODO: C++17升级 使用std::optional优化可能的空值返回
// TODO: C++17升级 考虑使用std::chrono替代QElapsedTimer
// TODO: Qt5.15.2升级 检查QAbstractItemModel相关API是否有变更

#include "Model.h"

#include <QDebug>            // 调试输出
#include <QElapsedTimer>     // 计时器
#include <QRandomGenerator>  // 随机数生成器
// TODO: Qt5.15.2升级 检查QRandomGenerator API是否有变更

#define _USE_MATH_DEFINES    // 启用数学常量定义
#include <cmath>             // 数学函数
// MSVC兼容性处理
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 步长宽度常量，控制x值的增量
static const qreal s_stepWidth = 0.1;

/**
 * @brief 构造函数
 * @details 初始化Model类的成员变量，设置定时器间隔，连接信号槽
 * 并预填充100个数据点
 * @note 定时器间隔设置为3毫秒，这意味着每秒大约添加333个数据点
 * @see appendPoints
 * @see setRunning
 * TODO: Qt5.15.2升级 检查QTimer::setInterval API是否有变更
 * TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
 */
Model::Model()
{
    // 设置定时器间隔为3毫秒
    m_appendTimer.setInterval(3);
    // 连接定时器的timeout信号到appendPoint槽函数
    connect(&m_appendTimer, &QTimer::timeout, this, &Model::appendPoint);
    // 预填充100个数据点，初始化图表显示
    appendPoints(100);
}

/**
 * @brief 获取列数
 * @param parent 父索引，本模型中无效
 * @return 列数，固定为2
 * @details 重写QAbstractItemModel的方法，返回模型的列数
 * 本模型固定返回2列：第0列是x值，第1列是y值
 * @note 由于模型是扁平结构，parent参数总是无效的
 */
int Model::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);  // 未使用的参数
    return 2;  // 第0列：x值，第1列：y值
}

/**
 * @brief 获取行数
 * @param parent 父索引，本模型中无效
 * @return 行数，即数据点数量
 * @details 重写QAbstractItemModel的方法，返回模型的行数
 * 行数等于数据向量中存储的数据点数量
 * @note 由于模型是扁平结构，parent参数总是无效的
 */
int Model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);  // 未使用的参数
    return m_data.count();  // 返回数据向量的大小
}

/**
 * @brief 创建索引
 * @param row 行号
 * @param column 列号
 * @param parent 父索引，本模型中无效
 * @return 创建的索引，如果参数无效则返回无效索引
 * @details 重写QAbstractItemModel的方法，为指定的行和列创建索引
 * 本模型是扁平结构，没有父项，所以parent参数必须无效
 * 列号必须小于2，行号必须小于数据点数量
 * @note 索引创建使用createIndex方法
 * TODO: Qt5.15.2升级 检查QModelIndex API是否有变更
 */
QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    // 检查列是否有效（必须小于2）
    // 检查行是否有效（必须小于数据点数量）
    // 检查父索引是否有效（本模型没有父节点）
    if (column > 1 || row >= m_data.count() || parent.isValid()) {
        return QModelIndex();  // 返回无效索引
    }
    return createIndex(row, column);  // 创建并返回有效索引
}

/**
 * @brief 获取父索引
 * @param index 当前索引
 * @return 父索引，始终返回无效索引
 * @details 重写QAbstractItemModel的方法，返回指定索引的父索引
 * 由于本模型是扁平结构，没有父项，因此始终返回无效索引
 * @note 无论输入的index参数如何，总是返回无效索引
 */
QModelIndex Model::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);  // 未使用的参数
    return QModelIndex();  // 返回无效索引
}

/**
 * @brief 获取数据
 * @param index 索引
 * @param role 数据角色
 * @return 数据值，如果参数无效则返回无效QVariant
 * @details 重写QAbstractItemModel的方法，根据索引和角色返回对应的数据值
 * 对于DisplayRole，第0列返回x值，第1列返回y值
 * 对于其他角色或无效索引，返回无效QVariant
 * @note x值的计算方式为行号乘以步长宽度s_stepWidth
 * @see s_stepWidth
 * @see rowCount
 * TODO: C++17升级 考虑使用std::optional优化返回值
 * TODO: Qt5.15.2升级 检查QModelIndex API是否有变更
 */
QVariant Model::data(const QModelIndex &index, int role) const
{
    // 检查角色是否为DisplayRole
    // 检查父索引是否无效
    // 检查列是否有效
    // 检查行是否有效
    if (role != Qt::DisplayRole || index.parent().isValid() || index.column() > 1 || index.row() >= m_data.count()) {
        return QVariant();  // 返回无效值
    }
    if (index.column() == 0) {
        // 第0列返回x值，计算方式为行号乘以步长
        return index.row() * s_stepWidth;
    } else {
        // 第1列返回y值，从数据向量中获取
        return m_data.at(index.row());
    }
}

/**
 * @brief 设置函数类型
 * @param f 函数类型
 * @details 设置当前使用的函数类型，用于生成数据
 * 切换函数类型后，新添加的数据点将使用该函数类型生成
 * @note 函数类型定义在Model::Function枚举中
 * @see Function
 * @see nextFunctionValue
 */
void Model::setFunction(Function f)
{
    m_function = f;
}

/**
 * @brief 设置是否运行
 * @param running 是否运行
 * @details 控制定时器是否启动，从而控制是否自动添加数据点
 * 当running为true时，启动定时器，每3毫秒添加一个数据点
 * 当running为false时，停止定时器
 * @note 定时器间隔在构造函数中设置为3毫秒
 * @see appendPoint
 * @see appendPoints
 * TODO: Qt5.15.2升级 检查QTimer::start/stop API是否有变更
 */
void Model::setRunning(bool running)
{
    if (running) {
        m_appendTimer.start();  // 启动定时器
    } else {
        m_appendTimer.stop();   // 停止定时器
    }
}

/**
 * @brief 追加一个数据点
 * @details 定时器触发的槽函数，每次添加一个数据点
 * 内部调用appendPoints(1)实现
 * @see appendPoints
 * @see setRunning
 */
void Model::appendPoint()
{
    appendPoints(1);  // 调用appendPoints方法添加一个数据点
}

/**
 * @brief 追加多个数据点
 * @param numPoints 要追加的数据点数量
 * @details 向数据模型中添加指定数量的数据点，并通知视图更新
 * 使用nextFunctionValue()生成每个新数据点的y值
 * 记录添加数据点所需的时间，用于性能分析
 * @note 该方法会触发数据模型的beginInsertRows和endInsertRows信号
 * @see nextFunctionValue
 * @see data
 * TODO: C++17升级 考虑使用std::generate_n和std::back_inserter优化数据添加
 * TODO: C++17升级 考虑使用结构化绑定优化调试输出
 * TODO: Qt5.15.2升级 检查QAbstractItemModel::beginInsertRows/endInsertRows API是否有变更
 */
void Model::appendPoints(int numPoints)
{
    Q_ASSERT(numPoints >= 1);  // 确保至少添加一个数据点

    // 通知模型即将插入数据
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count() + numPoints - 1);

    // 添加数据点
    for (int i = 0; i < numPoints; i++) {
        m_data.append(nextFunctionValue());  // 调用nextFunctionValue生成下一个数据点
    }

    // 计时并结束插入操作
    QElapsedTimer stopWatch;
    stopWatch.start();
    endInsertRows();  // 通知模型数据插入完成
    // 输出添加数据点所需的时间，用于性能分析
    qDebug() << "Adding" << numPoints << "data points to the existing" << m_data.count() - numPoints
             << "took" << stopWatch.elapsed() << "milliseconds";
}

/**
 * @brief 计算下一个函数值
 * @return 下一个函数值
 * @details 根据当前函数类型和x值计算下一个函数值，并更新x值
 * 支持多种函数类型：正弦函数、三角波函数、方波函数、噪声函数等
 * @note 函数类型由m_function成员变量控制
 * @see setFunction
 * @see Function
 * TODO: C++17升级 考虑使用constexpr优化数学计算
 * TODO: C++17升级 考虑使用std::optional处理可能的除以零情况
 * TODO: C++17升级 使用std::uniform_real_distribution替代自定义随机数生成
 */
qreal Model::nextFunctionValue()
{
    qreal fx = 0.0;  // 函数值

    // 根据函数类型计算函数值
    switch (m_function) {
    case SineFunction:
        fx = sin(m_x);  // 正弦函数: y = sin(x)
        break;
    case TriangleFunction: {
        // 三角波函数: 在0到π区间从-1线性上升到1，在π到2π区间从1线性下降到-1
        qreal x = fmod(m_x + 0.5 * M_PI, 2 * M_PI);  // 调整相位，使其看起来像正弦波
        if (x < M_PI) {
            fx = -1.0 + x * (2.0 / M_PI);  // 上升段
        } else {
            fx = 3.0 - x * (2.0 / M_PI);  // 下降段
        }
        break;
    }
    case SquareFunction: {
        // 方波函数: 在0到π区间为1，在π到2π区间为-1
        qreal x = fmod(m_x, 2 * M_PI);  // 取模，使x在0到2π之间
        fx = x < M_PI ? 1 : -1;  // 0到π为1，π到2π为-1
        break;
    }
    case NoiseFunction:
        // 噪声函数: 生成-1到1之间的随机数
        // TODO: C++17升级 使用std::uniform_real_distribution替代自定义随机数生成
        fx = -1.0 + qreal(QRandomGenerator::global()->generate()) * 2.0 / qreal(RAND_MAX);
        break;
    case SineOneDivFunction: {
        // sin(1/x)函数: 随着x接近0，震荡频率增加
        // 调整x值范围，使其重复并包含负值
        qreal x = fmod(m_x + 10, 20) - 10;
        if (qAbs(x) < 1e-6) {  // 避免除以零
            break;
        }
        fx = sin(1.0 / x);  // 计算sin(1/x)
        break;
    }
    case OneDivSineFunction: {
        // 1/sin函数: 当sin(x)接近0时，函数值趋向于无穷大
        qreal s = sin(m_x);  // 计算sin(x)
        fx = qAbs(s) > 1e-6 ? 1.0 / s : 0.0;  // 避免除以零
        break;
    }
    default:
        Q_ASSERT(false);  // 不应该到达这里
    }

    // 更新x值，准备下一次计算
    m_x += s_stepWidth;
    return fx;  // 返回计算得到的函数值
}
