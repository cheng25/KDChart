/****************************************************************************
**
** 此文件是KD Chart库的一部分。
** 原文注释: This file is part of the KD Chart library.
** 中文注释: 该文件是Gantt图表组件的单元测试入口，负责执行所有注册的Gantt单元测试
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB，一家KDAB集团公司 <info@kdab.com>
** 原文注释: SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
** 原文注释: SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "../../../src/KDGantt/unittest/testregistry.h"  // 包含单元测试注册表头文件

#include <QApplication>  // 包含Qt应用程序类
#include <iostream>      // 包含标准输入输出流

/**
 * @brief 应用程序入口函数，执行Gantt图表的单元测试
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组，可以指定要运行的测试组名称
 * @return 失败的测试用例数量，0表示所有测试通过
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // 创建Qt应用程序实例，单元测试需要Qt事件循环

    KDAB::UnitTest::Runner r;  // 创建单元测试运行器
    unsigned int failed = 0;   // 初始化失败测试用例计数器

    if (argc == 1) {            // 如果没有提供命令行参数
        failed = r.run();       // 运行所有注册的测试用例
    } else {                    // 如果提供了命令行参数
        // 遍历所有测试组参数
        for (int i = 1; i < argc; ++i) {
            if (argv[i] && *argv[i]) {  // 跳过空参数
                // 运行指定名称的测试组，并累加失败数量
                failed += r.run(argv[i]);
            } else {
                // 输出空参数警告信息到标准错误流
                std::cerr << argv[0] << ": skipping empty group name" << std::endl;
            }
        }
    }

    // 输出失败的测试用例总数到标准输出流
    std::cout << failed << " tests failed." << std::endl;

    return failed;  // 返回失败测试用例数量作为退出码
}
