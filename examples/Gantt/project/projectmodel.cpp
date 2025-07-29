/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
**
** 许可证：MIT
**
****************************************************************************/

#include "projectmodel.h"  // 包含项目模型头文件

#include <QDateTime>  // 包含Qt日期时间类
#include <QFile>      // 包含Qt文件类
#include <QList>      // 包含Qt列表类

#include <QDebug>     // 包含Qt调试工具

#include <KDGanttGlobal>             // 包含KDGantt全局定义
#include <KDGanttStyleOptionGanttItem>  // 包含KDGantt样式选项

#include <cassert>    // 包含断言库

typedef QAbstractItemModel BASE;  // 类型别名，简化代码

/**
 * @brief ProjectModel的内部节点类
 * 
 * 用于存储项目模型中的单个节点数据，实现树形结构
 */
class ProjectModel::Node
{
public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父节点指针
     */
    explicit Node(Node *parent = nullptr);

    /**
     * @brief 析构函数
     */
    virtual ~Node();

    /**
     * @brief 添加子节点
     * 
     * @param child 子节点指针
     */
    void addChild(Node *);

    /**
     * @brief 插入子节点
     * 
     * @param i 插入位置
     * @param child 子节点指针
     */
    void insertChild(int i, Node *);

    /**
     * @brief 移除子节点
     * 
     * @param child 子节点指针
     */
    void removeChild(Node *);

    /**
     * @brief 获取父节点
     * 
     * @return 父节点指针
     */
    Node *parent() const
    {
        return m_parent;
    }

    /**
     * @brief 获取子节点数量
     * 
     * @return 子节点数量
     */
    int childCount() const
    {
        return m_children.count();
    }

    /**
     * @brief 获取子节点的索引
     * 
     * @param n 子节点指针
     * @return 索引位置
     */
    int childNumber(Node *n) const
    {
        return m_children.indexOf(n);
    }

    /**
     * @brief 获取指定索引的子节点
     * 
     * @param i 索引
     * @return 子节点指针
     */
    Node *child(int i) const
    {
        return m_children.at(i);
    }

    /**
     * @brief 设置父节点
     * 
     * @param p 父节点指针
     */
    void setParent(Node *p)
    {
        m_parent = p;
    }

    /**
     * @brief 设置开始时间
     * 
     * @param dt 日期时间
     */
    void setStart(const QDateTime &dt)
    {
        m_bStart = m_start;
        m_start = dt;
    }

    /**
     * @brief 设置结束时间
     * 
     * @param dt 日期时间
     */
    void setEnd(const QDateTime &dt)
    {
        if (m_end.isValid())
            m_bEnd = m_end;
        m_end = dt;
    }

    /**
     * @brief 设置标签
     * 
     * @param str 标签字符串
     */
    void setLabel(const QString &str)
    {
        m_label = str;
    }

    /**
     * @brief 设置类型
     * 
     * @param t 类型值
     */
    void setType(int t)
    {
        m_type = static_cast<KDGantt::ItemType>(t);
        if (!m_start.isValid())
            m_start = m_bStart;
        if (!m_end.isValid())
            m_end = m_bEnd;
    }

    /**
     * @brief 设置完成度
     * 
     * @param c 完成度(0-100)
     */
    void setCompletion(int c)
    {
        m_completion = c;
    }

    /**
     * @brief 设置位置
     * 
     * @param p 位置枚举值
     */
    void setPosition(KDGantt::StyleOptionGanttItem::Position p)
    {
        m_position = p;
    }

    /**
     * @brief 获取开始时间
     * 
     * @return 开始时间
     */
    QDateTime start() const
    {
        return m_start;
    }

    /**
     * @brief 获取结束时间
     * 
     * @return 结束时间
     */
    QDateTime end() const
    {
        return m_end;
    }

    /**
     * @brief 获取标签
     * 
     * @return 标签字符串
     */
    QString label() const
    {
        return m_label;
    }

    /**
     * @brief 获取类型
     * 
     * @return 类型枚举值
     */
    KDGantt::ItemType type() const
    {
        return m_type;
    }

