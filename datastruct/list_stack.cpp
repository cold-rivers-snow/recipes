#include <iostream>
#include <list>       // 标准库链表
#include <stdexcept>  // 异常处理

template <typename T>
class ListStack {
private:
    std::list<T> stack;  // 底层容器：std::list（双向链表）

public:
    // 入栈：在链表头部添加元素（栈顶）
    void push(const T& val) {
        stack.push_front(val);  // O(1) 时间复杂度
    }

    // 出栈：移除并返回栈顶元素
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("栈为空，无法出栈");
        }
        T topVal = stack.front();  // 获取栈顶元素
        stack.pop_front();         // 移除栈顶（O(1)）
        return topVal;
    }

    // 获取栈顶元素（不删除）
    T top() const {
        if (isEmpty()) {
            throw std::runtime_error("栈为空，无法获取栈顶元素");
        }
        return stack.front();
    }

    // 判断栈是否为空
    bool isEmpty() const {
        return stack.empty();
    }

    // 获取栈的大小
    size_t size() const {
        return stack.size();
    }

    // 打印栈元素（从栈顶到栈底）
    void print() const {
        if (isEmpty()) {
            std::cout << "栈为空" << std::endl;
            return;
        }
        std::cout << "栈顶 -> ";
        for (const auto& elem : stack) {  // 遍历链表（栈顶到栈底）
            std::cout << elem << " -> ";
        }
        std::cout << "栈底" << std::endl;
    }
};

// 测试示例
int main() {
    try {
        ListStack<int> stack;

        // 入栈
        stack.push(10);
        stack.push(20);
        stack.push(30);
        std::cout << "入栈后（10, 20, 30）：" << std::endl;
        stack.print();  // 输出：栈顶 -> 30 -> 20 -> 10 -> 栈底
        std::cout << "栈大小：" << stack.size() << std::endl;  // 3

        // 获取栈顶
        std::cout << "栈顶元素：" << stack.top() << std::endl;  // 30

        // 出栈
        std::cout << "出栈元素：" << stack.pop() << std::endl;  // 30
        stack.print();  // 栈顶 -> 20 -> 10 -> 栈底
        std::cout << "栈大小：" << stack.size() << std::endl;  // 2

        // 清空栈
        stack.pop();  // 出栈 20
        stack.pop();  // 出栈 10
        std::cout << "清空后是否为空：" << (stack.isEmpty() ? "是" : "否") << std::endl;  // 是

        // 测试空栈操作（抛异常）
        stack.pop();  // 触发异常
    } catch (const std::exception& e) {
        std::cout << "异常：" << e.what() << std::endl;  // 输出错误信息
    }

    return 0;
}