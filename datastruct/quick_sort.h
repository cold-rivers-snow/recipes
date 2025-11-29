#include <vector>
#include <algorithm> // for std::swap

// 分区函数：将 arr[lo..hi] 分区，返回 pivot 的最终索引
int partition(std::vector<int>& arr, int lo, int hi) {
    int pivot = arr[hi];          // 选择最后一个元素作为 pivot
    int i = lo;                   // i 是“小于 pivot 区域”的右边界（初始为 lo）

    for (int j = lo; j < hi; ++j) {
        if (arr[j] < pivot) {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    std::swap(arr[i], arr[hi]);   // 将 pivot 放到正确位置
    return i;
}

// 快速排序主函数
void quickSort(std::vector<int>& arr, int lo, int hi) {
    if (lo >= hi) return;         // 基线条件：子数组长度 <= 1

    int p = partition(arr, lo, hi); // 前序位置：确定 pivot 位置
    quickSort(arr, lo, p - 1);    // 递归左子数组
    quickSort(arr, p + 1, hi);    // 递归右子数组
}

// 便捷调用接口
void quickSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    quickSort(arr, 0, arr.size() - 1);
}