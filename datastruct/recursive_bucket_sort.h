#ifndef RECURSIVE_BUCKET_SORT_H
#define RECURSIVE_BUCKET_SORT_H

#include <vector>
#include <algorithm>
#include "bucket_sort.h" // Need insertionSort

inline void recursiveBucketSort(std::vector<double>& nums, int depth = 0) {
    const int MAX_DEPTH = 5; // 防止无限递归
    if (nums.size() <= 1 || depth >= MAX_DEPTH) {
        insertionSort(nums);
        return;
    }

    // 1. 找到最大最小值
    double min_val = nums[0];
    double max_val = nums[0];
    for (double x : nums) {
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }

    if (min_val == max_val) return;

    int n = nums.size();
    std::vector<std::vector<double>> buckets(n);

    double range = max_val - min_val;
    for (double x : nums) {
        int idx = static_cast<int>((x - min_val) / range * n);
        if (idx >= n) idx = n - 1;
        if (idx < 0) idx = 0;
        buckets[idx].push_back(x);
    }

    nums.clear();
    for (auto& bucket : buckets) {
        if (!bucket.empty()) {
            recursiveBucketSort(bucket, depth + 1);
            nums.insert(nums.end(), bucket.begin(), bucket.end());
        }
    }
}

#endif // RECURSIVE_BUCKET_SORT_H