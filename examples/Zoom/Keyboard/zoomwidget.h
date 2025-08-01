/****************************************************************************
**
** 此文件是KD Chart库的缩放示例程序中的缩放部件头文件
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB, 一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
** 此文件定义了ZoomWidget类，扩展了KDChart::Widget，添加了键盘和鼠标缩放功能
**
****************************************************************************/

// 防止头文件重复包含
#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

// 包含KDChart库的Widget类头文件，ZoomWidget继承自此类
#include <KDChartWidget>

/**
 * @brief 缩放图表部件类
 * @details 此类继承自KDChart::Widget，添加了鼠标和键盘缩放功能
 *          支持鼠标滚轮缩放、鼠标点击设置缩放中心和键盘控制缩放
 */
class ZoomWidget : public KDChart::Widget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     * @details 初始化缩放图表部件
     */
    explicit ZoomWidget(QWidget *parent);

protected:
    /**
     * @brief 查找新的缩放中心
     * @param pos 鼠标位置
     * @return 新的缩放中心点
     * @details 根据鼠标位置计算图表中的缩放中心
     */
    QPointF findNewZoomCenter(const QPoint &pos);

    /**
     * @brief 鼠标按下事件处理
     * @param e 鼠标事件对象
     * @details 处理鼠标按下事件，设置缩放中心
     */
    void mousePressEvent(QMouseEvent *e) override;

    /**
     * @brief 鼠标滚轮事件处理
     * @param e 滚轮事件对象
     * @details 处理鼠标滚轮事件，实现图表缩放
     */
    void wheelEvent(QWheelEvent *e) override;

    /**
     * @brief 键盘按下事件处理
     * @param e 键盘事件对象
     * @details 处理键盘按下事件，支持方向键移动和+/-键缩放
     */
    void keyPressEvent(QKeyEvent *e) override;
};

#endif /* ZOOMWIDGET_H */

// TODO: Qt5.15.2升级 检查QWheelEvent和QKeyEvent的API是否有变更
// TODO: Qt5.15.2升级 确认事件重写方法的签名是否需要调整
// TODO: C++17升级 考虑使用constexpr优化常量定义
// TODO: C++17升级 可以使用std::optional处理可能为空的返回值
