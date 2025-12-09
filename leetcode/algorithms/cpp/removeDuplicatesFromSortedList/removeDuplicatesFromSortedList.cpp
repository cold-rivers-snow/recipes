// Source : https://oj.leetcode.com/problems/remove-duplicates-from-sorted-list/
// Author : Hao Chen
// Date   : 2014-06-21

/********************************************************************************** 
* 
* Given a sorted linked list, delete all duplicates such that each element appear only once.
* 
* For example,
* Given 1->1->2, return 1->2.
* Given 1->1->2->3->3, return 1->2->3.
* 
*               
**********************************************************************************/

#include <stdio.h>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

ListNode *deleteDuplicates(ListNode *head) {

    for(ListNode *p=head; p && p->next; ){
        if (p->val == p->next->val){
            p->next = p->next->next;
            continue;
        }
        p=p->next;
    }
    return head;
}

void printList(ListNode* h)
{
    while(h!=NULL){
        printf("%d ", h->val);
        h = h->next;
    }
    printf("\n");
}

ListNode* createList(int a[], int n)
{
    ListNode *head=NULL, *p=NULL;
    for(int i=0; i<n; i++){
        if (head == NULL){
            head = p = new ListNode(a[i]);
        }else{
            p->next = new ListNode(a[i]);
            p = p->next;
        }
    }
    return head;
}


int main()
{
    int a[]={1,1,2,3,3};
    int b[]={1,1,1};

    printList(deleteDuplicates(createList(a, sizeof(a)/sizeof(int))));
    printList(deleteDuplicates(createList(b, sizeof(b)/sizeof(int))));
    return 0;
}

//hjx code
//同removeDuplicatesFromSortedArray（26题），一个数组，一个链表，都是通过快指针找到不同的
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) {
            return nullptr;
        }
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast) {
            if (fast->val != slow->val) {
                slow->next = fast;
                slow = slow->next;
            }
            fast = fast->next;
        }
        slow->next = nullptr;
        return head;
    }
};