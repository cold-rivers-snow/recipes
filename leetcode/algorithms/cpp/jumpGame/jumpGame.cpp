// Source : https://oj.leetcode.com/problems/jump-game/
// Author : Hao Chen
// Date   : 2014-06-27

/********************************************************************************** 
* 
* Given an array of non-negative integers, you are initially positioned at the first index of the array.
* 
* Each element in the array represents your maximum jump length at that position. 
* 
* Determine if you are able to reach the last index.
* 
* For example:
* A = [2,3,1,1,4], return true.
* 
* A = [3,2,1,0,4], return false.
* 
*               
**********************************************************************************/

class Solution {
public:
    bool canJump(int A[], int n) {
        if (n<=0) return false;
        
        // the basic idea is traverse array, maintain the most far can go
        int coverPos=0;
        // the condition i<=coverPos means traverse all of covered position 
        for(int i=0; i<=coverPos && i<n; i++){
            
            if (coverPos < A[i] + i){
                coverPos = A[i] + i;
            }
            
            if (coverPos>=n-1){
                return true;
            }
        }
        return false;
    }
};

//hjx code
//贪心算法，每次都选择能跳的最远的，只要最远的值大于当前下标，即可继续跳。
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        int fastJamp = 0;
        for (int i = 0; i < n; i++) {
            if (fastJamp < i) {
                return false;
            }
            fastJamp = max(fastJamp, i + nums[i]);
            if (fastJamp >= n - 1) {
                return true;
            }
        }
        return true;
    }
};