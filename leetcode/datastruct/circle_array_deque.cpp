#include <iostream>
#include <vector>
#include <stdexcept>

// 环形数组类（复用基础实现，扩展双端操作所需的指针移动逻辑）
template <typename T>
class CycleArray {
protected:  // 保护成员，允许派生类访问
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;  // 队首（下一个要读的位置）
    size_t tail_;  // 队尾（下一个要写的位置）
    size_t size_;  // 当前元素数量

public:
    // 构造函数：初始化容量
    explicit CycleArray(size_t capacity) 
        : capacity_(capacity), head_(0), tail_(0), size_(0) 
    {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than zero.");
        }
        buffer_.resize(capacity);
    }

    // 基础状态判断
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == capacity_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

protected:
    // 辅助函数：计算上一个索引（用于队首入队）
    size_t prev_index(size_t index) const {
        return (index == 0) ? capacity_ - 1 : index - 1;
    }

    // 辅助函数：计算下一个索引（用于队尾入队/常规出队）
    size_t next_index(size_t index) const {
        return (index + 1) % capacity_;
    }
};

// 基于环形数组的双端队列
template <typename T>
class CircularDeque : public CycleArray<T> {
public:
    // 继承构造函数
    using CycleArray<T>::CycleArray;

    // 队首入队（头部插入）
    void push_front(const T& item) {
        if (this->full()) {
            throw std::overflow_error("双端队列已满，无法从队首入队");
            // 若需覆盖逻辑，可改为：pop_back(); 再执行以下操作
        }
        // 队首指针前移（环形处理）
        this->head_ = this->prev_index(this->head_);
        // 存入新元素
        this->buffer_[this->head_] = item;
        this->size_++;
    }

    // 队尾入队（尾部插入）
    void push_back(const T& item) {
        if (this->full()) {
            throw std::overflow_error("双端队列已满，无法从队尾入队");
            // 若需覆盖逻辑，可改为：pop_front(); 再执行以下操作
        }
        // 存入新元素（tail_ 原本指向空闲位置）
        this->buffer_[this->tail_] = item;
        // 队尾指针后移（环形处理）
        this->tail_ = this->next_index(this->tail_);
        this->size_++;
    }

    // 队首出队（移除并返回头部元素）
    T pop_front() {
        if (this->empty()) {
            throw std::underflow_error("双端队列为空，无法从队首出队");
        }
        T item = this->buffer_[this->head_];
        // 队首指针后移
        this->head_ = this->next_index(this->head_);
        this->size_--;
        return item;
    }

    // 队尾出队（移除并返回尾部元素）
    T pop_back() {
        if (this->empty()) {
            throw std::underflow_error("双端队列为空，无法从队尾出队");
        }
        // 队尾指针前移（指向最后一个元素）
        this->tail_ = this->prev_index(this->tail_);
        T item = this->buffer_[this->tail_];
        this->size_--;
        return item;
    }

    // 查看队首元素
    const T& front() const {
        if (this->empty()) {
            throw std::underflow_error("双端队列为空，无队首元素");
        }
        return this->buffer_[this->head_];
    }

    // 查看队尾元素
    const T& back() const {
        if (this->empty()) {
            throw std::underflow_error("双端队列为空，无队尾元素");
        }
        // tail_ 指向空闲位置，实际队尾是前一个索引
        size_t last_index = this->prev_index(this->tail_);
        return this->buffer_[last_index];
    }

    // 打印双端队列（从队首到队尾）
    void print() const {
        if (this->empty()) {
            std::cout << "双端队列为空" << std::endl;
            return;
        }
        std::cout << "双端队列（大小: " << this->size() 
                  << ", 容量: " << this->capacity() << "）：";
        std::cout << "队首 -> ";
        size_t current = this->head_;
        for (size_t i = 0; i < this->size(); ++i) {
            std::cout << this->buffer_[current] << " -> ";
            current = this->next_index(current);
        }
        std::cout << "队尾" << std::endl;
    }
};

// 测试示例
int main() {
    try {
        CircularDeque<int> deque(4);  // 容量为 4 的双端队列

        // 队尾入队
        deque.push_back(10);
        deque.push_back(20);
        deque.print();  // 队首 -> 10 -> 20 -> 队尾（size:2）

        // 队首入队
        deque.push_front(5);
        deque.push_front(3);
        deque.print();  // 队首 -> 3 -> 5 -> 10 -> 20 -> 队尾（size:4，已满）

        // 测试队列满时入队（抛异常）
        // deque.push_back(30);  // 触发 "已满" 异常

        // 查看首尾元素
        std::cout << "队首元素：" << deque.front() << std::endl;  // 3
        std::cout << "队尾元素：" << deque.back() << std::endl;   // 20

        // 队首出队
        std::cout << "队首出队：" << deque.pop_front() << std::endl;  // 3
        deque.print();  // 队首 -> 5 -> 10 -> 20 -> 队尾（size:3）

        // 队尾出队
        std::cout << "队尾出队：" << deque.pop_back() << std::endl;  // 20
        deque.print();  // 队首 -> 5 -> 10 -> 队尾（size:2）

        // 继续队首入队
        deque.push_front(2);
        deque.print();  // 队首 -> 2 -> 5 -> 10 -> 队尾（size:3）

        // 清空队列
        while (!deque.empty()) {
            std::cout << "队尾出队：" << deque.pop_back() << std::endl;  // 10,5,2
        }
        deque.print();  // 双端队列为空

    } catch (const std::exception& e) {
        std::cout << "异常：" << e.what() << std::endl;
    }

    return 0;
}