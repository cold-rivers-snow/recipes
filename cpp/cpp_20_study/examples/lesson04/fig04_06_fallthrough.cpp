#include <iostream>
using namespace std;

int main() {
    int num = 1;
    switch (num) {
        case 1:
            cout << "处理case 1的前置逻辑\n";
            [[fallthrough]]; // 显式声明穿透（无警告）
        case 2:
            cout << "处理case 1和case 2的通用逻辑\n";
            break; // 终止穿透
        case 3:
            cout << "处理case 3\n";
            // 无break，也无fallthrough → 编译器会报穿透警告
        default:
            cout << "处理默认情况\n";
    }
    return 0;
}

//g++ -std=c++17 fig04_06_fallthrough.cpp -o fig04_06_fallthrough