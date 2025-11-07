#include <iostream>
#include <vector>
#include <algorithm>  // 用于 std::max（维护节点高度，可选）
using namespace std;

// 二叉搜索树模板类（K：键类型，需支持比较；V：值类型）
template <typename K, typename V>
class BST {
private:
    // 节点结构（包含键值对、左右子树指针、子树节点数）
    struct TreeNode {
        K key;
        V value;
        TreeNode* left;
        TreeNode* right;
        int size;  // 以当前节点为根的子树节点总数（用于 select/rank）

        // 节点构造函数
        TreeNode(K k, V v) 
            : key(k), value(v), left(nullptr), right(nullptr), size(1) {}
    };

    TreeNode* root;  // BST 根节点

    // 辅助函数：获取子树节点数（空节点返回 0）
    int getSize(TreeNode* node) {
        return node == nullptr ? 0 : node->size;
    }

    // 辅助函数：更新节点的 size（左子树 size + 右子树 size + 1）
    void updateSize(TreeNode* node) {
        if (node != nullptr) {
            node->size = getSize(node->left) + getSize(node->right) + 1;
        }
    }

    // 【核心】增/改：递归插入节点，已存在则更新值
    TreeNode* put(TreeNode* node, K key, V value) {
        if (node == nullptr) {
            return new TreeNode(key, value);  // 新节点插入到叶子位置
        }

        // 按 BST 特性查找插入位置
        if (key < node->key) {
            node->left = put(node->left, key, value);
        } else if (key > node->key) {
            node->right = put(node->right, key, value);
        } else {
            node->value = value;  // key 已存在，更新 value
        }

        updateSize(node);  // 回溯时更新节点 size
        return node;
    }

    // 【核心】查：递归查找 key 对应的节点值
    V* get(TreeNode* node, K key) {
        if (node == nullptr) {
            return nullptr;  // key 不存在，返回空指针
        }

        if (key < node->key) {
            return get(node->left, key);  // 向左子树查找
        } else if (key > node->key) {
            return get(node->right, key);  // 向右子树查找
        } else {
            return &(node->value);  // 找到 key，返回 value 地址
        }
    }

    // 辅助函数：查找以 node 为根的最小节点（最左子节点）
    TreeNode* findMin(TreeNode* node) {
        if (node->left == nullptr) {
            return node;
        }
        return findMin(node->left);
    }

    // 辅助函数：删除以 node 为根的最小节点，返回新的根节点
    TreeNode* removeMin(TreeNode* node) {
        if (node->left == nullptr) {
            TreeNode* rightChild = node->right;
            delete node;  // 释放当前节点内存
            return rightChild;
        }
        node->left = removeMin(node->left);
        updateSize(node);  // 回溯时更新 size
        return node;
    }

    // 【核心】删：递归删除节点（处理叶子、单子树、双子树三种情况）
    TreeNode* remove(TreeNode* node, K key) {
        if (node == nullptr) {
            return nullptr;  // key 不存在，无需删除
        }

        // 第一步：找到目标节点
        if (key < node->key) {
            node->left = remove(node->left, key);
        } else if (key > node->key) {
            node->right = remove(node->right, key);
        } else {
            // 第二步：处理目标节点的删除
            if (node->left == nullptr) {  // 左子树为空，用右子树替代
                TreeNode* rightChild = node->right;
                delete node;
                return rightChild;
            }
            if (node->right == nullptr) {  // 右子树为空，用左子树替代
                TreeNode* leftChild = node->left;
                delete node;
                return leftChild;
            }

            // 双子树节点：用「后继节点」（右子树最小节点）替代当前节点
            TreeNode* successor = findMin(node->right);
            // 复制后继节点的键值到当前节点
            node->key = successor->key;
            node->value = successor->value;
            // 删除原后继节点
            node->right = removeMin(node->right);
        }

        updateSize(node);  // 回溯时更新 size
        return node;
    }

