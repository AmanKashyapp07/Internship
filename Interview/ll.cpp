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
#endif
using namespace std;

// ====================================================================================================
//                                    STANDARD DATA STRUCTURE DEFINITIONS
// ====================================================================================================

// Standard Singly Linked List Node
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Node with Random Pointer (LC 138)
class RandomNode {
public:
    int val;
    RandomNode* next;
    RandomNode* random;
    RandomNode(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

// Multilevel Doubly Linked List Node (LC 430)
class MultilevelNode {
public:
    int val;
    MultilevelNode* prev;
    MultilevelNode* next;
    MultilevelNode* child;
    MultilevelNode() : val(0), prev(nullptr), next(nullptr), child(nullptr) {}
    MultilevelNode(int _val) : val(_val), prev(nullptr), next(nullptr), child(nullptr) {}
};

// Binary Tree Node (for LC 426 BST to Doubly Linked List)
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | LC #   | Core Pattern / Technique           | Time     | Space    |
 |----|---------------------------------------------|--------|------------------------------------|----------|----------|
 | 01 | Linked List Cycle II                        | LC 142 | Floyd's Slow/Fast Pointer Cycle Meet| O(N)    | O(1)     |
 | 02 | Middle of the Linked List                   | LC 876 | Slow/Fast 1-Step vs 2-Step Gap     | O(N)     | O(1)     |
 | 03 | Remove Nth Node From End of List            | LC 19  | Dummy Head + N-Step Two Pointers   | O(N)     | O(1)     |
 | 04 | Intersection of Two Linked Lists            | LC 160 | Two-Pointer Traversal Switch (a+b) | O(M + N) | O(1)     |
 | 05 | Reverse Linked List (Iterative & Recursive) | LC 206 | 3-Pointer Flipping / Postorder DFS  | O(N)     | O(1)/O(N)|
 | 06 | Reverse Nodes in k-Group                    | LC 25  | Chunked Sublist Pointer Reversal   | O(N)     | O(1)     |
 | 07 | Palindrome Linked List                      | LC 234 | Mid-Split + In-Place Half Reversal  | O(N)     | O(1)     |
 | 08 | Reorder List                                | LC 143 | Mid-Split + Reverse + Interleave   | O(N)     | O(1)     |
 | 09 | Swap Nodes in Pairs                         | LC 24  | Dummy Node + Pairwise Rewiring     | O(N)     | O(1)     |
 | 10 | Merge Two Sorted Lists                      | LC 21  | Dummy Head + Single-Pass Splice    | O(M + N) | O(1)     |
 | 11 | Merge k Sorted Lists                        | LC 23  | Min-Heap Priority Queue & D&C Merge| O(N logK)| O(K)/O(1)|
 | 12 | Sort List (Merge Sort on Linked List)       | LC 148 | Fast-Slow Mid Split + Merge        | O(N logN)| O(log N) |
 | 13 | Remove Duplicates (Sorted & Delete All)     | LC 83/82| Dummy + Lookahead Streak Skipping  | O(N)     | O(1)     |
 | 14 | Add Two Numbers                             | LC 2   | Dummy Head + Running Carry Accum    | O(max)   | O(1)     |
 | 15 | Add Two Numbers II (Forward Order)          | LC 445 | Dual Explicit Stacks / Reversals   | O(M + N) | O(M + N) |
 | 16 | Copy List with Random Pointer               | LC 138 | Interleave-Clone-Detach (O(1) space)| O(N)    | O(1)     |
 | 17 | Flatten a Multilevel Doubly Linked List     | LC 430 | Iterative Child Splice with Tail   | O(N)     | O(1)     |
 | 18 | Convert BST to Sorted Doubly Linked List    | LC 426 | Inorder Rewiring + Circular Close  | O(N)     | O(H)     |
 | 19 | LRU Cache                                   | LC 146 | Hash Map + Doubly Linked List      | O(1) all | O(Cap)   |
 | 20 | LFU Cache                                   | LC 460 | Hash Map + Frequency Bucket DLLs   | O(1) all | O(Cap)   |
 ====================================================================================================
*/


// ====================================================================================================
// GROUP 1: POINTER TRAVERSAL & FAST/SLOW POINTERS
// ====================================================================================================

// =========================================================
// 1. LINKED LIST CYCLE II (LC 142)
// =========================================================

class Solution1 {
public:
    ListNode *detectCycle(ListNode *head) {
        if (!head || !head->next) return nullptr;
        
        ListNode *slow = head;
        ListNode *fast = head;
        
        // Phase 1: Determine if a cycle exists
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                // Phase 2: Find cycle entry point
                // Math: Distance from head to cycle entry equals distance from meeting point to cycle entry
                // Let L = head-to-entry, C = cycle length, X = entry-to-meeting.
                // 2 * dist(slow) = dist(fast) => 2(L + X) = L + nC + X => L = nC - X.
                ListNode *entry = head;
                while (entry != slow) {
                    entry = entry->next;
                    slow = slow->next;
                }
                return entry; // Both meet at the cycle entry node
            }
        }
        return nullptr; // No cycle
    }
};
// Interview Explanation:
// - Problem Statement: Given head of linked list, return node where cycle begins, or nullptr if no cycle (LC 142).
// - Core Pattern: Floyd's Tortoise and Hare Cycle Detection.
// - Intuition:
//   * Phase 1: Fast moves 2 steps, slow moves 1. If they meet, a cycle exists.
//   * Phase 2: Reset one pointer to head. Move both pointers 1 step at a time. They meet precisely at the entry.
// - Complexity:
//   * Time: O(N) — Slow pointer travels less than 2 cycles before meeting.
//   * Space: O(1) auxiliary space (no hash set of visited nodes).
// - Edge Cases: Empty list, single node pointing to itself (returns head), single node pointing to null (returns null), cycle at head.
// - Likely Follow-ups:
//   1. How to prove mathematical correctness? -> State 2(L + X) = L + nC + X => L = nC - X.
//   2. How to find the length of the cycle? -> Keep fast stationary at meeting point; advance slow until it returns (count steps).


