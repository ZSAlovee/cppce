// C++ Lambda 表达式入门教学
// 编译: g++ -std=c++11 test9.cpp -o test9 && ./test9

#include <iostream>
#include <vector>
#include <algorithm>  // sort, for_each, find_if

/*
 * ─── Lambda 是什么？ ───
 * Lambda = 匿名函数，也就是"没有名字的函数"
 * 可以就地定义、就地使用，不用专门写一个函数
 *
 * 基本语法:
 *   [捕获](参数列表) -> 返回类型 { 函数体 }
 *
 *   [捕获]      — 捕获外部变量（怎么把外面的变量拿进来）
 *   (参数列表)  — 和普通函数的参数一样
 *   -> 返回类型  — 可以省略，让编译器自己推断
 *   { 函数体 }  — 要执行的代码
 */

// 一个工具函数: 打印 vector
void printVec(const std::vector<int>& v, const std::string& label) {
    std::cout << "  " << label << ": ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 第一部分: 最简单的 Lambda
// ============================================================
void part1_basic() {
    std::cout << "========== 第一部分: 基本语法 ==========\n";

    // 最简单的 lambda: 没有参数，没有捕获，直接调用
    std::cout << "--- 示例1: hello lambda ---\n";
    [] {
        std::cout << "  你好, Lambda!\n";
    }();  // 后面的 () 表示"立即执行"

    // 带参数的 lambda
    std::cout << "\n--- 示例2: 带参数 ---\n";
    [](int a, int b) {
        std::cout << "  " << a << " + " << b << " = " << (a + b) << "\n";
    }(3, 5);  // 传入参数

    // 带返回值的 lambda
    std::cout << "\n--- 示例3: 带返回值 ---\n";
    int result = [](int a, int b) -> int {
        return a * b;
    }(4, 6);
    std::cout << "  4 * 6 = " << result << "\n";

    // 返回类型可以省略，编译器自动推断
    auto r2 = [](int a, int b) { return a - b; }(10, 3);
    std::cout << "  10 - 3 = " << r2 << "（自动推断返回类型）\n";

    // 把 lambda 存到变量里，反复使用
    std::cout << "\n--- 示例4: 存到变量里 ---\n";
    auto greet = [](const std::string& name) {
        std::cout << "  你好, " << name << "!\n";
    };
    greet("张三");
    greet("李四");
    // auto 就是 lambda 的类型，每个 lambda 类型都不同（编译器生成唯一类型）
}

// ============================================================
// 第二部分: 捕获外部变量 [ ]
// ============================================================
void part2_capture() {
    std::cout << "\n========== 第二部分: 捕获外部变量 ==========\n";

    int x = 10;
    int y = 20;

    // [ ] 默认不捕获任何外部变量，下面这行编译不通过:
    // auto f = []{ return x + y; };  // 编译错误！x, y 没被捕获

    // 示例 1: [=] 按值捕获所有变量（只读）
    std::cout << "--- 示例1: [=] 按值捕获 ---\n";
    auto f1 = [=] {
        // x = 100;  // 编译错误！按值捕获不能修改
        return x + y;
    };
    std::cout << "  x + y = " << f1() << "  （[=] 捕获的是当前值）\n";

    // 示例 2: [&] 按引用捕获所有变量（可修改）
    std::cout << "\n--- 示例2: [&] 按引用捕获 ---\n";
    auto f2 = [&] {
        x = 100;  // 可以修改！
        y = 200;
        return x + y;
    };
    std::cout << "  f2() = " << f2() << "\n";
    std::cout << "  x = " << x << ", y = " << y << "（原变量也被改了）\n";

    // 示例 3: 混合捕获 [=, &y] — x 按值, y 按引用
    std::cout << "\n--- 示例3: 混合捕获 ---\n";
    int a = 1, b = 2, c = 3;
    auto f3 = [=, &b] {  // a, c 按值（只读），b 按引用（可修改）
        b = 999;  // OK，b 是按引用
        // a = 888;  // 编译错误！a 是按值
        return a + b + c;
    };
    std::cout << "  f3() = " << f3() << "\n";
    std::cout << "  b 被改成: " << b << "\n";

    // 示例 4: 指定捕获特定变量
    std::cout << "\n--- 示例4: 指定捕获 ---\n";
    int m = 1, n = 2;
    auto f4 = [m, &n] {  // m 按值, n 按引用
        n = 888;
        return m + n;
    };
    std::cout << "  f4() = " << f4() << "\n";

    // 示例 5: mutable — 按值捕获也能修改（但只影响副本）
    std::cout << "\n--- 示例5: mutable 关键字 ---\n";
    int counter = 0;
    auto f5 = [counter]() mutable {
        counter++;  // mutable 允许修改按值捕获的副本
        return counter;
    };
    std::cout << "  f5() = " << f5() << "\n";
    std::cout << "  f5() = " << f5() << "\n";
    std::cout << "  原 counter = " << counter << "（没变，改的是副本）\n";
}

// ============================================================
// 第三部分: Lambda 的实际用途
// ============================================================
void part3_usage() {
    std::cout << "\n========== 第三部分: 实际用途 ==========\n";

    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // 用途 1: 自定义排序
    std::cout << "--- 用途1: 自定义排序 ---\n";
    auto v1 = nums;
    printVec(v1, "排序前");

    std::sort(v1.begin(), v1.end(), [](int a, int b) {
        return a > b;  // 从大到小排
    });
    printVec(v1, "从大到小");

    std::sort(v1.begin(), v1.end(), [](int a, int b) {
        return a < b;  // 从小到大排
    });
    printVec(v1, "从小到大");

    // 用途 2: 遍历并处理
    std::cout << "\n--- 用途2: for_each 遍历 ---\n";
    auto v2 = nums;
    printVec(v2, "原始");

    // 把所有偶数变成 0
    std::for_each(v2.begin(), v2.end(), [](int& x) {
        if (x % 2 == 0) x = 0;
    });
    printVec(v2, "偶数变0");

    // 用途 3: 条件查找
    std::cout << "\n--- 用途3: find_if 条件查找 ---\n";
    auto it = std::find_if(nums.begin(), nums.end(), [](int x) {
        return x > 5;  // 找第一个大于 5 的数
    });
    if (it != nums.end()) {
        std::cout << "  第一个大于 5 的数是: " << *it << "\n";
    }

    // 用途 4: count_if 条件计数
    int count = std::count_if(nums.begin(), nums.end(), [](int x) {
        return x % 2 == 1;  // 统计奇数个数
    });
    std::cout << "  奇数个数: " << count << "\n";

    // 用途 5: 结合捕获来做条件过滤
    std::cout << "\n--- 用途5: 捕获外部变量做过滤 ---\n";
    int lower = 3, upper = 7;
    auto inRange = [lower, upper](int x) {
        return x >= lower && x <= upper;
    };

    std::cout << "  在 [" << lower << ", " << upper << "] 范围内的数: ";
    for (int x : nums) {
        if (inRange(x)) std::cout << x << " ";
    }
    std::cout << "\n";
}

// ============================================================
// 第四部分: Lambda 的完整语法对比
// ============================================================
void part4_syntax() {
    std::cout << "\n========== 第四部分: 语法总结 ==========\n";

    std::cout << "  完整语法:\n";
    std::cout << "    [捕获](参数列表) -> 返回类型 { 函数体 }\n\n";

    std::cout << "  捕获方式:\n";
    std::cout << "    []      不捕获任何变量\n";
    std::cout << "    [=]     按值捕获所有变量（只读副本）\n";
    std::cout << "    [&]     按引用捕获所有变量（可修改原变量）\n";
    std::cout << "    [x]     只按值捕获 x\n";
    std::cout << "    [&x]    只按引用捕获 x\n";
    std::cout << "    [=, &x] x 按引用，其余按值\n";
    std::cout << "    [&, x]  x 按值，其余按引用\n";
    std::cout << "    mutable 允许修改按值捕获的副本\n\n";

    std::cout <<  "  省略规则:\n";
    std::cout << "    (参数)  可以省略，如果不需要参数的话\n";
    std::cout << "    -> 类型 可以省略，让编译器推断\n\n";

    // 各种写法的对比
    std::cout << "  写法对比:\n";

    // 最简: 无参数，无返回值，无捕获
    [] { std::cout << "    []{}() — 最小 lambda\n"; }();

    // 省略参数
    auto no_param = [] { return 42; };
    std::cout << "    省略参数: " << no_param() << "\n";

    // 省略返回类型
    auto no_return = [](int x) { return x * 2; };
    std::cout << "    省略返回类型: " << no_return(5) << "\n";

    // 全写
    auto full = [](int x) -> int { return x * 2; };
    std::cout << "    全写: " << full(5) << "\n";

    std::cout << "\n  一句话总结:\n";
    std::cout << "    Lambda = 就地写一个临时函数，不用给它起名字\n";
    std::cout << "    最常用于: sort/for_each/find_if 这种需要传\"回调\"的地方\n";
}

int main() {
    std::cout << "=== C++ Lambda 表达式入门 ===\n";

    part1_basic();   // 基本语法
    part2_capture(); // 捕获外部变量
    part3_usage();   // 实际用途
    part4_syntax();  // 语法总结

    std::cout << "\n程序结束。\n";
    return 0;
}
