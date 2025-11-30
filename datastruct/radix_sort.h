#include <vector>
#include <algorithm>
#include <climits>

// 对第 exp 位（1, 10, 100...）进行计数排序
void countingSortForRadix(std::vector<int>& arr, int exp) {
    int n = arr.size();
    std::vector<int> output(n);
    std::vector<int> count(10, 0); // 0~9

    // 统计当前位频次
    for (int i = 0; i < n; ++i) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // 前缀和（变为“<= digit”的个数）
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    // 从后往前填充（保证稳定）
    for (int i = n - 1; i >= 0; --i) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // 写回原数组
    for (int i = 0; i < n; ++i) {
        arr[i] = output[i];
    }
}

// 基数排序（支持负数）
void radixSort(std::vector<int>& nums) {
    if (nums.empty()) return;

    // 1. 处理负数：偏移至非负
    int min_val = *std::min_element(nums.begin(), nums.end());
    int offset = (min_val < 0) ? -min_val : 0;
    if (offset > 0) {
        for (int& x : nums) x += offset;
    }

    // 2. 找最大值以确定位数
    int max_val = *std::max_element(nums.begin(), nums.end());

    // 3. LSD: 从个位开始
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSortForRadix(nums, exp);
    }

    // 4. 恢复负数
    if (offset > 0) {
        for (int& x : nums) x -= offset;
    }
}