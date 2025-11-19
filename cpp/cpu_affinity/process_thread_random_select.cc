#include <stdio.h>
#include <sched.h>
#include <vector>
#include <random>
#include <string.h>
#include <unistd.h>          // getpid(), syscall()
#include <sys/syscall.h>     // SYS_gettid
#include <sys/prctl.h>       // prctl(), PR_SET_NAME
#include <pthread.h>         // pthread_self, pthread_setaffinity_np
#include <algorithm>
#include <thread>
#include <atomic>
#include <stdexcept>
#include <memory>

//线程数小于核心数，优先绑定未绑定的核心

// 线程参数结构体
struct ThreadArgs {
    int core_id;                                      // 未使用，保留扩展性
    const char* thread_name;
    std::vector<std::unique_ptr<std::atomic<bool>>>* core_used;
    const std::vector<int>* allowed_cores;
};

// 为进程绑定指定核心区间
bool set_process_affinity_ranges(const std::vector<std::pair<int, int>>& ranges) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    for (const auto& range : ranges) {
        int start = range.first;
        int end = range.second;
        if (start > end) {
            fprintf(stderr, "Invalid core range: %d-%d\n", start, end);
            return false;
        }
        for (int core = start; core <= end; ++core) {
            if (core >= CPU_SETSIZE) {
                fprintf(stderr, "Core ID %d exceeds CPU_SETSIZE (%d)\n", core, CPU_SETSIZE);
                return false;
            }
            CPU_SET(core, &cpuset);
        }
    }

    pid_t pid = getpid();
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_setaffinity failed");
        return false;
    }

    printf("Process %d bound to cores: ", pid);
    for (size_t i = 0; i < ranges.size(); ++i) {
        printf("%d-%d", ranges[i].first, ranges[i].second);
        if (i != ranges.size() - 1) printf(", ");
    }
    printf("\n");
    return true;
}

// 获取进程允许的所有核心ID
std::vector<int> get_allowed_cores() {
    std::vector<int> cores;
    cpu_set_t cpuset;
    pid_t pid = getpid();

    if (sched_getaffinity(pid, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_getaffinity failed");
        return cores;
    }

    for (int i = 0; i < CPU_SETSIZE; ++i) {
        if (CPU_ISSET(i, &cpuset)) {
            cores.push_back(i);
        }
    }
    return cores;
}

// 优化的核心选择逻辑
int select_unused_core_first(
    const std::vector<int>& allowed_cores,
    std::vector<std::unique_ptr<std::atomic<bool>>>& core_used
) {
    for (size_t i = 0; i < core_used.size(); ++i) {
        bool expected = false;
        if (core_used[i] && core_used[i]->compare_exchange_strong(expected, true)) {
            int selected_core = allowed_cores[i];
            printf("[Select] Unused core: %d (index: %lu)\n", selected_core, i);
            return selected_core;
        }
    }

    // 所有核心已使用：随机选择
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, allowed_cores.size() - 1);
    size_t selected_idx = dist(gen);
    int selected_core = allowed_cores[selected_idx];
    printf("[Select] All cores used, random: %d (index: %lu)\n", selected_core, selected_idx);
    return selected_core;
}

// 设置线程名称（安全版本）
void set_thread_name(const char* name) {
#ifdef __linux__
    char buf[16];  // 最多 15 字符 + \0
    snprintf(buf, sizeof(buf), "%.15s", name);
    if (prctl(PR_SET_NAME, (unsigned long)buf, 0, 0, 0) != 0) {
        perror("prctl(PR_SET_NAME) failed");
    }
#else
    fprintf(stderr, "Thread name setting not supported\n");
#endif
}

// 线程函数
void thread_func(ThreadArgs args) {
    const char* name = args.thread_name;
    const std::vector<int>& allowed_cores = *args.allowed_cores;
    auto& core_used = *args.core_used;

    // 选择核心
    int core = select_unused_core_first(allowed_cores, core_used);
    if (core == -1) {
        fprintf(stderr, "[Error] Thread %s failed to select core\n", name);
        return;
    }

    // ✅ 正确方式：使用 pthread_self() 获取当前线程句柄
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);

    pthread_t current_thread = pthread_self();
    if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np failed");
        // 回滚核心占用状态
        auto it = std::find(allowed_cores.begin(), allowed_cores.end(), core);
        if (it != allowed_cores.end()) {
            size_t idx = it - allowed_cores.begin();
            if (idx < core_used.size() && core_used[idx]) {
                (*core_used[idx]) = false;
            }
        }
        return;
    }

    // 设置线程名称
    set_thread_name(name);

    // 打印结果
    printf("[Thread] %s (tid: %lu) bound to CPU %d\n",
           name, static_cast<unsigned long>(syscall(SYS_gettid)), core);

    // 模拟持续运行
    while (true) {
        std::this_thread::yield();
    }
}

int main() {
    // 定义进程绑定的核心区间
    std::vector<std::pair<int, int>> core_ranges = {
        {40, 47},
        {102, 109}
    };

    // 绑定进程核心
    if (!set_process_affinity_ranges(core_ranges)) {
        fprintf(stderr, "[Error] Failed to set process affinity\n");
        return 1;
    }

    // 获取允许的核心列表
    std::vector<int> allowed_cores = get_allowed_cores();
    if (allowed_cores.empty()) {
        fprintf(stderr, "[Error] No allowed cores available\n");
        return 1;
    }

    // 初始化核心使用状态（原子布尔值）
    std::vector<std::unique_ptr<std::atomic<bool>>> core_used;
    for (size_t i = 0; i < allowed_cores.size(); ++i) {
        core_used.push_back(std::unique_ptr<std::atomic<bool>>(new std::atomic<bool>(false)));
    }

    // 打印允许的核心列表
    printf("Allowed cores list: ");
    for (size_t i = 0; i < allowed_cores.size(); ++i) {
        printf("%d", allowed_cores[i]);
        if (i != allowed_cores.size() - 1) printf(", ");
    }
    printf("\nTotal allowed cores: %lu\n", allowed_cores.size());

    // 创建线程
    const int THREAD_NUM = 10;
    std::vector<std::thread> threads;
    const char* thread_names[] = {
        "IO-Thread1", "IO-Thread2", "Net-Thread1", "Net-Thread2", "Worker-1",
        "Worker-2", "Worker-3", "Worker-4", "Worker-5", "Monitor-Thread"
    };

    try {
        for (int i = 0; i < THREAD_NUM; ++i) {
            ThreadArgs args = {
                -1,
                thread_names[i],
                &core_used,
                &allowed_cores
            };
            threads.emplace_back(thread_func, args);
        }

        // 为主线程设置名称（可选）
        set_thread_name("Main-Thread");

        // 等待所有线程结束（实际不会结束）
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "[Error] Thread creation failed: %s\n", e.what());
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        return 1;
    }

    return 0;
}