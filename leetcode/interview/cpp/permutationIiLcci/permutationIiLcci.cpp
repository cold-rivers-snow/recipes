//hjx code
//https://leetcode.cn/problems/permutation-ii-lcci/
/*
有重复字符串的排列组合

提示
有重复字符串的排列组合。编写一种方法，计算某字符串的所有排列组合。

示例 1：

 输入：S = "qqe"
 输出：["eqq","qeq","qqe"]
示例 2：

 输入：S = "ab"
 输出：["ab", "ba"]
提示:

字符都是英文字母。
字符串长度在[1, 9]之间。
*/

//全排列，使用回溯法，used数组标记，由于有重复字符，需要先排序，然后在遍历的时候跳过。
class Solution {
public:
    vector<string> permutation(string S) {
        vector<string> res;
        string tracks;
        vector<bool> used(S.length(), false);
        sort(S.begin(), S.end());
        backtracks(res, tracks, used, S);
        return res;
    }
    void backtracks(vector<string>& res, string& tracks, vector<bool>& used, string& S) {
        if (tracks.length() == S.length()) {
            res.push_back(tracks);
            return;
        }
        for (int i = 0; i < S.length(); i++) {
            if (used[i]) 
                continue;
            if (i > 0 && S[i] == S[i - 1] && !used[i-1])
                continue;
            tracks.push_back(S[i]);// string 也有 push_back,pop_back
            used[i] = true;
            backtracks(res, tracks, used, S);
            tracks.pop_back();
            used[i] = false;
        }
        return;
    }
};