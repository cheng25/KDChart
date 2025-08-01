/****************************************************************************
**
** 此文件是KD Chart库的缩放示例程序中的缩放部件实现文件
**
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB, 一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
** 此文件实现了ZoomWidget类，包括缩放中心计算、鼠标和键盘事件处理等功能
**
****************************************************************************/

// 包含缩放部件头文件
#include "zoomwidget.h"
// 包含KDChart库的抽象坐标平面类头文件
#include <KDChartAbstractCoordinatePlane>
// 包含Qt调试类头文件
#include <QDebug>
// 包含Qt滚轮事件类头文件
#include <QWheelEvent>

/**
 * @brief ZoomWidget类构造函数
 * @param parent 父窗口指针
 * @details 初始化缩放图表部件，设置焦点策略为滚轮焦点
 */
ZoomWidget::ZoomWidget(QWidget *parent)
    : KDChart::Widget(parent)
{
    // 设置焦点策略，允许通过鼠标滚轮获取焦点
    setFocusPolicy(Qt::WheelFocus);
}

/**
 * @brief 查找新的缩放中心
 * @param pos 鼠标位置
 * @return 新的缩放中心点
 * @details 根据鼠标位置计算图表中的缩放中心
 */
QPointF ZoomWidget::findNewZoomCenter(const QPoint &pos)
{
    // 如果部件高度或宽度为0，返回当前缩放中心
    if (!height() || !width())
        return coordinatePlane()->zoomCenter();

    // 坐标宽度和高度的标准化值
    const qreal coordWidth = 1.0;
    const qreal coordHeight = 1.0;

    // 计算X和Y方向的分辨率（每像素对应的坐标单位）
    const qreal resX = static_cast<qreal>(coordWidth / coordinatePlane()->zoomFactorX()) / width();
    const qreal resY = static_cast<qreal>(coordHeight / coordinatePlane()->zoomFactorY()) / height();

    // 计算鼠标位置相对于中心的偏移量（坐标单位）
    const qreal dX = (pos.x() - 0.5 * width()) * resX;
    const qreal dY = (pos.y() - 0.5 * height()) * resY;

    // 计算新的缩放中心
    const qreal zoomCenterX = coordinatePlane()->zoomCenter().x() + dX;
    const qreal zoomCenterY = coordinatePlane()->zoomCenter().y() + dY;
    return QPointF(zoomCenterX, zoomCenterY);
}

/**
 * @brief 鼠标按下事件处理
 * @param e 鼠标事件对象
 * @details 处理鼠标按下事件，设置缩放中心
 */
void ZoomWidget::mousePressEvent(QMouseEvent *e)
{
    // 根据鼠标位置计算新的缩放中心
    const QPointF zoomCenter(findNewZoomCenter(e->pos()));

    // 如果新的缩放中心与当前不同，则更新缩放中心
    if (zoomCenter != coordinatePlane()->zoomCenter()) {
        coordinatePlane()->setZoomCenter(zoomCenter);
        // 更新图表显示
        update();
    }
}

/**
 * @brief 鼠标滚轮事件处理
 * @param e 滚轮事件对象
 * @details 处理鼠标滚轮事件，实现图表缩放
 */
void ZoomWidget::wheelEvent(QWheelEvent *e)
{
    // 计算缩放增量（滚轮每滚动120度为一个单位）
    qreal delta = static_cast<qreal>(e->angleDelta().y()) / 120.0 / 10.0;

    // 更新X和Y方向的缩放因子
    coordinatePlane()->setZoomFactorX(coordinatePlane()->zoomFactorX() + delta);
    coordinatePlane()->setZoomFactorY(coordinatePlane()->zoomFactorY() + delta);

    // 注释掉的代码：根据鼠标位置更新缩放中心的两种方式
    /* new:
    const QPointF zoomCenter( findNewZoomCenter( e->pos() ) );
    if ( zoomCenter != coordinatePlane()->zoomCenter() ) {
        qDebug() << "zoom center = " << zoomCenter;
        coordinatePlane()->setZoomCenter( zoomCenter );
    }
*/
    /* old:
    qreal zoomCenterX = static_cast<qreal>( e->pos().x() ) / static_cast<qreal>( width() );
    qreal zoomCenterY = static_cast<qreal>( e->pos().y() ) / static_cast<qreal>( height() );
    QPointF zoomCenter( zoomCenterX, zoomCenterY );
    coordinatePlane()->setZoomCenter( zoomCenter );
*/

    // 更新图表显示
    update();
}

/**
 * @brief 键盘按下事件处理
 * @param e 键盘事件对象
 * @details 处理键盘按下事件，支持方向键移动和+/-键缩放
 */
void ZoomWidget::keyPressEvent(QKeyEvent *e)
{
    // 初始化缩放和移动增量
    int dZoom = 0;
    qreal dX = 0;
    qreal dY = 0;

    // 根据按键类型设置相应的增量
    switch (e->key()) {
    case Qt::Key_PageDown:  // 放大
        dZoom = 1;
        break;
    case Qt::Key_Down:      // 向下移动
        dY = 0.1;
        break;
    case Qt::Key_Right:     // 向右移动
        dX = 0.1;
        break;
    case Qt::Key_PageUp:    // 缩小
        dZoom = -1;
        break;
    case Qt::Key_Up:        // 向上移动
        dY = -0.1;
        break;
    case Qt::Key_Left:      // 向左移动
        dX = -0.1;
        break;
    default:
        // 忽略其他按键
        e->ignore();
    }

    // 如果有缩放或移动增量，则应用变更
    if (dZoom || dX || dY) {
        // 确保缩放因子不小于1.0
        const double factor = qMax(qreal(1.0), coordinatePlane()->zoomFactorX() + dZoom);
        // 计算新的缩放中心
        const qreal x = coordinatePlane()->zoomCenter().x() + dX;
        const qreal y = coordinatePlane()->zoomCenter().y() + dY;

        // 应用新的缩放因子和缩放中心
        coordinatePlane()->setZoomFactorX(factor);
        coordinatePlane()->setZoomFactorY(factor);
        coordinatePlane()->setZoomCenter(QPointF(x, y));

        // 更新图表显示
        update();
    }
}

// TODO: Qt5.15.2升级 检查QWheelEvent::angleDelta()的API是否有变更
// TODO: Qt5.15.2升级 确认KDChart::AbstractCoordinatePlane的缩放相关API是否有变化
// TODO: C++17升级 考虑使用std::clamp替代qMax
// TODO: C++17升级 可以使用结构化绑定简化代码
