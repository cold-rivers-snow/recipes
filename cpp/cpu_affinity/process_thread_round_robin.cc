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
#include <string>            // std::string

// 线程优先级枚举
enum class ThreadPriority {
    High,   // 主线程助手、关键路径
    Medium, // 工作线程
    Low     // IO、网络、监控等
};

// 线程参数结构体（使用 std::string 避免 c_str() 悬空问题）
struct ThreadArgs {
    std::string thread_name;
    ThreadPriority priority;
    const std::vector<int>* allowed_cores;
    std::vector<int>* core_assignment_count;
    int max_threads_per_core;
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
    const std::string& name = args.thread_name;
    ThreadPriority priority = args.priority;
    const std::vector<int>& allowed_cores = *args.allowed_cores;
    std::vector<int>& core_assignment_count = *args.core_assignment_count;
    int max_threads_per_core = args.max_threads_per_core;

    // 使用静态原子变量实现 round-robin 分配
    static std::atomic<size_t> next_core_index{0};
    int selected_core = -1;
    size_t attempts = 0;
    size_t total_cores = allowed_cores.size();

    while (attempts < total_cores * 2 && selected_core == -1) {
        size_t idx = (next_core_index++) % total_cores;
        int core = allowed_cores[idx];

        // 检查该核心是否还能接受更多线程
        if (core_assignment_count[idx] < max_threads_per_core) {
            ++core_assignment_count[idx];
            selected_core = core;
            break;
        }
        ++attempts;
    }

    if (selected_core == -1) {
        fprintf(stderr, "[Error] No available core for thread %s under limit %d\n",
                name.c_str(), max_threads_per_core);
        return;
    }

    // 绑定线程到选定核心
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(selected_core, &cpuset);

    pthread_t current_thread = pthread_self();
    if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np failed");
        // 回滚计数
        auto it = std::find(allowed_cores.begin(), allowed_cores.end(), selected_core);
        if (it != allowed_cores.end()) {
            size_t idx = it - allowed_cores.begin();
            --core_assignment_count[idx];
        }
        return;
    }

    // 设置线程名
    set_thread_name(name.c_str());

    // 打印信息（将优先级映射为可读字符串）
    const char* prio_str;
    switch (priority) {
        case ThreadPriority::High:   prio_str = "High";   break;
        case ThreadPriority::Medium: prio_str = "Medium"; break;
        case ThreadPriority::Low:    prio_str = "Low";    break;
        default:                     prio_str = "Unknown";
    }

    printf("[Thread] %-14s (tid: %lu, prio: %s) bound to CPU %d\n",
           name.c_str(),
           static_cast<unsigned long>(syscall(SYS_gettid)),
           prio_str,
           selected_core);

    // 模拟持续运行
    while (true) {
        std::this_thread::yield();
    }
}

int main() {
    // 定义进程绑定的核心区间
    std::vector<std::pair<int, int>> core_ranges = {
        {40, 47},   // 8 cores
        {102, 109}  // 8 cores
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

    int total_allowed_cores = allowed_cores.size();
    int max_threads_per_core = 2;
    int total_threads = total_allowed_cores * max_threads_per_core;

    printf("Allowed cores list: ");
    for (size_t i = 0; i < allowed_cores.size(); ++i) {
        printf("%d", allowed_cores[i]);
        if (i != allowed_cores.size() - 1) printf(", ");
    }
    printf("\n");
    printf("Total allowed cores: %d\n", total_allowed_cores);
    printf("Max threads per core: %d\n", max_threads_per_core);
    printf("Total threads to create: %d\n", total_threads);

    // 初始化每个核心的线程分配计数
    std::vector<int> core_assignment_count(total_allowed_cores, 0);

    // 定义线程规格：名称 + 优先级
    struct ThreadSpec {
        std::string name;
        ThreadPriority priority;
    };

    std::vector<ThreadSpec> thread_specs;

    // 按优先级顺序生成线程（高 -> 中 -> 低）
    for (int i = 0; i < total_threads; ++i) {
        if (i % 3 == 0) {
            thread_specs.push_back({ "Main-Help-" + std::to_string(i/3), ThreadPriority::High });
        } else if (i % 3 == 1) {
            thread_specs.push_back({ "Worker-" + std::to_string(i), ThreadPriority::Medium });
        } else {
            thread_specs.push_back({ "IO-Monitor-" + std::to_string(i), ThreadPriority::Low });
        }
    }

    // 按优先级排序：High > Medium > Low
    std::sort(thread_specs.begin(), thread_specs.end(),
              [](const ThreadSpec& a, const ThreadSpec& b) {
                  return static_cast<int>(a.priority) < static_cast<int>(b.priority);
              });

    // 创建线程
    std::vector<std::thread> threads;
    try {
        for (const auto& spec : thread_specs) {
            ThreadArgs args{
                spec.name,
                spec.priority,
                &allowed_cores,
                &core_assignment_count,
                max_threads_per_core
            };
            threads.emplace_back(thread_func, args);
        }

        // 为主线程设置名称
        set_thread_name("Main-Thread");

        printf("All %zu threads created and scheduled.\n", threads.size());

        // 等待所有线程（实际不会结束）
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