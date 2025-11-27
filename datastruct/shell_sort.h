#include <iostream>
#include <vector>

void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    
    // 1. 生成 Knuth 序列: 1, 4, 13, 40, ...
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;  // h = (3^k - 1) / 2
    }

    // 2. 从大到小使用 h 序列
    while (h >= 1) {
        // 对每个子序列执行插入排序
        for (int i = h; i < n; ++i) {
            int key = arr[i];
            int j = i;
            // 在子序列中向前比较（步长为 h）
            while (j >= h && arr[j - h] > key) {
                arr[j] = arr[j - h];
                j -= h;
            }
            arr[j] = key;
        }
        h /= 3;  // 下一个更小的 h
    }
}