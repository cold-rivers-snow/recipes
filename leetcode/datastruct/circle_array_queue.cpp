#include <iostream>
#include <vector>
#include <stdexcept>

// 环形数组类（复用提供的实现）
template <typename T>
class CycleArray {
private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;  // 下一个要读的元素的位置（队首）
    size_t tail_;  // 下一个要写的元素的位置（队尾）
    size_t size_;  // 当前元素数量

public:
    // 构造函数，初始化容量
    explicit CycleArray(size_t capacity) 
        : capacity_(capacity), head_(0), tail_(0), size_(0) 
    {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than zero.");
        }
        buffer_.resize(capacity);
    }

    // 检查环形数组是否为空
    bool empty() const {
        return size_ == 0;
    }

    // 检查环形数组是否已满
    bool full() const {
        return size_ == capacity_;
    }

    // 获取当前存储的元素数量
    size_t size() const {
        return size_;
    }

    // 获取容量
    size_t capacity() const {
        return capacity_;
    }

    // 插入元素 (EnQueue 方向)
    void push(const T& item) {
        if (full()) {
            // 满时覆盖最旧元素（队首后移）
            head_ = (head_ + 1) % capacity_; 
        } else {
            size_++;
        }
        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
    }

    // 弹出元素 (DeQueue 方向)
    T pop() {
        if (empty()) {
            throw std::out_of_range("CycleArray is empty. Cannot pop.");
        }
        T item = buffer_[head_];
        head_ = (head_ + 1) % capacity_;
        size_--;
        return item;
    }

    // 查看头部元素
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("CycleArray is empty. No front element.");
        }
        return buffer_[head_];
    }
};

// 基于环形数组的队列类
template <typename T>
class CircularQueue {
private:
    CycleArray<T> cycle_arr_;  // 底层环形数组

public:
    // 构造函数：指定队列容量
    explicit CircularQueue(size_t capacity) : cycle_arr_(capacity) {}

    // 入队：在队尾添加元素（满时覆盖最旧元素）
    void enqueue(const T& item) {
        cycle_arr_.push(item);  // 复用环形数组的 push 操作
    }

    // 出队：移除并返回队首元素
    T dequeue() {
        return cycle_arr_.pop();  // 复用环形数组的 pop 操作
    }

    // 查看队首元素（不删除）
    const T& front() const {
        return cycle_arr_.front();  // 复用环形数组的 front 操作
    }

    // 判断队列是否为空
    bool isEmpty() const {
        return cycle_arr_.empty();
    }

    // 判断队列是否已满
    bool isFull() const {
        return cycle_arr_.full();
    }

    // 获取当前元素数量
    size_t size() const {
        return cycle_arr_.size();
    }

    // 获取队列容量
    size_t capacity() const {
        return cycle_arr_.capacity();
    }

    // 打印队列元素（从队首到队尾）
    void print() const {
        if (isEmpty()) {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        std::cout << "Queue (size: " << size() << ", capacity: " << capacity() << ")：";
        std::cout << "front -> ";
        
        // 临时拷贝环形数组数据，按顺序打印（避免修改原数组）
        CycleArray<T> temp(cycle_arr_.capacity());
        size_t current_size = size();
        for (size_t i = 0; i < current_size; ++i) {
            T item = cycle_arr_.front();
            temp.push(item);
            std::cout << item << " -> ";
            // 临时移动原数组 head_（打印后恢复）
            const_cast<CycleArray<T>&>(cycle_arr_).pop();
        }
        // 恢复原数组数据
        for (size_t i = 0; i < current_size; ++i) {
            T item = temp.pop();
            const_cast<CycleArray<T>&>(cycle_arr_).push(item);
        }
        
        std::cout << "rear" << std::endl;
    }
};

// 测试示例
int main() {
    try {
        // 创建容量为 3 的队列
        CircularQueue<int> queue(3);

        // 入队操作
        queue.enqueue(10);
        queue.enqueue(20);
        queue.enqueue(30);
        queue.print();  // 输出：front -> 10 -> 20 -> 30 -> rear（size:3, capacity:3）

        // 队列已满，继续入队会覆盖队首（10）
        queue.enqueue(40);
        queue.print();  // 输出：front -> 20 -> 30 -> 40 -> rear（size:3, capacity:3）

        // 出队操作
        std::cout << "Dequeue: " << queue.dequeue() << std::endl;  // 出队 20
        queue.print();  // 输出：front -> 30 -> 40 -> rear（size:2, capacity:3）

        // 查看队首
        std::cout << "Front element: " << queue.front() << std::endl;  // 30

        // 继续入队
        queue.enqueue(50);
        queue.print();  // 输出：front -> 30 -> 40 -> 50 -> rear（size:3, capacity:3）

        // 清空队列
        while (!queue.isEmpty()) {
            std::cout << "Dequeue: " << queue.dequeue() << std::endl;  // 30,40,50
        }
        queue.print();  // 输出：Queue is empty.

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}