// =========================================================
// 2. MIDDLE OF THE LINKED LIST (LC 876)
// =========================================================

class Solution2 {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        
        // Fast moves 2 steps while slow moves 1 step
        // When fast reaches the end, slow is at the middle (2nd middle for even count)
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    // Follow-up: If 1st middle is requested for even-length lists (e.g. for split/mergesort)
    ListNode* firstMiddleNode(ListNode* head) {
        if (!head) return nullptr;
        ListNode* slow = head;
        ListNode* fast = head->next; // Offset fast by 1
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow; // Returns 1st middle (e.g., node 2 in [1, 2, 3, 4])
    }
};
// Interview Explanation:
// - Problem Statement: Return middle node of linked list; if two middle nodes, return the second (LC 876).
// - Core Pattern: Fast and Slow Two-Pointer technique.
// - Intuition:
//   * Since fast travels at 2x the speed of slow, when fast reaches the end, slow has covered exactly half the distance.
// - Complexity:
//   * Time: O(N) — Single pass (fast reaches end in N/2 iterations).
//   * Space: O(1) auxiliary space.
// - Edge Cases: Single node, 2 nodes (returns 2nd node), odd vs even length lists.
// - Likely Follow-ups:
//   1. How to get the 1st middle node instead? -> Initialize `fast = head->next` or condition on `while (fast->next && fast->next->next)`.
//   2. Why is 1st middle preferred in merge sort? -> Allows severing `slow->next = nullptr` without infinite recursion.


// =========================================================
// 3. REMOVE NTH NODE FROM END OF LIST (LC 19)
// =========================================================

class Solution3 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // Dummy node handles edge case where the head itself is removed
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;
        
        // Advance fast pointer by n + 1 steps to establish gap of size n between fast and slow
        for (int i = 0; i <= n; ++i) {
            fast = fast->next;
        }
        
        // Move both until fast reaches null (past the end of list)
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        
        // slow is now immediately BEFORE the target node to delete
        ListNode* nodeToDelete = slow->next;
        slow->next = slow->next->next;
        delete nodeToDelete; // Free memory in C++
        
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Remove nth node from end of list in one pass and return head (LC 19).
// - Core Pattern: Two-Pointer Fixed Gap with Dummy Node.
// - Intuition:
//   * Move fast n + 1 steps ahead. Then move fast and slow together.
//   * When fast hits nullptr, slow sits immediately before the target node.
// - Complexity:
//   * Time: O(N) — Strictly one pass.
//   * Space: O(1) auxiliary space.
// - Edge Cases: Removing the head node (n == length of list), single node list, removing the tail (n == 1).
// - Likely Follow-ups:
//   1. Why use dummy node? -> Eliminates special conditional branch when deleting `head`.
//   2. How to handle memory in C++? -> Save pointer `slow->next` before relinking and call `delete` to prevent memory leaks.


// =========================================================
// 4. INTERSECTION OF TWO LINKED LISTS (LC 160)
// =========================================================

class Solution4 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) return nullptr;
        
        ListNode* ptrA = headA;
        ListNode* ptrB = headB;
        
        // When ptrA reaches end, redirect to headB.
        // When ptrB reaches end, redirect to headA.
        // Both travel total distance of lenA + lenB and meet at intersection (or nullptr).
        while (ptrA != ptrB) {
            ptrA = (ptrA == nullptr) ? headB : ptrA->next;
            ptrB = (ptrB == nullptr) ? headA : ptrB->next;
        }
        return ptrA; // Either intersection node or nullptr
    }
};
// Interview Explanation:
// - Problem Statement: Find node at which two singly linked lists intersect (LC 160).
// - Core Pattern: Two-Pointer Traversal Switch (Floyd-like distance equalization).
// - Intuition:
//   * List A has length a + c, List B has length b + c (where c is shared segment).
//   * Pointer A travels a + c then b. Pointer B travels b + c then a.
//   * Both travel exactly a + b + c steps; they synchronize and collide at the intersection node.
// - Complexity:
//   * Time: O(M + N) — At most 2 passes over both lists.
//   * Space: O(1) auxiliary space (avoids O(M) hash set).
// - Edge Cases: Disconnected lists (both reach nullptr at the same step), lists intersecting at head, one list much longer.
// - Likely Follow-ups:
//   1. Can we calculate lengths explicitly? -> Yes: find lenA, lenB; advance longer pointer by |lenA - lenB|, then step together.
//   2. What if lists contain cycles? -> Reduces to finding cycle entry (LC 142); check if intersection occurs before or inside cycle.


// ====================================================================================================
// GROUP 2: IN-PLACE REVERSALS & REORDERING
// ====================================================================================================

// =========================================================
// 5. REVERSE LINKED LIST (LC 206)
// =========================================================

