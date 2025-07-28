/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
**
** 许可证：MIT
**
****************************************************************************/

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>  // 包含Qt抽象项模型头文件

/**
 * @brief 项目数据模型类
 * 
 * 继承自QAbstractItemModel，实现了一个树形数据模型，用于存储和管理项目数据
 */
class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父对象指针
     */
    explicit ProjectModel(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ProjectModel() override;

    /**
     * @brief 获取指定索引的行数
     * 
     * @param idx 父索引
     * @return 行数
     */
    /*reimp*/ int rowCount(const QModelIndex &idx) const override;

    /**
     * @brief 获取指定索引的列数
     * 
     * @param idx 父索引
     * @return 列数
     */
    /*reimp*/ int columnCount(const QModelIndex &idx) const override;

    /**
     * @brief 创建模型索引
     * 
     * @param row 行号
     * @param col 列号
     * @param parent 父索引
     * @return 创建的模型索引
     */
    /*reimp*/ QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 获取父索引
     * 
     * @param idx 子索引
     * @return 父索引
     */
    /*reimp*/ QModelIndex parent(const QModelIndex &idx) const override;

    /**
     * @brief 获取表头数据
     * 
     * @param section 章节
     * @param orientation 方向
     * @param role 角色
     * @return 表头数据
     */
    /*reimp*/ QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief 获取指定索引的数据
     * 
     * @param idx 模型索引
     * @param role 角色
     * @return 数据
     */
    /*reimp*/ QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;

    /**
     * @brief 设置指定索引的数据
     * 
     * @param idx 模型索引
     * @param value 要设置的值
     * @param role 角色
     * @return 是否设置成功
     */
    /*reimp*/ bool setData(const QModelIndex &idx, const QVariant &value,
                           int role = Qt::DisplayRole) override;

    /**
     * @brief 插入行
     * 
     * @param row 起始行号
     * @param count 行数
     * @param parent 父索引
     * @return 是否插入成功
     */
    /*reimp*/ bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    /**
     * @brief 获取项目标志
     * 
     * @param idx 模型索引
     * @return 项目标志
     */
    /*reimp*/ Qt::ItemFlags flags(const QModelIndex &) const override;

    /**
     * @brief 从文件加载数据
     * 
     * @param filename 文件名
     * @return 是否加载成功
     */
    bool load(const QString &filename);

    /**
     * @brief 保存数据到文件
     * 
     * @param filename 文件名
     * @return 是否保存成功
     */
    bool save(const QString &filename);

private:
    class Node;  // 节点类前向声明

    Node *m_root;  // 根节点指针
};

#endif /* PROJECTMODEL_H */
