// C++ 虚函数入门教学
// 编译: g++ -std=c++11 test10.cpp -o test10 && ./test10

#include <iostream>
#include <vector>

/*
 * ─── 虚函数是什么？ ───
 * 虚函数 = 让子类"覆盖"父类的方法
 * 当用父类指针/引用调用时，实际执行的是子类的方法（多态）
 *
 * 核心:
 *   virtual     → 声明虚函数
 *   override    → 明确告诉编译器"我要覆盖父类的方法"
 *   纯虚函数    → = 0，没有实现，子类必须实现（类似 Java 接口）
 */

// ============================================================
// 第一部分: 没有虚函数会怎样？
// ============================================================
void part1_no_virtual() {
    std::cout << "========== 第一部分: 没有虚函数 ==========\n";

    struct Animal {
        void speak() {  // 普通函数
            std::cout << "  Animal: 发出声音\n";
        }
    };

    struct Dog : Animal {
        void speak() {
            std::cout << "  Dog: 汪汪!\n";
        }
    };

    // 直接用子类对象，没问题
    Dog dog;
    dog.speak();  // 调用 Dog::speak()

    // 用父类指针指向子类 —— 问题来了！
    Animal* p = new Dog();
    p->speak();   // 调的是 Animal::speak()，不是 Dog::speak()！
    std::cout << "  普通函数：父类指针调不到子类的方法\n";
    delete p;
}

// ============================================================
// 第二部分: 用了虚函数之后
// ============================================================
void part2_virtual() {
    std::cout << "\n========== 第二部分: 加了 virtual ==========\n";

    struct Animal {
        virtual void speak() {  // 加上 virtual！
            std::cout << "  Animal: 发出声音\n";
        }
    };

    struct Dog : Animal {
        void speak() override {  // override 表示"覆盖父类方法"（可省略，但推荐写）
            std::cout << "  Dog: 汪汪!\n";
        }
    };

    struct Cat : Animal {
        void speak() override {
            std::cout << "  Cat: 喵喵~\n";
        }
    };

    // 父类指针调用 —— 现在调的是子类的方法！
    Animal* p1 = new Dog();
    Animal* p2 = new Cat();
    p1->speak();  // Dog::speak()
    p2->speak();  // Cat::speak()
    delete p1;
    delete p2;

    // 父类引用也是一样的效果
    std::cout << "\n  父类引用也一样:\n";
    Dog dog;
    Cat cat;
    Animal& ref1 = dog;
    Animal& ref2 = cat;
    ref1.speak();  // Dog::speak()
    ref2.speak();  // Cat::speak()

    std::cout << "\n  这就是多态：同样的 speak() 调用，表现出不同的行为\n";
}

// ============================================================
// 第三部分: 纯虚函数 & 抽象类
// ============================================================
void part3_pure_virtual() {
    std::cout << "\n========== 第三部分: 纯虚函数 ==========\n";

    // 纯虚函数 = 在父类里不实现，强制子类去实现
    // 包含纯虚函数的类 = 抽象类，不能直接创建对象

    struct Shape {  // 抽象类
        virtual double area() const = 0;  // 纯虚函数
        virtual ~Shape() {}              // 虚析构（后面会讲）
    };

    struct Circle : Shape {
        double r;
        Circle(double r) : r(r) {}
        double area() const override {
            return 3.14159 * r * r;
        }
    };

    struct Rect : Shape {
        double w, h;
        Rect(double w, double h) : w(w), h(h) {}
        double area() const override {
            return w * h;
        }
    };

    // Shape s;  // 编译错误！抽象类不能实例化

    Shape* s1 = new Circle(5);
    Shape* s2 = new Rect(4, 6);

    std::cout << "  圆形(半径5) 面积: " << s1->area() << "\n";
    std::cout << "  矩形(4x6)   面积: " << s2->area() << "\n";

    // 多态的威力：统一处理不同子类
    std::cout << "\n  多态数组：\n";
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(3));
    shapes.push_back(new Rect(2, 8));
    shapes.push_back(new Circle(10));

    double total = 0;
    for (Shape* s : shapes) {
        total += s->area();
    }
    std::cout << "  所有形状总面积: " << total << "\n";

    for (Shape* s : shapes) delete s;
}

// ============================================================
// 第四部分: 虚析构函数
// ============================================================
void part4_virtual_destructor() {
    std::cout << "\n========== 第四部分: 虚析构函数 ==========\n";

    struct Base {
        ~Base() {  // 非虚析构
            std::cout << "  ~Base()\n";
        }
    };

    struct Derived : Base {
        int* data;
        Derived() { data = new int[100]; std::cout << "  Derived: 申请了内存\n"; }
        ~Derived() {
            delete[] data;
            std::cout << "  ~Derived(): 释放了内存\n";
        }
    };

    Base* p = new Derived();
    delete p;  // 只调了 ~Base()！没调 ~Derived()！内存泄漏！
    std::cout << "  问题: 只调了 ~Base()，~Derived() 没执行，内存泄漏！\n";

    // 解决方法：把析构函数声明为 virtual
    std::cout << "\n  修正：加上 virtual 析构：\n";
    struct Base2 {
        virtual ~Base2() {
            std::cout << "  ~Base2()\n";
        }
    };

    struct Derived2 : Base2 {
        ~Derived2() override {
            std::cout << "  ~Derived2(): 正确释放\n";
        }
    };

    Base2* p2 = new Derived2();
    delete p2;  // 先 ~Derived2()，再 ~Base2()
    std::cout << "  virtual 析构: 子类析构先执行，再执行父类析构\n";

    std::cout << "\n  一句话: 如果类可能被继承，析构函数加 virtual\n";
}

// ============================================================
// 第五部分: 总结
// ============================================================
void part5_summary() {
    std::cout << "\n========== 总结 ==========\n";

    std::cout << "\n  virtual       — 让子类能覆盖父类的方法\n";
    std::cout << "  override      — 明确表示「我要覆盖父类方法」，写错编译器会报错\n";
    std::cout << "  纯虚函数 = 0  — 父类不实现，子类必须实现\n";
    std::cout << "  抽象类        — 有纯虚函数的类，不能直接 new\n";
    std::cout << "  虚析构        — 父类析构加 virtual，保证子类正确释放\n";

    std::cout << "\n  多态说白了就是一句话:\n";
    std::cout << "  「同样的代码，不同的对象，产生不同的行为」\n";
}

int main() {
    std::cout << "=== C++ 虚函数入门 ===\n";

    part1_no_virtual();    // 没有 virtual 会怎样
    part2_virtual();       // 有了 virtual 之后
    part3_pure_virtual();  // 纯虚函数 / 抽象类
    part4_virtual_destructor();  // 虚析构
    part5_summary();       // 总结

    std::cout << "\n程序结束。\n";
    return 0;
}
//ok