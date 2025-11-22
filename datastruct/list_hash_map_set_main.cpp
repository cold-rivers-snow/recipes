#include <iostream>
#include <string>
#include <vector>
#include "list_hashmap.h"
#include "list_hashset.h"
#include "list_hashset_direct.h"

int main() {
    // LinkedHashMap 示例
    std::cout << "=== LinkedHashMap 测试 ===" << std::endl;
    LinkedHashMap<std::string, int> lmap;
    
    lmap.put("apple", 1);
    lmap.put("banana", 2);
    lmap.put("cherry", 3);
    
    // 按插入顺序遍历
    std::cout << "按插入顺序遍历:" << std::endl;
    for (const auto& pair : lmap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    // 更新现有键
    lmap.put("banana", 20);
    std::cout << "\n更新 banana 后:" << std::endl;
    std::cout << "banana: " << *(lmap.get("banana")) << std::endl;
    
    // LinkedHashSet 示例
    std::cout << "\n=== LinkedHashSet 测试 ===" << std::endl;
    LinkedHashSet<std::string> lset;
    
    lset.add("first");
    lset.add("second");
    lset.add("third");
    lset.add("first"); // 重复添加，应该被忽略
    
    std::cout << "按插入顺序遍历 LinkedHashSet:" << std::endl;
    for (const auto& elem : lset.toVector()) {
        std::cout << elem << std::endl;
    }
    
    std::cout << "\nLinkedHashSet 大小: " << lset.size() << std::endl;
    
    return 0;
}