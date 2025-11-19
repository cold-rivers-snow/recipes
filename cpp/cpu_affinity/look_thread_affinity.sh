#!/bin/bash

# 替换为目标进程的PID
PID=$1

# 提取进程内所有线程的TID和线程名（跳过表头）
# 输出格式：TID 线程名
ps -T -p $PID | awk 'NR>1 {print $2, $5}' | while read tid name; do
  # 获取该线程绑定的核心列表
  core_list=$(taskset -cp $tid 2>/dev/null | grep -oP 'list: \K.*')
  # 打印结果（TID、线程名、绑定核心）
  echo "TID: $tid, 线程名: $name, 绑定核心: $core_list"
done