# cpu_affinity

## 进程与线程的 CPU 亲和性

进程亲和性：限制整个进程（包括其所有线程）只能在指定 CPU 核心集合上运行。
线程亲和性：更精细地限制单个线程在指定核心上运行（可覆盖进程亲和性）。
两者的实现均依赖操作系统提供的系统调用（Linux 中主要是 sched_setaffinity 系列函数）。

## lscpu 查看 cpu 核心数

核心编号有效性：绑定的核心编号不能超过系统实际核心数（可通过 lscpu 查看，核心编号从 0 开始），否则绑定失败。

进程启动默认不绑定核心，可通过 `taskset -p` 命令手动绑定。

您提供的信息：

```
NUMA node0 CPU(s):     0-7,64-71
NUMA node1 CPU(s):     8-15,72-79
...
NUMA node7 CPU(s):     56-63,120-127
```

这是一台典型的 **多插槽（Multi-Socket）、超线程（Hyper-Threading）服务器** 的 NUMA 架构输出。下面我们来**逐层解释 NUMA 的含义、为什么有 8 个节点、CPU 区间的意义，以及绑定时的影响范围**。

## 🔤 一、什么是 NUMA？

### ✅ NUMA 全称：**Non-Uniform Memory Access**（非一致性内存访问）

### 📌 核心思想：
> 在多 CPU 插槽的服务器中，**每个 CPU 插槽连接一部分本地内存（Local Memory）**。当一个 CPU 核心访问**自己所属插槽的内存**时，速度最快；访问**其他插槽的内存**时，需要通过互联总线（如 Intel 的 UPI 或 AMD 的 Infinity Fabric），延迟更高、带宽更低。

| 访问类型 | 速度 | 延迟 |
|--------|------|------|
| 访问本地内存（Local Memory） | 快 | 低 |
| 访问远程内存（Remote Memory） | 慢 | 高 |

这就是“非一致性”（Non-Uniform）的由来。

## lscpu

``` bash
# lscpu
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                128
On-line CPU(s) list:   0-127
Thread(s) per core:    2
Core(s) per socket:    32
Socket(s):             2
NUMA node(s):          8
Vendor ID:             HygonGenuine
CPU family:            24
Model:                 1
Model name:            Hygon C86 7280 32-core Processor
Stepping:              1
CPU MHz:               2490.778
BogoMIPS:              3999.40
Virtualization:        AMD-V
L1d cache:             32K
L1i cache:             64K
L2 cache:              512K
L3 cache:              8192K
NUMA node0 CPU(s):     0-7,64-71
NUMA node1 CPU(s):     8-15,72-79
NUMA node2 CPU(s):     16-23,80-87
NUMA node3 CPU(s):     24-31,88-95
NUMA node4 CPU(s):     32-39,96-103
NUMA node5 CPU(s):     40-47,104-111
NUMA node6 CPU(s):     48-55,112-119
NUMA node7 CPU(s):     56-63,120-127
Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_tsc rep_good nopl nonstop_tsc cpuid extd_apicid amd_dcm aperfmperf pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c rdrand lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw skinit wdt tce topoext perfctr_core perfctr_nb bpext perfctr_llc mwaitx cpb hw_pstate sme ssbd sev ibpb vmmcall fsgsbase bmi1 avx2 smep bmi2 rdseed adx smap clflushopt sha_ni xsaveopt xsavec xgetbv1 xsaves clzero irperf xsaveerptr arat npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold avic v_vmsave_vmload vgif overflow_recov succor smca
```

这台服务器有两个 socket（ CPU 物理芯片），八个 NUMA node。每个 socket 有 32 个 CPU Core，每个 Core 有两个 Hyper thread，所以每个socket有64个CPU，共128个CPU。每个 socket 有一个8MB大小的L3 Cache，每个 socket 有一个0.5MB大小的L2 Cache，和 64KB 的 L1 I-Cache和 32K 的 L1 D-Cache。