class Solution5 {
public:
    // Iterative: 3 Rolling Pointers (Preferred for production / interviews)
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        
        while (curr) {
            ListNode* nextTemp = curr->next; // Save next node before overwriting pointer
            curr->next = prev;                // Reverse pointer
            prev = curr;                       // Advance prev
            curr = nextTemp;                   // Advance curr
        }
        return prev; // prev is the new head
    }

    // Recursive DFS Alternative
    ListNode* reverseListRecursive(ListNode* head) {
        // Base case: empty list or single node
        if (!head || !head->next) return head;
        
        ListNode* newHead = reverseListRecursive(head->next);
        head->next->next = head; // Make next node point back to current node
        head->next = nullptr;    // Sever forward pointer to prevent cycle
        
        return newHead;
    }
};
// Interview Explanation:
// - Problem Statement: Reverse a singly linked list iteratively and recursively (LC 206).
// - Core Pattern: 3-Pointer Iterative Rewiring / Postorder DFS.
// - Intuition:
//   * Iterative: Maintain `prev`, `curr`, `nextTemp`. Flip `curr->next = prev`, then shift forward.
//   * Recursive: Recurse to end; as call stack unwinds, wire `head->next->next = head`.
// - Complexity:
//   * Iterative: Time: O(N), Space: O(1) auxiliary space.
//   * Recursive: Time: O(N), Space: O(N) recursion stack.
// - Edge Cases: Empty list (nullptr), single node (returns head unchanged), 2-node list.
// - Likely Follow-ups:
//   1. Why is recursive dangerous for long lists? -> Causes stack overflow on N > 10^5 (skewed chain).
//   2. Reverse between positions left and right (LC 92)? -> Walk to `left - 1`, reverse subsegment, reconnect boundaries.


// =========================================================
// 6. REVERSE NODES IN K-GROUP (LC 25)
// =========================================================

class Solution6 {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head || k <= 1) return head;
        
        ListNode dummy(0, head);
        ListNode* groupPrev = &dummy;
        
        while (true) {
            // Step 1: Check if at least k nodes remain
            ListNode* kth = groupPrev;
            for (int i = 0; i < k && kth; ++i) {
                kth = kth->next;
            }
            if (!kth) break; // Fewer than k nodes remain: keep original order
            
            ListNode* groupNext = kth->next;
            ListNode* prev = groupNext;
            ListNode* curr = groupPrev->next;
            
            // Step 2: Reverse k nodes in-place
            while (curr != groupNext) {
                ListNode* temp = curr->next;
                curr->next = prev;
                prev = curr;
                curr = temp;
            }
            
            // Step 3: Connect previous group to new group head and update groupPrev
            ListNode* oldGroupHead = groupPrev->next;
            groupPrev->next = kth;
            groupPrev = oldGroupHead;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Reverse nodes of linked list k at a time and return modified list; leftover nodes remain as-is (LC 25).
// - Core Pattern: Chunked Sublist Pointer Reversal with Dummy Sentinel.
// - Intuition:
//   * First verify that k nodes exist ahead. If not, break out and preserve order.
//   * Reverse the k-sized window by initializing `prev = groupNext` (connecting tail directly).
//   * Reconnect `groupPrev->next = kth` and advance `groupPrev` to the tail of the newly reversed group.
// - Complexity:
//   * Time: O(N) — Every node visited twice (once to count k, once to reverse).
//   * Space: O(1) auxiliary space.
// - Edge Cases: k = 1 (no changes), k = length of list, length < k (unchanged), length not divisible by k (last group intact).
// - Likely Follow-ups:
//   1. What if remaining < k nodes must ALSO be reversed? -> Remove the lookahead check; reverse whatever remains.
//   2. Can we do this recursively? -> Yes, reverse first k nodes, then `head->next = reverseKGroup(nextGroupHead, k)`.


// =========================================================
// 7. PALINDROME LINKED LIST (LC 234)
// =========================================================

class Solution7 {
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev;
    }

public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        
        // 1. Find middle of list using fast/slow
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 2. Reverse second half
        ListNode* secondHalfStart = reverse(slow->next);
        
        // 3. Compare values of first half and second half
        ListNode* p1 = head;
        ListNode* p2 = secondHalfStart;
        bool palindrome = true;
        while (p2) {
            if (p1->val != p2->val) {
                palindrome = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        
        // 4. Restore list structure (Microsoft best-practice etiquette)
        slow->next = reverse(secondHalfStart);
        
        return palindrome;
    }
};
// Interview Explanation:
// - Problem Statement: Check whether a singly linked list is a palindrome in O(N) time and O(1) space (LC 234).
// - Core Pattern: Fast/Slow Midpoint + In-Place Second Half Reversal + In-Place Restoration.
// - Intuition:
//   * Find mid with fast/slow. Reverse second half. Compare element-by-element with first half.
//   * Always restore the reversed half before returning to avoid side-effects on caller data.
// - Complexity:
//   * Time: O(N) — N/2 steps to find mid, N/2 steps to reverse, N/2 steps to compare.
//   * Space: O(1) auxiliary space.
// - Edge Cases: Single node (true), 2 nodes matching vs differing, odd vs even length lists.
// - Likely Follow-ups:
//   1. Why restore the list? -> Concurrency and functional purity: callers assume read-only operations don't mutate lists.
//   2. How to solve without modifying list at all? -> Recursion with a front pointer moving forward ($O(N)$ stack space).


// =========================================================
// 8. REORDER LIST (LC 143)
// =========================================================

class Solution8 {
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev;
    }

public:
    void reorderList(ListNode* head) {
        if (!head || !head->next || !head->next->next) return;
        
        // 1. Find 1st middle node
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 2. Split list into two halves and reverse second half
        ListNode* secondHalf = reverse(slow->next);
        slow->next = nullptr; // Sever connection
        
        // 3. Interleave two lists: L0 -> Ln -> L1 -> Ln-1 ...
        ListNode* firstHalf = head;
        while (secondHalf) {
            ListNode* next1 = firstHalf->next;
            ListNode* next2 = secondHalf->next;
            
            firstHalf->next = secondHalf;
            secondHalf->next = next1;
            
            firstHalf = next1;
            secondHalf = next2;
        }
    }
};
// Interview Explanation:
// - Problem Statement: Reorder L0 -> L1 -> ... -> Ln to L0 -> Ln -> L1 -> Ln-1 ... in-place (LC 143).
// - Core Pattern: Mid-Split + Half Reversal + Two-Pointer Alternating Interleaving.
// - Intuition:
//   * The required order pairs node `i` from the front with node `n - i` from the back.
//   * Reversing the second half allows us to iterate both halves forward and weave them together.
// - Complexity:
//   * Time: O(N) — Midpoint, reversal, and merge each take O(N).
//   * Space: O(1) auxiliary space.
// - Edge Cases: List of length <= 2 (no reordering needed), odd vs even length lists.
// - Likely Follow-ups:
//   1. What happens if slow->next is not set to nullptr? -> Causes an infinite circular loop during traversal.
//   2. Can we use a deque? -> Yes, push nodes to deque and pop from front/back ($O(N)$ extra space).


