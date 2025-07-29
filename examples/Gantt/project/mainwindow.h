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

#include <QDialog>      // 包含Qt对话框类
#include <QMainWindow>  // 包含Qt主窗口类

QT_BEGIN_NAMESPACE
class QLineEdit;    // 前向声明QLineEdit类
class QCheckBox;    // 前向声明QCheckBox类
QT_END_NAMESPACE

namespace KDGantt {
class View;         // 前向声明KDGantt::View类
}

class ProjectModel;  // 前向声明ProjectModel类

/**
 * @brief 保存PDF对话框类
 * 
 * 用于配置和执行PDF文件保存操作的对话框
 */
class SavePdfDialog : public QDialog
{
    Q_OBJECT
public:
    QLineEdit *m_fileEdit;      // 文件路径输入框
    QCheckBox *m_rowLabels;     // 行标签复选框
    QCheckBox *m_columnLabels;  // 列标签复选框

    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    SavePdfDialog(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 文件按钮点击槽函数
     * 
     * 用于打开文件选择对话框
     */
    void fileButtonClicked();
};

/**
 * @brief 主窗口类
 * 
 * 应用程序的主窗口，包含甘特图视图和各种操作菜单
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief 保存PDF文件槽函数
     */
    void slotFileSavePdf();

    /**
     * @brief 打印槽函数
     */
    void slotFilePrint();

    /**
     * @brief 退出槽函数
     */
    void slotFileQuit();

    /**
     * @brief 新建项目项槽函数
     */
    void slotToolsNewItem();

    /**
     * @brief 追加项目项槽函数
     */
    void slotToolsAppendItem();

    /**
     * @brief 全部折叠槽函数
     */
    void slotCollapseAll();

    /**
     * @brief 全部展开槽函数
     */
    void slotExpandAll();

    /**
     * @brief 左对齐槽函数
     */
    void slotAlignLeft();

    /**
     * @brief 居中对齐槽函数
     */
    void slotAlignCenter();

    /**
     * @brief 右对齐槽函数
     */
    void slotAlignRight();

    /**
     * @brief 隐藏对齐槽函数
     */
    void slotAlignHidden();

private:
    ProjectModel *m_model;  // 项目模型指针
    KDGantt::View *m_view;  // 甘特图视图指针
};

#endif /* MAINWINDOW_H */
