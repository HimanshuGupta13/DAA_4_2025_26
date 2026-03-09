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
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* temp = head;
        int c = 1;
        while(temp!=0 && c <= k){
            temp = temp->next;
            c++;
        }
        if(c <= k) {
            return head;
        }
        ListNode* pv = NULL;
        ListNode* cr = head;
        ListNode* next = NULL;
        c = 1;
        while(cr!=NULL && c <= k){
            next = cr->next;
            cr->next = pv;
            pv = cr;
            cr = next;
            c++;
        }
        head->next = reverseKGroup(cr, k);
        return pv;
    }
};