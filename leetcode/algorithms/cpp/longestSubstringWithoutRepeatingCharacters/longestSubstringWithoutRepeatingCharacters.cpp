// Source : https://oj.leetcode.com/problems/longest-substring-without-repeating-characters/
// Author : Hao Chen
// Date   : 2014-07-19

/********************************************************************************** 
* 
* Given a string, find the length of the longest substring without repeating characters. 
* For example, the longest substring without repeating letters for "abcabcbb" is "abc", 
* which the length is 3. For "bbbbb" the longest substring is "b", with the length of 1.
*               
**********************************************************************************/

#include <string.h>
#include <iostream>
#include <string>
#include <map>
using namespace std;
/*
 * Idea:
 * 
 * Using a map store each char's index.
 * 
 * So, we can be easy to know the when duplication and the previous duplicated char's index.
 * 
 * Then we can take out the previous duplicated char, and keep tracking the maxiumn length. 
 * 
 */
int lengthOfLongestSubstring1(string s) {
    map<char, int> m;
    int maxLen = 0;
    int lastRepeatPos = -1;
    for(int i=0; i<s.size(); i++){
        if (m.find(s[i])!=m.end() && lastRepeatPos < m[s[i]]) {
            lastRepeatPos = m[s[i]];
        }
        if ( i - lastRepeatPos > maxLen ){
            maxLen = i - lastRepeatPos;
        }
        m[s[i]] = i;
    }
    return maxLen;
}
//don't use <map>
int lengthOfLongestSubstring(string s) {
    const int MAX_CHARS = 256;
    int m[MAX_CHARS];
    memset(m, -1, sizeof(m));

    int maxLen = 0;
    int lastRepeatPos = -1;
    for(int i=0; i<s.size(); i++){
        if (m[s[i]]!=-1 && lastRepeatPos < m[s[i]]) {
            lastRepeatPos = m[s[i]];
        }
        if ( i - lastRepeatPos > maxLen ){
            maxLen = i - lastRepeatPos;
        }
        m[s[i]] = i;
    }
    return maxLen;
}

int main(int argc, char** argv)
{
    string s = "abcabcbb";
    cout << s << " : " << lengthOfLongestSubstring(s) << endl;

    s = "bbbbb";
    cout << s << " : " << lengthOfLongestSubstring(s) << endl;

    s = "bbabcdb";
    cout << s << " : " << lengthOfLongestSubstring(s) << endl;

    if (argc>1){
        s = argv[1];
        cout << s << " : " << lengthOfLongestSubstring(s) << endl;
    }

    return 0;
}

//hjx code
//滑动窗口，窗口内字符个数不能超过1
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> window;
        int left = 0, right = 0, len = 0;
        while(left <= right && right < s.length()) {
            char c = s[right];
            window[c]++;
            right++;
            while(window[c] > 1) {
                char c = s[left];
                window[c]--;
                left++;
            }
            len = max(len, right - left);
        }
        return len;
    }
};