#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>  // 用于strncpy
#include <sys/syscall.h>
#include <unistd.h>   

// 线程函数参数：包含要绑定的核心编号和线程名
struct ThreadArgs {
    int core_id;
    const char* thread_name;
};

// 线程函数：绑定自身到指定核心，并设置线程名称
void* thread_func(void* args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    int core = targs->core_id;
    const char* name = targs->thread_name;

    // 设置线程亲和性（绑定到指定CPU核心）
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    pthread_t tid = pthread_self();
    if (pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np failed");
        return nullptr;
    }

    //重新帮到固定的上面
    cpu_set_t un_cpu_set;
    memcpy(&un_cpu_set, &cpuset, sizeof(cpu_set_t));
    CPU_ZERO(&un_cpu_set);
    CPU_SET(4, &un_cpu_set);
    pid_t kernel_tid = syscall(SYS_gettid);
    if (sched_setaffinity(kernel_tid, sizeof(cpu_set_t), &un_cpu_set) != 0) {
        perror("sched_setaffinity failed");
        return nullptr;
    }

    // 设置线程名称（关键：让top等工具能显示可读名称）
    // 注意：Linux线程名长度限制为15个字符（含结束符）
    if (pthread_setname_np(tid, name) != 0) {
        perror("pthread_setname_np failed");
        return nullptr;
    }

    printf("Thread %s (tid: %lu) bound to CPU %d\n", name, tid, core);

    // 线程业务逻辑（无限循环，方便在top中观察）
    while (1) {
        sched_yield(); // 让出CPU，减少资源占用
    }
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;
    // 线程名长度建议不超过15字符（Linux限制）
    ThreadArgs args1 = {0, "IO-Thread"}, args2 = {1, "Net-Thread"};

    // 创建线程
    pthread_create(&thread1, nullptr, thread_func, &args1);
    pthread_create(&thread2, nullptr, thread_func, &args2);

    // 主线程也设置一个名称（可选）
    pthread_setname_np(pthread_self(), "Main-Thread");

    // 等待子线程结束（实际中可根据需求调整）
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    return 0;
}
//g++ -o thread_affinity thread_affinity.cc -lpthread  