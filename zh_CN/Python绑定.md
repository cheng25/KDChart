# Python 绑定

- 分钟等级: 进阶

KDChart 提供了 Python 绑定,允许您直接从 Python 代码中使用其强大的图表功能。本文档解释了如何在您的 Python 应用程序中有效地安装、配置和使用 KDChart Python 绑定。

KDChart Python 绑定使用 Shiboken(PySide 绑定生成器)实现,并通过 PyKDChart 模块提供对 KDChart 功能的访问。目前,Python 绑定主要专注于提供对 KDGantt 组件的访问,允许您创建强大的甘特图用于项目管理可视化。

来源:CMakeLists.txt

## 要求

在使用 KDChart 的 Python 绑定之前,请确保已安装以下先决条件:

* Python 3.6 或更高版本
* Qt5(用于 PySide2)或 Qt6(用于 PySide6)
* PySide2 或 PySide6(与您的 Qt 版本匹配)
* Shiboken2 或 Shiboken6(与您的 PySide 版本匹配)

来源:CMakeLists.txt#L27-L29

## 安装

### 从源代码构建

Python 绑定作为 KDChart 库的一部分进行构建。要构建和安装它们:

1. 配置 KDChart 以启用 Python 绑定:

   BASH

   ```
   cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/your/install/path -DKDChart_PYTHON_BINDINGS=ON
   ```
2. 构建 KDChart:

   BASH

   ```
   cmake --build build
   ```
3. 安装 KDChart:

   BASH

   ```
   cmake --install build
   ```

Python 绑定将安装在 Python 站点包目录中,使它们可以在您的 Python 应用程序中导入。

来源:CMakeLists.txt#L30-L44

## 模块结构

Python 绑定按以下模块结构组织:

```
PyKDChart └── KDGantt
```

### 可用模块

目前,Python 绑定主要公开 KDGantt 组件。KDGantt 模块提供了用于创建甘特图的类,这对于项目管理和日程安排可视化非常有用。

来源:PyKDChart/CMakeLists.txt, KDGantt/CMakeLists.txt

## 在 Python 中使用 KDGantt

### 基本导入

要在您的 Python 代码中使用 KDGantt,请按如下方式导入:

PYTHON

```
# 用于 PySide2 from PySide2.QtCore import Qt from PySide2.QtWidgets import QApplication from PyKDChart.KDGantt import View, DateTimeGrid, Constraint, ConstraintModel from PyKDChart import KDGantt # 用于枚举值 # 用于 PySide6 from PySide6.QtCore import Qt from PySide6.QtWidgets import QApplication from PyKDChartQt6.KDGantt import View, DateTimeGrid, Constraint, ConstraintModel from PyKDChartQt6 import KDGantt # 用于枚举值
```

来源:examples/view/main.py#L17-L22

### 关键类和概念

KDGantt Python 绑定中提供了以下关键类:

| 类 | 描述 |
| --- | --- |
| View | 显示甘特图的主小部件 |
| DateTimeGrid | 支持基于日期和时间的坐标的网格 |
| AbstractGrid | 网格实现的基类 |
| Constraint | 表示甘特图项之间的约束 |
| ConstraintModel | 管理项之间约束的模型 |
| GraphicsView | 用于渲染甘特图的图形视图 |
| GraphicsScene | 包含图表项的场景 |
| StyleOptionGanttItem | 甘特图项的样式选项 |
| ItemDelegate | 控制项的渲染方式 |

来源:KDGantt/typesystem\_kdgantt.xml#L12-L103

### 创建一个简单的甘特图

以下是一个使用 KDChart Python 绑定创建甘特图的基本示例:

PYTHON

