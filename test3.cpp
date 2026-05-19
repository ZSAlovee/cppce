
#include <iostream>
#include <cstring>
using namespace std;
//sizeof (运算符)： 它是编译器在编译阶段就算好的。它关心的是“这个变量/类型在内存里总共占了多少个坑位（字节）”。

//strlen (函数)： 它是程序在运行阶段执行的。它的逻辑非常死板：从你给它的起始地址开始，一个字节一个字节地往后数，
//直到看见 \0 (ASCII码为0) 才停下来。
int main()
{
    // 1. 字符串字符数组
    char str1[] = "abc";
    cout << "=== char str1[] = \"abc\" ===" << endl;
    cout << "sizeof(str1) = " << sizeof(str1) << endl;   // 4  (含隐含的 '\0')
    cout << "strlen(str1) = " << strlen(str1) << endl;   // 3  (遇到 '\0' 停止计算)
    cout << endl;

    // 2. 手动初始化无 \0
    char str2[3] = {'a', 'b', 'c'};
    cout << "=== char str2[3] = {'a','b','c'} ===" << endl;
    cout << "sizeof(str2) = " << sizeof(str2) << endl;   // 3  (固定分配了3个字节)
    // cout << "strlen(str2) = " << strlen(str2) << endl; // 危险！无结束符'\0'，会导致越界访问，直到在内存中随机碰到一个'\0'为止
    cout << endl;

    // 3. 字符指针（修复点：必须加 const）
    const char *p = "abcd"; 
    cout << "=== const char *p = \"abcd\" ===" << endl;
    cout << "sizeof(p) = " << sizeof(p) << endl;         // 4 或 8 (取决于编译器是32位还是64位，测量的是指针本身的体积)
    cout << "strlen(p) = " << strlen(p) << endl;         // 4
    cout << endl;

    // 4. 固定长度数组，内容短
    char str3[10] = "hi";
    cout << "=== char str3[10] = \"hi\" ===" << endl;
    cout << "sizeof(str3) = " << sizeof(str3) << endl;  // 10 (数组声明的大小)
    cout << "strlen(str3) = " << strlen(str3) << endl;  // 2  (读取了 'h', 'i' 后遇到 '\0' 停止)
    cout << endl;

    // 5. const 字符串引用
    const char *sp = "hello";
    cout << "=== const char *sp = \"hello\" ===" << endl;
    cout << "sizeof(sp) = " << sizeof(sp) << endl;      // 4 或 8 (指针大小)
    cout << "strlen(sp) = " << strlen(sp) << endl;      // 5

    return 0;
}