    /**
     * @brief 获取完成度
     * 
     * @return 完成度
     */
    int completion() const
    {
        return m_completion;
    }

    /**
     * @brief 获取位置
     * 
     * @return 位置枚举值
     */
    KDGantt::StyleOptionGanttItem::Position position() const
    {
        return m_position;
    }

private:
    Node *m_parent;  // 父节点指针
    QList<Node *> m_children;  // 子节点列表

    KDGantt::ItemType m_type = KDGantt::TypeTask;  // 节点类型，默认为任务
    QDateTime m_start, m_end;  // 开始和结束时间
    QDateTime m_bStart, m_bEnd;  // 备份的开始和结束时间
    QString m_label;  // 节点标签
    int m_completion = -1;  // 完成度，默认为-1表示未设置
    KDGantt::StyleOptionGanttItem::Position m_position = KDGantt::StyleOptionGanttItem::Right;  // 位置，默认为右侧
};

static int unnamed_count = 0;  // 未命名任务计数器

/**
 * @brief Node类构造函数实现
 * 
 * @param parent 父节点指针
 */
ProjectModel::Node::Node(Node *parent)
    : m_parent(parent)
    , m_start(QDateTime::currentDateTime())  // 初始化开始时间为当前时间
    , m_end(QDateTime::currentDateTime().addDays(1))  // 初始化结束时间为明天
    , m_label(tr("Unnamed task %1").arg(++unnamed_count))  // 生成默认标签
{
    if (m_parent)
        m_parent->addChild(this);  // 如果有父节点，添加到父节点
}

/**
 * @brief Node类析构函数实现
 */
ProjectModel::Node::~Node()
{
    while (!m_children.isEmpty())
        delete m_children.front();  // 递归删除所有子节点
    if (m_parent)
        m_parent->removeChild(this);  // 从父节点中移除
}

/**
 * @brief 添加子节点实现
 * 
 * @param child 子节点指针
 */
void ProjectModel::Node::addChild(Node *child)
{
    child->setParent(this);  // 设置子节点的父节点
    m_children.push_back(child);  // 添加到子节点列表
}

/**
 * @brief 插入子节点实现
 * 
 * @param i 插入位置
 * @param child 子节点指针
 */
void ProjectModel::Node::insertChild(int i, Node *child)
{
    child->setParent(this);  // 设置子节点的父节点
    m_children.insert(i, child);  // 在指定位置插入子节点
}

/**
 * @brief 移除子节点实现
 * 
 * @param child 子节点指针
 */
void ProjectModel::Node::removeChild(Node *child)
{
    child->setParent(nullptr);  // 清空子节点的父节点
    m_children.removeAll(child);  // 从列表中移除子节点
}

/**
 * @brief ProjectModel类构造函数实现
 * 
 * @param parent 父对象指针
 */
ProjectModel::ProjectModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(new Node)  // 创建根节点
{
}

/**
 * @brief ProjectModel类析构函数实现
 */
ProjectModel::~ProjectModel()
{
    delete m_root;  // 删除根节点，会递归删除所有子节点
}

/**
 * @brief 从文件加载数据
 * 
 * @param filename 文件名
 * @return 是否加载成功
 */
bool ProjectModel::load(const QString &filename)
{
    Q_UNUSED(filename)  // 未使用的参数
    // TODO: read data
    delete m_root;  // 删除现有根节点
    m_root = new Node;  // 创建新的根节点

    return true;  // 返回成功
}

/**
 * @brief 保存数据到文件
 * 
 * @param filename 文件名
 * @return 是否保存成功
 */
bool ProjectModel::save(const QString &filename)
{
    Q_UNUSED(filename);  // 未使用的参数
    return true;  // 返回成功
}

/**
 * @brief 获取行数
 * 
 * @param idx 父索引
 * @return 行数
 */
int ProjectModel::rowCount(const QModelIndex &idx) const
{
    if (idx.isValid())
        return static_cast<Node *>(idx.internalPointer())->childCount();  // 如果索引有效，返回子节点数量
    else
        return m_root->childCount();  // 否则返回根节点的子节点数量
}