```bash
$ sudo numactl -H
available: 8 nodes (0-7)
node 0 cpus: 0 1 2 3 4 5 6 7 64 65 66 67 68 69 70 71
node 0 size: 15298 MB
node 0 free: 120 MB
node 1 cpus: 8 9 10 11 12 13 14 15 72 73 74 75 76 77 78 79
node 1 size: 16047 MB
node 1 free: 55 MB
node 2 cpus: 16 17 18 19 20 21 22 23 80 81 82 83 84 85 86 87
node 2 size: 16035 MB
node 2 free: 43 MB
node 3 cpus: 24 25 26 27 28 29 30 31 88 89 90 91 92 93 94 95
node 3 size: 16033 MB
node 3 free: 43 MB
node 4 cpus: 32 33 34 35 36 37 38 39 96 97 98 99 100 101 102 103
node 4 size: 15995 MB
node 4 free: 51 MB
node 5 cpus: 40 41 42 43 44 45 46 47 104 105 106 107 108 109 110 111
node 5 size: 15947 MB
node 5 free: 62 MB
node 6 cpus: 48 49 50 51 52 53 54 55 112 113 114 115 116 117 118 119
node 6 size: 15975 MB
node 6 free: 210 MB
node 7 cpus: 56 57 58 59 60 61 62 63 120 121 122 123 124 125 126 127
node 7 size: 15991 MB
node 7 free: 294 MB
node distances:
node   0   1   2   3   4   5   6   7
  0:  10  16  16  16  28  28  22  28
  1:  16  10  16  16  28  28  28  22
  2:  16  16  10  16  22  28  28  28
  3:  16  16  16  10  28  22  28  28
  4:  28  28  22  28  10  16  16  16
  5:  28  28  28  22  16  10  16  16
  6:  22  28  28  28  16  16  10  16
  7:  28  22  28  28  16  16  16  10
```

等价于 numactl --hardware 各个 numa 上的核心值，内存总空间大小和可用空间大小，每个 NUMA node 有 16 个逻辑 CPU（8 个物理核心 × 2 超线程）

有单独的 numa 库，可以自适应获取 numa 的个数 numa_num_task_nodes()

### 🔢 NUMA node CPU 区间 `0-7,64-71` 解释

以 `NUMA node0: 0-7,64-71` 为例：

- `0-7`：这是 **8 个物理核心**（Core 0 到 Core 7）
- `64-71`：这是 **超线程对应的逻辑核心**（Hyper-Threading）
  - Core 0 的第二个线程 → CPU 64
  - Core 1 的第二个线程 → CPU 65
  - ...
  - Core 7 的第二个线程 → CPU 71

> 💡 **规律**：在某些 BIOS 设置中，Linux 会把所有物理核排在前面，所有超线程核排在后面（称为 `clustered` 或 `symmetric` 调度模式）。

所以：
- CPU 0~63：前 8 个 NUMA 节点的物理核（每节点 8 个）
- CPU 64~127：所有 NUMA 节点的超线程核（每节点 8 个）

---

## 🧵 绑定 CPU 时，“绑定的是整个进程”吗？

### ✅ 是的，但要分情况：

### 1. **进程 vs 线程**
- 一个进程可以有多个线程。
- 当你说“绑定进程”，实际上是**绑定该进程的所有线程**，除非线程内部重新设置了亲和性。

### 2. **绑定方式决定范围**

| 绑定方式 | 影响范围 |
|---------|----------|
| `taskset -c 0-7 ./my_program` | 整个进程及其所有线程只能在 CPU 0-7 上运行 |
| `pthread_setaffinity_np()` 在某个线程中调用 | 仅该线程被绑定，其他线程不受影响 |
| `systemd CPUAffinity=0-7` | 该服务所有进程和线程都绑定到 0-7 |

### 3. **推荐做法（高性能场景）**
```bash
# 示例：将 Ceph OSD 进程绑定到 NUMA node0 的所有核心（本地内存 + 本地 CPU）
numactl --cpunodebind=0 --membind=0 ceph-osd --id 0
```

这表示：
- **CPU 绑定**：只使用 NUMA node0 的 CPU（0-7,64-71）
- **内存绑定**：只从 NUMA node0 的本地内存分配内存，避免跨节点访问

---

## 🎯 五、NUMA 架构下的最佳实践建议

### ✅ 1. **进程/服务绑定到单个 NUMA 节点**

- 避免跨 NUMA 访问内存
- 提升缓存和内存带宽利用率

```bash
numactl --cpunodebind=0 --membind=0 ./my_high_performance_app
```

### ✅ 2. **为关键服务预留 CPU 核心**

- 使用 `isolcpus=8-127` 内核参数，把 0-7 留给系统，8-127 给应用专用
- 避免调度干扰

