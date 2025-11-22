#include <unordered_set>
#include <list>

template<typename T>
class LinkedHashSet {
private:
    // 使用一个 dummy 值来复用 LinkedHashMap 的实现
    LinkedHashMap<T, bool> map_;
    static constexpr bool DUMMY_VALUE = true;

public:
    // 添加元素
    void add(const T& element) {
        map_.put(element, DUMMY_VALUE);
    }

    // 删除元素
    void remove(const T& element) {
        map_.remove(element);
    }

    // 检查是否包含元素
    bool contains(const T& element) const {
        return map_.contains(element);
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
        map_.clear();
    }

    // 迭代器支持（按插入顺序遍历）
    auto begin() const { 
        // 创建一个转换迭代器，只返回键
        return map_.begin(); 
    }
    auto end() const { 
        return map_.end(); 
    }

    // 获取所有元素（按插入顺序）
    std::vector<T> toVector() const {
        std::vector<T> result;
        for (const auto& pair : map_) {
            result.push_back(pair.first);
        }
        return result;
    }
};