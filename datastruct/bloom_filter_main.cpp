#include <iostream>
#include <string>
#include "base_bloom_filter.h"
#include "optimized_bloom_filter.h"

int main() {
    // 创建布隆过滤器，预期10000个元素，误判率1%
    BloomFilter bf(10000, 0.01);
    
    std::cout << "位数组大小: " << bf.getSize() << " bits" << std::endl;
    std::cout << "哈希函数个数: " << bf.getNumHashFunctions() << std::endl;
    
    // 添加一些元素
    bf.add("apple");
    bf.add("banana");
    bf.add("cherry");
    
    // 测试存在性
    std::cout << "\n测试存在性:" << std::endl;
    std::cout << "apple: " << (bf.contains("apple") ? "可能存在" : "确定不存在") << std::endl;
    std::cout << "banana: " << (bf.contains("banana") ? "可能存在" : "确定不存在") << std::endl;
    std::cout << "orange: " << (bf.contains("orange") ? "可能存在" : "确定不存在") << std::endl;
    
    // 注意：orange 可能会误判为存在（假阳性），但概率很低（约1%）
    
    return 0;
}