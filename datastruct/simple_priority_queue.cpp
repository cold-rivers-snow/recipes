#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>

template<typename T>
class SimplePriorityQueue {
private:
    std::vector<T> heap;
    
    // 获取父节点索引
    int parent(int i) { return (i - 1) / 2; }
    
    // 获取左子节点索引
    int left(int i) { return 2 * i + 1; }
    
    // 获取右子节点索引
    int right(int i) { return 2 * i + 2; }
    
    // 上浮操作
    void swim(int i) {
        while (i > 0 && heap[i] > heap[parent(i)]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }
    
    // 下沉操作
    void sink(int i) {
        int n = heap.size();
        while (left(i) < n) {
            int maxChild = left(i);
            // 找到较大的子节点
            if (right(i) < n && heap[right(i)] > heap[left(i)]) {
                maxChild = right(i);
            }
            
            // 如果当前节点已经大于等于较大子节点，停止下沉
            if (heap[i] >= heap[maxChild]) {
                break;
            }
            
            std::swap(heap[i], heap[maxChild]);
            i = maxChild;
        }
    }

public:
    // 插入元素
    void push(const T& val) {
        heap.push_back(val);
        swim(heap.size() - 1);
    }
    
    // 删除并返回堆顶元素
    T pop() {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        
        T top = heap[0];
        std::swap(heap[0], heap[heap.size() - 1]);
        heap.pop_back();
        
        if (!heap.empty()) {
            sink(0);
        }
        
        return top;
    }
    
    // 查看堆顶元素
    T peek() const {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0];
    }
    
    // 检查是否为空
    bool empty() const {
        return heap.empty();
    }
    
    // 获取元素数量
    size_t size() const {
        return heap.size();
    }
};

void test_basic_operations() {
    std::cout << "Testing basic operations..." << std::endl;
    SimplePriorityQueue<int> pq;
    
    assert(pq.empty());
    assert(pq.size() == 0);
    
    pq.push(10);
    assert(!pq.empty());
    assert(pq.size() == 1);
    assert(pq.peek() == 10);
    
    pq.push(30);
    assert(pq.peek() == 30); // Max heap property
    
    pq.push(20);
    assert(pq.peek() == 30);
    
    pq.push(40);
    assert(pq.peek() == 40);
    
    std::cout << "  Push operations passed." << std::endl;
    
    assert(pq.pop() == 40);
    assert(pq.peek() == 30);
    
    assert(pq.pop() == 30);
    assert(pq.peek() == 20);
    
    assert(pq.pop() == 20);
    assert(pq.peek() == 10);
    
    assert(pq.pop() == 10);
    assert(pq.empty());
    
    std::cout << "  Pop operations passed." << std::endl;
}

void test_random_order() {
    std::cout << "Testing random order insertion..." << std::endl;
    SimplePriorityQueue<int> pq;
    std::vector<int> inputs = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    for(int x : inputs) {
        pq.push(x);
    }
    
    std::vector<int> expected = {9, 6, 5, 5, 4, 3, 2, 1, 1};
    std::vector<int> result;
    while(!pq.empty()) {
        result.push_back(pq.pop());
    }
    
    assert(result == expected);
    std::cout << "  Random order sort passed." << std::endl;
}

void test_exceptions() {
    std::cout << "Testing exceptions..." << std::endl;
    SimplePriorityQueue<int> pq;
    try {
        pq.pop();
        assert(false && "Should have thrown exception");
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught expected exception on pop(): " << e.what() << std::endl;
    }
    
    try {
        pq.peek();
        assert(false && "Should have thrown exception");
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught expected exception on peek(): " << e.what() << std::endl;
    }
}

int main() {
    try {
        test_basic_operations();
        test_random_order();
        test_exceptions();
        
        std::cout << "\nAll tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}