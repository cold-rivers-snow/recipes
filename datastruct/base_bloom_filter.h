#include <vector>
#include <functional>
#include <cmath>

class BloomFilter {
private:
    std::vector<bool> bitArray;  // 位数组
    size_t numHashFunctions;     // 哈希函数个数
    
    // 多个哈希函数的实现
    size_t hash1(const std::string& key) const {
        std::hash<std::string> hasher;
        return hasher(key) % bitArray.size();
    }
    
    size_t hash2(const std::string& key) const {
        // 使用不同的哈希策略
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % bitArray.size();
    }
    
    // 生成第i个哈希值
    size_t getHash(const std::string& key, size_t i) const {
        if (i == 0) return hash1(key);
        else return (hash1(key) + i * hash2(key)) % bitArray.size();
    }

public:
    // 构造函数：根据预期元素数量和误判率计算最优参数
    BloomFilter(size_t expectedElements, double falsePositiveRate = 0.01) {
        // 计算位数组大小 m = -n * ln(p) / (ln(2))^2
        // 计算位数组大小 m = -n * ln(p) / (ln(2))^2
        // 注意：必须先将 expectedElements 转为 double，否则 -expectedElements (unsigned) 会溢出
        size_t m = static_cast<size_t>(
            -static_cast<double>(expectedElements) * std::log(falsePositiveRate) / 
            (std::log(2) * std::log(2))
        );
        
        if (m == 0) m = 1; // 避免大小为0
        
        // 计算哈希函数个数 k = (m/n) * ln(2)
        numHashFunctions = static_cast<size_t>(
            (static_cast<double>(m) / expectedElements) * std::log(2)
        );
        
        // 确保至少有1个哈希函数
        if (numHashFunctions == 0) numHashFunctions = 1;
        
        bitArray.resize(m, false);
    }
    
    // 添加元素
    void add(const std::string& key) {
        for (size_t i = 0; i < numHashFunctions; ++i) {
            size_t index = getHash(key, i);
            bitArray[index] = true;
        }
    }
    
    // 检查元素是否存在
    bool contains(const std::string& key) const {
        for (size_t i = 0; i < numHashFunctions; ++i) {
            size_t index = getHash(key, i);
            if (!bitArray[index]) {
                return false; // 确定不存在
            }
        }
        return true; // 可能存在（可能有误判）
    }
    
    // 获取位数组大小
    size_t getSize() const {
        return bitArray.size();
    }
    
    // 获取哈希函数个数
    size_t getNumHashFunctions() const {
        return numHashFunctions;
    }
};