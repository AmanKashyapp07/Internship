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
#include <list>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

// Definition for singly-linked list node
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Definition for a multi-purpose Node (Random, Doubly, Tree, Circular)
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
    Node* random;
    Node* left;
    Node* right;
    Node() : val(0), prev(nullptr), next(nullptr), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val) : val(_val), prev(nullptr), next(nullptr), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _next) : val(_val), prev(nullptr), next(_next), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _prev, Node* _next) : val(_val), prev(_prev), next(_next), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _prev, Node* _next, Node* _child) : val(_val), prev(_prev), next(_next), child(_child), random(nullptr), left(nullptr), right(nullptr) {}
};

/*
 ====================================================================================================
                       LINKED LIST — TOP OA / INTERVIEW PROBLEMS (MICROSOFT-TIER)
 ====================================================================================================
 | #  | Problem Name                                     | Pattern / Technique                | Time      | Space  |
 |----|---------------------------------------------------|-------------------------------------|-----------|--------|
 | 01 | Reverse Linked List                                | Iterative Pointer Reversal          | O(N)      | O(1)   |
 | 02 | Reverse Linked List II                             | Sublist Pointer Inversion           | O(N)      | O(1)   |
 | 03 | Reverse Nodes in k-Group                           | Group Length Check + Sublist Rev    | O(N)      | O(1)   |
 | 04 | Merge Two Sorted Lists                             | Two-Pointer Linked Splice           | O(N+M)    | O(1)   |
 | 05 | Merge k Sorted Lists                               | Min-Heap Priority Queue Merge       | O(N logK) | O(K)   |
 | 06 | Sort List                                          | Merge Sort (Slow/Fast Split)        | O(N logN) | O(logN)|
 | 07 | Linked List Cycle                                  | Floyd's Tortoise & Hare             | O(N)      | O(1)   |
 | 08 | Linked List Cycle II (Find Cycle Start)            | Floyd's Cycle + Reset Pointer       | O(N)      | O(1)   |
 | 09 | Middle of the Linked List                          | Slow/Fast Pointer                   | O(N)      | O(1)   |
 | 10 | Remove Nth Node From End of List                   | Two-Pointer Gap Technique           | O(N)      | O(1)   |
 | 11 | Reorder List                                       | Mid Find + Reverse + Interleave     | O(N)      | O(1)   |
 | 12 | Palindrome Linked List                              | Mid Find + Reverse + Compare        | O(N)      | O(1)   |
 | 13 | Odd Even Linked List                                | In-place Bucket Splitting           | O(N)      | O(1)   |
 | 14 | Swap Nodes in Pairs                                 | Adjacent Node Pointer Swap          | O(N)      | O(1)   |
 | 15 | Rotate List                                         | Ring Formation & Modular Cut        | O(N)      | O(1)   |
 | 16 | Partition List                                      | Two Dummy Heads (Less/GreaterEq)    | O(N)      | O(1)   |
 | 17 | Remove Duplicates from Sorted List                 | Single Pass Skip                    | O(N)      | O(1)   |
 | 18 | Remove Duplicates from Sorted List II               | Dummy Node + Skip Entire Runs       | O(N)      | O(1)   |
 | 19 | Remove Linked List Elements                        | Dummy Node + Value Filter           | O(N)      | O(1)   |
 | 20 | Add Two Numbers                                     | Dummy Node + Carry Propagation      | O(max(N,M))| O(max(N,M))|
 | 21 | Add Two Numbers II                                 | Stack Reversal + Carry Propagation  | O(N+M)    | O(N+M) |
 | 22 | Intersection of Two Linked Lists                   | Two-Pointer Switch-Path Alignment   | O(N+M)    | O(1)   |
 | 23 | Copy List with Random Pointer                       | Interleaving Clone Nodes            | O(N)      | O(1)   |
 | 24 | Flatten a Multilevel Doubly Linked List             | DFS Stack-Based Flattening          | O(N)      | O(N)   |
 | 25 | Convert BST to Sorted Doubly Linked List            | In-order Traversal + Node Relinking | O(N)      | O(H)   |
 | 26 | Split Linked List in Parts                          | Size Calc + Balanced Chunking       | O(N+K)    | O(K)   |
 | 27 | Next Greater Node In Linked List                    | Stack (Monotonic) on Array Convert  | O(N)      | O(N)   |
 | 28 | Insert into a Sorted Circular Linked List           | Circular Traversal Edge Handling    | O(N)      | O(1)   |
 | 29 | Merge In Between Linked Lists                       | Pointer Splice at Boundaries        | O(N+M)    | O(1)   |
 | 30 | Delete Node in a Linked List (no head access)       | Value/Next Copy Trick               | O(1)      | O(1)   |
 | 31 | Design Linked List                                  | Custom Singly/Doubly Implementation | O(N)/op   | O(N)   |
 | 32 | LRU Cache                                           | Hash Map + Doubly-Linked List        | O(1) ops  | O(C)   |
 | 33 | LFU Cache                                           | Hash Map + Doubly-Linked Buckets    | O(1) ops  | O(C)   |
 | 34 | Find the Duplicate Number                           | Array-as-Linked-List Floyd Cycle    | O(N)      | O(1)   |
 | 35 | Design Browser History                              | Doubly-Linked List / Two Stacks     | O(1) avg  | O(N)   |
 ====================================================================================================
*/