```
import sys from PySide2.QtCore import Qt, QDateTime from PySide2.QtGui import QStandardItemModel, QStandardItem from PySide2.QtWidgets import QApplication, QWidget, QVBoxLayout from PyKDChart.KDGantt import View, DateTimeGrid from PyKDChart import KDGantt # 创建一个标准项模型用于甘特图 model = QStandardItemModel() model.setColumnCount(5) # 名称、类型、开始、结束、完成度 # 创建一个任务 task = QStandardItem("任务 1") task_type = QStandardItem(KDGantt.TypeTask) start_time = QStandardItem() start_time.setData(QDateTime.currentDateTime(), KDGantt.StartTimeRole) end_time = QStandardItem() end_time.setData(QDateTime.currentDateTime().addDays(3), KDGantt.EndTimeRole) completion = QStandardItem("50%") # 将任务添加到模型 model.appendRow([task, task_type, start_time, end_time, completion]) # 创建视图和网格 app = QApplication(sys.argv) widget = QWidget() layout = QVBoxLayout(widget) gantt_view = View() grid = DateTimeGrid() # 配置网格 grid.setStartDateTime(QDateTime.currentDateTime().addDays(-1)) grid.setDayWidth(100) # 设置视图 gantt_view.setGrid(grid) gantt_view.setModel(model) # 将视图添加到小部件 layout.addWidget(gantt_view) widget.resize(800, 400) widget.show() sys.exit(app.exec_())
```

此示例创建了一个简单的甘特图,其中包含一个持续三天的任务。

来源:examples/view/main.py#L40-L107

### 处理约束

约束表示甘特图中任务之间的依赖关系。以下是如何添加约束:

PYTHON

```
# 创建一个约束模型 constraint_model = ConstraintModel() # 在项之间添加约束(index_from 和 index_to 是 QModelIndex 对象) constraint_model.addConstraint(Constraint(index_from, index_to)) # 在视图上设置约束模型 gantt_view.setConstraintModel(constraint_model)
```

来源:examples/view/main.py#L84-L94

### 数据模型结构

KDGantt 使用 QStandardItemModel,具有以下列:

1. 标题 - 项的显示文本
2. 类型 - 甘特图项的类型(任务、事件、摘要等)
3. 开始时间 - 项的开始时间
4. 结束时间 - 项的结束时间
5. 完成度 - 项的完成百分比

类型列应包含以下 KDGantt 项类型之一:

| 项类型 | 描述 |
| --- | --- |
| KDGantt.TypeTask | 带有可选完成指示器的任务条 |
| KDGantt.TypeEvent | 里程碑或事件标记 |
| KDGantt.TypeSummary | 摘要任务 |
| KDGantt.TypeMulti | 可以包含其他项的多项 |

来源:examples/view/main.py#L47-L77, KDGantt/typesystem\_kdgantt.xml#L10

## 高级功能

### 自定义网格

DateTimeGrid 类提供了几种自定义选项:

PYTHON

```
grid = DateTimeGrid() # 设置比例 grid.setScale(DateTimeGrid.ScaleDay) # 显示天数 # 设置日宽度(控制缩放级别) grid.setDayWidth(100) # 每天像素 # 设置开始日期/时间 grid.setStartDateTime(QDateTime.currentDateTime().addDays(-7)) # 自定义比例格式化器 formatter = DateTimeScaleFormatter() grid.setUserDefinedUpperScale(formatter)
```

来源:examples/view/main.py#L102-L103, KDGantt/typesystem\_kdgantt.xml#L35-L47

### 打印和导出

KDGantt 图表可以打印或渲染为图像:

PYTHON

```
# 打印到打印机 printer = QPrinter(QPrinter.HighResolution) printDialog = QPrintDialog(printer, parent) if printDialog.exec_() == QDialog.Accepted: view.print(printer, rect.left(), rect.right()) # 渲染到 QPixmap pixmap = QPixmap(1000, 200) pixmap.fill(Qt.white) painter = QPainter(pixmap) view.print(painter, pixmap.rect()) painter.end()
```

来源:examples/view/main.py#L126-L145

## 故障排除

### 常见问题

* 缺失模块:确保 PyKDChart 在您的 Python 路径中
* 版本不匹配:确保您的 PySide/Shiboken 版本与您的 Qt 版本匹配
* 导入错误:检查您是否使用了正确的导入(Qt5 使用 PyKDChart,Qt6 使用 PyKDChartQt6)

### 调试信息

如果遇到问题,请检查您的构建配置以确保启用了 Python 绑定:

BASH

```
cmake -LA | grep PYTHON
```

## 其他资源

* 查看 python/examples/ 目录中的示例代码以获取完整的工作示例
* python/examples-qt6/ 目录包含适用于 Qt6/PySide6 的示例
* 底层的 C++ 类具有相同的 API,因此 C++ 文档有助于理解 Python API