#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Middle of the Linked List (LC 876)          | Fast & Slow Pointers (Tortoise/Hare)| O(N)   | O(1)     |
 | 2  | Detect Cycle in Linked List (LC 141)        | Floyd's Cycle-Finding Algorithm   | O(N)     | O(1)     |
 | 3  | Remove N-th Node From End of List (LC 19)   | Two Pointers with Dummy Sentinel  | O(N)     | O(1)     |
 | 4  | Intersection Point of Two Lists (LC 160)    | Two Pointers Alternate Redirection| O(L1+L2) | O(1)     |
 | 5  | Sort Linked List (Merge Sort on LL) (LC 148)| Top-Down Merge Sort + Midpoint Cut| O(N log N)| O(log N)|
 | 6  | Segregate Odd & Even Nodes in LL (LC 328)   | Two-Pointer Parity List Weaving   | O(N)     | O(1)     |
 | 7  | Reverse Linked List (LC 206)                | Iterative 3-Pointer Reversal      | O(N)     | O(1)     |
 | 8  | Merge Two Sorted Linked Lists (LC 21)       | Dummy Head + Linear Merge Step    | O(N + M) | O(1)     |
 | 9  | Merge K Sorted Linked Lists (LC 23)         | Min-Heap Priority Queue on Heads  | O(N log K)| O(K)    |
 | 10 | Reorder List (LC 143)                       | Midpoint Split + Reverse + Weave  | O(N)     | O(1)     |
 | 11 | Add Two Numbers (LC 2)                      | Linear Digit Simulation + Carry   | O(max(N,M))| O(1)   |
 | 12 | Delete Node in a Linked List O(1) (LC 237)  | Value Copying + Next Node Bypass  | O(1)     | O(1)     |
 | 13 | Reverse Nodes in k-Group (LC 25)            | Bounded K-Group Iterative Reversal| O(N)     | O(1)     |
 | 14 | Palindrome Linked List (LC 234)             | Midpoint Cut + Reverse Half + Match| O(N)    | O(1)     |
 | 15 | Find Starting Point of Loop in LL (LC 142)  | Floyd's Cycle + Head-Meeting Reset| O(N)     | O(1)     |
 | 16 | Flattening a Linked List (GFG)              | Bottom-Up Recursive Merge 2 Lists | O(N * K) | O(1)     |
 | 17 | Rotate List by K Places (LC 61)             | Circular Ring Cut at (Len - K % Len)| O(N)   | O(1)     |
 | 18 | Copy List with Random Pointer (LC 138)      | 3-Pass In-Place Node Interweaving | O(N)     | O(1)     |
 ====================================================================================================