// =========================================================
// 1. REVERSE LINKED LIST (LC 206)
// =========================================================

class Solution1 {
public:
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
};
// Interview Explanation:
// - Problem Statement: Reverse a singly linked list and return the reversed list head.
// - Approach: Three-pointer iterative pointer reversal (`prev`, `curr`, `nextNode`).
// - Intuition: At each node, invert its `next` pointer to point to `prev`, then advance `prev` and `curr`.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 2. REVERSE LINKED LIST II (LC 92)
// =========================================================

class Solution2 {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if (!head || left == right) return head;

        ListNode dummy(0, head);
        ListNode* prev = &dummy;
        for (int i = 1; i < left; ++i) {
            prev = prev->next;
        }

        ListNode* curr = prev->next;
        for (int i = 0; i < right - left; ++i) {
            ListNode* temp = curr->next;
            curr->next = temp->next;
            temp->next = prev->next;
            prev->next = temp;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Reverse nodes from position `left` to `right` in a single pass.
// - Approach: Locate the node preceding `left`. Repeatedly extract `curr->next` and splice it directly after `prev`.
// - Intuition: Head-insertion inside the window reverses the subsegment in-place without rebuilding.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 3. REVERSE NODES IN K-GROUP (LC 25)
// =========================================================

class Solution3 {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0, head);
        ListNode* groupPrev = &dummy;

        while (true) {
            ListNode* kth = groupPrev;
            for (int i = 0; i < k && kth; ++i) {
                kth = kth->next;
            }
            if (!kth) break;

            ListNode* groupNext = kth->next;
            ListNode* prev = groupNext;
            ListNode* curr = groupPrev->next;

            while (curr != groupNext) {
                ListNode* temp = curr->next;
                curr->next = prev;
                prev = curr;
                curr = temp;
            }

            ListNode* temp = groupPrev->next;
            groupPrev->next = kth;
            groupPrev = temp;
        }

        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Reverse nodes of a linked list in chunks of size `k`. Remaining nodes at end stay unreversed.
// - Approach: Check if $k$ nodes exist ahead. If so, reverse the $k$-window with `groupNext` as initial `prev`.
// - Intuition: Connect reversed subsegment tail to next group, then advance `groupPrev` to end of reversed group.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 4. MERGE TWO SORTED LISTS (LC 21)
// =========================================================

class Solution4 {
public:
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
};
// Interview Explanation:
// - Problem Statement: Merge two sorted singly linked lists into one sorted linked list.
// - Approach: Two pointers with dummy head. Append the smaller node at each step and splice remaining list at the end.
// - Intuition: Splicing existing nodes preserves memory allocations and achieves linear time.
// - Complexity: Time: O(N + M), Space: O(1) auxiliary space.


// =========================================================
// 5. MERGE K SORTED LISTS (LC 23)
// =========================================================

struct CompareListNode {
    bool operator()(const ListNode* a, const ListNode* b) const {
        return a->val > b->val;
    }
};

class Solution5 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        priority_queue<ListNode*, vector<ListNode*>, CompareListNode> pq;
        for (ListNode* node : lists) {
            if (node) pq.push(node);
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (!pq.empty()) {
            ListNode* smallest = pq.top();
            pq.pop();
            tail->next = smallest;
            tail = tail->next;
            if (smallest->next) {
                pq.push(smallest->next);
            }
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Merge `k` sorted linked lists into one consolidated sorted list.
// - Approach: Min-heap priority queue storing current heads of all non-empty lists.
// - Intuition: Heap size is at most $k$. Popping the minimum takes $O(\log k)$, and pushing its successor maintains sorted ordering.
// - Complexity: Time: O(N log k), Space: O(k) for the priority queue (where N is total nodes).


// =========================================================
// 6. SORT LIST (LC 148)
// =========================================================

class Solution6 {
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }

public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        ListNode* prev = nullptr;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        prev->next = nullptr; // Sever list into two halves

        ListNode* left = sortList(head);
        ListNode* right = sortList(slow);
        return merge(left, right);
    }
};
// Interview Explanation:
// - Problem Statement: Sort a linked list in O(N log N) time and O(log N) stack space.
// - Approach: Divide and conquer merge sort. Find middle using fast/slow pointers, sever list, recurse on both halves, and merge.
// - Intuition: Linked lists support O(1) in-place merging, avoiding auxiliary array allocation.
// - Complexity: Time: O(N log N), Space: O(log N) recursion stack space.


// =========================================================
// 7. LINKED LIST CYCLE (LC 141)
// =========================================================

class Solution7 {
public:
    bool hasCycle(ListNode *head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if a linked list contains a cycle.
// - Approach: Floyd's cycle-finding algorithm (Tortoise and Hare).
// - Intuition: Fast pointer moves at speed 2, slow at speed 1. If a cycle exists, relative speed is 1, so fast must catch slow.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 8. LINKED LIST CYCLE II (FIND CYCLE START) (LC 142)
// =========================================================

class Solution8 {
public:
    ListNode *detectCycle(ListNode *head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                ListNode* ptr = head;
                while (ptr != slow) {
                    ptr = ptr->next;
                    slow = slow->next;
                }
                return ptr;
            }
        }
        return nullptr;
    }
};
// Interview Explanation:
// - Problem Statement: Return the node where cycle begins. If no cycle exists, return nullptr.
// - Approach: Detect cycle using Floyd's algorithm. When slow meets fast, reset one pointer to head and advance both by 1 step.
// - Intuition: Distance from head to cycle entrance equals distance from meeting point to cycle entrance modulo cycle length.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 9. MIDDLE OF THE LINKED LIST (LC 876)
// =========================================================

class Solution9 {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
};
// Interview Explanation:
// - Problem Statement: Return middle node of linked list. If two middle nodes exist, return second middle node.
// - Approach: Two pointers where slow moves 1 step and fast moves 2 steps.
// - Intuition: When fast reaches end or one step past, slow lands on index $\lfloor N/2 \rfloor$.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 10. REMOVE NTH NODE FROM END OF LIST (LC 19)
// =========================================================

class Solution10 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;

        for (int i = 0; i < n; ++i) {
            fast = fast->next;
        }

        while (fast->next) {
            slow = slow->next;
            fast = fast->next;
        }

        ListNode* toDelete = slow->next;
        slow->next = slow->next->next;
        delete toDelete;
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Remove $n$-th node from end of list and return its head.
// - Approach: Two pointers with gap of $n$. Advance `fast` $n$ steps ahead of `slow`, then advance both together.
// - Intuition: When `fast` reaches the last node, `slow` sits right before the node to be removed.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 11. REORDER LIST (LC 143)
// =========================================================

class Solution11 {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;

        // 1. Find middle
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 2. Reverse second half
        ListNode* prev = nullptr;
        ListNode* curr = slow->next;
        slow->next = nullptr;
        while (curr) {
            ListNode* nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }

        // 3. Interleave two halves
        ListNode* first = head;
        ListNode* second = prev;
        while (second) {
            ListNode* t1 = first->next;
            ListNode* t2 = second->next;
            first->next = second;
            second->next = t1;
            first = t1;
            second = t2;
        }
    }
};
// Interview Explanation:
// - Problem Statement: Reorder list into $L_0 \to L_n \to L_1 \to L_{n-1} \dots$ in-place.
// - Approach: 1) Find middle node; 2) Reverse second half; 3) Interleave two halves alternately.
// - Intuition: Break complex reordering into 3 fundamental pointer manipulation building blocks.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 12. PALINDROME LINKED LIST (LC 234)
// =========================================================

class Solution12 {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;

        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // Reverse second half
        ListNode* prev = nullptr;
        ListNode* curr = slow->next;
        while (curr) {
            ListNode* nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }

