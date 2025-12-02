#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

template<typename K, typename V>
class ArrayHashMap {
private:
    std::vector<K> keys;                    // 存储所有 key（紧凑）
    std::vector<V> values;                  // 存储对应 value
    std::unordered_map<K, int> indexMap;    // key -> 在 keys 中的索引
    int size;

    // 用最后一个元素覆盖指定位置，并删除最后一个元素
    void swapAndPopLast(int idx) {
        if (idx >= static_cast<int>(keys.size())) return;

        // 获取最后一个元素
        K lastKey = keys.back();
        V lastVal = values.back();

        // 覆盖要删除的位置
        keys[idx] = lastKey;
        values[idx] = lastVal;

        // 更新最后一个元素在 hash map 中的索引
        indexMap[lastKey] = idx;

        // 删除最后一个元素
        keys.pop_back();
        values.pop_back();
        indexMap.erase(lastKey);
    }

public:
    ArrayHashMap() : size(0) {
        srand(time(nullptr)); // 初始化随机种子
    }

    void put(const K& key, const V& value) {
        auto it = indexMap.find(key);
        if (it != indexMap.end()) {
            // 已存在，更新 value
            int idx = it->second;
            values[idx] = value;
        } else {
            // 新增
            keys.push_back(key);
            values.push_back(value);
            indexMap[key] = size;
            size++;
        }
    }

    V get(const K& key) {
        auto it = indexMap.find(key);
        if (it == indexMap.end()) {
            throw std::out_of_range("Key not found");
        }
        int idx = it->second;
        return values[idx];
    }

    bool containsKey(const K& key) const {
        return indexMap.find(key) != indexMap.end();
    }

    void remove(const K& key) {
        auto it = indexMap.find(key);
        if (it == indexMap.end()) return;

        int idx = it->second;
        swapAndPopLast(idx);
        size--;
    }

    K randomKey() {
        if (size == 0) {
            throw std::out_of_range("Cannot call randomKey on empty map");
        }

        // 生成 [0, size) 的随机索引
        int randomIndex = rand() % size;
        return keys[randomIndex];
    }

    int getSize() const { return size; }

    void print() const {
        std::cout << "{ ";
        for (int i = 0; i < size; ++i) {
            std::cout << keys[i] << ": " << values[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << " }" << std::endl;
    }
};

int main() {
    ArrayHashMap<std::string, int> map;

    // 添加数据
    map.put("apple", 10);
    map.put("banana", 20);
    map.put("cherry", 30);
    map.put("date", 40);
    map.put("elderberry", 50);

    std::cout << "Map: ";
    map.print();

    // 测试 randomKey 多次
    std::cout << "\nRandom Keys (10 times):" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << map.randomKey() << " ";
    }
    std::cout << std::endl;

    // 删除一个 key 后再随机
    std::cout << "\nRemoving 'banana'..." << std::endl;
    map.remove("banana");

    std::cout << "Map after remove: ";
    map.print();

    std::cout << "Random Keys after remove:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << map.randomKey() << " ";
    }
    std::cout << std::endl;

    return 0;
}