// g++ compile.cpp -Wall -Werror -std=c++11 -o a.out;./a.out
// g++ compile.cpp -Wall -Werror -std=c++14 -o a.out;./a.out
//
// g++ compile.cpp -DNDEBUG -std=c++11 -o a.out;./a.out
//
// gcc -E -dM - < /dev/null

#include <cassert>

#include <typeinfo>
#include <string>
#include <iostream>
#include <stdexcept>
#include <type_traits>

template<int N>
struct fib
{
    static_assert(N >= 0, "N must be postive");

    static const int value =
        fib<N - 1>::value + fib<N - 2>::value;
};

template<>
struct fib<0>
{
    static const int value = 1;
};

template<>
struct fib<1>
{
    static const int value = 1;
};

#if __cplusplus >= 201402
constexpr int const_fib(int n)
{
    if (n <= 1) {
        return 1;
    }

    return const_fib(n - 1) + const_fib(n - 2);
}
#else   // C++11
int const_fib(int n)
{
    return 42;
}
#endif

[[deprecated("deadline:2020-12-31")]]      // c++14 or later   表示改函数已经启用，如果不使用这个编译就不会告警
// [[gnu::deprecated]] // c+11 or later
int old_func()
{
    //[[gnu::deprecated("I hate this")]]
    int value = 0;

    return value;
}

[[gnu::constructor]]    //标记一个函数，在 程序 main() 函数执行前自动调用。在全局对象构造之前或期间（取决于优先级）,在 main() 之前, 非常适合用于库的自动初始化
void first_func()    
{
    // can not use cout!
    printf("before main()\n");
    std::cout << "[INIT] Program is starting...\n";
}

[[gnu::destructor]]   //标记一个函数，在 程序正常退出（如 exit() 或 main() 返回）后自动调用。在 main() 返回之后, 在全局对象析构之后, 注意：不会在 std::abort()、信号终止（如 SIGKILL）时调用
void last_func()
{
    // can not use cout!
    printf("after main()\n");
    std::cout << "[CLEANUP] Program is exiting...\n";
}

[[gnu::constructor(101)]]  //指定 优先级，控制执行顺序, 数值越小，constructor 越早执行，destructor 越晚执行, 默认优先级是 65535, 有效范围：1 到 65535（0 和 65536+ 保留）
void init_early() {
    std::cout << "[INIT] Early init (priority 100)\n";
}

[[gnu::destructor(200)]]
void cleanup_late() {
    std::cout << "[CLEANUP] Late cleanup (priority 200)\n";
}

[[gnu::always_inline]] inline  //强制要求编译器尽可能将函数内联。编译器的属性比 inline强，inline可能不会内敛
int get_num()
{
    return 42;
}

[[noreturn]]
int case1(bool flag)
{
    throw std::runtime_error("XXX");
}

void case2()
{
    using namespace std;

    [[gnu::unused]]
    int nouse;

    cout << "case2" << endl;
}

[[gnu::hot]]   //告诉编译器：这个函数是“热点函数”（frequently called），应该优先进行优化（如内联、循环展开、寄存器分配等），以提升性能
void case3()
{
    using namespace std;

    cout << "case3" << endl;
}

void case4()
{
    static_assert(sizeof(int) == 4, "int must be 32bit");
    static_assert(sizeof(long) >= 8, "must run on x64");
}

template<typename T>
void check_type(T v)
{
    using namespace std;

    static_assert(is_integral<T>::value, "int");
    // static_assert(is_pointer<T>::value, "ptr");
    //static_assert(is_default_constructible<T>::value, "is_default_constructible");

    cout << "static_assert : " << typeid(v).name() << endl;

    cout << is_void<void>::value << endl;
}

void case5()
{
    int     i = 10;
    int    *p = &i;

    assert(i > 0 && "i must be greater than zero");
    assert(p != nullptr);

    std::string str = "hello";
    assert(!str.empty());
}

int main()
{
    using namespace std;

    cout << fib<2>::value << endl;
    cout << fib<3>::value << endl;
    cout << fib<4>::value << endl;
    cout << fib<5>::value << endl;

    cout << const_fib(10) << endl;

    // old_func();
    get_num();

    case2();
    case3();

    case4();
    case5();

    check_type(10);
    // check_type((void*)0);

    cout << "compile stage demo" << endl;
}