        // Compare first half and reversed second half
        ListNode* p1 = head;
        ListNode* p2 = prev;
        bool match = true;
        while (p2) {
            if (p1->val != p2->val) {
                match = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        return match;
    }
};
// Interview Explanation:
// - Problem Statement: Determine whether a singly linked list is a palindrome.
// - Approach: Find midpoint, reverse second half in-place, and compare element-by-element with first half.
// - Intuition: Reversal enables bidirectional matching without requiring $O(N)$ extra memory.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 13. ODD EVEN LINKED LIST (LC 328)
// =========================================================

class Solution13 {
public:
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
};
// Interview Explanation:
// - Problem Statement: Group odd-indexed nodes together followed by even-indexed nodes in O(1) space.
// - Approach: Maintain two pointers `odd` and `even`, alternating forward jumps, then link `odd` tail to `evenHead`.
// - Intuition: In-place pointer skipping cleanly divides nodes by parity in a single linear pass.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 14. SWAP NODES IN PAIRS (LC 24)
// =========================================================

class Solution14 {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;
            ListNode* second = first->next;

            first->next = second->next;
            second->next = first;
            prev->next = second;

            prev = first;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Swap every two adjacent nodes and return head without modifying values.
// - Approach: Use dummy head and iteratively rewire pointers for pairs $(A, B)$ into $(B, A)$.
// - Intuition: For pair $(A, B)$ after `prev`: link `prev->next = B`, `B->next = A`, `A->next = nextPair`.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 15. ROTATE LIST (LC 61)
// =========================================================

class Solution15 {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;

