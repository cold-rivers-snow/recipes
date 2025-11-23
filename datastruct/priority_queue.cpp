#include <vector>
#include <functional>
#include <stdexcept>

template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> heap;
    Compare comp; // 比较器
    
    // 获取父节点索引
    int parent(int i) const { return (i - 1) / 2; }
    
    // 获取左子节点索引
    int left(int i) const { return 2 * i + 1; }
    
    // 获取右子节点索引
    int right(int i) const { return 2 * i + 2; }
    
    // 上浮操作
    void swim(int i) {
        while (i > 0 && comp(heap[parent(i)], heap[i])) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }
    
    // 下沉操作
    void sink(int i) {
        int n = heap.size();
        while (left(i) < n) {
            int maxChild = left(i);
            // 使用比较器找到应该上浮的子节点
            if (right(i) < n && comp(heap[maxChild], heap[right(i)])) {
                maxChild = right(i);
            }
            
            // 如果当前节点已经满足堆性质，停止下沉
            if (!comp(heap[i], heap[maxChild])) {
                break;
            }
            
            std::swap(heap[i], heap[maxChild]);
            i = maxChild;
        }
    }

public:
    // 构造函数
    explicit PriorityQueue(const Compare& c = Compare()) : comp(c) {}
    
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
    const T& top() const {
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
    
    // 清空队列
    void clear() {
        heap.clear();
    }
};

// 使用示例
#include <iostream>

int main() {
    // 最大堆（默认）
    PriorityQueue<int> maxHeap;
    maxHeap.push(3);
    maxHeap.push(1);
    maxHeap.push(4);
    maxHeap.push(1);
    maxHeap.push(5);
    
    std::cout << "Max Heap: ";
    while (!maxHeap.empty()) {
        std::cout << maxHeap.pop() << " "; // 输出: 5 4 3 1 1
    }
    std::cout << std::endl;
    
    // 最小堆
    PriorityQueue<int, std::greater<int>> minHeap;
    minHeap.push(3);
    minHeap.push(1);
    minHeap.push(4);
    minHeap.push(1);
    minHeap.push(5);
    
    std::cout << "Min Heap: ";
    while (!minHeap.empty()) {
        std::cout << minHeap.pop() << " "; // 输出: 1 1 3 4 5
    }
    std::cout << std::endl;
    
    return 0;
}