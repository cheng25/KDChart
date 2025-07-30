/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

// 中文注释：本文件定义了BigDataset应用程序的自定义数据模型类Model
// 该类继承自QAbstractItemModel，用于为KD Chart库提供大数据集的数据支持
// 支持多种函数类型的数据生成（如正弦函数、三角波函数、方波函数等）
// 可以动态添加数据点，也可以通过定时器自动添加数据点，用于展示动态数据更新
// 优化了大数据集的处理性能，展示KD Chart库的高效渲染能力

#ifndef MODEL_H
#define MODEL_H

// 抽象项模型基类头文件
// TODO: Qt5.15.2升级 检查QAbstractItemModel相关API是否有变更
#include <QAbstractItemModel>
// 定时器类头文件
// TODO: Qt5.15.2升级 检查QTimer相关API是否有变更
#include <QTimer>
// 动态数组类头文件
// TODO: Qt5.15.2升级 检查QVector相关API是否有变更
#include <QVector>

/**
 * @brief 自定义数据模型类
 * @details 继承自QAbstractItemModel，用于生成和提供各种函数类型的数据
 * 支持动态添加数据点，并可以通过定时器自动添加数据
 * 主要用于展示大数据集的图表绘制性能
 * 包含x和y两列数据，其中x为自变量，y为函数值
 * TODO: C++17升级 考虑使用std::vector替代QVector以支持更多C++17特性
 * TODO: C++17升级 考虑使用std::optional处理可能的空数据
 */
class Model : public QAbstractItemModel
{
    Q_OBJECT
public:
    /**
     * @brief 函数类型枚举
     * @details 定义了支持的各种函数类型，用于生成不同形状的数据曲线
     */
    enum Function
    {
        SineFunction = 0,        ///< 正弦函数: y = sin(x)，平滑的周期性曲线
        TriangleFunction,        ///< 三角波函数: 周期性线性变化，形成三角形波形
        SquareFunction,          ///< 方波函数: 周期性跳变，在高电平和低电平之间切换
        NoiseFunction,           ///< 噪声函数: 随机值，无规律变化
        OneDivSineFunction,      ///< 1/sin函数: y = 1/sin(x)，在sin(x)=0处有垂直渐近线
        SineOneDivFunction       ///< sin(1/x)函数: y = sin(1/x)，在x=0附近有高频震荡
    };

    /**
     * @brief 构造函数
     * @details 初始化数据模型，设置默认函数类型为正弦函数
     * 初始化定时器，设置单次触发模式
     * 初始化数据存储向量
     * TODO: Qt5.15.2升级 检查构造函数中使用的QTimer API是否有变更
     */
    Model();

    /**
     * @brief 获取列数
     * @param parent 父索引，本模型中无效
     * @return 列数，固定为2（x和y值）
     * @details 重写QAbstractItemModel的纯虚函数，返回模型的列数
     */
    int columnCount(const QModelIndex &parent) const override;

    /**
     * @brief 获取行数
     * @param parent 父索引，本模型中无效
     * @return 数据点数量
     * @details 重写QAbstractItemModel的纯虚函数，返回模型的行数（数据点数量）
     */
    int rowCount(const QModelIndex &parent) const override;

    /**
     * @brief 创建索引
     * @param row 行号
     * @param column 列号
     * @param parent 父索引，本模型中无效
     * @return 创建的索引
     * @details 重写QAbstractItemModel的纯虚函数，为指定的行和列创建索引
     */
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    /**
     * @brief 获取父索引
     * @param index 当前索引
     * @return 父索引，本模型中始终返回无效索引
     * @details 重写QAbstractItemModel的纯虚函数，返回指定索引的父索引
     * 由于本模型是扁平结构，没有父项，因此始终返回无效索引
     */
    QModelIndex parent(const QModelIndex &index) const override;

    /**
     * @brief 获取数据
     * @param index 索引
     * @param role 数据角色
     * @return 数据值
     * @details 重写QAbstractItemModel的纯虚函数，为指定索引和角色返回对应的数据
     * 对于DisplayRole，返回x或y值
     * 对于其他角色，返回无效的QVariant
     * TODO: C++17升级 考虑使用std::optional优化返回值
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief 设置函数类型
     * @param f 函数类型
     * @details 更改当前的数据生成函数类型
     * 重置数据并通知视图更新
     * TODO: Qt5.15.2升级 检查模型数据更新机制是否有变更
     */
    void setFunction(Function f);

    /**
     * @brief 追加数据点
     * @param numPoints 要追加的数据点数量
     * @details 向数据模型中添加指定数量的新数据点
     * 通知视图数据已变更
     * 使用nextFunctionValue()计算每个新数据点的y值
     * TODO: C++17升级 考虑使用std::generate_n填充数据
     * TODO: C++17升级 考虑使用范围for循环优化数据添加
     */
    void appendPoints(int numPoints);

public slots:
    /**
     * @brief 设置是否运行
     * @param running 是否运行
     * @details 控制定时器是否启动，从而控制是否自动添加数据点
     * 当running为true时，启动定时器，每100ms添加一个数据点
     * 当running为false时，停止定时器
     * TODO: Qt5.15.2升级 检查QTimer::start/stop API是否有变更
     */
    void setRunning(bool);

private slots:
    /**
     * @brief 追加一个数据点
     * @details 定时器触发的槽函数，每次添加一个数据点
     * 调用appendPoints(1)实现
     */
    void appendPoint();

private:
    /**
     * @brief 计算下一个函数值
     * @return 下一个函数值
     * @details 根据当前函数类型和x值计算下一个函数值
     * 更新x值为下一个位置
     * TODO: C++17升级 考虑使用constexpr优化函数计算
     */
    qreal nextFunctionValue();

    qreal m_x = 0;               ///< 当前x值，用于计算函数值，每次调用nextFunctionValue后递增
    Function m_function = SineFunction;  ///< 当前函数类型，默认为正弦函数
    QVector<qreal> m_data;       ///< 存储数据的向量，每个元素为一个y值，与x值一一对应
    QTimer m_appendTimer;        ///< 用于定时添加数据点的定时器，默认为100ms间隔
};

#endif