// =========================================================
// 9. SWAP NODES IN PAIRS (LC 24)
// =========================================================

class Solution9 {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;
        
        // While at least 2 nodes remain ahead to swap
        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;
            ListNode* second = prev->next->next;
            
            // In-place pointer rewiring
            first->next = second->next; // 1 -> 3
            second->next = first;       // 2 -> 1
            prev->next = second;        // prev -> 2
            
            // Advance prev to end of swapped pair (first is now second in pair)
            prev = first;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Swap every two adjacent nodes and return head without modifying node values (LC 24).
// - Core Pattern: Dummy Head + Pairwise In-Place Rewiring.
// - Intuition:
//   * Rewire 3 pointers: `first->next = second->next`, `second->next = first`, `prev->next = second`.
//   * Dummy node cleanly updates the list head when the first pair swaps.
// - Complexity:
//   * Time: O(N) — Each node visited once.
//   * Space: O(1) auxiliary space.
// - Edge Cases: Empty list, single node (unchanged), odd length lists (last node unswapped).
// - Likely Follow-ups:
//   1. Can we just swap node values? -> Forbidden in interview specs; nodes may be large structs or immutable.
//   2. How is this related to LC 25 (k-group)? -> Identical problem with k = 2.


// ====================================================================================================
// GROUP 3: MERGING & SORTING
// ====================================================================================================

// =========================================================
// 10. MERGE TWO SORTED LISTS (LC 21)
// =========================================================

class Solution10 {
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
        
        // Splice remainder in O(1) (advantage over arrays!)
        tail->next = list1 ? list1 : list2;
        
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Merge two sorted singly linked lists into one sorted list (LC 21).
// - Core Pattern: Dummy Head + Single-Pass Two-Pointer Splice.
// - Intuition:
//   * Advance tail by picking smaller of list1 and list2.
//   * When one list exhausts, attach the remainder of the other list in O(1) pointer assignment.
// - Complexity:
//   * Time: O(M + N) — Each node inspected at most once.
//   * Space: O(1) auxiliary space.
// - Edge Cases: One or both lists empty, duplicate values, lists of drastically different lengths.
// - Likely Follow-ups:
//   1. Can you do this recursively? -> `if (!l1) return l2; if (l1->val < l2->val) { l1->next = merge(l1->next, l2); return l1; }` ($O(M+N)$ stack).
//   2. How does linked list merge differ from array merge? -> Linked list attaches remaining suffix in $O(1)$ without copying elements.


// =========================================================
// 11. MERGE K SORTED LISTS (LC 23)
// =========================================================

class Solution11 {
    // Helper: Merge two sorted lists in O(1) space
    ListNode* mergeTwo(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                list2_next:
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }

public:
    // Approach 1: Min-Heap / Priority Queue (Best for streaming data)
    ListNode* mergeKListsHeap(vector<ListNode*>& lists) {
        auto comp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(comp)> minHeap(comp);
        
        // Push head of each non-empty list into min-heap
        for (ListNode* head : lists) {
            if (head) minHeap.push(head);
        }
        
        ListNode dummy(0);
        ListNode* tail = &dummy;
        
        while (!minHeap.empty()) {
            ListNode* smallest = minHeap.top();
            minHeap.pop();
            
            tail->next = smallest;
            tail = tail->next;
            
            if (smallest->next) {
                minHeap.push(smallest->next);
            }
        }
        return dummy.next;
    }

    // Approach 2: Divide and Conquer Pairwise Merging (O(1) Auxiliary Heap Space)
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        int interval = 1;
        int n = lists.size();
        
        // Pairwise merge like bottom-up merge sort
        while (interval < n) {
            for (int i = 0; i + interval < n; i += interval * 2) {
                lists[i] = mergeTwo(lists[i], lists[i + interval]);
            }
            interval *= 2;
        }
        return lists[0];
    }
};
// Interview Explanation:
// - Problem Statement: Merge k sorted linked lists and return as one sorted list (LC 23).
// - Core Pattern: Min-Heap Priority Queue OR Divide-and-Conquer Pairwise Merge.
// - Intuition:
//   * Min-Heap: Maintain k heads in min-heap. Pop minimum, append to tail, push `smallest->next`.
//   * Divide-and-Conquer: Merge lists in pairs (0 with 1, 2 with 3). Reduces k lists to k/2, then k/4... in log k rounds.
// - Complexity:
//   * Time: O(N log K) where N is total node count across all k lists.
//   * Space: Heap uses O(K) memory. Divide-and-Conquer uses O(1) auxiliary memory.
// - Edge Cases: lists is empty ([]), lists contains only nulls ([[], []]), k = 1.
// - Likely Follow-ups:
//   1. Which approach is preferred in distributed systems? -> Min-heap supports streaming inputs from k external network sockets.
//   2. Why is naive sequential merging slow? -> Sequential takes O(k * N) = O(k^2 * averageLen); D&C does it in O(N log k).


// =========================================================
// 12. SORT LIST (LC 148)
// =========================================================

class Solution12 {
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
    // Top-Down Merge Sort: O(N log N) Time, O(log N) Stack Space
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;
        
