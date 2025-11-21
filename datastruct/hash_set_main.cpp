#include <iostream>
#include <string>
#include "myhashset.h"
#include "LinearProbingHashSet.h"
#include "StdUnorderSet.h"

int main() {
    // 使用自定义哈希集合
    MyHashSet<int> intSet;
    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);
    
    std::cout << "Contains 2: " << (intSet.contains(2) ? "Yes" : "No") << std::endl;
    intSet.erase(2);
    std::cout << "After erase, contains 2: " << (intSet.contains(2) ? "Yes" : "No") << std::endl;
    
    // 存储字符串
    MyHashSet<std::string> stringSet;
    stringSet.insert("hello");
    stringSet.insert("world");
    std::cout << "Contains 'hello': " << (stringSet.contains("hello") ? "Yes" : "No") << std::endl;
    
    // 使用标准库版本
    SimpleHashSet simpleSet;
    simpleSet.add(10);
    simpleSet.add(20);
    std::cout << "Simple set contains 10: " << (simpleSet.contains(10) ? "Yes" : "No") << std::endl;
    
    return 0;
}