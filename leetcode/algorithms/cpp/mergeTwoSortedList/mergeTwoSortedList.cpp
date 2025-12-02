// Source : https://oj.leetcode.com/problems/merge-two-sorted-lists/
// Author : Hao Chen
// Date   : 2014-07-06

/********************************************************************************** 
* 
* Merge two sorted linked lists and return it as a new list. The new list should be 
* made by splicing together the nodes of the first two lists.
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
    Solution(){
        srand(time(NULL));
    }
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
        switch (random()%3){
            case 0:
                return mergeTwoLists01(l1, l2);
            case 1:
                return mergeTwoLists02(l1, l2);
            default:
                return mergeTwoLists03(l1, l2);
        }
    }
    
    /* merge the 2nd list into 1st list*/
    ListNode *mergeTwoLists01(ListNode* head1, ListNode* head2){
        ListNode *p1 = head1,  *p2=head2;
        static ListNode dummy(0);
    
        dummy.next = p1;
        ListNode *prev = &dummy;
    
        while(p1 && p2){
            if(p1->val < p2->val){
                prev = p1;
                p1 = p1->next;
            }else{
                prev->next = p2;
                p2 = p2->next;
                prev = prev->next;
                prev->next = p1;
            }
        }
        if (p2){
            prev->next = p2;
        }
    
        return dummy.next;
    }


    /* merge two lists to the new list */
    ListNode *mergeTwoLists02(ListNode *l1, ListNode *l2) {
        ListNode *l=NULL, *p=NULL;
        
        while (l1!=NULL && l2!=NULL ){
            ListNode *n=NULL;
            if (l1->val < l2-> val){
                n = l1;
                l1=l1->next;
            }else{
                n = l2;
                l2=l2->next;
            }
            if (l==NULL){
                l = p = n;
            }else{
                p->next = n;
                p = p->next;
            }
        }
        
        ListNode* rest = l1 ? l1 :l2;
        
        l = mergeTheRest(rest, l, p);
       
        
        return l;
    }

    ListNode* mergeTheRest(ListNode* l, ListNode*head, ListNode* tail){
        if (l){
            if (head && tail ){
                tail->next = l;
            }else{
                head = l;
            }
        }
        return head;
    }

    /*  
     * You can see the 2nd slution's code is quite complicated, 
     * because it need to check the (head==NULL) situation.
     * We can use the "pointer to pointer" to make the code more clean
     * however, this would be bad for performance.
     */
    ListNode *mergeTwoLists03(ListNode *l1, ListNode *l2) {
        ListNode *head = NULL;
        ListNode **pTail = &head;
        while (l1 != NULL && l2 != NULL) {
            if (l1->val < l2->val) {
                *pTail = l1;
                l1 = l1->next;
            } else {
                *pTail = l2;
                l2 = l2->next;
            }
            pTail = &(*pTail)->next;
        }
        *pTail = (l1 != NULL ? l1 : l2);
        return head;
    }

};


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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* head1 = list1;
        ListNode* head2 = list2;
        ListNode* head = new ListNode();
        ListNode* cur = head;
        while(head1 != nullptr && head2 != nullptr) {
            if (head1->val <= head2->val) {
                cur->next = head1;
                head1 = head1->next;
            } else {
                cur->next = head2;
                head2 = head2->next;
            }
            cur = cur->next;  //注意指针移动
        }
        if (head1) {
            cur->next = head1;
        }
        if (head2) {
            cur->next = head2;
        }
        return head->next;
    }
};