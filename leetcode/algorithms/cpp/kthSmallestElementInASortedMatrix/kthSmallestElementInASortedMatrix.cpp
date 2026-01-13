//hjx code
//https://leetcode.cn/problems/kth-smallest-element-in-a-sorted-matrix/description/
/*
有序矩阵中第 K 小的元素

给你一个 n x n 矩阵 matrix ，其中每行和每列元素均按升序排序，找到矩阵中第 k 小的元素。
请注意，它是 排序后 的第 k 小元素，而不是第 k 个 不同 的元素。

你必须找到一个内存复杂度优于 O(n2) 的解决方案。

 

示例 1：

输入：matrix = [[1,5,9],[10,11,13],[12,13,15]], k = 8
输出：13
解释：矩阵中的元素为 [1,5,9,10,11,12,13,13,15]，第 8 小元素是 13
示例 2：

输入：matrix = [[-5]], k = 1
输出：-5
 

*/
// 直接将二维数组排序，或者使用最小堆，这个问题的思想就是排序。
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();
        priority_queue<tuple<int, int, int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>>> minHeap;
        for (int i = 0; i < n;i++) {
            minHeap.emplace(matrix[i][0], i, 0);
        }
        int result = 0;
        for (int i =0 ; i < k; i++) {
            auto [v, x, y] = minHeap.top();
            minHeap.pop();
            result = v;
            if (y + 1 != n) {
                minHeap.emplace(matrix[x][y+1], x, y+1);
            }
        }
        return result;
    }
};