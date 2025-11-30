#include <vector>

// 合并两个有序子数组 arr[lo..mid] 和 arr[mid+1..hi]
void merge(std::vector<int>& arr, int lo, int mid, int hi) {
    // 1. 复制到临时数组
    std::vector<int> temp(arr.begin() + lo, arr.begin() + hi + 1);
    
    // 2. 双指针合并
    int i = 0;                 // temp 左半起始索引 (对应 arr[lo])
    int j = mid - lo + 1;      // temp 右半起始索引 (对应 arr[mid+1])
    int k = lo;                // 原数组写入位置

    while (i <= mid - lo && j <= hi - lo) {
        if (temp[i] <= temp[j]) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = temp[j++];
        }
    }

    // 3. 拷回剩余元素（左半或右半）
    while (i <= mid - lo) arr[k++] = temp[i++];
    while (j <= hi - lo) arr[k++] = temp[j++];
}

// 归并排序主函数
void mergeSort(std::vector<int>& arr, int lo, int hi) {
    if (lo >= hi) return;               // 基线条件：子数组长度 <= 1

    int mid = lo + (hi - lo) / 2;       // 防止溢出
    mergeSort(arr, lo, mid);            // 递归左半
    mergeSort(arr, mid + 1, hi);        // 递归右半
    merge(arr, lo, mid, hi);            // 后序位置：合并
}

// 便捷调用接口
void mergeSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    mergeSort(arr, 0, arr.size() - 1);
}