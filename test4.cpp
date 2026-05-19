// 大端：高字节放低地址，正序；
// 小端：低字节放低地址，倒序；
// PC 手机是小端，网络协议是大端。
#include <iostream>
using namespace std;

int main()
{
    int num = 0x12345678;
     // 强制按字符指针读取最低地址
    char *p = (char *)&num;

    if (*p == 0x78)
        cout << "xiao" << endl;
    else
        cout << "dadua" << endl;

    return 0;
}