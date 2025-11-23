#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>

class SegmentTree {
private:
    std::vector<int> tree;  // 线段树数组
    int n;                  // 原数组长度
    
    // 构建线段树
    void build(const std::vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            // 叶子节点
            tree[node] = nums[start];
        } else {
            int mid = (start + end) / 2;
            // 递归构建左右子树
            build(nums, 2 * node + 1, start, mid);
            build(nums, 2 * node + 2, mid + 1, end);
            // 聚合子节点信息
            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }
    }
    
    // 更新操作
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            // 找到要更新的叶子节点
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid) {
                // 在左子树中更新
                update(2 * node + 1, start, mid, idx, val);
            } else {
                // 在右子树中更新
                update(2 * node + 2, mid + 1, end, idx, val);
            }
            // 更新当前节点的值
            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }
    }
    
    // 查询操作
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            // 区间完全不重叠，返回0（求和的单位元）
            return 0;
        }
        if (l <= start && end <= r) {
            // 区间完全包含，直接返回节点值
            return tree[node];
        }
        // 区间部分重叠，递归查询左右子树
        int mid = (start + end) / 2;
        int left_sum = query(2 * node + 1, start, mid, l, r);
        int right_sum = query(2 * node + 2, mid + 1, end, l, r);
        return left_sum + right_sum;
    }

public:
    // 构造函数
    SegmentTree(const std::vector<int>& nums) {
        n = nums.size();
        if (n == 0) return;
        
        // 线段树数组大小：4 * n 是安全的上界
        tree.resize(4 * n);
        build(nums, 0, 0, n - 1);
    }
    
    // 更新接口
    void update(int index, int val) {
        update(0, 0, n - 1, index, val);
    }
    
    // 查询接口
    int query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }
};

int main() {
    std::vector<int> nums = {1, 3, 5, 7, 9, 11};
    SegmentTree st(nums);
    
    // 查询区间 [1, 3] 的和：3 + 5 + 7 = 15
    std::cout << "Query [1,3]: " << st.query(1, 3) << std::endl;
    
    // 更新索引 1 的值为 10
    st.update(1, 10);
    
    // 再次查询区间 [1, 3] 的和：10 + 5 + 7 = 22
    std::cout << "Query [1,3] after update: " << st.query(1, 3) << std::endl;
    
    // 查询整个数组的和
    std::cout << "Total sum: " << st.query(0, nums.size() - 1) << std::endl;
    
    return 0;
}