### ✅ 3. **Ceph / 数据库 / 高性能存储建议**

- 每个 进程绑定到独立 NUMA 节点
- 使用 `--membind` 强制使用本地内存
- 中断（IRQ）也绑定到对应 NUMA 节点

### ✅ 4. **容器/K8s 中使用 `cpuset`**

Kubelet 配置：启用 static CPU 管理策略

这个策略 必须在 Kubelet 启动时配置，不能通过 Pod 配置。

```yaml
# kubelet 配置文件（如 config.yaml）
apiVersion: kubelet.config.k8s.io/v1beta1
kind: KubeletConfiguration
cpuManagerPolicy: static
cpuManagerReconcilePeriod: 10s
```

```yaml
resources:
  limits:
    memory: 32Gi
    cpu: 16
  requests:
    memory: 32Gi
    cpu: 16
  # 使用 static 调度器 + cpuset，实现 CPU 绑核
```

#### 查看容器内可见的 CPU

```bash
cat /sys/fs/cgroup/cpuset/cpuset.cpus
# 输出示例：32-47
```

#### 在宿主机上查看

```bash
# 找到容器 PID
docker inspect <container_id> | grep Pid

# 查看该进程的 CPU 亲和性
taskset -p <pid>
# 输出示例：pid 1234's current affinity mask: ffff00000000
# 表示只允许在 CPU 32-47 运行
```

## ✅ 总结

| 问题 | 回答 |
|------|------|
| **NUMA 是什么？** | 多 CPU 插槽下，内存访问延迟不一致的架构 |
| **CPU 区间 `0-7,64-71` 含义？** | 0-7 是物理核，64-71 是超线程核（HT） |
| **绑定是整个进程吗？** | 是，除非线程单独设置亲和性 |
| **最佳绑定策略？** | `numactl --cpunodebind=N --membind=N` 绑定到单个 NUMA 节点 |

📌 **一句话建议**：  
在高性能场景（如 Ceph、数据库、AI 训练），**务必让进程运行在某个 NUMA 节点，并从该节点分配内存**，否则性能可能下降 10%~30%。

## 查看是否绑核

1. 查看进程的 CPU 亲和性（所有线程的默认绑定）

```bash
# 语法：taskset -cp <进程PID>
taskset -cp 12345  # 替换为实际进程的PID
```

2. 查看单个线程的 CPU 亲和性

需先获取线程的 TID（线程 ID），再用 taskset 查看：

```bash
# 步骤1：查看进程内所有线程的TID（以PID=12345为例）
ps -T -p 12345
# 输出示例：
  PID  SPID TTY          TIME CMD
12345 12345 ?        00:00:00 java
12345 12346 ?        00:00:05 IO-Thread  # SPID即TID
12345 12347 ?        00:00:03 Net-Thread

# 步骤2：查看指定线程（如TID=12346）的绑定核心
taskset -cp 12346
# 输出示例：
  pid 12346's current affinity list: 1  # 表示该线程绑定到核心1
```

3. ps 命令结合 taskset（快速筛选线程）

若只需查看进程内是否有线程绑定到特定核心，可组合使用 ps 和 taskset：

```bash
# 查看PID=12345进程内所有线程的绑定核心
for tid in $(ps -T -p 12345 | awk 'NR>1 {print $2}'); do
  echo "TID: $tid, 绑定核心: $(taskset -cp $tid | grep -oP 'list: \K.*')"
done
```

```bash
#!/bin/bash

# 替换为目标进程的PID
PID=12345

# 提取进程内所有线程的TID和线程名（跳过表头）
# 输出格式：TID 线程名
ps -T -p $PID | awk 'NR>1 {print $2, $5}' | while read tid name; do
  # 获取该线程绑定的核心列表
  core_list=$(taskset -cp $tid 2>/dev/null | grep -oP 'list: \K.*')
  # 打印结果（TID、线程名、绑定核心）
  echo "TID: $tid, 线程名: $name, 绑定核心: $core_list"
done
```

## Reference

https://www.glennklockwood.com/hpc-howtos/process-affinity.html
https://www.admin-magazine.com/HPC/Articles/Processor-Affinity-for-OpenMP-and-MPI
https://blog.bluebird.icu/posts/kube-numa/
https://developer.aliyun.com/article/784148
https://juejin.cn/post/7501589510267387904
https://www.man7.org/linux/man-pages/man3/
