// Source : https://leetcode.cn/problems/permutation-in-string/
// Author : hjx
// Date   : 2025-12-11

/*
字符串的排列，
给你两个字符串 s1 和 s2 ，写一个函数来判断 s2 是否包含 s1 的 排列。如果是，返回 true ；否则，返回 false 。

换句话说，s1 的排列之一是 s2 的 子串 。

 

示例 1：

输入：s1 = "ab" s2 = "eidbaooo"
输出：true
解释：s2 包含 s1 的排列之一 ("ba").
示例 2：

输入：s1= "ab" s2 = "eidboaoo"
输出：false
 

提示：

1 <= s1.length, s2.length <= 104
s1 和 s2 仅包含小写字母
*/
//同第76题一样，计算最小的子串，但是这个是排列，中间不会有非当前字符串的字符，也就是说需要的最小的子串一定和s1的字符个数相等
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        unordered_map<char, int> need, window;
        for (char c : s1) {
            need[c]++;
        }
        int left = 0, right = 0, vaild = 0, len = INT_MAX, start = 0;
        while(right < s2.length()) {
            char c = s2[right];
            right++;
            if (need.count(c)) {
                window[c]++;
                if (window[c] == need[c]) {
                    vaild++;
                }
            }
            while(vaild == need.size()) {
                if (len > right - left) {
                    len = right - left;
                    start = left;
                }
                char c = s2[left];
                if (need.count(c)) {
                    if (window[c] == need[c]) {
                        vaild--;
                    }
                    window[c]--;
                }
                left++;
            }
        }
        if (len == s1.length()) {
            return true;
        }
        return false;
    }
};