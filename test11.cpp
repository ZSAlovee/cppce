// C++ 常用容器入门教学
// 编译: g++ -std=c++11 test11.cpp -o test11 && ./test11
//新改动
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

/*
 * ─── 容器是什么？ ───
 * 容器就是"存数据的数据结构"，STL 提供了现成的，不用自己写
 *
 * 常用容器分三类：
 *   序列式容器  — 按顺序存，位置重要
 *      vector  动态数组（最常用）
 *      list    双向链表
 *      deque   双端队列
 *
 *   关联式容器  — 自动排序，查找快
 *      set     集合（不重复，有序）
 *      map     字典（key-value，有序）
 *
 *   无序关联容器 — 哈希表，查找更快
 *      unordered_set   无序集合
 *      unordered_map   无序字典（C++11）
 */

// 工具：打印分隔线
void sep(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

// ============================================================
// 1. vector — 动态数组（最常用）
// ============================================================
// 特点:  连续内存，支持随机访问，尾部插入快
// 适用:  大多数场景，不知道用啥时就用 vector
void demo_vector() {
    sep("vector — 动态数组");

    // 创建
    std::vector<int> v;          // 空的
    std::vector<int> v2(5, 10);  // 5 个 10

    // 添加元素
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);

    std::cout << "  尾部添加后: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // 访问元素
    std::cout << "  v[1] = " << v[1] << "\n";           // 下标访问
    std::cout << "  v.at(2) = " << v.at(2) << "\n";      // 带边界检查
    std::cout << "  首元素: " << v.front() << "\n";
    std::cout << "  尾元素: " << v.back() << "\n";

    // 删除元素
    v.pop_back();  // 删除尾部
    std::cout << "  pop_back 后: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // 插入/删除中间（效率低，因为要移动元素）
    v.insert(v.begin() + 1, 99);  // 在下标1位置插入
    std::cout <<  "  在下标1插入99: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // 大小和容量
    std::cout << "  size = " << v.size() << "\n";
    std::cout << "  empty = " << (v.empty() ? "是" : "否") << "\n";

    // 遍历方式
    std::cout << "  下标遍历: ";
    for (size_t i = 0; i < v.size(); ++i) std::cout << v[i] << " ";

    std::cout << "\n  迭代器遍历: ";
    for (auto it = v.begin(); it != v.end(); ++it) std::cout << *it << " ";

    std::cout << "\n  范围for:     ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "  特点: 连续内存，随机访问 O(1)，尾部插入 O(1)，中间插入 O(n)\n";
}

// ============================================================
// 2. list — 双向链表
// ============================================================
// 特点: 不连续内存，任意位置插入/删除快，不支持随机访问
// 适用: 需要频繁在中间插入/删除
void demo_list() {
    sep("list — 双向链表");

    std::list<int> l;

    // 添加
    l.push_back(20);
    l.push_back(30);
    l.push_front(10);   // 头部插入
    l.push_back(40);

    std::cout <<  "  初始: ";
    for (int x : l) std::cout << x << " ";
    std::cout << "\n";

    // 删除
    l.pop_front();      // 删除头部
    std::cout <<  "  pop_front 后: ";
    for (int x : l) std::cout << x << " ";
    std::cout << "\n";

    // 中间插入 (list 插入不需要移动元素)
    auto it = l.begin();
    ++it; ++it;  // 移动到第3个位置
    l.insert(it, 99);
    std::cout << "  中间插入99后: ";
    for (int x : l) std::cout << x << " ";
    std::cout << "\n";

    // list 不能随机访问!
    // int x = l[2];  // 编译错误！list 不支持下标

    std::cout << "  特点: 双向链表，任意位置插入/删除 O(1)，不能下标访问\n";
}

// ============================================================
// 3. deque — 双端队列
// ============================================================
// 特点: 像 vector 但头尾插入都快
// 适用: 需要频繁在头尾操作
void demo_deque() {
    sep("deque —- 双端队列");

    std::deque<int> d;

    d.push_back(20);
    d.push_back(30);
    d.push_front(10);
    d.push_front(5);

    std::cout << "  初始: ";
    for (int x : d) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "  支持下标访问: d[2] = " << d[2] << "\n";
    std::cout << "  首: " << d.front() << ", 尾: " << d.back() << "\n";

    d.pop_front();
    std::cout << "  pop_front 后: ";
    for (int x : d) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "  特点: 头尾插入/删除 O(1)，支持下标，比 vector 多了头插\n";
}

