# KDChart 示例项目集合

## 项目概述

本仓库包含了KDChart库的多个示例项目，展示了如何使用KDChart库创建各种类型的图表和数据可视化界面。KDChart是一个功能强大的Qt图表库，支持柱状图、折线图、饼图等多种图表类型，以及丰富的自定义选项。

## 项目结构

### 顶级目录功能说明

- **.github/**: GitHub 相关配置文件
- **assets/**: 资源文件
- **cmake/**: CMake 配置模块
- **conan/**: Conan 包管理器相关文件
- **deploy/**: 部署相关文件
- **distro/**: 发行版相关配置
- **docs/**: 文档，包括 API 参考和程序员手册
- **examples/**: 示例程序，展示各种图表类型的使用方法
- **features/**: 特性文件
- **images/**: 图像资源
- **kdablibfakes/**: KDAB 库模拟实现
- **python/**: Python 绑定相关代码
- **qtests/**: Qt 单元测试
- **scripts/**: 脚本文件
- **src/**: 源代码，包含 KD Chart 库的核心实现
- **tests/**: 测试代码

### 示例目录详细说明

**examples/** 目录包含了各种图表类型的示例程序，以下是主要子目录的说明：

- **Axis/**: 坐标轴相关示例;展示如何配置图表坐标轴的各种属性，包括位置、标题、标签、刻度线等
  - `Chart`: 坐标轴配置的具体实现示例
  - `Labels`: 坐标轴标签配置示例
  - `Parameters`: 坐标轴参数配置示例
  - `Widget`: 坐标轴部件示例
- **Background/**: 背景设置示例;展示如何设置图表的背景、边框、标题和文本属性等
- **Bars/**: 柱状图示例;展示柱状图的各种配置和用法
- **DataValueTexts/**: 数据值文本示例;展示如何配置数据值文本的显示方式
- **DrawIntoPainter/**: 绘制到画家示例;展示如何将图表绘制到QPainter中
- **EmptyValues/**: 空值处理示例;展示如何处理图表中的空值
- **Gantt/**: 甘特图示例;展示甘特图的用法
- **Grids/**: 网格示例;展示图表网格线的配置
- **HeadersFooters/**: 页眉页脚示例;展示图表页眉和页脚的配置
- **Legends/**: 图例示例;展示图表图例的配置
- **LeveyJennings/**: Levey-Jennings 图表示例;展示Levey-Jennings图表的用法
- **Lines/**: 折线图示例;展示折线图的各种配置和用法
- **ModelView/**: 模型视图示例;展示如何将KDChart与Qt的模型视图架构结合使用
  - `HidingData`: 展示如何动态隐藏/显示数据系列
  - `TableView`: 展示表格视图与图表的数据交互
- **NoValues/**: 无值示例;展示无数据值的图表配置
- **Pie/**: 饼图示例;展示饼图的各种配置和用法
- **Plane/**: 平面示例;展示平面图表的用法
- **Plotter/**: 绘图仪示例;展示绘图仪图表的用法
- **Polar/**: 极坐标图表示例;展示极坐标图表的用法
  - **Advanced/**: 高级极坐标图表示例，包含交互控件
  - **Parameters/**: 参数化极坐标图表示例，展示各种配置选项
  - **Simple/**: 简单极坐标图表示例，展示基本用法
- **RealTime/**: 实时数据示例;展示实时更新的图表用法
- **SharedAbscissa/**: 共享横坐标示例;展示共享横坐标的多图表配置
- **Sql/**: SQL 数据示例;展示如何将KDChart与SQL数据库结合使用
- **Stock/**: 股票图表示例;展示股票图表的用法
- **TernaryCharts/**: 三元图表示例;展示三元图表的用法
- **Widget/**: 窗口部件示例;展示图表部件的基本用法
- **Zoom/**: 缩放功能示例;展示图表缩放功能的用法
- **demo/**: 综合演示;展示KDChart库的综合示例
- **tools/**: 包含一些辅助工具
## 版本升级说明

### Qt5.15.2升级说明

#### 主要变更点

1. **QDateTime API更新**
   - 检查并验证了`QDateTime::fromSecsSinceEpoch`方法在Qt5.15.2中的行为
   - 确保时间戳转换功能正常工作

2. **QSortFilterProxyModel API更新**
   - 验证了`QSortFilterProxyModel::invalidateFilter`方法的正确性
   - 确保数据过滤功能不受版本升级影响

   

#### 影响范围

- 所有使用Qt框架的代码文件
- 特别是时间处理相关功能（TimeAxis和TimeChartModel类）
- 数据过滤和模型相关功能



### C++17升级说明

#### 主要调整点

1. **使用结构化绑定**
   - 优化了`QPair`等数据结构的使用方式
   - 提高代码可读性和简洁性
2. **使用std::chrono处理时间**
   - 替代了部分手动时间计算
   - 使时间处理代码更加清晰和类型安全
3. **使用std::optional处理无效数据**
   - 提高了数据处理的安全性
   - 避免了空指针和未定义行为
4. **使用std::from_chars提高性能**
   - 替代了`toDouble`等数值转换函数
   - 提高了数值转换的性能和安全性

5. constexpr

#### 采用的新特性

- 结构化绑定（Structured Bindings）
- std::chrono库
- std::optional
- std::from_chars
- 其他C++17核心语言特性和标准库增强

### 升级代码标记汇总

#### Qt5.15.2升级标记

```cpp
// TODO: Qt5.15.2升级 检查QApplication构造函数参数是否需要调整
// TODO: Qt5.15.2升级 处理QApplication::exec()返回值
// TODO: Qt5.15.2升级 检查QDateTime API是否有变更
// TODO: Qt5.15.2升级 检查QSortFilterProxyModel API是否有变更
// TODO: Qt5.15.2升级 检查KDChartCartesianAxis API是否有变更
// TODO: Qt5.15.2升级 检查QDateTime::fromSecsSinceEpoch是否有变更
// TODO: Qt5.15.2升级 检查invalidateFilter是否有变更
// TODO: Qt5.15.2升级 检查QObject::connect API是否有变更
```

#### C++17升级标记

```cpp
// TODO: C++17升级 考虑使用std::cmath替代cmath
// TODO: C++17升级 考虑使用std::generate_n替代循环
// TODO: C++17升级 使用std::sin替代sin
// TODO: C++17升级 使用std::from_chars替代toDouble提高性能
// TODO: C++17升级 使用结构化绑定简化代码
// TODO: C++17升级 使用std::chrono替代手动时间计算
// TODO: C++17升级 使用std::optional处理无效数据
```

