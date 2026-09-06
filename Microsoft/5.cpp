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
                   LINKED LIST — MICROSOFT OA HIGH-YIELD PROBLEMS
 ====================================================================================================
 | #  | Problem Name                                     | Pattern / Technique                | Time      | Space  |
 |----|---------------------------------------------------|-------------------------------------|-----------|--------|
 | 01 | Reverse Linked List II (LC 92)                    | Sublist Pointer Inversion           | O(N)      | O(1)   |
 | 02 | Reverse Nodes in k-Group (LC 25)                  | Chunked Sublist Reversal            | O(N)      | O(1)   |
 | 03 | Merge k Sorted Lists (LC 23)                      | Min-Heap Priority Queue Merge       | O(N logK) | O(K)   |
 | 04 | Sort List (LC 148)                                | Merge Sort (Slow/Fast Split)        | O(N logN) | O(logN)|
 | 05 | Linked List Cycle (LC 141)                        | Floyd's Tortoise & Hare             | O(N)      | O(1)   |
 | 06 | Linked List Cycle II (Find Cycle Start) (LC 142)  | Floyd's Cycle + Reset Pointer       | O(N)      | O(1)   |
 | 07 | Find the Duplicate Number (LC 287)                | Array-as-Linked-List Floyd Cycle    | O(N)      | O(1)   |
 | 08 | Remove Nth Node From End of List (LC 19)          | Two-Pointer Gap Technique           | O(N)      | O(1)   |
 | 09 | Reorder List (LC 143)                             | Mid Find + Reverse + Interleave     | O(N)      | O(1)   |
 | 10 | Palindrome Linked List (LC 234)                   | Mid Find + Reverse + Compare        | O(N)      | O(1)   |
 | 11 | Odd Even Linked List (LC 328)                     | In-place Bucket Splitting           | O(N)      | O(1)   |
 | 12 | Rotate List (LC 61)                               | Ring Formation & Modular Cut        | O(N)      | O(1)   |
 | 13 | Partition List (LC 86)                            | Two Dummy Heads (Less/GreaterEq)    | O(N)      | O(1)   |
 | 14 | Remove Duplicates from Sorted List II (LC 82)     | Dummy Node + Skip Entire Runs       | O(N)      | O(1)   |
 | 15 | Add Two Numbers (LC 2)                            | Dummy Node + Carry Propagation      | O(max(N,M))| O(max(N,M))|
 | 16 | Add Two Numbers II (LC 445)                       | Stack Reversal + Carry Propagation  | O(N+M)    | O(N+M) |
 | 17 | Intersection of Two Linked Lists (LC 160)         | Two-Pointer Switch-Path Alignment   | O(N+M)    | O(1)   |
 | 18 | Copy List with Random Pointer (LC 138)             | Interleaving Clone Nodes (O(1))     | O(N)      | O(1)   |
 | 19 | Split Linked List in Parts (LC 725)                | Size Calc + Balanced Chunking       | O(N+K)    | O(K)   |
 | 20 | Next Greater Node In Linked List (LC 1019)         | Stack (Monotonic) on Array Convert  | O(N)      | O(N)   |
 | 21 | Merge In Between Linked Lists (LC 1669)            | Pointer Splice at Boundaries        | O(N+M)    | O(1)   |
 ====================================================================================================
*/

// =========================================================
// 1. REVERSE LINKED LIST II (LC 92)
// =========================================================

class Solution1 {
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
// 2. REVERSE NODES IN K-GROUP (LC 25)
// =========================================================

class Solution2 {
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
// - Approach: Find the $k$-th node of the current group. If fewer than $k$ nodes remain, terminate. Otherwise reverse group in-place and reconnect boundaries.
// - Intuition: Local pointer reversal per $k$-window anchored by `groupPrev` and `groupNext`.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 3. MERGE K SORTED LISTS (LC 23)
// =========================================================

class Solution3 {
    struct Compare {
        bool operator()(const ListNode* a, const ListNode* b) const {
            return a->val > b->val;
        }
    };

public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        priority_queue<ListNode*, vector<ListNode*>, Compare> pq;
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
// 4. SORT LIST (LC 148)
// =========================================================

class Solution4 {
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
// 5. LINKED LIST CYCLE (LC 141)
// =========================================================

class Solution5 {
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
// 6. LINKED LIST CYCLE II (FIND CYCLE START) (LC 142)
// =========================================================

class Solution6 {
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
// 7. FIND THE DUPLICATE NUMBER (LC 287)
// =========================================================

class Solution7 {
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
// 8. REMOVE NTH NODE FROM END OF LIST (LC 19)
// =========================================================

class Solution8 {
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
// 9. REORDER LIST (LC 143)
// =========================================================

class Solution9 {
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
// 10. PALINDROME LINKED LIST (LC 234)
// =========================================================

class Solution10 {
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
// 11. ODD EVEN LINKED LIST (LC 328)
// =========================================================

class Solution11 {
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
// 12. ROTATE LIST (LC 61)
// =========================================================

class Solution12 {
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
// 13. PARTITION LIST (LC 86)
// =========================================================

class Solution13 {
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
// - Approach: Two separate dummy heads (`less` and `greater`). Append each node to appropriate chain, then concatenate.
// - Intuition: Stable two-way partitioning using two accumulator linked lists.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 14. REMOVE DUPLICATES FROM SORTED LIST II (LC 82)
// =========================================================

class Solution14 {
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
// 15. ADD TWO NUMBERS (LC 2)
// =========================================================

class Solution15 {
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
// 16. ADD TWO NUMBERS II (LC 445)
// =========================================================

class Solution16 {
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
// 17. INTERSECTION OF TWO LINKED LISTS (LC 160)
// =========================================================

class Solution17 {
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
// 18. COPY LIST WITH RANDOM POINTER (LC 138)
// =========================================================

class Solution18 {
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
// 19. SPLIT LINKED LIST IN PARTS (LC 725)
// =========================================================

class Solution19 {
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
// 20. NEXT GREATER NODE IN LINKED LIST (LC 1019)
// =========================================================

class Solution20 {
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
// 21. MERGE IN BETWEEN LINKED LISTS (LC 1669)
// =========================================================

class Solution21 {
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
