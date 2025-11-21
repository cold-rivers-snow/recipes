#include <vector>
#include <optional>
#include <functional>

template <typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>>
class LinearProbingHashSet {
private:
    struct Node {
        T value;
        bool deleted;
        Node(const T& v) : value(v), deleted(false) {}
    };

    std::vector<std::optional<Node>> table;
    size_t size;
    static const size_t DEFAULT_CAPACITY = 16;
    static constexpr double LOAD_FACTOR = 0.75;
    Hash hashFunc;
    Equal equalFunc;

    size_t hash(const T& key) const {
        return hashFunc(key) % table.size();
    }

    int find(const T& key) const {
        size_t idx = hash(key);
        while (table[idx]) {
            if (!table[idx]->deleted && equalFunc(table[idx]->value, key)) {
                return static_cast<int>(idx);
            }
            idx = (idx + 1) % table.size();
        }
        return static_cast<int>(idx);
    }

    void rehash() {
        auto old_table = std::move(table);
        table.resize(old_table.size() * 2);
        size = 0;

        for (auto& node : old_table) {
            if (node && !node->deleted) {
                insert(node->value);
            }
        }
    }

public:
    LinearProbingHashSet(const Hash& hash = Hash(), const Equal& equal = Equal())
        : table(DEFAULT_CAPACITY), size(0), hashFunc(hash), equalFunc(equal) {}

    void insert(const T& value) {
        if (size >= table.size() * LOAD_FACTOR) {
            rehash();
        }

        int idx = find(value);
        if (!table[idx]) {
            table[idx] = Node(value);
            size++;
        } else if (table[idx]->deleted) {
            table[idx]->value = value;
            table[idx]->deleted = false;
            size++;
        }
        // 如果已存在，不执行任何操作
    }

    void erase(const T& value) {
        int idx = find(value);
        if (table[idx] && !table[idx]->deleted) {
            table[idx]->deleted = true;
            size--;
        }
    }

    bool contains(const T& value) const {
        int idx = find(value);
        return (table[idx] && !table[idx]->deleted);
    }

    size_t getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
};