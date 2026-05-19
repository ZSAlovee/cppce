#include <iostream>

using namespace std;

int main() {
    int a = 10;
    int b = 20;

    // 指针常量（pointer to const）：指针指向的值是常量，不能修改值，但可以改变指向
    // const int* p; 指针指向常量
    const int* p1 = &a;

    cout << "===== 指针常量：const int* p1 =====" << endl;
    cout << "p1 指向 a，*p1 = " << *p1 << endl;

    p1 = &b;
    cout << "p1 现在指向 b，*p1 = " << *p1 << endl;

    // *p1 = 30; // 错误：不能修改常量值

    cout << endl;

    int x = 100;
    int y = 200;

    // 常量指针（const pointer）：指针本身是常量，不能改变指向，但可以修改值
    // int* const p; 常量指针
    int* const p2 = &x;

    cout << "===== 常量指针：int* const p2 =====" << endl;
    cout << "p2 指向 x，*p2 = " << *p2 << endl;

    *p2 = 300;
    cout << "修改 p2 指向的 x 值 = " << x << endl;

    // p2 = &y; // 错误：p2 不能改变指向

    cout << endl;

    // 常量指针常量（const pointer to const）：指针和指向的值都是常量
    const int* const p3 = &y;
    cout << "===== 指针常量和常量指针：const int* const p3 =====" << endl;
    cout << "p3 指向 y，*p3 = " << *p3 << endl;

    // *p3 = 400; // 错误：不能修改常量值
    // p3 = &x;   // 错误：不能改变指向

    return 0;
}