        // 1. Split list into two halves using slow/fast pointers
        ListNode* prev = nullptr;
        ListNode* slow = head;
        ListNode* fast = head;
        
        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        
        prev->next = nullptr; // Sever connection to form two distinct lists
        
        // 2. Recursively sort each half
        ListNode* l1 = sortList(head);
        ListNode* l2 = sortList(slow);
        
        // 3. Merge sorted halves
        return merge(l1, l2);
    }
};
// Interview Explanation:
// - Problem Statement: Sort linked list in O(N log N) time and O(1) or O(log N) space (LC 148).
// - Core Pattern: Merge Sort on Linked Lists (Fast/Slow split + Two-way merge).
// - Intuition:
//   * Unlike arrays, linked lists can be merged in O(1) auxiliary space without buffer arrays.
//   * Find mid with fast/slow, sever `prev->next = nullptr`, recurse on both halves, then merge.
// - Complexity:
//   * Time: O(N log N) — Recurrence T(N) = 2T(N/2) + O(N).
//   * Space: O(log N) call stack. (Bottom-up iterative merge sort achieves strictly O(1) space).
// - Edge Cases: Empty list, single node, already sorted, descending sorted, all duplicates.
// - Likely Follow-ups:
//   1. Why is QuickSort rarely used on singly linked lists? -> Worst case O(N^2) and lack of random access makes pivot partitioning inefficient.
//   2. How to achieve strictly O(1) space? -> Bottom-up iterative merge sort with step sizes 1, 2, 4, 8...


// =========================================================
// 13. REMOVE DUPLICATES FROM SORTED LIST (LC 83 & LC 82)
// =========================================================

