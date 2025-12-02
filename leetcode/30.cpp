//
// Created by root on 25-5-25.
//
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

bool InWords(string s, vector<string>& words) {
    unordered_map<string, int> cnt;
    for (const auto& w : words) {
        cnt[w]++;
    }
    int n = words[0].length();
    for (int left = 0; left < s.length(); left += n) {
        string tmp = s.substr(left, n);
        if (--cnt[tmp] < 0) {
            return false;
        }
    }
    return true;
}

vector<int> findSubstring(string s, vector<string>& words) {
    vector<int> ans;
    int n = words.size() * words[0].length();

    for (int left = 0; left < s.length(); ) {
        string tmp = s.substr(left, n);
        if (tmp.length() == n && InWords(tmp, words)) {
            ans.push_back(left);
            left ++;
        } else {
            left++;
        }
    }
    return ans;
}

int main() {
    string s = "barfoofoobarthefoobarman";
    vector<string> words = {"bar","foo","the"};
    vector<int> ans = findSubstring(s, words);
    for (auto& x : ans) {
        cout << x << endl;
    }
}