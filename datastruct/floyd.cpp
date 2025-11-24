#include <vector>
#include <climits>
#include <algorithm>
#include <iostream>
using namespace std;

// Floyd算法C++实现
void floyd(vector<vector<int>>& dist) {
    int n = dist.size();
    
    // dp[k][i][j] = min(dp[k-1][i][j], dp[k-1][i][k] + dp[k-1][k][j])
    // 优化为二维DP，k作为最外层循环
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // 避免整数溢出
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

// 使用示例
int main() {
    // 示例：4个节点的图
    const int INF = INT_MAX;
    vector<vector<int>> dist = {
        {0, 3, INF, 5},
        {2, 0, INF, 4},
        {INF, 1, 0, INF},
        {INF, INF, 2, 0}
    };
    
    floyd(dist);
    
    // 输出结果
    for (const auto& row : dist) {
        for (int d : row) {
            if (d == INF) cout << "INF ";
            else cout << d << " ";
        }
        cout << endl;
    }
    
    return 0;
}