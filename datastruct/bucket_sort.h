#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>
#include <algorithm>
#include <cmath>

// 插入排序（用于小数组）
inline void insertionSort(std::vector<double>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        double key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// 桶排序（支持任意范围浮点数）
inline void bucketSort(std::vector<double>& nums) {
    if (nums.empty()) return;

    // 1. 找到最大最小值
    double min_val = nums[0];
    double max_val = nums[0];
    for (double x : nums) {
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }

    if (min_val == max_val) return;

    int n = nums.size();
    std::vector<std::vector<double>> buckets(n); // 创建 n 个桶

    // 2. 分配元素到桶
    double range = max_val - min_val;
    for (double x : nums) {
        // 映射到 [0, n-1]
        int idx = static_cast<int>((x - min_val) / range * n);
        if (idx >= n) idx = n - 1;
        if (idx < 0) idx = 0;
        buckets[idx].push_back(x);
    }

    // 3. 对每个桶排序
    for (auto& bucket : buckets) {
        insertionSort(bucket);
    }

    // 4. 合并桶
    int index = 0;
    for (const auto& bucket : buckets) {
        for (double x : bucket) {
            nums[index++] = x;
        }
    }
}

#endif // BUCKET_SORT_H