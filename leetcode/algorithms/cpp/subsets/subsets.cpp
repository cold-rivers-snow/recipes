// Source : https://oj.leetcode.com/problems/subsets/
// Author : Hao Chen
// Date   : 2014-07-03

/********************************************************************************** 
* 
* Given a set of distinct integers, S, return all possible subsets.
* 
* Note:
* 
* Elements in a subset must be in non-descending order.
* The solution set must not contain duplicate subsets.
* 
* For example,
* If S = [1,2,3], a solution is:
* 
* [
*   [3],
*   [1],
*   [2],
*   [1,2,3],
*   [1,3],
*   [2,3],
*   [1,2],
*   []
* ]
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

void getCombination(vector<int>& v, int n, int k, vector<int>& solution, vector< vector<int> >& result );
vector<vector<int> > combine(vector<int>& v, int k); 
vector<vector<int> > combine1(vector<int>& v, int k); 
vector<vector<int> > combine2(vector<int>& v, int k);

vector<vector<int> > subsets(vector<int> &S) {
    vector<vector<int> > result;
    for(int i=0; i<=S.size(); i++){
        vector<vector<int> > r = combine(S, i); 
        result.insert(result.end(), r.begin(), r.end()); 
    } 
    return result;
}


vector<vector<int> > combine(vector<int>& v, int k) {
    if (random()%2){
        cout << "recusive method!" << endl;
        return combine1(v, k);
    }
    cout << "non-recusive method!" << endl;
    return combine2(v, k);
}

vector<vector<int> > combine1(vector<int> &v, int k) {
    vector<vector<int> > result;
    vector<int> solution;
    getCombination(v, v.size(), k, solution, result);
    return result;
}

void getCombination(vector<int> &v, int n, int k, vector<int>& solution, vector< vector<int> >& result ){
    if (k==0){
        //sort to meet LeetCode requirement
        vector<int> v = solution;
        sort(v.begin(), v.end());
        result.push_back(v);
        return;
    }
    for(int i=n; i>0; i--){
        solution.push_back(v[i-1]);
        getCombination(v, i-1, k-1, solution, result);
        solution.pop_back();
    }
}

vector<vector<int> > combine2(vector<int> &v, int k) {
    
    vector<vector<int> > result;
    vector<int> d;
    int n = v.size();
    for (int i=0; i<n; i++){
        d.push_back( (i<k) ? 1 : 0 );
    }

    //1) from the left, find the [1,0] pattern, change it to [0,1]
    //2) move all of the 1 before the pattern to the most left side
    //3) check all of 1 move to the right
    while(1){
        vector<int> tmp;
        for(int x=0; x<n; x++){
            if (d[x]) tmp.push_back(v[x]);
        }
        sort(tmp.begin(), tmp.end());
        result.push_back(tmp);
        //step 1), find [1,0] pattern
        int i;
        bool found = false;
        int ones =0;
        for(i=0; i<n-1; i++){
            
            if (d[i]==1 && d[i+1]==0){
                d[i]=0; d[i+1]=1;
                found = true;
                //step 2) move all of right 1 to the most left side
                for (int j=0; j<i; j++){
                    d[j]=( ones > 0 ) ? 1 : 0;
                    ones--;
                }
                break; 
            } 
            if (d[i]==1) ones++;
        }
        if (!found){
            break;
        }
        
    }
    return result;
    
}




void printResult(vector<vector<int> >& result)
{
    for(int i=0; i<result.size(); i++){
        cout << "{";
        for(int j=0; j<result[i].size(); j++){
            cout << " " << result[i][j];
        }
        cout << " }" <<endl;
    }
}

void printArray(vector<int>& v){
    cout << "{";
    for(int i=0; i<v.size(); i++) {
        cout << " " << v[i];
    }
    cout << " }" << endl;
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    int n = 3;
    if (argc>1){
        n = atoi(argv[1]);
    }
    vector<int> v;
    for(int i=n; i>0; i--){
        v.push_back(i);
    }
    printArray(v);
    vector<vector<int> > r = subsets(v);
    printResult(r);
}

//hjx code
//回溯法，遍历每个节点，start 控制每个节点的遍历起始位置，
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        list<int> tracks;
        backtracks(res, tracks, nums, 0);
        return res;
    }
    void backtracks(vector<vector<int>>& res, list<int>& tracks, vector<int>& nums, int start) {
        res.push_back(vector<int>(tracks.begin(), tracks.end()));
        for (int i = start; i < nums.size(); i++) {
            tracks.push_back(nums[i]);
            backtracks(res, tracks, nums, i+1);
            tracks.pop_back();
        }
        return;
    }
};