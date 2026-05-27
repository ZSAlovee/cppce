// C++ 智能指针入门教学
// 编译: g++ -std=c++11 test8.cpp -o test8 && ./test8

#include <iostream>
#include <memory>  // 智能指针都在这个头文件里

/*
 * ─── 为什么要用智能指针？ ───
 * 普通指针: 用完了要自己 delete，忘记 delete 就内存泄漏
 * 智能指针: 自动帮你 delete，不需要手动释放
 *
 * 有三种智能指针:
 *   unique_ptr  → 独享所有权（不能复制，只能移动）
 *   shared_ptr  → 共享所有权（复制时引用计数+1）
 *   weak_ptr    → 旁观者（不影响引用计数）
 */

// 一个简单的类，用来观察对象的创建和销毁
struct Person {
    std::string name;
    Person(const std::string& n) : name(n) {
        std::cout << "  创建: " << name << "\n";
    }
    ~Person() {
        std::cout << "  销毁: " << name << "\n";
    }
    void say(const std::string& msg) const {
        std::cout << "  " << name << " 说: " << msg << "\n";
    }
};

// ============================================================
// 第一部分: unique_ptr — 独享所有权
// ============================================================
// 特点:
//   - 一个对象只能被一个 unique_ptr 拥有
//   - 不能拷贝，只能移动（std::move）
//   - 出作用域自动释放
//   - 零额外开销（和裸指针一样快）
void part1_unique_ptr() {
    std::cout << "\n========== 第一部分: unique_ptr ==========\n";

    // 示例 1: 基本用法
    std::cout << "--- 示例1: 自动释放 ---\n";
    {
        std::unique_ptr<Person> p(new Person("张三"));  // 创建智能指针
        p->say("你好!");
        // 不需要 delete，出大括号自动释放
    }
    std::cout << "  出了大括号，张三自动被销毁了\n";

    // 示例 2: unique_ptr 不能复制
    std::cout << "\n--- 示例2: 不能复制，只能移动 ---\n";
    {
        std::unique_ptr<Person> p1(new Person("李四"));

        // std::unique_ptr<Person> p2 = p1;  ← 编译错误！不能复制

        std::unique_ptr<Person> p2 = std::move(p1);  // 移动: 把所有权转给 p2
        // 现在 p1 空了，p2 拥有对象
        if (p1 == nullptr) {
            std::cout << "  p1 已经是空指针了\n";
        }
        p2->say("我的新主人是 p2");
        // p2 出大括号时释放
    }

    // 示例 3: 用普通指针和 unique_ptr 对比
    std::cout << "\n--- 示例3: 对比普通指针 ---\n";
    {
        std::cout << "  普通指针:\n";
        Person* raw = new Person("王五");
        raw->say("需要手动 delete");
        delete raw;  // 万一忘记写这行，就内存泄漏了

        std::cout << "  unique_ptr:\n";
        std::unique_ptr<Person> sp(new Person("赵六"));
        sp->say("不用管我，自动释放");
        // 不需要 delete
    }
}

// ============================================================
// 第二部分: shared_ptr — 共享所有权
// ============================================================
// 特点:
//   - 多个 shared_ptr 可以指向同一个对象
//   - 内部有"引用计数"，每复制一次 +1
//   - 最后一个 shared_ptr 销毁时，对象才被释放
void part2_shared_ptr() {
    std::cout << "\n========== 第二部分: shared_ptr ==========\n";

    // 示例 1: 多个 shared_ptr 共享一个对象
    std::cout << "--- 示例1: 共享所有权 ---\n";
    {
        std::shared_ptr<Person> p1(new Person("小明"));
        std::cout << "  引用计数: " << p1.use_count() << "\n";  // 1

        {
            std::shared_ptr<Person> p2 = p1;  // 复制，引用计数 +1
            std::shared_ptr<Person> p3 = p1;  // 复制，引用计数 +1
            std::cout << "  复制后引用计数: " << p1.use_count() << "\n";  // 3

            p2->say("我是 p2");
            p3->say("我是 p3");
        }  // p2, p3 销毁，引用计数 -2

        std::cout << "  p2/p3 销毁后引用计数: " << p1.use_count() << "\n";  // 1
    }  // p1 销毁，引用计数变 0，小明被释放

    // 示例 2: shared_ptr 的坑 — 循环引用
    std::cout << "\n--- 示例2: 循环引用（常见坑）---\n";
    struct Dad;
    struct Son;

    struct Dad {
        std::shared_ptr<Son> son;  // 爸爸有儿子
        ~Dad() { std::cout << "  ~Dad()\n"; }
    };
    struct Son {
        std::shared_ptr<Dad> dad;  // 儿子有爸爸（有问题！）
        ~Son() { std::cout << "  ~Son()\n"; }
    };

    {
        auto dad = std::make_shared<Dad>();
        auto son = std::make_shared<Son>();
        dad->son = son; 
        son->dad = dad;  // ← 循环引用！
        std::cout << "  dad.use_count = " << dad.use_count() << "\n";  // 2
        std::cout << "  son.use_count = " << son.use_count() << "\n";  // 2
        std::cout << "  注意: 出大括号后不会打印 ~Dad() 和 ~Son()，内存泄漏了!\n";
    }  // 这里不会析构！因为互相引用，计数永远到不了 0
    std::cout << "  果然没有销毁...\n";
    std::cout << "  解决方法: 把其中一方的 shared_ptr 改成 weak_ptr（看第三部分）\n";
}

