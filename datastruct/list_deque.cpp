#include <iostream>
#include <list>       // 标准库双向链表
#include <stdexcept>  // 异常处理

template <typename T>
class LinkedListDeque {
private:
    std::list<T> deque;  // 底层容器：std::list（双向链表）

public:
    // 队首入队（头部插入）
    void push_front(const T& val) {
        deque.push_front(val);  // O(1) 操作
    }

    // 队尾入队（尾部插入）
    void push_back(const T& val) {
        deque.push_back(val);   // O(1) 操作
    }

    // 队首出队（移除并返回头部元素）
    T pop_front() {
        if (isEmpty()) {
            throw std::runtime_error("双端队列为空，无法从队首出队");
        }
        T frontVal = deque.front();  // 获取头部元素
        deque.pop_front();           // 移除头部元素（O(1)）
        return frontVal;
    }

    // 队尾出队（移除并返回尾部元素）
    T pop_back() {
        if (isEmpty()) {
            throw std::runtime_error("双端队列为空，无法从队尾出队");
        }
        T backVal = deque.back();    // 获取尾部元素
        deque.pop_back();            // 移除尾部元素（O(1)）
        return backVal;
    }

    // 查看队首元素（不删除）
    const T& front() const {
        if (isEmpty()) {
            throw std::runtime_error("双端队列为空，无队首元素");
        }
        return deque.front();
    }

    // 查看队尾元素（不删除）
    const T& back() const {
        if (isEmpty()) {
            throw std::runtime_error("双端队列为空，无队尾元素");
        }
        return deque.back();
    }

    // 判断队列是否为空
    bool isEmpty() const {
        return deque.empty();
    }

    // 获取队列大小
    size_t size() const {
        return deque.size();
    }

    // 打印双端队列元素（从队首到队尾）
    void print() const {
        if (isEmpty()) {
            std::cout << "双端队列为空" << std::endl;
            return;
        }
        std::cout << "队首 -> ";
        for (const auto& elem : deque) {  // 遍历链表（队首到队尾）
            std::cout << elem << " -> ";
        }
        std::cout << "队尾" << std::endl;
    }
};

// 测试示例
int main() {
    try {
        LinkedListDeque<int> deque;

        // 队尾入队
        deque.push_back(10);
        deque.push_back(20);
        std::cout << "队尾入队 10, 20 后：" << std::endl;
        deque.print();  // 输出：队首 -> 10 -> 20 -> 队尾
        std::cout << "大小：" << deque.size() << std::endl;  // 2

        // 队首入队
        deque.push_front(5);
        std::cout << "队首入队 5 后：" << std::endl;
        deque.print();  // 输出：队首 -> 5 -> 10 -> 20 -> 队尾
        std::cout << "大小：" << deque.size() << std::endl;  // 3

        // 查看首尾元素
        std::cout << "队首元素：" << deque.front() << std::endl;  // 5
        std::cout << "队尾元素：" << deque.back() << std::endl;   // 20

        // 队首出队
        std::cout << "队首出队：" << deque.pop_front() << std::endl;  // 5
        deque.print();  // 输出：队首 -> 10 -> 20 -> 队尾
        std::cout << "大小：" << deque.size() << std::endl;  // 2

        // 队尾出队
        std::cout << "队尾出队：" << deque.pop_back() << std::endl;  // 20
        deque.print();  // 输出：队首 -> 10 -> 队尾
        std::cout << "大小：" << deque.size() << std::endl;  // 1

        // 清空队列
        deque.pop_front();
        std::cout << "清空后是否为空：" << (deque.isEmpty() ? "是" : "否") << std::endl;  // 是

        // 测试空队列操作（抛异常）
        deque.pop_front();  // 触发异常
    } catch (const std::exception& e) {
        std::cout << "异常：" << e.what() << std::endl;  // 输出错误信息
    }

    return 0;
}