        int len = 1;
        ListNode* tail = head;
        while (tail->next) {
            tail = tail->next;
            len++;
        }

        k %= len;
        if (k == 0) return head;

        tail->next = head; // Form circular ring

        ListNode* newTail = head;
        for (int i = 0; i < len - k - 1; ++i) {
            newTail = newTail->next;
        }

        ListNode* newHead = newTail->next;
        newTail->next = nullptr; // Sever ring
        return newHead;
    }
};
// Interview Explanation:
// - Problem Statement: Rotate the list to the right by $k$ places.
// - Approach: Calculate length, close the list into a circular ring, traverse $(len - k \% len - 1)$ steps to new tail, and break ring.
// - Intuition: Right-rotation by $k$ shifts new head to index $(len - k \pmod{len})$.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 16. PARTITION LIST (LC 86)
// =========================================================

class Solution16 {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode lessDummy(0);
        ListNode* less = &lessDummy;
        ListNode greaterDummy(0);
        ListNode* greater = &greaterDummy;

        while (head) {
            if (head->val < x) {
                less->next = head;
                less = less->next;
            } else {
                greater->next = head;
                greater = greater->next;
            }
            head = head->next;
        }
        greater->next = nullptr;
        less->next = greaterDummy.next;
        return lessDummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Partition list so nodes $< x$ precede nodes $\ge x$ while preserving relative original order.
// - Approach: Two separate dummy heads (`less` and `ge`). Append each node to appropriate chain, then concatenate.
// - Intuition: Stable two-way partitioning using two accumulator linked lists.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 17. REMOVE DUPLICATES FROM SORTED LIST (LC 83)
// =========================================================

class Solution17 {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* curr = head;
        while (curr && curr->next) {
            if (curr->val == curr->next->val) {
                ListNode* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Remove duplicate values from a sorted list such that each value appears once.
// - Approach: Single pass. If `curr->val == curr->next->val`, skip duplicate node; otherwise advance `curr`.
// - Intuition: Sorted order guarantees all identical elements are contiguous.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 18. REMOVE DUPLICATES FROM SORTED LIST II (LC 82)
// =========================================================

class Solution18 {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        while (head) {
            if (head->next && head->val == head->next->val) {
                while (head->next && head->val == head->next->val) {
                    head = head->next;
                }
                prev->next = head->next;
            } else {
                prev = prev->next;
            }
            head = head->next;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Delete all nodes that have duplicate numbers, leaving only distinct numbers from original list.
// - Approach: Dummy node with `prev`. When duplicate run detected, fast-forward `head` to run end and link `prev->next = head->next`.
// - Intuition: Whole duplicate cluster is dropped without moving `prev` forward until a unique node is found.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 19. REMOVE LINKED LIST ELEMENTS (LC 203)
// =========================================================

class Solution19 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode dummy(0, head);
        ListNode* curr = &dummy;

        while (curr->next) {
            if (curr->next->val == val) {
                ListNode* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Remove all nodes in linked list that have value equal to `val`.
// - Approach: Dummy node before head. Iterate and unlink matching nodes.
// - Intuition: Dummy node unifies head deletion with general interior node deletion.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 20. ADD TWO NUMBERS (LC 2)
// =========================================================

class Solution20 {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        int carry = 0;

        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }
            carry = sum / 10;
            tail->next = new ListNode(sum % 10);
            tail = tail->next;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Add two numbers represented by linked lists in reverse order.
// - Approach: Dummy head accumulator. Sum digits along with carry, append new node `sum % 10`, propagate `carry = sum / 10`.
// - Intuition: Standard schoolbook addition traversing least-to-most significant digits.
// - Complexity: Time: O(max(N, M)), Space: O(max(N, M)) for output list.


// =========================================================
// 21. ADD TWO NUMBERS II (LC 445)
// =========================================================

class Solution21 {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        stack<int> s1, s2;
        while (l1) {
            s1.push(l1->val);
            l1 = l1->next;
        }
        while (l2) {
            s2.push(l2->val);
            l2 = l2->next;
        }

        ListNode* head = nullptr;
        int carry = 0;

        while (!s1.empty() || !s2.empty() || carry) {
            int sum = carry;
            if (!s1.empty()) {
                sum += s1.top();
                s1.pop();
            }
            if (!s2.empty()) {
                sum += s2.top();
                s2.pop();
            }
            carry = sum / 10;
            ListNode* node = new ListNode(sum % 10);
            node->next = head;
            head = node;
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Add two numbers represented by linked lists where digits are stored most-significant-first, without modifying inputs.
// - Approach: Push values onto two stacks, pop to add from least significant digits, prepend new nodes to head.
// - Intuition: Stacks reverse digit order non-destructively without modifying inputs.
// - Complexity: Time: O(N + M), Space: O(N + M) auxiliary stack space.


// =========================================================
// 22. INTERSECTION OF TWO LINKED LISTS (LC 160)
// =========================================================

class Solution22 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) return nullptr;
        ListNode* pA = headA;
        ListNode* pB = headB;

        while (pA != pB) {
            pA = pA ? pA->next : headB;
            pB = pB ? pB->next : headA;
        }
        return pA;
    }
};
// Interview Explanation:
// - Problem Statement: Find node where two singly linked lists intersect.
// - Approach: Two pointers traverse their respective lists. When reaching end, redirect to opposite list head.
// - Intuition: Path equalization: $(lenA + lenB) = (lenB + lenA)$. Both pointers arrive at intersection or nullptr simultaneously.
// - Complexity: Time: O(N + M), Space: O(1) auxiliary space.


// =========================================================
// 23. COPY LIST WITH RANDOM POINTER (LC 138)
// =========================================================

class Solution23 {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;

        // Step 1: Interleave cloned nodes
        Node* curr = head;
        while (curr) {
            Node* clone = new Node(curr->val);
            clone->next = curr->next;
            curr->next = clone;
            curr = clone->next;
        }

        // Step 2: Assign cloned random pointers
        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }

        // Step 3: Separate original and cloned lists
        Node* dummy = new Node(0);
        Node* cloneTail = dummy;
        curr = head;

        while (curr) {
            Node* clone = curr->next;
            curr->next = clone->next;
            cloneTail->next = clone;
            cloneTail = clone;
            curr = curr->next;
        }

        Node* newHead = dummy->next;
        delete dummy;
        return newHead;
    }
};
// Interview Explanation:
// - Problem Statement: Deep copy a linked list with next and random pointers in O(1) auxiliary space.
// - Approach: 3-pass weave algorithm: 1) Insert copy nodes right after original nodes; 2) Set `copy->random = orig->random->next`; 3) Unweave lists.
// - Intuition: Interleaving original and copy avoids $O(N)$ hash map memory.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 24. FLATTEN A MULTILEVEL DOUBLY LINKED LIST (LC 430)
// =========================================================

class Solution24 {
public:
    Node* flatten(Node* head) {
        if (!head) return nullptr;
        Node* curr = head;

        while (curr) {
            if (curr->child) {
                Node* nextNode = curr->next;
                Node* childTail = curr->child;

                while (childTail->next) {
                    childTail = childTail->next;
                }

                curr->next = curr->child;
                curr->child->prev = curr;
                curr->child = nullptr;

                if (nextNode) {
                    childTail->next = nextNode;
                    nextNode->prev = childTail;
                }
            }
            curr = curr->next;
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Flatten a multilevel doubly linked list with child pointers into a single-level doubly linked list.
// - Approach: When encountering a child node, find child's tail, splice child list between `curr` and `curr->next`, reset child to null.
// - Intuition: In-place pointer re-linking flattens nested lists level-by-level without recursion overhead.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 25. CONVERT BST TO SORTED DOUBLY LINKED LIST (LC 426)
// =========================================================

class Solution25 {
    Node* first = nullptr;
    Node* prev = nullptr;

    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);

        if (prev) {
            prev->right = node;
            node->left = prev;
        } else {
            first = node;
        }
        prev = node;

        inorder(node->right);
    }

public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        first = nullptr;
        prev = nullptr;

        inorder(root);

        // Make doubly linked list circular
        prev->right = first;
        first->left = prev;

        return first;
    }
};
// Interview Explanation:
// - Problem Statement: Convert a Binary Search Tree to a circular sorted Doubly Linked List in-place.
// - Approach: Recursive inorder traversal. Link `prev->right = curr` and `curr->left = prev`. Connect head and tail at the end.
// - Intuition: Inorder traversal naturally visits BST nodes in monotonically increasing order.
// - Complexity: Time: O(N), Space: O(H) recursion stack space (where H is tree height).


// =========================================================
// 26. SPLIT LINKED LIST IN PARTS (LC 725)
// =========================================================

class Solution26 {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        int len = 0;
        ListNode* curr = head;
        while (curr) {
            len++;
            curr = curr->next;
        }

        int partSize = len / k;
        int remainder = len % k;

        vector<ListNode*> result(k, nullptr);
        curr = head;

        for (int i = 0; i < k && curr; ++i) {
            result[i] = curr;
            int currentChunkSize = partSize + (i < remainder ? 1 : 0);

            for (int j = 1; j < currentChunkSize; ++j) {
                curr = curr->next;
            }

            ListNode* nextPart = curr->next;
            curr->next = nullptr;
            curr = nextPart;
        }

        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Split linked list into $k$ consecutive parts of balanced sizes (difference $\le 1$, earlier parts larger).
// - Approach: Compute length $N$. Each bucket receives base size $N / k$, and first $N \% k$ buckets receive 1 extra node. Sever links after each chunk.
// - Intuition: Partitioning remainder evenly across initial parts distributes extra nodes with difference at most 1.
// - Complexity: Time: O(N + K), Space: O(K) for result array.


// =========================================================
// 27. NEXT GREATER NODE IN LINKED LIST (LC 1019)
// =========================================================

class Solution27 {
public:
    vector<int> nextLargerNodes(ListNode* head) {
        vector<int> vals;
        while (head) {
            vals.push_back(head->val);
            head = head->next;
        }

        int n = vals.size();
        vector<int> result(n, 0);
        stack<int> st;

        for (int i = 0; i < n; ++i) {
            while (!st.empty() && vals[st.top()] < vals[i]) {
                result[st.top()] = vals[i];
                st.pop();
            }
            st.push(i);
        }

        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Find value of next strictly greater node for each node in linked list. Return 0 if none.
// - Approach: Transfer list values into an array, then utilize a monotonic decreasing index stack.
// - Intuition: Monotonic stack efficiently resolves next greater element in one pass by popping smaller preceding values.
// - Complexity: Time: O(N), Space: O(N) auxiliary space.


// =========================================================
// 28. INSERT INTO A SORTED CIRCULAR LINKED LIST (LC 708)
// =========================================================

class Solution28 {
public:
    Node* insert(Node* head, int insertVal) {
        if (!head) {
            Node* newNode = new Node(insertVal);
            newNode->next = newNode;
            return newNode;
        }

        Node* prev = head;
        Node* curr = head->next;
        bool inserted = false;

        do {
            // Case 1: insertVal lies between prev and curr
            if (prev->val <= insertVal && insertVal <= curr->val) {
                inserted = true;
                break;
            }
            // Case 2: Boundary inflection point (max element wrapping to min element)
            if (prev->val > curr->val) {
                if (insertVal >= prev->val || insertVal <= curr->val) {
                    inserted = true;
                    break;
                }
            }
            prev = curr;
            curr = curr->next;
        } while (prev != head);

        // Case 3: Insertion spot found, or all elements identical / loop finished
        prev->next = new Node(insertVal, curr);
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Insert value into a sorted circular linked list in proper sorted position.
// - Approach: Traverse circular list. Check if `insertVal` fits monotonically between `prev` and `curr`, or spans across min/max boundary point.
// - Intuition: If no interior point matches, value must be either strictly $\ge \max$ or $\le \min$ at the wrap-around edge.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 29. MERGE IN BETWEEN LINKED LISTS (LC 1669)
// =========================================================

class Solution29 {
public:
    ListNode* mergeInBetween(ListNode* list1, int a, int b, ListNode* list2) {
        ListNode* prevA = list1;
        for (int i = 0; i < a - 1; ++i) {
            prevA = prevA->next;
        }

        ListNode* afterB = prevA;
        for (int i = 0; i < b - a + 2; ++i) {
            afterB = afterB->next;
        }

        prevA->next = list2;

        ListNode* tail2 = list2;
        while (tail2->next) {
            tail2 = tail2->next;
        }

        tail2->next = afterB;
        return list1;
    }
};
// Interview Explanation:
// - Problem Statement: Remove nodes from index $a$ to $b$ in `list1`, and splice `list2` in their place.
// - Approach: Advance pointer to index $a - 1$ and index $b + 1$. Wire $(a - 1)$ to `list2` head, and `list2` tail to $(b + 1)$.
// - Intuition: Constant number of pointer reconnections once boundary positions are reached.
// - Complexity: Time: O(N + M), Space: O(1) auxiliary space.


// =========================================================
// 30. DELETE NODE IN A LINKED LIST (NO HEAD ACCESS) (LC 237)
// =========================================================

class Solution30 {
public:
    void deleteNode(ListNode* node) {
        ListNode* nextNode = node->next;
        node->val = nextNode->val;
        node->next = nextNode->next;
        delete nextNode;
    }
};
// Interview Explanation:
// - Problem Statement: Delete given node without access to list head (guaranteed not to be tail).
// - Approach: Overwrite current node's value with successor's value, then bypass successor.
// - Intuition: Value copying converts deletion of target into deletion of next node.
// - Complexity: Time: O(1), Space: O(1) auxiliary space.


// =========================================================
// 31. DESIGN LINKED LIST (LC 707)
// =========================================================

class Solution31 {
public:
    class MyLinkedList {
        struct DNode {
            int val;
            DNode* prev;
            DNode* next;
            DNode(int v) : val(v), prev(nullptr), next(nullptr) {}
        };

        DNode* head;
        DNode* tail;
        int size;

    public:
        MyLinkedList() {
            head = new DNode(0);
            tail = new DNode(0);
            head->next = tail;
            tail->prev = head;
            size = 0;
        }

        int get(int index) {
            if (index < 0 || index >= size) return -1;
            DNode* curr = head->next;
            for (int i = 0; i < index; ++i) curr = curr->next;
            return curr->val;
        }

        void addAtHead(int val) {
            addAtIndex(0, val);
        }

        void addAtTail(int val) {
            addAtIndex(size, val);
        }

        void addAtIndex(int index, int val) {
            if (index < 0 || index > size) return;
            DNode* pred = head;
            for (int i = 0; i < index; ++i) pred = pred->next;
            DNode* succ = pred->next;

            DNode* node = new DNode(val);
            node->prev = pred;
            node->next = succ;
            pred->next = node;
            succ->prev = node;
            size++;
        }

        void deleteAtIndex(int index) {
            if (index < 0 || index >= size) return;
            DNode* pred = head;
            for (int i = 0; i < index; ++i) pred = pred->next;
            DNode* toDelete = pred->next;
            DNode* succ = toDelete->next;

            pred->next = succ;
            succ->prev = pred;
            delete toDelete;
            size--;
        }
    };
};
using MyLinkedList = Solution31::MyLinkedList;

// Interview Explanation:
// - Problem Statement: Design singly/doubly linked list supporting get, addAtHead, addAtTail, addAtIndex, deleteAtIndex.
// - Approach: Doubly-linked list with sentinel head and tail nodes to simplify edge deletions/insertions.
// - Intuition: Sentinel nodes eliminate null checks at boundaries.
// - Complexity: Time: O(index) for get/addAtIndex/deleteAtIndex, O(1) for addAtHead/addAtTail, Space: O(N).


// =========================================================
// 32. LRU CACHE (LC 146)
// =========================================================

class Solution32 {
public:
    class LRUCache {
        struct CacheNode {
            int key;
            int val;
            CacheNode* prev;
            CacheNode* next;
            CacheNode(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
        };

        int cap;
        unordered_map<int, CacheNode*> mp;
        CacheNode* head;
        CacheNode* tail;

        void removeNode(CacheNode* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        void addToHead(CacheNode* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
        }

        void moveToHead(CacheNode* node) {
            removeNode(node);
            addToHead(node);
        }

    public:
        LRUCache(int capacity) : cap(capacity) {
            head = new CacheNode(0, 0);
            tail = new CacheNode(0, 0);
            head->next = tail;
            tail->prev = head;
        }

        int get(int key) {
            if (!mp.count(key)) return -1;
            CacheNode* node = mp[key];
            moveToHead(node);
            return node->val;
        }

        void put(int key, int value) {
            if (mp.count(key)) {
                CacheNode* node = mp[key];
                node->val = value;
                moveToHead(node);
            } else {
                if ((int)mp.size() >= cap) {
                    CacheNode* lru = tail->prev;
                    mp.erase(lru->key);
                    removeNode(lru);
                    delete lru;
                }
                CacheNode* newNode = new CacheNode(key, value);
                mp[key] = newNode;
                addToHead(newNode);
            }
        }
    };
};
using LRUCache = Solution32::LRUCache;

// Interview Explanation:
// - Problem Statement: Design Least Recently Used (LRU) Cache supporting get and put in O(1) time.
// - Approach: Hash map paired with doubly-linked list. Sentinel `head` (most recent) and `tail` (least recent).
// - Intuition: Hash map provides O(1) key-to-node lookup; doubly-linked list provides O(1) node relocation and eviction.
// - Complexity: Time: O(1) for get and put, Space: O(capacity).


// =========================================================
// 33. LFU CACHE (LC 460)
// =========================================================

class Solution33 {
public:
    class LFUCache {
        struct Node {
            int key;
            int val;
            int freq;
        };

        int cap;
        int minFreq;
        unordered_map<int, list<Node>::iterator> keyMap;
        unordered_map<int, list<Node>> freqMap;

        void updateFreq(list<Node>::iterator it) {
            int key = it->key;
            int val = it->val;
            int freq = it->freq;

            freqMap[freq].erase(it);
            if (freqMap[freq].empty()) {
                freqMap.erase(freq);
                if (minFreq == freq) {
                    minFreq++;
                }
            }

            freqMap[freq + 1].push_front({key, val, freq + 1});
            keyMap[key] = freqMap[freq + 1].begin();
        }

    public:
        LFUCache(int capacity) : cap(capacity), minFreq(0) {}

        int get(int key) {
            if (cap == 0 || !keyMap.count(key)) return -1;
            auto it = keyMap[key];
            int val = it->val;
            updateFreq(it);
            return val;
        }

        void put(int key, int value) {
            if (cap == 0) return;

            if (keyMap.count(key)) {
                auto it = keyMap[key];
                it->val = value;
                updateFreq(it);
                return;
            }

            if ((int)keyMap.size() >= cap) {
                auto& minList = freqMap[minFreq];
                int evictKey = minList.back().key;
                minList.pop_back();
                if (minList.empty()) {
                    freqMap.erase(minFreq);
                }
                keyMap.erase(evictKey);
            }

            minFreq = 1;
            freqMap[1].push_front({key, value, 1});
            keyMap[key] = freqMap[1].begin();
        }
    };
};
using LFUCache = Solution33::LFUCache;

// Interview Explanation:
// - Problem Statement: Design Least Frequently Used (LFU) Cache supporting get and put in O(1) time.
// - Approach: Hash map of iterators + frequency-to-doubly-linked-list buckets (`freqMap`). Maintain `minFreq`.
// - Intuition: Access moves node from frequency bucket $f$ to $f+1$. Eviction removes from back of `minFreq` bucket.
// - Complexity: Time: O(1) for get and put, Space: O(capacity).


// =========================================================
// 34. FIND THE DUPLICATE NUMBER (LC 287)
// =========================================================

class Solution34 {
public:
    int findDuplicate(vector<int>& nums) {
        int slow = nums[0];
        int fast = nums[0];

        // Phase 1: Cycle detection
        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        } while (slow != fast);

        // Phase 2: Find cycle entrance
        slow = nums[0];
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }

        return slow;
    }
};
// Interview Explanation:
// - Problem Statement: Given array with $n + 1$ integers in range $[1, n]$, find duplicate without modifying array and in $O(1)$ extra space.
// - Approach: Interpret array as linked list where index $i$ points to $nums[i]$. Use Floyd's cycle detection.
// - Intuition: Duplicate number causes multiple indices to point to same target, forming the cycle entrance node.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 35. DESIGN BROWSER HISTORY (LC 1472)
// =========================================================

class Solution35 {
public:
    class BrowserHistory {
        struct HistoryNode {
            string url;
            HistoryNode* prev;
            HistoryNode* next;
            HistoryNode(string u) : url(u), prev(nullptr), next(nullptr) {}
        };
        HistoryNode* curr;

    public:
        BrowserHistory(string homepage) {
            curr = new HistoryNode(homepage);
        }

        void visit(string url) {
            HistoryNode* node = new HistoryNode(url);
            curr->next = node;
            node->prev = curr;
            curr = node; // Naturally discards forward history
        }

        string back(int steps) {
            while (curr->prev && steps > 0) {
                curr = curr->prev;
                steps--;
            }
            return curr->url;
        }

        string forward(int steps) {
            while (curr->next && steps > 0) {
                curr = curr->next;
                steps--;
            }
            return curr->url;
        }
    };
};
using BrowserHistory = Solution35::BrowserHistory;

// Interview Explanation:
// - Problem Statement: Design browser history starting at homepage, with visit(url), back(steps), and forward(steps).
// - Approach: Doubly-linked list. Current page pointer navigates backward/forward; visit appends new node and drops forward branch.
// - Intuition: Doubly-linked list provides natural bidirectional linear navigation and O(1) branch truncation.
// - Complexity: Time: visit O(1), back/forward O(min(steps, len)), Space: O(N) total history nodes.