    // 【核心】keys()：中序遍历收集所有键（天然升序）
    void inorder(TreeNode* node, vector<K>& keyList) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left, keyList);    // 遍历左子树
        keyList.push_back(node->key);    // 访问当前节点
        inorder(node->right, keyList);   // 遍历右子树
    }

    // 【核心】selectKey：查找排名为 k 的键（k 从 0 开始，即第 k+1 小）
    TreeNode* select(TreeNode* node, int k) {
        if (node == nullptr) {
            return nullptr;  // 排名超出范围
        }

        int leftSize = getSize(node->left);  // 左子树节点数（小于当前节点的键数）
        if (leftSize > k) {
            return select(node->left, k);  // 排名在左子树
        } else if (leftSize == k) {
            return node;  // 当前节点即为排名 k 的键
        } else {
            // 排名在右子树，k 调整为 k - 左子树大小 - 1
            return select(node->right, k - leftSize - 1);
        }
    }

    // 【核心】rank：查找 key 的排名（小于 key 的键的个数）
    int rank(TreeNode* node, K key) {
        if (node == nullptr) {
            return 0;  // key 不存在，返回 0
        }

        if (key < node->key) {
            return rank(node->left, key);  // key 在左子树，排名为左子树中的排名
        } else if (key > node->key) {
            // key 在右子树，排名 = 左子树大小 + 1（当前节点） + 右子树中的排名
            return getSize(node->left) + 1 + rank(node->right, key);
        } else {
            return getSize(node->left);  // key 存在，排名 = 左子树大小
        }
    }

    // 辅助函数：递归释放所有节点内存（避免内存泄漏）
    void destroy(TreeNode* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    // BST 构造函数（初始化空树）
    BST() : root(nullptr) {}

    // BST 析构函数（释放所有节点内存）
    ~BST() {
        destroy(root);
    }

    // 对外接口：增/改
    void put(K key, V value) {
        root = put(root, key, value);
    }

    // 对外接口：查（返回 value 引用，不存在则抛出异常）
    V& get(K key) {
        V* valPtr = get(root, key);
        if (valPtr == nullptr) {
            throw runtime_error("Key not found");
        }
        return *valPtr;
    }

    // 对外接口：删
    void remove(K key) {
        root = remove(root, key);
    }

    // 对外接口：判断 key 是否存在
    bool containsKey(K key) {
        return get(root, key) != nullptr;
    }

    // 对外接口：firstKey（最小键）
    K firstKey() {
        if (root == nullptr) {
            throw runtime_error("BST is empty");
        }
        return findMin(root)->key;
    }

    // 对外接口：lastKey（最大键）
    K lastKey() {
        if (root == nullptr) {
            throw runtime_error("BST is empty");
        }
        TreeNode* curr = root;
        // 一直向右子树遍历，直到右子树为空
        while (curr->right != nullptr) {
            curr = curr->right;
        }
        return curr->key;
    }

    // 对外接口：keys（返回升序键集合）
    vector<K> keys() {
        vector<K> keyList;
        inorder(root, keyList);
        return keyList;
    }

    // 对外接口：selectKey（查找排名为 k 的键）
    K selectKey(int k) {
        TreeNode* target = select(root, k);
        if (target == nullptr) {
            throw runtime_error("Rank out of range");
        }
        return target->key;
    }

    // 对外接口：rank（查找 key 的排名）
    int rank(K key) {
        return rank(root, key);
    }

    // 对外接口：获取树的总节点数
    int size() {
        return getSize(root);
    }
};

// 测试代码（验证核心功能）
int main() {
    BST<int, string> bst;

    // 1. 增/改
    bst.put(3, "A");
    bst.put(1, "B");
    bst.put(4, "C");
    bst.put(2, "D");
    bst.put(3, "A+");  // 更新 key=3 的值

    // 2. 查
    cout << "get(3): " << bst.get(3) << endl;  // 输出 A+
    cout << "containsKey(2): " << (bst.containsKey(2) ? "true" : "false") << endl;  // true

    // 3. firstKey/lastKey
    cout << "firstKey: " << bst.firstKey() << endl;  // 1
    cout << "lastKey: " << bst.lastKey() << endl;    // 4

    // 4. keys（升序输出）
    vector<int> keyList = bst.keys();
    cout << "keys: ";
    for (int key : keyList) cout << key << " ";  // 输出 1 2 3 4
    cout << endl;

    // 5. selectKey/rank
    cout << "selectKey(2): " << bst.selectKey(2) << endl;  // 排名 2（第 3 小）的键是 3
    cout << "rank(4): " << bst.rank(4) << endl;            // 小于 4 的键有 3 个，排名为 3

    // 6. 删
    bst.remove(2);
    cout << "after remove(2), keys: ";
    keyList = bst.keys();
    for (int key : keyList) cout << key << " ";  // 输出 1 3 4
    cout << endl;

    return 0;
}