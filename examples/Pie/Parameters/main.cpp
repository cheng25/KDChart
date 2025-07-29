/****************************************************************************
**
** This file is part of the KD Chart library.
** 此文件是KD Chart库的饼图参数配置示例文件。
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
** 版权所有 (C) 2001 Klarälvdalens Datakonsult AB，KDAB集团公司 <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 许可证: MIT
**
****************************************************************************/

#include <KDChartBackgroundAttributes>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartPieAttributes>
#include <KDChartPieDiagram>
#include <KDChartPosition>
#include <QtGui>

#include <QApplication>

using namespace KDChart;

/**
 * @brief 自定义图表窗口类
 * 
 * 该类继承自QWidget，用于展示饼图的各种参数配置，包括：
 * - 扇区爆炸效果
 * - 自定义扇区画笔样式
 * - 数据值显示配置
 * - 背景属性设置
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     * 
     * 初始化图表窗口，创建数据模型，配置饼图属性。
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型
        m_model.insertRows(0, 1, QModelIndex()); // 插入1行
        m_model.insertColumns(0, 6, QModelIndex()); // 插入6列
        // 填充数据
        for (int row = 0; row < 1; ++row) {
            for (int column = 0; column < 6; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row + 1 * column + 1)); // 设置数据值

                // 为数据项设置工具提示
                m_model.setData(index, QString("row: %1,  column: %2").arg(row).arg(column), Qt::ToolTipRole);
            }
        }
        // 创建极坐标平面（饼图需要极坐标平面）
        auto *polarPlane = new PolarCoordinatePlane(&m_chart);
        // 替换图表的默认坐标平面为极坐标平面
        m_chart.replaceCoordinatePlane(polarPlane);

        // 创建饼图对象并设置数据模型
        auto *diagram = new PieDiagram;
        diagram->setModel(&m_model);

        // 配置饼图特定属性

        // 设置扇区爆炸效果
        PieAttributes pa(diagram->pieAttributes()); // 获取默认饼图属性
        pa.setExplodeFactor(0.1); // 设置爆炸因子

        // 应用属性到第二个扇区（索引为1）
        diagram->setPieAttributes(1, pa);

        // 配置通用属性（适用于所有图表类型）
        QPen sectionPen; // 创建扇区画笔
        sectionPen.setWidth(5); // 设置线宽
        sectionPen.setStyle(Qt::DashLine); // 设置线型为虚线
        sectionPen.setColor(Qt::magenta); // 设置颜色为洋红色

        diagram->setPen(1, sectionPen); // 应用画笔到第二个扇区

        // 显示数据值
        const QFont font(QFont("Comic", 10)); // 创建字体
        const int colCount = diagram->model()->columnCount(); // 获取列数
        for (int iColumn = 0; iColumn < colCount; ++iColumn) {
            DataValueAttributes dva(diagram->dataValueAttributes(iColumn)); // 获取数据值属性
            TextAttributes ta(dva.textAttributes()); // 获取文本属性
            ta.setRotation(0); // 设置旋转角度
            ta.setFont(font); // 设置字体
            ta.setPen(QPen(Qt::darkBlue)); // 设置文本颜色
            ta.setVisible(true); // 设置可见
            dva.setTextAttributes(ta); // 应用文本属性

            BackgroundAttributes back(dva.backgroundAttributes()); // 获取背景属性
            back.setBrush(QBrush(QColor(255, 0, 0))); // 设置背景刷（红色）
            back.setVisible(true); // 设置可见
            dva.setBackgroundAttributes(back); // 应用背景属性

            RelativePosition posPos(dva.positivePosition()); // 获取位置属性
            posPos.setReferencePosition(KDChart::Position::North); // 设置参考位置
            posPos.setAlignment(Qt::AlignCenter); // 设置对齐方式
            posPos.setHorizontalPadding(KDChart::Measure(0.0)); // 设置水平内边距
            posPos.setVerticalPadding(KDChart::Measure(-1000.0)); // 设置垂直内边距
            dva.setPositivePosition(posPos); // 应用位置属性
            dva.setVisible(true); // 设置可见
            diagram->setDataValueAttributes(iColumn, dva); // 应用数据值属性
        }

        // 将图表分配给Chart对象
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 设置窗口布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        setLayout(l);
    } // TODO: Qt5.15.2升级 检查PolarCoordinatePlane在Qt5.15.2中的API变化
    // TODO: Qt5.15.2升级 验证PieAttributes在Qt5.15.2中的行为
    // TODO: C++17升级 考虑使用std::make_unique管理动态内存

private:
    Chart m_chart; // 图表对象
    QStandardItemModel m_model; // 数据模型对象
}; // TODO: C++17升级 考虑使用final修饰符

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建应用程序实例和图表窗口，启动饼图参数配置示例。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序实例

    ChartWidget w; // 创建图表窗口实例
    w.show(); // 显示图表窗口

    return app.exec(); // 运行应用程序事件循环
} // TODO: Qt5.15.2升级 检查QApplication在Qt5.15.2中的行为变化
// TODO: C++17升级 考虑使用constexpr优化常量表达式

#include "main.moc"
