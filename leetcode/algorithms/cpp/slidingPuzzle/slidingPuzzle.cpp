//hjx code 
//https://leetcode.cn/problems/sliding-puzzle/description/
/*
滑动谜题
在一个 2 x 3 的板上（board）有 5 块砖瓦，用数字 1~5 来表示, 以及一块空缺用 0 来表示。一次 移动 定义为选择 0 与一个相邻的数字（上下左右）进行交换.

最终当板 board 的结果是 [[1,2,3],[4,5,0]] 谜板被解开。

给出一个谜板的初始状态 board ，返回最少可以通过多少次移动解开谜板，如果不能解开谜板，则返回 -1 
*/
//通过图的 bfs 层次遍历方式，求最小路径，把初始时的二维数组看出一个开始状态，改变0的相邻的位置交换，依次类推，找出所有的可能性。通过层次遍历，找到目标状态，把二维数组值串成一个字符串。使用 visited 记录是否遍历过的，避免重复遍历。
class Solution {
public:
    int slidingPuzzle(vector<vector<int>>& board) {
        string target = "123450";
        string start = "";
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                start += to_string(board[i][j]);
            }
        }
        queue<string> q;
        unordered_set<string> visited;
        q.push(start);
        visited.insert(start);
        int step = 0;
        while(!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                string cur = q.front();
                q.pop();
                if (target == cur)
                    return step;
                for (string neighborBoard : getNeighbors(cur)) {
                    if (!visited.count(neighborBoard)) {
                        q.push(neighborBoard);
                        visited.insert(neighborBoard);
                    }
                }
            }
            step++;
        }
        return -1;
    }

    vector<string> getNeighbors(string board) {
        // 记录一维字符串的相邻索引
        vector<vector<int>> mapping = {
            {1, 3},
            {0, 4, 2},
            {1, 5},
            {0, 4},
            {3, 1, 5},
            {4, 2}
        };

        int idx = board.find('0');
        vector<string> neighbors;
        for (int adj : mapping[idx]) {
            string new_board = swap(board, idx, adj);
            neighbors.push_back(new_board);
        }
        return neighbors;
    }

    string swap(string board, int i, int j) {
        char temp = board[i];
        board[i] = board[j];
        board[j] = temp;
        return board;
    }
};