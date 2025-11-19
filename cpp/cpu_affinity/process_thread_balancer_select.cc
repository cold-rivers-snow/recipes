#include <pthread.h>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>  // 包含setpriority相关定义

// 高优先级可用核心池
const std::vector<int> HIGH_AVAILABLE_CORES = {40, 41, 42, 43, 44, 45, 46, 47};
const int REBALANCE_INTERVAL = 10000; // 负载再平衡间隔（毫秒）

// 核心负载信息
struct CoreLoad {
    int core_id;
    double idle_rate; // 空闲率（越高越空闲）
};

// 读取/proc/stat获取各核心负载
std::vector<CoreLoad> get_core_loads() {
    std::vector<CoreLoad> core_loads;
    std::ifstream proc_stat("/proc/stat");
    std::string line;

    while (std::getline(proc_stat, line)) {
        if (line.substr(0, 3) != "cpu") continue;

        std::istringstream iss(line);
        std::string cpu_label;
        iss >> cpu_label;

        if (cpu_label == "cpu") continue; // 跳过总览行
        int core_id = std::stoi(cpu_label.substr(3));

        if (std::find(HIGH_AVAILABLE_CORES.begin(), HIGH_AVAILABLE_CORES.end(), core_id) == HIGH_AVAILABLE_CORES.end()) {
            continue;
        }

        unsigned long long user, nice, system, idle, iowait, irq, softirq;
        iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

        unsigned long long total = user + nice + system + idle + iowait + irq + softirq;
        double idle_rate = static_cast<double>(idle) / total * 100.0;
        core_loads.push_back({core_id, idle_rate});
    }

    std::sort(core_loads.begin(), core_loads.end(), [](const CoreLoad& a, const CoreLoad& b) {
        return a.idle_rate > b.idle_rate;
    });

    return core_loads;
}

// 绑定线程到指定核心
bool set_thread_affinity(pthread_t thread, int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    int ret = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (ret != 0) {
        std::cerr << "线程绑定核心" << core_id << "失败：" << strerror(ret) << "\n";
        return false;
    }
    return true;
}

// 设置线程优先级（兼容root和普通用户）
bool set_thread_priority(pthread_t thread) {
    bool is_root = (getuid() == 0);

    if (is_root) {
        // Root用户：使用实时调度策略SCHED_RR
        struct sched_param param;
        param.sched_priority = 50; // 1~99范围内的实时优先级
        int ret = pthread_setschedparam(thread, SCHED_RR, &param);
        if (ret != 0) {
            std::cerr << "Root用户设置实时优先级失败：" << strerror(ret) << "\n";
            return false;
        }
        return true;
    } else {
        // 普通用户：使用PRIO_PROCESS替代PRIO_THREAD（Linux兼容写法）
        int nice_value = 0; // 普通用户最高可用nice值
        // 注意：pthread_t本质是线程ID，可作为setpriority的ID参数
        int ret = setpriority(PRIO_PROCESS, thread, nice_value);
        if (ret != 0) {
            std::cerr << "普通用户设置nice值失败：" << strerror(errno) << "\n";
            return false;
        }
        return true;
    }
}

// 动态选核：选择最空闲核心
int select_best_core() {
    std::vector<CoreLoad> core_loads = get_core_loads();
    if (core_loads.empty()) {
        return HIGH_AVAILABLE_CORES[0];
    }
    if (core_loads.size() >= 2 && (core_loads[0].idle_rate - core_loads[1].idle_rate) < 5.0) {
        return core_loads[rand() % 2].core_id;
    }
    return core_loads[0].core_id;
}

// 线程池类
class DynamicThreadPool {
public:
    DynamicThreadPool(size_t thread_count) : thread_count_(thread_count) {
        start_threads();
        start_rebalance_thread();
    }

    ~DynamicThreadPool() {
        stop_ = true;
        rebalance_stop_ = true;
        if (rebalance_thread_.joinable()) {
            rebalance_thread_.join();
        }
        for (auto& t : threads_) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

private:
    void start_threads() {
        for (size_t i = 0; i < thread_count_; ++i) {
            threads_.emplace_back(&DynamicThreadPool::thread_func, this, i);
        }
    }

    void thread_func(size_t thread_id) {
        pthread_t thread = pthread_self();
        int current_core = select_best_core();

        {
            std::lock_guard<std::mutex> lock(core_map_mutex_);
            thread_core_map_[thread_id] = current_core;
        }

        if (!set_thread_affinity(thread, current_core)) {
            std::cerr << "线程" << thread_id << "绑定核心失败\n";
        }
        if (!set_thread_priority(thread)) {
            std::cerr << "线程" << thread_id << "设置优先级失败\n";
        }

        while (!stop_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void rebalance_thread() {
        while (!rebalance_stop_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(REBALANCE_INTERVAL));
            std::vector<CoreLoad> core_loads = get_core_loads();
            if (core_loads.empty()) continue;

            std::vector<int> overloaded_cores;
            for (const auto& cl : core_loads) {
                if (cl.idle_rate < 20.0) {
                    overloaded_cores.push_back(cl.core_id);
                }
            }
            if (overloaded_cores.empty()) continue;

            std::lock_guard<std::mutex> lock(core_map_mutex_);
            for (auto& entry : thread_core_map_) {
                size_t thread_id = entry.first;
                int core_id = entry.second;

                if (std::find(overloaded_cores.begin(), overloaded_cores.end(), core_id) != overloaded_cores.end()) {
                    int new_core = select_best_core();
                    pthread_t thread = threads_[thread_id].native_handle();
                    if (set_thread_affinity(thread, new_core)) {
                        thread_core_map_[thread_id] = new_core;
                        std::cout << "线程" << thread_id << "从核心" << core_id << "迁移到核心" << new_core << "\n";
                    }
                }
            }
        }
    }

    void start_rebalance_thread() {
        rebalance_thread_ = std::thread(&DynamicThreadPool::rebalance_thread, this);
    }

    size_t thread_count_;
    std::vector<std::thread> threads_;
    std::atomic<bool> stop_{false};

    std::unordered_map<size_t, int> thread_core_map_;
    std::mutex core_map_mutex_;

    std::thread rebalance_thread_;
    std::atomic<bool> rebalance_stop_{false};
};

// 单线程动态分配
void start_dynamic_single_thread(void (*task)()) {
    std::thread t([task]() {
        pthread_t thread = pthread_self();
        int core = select_best_core();
        set_thread_affinity(thread, core);
        set_thread_priority(thread);
        task();
    });
    t.detach();
}

// 示例任务
void high_priority_task() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    DynamicThreadPool pool1(100);
    DynamicThreadPool pool2(100);
    DynamicThreadPool pool3(100);
    DynamicThreadPool pool4(2128);
    DynamicThreadPool pool5(222);

    start_dynamic_single_thread(high_priority_task);

    std::cout << "高优先级动态线程池启动完成（用户ID：" << getuid() << "），按任意键退出...\n";
    std::cin.get();
    return 0;
}