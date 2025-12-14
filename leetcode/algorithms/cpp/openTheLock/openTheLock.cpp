//hjx code 
//https://leetcode.cn/problems/open-the-lock/description/
/*
打开转盘锁

你有一个带有四个圆形拨轮的转盘锁。每个拨轮都有10个数字： '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' 。每个拨轮可以自由旋转：例如把 '9' 变为 '0'，'0' 变为 '9' 。每次旋转都只能旋转一个拨轮的一位数字。

锁的初始数字为 '0000' ，一个代表四个拨轮的数字的字符串。

列表 deadends 包含了一组死亡数字，一旦拨轮的数字和列表里的任何一个元素相同，这个锁将会被永久锁定，无法再被旋转。

字符串 target 代表可以解锁的数字，你需要给出解锁需要的最小旋转次数，如果无论如何不能解锁，返回 -1
*/
//给出了 target, 知道一开始为 0000, 求最小次数，即最小路径问题，每一位数可以上下拨动，则0000可以衍生出 0001, 0009, 0010, 0090, 0100, 0900, 1000, 9000八为数，这八为数每位又可以继续衍生出8位数，即可以看作是一个八叉树，即遍历这个八叉树，即可找到最小值，同时有deadends 列表不能触碰，即这些默认即访问过了，不要访问，由于每次到9000其还会出现0000类似情况，所以要增加visited标记，避免重复访问。最小路径问题，采用bfs层次遍历，每个层次产生的八个数，通过getNeighbors函数获取。（即为图的bfs遍历）
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> deads(deadends.begin(), deadends.end());
        if (deads.count("0000"))
            return -1;
        unordered_set<string> visited;
        queue<string> q;
        q.push("0000");
        visited.insert("0000");
        int step = 0;
        while(!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++){
                string cur = q.front();
                q.pop();
                if (cur == target) {
                    return step;
                }
                for (string s : getNeighbors(cur)) {
                    if (!visited.count(s) && !deads.count(s)) {
                        q.push(s);
                        visited.insert(s);
                    }
                }
            }
            step++;
        }
        return -1;
    }
    vector<string> getNeighbors(string s) {
        vector<string> res;
        for (int i = 0; i < 4; i++) {
            res.push_back(plusOne(s, i));
            res.push_back(minusOne(s, i));
        }
        return res;
    }
    string plusOne(string s, int i) {
        if (s[i] == '9') {
            s[i] = '0';
        } else {
            s[i] += 1;
        }
        return s;
    }
    string minusOne(string s, int i) {
        if (s[i] == '0') {
            s[i] = '9';
        } else {
            s[i] -= 1;
        }
        return s;
    }
};

//双向 bfs 优化，如果知道开始和目标，可以同时从两端开始搜索，从两端相向而行，可以快速到达交集。
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> deads(deadends.begin(), deadends.end());
        
        // base case
        if (deads.count("0000")) return -1;
        if (target == "0000") return 0;

        // 用集合不用队列，可以快速判断元素是否存在
        unordered_set<string> q1;
        unordered_set<string> q2;
        unordered_set<string> visited;

        int step = 0;
        q1.insert("0000");
        visited.insert("0000");
        q2.insert(target);
        visited.insert(target);

        while (!q1.empty() && !q2.empty()) {

            // 在这里增加步数
            step++;

            // 哈希集合在遍历的过程中不能修改，所以用 newQ1 存储邻居节点
            unordered_set<string> newQ1;

            // 获取 q1 中的所有节点的邻居
            for (const string& cur : q1) {
                // 将一个节点的未遍历相邻节点加入集合
                for (const string& neighbor : getNeighbors(cur)) {
                    // 判断是否到达终点
                    if (q2.count(neighbor)) {
                        return step;
                    }
                    if (!visited.count(neighbor) && !deads.count(neighbor)) {
                        newQ1.insert(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
            // newQ1 存储着 q1 的邻居节点
            q1 = newQ1;
            // 因为每次 BFS 都是扩散 q1，所以把元素数量少的集合作为 q1
            if (q1.size() > q2.size()) {
                swap(q1, q2);
            }
        }
        return -1;
    }

    // 将 s[j] 向上拨动一次
    string plusOne(string s, int j) {
        if (s[j] == '9')
            s[j] = '0';
        else
            s[j] += 1;
        return s;
    }

    // 将 s[i] 向下拨动一次
    string minusOne(string s, int j) {
        if (s[j] == '0')
            s[j] = '9';
        else
            s[j] -= 1;
        return s;
    }

    vector<string> getNeighbors(string s) {
        vector<string> neighbors;
        for (int i = 0; i < 4; i++) {
            neighbors.push_back(plusOne(s, i));
            neighbors.push_back(minusOne(s, i));
        }
        return neighbors;
    }
};