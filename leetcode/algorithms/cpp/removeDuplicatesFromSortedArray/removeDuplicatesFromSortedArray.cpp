// Source : https://oj.leetcode.com/problems/remove-duplicates-from-sorted-array/
// Author : Hao Chen
// Date   : 2014-06-22

/********************************************************************************** 
* 
* Given a sorted array, remove the duplicates in place such that each element appear 
* only once and return the new length.
* 
* Do not allocate extra space for another array, you must do this in place with constant memory.
* 
* For example,
* Given input array A = [1,1,2],
* 
* Your function should return length = 2, and A is now [1,2].
* 
*               
**********************************************************************************/

class Solution {
public:
    int removeDuplicates(int A[], int n) {
        
        if (n<=1) return n;
        
        int pos=0;
        for(int i=0; i<n-1; i++){
            if (A[i]!=A[i+1]){
                A[++pos] = A[i+1];
            }
        }
        return pos+1;
    }
};

//hjx code
// 快指针一直往前走，慢指针只在不重复的时候往前走，修改原数组
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size() == 0) {
            return 0;
        }
        int slow = 0, fast = 0;
        while(fast < nums.size()) {
            if (nums[slow] != nums[fast]) {
                slow++;
                nums[slow] = nums[fast];
            }
            fast++;
        }
        return slow + 1;
    }
};