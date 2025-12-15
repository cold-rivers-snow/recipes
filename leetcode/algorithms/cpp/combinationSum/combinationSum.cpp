// Source : https://oj.leetcode.com/problems/combination-sum/
// Author : Hao Chen
// Date   : 2014-07-19

/********************************************************************************** 
* 
* Given a set of candidate numbers (C) and a target number (T), find all unique combinations 
* in C where the candidate numbers sums to T. 
* 
* The same repeated number may be chosen from C unlimited number of times.
* 
* Note:
* 
* All numbers (including target) will be positive integers.
* Elements in a combination (a1, a2, … , ak) must be in non-descending order. (ie, a1 ≤ a2 ≤ … ≤ ak).
* The solution set must not contain duplicate combinations.
* 
* For example, given candidate set 2,3,6,7 and target 7, 
* A solution set is: 
* [7] 
* [2, 2, 3] 
* 
*               
**********************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void combinationSumHelper(vector<int> &candidates, int start, int target, vector<int> &solution, vector< vector<int> > &result) {
    if (target<0){
        return;
    }
    if (target==0){
        result.push_back(solution);
        return;
    }
    for(int i=start; i<candidates.size(); i++){
        //skip duplicates
        if (i>start && candidates[i] == candidates[i-1]) {
            continue;
        }
        solution.push_back(candidates[i]);
        combinationSumHelper(candidates, i, target - candidates[i], solution, result);
        solution.pop_back();
    }
}

vector<vector<int> > combinationSum(vector<int> &candidates, int target) {
    vector< vector<int> > result;
    if (candidates.size()<=0){
        return result;
    }
    sort(candidates.begin(), candidates.end());
    
    vector<int> solution;
    combinationSumHelper(candidates, 0, target, solution, result);

    return result;
}

void printMatrix(vector< vector<int> > &vv)
{
    for(int i=0; i<vv.size(); i++) {
        cout << "[";
        for(int j=0; j<vv[i].size(); j++) {
            cout << " " << vv[i][j];
        }
        cout << "]" << endl;;
    }
}

void printArray(vector<int> &v)
{
    cout << "{";
    for(int i=0; i<v.size(); i++) {
        cout << " " << v[i];
    }
    cout << "}" << endl;
}


int main(int argc, char** argv)
{
    int a[] = {4,2,3,3,5,7};
    vector<int> v(a, a+sizeof(a)/sizeof(int));
    int target = 7;
    cout << "array  = ";
    printArray(v);
    cout << "target = " << target << endl;
    
    vector< vector<int> > vv = combinationSum(v, target);
    printMatrix(vv);

    return 0;
}

//hjx code
//组合，则使用回溯法，加 start 写入所有的组合，因为和为target,因此在每次选择的时候都进行相加，取消选择的时候进行相减，如果想到则选择这个组合，大于 target 直接返回，减少无效遍历导致死循环，因为可以重复使用，所以start可以不加1.
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> tracks;
        int tracks_sum = 0;
        backtracks(res, tracks, candidates, target, 0, tracks_sum);
        return res;
    }
    void backtracks(vector<vector<int>>& res, vector<int>& tracks, vector<int>& candidates, int target, int start, int tracks_sum) {
        if (tracks_sum == target) {
            res.push_back(tracks);
            return;
        }
        if (tracks_sum > target) {
            return;
        }
        for (int i = start; i < candidates.size(); i++) {
            tracks.push_back(candidates[i]);
            tracks_sum += candidates[i];
            // 注意：这里传入的是 i 而不是 i+1，表示可以重复选择当前元素
            backtracks(res, tracks, candidates, target, i, tracks_sum);
            tracks.pop_back();
            tracks_sum -= candidates[i];
        }
        return;
    }
};