#include <iostream>
#include <vector>
// 对选择排序进一步优化，向左侧有序数组中插入元素
// 这个算法有另一个名字，叫做插入排序
void insertSort(std::vector<int>& nums) {
    int n = nums.size();
    // 维护 [0, sortedIndex) 是有序数组
    int sortedIndex = 0;
    while (sortedIndex < n) {
        // 将 nums[sortedIndex] 插入到有序数组 [0, sortedIndex) 中
        for (int i = sortedIndex; i > 0; i--) {
            if (nums[i] < nums[i - 1]) {
                // swap(nums[i], nums[i - 1])
                int tmp = nums[i];
                nums[i] = nums[i - 1];
                nums[i - 1] = tmp;
            } else {
                break;
            }
        }
        sortedIndex++;
    }
}