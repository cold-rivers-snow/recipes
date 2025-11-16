#include <iostream>
#include <list>       // 标准库双向链表
#include <stdexcept>  // 异常处理

template <typename T>
class ListQueue {
private:
    std::list<T> queue;  // 底层容器：std::list

public:
    // 入队：在链表尾部添加元素（队尾）
    void enqueue(const T& val) {
        queue.push_back(val);  // O(1) 操作
    }

    // 出队：移除并返回链表头部元素（队首）
    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("队列为空，无法出队");
        }
        T frontVal = queue.front();  // 获取队首元素
        queue.pop_front();           // 移除队首（O(1) 操作）
        return frontVal;
    }

    // 获取队首元素（不删除）
    T front() const {
        if (isEmpty()) {
            throw std::runtime_error("队列为空，无法获取队首元素");
        }
        return queue.front();
    }

    // 获取队尾元素（不删除）
    T back() const {
        if (isEmpty()) {
            throw std::runtime_error("队列为空，无法获取队尾元素");
        }
        return queue.back();
    }

    // 判断队列是否为空
    bool isEmpty() const {
        return queue.empty();
    }

    // 获取队列大小
    size_t size() const {
        return queue.size();
    }

    // 打印队列元素（从队首到队尾）
    void print() const {
        if (isEmpty()) {
            std::cout << "队列为空" << std::endl;
            return;
        }
        std::cout << "队首 -> ";
        for (const auto& elem : queue) {  // 遍历链表（队首到队尾）
            std::cout << elem << " -> ";
        }
        std::cout << "队尾" << std::endl;
    }
};

// 测试示例
int main() {
    try {
        ListQueue<int> q;

        // 入队操作
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        std::cout << "入队后（10, 20, 30）：" << std::endl;
        q.print();  // 输出：队首 -> 10 -> 20 -> 30 -> 队尾
        std::cout << "队列大小：" << q.size() << std::endl;  // 3

        // 获取队首和队尾
        std::cout << "队首元素：" << q.front() << std::endl;  // 10
        std::cout << "队尾元素：" << q.back() << std::endl;   // 30

        // 出队操作
        std::cout << "出队元素：" << q.dequeue() << std::endl;  // 10
        q.print();  // 输出：队首 -> 20 -> 30 -> 队尾
        std::cout << "队列大小：" << q.size() << std::endl;  // 2

        // 清空队列
        q.dequeue();  // 出队 20
        q.dequeue();  // 出队 30
        std::cout << "清空后是否为空：" << (q.isEmpty() ? "是" : "否") << std::endl;  // 是

        // 测试空队列操作（抛异常）
        q.dequeue();  // 触发异常
    } catch (const std::exception& e) {
        std::cout << "异常：" << e.what() << std::endl;  // 输出错误信息
    }

    return 0;
}