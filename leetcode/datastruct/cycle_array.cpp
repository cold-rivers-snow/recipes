#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>
#include <numeric>

template <typename T>
class CycleArray {
private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;  // 下一个要读的元素的位置
    size_t tail_;  // 下一个要写的元素的位置
    size_t size_;  // 当前元素数量

public:
    // 构造函数，初始化容量
    explicit CycleArray(size_t capacity) 
        : capacity_(capacity), head_(0), tail_(0), size_(0) 
    {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than zero.");
        }
        // 实际存储容量比传入的 capacity 大 1，这是为了区分满和空的状态。
        // 然而，为了简化 size_ 逻辑，我们直接用 size_ 字段跟踪实际元素数量，
        // 并且让 capacity_ 就是用户指定的容量。
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

    // 插入元素 (EnQueue)
    void push(const T& item) {
        if (full()) {
            // 如果满了，可以选择抛出异常，或覆盖最旧的元素 (FIFO原则)
            // 这里选择覆盖最旧的元素（Head 前进）
            head_ = (head_ + 1) % capacity_; 
        } else {
            // 只有在未满时才增加 size
            size_++;
        }
        
        // 插入新元素
        buffer_[tail_] = item;
        
        // Tail 指针前进
        tail_ = (tail_ + 1) % capacity_;
    }

    // 弹出元素 (DeQueue)
    T pop() {
        if (empty()) {
            throw std::out_of_range("CycleArray is empty. Cannot pop.");
        }

        // 取出头部元素
        T item = buffer_[head_];
        
        // Head 指针前进
        head_ = (head_ + 1) % capacity_;
        
        // 元素数量减少
        size_--;
        
        return item;
    }

    // 查看头部元素 (Peek)
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("CycleArray is empty. No front element.");
        }
        return buffer_[head_];
    }
};

// 辅助打印函数
template <typename T>
void print_state(const CycleArray<T>& arr, const std::string& msg) {
    std::cout << "--- " << msg << " ---" << std::endl;
    std::cout << "Size: " << arr.size() << ", Full: " << (arr.full() ? "Yes" : "No") << ", Empty: " << (arr.empty() ? "Yes" : "No") << std::endl;
}

void run_test() {
    std::cout << "Starting CycleArray Test (Capacity = 4)" << std::endl;
    CycleArray<int> arr(4);
    
    // ----------------------------------------------------
    // Test 1: Basic Push and Pop (未满状态)
    // ----------------------------------------------------
    arr.push(10); // [10, -, -, -]
    arr.push(20); // [10, 20, -, -]
    print_state(arr, "After pushing 10, 20"); 
    // Output: Size: 2, Full: No, Empty: No

    std::cout << "Front element: " << arr.front() << std::endl; // Output: 10
    
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 10. Buffer conceptually: [-, 20, -, -]
    print_state(arr, "After popping 10");
    // Output: Size: 1, Full: No, Empty: No

    // ----------------------------------------------------
    // Test 2: Filling and Wrap-around (填满和环绕)
    // ----------------------------------------------------
    arr.push(30); // [-, 20, 30, -]
    arr.push(40); // [-, 20, 30, 40]
    arr.push(50); // [50, 20, 30, 40] - Now full, size 4
    print_state(arr, "After filling to max (20, 30, 40, 50)");
    // Output: Size: 4, Full: Yes, Empty: No

    // ----------------------------------------------------
    // Test 3: Overwrite (覆盖)
    // ----------------------------------------------------
    std::cout << "\nPushing 60 (should overwrite 20)" << std::endl;
    arr.push(60); // [50, 60, 30, 40] - Head moves from 20 to 30
    print_state(arr, "After pushing 60 (overwrite)"); 
    // Output: Size: 4, Full: Yes, Empty: No

    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 30 (The oldest remaining element)
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 40
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 50
    print_state(arr, "After three more pops");
    // Output: Size: 1, Full: No, Empty: No

    // ----------------------------------------------------
    // Test 4: Empty and Exception Handling (清空和异常处理)
    // ----------------------------------------------------
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 60
    print_state(arr, "After final pop");
    // Output: Size: 0, Full: No, Empty: Yes
    
    // Try to pop from an empty array
    try {
        arr.pop();
    } catch (const std::out_of_range& e) {
        std::cout << "\nCaught expected exception: " << e.what() << std::endl;
    }
}

int main() {
    run_test();
    return 0;
}