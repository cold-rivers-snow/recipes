// author jx H
// date 2025-09-27
//暴力破解法
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int* result = malloc(sizeof(int) * 2);
    for (int i = 0; i < numsSize - 1; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                result[0] = i, result[1] = j;
                *returnSize = 2;
                return result;
            }
        }
    }
    *returnSize = 0;
    return NULL;
}

//哈希表
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

struct hashTable{
    int key;
    int val;
    UT_hash_handle hh;
};

struct hashTable* hashtable;

struct hashTable* find(int ikey) {
    struct hashTable* tmp;
    HASH_FIND_INT(hashtable, &ikey, tmp);
    return tmp;
}

void insert(int ikey, int ival) {
    struct hashTable* it= find(ikey);
    if (it == NULL) {
        struct hashTable* tmp = malloc(sizeof(struct hashTable));
        tmp->key = ikey, tmp->val = ival;
        HASH_ADD_INT(hashtable, key, tmp);
    } else {
        it->val = ival;
    }
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    hashtable = NULL;
    for (int i = 0; i < numsSize; i++) {
        struct hashTable* it = find(target - nums[i]);
        if (it != NULL) {
            int* ret = malloc(sizeof(int) * 2);
            ret[0] = it->val;
            ret[1] = i;
            *returnSize = 2;
            return ret;
        }
        insert(nums[i],i);
    }
    *returnSize = 0;
    return NULL;
}

/*
`UT_hash_handle`、`HASH_FIND_INT` 和 `HASH_ADD_INT` 均来自 **uthash** 库——这是一个用 C 语言编写的轻量级哈希表实现，通过宏定义提供了哈希表的创建、查找、插入等功能。由于 C 标准库中没有内置哈希表，uthash 是开发中常用的第三方解决方案（只需包含一个头文件 `uthash.h` 即可使用）。


### 1. `UT_hash_handle hh;` 解析
`UT_hash_handle` 是 uthash 库的核心结构体，用于**维护哈希表的内部状态**（如链表指针、哈希值、桶索引等）。  
- 它必须作为自定义哈希表结构体的一个成员（如你代码中的 `struct hashTable`），用于 uthash 宏在内部管理哈希表节点。  
- 该成员不需要手动赋值或操作，仅供 uthash 库内部使用。


### 2. `HASH_FIND_INT` 宏解析
`HASH_FIND_INT` 是 uthash 提供的**按整数键查找哈希表**的宏，语法如下：  
```c
HASH_FIND_INT(hashtable, key_ptr, out_ptr);
```
- **参数说明**：  
  - `hashtable`：指向哈希表的指针（如你的代码中的 `hashtable`）；  
  - `key_ptr`：指向要查找的整数键的指针（如 `&ikey`）；  
  - `out_ptr`：输出参数，用于接收找到的节点指针（找到则为非 NULL，否则为 NULL）。  


### 3. `HASH_ADD_INT` 宏解析
`HASH_ADD_INT` 是 uthash 提供的**向哈希表插入整数键节点**的宏，语法如下：  
```c
HASH_ADD_INT(hashtable, key_member, node_ptr);
```
- **参数说明**：  
  - `hashtable`：指向哈希表的指针（会被宏自动更新，无需手动处理）；  
  - `key_member`：自定义结构体中表示“键”的成员名（如你的代码中的 `key`）；  
  - `node_ptr`：指向要插入的节点的指针（如 `tmp`）。  

### 总结
- `UT_hash_handle hh;`：哈希表节点的“管理字段”，必须包含在自定义结构体中，供 uthash 内部使用。  
- `HASH_FIND_INT`：按整数键查找哈希表中的节点。  
- `HASH_ADD_INT`：向哈希表插入以整数为键的节点。
*/