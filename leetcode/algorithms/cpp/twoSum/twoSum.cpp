// Source : https://oj.leetcode.com/problems/two-sum/
// Author : Hao Chen
// Date   : 2014-06-17

/********************************************************************************** 
* 
* Given an array of integers, find two numbers such that they add up to a specific target number.
* 
* The function twoSum should return indices of the two numbers such that they add up to the target, 
* where index1 must be less than index2. Please note that your returned answers (both index1 and index2) 
* are not zero-based.
* 
* You may assume that each input would have exactly one solution.
* 
* Input: numbers={2, 7, 11, 15}, target=9
* Output: index1=1, index2=2
* 
*               
**********************************************************************************/

class Solution {
public:
    /*
     *   The easy solution is O(n^2) run-time complexity.
     *   ```
     *       foreach(item1 in array) {
     *           foreach(item2 in array){
     *               if (item1 + item2 == target) {
     *                   return result
     *               }
     *           }
     *   ```
     *   
     *   We can see the nested loop just for searching, 
     *   So, we can use a hashmap to reduce the searching time complexity from O(n) to O(1)
     *   (the map's `key` is the number, the `value` is the position)
     *   
     *   But be careful, if there are duplication numbers in array, 
     *   how the map store the positions for all of same numbers?
     *
     */


    //下·
    // The implementation as below is bit tricky. but not difficult to understand
    //
    //  1) Traverse the array one by one
    //  2) just put the `target - num[i]`(not `num[i]`) into the map
    //     so, when we checking the next num[i], if we found it existed in the map,
    //     it means we found the second one.
    //      
    vector<int> twoSum(vector<int> &numbers, int target) {
        unordered_map<int, int> m;
        vector<int> result;
        for(int i=0; i<numbers.size(); i++){
            // not found the second one
            if (m.find(numbers[i])==m.end() ) { 
                // store the first one position into the second one's key
                m[target - numbers[i]] = i; 
            }else { 
                // found the second one
                result.push_back(m[numbers[i]]+1);  //这两行应该没有+1操作。
                result.push_back(i+1);
                break;
            }
        }
        return result;
    }

    // we also can store nums[i] into map, and find target - nums[i]
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> m;
        vector<int> result;
        for (int i=0; i<nums.size(); i++) {
            if ( m.find(target - nums[i]) == m.end() ) {
                m[nums[i]] = i;
            }else{
                result.push_back(m[target - nums[i]]);
                result.push_back(i);
            }
        }
        return result;
    }
};

// author jx H
// date 2025-09-27

//O(n^2) 与上面的 foreash 类似，暴力枚举
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> result;
        if (nums.size() < 2) {
            return result;
        }
        for (int i = 0; i < nums.size() - 1; i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                if (nums[i] + nums[j] == target) {
                    result.push_back(i);
                    result.push_back(j);
                    return result;
                }
            }
        }
        return result;
    }
};

//哈希表法参考上面解法，
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> result;
        unordered_map<int, int> m_mid;
        for (int i = 0; i < nums.size();i++) {
            if (m_mid.find(nums[i]) == m_mid.end()) {
                m_mid[target-nums[i]] = i;
            } else {
                result.push_back(m_mid[nums[i]]);
                result.push_back(i);
                break;
            }
        }
        return result;
    }
};