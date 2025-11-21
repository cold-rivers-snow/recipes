#include <vector>
#include <list>
#include <functional>
#include <utility>

template <typename T, 
          typename Hash = std::hash<T>,
          typename Equal = std::equal_to<T>>
class MyHashSet {
private:
    static constexpr double LOAD_FACTOR = 0.75;
    size_t capacity;
    size_t size;
    std::vector<std::list<T>> buckets;
    Hash hashFunc;
    Equal equalFunc;

    size_t hash(const T& key) const {
        return hashFunc(key) % capacity;
    }

    void resize() {
        std::vector<std::list<T>> oldBuckets = std::move(buckets);
        
        // 扩大容量
        capacity *= 2;
        buckets.resize(capacity);
        
        // 重置大小
        size = 0;
        
        // 重新哈希所有元素
        for (const auto& bucket : oldBuckets) {
            for (const auto& key : bucket) {
                insert(key);
            }
        }
    }

public:
    explicit MyHashSet(size_t initialCapacity = 16, 
                      const Hash& hash = Hash(), 
                      const Equal& equal = Equal())
        : capacity(initialCapacity), size(0), 
          buckets(initialCapacity), hashFunc(hash), equalFunc(equal) {}
    
    void insert(const T& key) {
        if (contains(key)) return;
        
        // 检查是否需要扩容
        if (size >= capacity * LOAD_FACTOR) {
            resize();
        }
        
        size_t index = hash(key);
        buckets[index].push_back(key);
        size++;
    }
    
    void erase(const T& key) {
        if (!contains(key)) return;
        
        size_t index = hash(key);
        auto& bucket = buckets[index];
        
        // 使用equalFunc比较元素
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (equalFunc(*it, key)) {
                bucket.erase(it);
                size--;
                break;
            }
        }
    }
    
    bool contains(const T& key) const {
        size_t index = hash(key);
        const auto& bucket = buckets[index];
        
        for (const auto& k : bucket) {
            if (equalFunc(k, key)) {
                return true;
            }
        }
        return false;
    }
    
    size_t getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
};