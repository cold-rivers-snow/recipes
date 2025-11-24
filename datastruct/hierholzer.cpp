#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;

// 无向图的欧拉路径/回路
class UndirectedEulerian {
private:
    // 邻接表表示图，pair.second表示边是否被访问过
    unordered_map<int, vector<pair<int, bool>>> graph;
    vector<pair<int, int>> path; // 存储欧拉路径的边
    
    // 递归DFS实现
    void dfs(int u) {
        for (auto& [v, visited] : graph[u]) {
            if (!visited) {
                // 标记边为已访问
                visited = true;
                // 由于是无向图，需要同时标记反向边
                for (auto& [x, vis] : graph[v]) {
                    if (x == u && !vis) {
                        vis = true;
                        break;
                    }
                }
                dfs(v);
                path.push_back({u, v});
            }
        }
    }
    
    // Hierholzer算法的栈实现
    void hierholzer(int start) {
        stack<int> nodeStack;
        nodeStack.push(start);
        
        while (!nodeStack.empty()) {
            int u = nodeStack.top();
            bool hasUnvisitedEdge = false;
            
            for (auto& [v, visited] : graph[u]) {
                if (!visited) {
                    visited = true;
                    // 标记反向边
                    for (auto& [x, vis] : graph[v]) {
                        if (x == u && !vis) {
                            vis = true;
                            break;
                        }
                    }
                    nodeStack.push(v);
                    hasUnvisitedEdge = true;
                    break;
                }
            }
            
            if (!hasUnvisitedEdge) {
                nodeStack.pop();
                if (!nodeStack.empty()) {
                    path.push_back({nodeStack.top(), u});
                }
            }
        }
    }
    
public:
    void addEdge(int u, int v) {
        graph[u].push_back({v, false});
        graph[v].push_back({u, false}); // 无向图
    }
    
    // 寻找欧拉回路
    bool findEulerCircuit(int start) {
        path.clear();
        
        // 检查度数条件
        for (auto& [node, edges] : graph) {
            if (edges.size() % 2 != 0) {
                return false; // 存在奇度数节点，不是欧拉图
            }
        }
        
        // 使用Hierholzer算法
        hierholzer(start);
        
        // 检查是否遍历了所有边
        size_t edgeCount = 0;
        for (auto& [node, edges] : graph) {
            edgeCount += edges.size();
        }
        edgeCount /= 2; // 每条边在邻接表中出现两次
        
        return path.size() == edgeCount;
    }
    
    // 寻找欧拉路径
    bool findEulerPath() {
        path.clear();
        
        // 检查度数条件
        vector<int> oddDegreeNodes;
        for (auto& [node, edges] : graph) {
            if (edges.size() % 2 != 0) {
                oddDegreeNodes.push_back(node);
            }
        }
        
        // 要么没有奇度节点，要么恰好有两个
        if (oddDegreeNodes.size() != 0 && oddDegreeNodes.size() != 2) {
            return false;
        }
        
        int startNode = (oddDegreeNodes.size() == 2) ? oddDegreeNodes[0] : graph.begin()->first;
        
        // 使用Hierholzer算法
        hierholzer(startNode);
        
        // 检查是否遍历了所有边
        size_t edgeCount = 0;
        for (auto& [node, edges] : graph) {
            edgeCount += edges.size();
        }
        edgeCount /= 2; // 每条边在邻接表中出现两次
        
        return path.size() == edgeCount;
    }
    
    void printPath() {
        cout << "欧拉路径/回路: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << "(" << path[i].first << "->" << path[i].second << ") ";
        }
        cout << endl;
    }
};

// 有向图的欧拉路径/回路
class DirectedEulerian {
private:
    // 邻接表表示图，pair.second表示边是否被访问过
    unordered_map<int, vector<pair<int, bool>>> graph;
    vector<pair<int, int>> path; // 存储欧拉路径的边
    
    // Hierholzer算法的栈实现
    void hierholzer(int start) {
        stack<int> nodeStack;
        nodeStack.push(start);
        
        while (!nodeStack.empty()) {
            int u = nodeStack.top();
            bool hasUnvisitedEdge = false;
            
            for (auto& [v, visited] : graph[u]) {
                if (!visited) {
                    visited = true;
                    nodeStack.push(v);
                    hasUnvisitedEdge = true;
                    break;
                }
            }
            
            if (!hasUnvisitedEdge) {
                nodeStack.pop();
                if (!nodeStack.empty()) {
                    path.push_back({nodeStack.top(), u});
                }
            }
        }
    }
    
public:
    void addEdge(int u, int v) {
        graph[u].push_back({v, false});
    }
    