*/

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct RandomNode {
    int val;
    RandomNode* next;
    RandomNode* random;
    RandomNode(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

struct FlattenNode {
    int data;
    FlattenNode *next;
    FlattenNode *bottom;
    FlattenNode(int d) : data(d), next(nullptr), bottom(nullptr) {}
};

class Solution {
public:

    // =========================================================
    // 1. MIDDLE OF THE LINKED LIST (LEETCODE 876)
    // =========================================================

    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
    // Interview Explanation:
    // - Problem Statement: Find middle node of linked list; if two middle nodes, return second.
    // - Approach: Floyd's Fast and Slow Pointers (Tortoise and Hare).
    // - Intuition: When fast moves 2 steps and slow moves 1 step, slow reaches exact midpoint when fast hits end.
    // - Complexity: Time: O(N) single pass, Space: O(1) auxiliary.


    // =========================================================
    // 2. DETECT CYCLE IN LINKED LIST (LEETCODE 141)
    // =========================================================

    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }
    // Interview Explanation:
    // - Problem Statement: Determine if linked list contains a cycle.
    // - Approach: Floyd's Cycle-Finding Algorithm.
    // - Intuition: In a cycle, relative distance between fast and slow decreases by 1 each step, guaranteeing collision.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 3. REMOVE N-TH NODE FROM END OF LIST (LEETCODE 19)
    // =========================================================

    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;

        for (int i = 0; i <= n; i++) {
            fast = fast->next;
        }

        while (fast) {
            slow = slow->next;
            fast = fast->next;
        }

        ListNode* nodeToDelete = slow->next;
        slow->next = slow->next->next;
        delete nodeToDelete;

        return dummy.next;
    }
    // Interview Explanation:
    // - Problem Statement: Remove N-th node from end of linked list in one pass.
    // - Approach: Two pointers separated by gap of N + 1 using a dummy head sentinel.
    // - Intuition: When fast reaches nullptr, slow rests on predecessor of node to remove.
    // - Complexity: Time: O(N) single pass, Space: O(1).


    // =========================================================
    // 4. INTERSECTION POINT OF TWO LINKED LISTS (LEETCODE 160)
    // =========================================================

    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        if (!headA || !headB) return nullptr;
        ListNode* ptrA = headA;
        ListNode* ptrB = headB;

        while (ptrA != ptrB) {
            ptrA = (ptrA == nullptr) ? headB : ptrA->next;
            ptrB = (ptrB == nullptr) ? headA : ptrB->next;
        }

        return ptrA;
    }
    // Interview Explanation:
    // - Problem Statement: Find node at which two singly linked lists intersect.
    // - Approach: Two pointers switching heads at end of their lists.
    // - Intuition: Both pointers traverse L1 + L2 steps, neutralizing length disparity.
    // - Complexity: Time: O(L1 + L2), Space: O(1).


    // =========================================================
    // 5. SORT LINKED LIST (MERGE SORT ON LL) (LEETCODE 148)
    // =========================================================

    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // Split list using slow & fast
        ListNode* prev = nullptr;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        prev->next = nullptr; // Sever left and right halves

        ListNode* left = sortList(head);
        ListNode* right = sortList(slow);

        return mergeTwoLists(left, right);
    }
    // Interview Explanation:
    // - Problem Statement: Sort a linked list in O(N log N) time and O(log N) stack space.
    // - Approach: Top-Down Merge Sort on Linked List.
    // - Intuition: Midpoint split with Tortoise/Hare followed by standard sorted list merge.
    // - Complexity: Time: O(N log N), Space: O(log N) recursion stack.


    // =========================================================
    // 6. SEGREGATE ODD AND EVEN NODES (LEETCODE 328)
    // =========================================================

    ListNode* oddEvenList(ListNode* head) {
        if (!head || !head->next) return head;

        ListNode* odd = head;
        ListNode* even = head->next;
        ListNode* evenHead = even;

        while (even && even->next) {
            odd->next = even->next;
            odd = odd->next;
            even->next = odd->next;
            even = even->next;
        }

        odd->next = evenHead;
        return head;
    }
    // Interview Explanation:
    // - Problem Statement: Group odd-indexed nodes followed by even-indexed nodes in O(1) space.
    // - Approach: Two-pointer weaving maintaining separate odd and even chains.
    // - Intuition: Connect odd->next to even->next, advance, then connect odd tail to evenHead.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 7. REVERSE LINKED LIST (LEETCODE 206)
    // =========================================================

    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }
        return prev;
    }
    // Interview Explanation:
    // - Problem Statement: Reverse singly linked list in-place iteratively.
    // - Approach: Three pointers (prev, curr, nextNode).
    // - Intuition: At each node, invert pointer to point backwards to prev.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 8. MERGE TWO SORTED LINKED LISTS (LEETCODE 21)
    // =========================================================

    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (list1 && list2) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        tail->next = list1 ? list1 : list2;
        return dummy.next;
    }
    // Interview Explanation:
    // - Problem Statement: Merge two sorted singly linked lists into one sorted list.
    // - Approach: Two-pointer comparison with dummy sentinel head.
    // - Complexity: Time: O(N + M), Space: O(1).


    // =========================================================
    // 9. MERGE K SORTED LINKED LISTS (LEETCODE 23)
    // =========================================================

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto comp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(comp)> pq(comp);

        for (ListNode* head : lists) {
            if (head) pq.push(head);
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (!pq.empty()) {
            ListNode* node = pq.top();
            pq.pop();
            tail->next = node;
            tail = tail->next;
            if (node->next) pq.push(node->next);
        }

        return dummy.next;
    }
    // Interview Explanation:
    // - Problem Statement: Merge K sorted linked lists into one sorted linked list.
    // - Approach: Min-Heap priority queue holding active heads of K lists.
    // - Intuition: Pop minimum head, attach to result, and insert its next node into heap.
    // - Complexity: Time: O(N log K) where N is total nodes, Space: O(K) for priority queue.


    // =========================================================
    // 10. REORDER LIST (LEETCODE 143)
    // =========================================================

    void reorderList(ListNode* head) {
        if (!head || !head->next) return;

        // 1. Find midpoint
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 2. Reverse second half
        ListNode* second = reverseList(slow->next);
        slow->next = nullptr;

        // 3. Weave two halves
        ListNode* first = head;
        while (second) {
            ListNode* tmp1 = first->next;
            ListNode* tmp2 = second->next;

            first->next = second;
            second->next = tmp1;

            first = tmp1;
            second = tmp2;
        }
    }
    // Interview Explanation:
    // - Problem Statement: Reorder L0 -> L1 -> ... -> Ln to L0 -> Ln -> L1 -> Ln-1 -> ... in-place.
    // - Approach: 3 Steps: Find Midpoint -> Reverse Second Half -> Interleave / Weave Two Lists.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 11. ADD TWO NUMBERS (LEETCODE 2)
    // =========================================================

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        int carry = 0;

        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) { sum += l1->val; l1 = l1->next; }
            if (l2) { sum += l2->val; l2 = l2->next; }
            carry = sum / 10;
            curr->next = new ListNode(sum % 10);
            curr = curr->next;
        }

        return dummy.next;
    }
    // Interview Explanation:
    // - Problem Statement: Add two numbers represented by reverse-ordered linked lists.
    // - Approach: Digit-by-digit summation simulating grade-school addition with carry.
    // - Complexity: Time: O(max(N, M)), Space: O(1) auxiliary space beyond result list.


    // =========================================================
    // 12. DELETE NODE IN A LINKED LIST O(1) (LEETCODE 237)
    // =========================================================

    void deleteNode(ListNode* node) {
        node->val = node->next->val;
        ListNode* temp = node->next;
        node->next = node->next->next;
        delete temp;
    }
    // Interview Explanation:
    // - Problem Statement: Delete node in a singly linked list given only access to that node (guaranteed not tail).
    // - Approach: Overwrite value with next node's value and bypass next node.
    // - Complexity: Time: O(1), Space: O(1).


    // =========================================================
    // 13. REVERSE NODES IN K-GROUP (LEETCODE 25)
    // =========================================================

    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head || k == 1) return head;

        ListNode dummy(0, head);
        ListNode* prevGroup = &dummy;

        while (true) {
            ListNode* kth = prevGroup;
            for (int i = 0; i < k && kth; i++) kth = kth->next;
            if (!kth) break;

            ListNode* nextGroup = kth->next;
            ListNode* prev = nextGroup;
            ListNode* curr = prevGroup->next;

            while (curr != nextGroup) {
                ListNode* nxt = curr->next;
                curr->next = prev;
                prev = curr;
                curr = nxt;
            }

            ListNode* groupHead = prevGroup->next;
            prevGroup->next = kth;
            prevGroup = groupHead;
        }

        return dummy.next;
    }
    // Interview Explanation:
    // - Problem Statement: Reverse nodes of a linked list k at a time; leave remaining < k nodes intact.
    // - Approach: Iterative group verification + bounded pointer reversal.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 14. PALINDROME LINKED LIST (LEETCODE 234)
    // =========================================================

    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;

        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* second = reverseList(slow->next);
        ListNode* first = head;

        bool palindrome = true;
        while (second) {
            if (first->val != second->val) {
                palindrome = false;
                break;
            }
            first = first->next;
            second = second->next;
        }

        return palindrome;
    }
    // Interview Explanation:
    // - Problem Statement: Determine if singly linked list values form a palindrome.
    // - Approach: Find midpoint -> Reverse second half -> Compare values pairwise.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 15. LINKED LIST CYCLE II (STARTING POINT) (LEETCODE 142)
    // =========================================================

    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                ListNode* entry = head;
                while (entry != slow) {
                    entry = entry->next;
                    slow = slow->next;
                }
                return entry;
            }
        }
        return nullptr;
    }
    // Interview Explanation:
    // - Problem Statement: Return node where cycle begins in linked list, or nullptr.
    // - Approach: Floyd's Cycle Detection + Entry pointer reset to head.
    // - Intuition: Mathematical invariant: distance from head to cycle entrance equals distance from meeting point to entrance modulo loop length.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 16. FLATTENING A LINKED LIST (GEEKSFORGEEKS)
    // =========================================================

    FlattenNode* mergeFlatten(FlattenNode* a, FlattenNode* b) {
        if (!a) return b;
        if (!b) return a;
        FlattenNode* res = nullptr;
        if (a->data < b->data) {
            res = a;
            res->bottom = mergeFlatten(a->bottom, b);
        } else {
            res = b;
            res->bottom = mergeFlatten(a, b->bottom);
        }
        res->next = nullptr;
        return res;
    }

    FlattenNode* flatten(FlattenNode* root) {
        if (!root || !root->next) return root;
        root->next = flatten(root->next);
        root = mergeFlatten(root, root->next);
        return root;
    }
    // Interview Explanation:
    // - Problem Statement: Flatten a 2D linked list where each node has `next` and sorted `bottom` pointers into a single sorted bottom-linked list.
    // - Approach: Post-order recursive merge of 2 sorted lists using `bottom` pointers.
    // - Complexity: Time: O(N * K) where N is number of main nodes and K is average depth, Space: O(N) recursion stack.


    // =========================================================
    // 17. ROTATE LIST BY K PLACES (LEETCODE 61)
    // =========================================================

    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;

        int len = 1;
        ListNode* tail = head;
        while (tail->next) {
            tail = tail->next;
            len++;
        }

        k = k % len;
        if (k == 0) return head;

        tail->next = head; // Make ring
        int stepsToNewTail = len - k;
        ListNode* newTail = head;
        for (int i = 1; i < stepsToNewTail; i++) {
            newTail = newTail->next;
        }

        ListNode* newHead = newTail->next;
        newTail->next = nullptr; // Sever ring

        return newHead;
    }
    // Interview Explanation:
    // - Problem Statement: Rotate list to right by k places.
    // - Approach: Calculate length, make circular ring by linking tail to head, then break ring at (len - k % len).
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 18. COPY LIST WITH RANDOM POINTER (LEETCODE 138)
    // =========================================================

    RandomNode* copyRandomList(RandomNode* head) {
        if (!head) return nullptr;

        // Step 1: Create interleaved duplicate nodes (A -> A' -> B -> B')
        RandomNode* curr = head;
        while (curr) {
            RandomNode* copy = new RandomNode(curr->val);
            copy->next = curr->next;
            curr->next = copy;
            curr = copy->next;
        }

        // Step 2: Copy random pointers
        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }

        // Step 3: Separate original and cloned lists
        RandomNode* dummy = new RandomNode(0);
        RandomNode* copyTail = dummy;
        curr = head;

        while (curr) {
            copyTail->next = curr->next;
            copyTail = copyTail->next;
            curr->next = curr->next->next;
            curr = curr->next;
        }

        RandomNode* clonedHead = dummy->next;
        delete dummy;
        return clonedHead;
    }
    // Interview Explanation:
    // - Problem Statement: Construct a deep copy of a linked list with next and random pointers in O(1) extra space.
    // - Approach: 3-Pass In-Place Interweaving:
    //   1. Interleave cloned nodes after original nodes.
    //   2. Assign cloned random pointers: `curr->next->random = curr->random->next`.
    //   3. Separate original and cloned lists.
    // - Complexity: Time: O(N) 3-pass linear scan, Space: O(1) auxiliary space.
};