class Solution13 {
public:
    // --- LC 83: Remove Duplicates (Keep Exactly One Occurrence) ---
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* curr = head;
        while (curr && curr->next) {
            if (curr->val == curr->next->val) {
                ListNode* duplicate = curr->next;
                curr->next = curr->next->next; // Skip duplicate node
                delete duplicate;               // Free memory
            } else {
                curr = curr->next;
            }
        }
        return head;
    }

    // --- LC 82: Remove Duplicates II (Delete ALL Nodes with Duplicate Values) ---
    ListNode* deleteDuplicatesAll(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy; // Sits immediately before sublist of duplicates
        ListNode* curr = head;
        
        while (curr) {
            // Check if curr starts a run of duplicates
            if (curr->next && curr->val == curr->next->val) {
                int dupVal = curr->val;
                // Skip ALL nodes with value dupVal
                while (curr && curr->val == dupVal) {
                    ListNode* temp = curr;
                    curr = curr->next;
                    delete temp; // Clean memory
                }
                prev->next = curr; // Link around entire duplicate run
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: LC 83 keeps one instance of duplicates; LC 82 deletes ALL nodes having duplicate numbers.
// - Core Pattern: Lookahead Run Detection with Dummy Sentinel.
// - Intuition:
//   * LC 83: If `curr->val == curr->next->val`, cut out `curr->next`. Advance `curr` only when unequal.
//   * LC 82: When a duplicate run is detected (`curr->val == curr->next->val`), consume all matching nodes in a loop, then link `prev->next = curr`.
// - Complexity:
//   * Time: O(N) — Every node visited once.
//   * Space: O(1) auxiliary space.
// - Edge Cases: All nodes are duplicates ([1, 1, 1] -> LC 82 returns nullptr), duplicates at head, no duplicates.
// - Likely Follow-ups:
//   1. What if list is UNSORTED? -> Use an `unordered_set<int>` in O(N) time and O(N) space, or sort first in O(N log N).
//   2. Always remember to `delete` discarded nodes in C++!


// ====================================================================================================
// GROUP 4: ARITHMETIC & NUMBER REPRESENTATION
// ====================================================================================================

// =========================================================
// 14. ADD TWO NUMBERS (LC 2)
// =========================================================

class Solution14 {
public:
    // Digits stored in REVERSE order: 2 -> 4 -> 3 represents 342
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
// - Problem Statement: Add two numbers represented as linked lists in reverse order (LC 2).
// - Core Pattern: Elementary School Addition with Dummy Head and Running Carry.
// - Intuition:
//   * Since digits are reverse-ordered, the heads represent the least significant digits.
//   * Loop runs while `l1 || l2 || carry` to handle carry overflow at the most significant digit (e.g. 99 + 1 = 100).
// - Complexity:
//   * Time: O(max(M, N)) — Traverses both lists simultaneously.
//   * Space: O(max(M, N)) for the newly created result list (O(1) auxiliary space).
// - Edge Cases: Unequal lengths, final carry generates a new MSB node (5 + 5 = 0 -> 1), zeroes (0 + 0 = 0).
// - Likely Follow-ups:
//   1. Can we modify l1 in-place to avoid allocations? -> Yes: overwrite `l1->val` and append carry node if needed.
//   2. What if digits are stored in FORWARD order? -> LC 445 (Add Two Numbers II).


// =========================================================
// 15. ADD TWO NUMBERS II (LC 445)
// =========================================================

class Solution15 {
public:
    // Digits stored in FORWARD order (MSB first) WITHOUT reversing input lists
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        stack<int> s1, s2;
        
        while (l1) { s1.push(l1->val); l1 = l1->next; }
        while (l2) { s2.push(l2->val); l2 = l2->next; }
        
        ListNode* head = nullptr; // Build result list from right to left (prepending)
        int carry = 0;
        
        while (!s1.empty() || !s2.empty() || carry) {
            int sum = carry;
            if (!s1.empty()) { sum += s1.top(); s1.pop(); }
            if (!s2.empty()) { sum += s2.top(); s2.pop(); }
            
            carry = sum / 10;
            
            // Prepend new node to head of result list
            ListNode* newNode = new ListNode(sum % 10);
            newNode->next = head;
            head = newNode;
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Add two numbers where most significant digit is at head; cannot reverse inputs (LC 445).
// - Core Pattern: Dual Stacks for LIFO Access + Front Prepending.
// - Intuition:
//   * Stacks naturally reverse the order of digits, allowing LSB addition without mutating the input lists.
//   * Prepending `newNode->next = head; head = newNode;` builds the forward-order list seamlessly.
// - Complexity:
//   * Time: O(M + N) — Push all elements to stacks, then pop and add.
//   * Space: O(M + N) stack memory.
// - Edge Cases: Different number lengths (e.g. 7243 + 564), final carry creating extra leading 1, zeros.
// - Likely Follow-ups:
//   1. What if input lists CAN be modified? -> Reverse both lists (LC 206), run LC 2, reverse result list in O(1) extra space.
//   2. How to do without stacks or reversal? -> Pad shorter list with zeros, use recursion stack to add from right to left.


// ====================================================================================================
// GROUP 5: DEEP COPY & MULTI-LEVEL LISTS
// ====================================================================================================

// =========================================================
// 16. COPY LIST WITH RANDOM POINTER (LC 138)
// =========================================================

class Solution16 {
public:
    // Approach 1: O(1) Auxiliary Space (Interleave-Clone-Detach — Microsoft Favorite)
    RandomNode* copyRandomList(RandomNode* head) {
        if (!head) return nullptr;
        
        // Pass 1: Interleave cloned nodes directly after original nodes
        // A -> A' -> B -> B' -> C -> C'
        RandomNode* curr = head;
        while (curr) {
            RandomNode* clone = new RandomNode(curr->val);
            clone->next = curr->next;
            curr->next = clone;
            curr = clone->next;
        }
        
        // Pass 2: Set random pointers for cloned nodes
        // Since clone sits right after original, clone->random = original->random->next
        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }
        
        // Pass 3: Detach original and cloned lists
        curr = head;
        RandomNode* cloneHead = head->next;
        RandomNode* cloneCurr = cloneHead;
        
        while (curr) {
            curr->next = curr->next->next;
            cloneCurr->next = cloneCurr->next ? cloneCurr->next->next : nullptr;
            
            curr = curr->next;
            cloneCurr = cloneCurr->next;
        }
        return cloneHead;
    }

    // Approach 2: Hash Map Mapping (O(N) Extra Space)
    RandomNode* copyRandomListMap(RandomNode* head) {
        if (!head) return nullptr;
        unordered_map<RandomNode*, RandomNode*> cloneMap;
        
        // Pass 1: Create all clone nodes
        RandomNode* curr = head;
        while (curr) {
            cloneMap[curr] = new RandomNode(curr->val);
            curr = curr->next;
        }
        
        // Pass 2: Wire next and random pointers
        curr = head;
        while (curr) {
            cloneMap[curr]->next = cloneMap[curr->next];
            cloneMap[curr]->random = cloneMap[curr->random];
            curr = curr->next;
        }
        return cloneMap[head];
    }
};
// Interview Explanation:
// - Problem Statement: Deep copy a linked list where each node contains `val`, `next`, and `random` pointer (LC 138).
// - Core Pattern: Interleave-Clone-Detach (eliminates hash map).
// - Intuition:
//   * Inserting `A'` after `A` associates each original node with its clone in O(1) space.
//   * `A'->random` is simply `A->random->next`.
//   * Detaching splits the interleaved list back into two original separate lists.
// - Complexity:
//   * Time: O(N) — 3 linear passes.
//   * Space: O(1) auxiliary space (cloned nodes are the required output).
// - Edge Cases: Empty list, random pointers pointing to nullptr, random pointer pointing to self (`A->random = A`).
// - Likely Follow-ups:
//   1. Why must the original list be restored? -> Mutation of caller input is a critical bug in production systems.
//   2. What is the Hash Map trade-off? -> O(N) extra memory, but doesn't modify the input list during computation.


// =========================================================
// 17. FLATTEN A MULTILEVEL DOUBLY LINKED LIST (LC 430)
// =========================================================

class Solution17 {
public:
    MultilevelNode* flatten(MultilevelNode* head) {
        MultilevelNode* curr = head;
        
        while (curr) {
            if (curr->child) {
                // 1. Find tail of the child sublist
                MultilevelNode* childTail = curr->child;
                while (childTail->next) {
                    childTail = childTail->next;
                }
                
                // 2. Wire childTail to curr->next
                childTail->next = curr->next;
                if (curr->next) {
                    curr->next->prev = childTail;
                }
                
                // 3. Wire curr to curr->child
                curr->next = curr->child;
                curr->child->prev = curr;
                curr->child = nullptr; // Must set child pointer to nullptr
            }
            curr = curr->next;
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Flatten multilevel doubly linked list so nodes appear in single-level doubly linked list (LC 430).
// - Core Pattern: Iterative Child Splicing with Tail Reattachment.
// - Intuition:
//   * Walk across list. When `curr->child` is found, find the tail of that child list.
//   * Splice the child list between `curr` and `curr->next`.
//   * Zero out `curr->child` and continue walking forward.
// - Complexity:
//   * Time: O(N) — Each node is visited at most twice.
//   * Space: O(1) auxiliary space.
// - Edge Cases: List with no children, child lists nested multiple levels deep, child on last node of a level.
// - Likely Follow-ups:
//   1. How to solve using recursion / stack? -> DFS preorder traversal maintaining `prev` pointer (costs O(D) stack space).
//   2. Forget setting `curr->child = nullptr`? -> Will fail validation; doubly linked list nodes must have child set to null.


// =========================================================
// 18. CONVERT BST TO SORTED DOUBLY LINKED LIST (LC 426)
// =========================================================

class Solution18 {
    TreeNode* head = nullptr;
    TreeNode* prev = nullptr;

    void inorder(TreeNode* curr) {
        if (!curr) return;
        
        inorder(curr->left);
        
        // Rewire prev and curr
        if (prev) {
            prev->right = curr; // prev->next = curr
            curr->left = prev;  // curr->prev = prev
        } else {
            head = curr;        // Smallest node becomes DLL head
        }
        prev = curr;
        
        inorder(curr->right);
    }

public:
    TreeNode* treeToDoublyList(TreeNode* root) {
        if (!root) return nullptr;
        
        head = nullptr;
        prev = nullptr;
        
        inorder(root);
        
        // Circular closure: link head and tail (prev)
        prev->right = head;
        head->left = prev;
        
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Convert BST to sorted circular doubly linked list in-place (LC 426).
// - Core Pattern: Inorder DFS with Tracking Pointers + Circular Loop Closure.
// - Intuition:
//   * Inorder traversal visits BST keys in sorted ascending order.
//   * Wire `prev->right = curr` and `curr->left = prev` during traversal.
//   * At the end, wire `tail->right = head` and `head->left = tail` to close the ring.
// - Complexity:
//   * Time: O(N) — Standard inorder traversal.
//   * Space: O(H) recursion stack space (O(N) for skewed BST). Can be O(1) using Morris Traversal.
// - Edge Cases: Single node (points to itself circularly), empty tree (nullptr).
// - Likely Follow-ups:
//   1. Can you do this iteratively? -> Yes, standard explicit stack inorder traversal.
//   2. Can you do this in strictly O(1) auxiliary space? -> Yes, Morris Inorder Traversal.


// ====================================================================================================
// GROUP 6: DESIGN & CACHE ARCHITECTURES
// ====================================================================================================

// =========================================================
// 19. LRU CACHE (LC 146)
// =========================================================

class LRUCache {
    struct DNode {
        int key;
        int val;
        DNode* prev;
        DNode* next;
        DNode(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    unordered_map<int, DNode*> cache;
    DNode* head;
    DNode* tail;

    // Helper: Remove node from doubly linked list
    void removeNode(DNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // Helper: Add node right after head sentinel (Most Recently Used)
    void addToHead(DNode* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    // Helper: Move existing node to head
    void moveToHead(DNode* node) {
        removeNode(node);
        addToHead(node);
    }

    // Helper: Evict Least Recently Used node (before tail sentinel)
    DNode* popTail() {
        DNode* lru = tail->prev;
        removeNode(lru);
        return lru;
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new DNode(0, 0);
        tail = new DNode(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        DNode* curr = head;
        while (curr) {
            DNode* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }

    int get(int key) {
        if (!cache.count(key)) return -1;
        DNode* node = cache[key];
        moveToHead(node); // Record access
        return node->val;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            DNode* node = cache[key];
            node->val = value;
            moveToHead(node);
        } else {
            DNode* newNode = new DNode(key, value);
            cache[key] = newNode;
            addToHead(newNode);
            
            if ((int)cache.size() > capacity) {
                DNode* lru = popTail();
                cache.erase(lru->key);
                delete lru; // Free memory
            }
        }
    }
};
// Interview Explanation:
// - Problem Statement: Design Least Recently Used (LRU) Cache with O(1) get and put operations (LC 146).
// - Core Pattern: Hash Map + Doubly Linked List with Sentinel Head & Tail.
// - Intuition:
//   * Hash map gives O(1) lookup to the node.
//   * Doubly linked list allows O(1) node removal and insertion to the front (MRU).
//   * Sentinels eliminate null-checks on boundary insertions/deletions.
// - Complexity:
//   * Time: O(1) for both `get` and `put`.
//   * Space: O(Capacity) — Stores at most capacity nodes and hash entries.
// - Edge Cases: Updating an existing key (must update value AND move to head), capacity = 1.
// - Likely Follow-ups:
//   1. How to make LRU thread-safe? -> `std::mutex` or `std::shared_mutex` (reader/writer lock) around get/put.
//   2. What if TTL (time-to-live) is required? -> Priority queue of expiration timestamps or tick-based lazy eviction.


// =========================================================
// 20. LFU CACHE (LC 460)
// =========================================================

class LFUCache {
    struct LFUNode {
        int key, val, freq;
        LFUNode *prev, *next;
        LFUNode(int k, int v) : key(k), val(v), freq(1), prev(nullptr), next(nullptr) {}
    };

    struct DList {
        LFUNode *head, *tail;
        int size;
        DList() {
            head = new LFUNode(0, 0);
            tail = new LFUNode(0, 0);
            head->next = tail;
            tail->prev = head;
            size = 0;
        }
        ~DList() {
            LFUNode* curr = head;
            while (curr) {
                LFUNode* nxt = curr->next;
                delete curr;
                curr = nxt;
            }
        }
        void addFront(LFUNode* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
            size++;
        }
        void remove(LFUNode* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            size--;
        }
        LFUNode* removeTail() {
            if (size == 0) return nullptr;
            LFUNode* node = tail->prev;
            remove(node);
            return node;
        }
    };

    int capacity;
    int minFreq;
    unordered_map<int, LFUNode*> keyTable;
    unordered_map<int, DList*> freqTable;

    void updateFreq(LFUNode* node) {
        int oldFreq = node->freq;
        freqTable[oldFreq]->remove(node);
        
        // If the emptied list had minFreq, increment minFreq
        if (freqTable[oldFreq]->size == 0 && minFreq == oldFreq) {
            minFreq++;
        }
        
        node->freq++;
        if (!freqTable.count(node->freq)) {
            freqTable[node->freq] = new DList();
        }
        freqTable[node->freq]->addFront(node);
    }

public:
    LFUCache(int cap) : capacity(cap), minFreq(0) {}

    int get(int key) {
        if (!keyTable.count(key)) return -1;
        LFUNode* node = keyTable[key];
        updateFreq(node);
        return node->val;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;
        
        if (keyTable.count(key)) {
            LFUNode* node = keyTable[key];
            node->val = value;
            updateFreq(node);
        } else {
            if ((int)keyTable.size() >= capacity) {
                // Evict LRU node from minFreq bucket
                LFUNode* lfu = freqTable[minFreq]->removeTail();
                keyTable.erase(lfu->key);
                delete lfu;
            }
            
            LFUNode* newNode = new LFUNode(key, value);
            keyTable[key] = newNode;
            minFreq = 1;
            
            if (!freqTable.count(1)) {
                freqTable[1] = new DList();
            }
            freqTable[1]->addFront(newNode);
        }
    }
};
// Interview Explanation:
// - Problem Statement: Design Least Frequently Used (LFU) cache with tie-breaking by LRU in O(1) operations (LC 460).
// - Core Pattern: Hash Map + Frequency Bucket Doubly Linked Lists + `minFreq` tracker.
// - Intuition:
//   * Group nodes into separate doubly linked lists keyed by their access frequency.
//   * Maintain `minFreq`. When evicting, pop from the tail of `freqTable[minFreq]`.
// - Complexity:
//   * Time: O(1) strictly for both `get` and `put`.
//   * Space: O(Capacity).
// - Edge Cases: capacity = 0, updating existing keys (increases freq without eviction).
// - Likely Follow-ups:
//   1. Why two hash maps? -> `keyTable` gives O(1) key-to-node lookup; `freqTable` gives O(1) frequency bucket management.
//   2. What is the tie-breaker rule? -> LRU order within the same frequency bucket (enforced by the DLL).


/*
 ====================================================================================================
                                  A. CORE PATTERNS QUICK-GLANCE TABLE
 ====================================================================================================
 | Pattern                          | Problems Covered                  | Trigger / Signal in Interviews                                        |
 | :--------------------------------| :---------------------------------| :---------------------------------------------------------------------|
 | Fast & Slow Two-Pointer          | LC 142, LC 876, LC 234, LC 143    | "Find middle", "detect cycle", "cycle entry point", "palindrome"      |
 | Fixed Gap Two-Pointer            | LC 19                             | "N-th node from end of list", "fixed window distance"                 |
 | Pointer Switch (a + b = b + a)   | LC 160                            | "Find intersection of two linked lists" without hash set              |
 | Dummy Sentinel Node              | LC 19, LC 21, LC 24, LC 82, LC 2  | Any problem where the `head` itself might be deleted, swapped, or built|
 | In-Place 3-Pointer Reversal      | LC 206, LC 25, LC 234, LC 143     | "Reverse list", "reverse in groups of K", "half reversal"              |
 | Interleave-Clone-Detach          | LC 138                            | "Deep copy list with random pointers" in O(1) auxiliary space          |
 | Min-Heap / Divide & Conquer Merge| LC 23, LC 148                     | "Merge K sorted lists", "sort linked list in O(N log N)"               |
 | Hash Map + Doubly Linked List    | LC 146, LC 460                    | "LRU cache", "LFU cache", O(1) eviction architectures                  |
 ====================================================================================================

 ====================================================================================================
                               B. MICROSOFT-SPECIFIC INTERVIEW CHECKLIST
 ====================================================================================================
 1. DUMMY HEAD TECHNIQUE (AVOID NULL CHECKS):
    - Always initialize a stack-allocated dummy node: `ListNode dummy(0, head); ListNode* tail = &dummy;`.
    - Stack allocation (`ListNode dummy(0);`) avoids heap allocation and memory leak risks.
    - Return `dummy.next` at the end. Use it whenever head can change, get deleted, or when building a new list.

 2. MEMORY MANAGEMENT (AVOID C++ LEAKS):
    - In Microsoft interviews, failing to `delete` unlinked nodes is viewed as a red flag.
    - When removing nodes (LC 19, LC 82, LC 83, LRU eviction), save `toDelete = slow->next;` before relinking,
      then explicitly call `delete toDelete;`.

 3. STRICT O(1) AUXILIARY SPACE DISCIPLINE:
    - Never dump linked list values into a `std::vector<int>`, sort/modify, and reconstruct nodes.
    - Doing this wastes O(N) heap allocations and demonstrates lack of pointer mastery.
    - Master pointer-flipping (`prev, curr, nextTemp`) and splicing techniques cold.

 4. RESTORING MODIFIED LISTS:
    - If you reverse or split a list to check a condition (e.g. Palindrome LC 234), restore the original
      structure before returning `true`/`false`. Interviewers look for this attention to caller safety.
 ====================================================================================================
*/
