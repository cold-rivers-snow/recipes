// Source : https://oj.leetcode.com/problems/remove-nth-node-from-end-of-list/
// Author : Hao Chen
// Date   : 2014-06-21

/********************************************************************************** 
* 
* Given a linked list, remove the nth node from the end of list and return its head.
* 
* For example,
* 
*    Given linked list: 1->2->3->4->5, and n = 2.
* 
*    After removing the second node from the end, the linked list becomes 1->2->3->5.
* 
* Note:
* Given n will always be valid.
* Try to do this in one pass.
* 
*               
**********************************************************************************/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (head == NULL || n <= 0) {
            return NULL;
        }
        ListNode fakeHead(0);
        fakeHead.next = head;
        head = &fakeHead;

        ListNode *p1, *p2;
        p1 = p2 = head;
        for (int i = 0; i < n; i++) {
            if (p2 == NULL) return NULL;
            p2 = p2->next;
        }
        while (p2->next != NULL) {
            p2 = p2->next;
            p1 = p1->next;
        }

        p1->next = p1->next->next;
        return head->next;
    }
};


//hjx code
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 虚拟头结点
        ListNode* dummy = new ListNode(-1);
        dummy->next = head;
        // 删除倒数第 n 个，要先找倒数第 n + 1 个节点
        ListNode* x = findFromEnd(dummy, n + 1);
        // 删掉倒数第 n 个节点
        x->next = x->next->next;
        return dummy->next;
    }

    ListNode* findFromEnd(ListNode* head, int k) {
        ListNode* p1 = head;
        // p1 先走 k 步
        for (int i = 0; i < k; i++) {
            p1 = p1->next;
        }
        ListNode* p2 = head;
        // p1 和 p2 同时走 n - k 步
        while (p1 != nullptr) {
            p2 = p2->next;
            p1 = p1->next;
        }
        // p2 现在指向第 n - k + 1 个节点，即倒数第 k 个节点
        return p2;
    }
};