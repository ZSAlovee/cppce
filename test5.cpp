#include <iostream>
using namespace std;

int main()
{
    // ──────────────────────────────────────────────
    // 1. 常量指针 (const pointer) — 指针本身是常量，不可变指向
    //    int* const p = &a;  p 不能指向别处，但 *p 可以改
    // ──────────────────────────────────────────────
    int a = 10, b = 20;
    int* const cp = &a;      // cp 永远指向 a

    *cp = 100;               // 可以改值，a 变成 100
    // cp = &b;              // 编译错误！不能改变指向

    cout << "=== 常量指针 int* const cp ===" << endl;
    cout << "*cp = " << *cp << ", a = " << a << endl;
    cout << endl;

    // ──────────────────────────────────────────────
    // 2. 指针常量 (pointer to const) — 指向常量的指针，所指内容不可变
    //    const int* p = &a;  不能通过 p 修改 a 的值
    // ──────────────────────────────────────────────
    const int* pc = &a;      // pc 可以指向别处，但不能通过 pc 改值
    // *pc = 200;            // 编译错误！不能通过 pc 修改

    pc = &b;                 // 可以改变指向
    cout << "=== 指针常量 const int* pc ===" << endl;
    cout << "*pc = " << *pc << " (指向 b)" << endl;
    cout << endl;

    // ──────────────────────────────────────────────
    // 3. 数组指针 (array pointer) — 指向整个数组的指针
    //    int (*parr)[n]      是一个指针，指向一个长度为 n 的 int 数组
    // ──────────────────────────────────────────────
    int arr[3] = {1, 2, 3};
    int (*parr)[3] = &arr;   // parr 指向整个数组（类型为 int[3]）

    cout << "=== 数组指针 int (*parr)[3] ===" << endl;
    cout << "(*parr)[0] = " << (*parr)[0] << endl;   // 1
    cout << "(*parr)[1] = " << (*parr)[1] << endl;   // 2
    cout << "(*parr)[2] = " << (*parr)[2] << endl;   // 3
    // parr + 1 会跳过整个数组（3 个 int）
    cout << "sizeof(*parr) = " << sizeof(*parr) << " 字节" << endl;  // 12
    cout << endl;

    // ──────────────────────────────────────────────
    // 4. 指针数组 (pointer array) — 数组的每个元素都是指针
    //    int* pary[n]        是一个数组，里面存了 n 个 int*
    // ──────────────────────────────────────────────
    int x = 10, y = 20, z = 30;
    int* pary[3] = {&x, &y, &z};   // 数组，每个元素是一个 int*

    cout << "=== 指针数组 int* pary[3] ===" << endl;
    for (int i = 0; i < 3; i++)
        cout << "*pary[" << i << "] = " << *pary[i] << endl;

    return 0;
}
