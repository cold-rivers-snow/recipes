#include <unordered_set>

class SimpleHashSet {
private:
    std::unordered_set<int> set;
    
public:
    void add(int key) {
        set.insert(key);
    }
    
    void remove(int key) {
        set.erase(key);
    }
    
    bool contains(int key) const {
        return set.find(key) != set.end();
    }
    
    size_t size() const {
        return set.size();
    }
    
    bool empty() const {
        return set.empty();
    }
};