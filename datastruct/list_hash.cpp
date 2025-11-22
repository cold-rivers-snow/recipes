#include <vector>
#include <iostream>

// 链表节点结构
struct Node {
    int key;
    int value;
    Node* next;

    Node(int k, int v) : key(k), value(v), next(nullptr) {}
};

class MyHashMap {
private:
    // 底层是一个 Node 指针的数组（即桶数组）
    std::vector<Node*> table;
    // 固定容量，可以根据需要调整
    static const int CAPACITY = 10000;

    int hash(int key) {
        return key % CAPACITY;
    }

public:
    MyHashMap() : table(CAPACITY, nullptr) {}

    // 释放所有动态分配的内存，防止内存泄漏
    ~MyHashMap() {
        for (Node* bucketHead : table) {
            Node* current = bucketHead;
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
    }

    // 插入或更新键值对
    void put(int key, int value) {
        int idx = hash(key); 

        // 1. 遍历链表，检查 key 是否已存在
        Node* current = table[idx];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; // 更新 value
                return;
            }
            current = current->next;
        }

        // 2. key 不存在，创建新节点并插入到链表头部（头插法）
        Node* newNode = new Node(key, value);
        newNode->next = table[idx]; // 新节点的 next 指向原链表头
        table[idx] = newNode;       // 桶指向新节点
    }

    int get(int key) {
        int idx = hash(key);
        Node* current = table[idx];

        // 遍历链表查找 key
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return -1; // 未找到
    }

    void remove(int key) {
        int idx = hash(key);
        Node* head = table[idx];

        // 处理空链表的情况
        if (head == nullptr) return;

        // 1. 如果要删除的是头节点
        if (head->key == key) {
            table[idx] = head->next; // 桶指向下一个节点
            delete head;
            return;
        }

        // 2. 遍历链表，寻找目标节点的前驱节点
        Node* prev = head;
        while (prev->next != nullptr) {
            if (prev->next->key == key) {
                Node* toDelete = prev->next;
                prev->next = toDelete->next; // 跳过目标节点，不能之间next->next,赋值，需要释放内存
                delete toDelete;
                return;
            }
            prev = prev->next;
        }
    }
};

int main() {
    MyHashMap myHashMap;

    std::cout << "=== MyHashMap 测试开始 ===" << std::endl;

    // 1. 测试 put 和 get
    std::cout << "\n[测试 1] 插入和获取 (Put & Get):" << std::endl;
    myHashMap.put(1, 1);
    myHashMap.put(2, 2);
    std::cout << "put(1, 1), put(2, 2)" << std::endl;
    std::cout << "get(1): " << myHashMap.get(1) << " (预期: 1)" << std::endl;
    std::cout << "get(2): " << myHashMap.get(2) << " (预期: 2)" << std::endl;

    // 2. 测试更新值
    std::cout << "\n[测试 2] 更新值 (Update):" << std::endl;
    myHashMap.put(2, 100);
    std::cout << "put(2, 100)" << std::endl;
    std::cout << "get(2): " << myHashMap.get(2) << " (预期: 100)" << std::endl;

    // 3. 测试获取不存在的键
    std::cout << "\n[测试 3] 获取不存在的键 (Get Non-existent):" << std::endl;
    std::cout << "get(3): " << myHashMap.get(3) << " (预期: -1)" << std::endl;

    // 4. 测试删除
    std::cout << "\n[测试 4] 删除键 (Remove):" << std::endl;
    myHashMap.remove(2);
    std::cout << "remove(2)" << std::endl;
    std::cout << "get(2): " << myHashMap.get(2) << " (预期: -1)" << std::endl;

    // 5. 测试哈希冲突 (Collision)
    // CAPACITY = 10000, 所以 1 和 10001 会映射到同一个桶
    std::cout << "\n[测试 5] 哈希冲突处理 (Collision Handling):" << std::endl;
    myHashMap.put(10001, 999);
    std::cout << "put(10001, 999) -> 应该与 key=1 冲突" << std::endl;
    std::cout << "get(1): " << myHashMap.get(1) << " (预期: 1)" << std::endl;
    std::cout << "get(10001): " << myHashMap.get(10001) << " (预期: 999)" << std::endl;

    // 删除冲突链中的一个
    myHashMap.remove(1);
    std::cout << "remove(1) -> 删除链表头节点" << std::endl;
    std::cout << "get(1): " << myHashMap.get(1) << " (预期: -1)" << std::endl;
    std::cout << "get(10001): " << myHashMap.get(10001) << " (预期: 999)" << std::endl;

    std::cout << "\n=== 测试结束 ===" << std::endl;

    return 0;
}