# DelayedData测试工程说明

## 概述
本目录包含了KD Chart库的延迟数据测试工程，用于演示动态添加行和列数据时图表的实时更新效果。

## 文件说明
- `main.cpp`: 测试工程的主文件，包含图表显示和数据操作逻辑

## 功能说明
1. 动态添加行数据：点击"Add rows"按钮可以向图表添加一行数据
2. 动态添加列数据：点击"Add columns"按钮可以向图表添加一列数据
3. 实时更新图表：当数据模型发生变化时，图表会自动更新显示

## 代码执行逻辑
1. 创建`ChartWidget`窗口部件，初始化空的数据模型
2. 创建柱状图对象，并关联到数据模型
3. 配置图表的坐标轴和图例
4. 设置添加行和列的按钮，并连接到相应的槽函数
5. 当点击按钮时，动态修改数据模型，并触发图表更新

## Qt5.15.2升级说明
本测试工程已检查Qt5.15.2兼容性，未发现使用已废弃API的情况。所有Qt相关代码均符合Qt5.15.2标准。

## C++17升级说明
本测试工程已调整为兼容C++17标准，使用了以下C++17特性：
- 类型推导（auto）
- Lambda表达式
- nullptr代替NULL

所有代码均符合C++17标准规范。