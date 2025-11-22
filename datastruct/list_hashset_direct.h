template<typename T>
class LinkedHashSetDirect {
private:
    std::list<T> order_list_;
    std::unordered_set<T> set_;

public:
    void add(const T& element) {
        if (set_.find(element) == set_.end()) {
            order_list_.push_back(element);
            set_.insert(element);
        }
    }

    void remove(const T& element) {
        if (set_.erase(element)) {
            // 需要从链表中删除，但 unordered_set 不提供位置信息
            // 这里需要遍历链表，效率较低
            order_list_.remove(element);
        }
    }

    bool contains(const T& element) const {
        return set_.find(element) != set_.end();
    }

    size_t size() const {
        return set_.size();
    }

    bool empty() const {
        return set_.empty();
    }

    void clear() {
        order_list_.clear();
        set_.clear();
    }

    auto begin() const { return order_list_.begin(); }
    auto end() const { return order_list_.end(); }

    std::vector<T> toVector() const {
        return std::vector<T>(order_list_.begin(), order_list_.end());
    }
};