// ============================================================
// 第三部分: weak_ptr — 旁观者
// ============================================================
// 特点:
//   - 不影响 shared_ptr 的引用计数
//   - 只能从 shared_ptr 创建
//   - 不能直接访问对象，要用 lock() 获得临时 shared_ptr
//   - 用来解决 shared_ptr 循环引用问题
void part3_weak_ptr() {
    std::cout << "\n========== 第三部分: weak_ptr ==========\n";

    // 示例 1: 解决循环引用
    std::cout << "--- 示例1: 用 weak_ptr 修复循环引用 ---\n";
    struct Dad;
    struct Son;

    struct Dad {
        std::shared_ptr<Son> son;
        ~Dad() { std::cout << "  ~Dad()\n"; }
    };
    struct Son {
        std::weak_ptr<Dad> dad;  // 改成 weak_ptr！不增加引用计数
        ~Son() { std::cout << "  ~Son()\n"; }
    };

    {
        auto dad = std::make_shared<Dad>();
        auto son = std::make_shared<Son>();
        dad->son = son;
        son->dad = dad;  // weak_ptr，不增加计数
        std::cout << "  dad.use_count = " << dad.use_count() << "\n";  // 1（不是2了）
    }  // 正常析构！
    std::cout << "  父子都正常销毁了，没有泄漏\n";

    // 示例 2: weak_ptr 的使用方式
    std::cout << "\n--- 示例2: lock() 和 expired() ---\n";
    {
        std::shared_ptr<Person> sp(new Person("小红"));
        std::weak_ptr<Person> wp = sp;  // 从 shared_ptr 创建 weak_ptr

        // 通过 lock() 获得 shared_ptr 才能访问对象
        if (auto temp = wp.lock()) {  // lock() 返回 shared_ptr
            temp->say("通过 weak_ptr 访问到了我");
        }

        sp.reset();  // 释放 shared_ptr 的所有权
        std::cout << "  sp.reset() 后，对象被销毁\n";

        if (wp.expired()) {  // expired() 检查对象是否还存在
            std::cout << "  weak_ptr 过期了，对象已经没了\n";
        }

        auto temp = wp.lock();
        if (temp == nullptr) {
            std::cout << "  lock() 返回空指针，无法访问\n";
        }
    }
}

// ============================================================
// 第四部分: 实际中用哪个？
// ============================================================
void part4_guide() {
    std::cout << "\n========== 总结: 该用哪个？ ==========\n";
    std::cout << "\n";
    std::cout << "  场景                            | 用哪个\n";
    std::cout << "  ─────────────────────────────────┼────────────\n";
    std::cout << "  一个对象只有一个主人            | unique_ptr\n";
    std::cout << "  多个地方需要共享同一个对象      | shared_ptr\n";
    std::cout << "  想引用对象但不拥有它（打破循环）| weak_ptr\n";
    std::cout << "\n";
    std::cout << "  总原则: 默认用 unique_ptr\n";
    std::cout << "          真的需要共享才用 shared_ptr\n";
    std::cout << "          weak_ptr 专门解决 shared_ptr 的循环引用\n";
}

int main() {
    std::cout << "=== C++ 智能指针入门 ===\n";

    part1_unique_ptr();   // unique_ptr 独享
    part2_shared_ptr();   // shared_ptr 共享
    part3_weak_ptr();     // weak_ptr 旁观
    part4_guide();        // 选择指南

    std::cout << "\n程序结束，所有对象已清理。\n";
    return 0;
}
