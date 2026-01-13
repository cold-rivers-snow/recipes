//hjx code
/*
两数相加 II
给你两个 非空 链表来代表两个非负整数。数字最高位位于链表开始位置。它们的每个节点只存储一位数字。将这两数相加会返回一个新的链表。

你可以假设除了数字 0 之外，这两个数字都不会以零开头。


示例1：

输入：l1 = [7,2,4,3], l2 = [5,6,4]
输出：[7,8,0,7]
示例2：

输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[8,0,7]
*/

//之前题目是根据链表的顺序，个位依次相加，这次正好相反，因此利用栈的特性，弹出时正好变成个位，同时注意，构建链表时使用头插法，保证链表的输出顺序

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        stack<int> s1, s2;
        while(l1) {
            s1.push(l1->val);
            l1 = l1->next;
        }
        while(l2) {
            s2.push(l2->val);
            l2 = l2->next;
        }
        ListNode* result = nullptr;
        int carry = 0;
        while(!s1.empty() || !s2.empty() || carry != 0) {
            int val1 = s1.empty() ? 0 : s1.top();
            int val2 = s2.empty() ? 0 : s2.top();
            if (!s1.empty()) {
                s1.pop();
            }
            if (!s2.empty()) {
                s2.pop();
            }
            int sum = val1 + val2 + carry;
            int curval = sum % 10;
            carry = sum / 10;
            
            // 头插法构建结果链表（保证正序）
            ListNode* newNode = new ListNode(curval);
            newNode->next = result;
            result = newNode;
        }
        return result;
    }
};