// Source : https://oj.leetcode.com/problems/permutations-ii/
// Author : Hao Chen
// Date   : 2014-06-21

/********************************************************************************** 
* 
* Given a collection of numbers that might contain duplicates, return all possible unique permutations.
* 
* For example,
* [1,1,2] have the following unique permutations:
* [1,1,2], [1,2,1], and [2,1,1].
* 
*               
**********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// To deal with the duplication number, we need do those modifications:
//    1) sort the array [pos..n].
//    2) skip the same number.
vector<vector<int> > permute(vector<int> &num) {

    vector<vector<int> > vv;
    vv.push_back(num);

    if (num.size() <2){
        return vv;
    }
        
    int pos=0;
    while(pos<num.size()-1){
        int size = vv.size();
        for(int i=0; i<size; i++){
            //sort the array, so that the same number will be together
            sort(vv[i].begin()+pos, vv[i].end());
            //take each number to the first
            for (int j=pos+1; j<vv[i].size(); j++) {
                vector<int> v = vv[i];
                //skip the same number 
                if (j>0 && v[j]==v[j-1]){
                    continue;
                }
                int t = v[j]; 
                v[j] = v[pos];
                v[pos] = t;
                vv.push_back(v);
            }
        }
        pos++;
    }
    return vv;
}

void printVector( vector<int>&  pt)
{
    cout << "{ ";
    for(int j=0; j<pt.size(); j++){
        cout << pt[j] << " ";
    }
    cout << "} " << endl;
}


int main(int argc, char** argv)
{
    int n = 3;
    if (argc>1){
       n = atoi(argv[1]); 
    }

    srand(time(NULL));
    vector<int> v;
    for (int i=0; i<n; i++) {
        v.push_back(random()%n+1);
    }
    /*v[0] =0;
    v[1] =1;
    v[2] =0;
    v[3] =0;
    v[4] =9;*/

    printVector(v); 
    cout << "-----------------" << endl;


    vector<vector<int> > vv;
    vv = permute(v);
    
    for(int i=0; i<vv.size(); i++) {
        printVector(vv[i]);
    }

    return 0;
}

//hjx code
//回溯法，全排列问题，遍历决策树（多叉树的遍历）。全排列用used 标记是否使用过，子集、组合使用start往下查找。这个题目有重复的数字，所以会出现相同的排列，先排序，保证相同元素在一起，然后遍历时跳过重复元素，同时是上一层没有使用过的。
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> tracks;
        vector<bool> used(nums.size(), false);
        sort(nums.begin(), nums.end());
        backtracks(res, tracks, nums, used);
        return res;
    }
    void backtracks(vector<vector<int>>& res, vector<int>& tracks, vector<int>& nums, vector<bool>& used) {
        if (tracks.size() == nums.size()) { 
            res.push_back(tracks);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            // 剪枝1：如果元素已经被使用，则跳过
            if (used[i]) 
                continue;
            // 剪枝2：如果当前元素与前一个元素相同，且前一个元素未被使用（在同一层），则跳过
            if (i > 0 && nums[i] == nums[i - 1] && !used[i-1])
                continue;
            tracks.push_back(nums[i]);
            used[i] = true;
            backtracks(res, tracks, nums, used);
            tracks.pop_back();
            used[i] = false;
        }
        return;
    }
};