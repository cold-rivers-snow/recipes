//hjx code
/*
查找和最小的 K 对数字

给定两个以 非递减顺序排列 的整数数组 nums1 和 nums2 , 以及一个整数 k 。

定义一对值 (u,v)，其中第一个元素来自 nums1，第二个元素来自 nums2 。

请找到和最小的 k 个数对 (u1,v1),  (u2,v2)  ...  (uk,vk) 。

 

示例 1:

输入: nums1 = [1,7,11], nums2 = [2,4,6], k = 3
输出: [1,2],[1,4],[1,6]
解释: 返回序列中的前 3 对数：
     [1,2],[1,4],[1,6],[7,2],[7,4],[11,2],[7,6],[11,4],[11,6]
示例 2:

输入: nums1 = [1,1,2], nums2 = [1,2,3], k = 2
输出: [1,1],[1,1]
解释: 返回序列中的前 2 对数：
     [1,1],[1,1],[1,2],[2,1],[1,2],[2,2],[1,3],[1,3],[2,3]

*/

//由于每个数组是升序的，所以每次放是放每一行的第 0 列。通过 二叉堆 排序，得到一个排序。

class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<vector<int>> result;
        int m = nums1.size(), n = nums2.size();
        if (m == 0 || n == 0 || k == 0) {
            return result;
        }
        priority_queue<tuple<int, int, int>, vector<tuple<int, int,int>>, greater<tuple<int, int, int>>> q;
        for (int i = 0; i < min(m, k);i++) {
            q.emplace(nums1[i] + nums2[0], i, 0);
        }
        while(!q.empty() && result.size() < k) {
            auto [sum, i ,j] = q.top();
            result.push_back(vector<int>{nums1[i], nums2[j]});
            q.pop();
            if(j + 1 < n) {
                q.emplace(nums1[i]+nums2[j+1], i, j+1);
            }
        }
        return result;
    }
};