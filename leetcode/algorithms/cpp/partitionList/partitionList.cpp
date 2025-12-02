// Source : https://oj.leetcode.com/problems/partition-list/
// Author : Hao Chen
// Date   : 2014-06-21

/********************************************************************************** 
* 
* Given a linked list and a value x, partition it such that all nodes less than x come 
* before nodes greater than or equal to x.
* 
* You should preserve the original relative order of the nodes in each of the two partitions.
* 
* For example,
* Given 1->4->3->2->5->2 and x = 3,
* return 1->2->2->4->3->5.
* 
*               
**********************************************************************************/

#include <stdio.h>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

ListNode *partition(ListNode *head, int x) {
    ListNode fakeHead(0);
    fakeHead.next = head;
    head = &fakeHead;
    
    ListNode *pos = NULL;
    for(ListNode *p = head; p!=NULL && p->next!=NULL; ){
        if (!pos && p->next->val >= x){
            pos = p;
            p=p->next;
            continue;
        }
        if (pos && p->next->val < x){
            ListNode *pNext = p->next;
            p->next = pNext->next;
            pNext->next = pos->next;
            pos->next = pNext;
            pos = pNext;
            continue;
        }
        p=p->next;
    }
    
    return head->next;
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
    //int a[] = {1}; int x =2;
    //int a[] = {2,3,1}; int x=2;
    int a[] = {3,1,2}; int x=3;
    ListNode* p = createList(a, sizeof(a)/sizeof(int)); 
    printList(p);
    p = partition(p, x);
    printList(p);

    return 0;
}

//hjx code
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
    ListNode* partition(ListNode* head, int x) {
        ListNode* less = new ListNode();
        ListNode* great = new ListNode();
        ListNode* cur = head;
        ListNode* less_cur = less;
        ListNode* great_cur = great;
        while(cur) {
            if (cur->val < x) {
                less_cur->next = cur;
                less_cur = less_cur->next;
            } else {
                great_cur->next = cur;
                great_cur = great_cur->next;
            }
            ListNode* temp = cur->next;  //注意这三行，断开与原链表的连接
            cur->next = nullptr;
            cur = temp;
        }
        less_cur->next = great->next;
        return less->next;
    }
};