// 面试回答：指针函数是返回指针的函数，函数指针是指向函数的指针。
// 看变量名先和谁结合——先跟()是函数(指针函数)，先跟*是指针(函数指针)。
#include <iostream>
using namespace std;

// ──────────────────────────────────────────────
// 指针函数 — 返回值是指针的函数
//     int* func(...)   函数返回一个 int*
// ──────────────────────────────────────────────

// 指针函数：返回 int 指针
int* getMax(int* a, int* b)
{
    return (*a > *b) ? a : b;   // 返回较大的那个数的地址
}

// 指针函数：返回动态分配的数组
int* makeArray(int size)
{
    int* arr = new int[size];
    for (int i = 0; i < size; i++)
        arr[i] = (i + 1) * 10;
    return arr;                  // 返回数组首地址
}

// ──────────────────────────────────────────────
// 函数指针 — 指向函数的指针变量
//     int (*pf)(int, int)      pf 是一个指向函数的指针
// ──────────────────────────────────────────────

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return (b != 0) ? a / b : 0; }

// ──────────────────────────────────────────────
// 函数指针的典型用途：回调 —— 把函数当作参数传进去
// ──────────────────────────────────────────────
int calc(int a, int b, int (*op)(int, int))
{
    return op(a, b);    // 通过函数指针调用具体运算
}

int main()
{
    cout << "══════════════════════════════════════" << endl;
    cout << "一、指针函数（返回指针的函数）" << endl;
    cout << "══════════════════════════════════════" << endl;

    int x = 50, y = 80;
    int* result = getMax(&x, &y);
    cout << "getMax(&x, &y) = " << *result << endl;     // 80

    int* arr = makeArray(5);
    cout << "makeArray(5) = ";
    for (int i = 0; i < 5; i++)
        cout << arr[i] << " ";                           // 10 20 30 40 50
    cout << endl;
    delete[] arr;

    cout << endl;

    cout << "══════════════════════════════════════" << endl;
    cout << "二、函数指针（指向函数的指针）" << endl;
    cout << "══════════════════════════════════════" << endl;

    // 定义函数指针并赋值
    int (*fp)(int, int);        // 定义一个函数指针
    fp = add;                   // 指向 add 函数
    cout << "fp = add;    fp(6, 4) = " << fp(6, 4) << endl;   // 10

    fp = sub;
    cout << "fp = sub;    fp(6, 4) = " << fp(6, 4) << endl;   // 2

    fp = mul;
    cout << "fp = mul;    fp(6, 4) = " << fp(6, 4) << endl;   // 24

    cout << endl;

    cout << "══════════════════════════════════════" << endl;
    cout << "三、函数指针的用途：回调函数" << endl;
    cout << "══════════════════════════════════════" << endl;

    // 把函数作为参数传入另一个函数
    cout << "calc(10, 5, add) = " << calc(10, 5, add) << endl;   // 15
    cout << "calc(10, 5, sub) = " << calc(10, 5, sub) << endl;   // 5
    cout << "calc(10, 5, mul) = " << calc(10, 5, mul) << endl;   // 50
    cout << "calc(10, 5, divide) = " << calc(10, 5, divide) << endl; // 2

    cout << endl;

    cout << "══════════════════════════════════════" << endl;
    cout << "附：快速区分口诀" << endl;
    cout << "══════════════════════════════════════" << endl;
    cout << "指针函数  int* func()    — 函数返回指针" << endl;
    cout << "函数指针  int (*p)()     — 指针指向函数" << endl;
    cout << "看 函数名/指针名 先和谁结合:" << endl;
    cout << "  func() 先结合 → 函数     → 指针函数   (返回值是指针)" << endl;
    cout << "  (*p)   先结合 → 指针     → 函数指针   (指向函数)" << endl;

    return 0;
}
