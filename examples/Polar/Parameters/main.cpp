/****************************************************************************
**
** 此文件是KD Chart库的一部分。
**
** 版权所有: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
**
** 许可证: MIT
**
****************************************************************************/
// TODO: Qt5.15.2升级 检查头文件包含是否需要调整

#include <KDChartBackgroundAttributes> // 背景属性类，用于设置图表背景
#include <KDChartChart>                 // 图表主类
#include <KDChartDataValueAttributes>   // 数据值属性类，用于设置数据点的显示方式
#include <KDChartPolarDiagram>          // 极坐标图表类
#include <QtGui>                        // Qt GUI库
// TODO: Qt5.15.2升级 考虑使用QtWidgets代替QtGui，因为Qt5.15中部分API已移动

#include <QApplication>

using namespace KDChart;

/**
 * @brief ChartWidget 自定义图表窗口类
 * 
 * 该类继承自QWidget，用于展示极坐标图表，并配置图表的各种属性，
 * 包括背景、数据点标记、数据值显示等。
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     * 
     * 初始化图表组件、数据模型和UI布局。
     */
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 初始化数据模型并填充数据
        m_model.insertRows(0, 10);  // 插入10行
        m_model.insertColumns(0, 5); // 插入5列
        int value = 0;
        // 遍历所有列和行，填充数据
        for (int column = 0; column < m_model.columnCount(); ++column) {
            for (int row = 0; row < m_model.rowCount(); ++row) {
                QModelIndex index = m_model.index(row, column); // 获取单元格索引
                m_model.setData(index, QVariant(value++)); // 设置单元格数据并递增
            }
        }
        // 需要为极坐标图表创建极坐标平面
        auto *polarPlane = new PolarCoordinatePlane(&m_chart);
        // 用极坐标平面替换默认的笛卡尔平面
        m_chart.replaceCoordinatePlane(polarPlane);

        // 创建极坐标图表并设置数据模型
        auto *diagram = new PolarDiagram;
        diagram->setModel(&m_model);
        // TODO: Qt5.15.2升级 检查PolarDiagram API是否有变更

        // 配置极坐标平面的背景
        BackgroundAttributes pba;
        pba.setBrush(QBrush(QColor(0x20, 0x20, 0x60))); // 设置背景画刷为深蓝色
        pba.setVisible(true); // 设置背景可见
        polarPlane->setBackgroundAttributes(pba); // 应用背景属性到极坐标平面

        // 配置全局/数据集/单元格特定属性

        // 获取并配置数据值属性
        DataValueAttributes dva(diagram->dataValueAttributes());
        MarkerAttributes ma(dva.markerAttributes());
        ma.setVisible(true); // 设置标记可见
        ma.setMarkerStyle(MarkerAttributes::MarkerSquare); // 设置标记样式为方形
        ma.setMarkerSize(QSize(6, 6)); // 设置标记大小
        dva.setMarkerAttributes(ma);
        // 设置数据值文本的位置
        // 我们希望它们居中显示在各自标记的顶部
        RelativePosition relativePosition(dva.positivePosition());
        relativePosition.setReferencePosition(Position::Center); // 设置参考位置为中心
        relativePosition.setAlignment(Qt::AlignBottom | Qt::AlignHCenter); // 设置对齐方式
        relativePosition.setHorizontalPadding(KDChart::Measure(0.0, KDChartEnums::MeasureCalculationModeAbsolute)); // 设置水平边距
        relativePosition.setVerticalPadding(KDChart::Measure(0.0, KDChartEnums::MeasureCalculationModeAbsolute)); // 设置垂直边距
        dva.setPositivePosition(relativePosition);
        diagram->setDataValueAttributes(dva); // 应用数据值属性到图表

        // 显示数据值
        const QFont font(QFont("Comic", 10)); // 设置字体
        const int colCount = diagram->model()->columnCount(); // 获取列数
        // 遍历所有列，配置每列的数据值属性
        for (int iColumn = 0; iColumn < colCount; ++iColumn) {
            DataValueAttributes dva(diagram->dataValueAttributes(iColumn));
            TextAttributes ta(dva.textAttributes());
            ta.setRotation(0); // 设置文本旋转角度
            ta.setFont(font); // 设置文本字体
            ta.setPen(QPen(Qt::gray)); // 设置文本颜色为灰色
            ta.setVisible(true); // 设置文本可见
            dva.setTextAttributes(ta);
            dva.setVisible(true); // 设置数据值可见
            diagram->setDataValueAttributes(iColumn, dva); // 应用数据值属性到列
        }

        // 设置单个单元格的标记样式不同，以展示如何使用单元格特定的标记属性
        const QModelIndex index = diagram->model()->index(1, 2, QModelIndex()); // 获取(1,2)单元格的索引
        dva = diagram->dataValueAttributes(index);
        ma = dva.markerAttributes();
        ma.setMarkerStyle(MarkerAttributes::MarkerCircle); // 设置标记样式为圆形
        ma.setMarkerSize(QSize(40, 40)); // 增大标记大小

        // 这是调整标记颜色的标准方法：
        // 默认颜色无效，所以我们使用显式的后备颜色
        // 以确保我们获得正确的颜色，就像KD Chart内置逻辑所使用的那样：
        QColor semiTrans(ma.markerColor());
        if (!semiTrans.isValid())
            semiTrans = diagram->brush(index).color();

        semiTrans.setAlpha(164); // 设置透明度
        ma.setMarkerColor(semiTrans.darker()); // 设置标记颜色为暗色调

        dva.setMarkerAttributes(ma);

        // 同时，我们还将此特殊点的文本对齐方式设置为居中
        relativePosition = dva.positivePosition();
        relativePosition.setAlignment(Qt::AlignCenter); // 设置对齐方式为居中
        dva.setPositivePosition(relativePosition);
        diagram->setDataValueAttributes(index, dva); // 应用数据值属性到单元格

        // 将图表分配给坐标平面
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // 我们希望在极坐标图表周围有一个漂亮的间隙，
        // 但我们也希望坐标平面的背景覆盖该区域，
        // 所以我们使用一些缩放。
        // 注意：设置缩放因子不能在指定图表并将其分配给坐标平面之前完成！
        polarPlane->setZoomFactorX(0.9); // 设置X轴缩放因子
        polarPlane->setZoomFactorY(0.9); // 设置Y轴缩放因子

        // 创建垂直布局
        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart); // 将图表添加到布局
        m_chart.setGlobalLeadingTop(5); // 设置图表顶部全局边距为5像素
        m_chart.setGlobalLeadingBottom(5); // 设置图表底部全局边距为5像素
        setLayout(l); // 设置窗口布局
    }

private:
    Chart m_chart;               // 图表对象
    QStandardItemModel m_model;  // 数据模型对象
};

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 应用程序的入口点，创建应用程序对象、图表窗口并显示。
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv); // 创建Qt应用程序对象

    ChartWidget w; // 创建图表窗口对象
    w.show(); // 显示图表窗口

    return app.exec(); // 进入应用程序事件循环
}

#include "main.moc"
