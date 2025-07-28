/****************************************************************************
**
** 此文件是 KD Chart 库的一部分。
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，一家 KDAB 集团公司 <info@kdab.com>
**
** 许可证：MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>  // 包含Qt项选择类
#include <QMainWindow>     // 包含Qt主窗口基类

QT_BEGIN_NAMESPACE
class QStandardItemModel;  // 前向声明Qt标准项模型类
class QCloseEvent;         // 前向声明Qt关闭事件类
namespace Ui {
class MainWindow;          // 前向声明UI命名空间中的MainWindow类
} // namespace Ui
QT_END_NAMESPACE

namespace KDGantt {
class ConstraintModel;     // 前向声明KDGantt约束模型类
class DateTimeGrid;        // 前向声明KDGantt日期时间网格类
class Legend;              // 前向声明KDGantt图例类
} // namespace KDGantt

/**
 * @brief 主窗口类
 * 
 * 该类是甘特图示例的主窗口，负责初始化UI、数据模型和甘特图组件，
 * 并处理用户交互事件，如添加/删除任务、缩放、更改时间刻度等。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     * @param flags 窗口标志
     */
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});

    /**
     * @brief 析构函数
     */
    ~MainWindow() override;

    /**
     * @brief 关闭事件处理函数
     * 
     * 重写父类的关闭事件处理，用于在窗口关闭时执行清理工作
     * @param event 关闭事件对象
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    /**
     * @brief 添加新任务条目
     * 
     * 弹出任务编辑对话框，添加新的任务到模型中
     */
    void addNewEntry();

    /**
     * @brief 删除选中的任务条目
     * 
     * 从模型中删除当前选中的任务条目
     */
    void removeEntry();

    /**
     * @brief 显示上下文菜单
     * 
     * 在指定位置显示上下文菜单
     * @param pos 鼠标位置
     */
    void showContextMenu(const QPoint &pos);

    /**
     * @brief 根据选择启用动作
     * 
     * 根据当前选中的项启用或禁用相关操作
     * @param selected 当前选中的项
     */
    void enableActions(const QItemSelection &selected);

    /**
     * @brief 放大甘特图
     */
    void zoomIn();

    /**
     * @brief 缩小甘特图
     */
    void zoomOut();

    /**
     * @brief 适配甘特图到视图
     */
    void zoomFit();

    /**
     * @brief 自动缩放
     */
    void scaleAuto();

    /**
     * @brief 设置小时刻度
     */
    void scaleHour();

    /**
     * @brief 设置天刻度
     */
    void scaleDay();

    /**
     * @brief 设置周刻度
     */
    void scaleWeek();

    /**
     * @brief 设置月刻度
     */
    void scaleMonth();

private:
    /**
     * @brief 初始化数据模型
     * 
     * 创建并配置任务数据模型
     */
    void initModel();

    /**
     * @brief 初始化动作
     * 
     * 创建并配置菜单和工具栏动作
     */
    void initActions();

    /**
     * @brief 初始化项委托
     * 
     * 设置自定义项委托用于编辑任务
     */
    void initItemDelegate();

    /**
     * @brief 初始化网格
     * 
     * 配置甘特图的日期时间网格
     */
    void initGrid();

    /**
     * @brief 设置项的只读状态
     * 
     * @param index 模型索引
     * @param readOnly 是否只读
     */
    void setReadOnly(const QModelIndex &index, bool readOnly);

    /**
     * @brief 添加任务约束
     * 
     * 在两个任务之间添加依赖约束
     * @param index1 前置任务索引
     * @param index2 后续任务索引
     */
    void addConstraint(const QModelIndex &index1, const QModelIndex &index2);

    QStandardItemModel *model;              // 任务数据模型
    KDGantt::ConstraintModel *constraintModel;  // 任务约束模型
    KDGantt::DateTimeGrid *grid;            // 日期时间网格
    KDGantt::Legend *smallLegend;           // 小型图例
    KDGantt::Legend *detailedLegend;        // 详细图例

    QAction *newEntryAction;                // 添加新任务动作
    QAction *removeEntryAction;             // 删除任务动作
    QAction *zoomInAction;                  // 放大动作
    QAction *zoomOutAction;                 // 缩小动作
    QAction *zoomFitAction;                 // 适配动作

    Ui::MainWindow *ui;                     // UI界面指针
}; // class MainWindow

#endif /* MAINWINDOW_H */