// ============================================================
// 4. set — 集合（自动排序，不重复）
// ============================================================
void demo_set() {
    sep("set — 有序集合");

    std::set<int> s;

    // 插入（自动排序，重复的不会插入）
    s.insert(50);
    s.insert(10);
    s.insert(30);
    s.insert(20);
    s.insert(40);
    s.insert(30);  // 重复，被忽略

    std::cout << "  内容（自动升序，无重复）: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // 查找（快！O(log n)）
    if (s.find(30) != s.end()) {
        std::cout << "  30 在集合中\n";
    }
    if (s.find(99) == s.end()) {
        std::cout << "  99 不在集合中\n";
    }

    // count 也能判断是否存在（set 里要么 0 要么 1）
    std::cout << "  20 出现 " << s.count(20) << " 次\n";

    // 删除
    s.erase(20);
    std::cout << "  删除20后: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "  特点: 自动排序，不重复，查找 O(log n)\n";
}

// ============================================================
// 5. map — 字典（key-value，自动排序）
// ============================================================
void demo_map() {
    sep("map — 有序字典");

    std::map<std::string, int> scores;

    // 插入
    scores["张三"] = 95;
    scores["李四"] = 87;
    scores["王五"] = 92;
    scores.insert(std::make_pair("赵六", 78));

    // 遍历（按键的字母序）
    std::cout << "  所有成绩（按名字排序）:\n";
    for (const auto& pair : scores) {
        std::cout << "    " << pair.first << ": " << pair.second << "\n";
    }

    // 访问
    std::cout << "\n  张三的成绩: " << scores["张三"] << "\n";

    // 查找
    auto it = scores.find("李四");
    if (it != scores.end()) {
        std::cout << "  李四的成绩: " << it->second << "\n";
    }

    // 检查 key 是否存在
    if (scores.count("王五") > 0) {
        std::cout << "  王五在字典里\n";
    }

    // 删除
    scores.erase("赵六");
    std::cout << "  删除赵六后，剩余 " << scores.size() << " 人\n";

    std::cout << "  特点: key-value 键值对，按键排序，查找 O(log n)\n";
}

// ============================================================
// 6. unordered_set / unordered_map — 哈希版（C++11）
// ============================================================
void demo_unordered() {
    sep("unordered_set / unordered_map — 无序（哈希）");

    // unordered_set: 不排序，但查找更快
    std::unordered_set<int> us;
    us.insert(50);
    us.insert(10);
    us.insert(30);
    us.insert(20);
    us.insert(40);

    std::cout << "  unordered_set（无序）: ";
    for (int x : us) std::cout << x << " ";
    std::cout << "\n";

    // unordered_map
    std::unordered_map<std::string, int> ages;
    ages["张三"] = 25;
    ages["李四"] = 30;
    ages["王五"] = 28;

    std::cout << "\n  unordered_map（无序）:\n";
    for (const auto& p : ages) {
        std::cout << "    " << p.first << ": " << p.second << "\n";
    }

    std::cout << "\n  特点: 哈希表，查找 O(1)，但无序\n";
    std::cout << "  什么时候用? 只管查找快不快，不管排不排序\n";
}

// ============================================================
// 7. 容器选择指南
// ============================================================
void demo_guide() {
    sep("★ 容器选择指南");

    std::cout << R"(
  场景                                  推荐容器
  ───────────────────────────────────── ────────────────
  不知道用啥，就存一堆数，下标访问       vector（首选）
  经常头尾插入/删除                     deque
  频繁在中间插入/删除                   list
  元素不重复，自动排序                   set
  存 key-value，自动排序                 map
  只要查找快，不关心顺序                 unordered_set/map

  总结一句话：
    默认用 vector，需要 key-value 用 map
    需要去重/查找用 set，要更快用 unordered_xxx
)";
}

int main() {
    std::cout << "=== C++ 常用容器入门 ===\n";

    demo_vector();
    demo_list();
    demo_deque();
    demo_set();
    demo_map();
    demo_unordered();
    demo_guide();

    std::cout << "\n程序结束。\n";
    return 0;
}
