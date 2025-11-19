#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

// 绑定当前进程到指定CPU核心（core：核心编号，如0、1、2）
int bind_process_to_core(int core) {
  cpu_set_t cpuset;       // CPU核心集合
  CPU_ZERO(&cpuset);     // 初始化集合
  CPU_SET(core, &cpuset); // 将指定核心加入集合

  // 获取当前进程ID
  pid_t pid = getpid();

  // 设置进程亲和性：仅允许在cpuset指定的核心上运行
  if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset) == -1) {
      perror("sched_setaffinity failed");
      return -1;
  }

  printf("Process %d bound to CPU %d\n", pid, core);
  return 0;
}

int main() {
  // 进程启动时绑定到CPU 0
  if (bind_process_to_core(0) != 0) {
      return 1;
  }

  // 后续业务逻辑...
  while (1) {
      sleep(1);
  }
  return 0;
}

//g++ -o process_affinity process_affinity.cc -lpthread