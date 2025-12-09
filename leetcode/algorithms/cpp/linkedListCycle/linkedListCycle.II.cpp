// Source : https://oj.leetcode.com/problems/linked-list-cycle-ii/
// Author : Hao Chen
// Date   : 2014-07-03

/********************************************************************************** 
* 
* Given a linked list, return the node where the cycle begins. If there is no cycle, return null.
* 
* Follow up:
* Can you solve it without using extra space?
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
    
private:
    ListNode *p1, *p2;
public:
    bool hasCycle(ListNode *head) {
        
        if (head==NULL) return false;
        p1=head;
        p2=head;
        
        while (p1!=NULL && p2!=NULL){
            
            p1=p1->next;
            
            if (p2->next == NULL) return false;
            
            p2=p2->next->next;
            
            if (p1==p2) return true;
        }
        
        return false;
        
    }  
    
    /* 
     * So, the idea is:
     *   1) Using the cycle-chcking algorithm.
     *   2) Once p1 and p1 meet, then reset p1 to head, and move p1 & p2 synchronously
     *      until p1 and p2 meet again, that place is the cycle's start-point 
     */
    ListNode *detectCycle(ListNode *head) {
        
        if (hasCycle(head)==false){
            return NULL;
        }
        
        p1 = head;
        
        while (p1!=p2) {
            p1 = p1->next;
            p2 = p2->next;
        }
        
        return p1;
    }
};

//hjx code 快慢指针， 快的每次走两步， 慢的每次走一步， 如果有环， 快的和慢的一定会相遇
//相遇后， 将快的指针重新指向头节点， 慢的指针保持不变， 然后快的指针每次走一步， 慢的指针每次走一步， 当快的指针和慢的指针相遇时， 就是环的入口
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
    ListNode *detectCycle(ListNode *head) {
        ListNode* fast = head;
        ListNode* slow = head;
        while(fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
            if (slow == fast)
                break;
        }
        if (!fast || !fast->next) {
            return nullptr;
        }
        slow = head;
        while(slow != fast) {
            fast = fast->next;
            slow = slow->next;
        }
        return slow;
    }
};