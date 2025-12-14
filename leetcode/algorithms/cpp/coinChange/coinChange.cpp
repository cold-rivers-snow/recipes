// Source : https://leetcode.com/problems/coin-change/
// Author : Calinescu Valentin, Hao Chen
// Date   : 2015-12-28

/*************************************************************************************** 
 *
 * You are given coins of different denominations and a total amount of money amount. 
 * Write a function to compute the fewest number of coins that you need to make up that
 * amount. If that amount of money cannot be made up by any combination of the coins,
 * return -1.
 * 
 * Example 1:
 * coins = [1, 2, 5], amount = 11
 * return 3 (11 = 5 + 5 + 1)
 * 
 * Example 2:
 * coins = [2], amount = 3
 * return -1.
 * 
 * Note:
 * You may assume that you have an infinite number of each kind of coin.
 * 
 * Credits:
 * Special thanks to @jianchao.li.fighter for adding this problem and creating all test 
 * cases.
 * 
 ***************************************************************************************/


/* Recursive solution - TIME LIMIT ERROR */
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {  
        int result = INT_MAX;
        if ( amount == 0 ) return 0;
        if ( amount < 0 ) return -1;
        for (int i=0; i<coins.size(); i++) {
            if ( amount - coins[i] < 0 ) continue;
            int r = coinChange(coins, amount - coins[i]);
            if ( r == -1 ) continue;
            if (result > r )  result = r + 1;
        }
        return result == INT_MAX ? -1 : result;
    }
}


 /* 
 * Solution 1 - O(N * amount)
 * =========
 *
 * This problem can be solved using dynamic programming, thus building the optimal
 * solution from previous smaller ones. For every coin of value t and every sum of money
 * i the sum can be traced back to a previous sum i - t that was already computed and uses
 * the smallest number of coins possible. This way we can construct every sum i as the
 * minimum of all these previous sums for every coin value. To be sure we'll find a minimum
 * we can populate the solution vector with an amount bigger than the maximum possible, 
 * which is amount + 1(when the sum is made up of only coins of value 1). The only exception
 * is sol[0] which is 0 as we need 0 coins to have a sum of 0. In the end we need to look
 * if the program found a solution in sol[amount] or it remained the same, in which case we
 * can return -1.
 * 
 */
class Solution {
public:
    
    int coinChange(vector<int>& coins, int amount) {
        int sol[amount + 1];
        sol[0] = 0;
        for(int i = 1; i <= amount; i++)
            sol[i] = amount + 1;
        for(int i = 0; i < coins.size(); i++)
        {
            for(int j = coins[i]; j <= amount; j++)
                sol[j] = min(sol[j], sol[j - coins[i]] + 1);
        }
        if(sol[amount] != amount + 1)
            return sol[amount];
        else
            return -1;
    }
};


//Another DP implmentation, same idea above 
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        const int MAX = amount +1;
        vector<int> dp(amount+1, MAX);
        dp[0]=0;
        
        for(int i=1; i<=amount; i++) {
            for (int j=0; j<coins.size(); j++){
                if (i >= coins[j]) {
                    dp[i] = min( dp[i], dp[i-coins[j]] + 1 );
                }
            }
        }

        return dp[amount]==MAX ? -1 : dp[amount];
    }
};

//hjx code
//自顶向下
//目标是amount，coins中的面值是固定不变的，可无限使用的，amount是可变的，如果为负数,则为面值不存在，如果amount是0,则为0,如果是1,则为1个1coin的面值。那就是说amount的面值个数，就是 amount-1 的面值的最少值，以此类推。
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        if (amount < 0)
            return -1;
        if (amount == 0) 
            return 0;
        int res = INT_MAX;
        for (auto c : coins) {
            int sub_c = coinChange(coins, amount - c);
            if (sub_c == -1)
                continue;
            res = min(res, sub_c + 1);
        }
        return res == INT_MAX ? -1 : res;
    }
};

//但是这样多次相同的值会多次递归计算，容易超时，过不了 leetcode, 那就剪枝，带备忘录的数组来存便利过的值。

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount+1, INT_MAX);
        return depth(coins, amount, dp);
    }
    int depth(vector<int>& coins, int amount, vector<int>& dp) {
        if (amount < 0)
            return -1;
        if (amount == 0) 
            return 0;
        if (dp[amount] != INT_MAX)
            return dp[amount];
        int res = INT_MAX;
        for (auto c : coins) {
            int sub_c = depth(coins, amount - c, dp);
            if (sub_c == -1)
                continue;
            res = min(res, sub_c + 1);
        }
        dp[amount] = res == INT_MAX ? -1 : res;
        return dp[amount];
    }
};

//自底向上思考，定义dp table, i 代表amount，dp[i] 代表amount=i时的最少硬币数，dp[i] = min(dp[i], dp[i-c] + 1); dp 的初始化不能为 INT_MAX，因为dp[i-c]+1 ,如果 dp[i-c] == INI_MAX，则超限。最后返回要判断dp[amount]是否为初始值，是则返回-1，否则返回dp[amount]。

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount+1, amount+1);
        dp[0] = 0;
        for (int i = 1; i <= amount; i++) {
            for (auto c : coins) {
                if (i - c < 0)
                    continue;
                dp[i] = min(dp[i], dp[i-c] + 1);
            }
        }
        return dp[amount] == amount + 1 ? -1 : dp[amount];
    }
};
