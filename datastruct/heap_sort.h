#include <vector>
#include <algorithm>

void sink(std::vector<int>& arr, int k, int n) {
    while (2 * k + 1 < n) {
        int j = 2 * k + 1;
        if (j + 1 < n && arr[j] < arr[j + 1]) j++;
        if (arr[k] >= arr[j]) break;
        std::swap(arr[k], arr[j]);
        k = j;
    }
}

void heapSort(std::vector<int>& nums) {
    int n = nums.size();
    // 建堆
    for (int i = n / 2 - 1; i >= 0; --i)
        sink(nums, i, n);
    // 排序
    for (int i = n - 1; i > 0; --i) {
        std::swap(nums[0], nums[i]);
        sink(nums, 0, i);
    }
}