int ProjectModel::columnCount(const QModelIndex &idx) const
{
    Q_UNUSED(idx);
    return 5;
}

QModelIndex ProjectModel::index(int row, int col, const QModelIndex &parent) const
{
    Node *p = m_root;
    if (parent.isValid()) {
        p = static_cast<Node *>(parent.internalPointer());
    }
    if (row < 0 || row >= p->childCount())
        return QModelIndex();
    else
        return createIndex(row, col, p->child(row));
}

QModelIndex ProjectModel::parent(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return QModelIndex();

    Node *n = static_cast<Node *>(idx.internalPointer());
    assert(n);
    Node *p = n->parent();
    if (p == m_root)
        return QModelIndex();

    Node *pp = p->parent();
    assert(pp);
    return createIndex(pp->childNumber(p), 0, p);
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    switch (section) {
    case 0:
        return tr("Name");
    case 1:
        return tr("Type");
    case 2:
        return tr("Start");
    case 3:
        return tr("End");
    case 4:
        return tr("Completion %");
    default:
        return QVariant();
    }
}

QVariant ProjectModel::data(const QModelIndex &idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    Node *n = static_cast<Node *>(idx.internalPointer());
    assert(n);
    if (idx.column() == 0) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return n->label();
        case KDGantt::TextPositionRole:
            return n->position();
        }
    } else if (idx.column() == 1) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return QVariant::fromValue<int>(n->type());
        }
    } else if (idx.column() == 2) {
        switch (role) {
        case Qt::DisplayRole:
            return n->start().date().toString("dd-MM-yyyy");
        case Qt::EditRole:
        case KDGantt::StartTimeRole:
            return n->start();
        }
    } else if (idx.column() == 3) {
        switch (role) {
        case Qt::DisplayRole:
            return n->end().date().toString("dd-MM-yyyy");
        case Qt::EditRole:
        case KDGantt::EndTimeRole:
            return n->end();
        }
    } else if (idx.column() == 4 && n->completion() >= 0) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return n->completion();
        }
    } else if (idx.column() == 5) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return n->label();
        case KDGantt::ItemTypeRole:
            return n->type();
        case KDGantt::StartTimeRole:
            return n->start();
        case KDGantt::EndTimeRole:
            return n->end();
        case KDGantt::TaskCompletionRole:
            if (n->completion() >= 0)
                return n->completion();
            break;
        }
    }
    return QVariant();
}

bool ProjectModel::setData(const QModelIndex &idx, const QVariant &value,
                           int role)
{
    if (!idx.isValid())
        return false;
    qDebug() << "ProjectModel::setData" << idx.column() << role << value;

    Node *n = static_cast<Node *>(idx.internalPointer());
    assert(n);

    if (idx.column() == 0) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            n->setLabel(value.toString());
            emit dataChanged(idx, idx);
            break;
        case KDGantt::TextPositionRole:
            n->setPosition(static_cast<KDGantt::StyleOptionGanttItem::Position>(value.toInt()));
            emit dataChanged(idx, idx);
            break;
        }
    } else if (idx.column() == 1) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            n->setType(value.toInt());
            emit dataChanged(idx, idx);
            break;
        }
    } else if (idx.column() == 2) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case KDGantt::StartTimeRole:
            n->setStart(value.toDateTime());
            emit dataChanged(idx, idx);
            break;
        }
    } else if (idx.column() == 3) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case KDGantt::EndTimeRole:
            n->setEnd(value.toDateTime());
            emit dataChanged(idx, idx);
            break;
        }
    } else if (idx.column() == 4) {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            n->setCompletion(value.toInt());
            emit dataChanged(idx, idx);
            break;
        }
    }
    return true;
}

bool ProjectModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    Node *p = m_root;
    if (parent.isValid())
        p = static_cast<Node *>(parent.internalPointer());
    assert(p);

    for (int i = 0; i < count; ++i) {
        Node *n = new Node;
        p->insertChild(row + i, n);
    }
    endInsertRows();
    return true;
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex &idx) const
{
    return BASE::flags(idx) | Qt::ItemIsEditable;
}
