#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>

template<typename K, typename V>
class LinkedHashMap {
private:
    // 双向链表存储键值对，维护插入顺序
    std::list<std::pair<K, V>> order_list_;
    
    // 哈希表：键 -> 链表迭代器的映射
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map_;

public:
    // 插入或更新键值对
    void put(const K& key, const V& value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            // 键已存在，更新值
            it->second->second = value;
            // 可选：将更新的元素移到链表尾部（保持最近更新在尾部）
            // order_list_.splice(order_list_.end(), order_list_, it->second);
        } else {
            // 键不存在，插入新元素到链表尾部
            order_list_.push_back({key, value});
            map_[key] = std::prev(order_list_.end());
        }
    }

    // 获取值，返回指针（nullptr 表示未找到）
    V* get(const K& key) {
        auto it = map_.find(key);
        if (it == map_.end()) {
            return nullptr;
        }
        // 可选：实现访问顺序（LRU策略），将访问的元素移到尾部
        // order_list_.splice(order_list_.end(), order_list_, it->second);
        return &(it->second->second);
    }

    // 删除键值对
    void remove(const K& key) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            order_list_.erase(it->second);
            map_.erase(it);
        }
    }

    // 检查是否包含指定键
    bool contains(const K& key) const {
        return map_.find(key) != map_.end();
    }

    // 获取元素数量
    size_t size() const {
        return map_.size();
    }

    // 检查是否为空
    bool empty() const {
        return map_.empty();
    }

    // 清空所有元素
    void clear() {
        order_list_.clear();
        map_.clear();
    }

    // 迭代器支持（按插入顺序遍历）
    auto begin() const { return order_list_.begin(); }
    auto end() const { return order_list_.end(); }
    
    // 反向迭代器
    auto rbegin() const { return order_list_.rbegin(); }
    auto rend() const { return order_list_.rend(); }

    // 获取所有键（按插入顺序）
    std::vector<K> keys() const {
        std::vector<K> result;
        for (const auto& pair : order_list_) {
            result.push_back(pair.first);
        }
        return result;
    }

    // 获取所有值（按插入顺序）
    std::vector<V> values() const {
        std::vector<V> result;
        for (const auto& pair : order_list_) {
            result.push_back(pair.second);
        }
        return result;
    }
};