    // 计算入度和出度
    void getInOutDegrees(unordered_map<int, int>& inDegrees, unordered_map<int, int>& outDegrees) {
        for (auto& [u, edges] : graph) {
            outDegrees[u] = edges.size();
            for (auto& [v, visited] : edges) {
                inDegrees[v]++;
            }
        }
    }
    
    // 寻找欧拉回路
    bool findEulerCircuit(int start) {
        path.clear();
        
        // 检查度数条件
        unordered_map<int, int> inDegrees, outDegrees;
        getInOutDegrees(inDegrees, outDegrees);
        
        for (auto& [node, edges] : graph) {
            if (inDegrees[node] != outDegrees[node]) {
                return false; // 入度不等于出度，不是欧拉图
            }
        }
        
        // 使用Hierholzer算法
        hierholzer(start);
        
        // 检查是否遍历了所有边
        size_t edgeCount = 0;
        for (auto& [node, edges] : graph) {
            edgeCount += edges.size();
        }
        
        return path.size() == edgeCount;
    }
    
    // 寻找欧拉路径
    bool findEulerPath() {
        path.clear();
        
        // 检查度数条件
        unordered_map<int, int> inDegrees, outDegrees;
        getInOutDegrees(inDegrees, outDegrees);
        
        int startCount = 0, endCount = 0;
        int startNode = -1;
        
        for (auto& [node, edges] : graph) {
            int diff = outDegrees[node] - inDegrees[node];
            if (diff == 1) {
                startCount++;
                startNode = node; // 起点
            } else if (diff == -1) {
                endCount++;
            } else if (diff != 0) {
                return false; // 度数差不为-1,0,1
            }
        }
        
        // 要么没有起点终点，要么恰好各一个
        if (!((startCount == 0 && endCount == 0) || (startCount == 1 && endCount == 1))) {
            return false;
        }
        
        // 如果没有指定起点，选择任意节点
        if (startNode == -1) {
            startNode = graph.begin()->first;
        }
        
        // 使用Hierholzer算法
        hierholzer(startNode);
        
        // 检查是否遍历了所有边
        size_t edgeCount = 0;
        for (auto& [node, edges] : graph) {
            edgeCount += edges.size();
        }
        
        return path.size() == edgeCount;
    }
    
    void printPath() {
        cout << "欧拉路径/回路: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << "(" << path[i].first << "->" << path[i].second << ") ";
        }
        cout << endl;
    }
};

// 测试示例
int main() {
    // 无向图欧拉回路示例
    cout << "===== 无向图欧拉回路示例 =====" << endl;
    UndirectedEulerian undirGraph;
    undirGraph.addEdge(0, 1);
    undirGraph.addEdge(1, 2);
    undirGraph.addEdge(2, 3);
    undirGraph.addEdge(3, 0);
    undirGraph.addEdge(0, 2);
    
    if (undirGraph.findEulerCircuit(0)) {
        cout << "找到欧拉回路!" << endl;
        undirGraph.printPath();
    } else {
        cout << "未找到欧拉回路，尝试寻找欧拉路径..." << endl;
        if (undirGraph.findEulerPath()) {
            cout << "找到欧拉路径!" << endl;
            undirGraph.printPath();
        } else {
            cout << "未找到欧拉路径" << endl;
        }
    }
    
    // 有向图欧拉路径示例
    cout << "\n===== 有向图欧拉路径示例 =====" << endl;
    DirectedEulerian dirGraph;
    dirGraph.addEdge(0, 1);
    dirGraph.addEdge(1, 2);
    dirGraph.addEdge(1, 3);
    dirGraph.addEdge(2, 1);
    dirGraph.addEdge(3, 0);
    dirGraph.addEdge(3, 4);
    dirGraph.addEdge(4, 3);
    
    if (dirGraph.findEulerPath()) {
        cout << "找到欧拉路径!" << endl;
        dirGraph.printPath();
    } else {
        cout << "未找到欧拉路径" << endl;
    }
    
    return 0;
}