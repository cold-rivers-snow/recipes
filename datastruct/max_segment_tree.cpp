#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include <cassert>

class MaxSegmentTree {
private:
    std::vector<int> tree;
    int n;
    
    void build(const std::vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            tree[node] = nums[start];
        } else {
            int mid = (start + end) / 2;
            build(nums, 2 * node + 1, start, mid);
            build(nums, 2 * node + 2, mid + 1, end);
            tree[node] = std::max(tree[2 * node + 1], tree[2 * node + 2]);
        }
    }
    
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid) {
                update(2 * node + 1, start, mid, idx, val);
            } else {
                update(2 * node + 2, mid + 1, end, idx, val);
            }
            tree[node] = std::max(tree[2 * node + 1], tree[2 * node + 2]);
        }
    }
    
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return INT_MIN; // 最大值查询的单位元
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = (start + end) / 2;
        int left_max = query(2 * node + 1, start, mid, l, r);
        int right_max = query(2 * node + 2, mid + 1, end, l, r);
        return std::max(left_max, right_max);
    }

public:
    MaxSegmentTree(const std::vector<int>& nums) {
        n = nums.size();
        if (n == 0) return;
        tree.resize(4 * n, INT_MIN);
        build(nums, 0, 0, n - 1);
    }
    
    void update(int index, int val) {
        update(0, 0, n - 1, index, val);
    }
    
    int query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }
};

int main() {
    std::cout << "Testing MaxSegmentTree..." << std::endl;
    
    std::vector<int> nums = {1, 3, 5, 7, 9, 11};
    MaxSegmentTree st(nums);
    
    // Test 1: Initial range query
    // Range [1, 3] corresponds to {3, 5, 7}, max is 7
    int max_val = st.query(1, 3);
    std::cout << "Query [1, 3] (expected 7): " << max_val << std::endl;
    assert(max_val == 7);
    
    // Test 2: Update and query
    // Update index 1 to 10. Array: {1, 10, 5, 7, 9, 11}
    st.update(1, 10);
    // Range [1, 3] corresponds to {10, 5, 7}, max is 10
    max_val = st.query(1, 3);
    std::cout << "Query [1, 3] after update (expected 10): " << max_val << std::endl;
    assert(max_val == 10);
    
    // Test 3: Full range query
    // Range [0, 5] corresponds to {1, 10, 5, 7, 9, 11}, max is 11
    max_val = st.query(0, 5);
    std::cout << "Query [0, 5] (expected 11): " << max_val << std::endl;
    assert(max_val == 11);
    
    // Test 4: Update to new max
    // Update index 4 to 20. Array: {1, 10, 5, 7, 20, 11}
    st.update(4, 20);
    max_val = st.query(0, 5);
    std::cout << "Query [0, 5] after update (expected 20): " << max_val << std::endl;
    assert(max_val == 20);

    // Test 5: Single element query
    max_val = st.query(2, 2);
    std::cout << "Query [2, 2] (expected 5): " << max_val << std::endl;
    assert(max_val == 5);
    
    std::cout << "All tests passed!" << std::endl;
    
    return 0;
}