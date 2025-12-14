// Source : https://oj.leetcode.com/problems/maximum-depth-of-binary-tree/
// Author : Hao Chen
// Date   : 2014-06-21

/********************************************************************************** 
* 
* Given a binary tree, find its maximum depth.
* 
* The maximum depth is the number of nodes along the longest path from the root node 
* down to the farthest leaf node.
*               
**********************************************************************************/

/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int maxDepth(TreeNode *root) {
        if (root==NULL){
            return 0;
        }
        if (!root->left && !root->right){
            return 1;
        }
        int left=1, right=1;
        if (root->left){
            left += maxDepth(root->left);
        }
        if (root->right){
            right += maxDepth(root->right);
        }
        return left>right?left:right;
    }
    
};

class Solution2 {
public:
    int maxDepth(TreeNode *root) {
        if (root==NULL) return 0;
        return max(maxDepth(root->left), maxDepth(root->right)) + 1;
    }
};

//hjx code
//用了全局遍历，深度遍历二叉树，前序位置是进入的位置，所以加一，后序位置是离开的位置，所以减一。一定是在前序加一之后才判断是否到达子节点，此时的深度是多少。
class Solution {
public:
    int res = 0, depth = 0;
    int maxDepth(TreeNode* root) {
        traverse(root);
        return res;
    }
    void traverse(TreeNode* root) {
        if (!root)
            return;
        depth++;
        if (!root->left && !root->right) {
            res = max(res, depth);
        }
        traverse(root->left);
        traverse(root->right);
        depth--;
    }
};

//分解问题：二叉树的最大深度就是子树的最大深度，只需要找到左右子树的最大深度，然后+1（根节点）
// 分解问题的思路
class Solution {
public:
    // 定义：输入一个节点，返回以该节点为根的二叉树的最大深度
    int maxDepth(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        // 利用定义，计算左右子树的最大深度
        int leftMax = maxDepth(root->left);
        int rightMax = maxDepth(root->right);

        // 根据左右子树的最大深度推出原二叉树的最大深度
        // 整棵树的最大深度等于左右子树的最大深度取最大值，
        // 然后再加上根节点自己
        return 1 + std::max(leftMax, rightMax);
    }
};