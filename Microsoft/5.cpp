#include <bits/stdc++.h>
using namespace std;

// Standard singly linked list node
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x = 0, ListNode* n = nullptr) : val(x), next(n) {}
};

// Generic node with multi-directional pointers (doubly, random, child, tree)
struct Node {
    int val;
    Node* prev;
    Node* next;
    Node* child;
    Node* random;
    Node* left;
    Node* right;

    Node(int v = 0)
        : val(v), prev(nullptr), next(nullptr), child(nullptr),
          random(nullptr), left(nullptr), right(nullptr) {}
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

// ====================================================================================================
//                               COMMON REUSABLE HELPER FUNCTIONS
// ====================================================================================================

// 1. Reverse an entire singly linked list (Used in: Solution 9, Solution 10)
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}

// 2. Find the middle node of a linked list (first mid for even length, ideal for bisecting) (Used in: Solution 4, 9, 10)
ListNode* getMid(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// 3. Merge two sorted linked lists into one sorted list (Used in: Solution 4 Sort List)
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (a != nullptr && b != nullptr) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    tail->next = (a != nullptr) ? a : b;

    return dummy.next;
}

// 4. Calculate the length of a linked list (Used in: Solution 19)
int getLength(ListNode* head) {
    int len = 0;
    while (head != nullptr) {
        len++;
        head = head->next;
    }
    return len;
}

// ====================================================================================================
//                                      HIGH-YIELD PROBLEMS
// ====================================================================================================

// 1. Reverse Linked List II
// Reverse nodes from position `left` to `right` (1-indexed) in a single pass, in-place.
struct Solution1 {
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head); // Dummy node to simplify edge cases at head
        ListNode* prev = &dummy; // Pointer to node before the reversal segment

        // Move `prev` to the node immediately before `left`
        for (int i = 1; i < left; i++) {
            prev = prev->next;
        }

        // Start of the segment to reverse
        ListNode* curr = prev->next;

        // Move each next node to the front of the reversed portion
        for (int i = 0; i < right - left; i++) {
            ListNode* nodeToMove = curr->next;

            curr->next = nodeToMove->next; // Remove nodeToMove from its current position
            nodeToMove->next = prev->next; // Insert it at the front of the reversed part
            prev->next = nodeToMove;       // Connect prev to the new front
        }

        return dummy.next; // Return the new head (could be unchanged if left > 1)
    }
};

// 2. Reverse Nodes in k-Group
// Reverse nodes in chunks of size k; leftover nodes (< k) at the end stay unreversed.
struct Solution2 {
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0, head);
        ListNode* groupPrev = &dummy;

        while (true) {
            // Find the k-th node
            ListNode* kth = groupPrev;
            int count = 0;

            while (count < k && kth) {
                kth = kth->next; // Move to the k-th node
                count++;
            }

            // Less than k nodes remain
            if (!kth) break;

            ListNode* groupNext = kth->next;

            // Reverse current group
            ListNode* prev = groupNext;
            ListNode* curr = groupPrev->next;

            while (curr != groupNext) {
                ListNode* next = curr->next;
                curr->next = prev;
                prev = curr;
                curr = next;
            }

            // Connect reversed group
            ListNode* oldGroupStart = groupPrev->next;
            groupPrev->next = kth;
            groupPrev = oldGroupStart;
        }

        return dummy.next;
    }
};

// 3. Merge k Sorted Lists
// Merge k sorted linked lists into one sorted list.
struct Solution3 {
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;
        };

        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp); // Min-heap based on node values

        for (auto node : lists) {
            if (node) pq.push(node);
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (!pq.empty()) {
            ListNode* node = pq.top();
            pq.pop();

            tail->next = node; // Append the smallest node to the merged list
            tail = tail->next; // Move tail forward

            if (node->next) {
                pq.push(node->next);
            }
        }

        return dummy.next;
    }
};

// 4. Sort List
// Sort a linked list in O(N log N) time, O(log N) space using Merge Sort.
struct Solution4 {
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // Split the list at midpoint using the getMid helper
        ListNode* mid = getMid(head);
        ListNode* rightHead = mid->next;
        mid->next = nullptr;

        // Recursively sort both halves and merge using the mergeTwoLists helper
        ListNode* left = sortList(head);
        ListNode* right = sortList(rightHead);

        return mergeTwoLists(left, right);
    }
};

// 5. Linked List Cycle
// Determine whether the list contains a cycle.
struct Solution5 {
    bool hasCycle(ListNode* head) {
        ListNode* slow = head; // Moves 1 step
        ListNode* fast = head; // Moves 2 steps

        // Fast pointer moves two steps, slow pointer moves one step
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;

            // If pointers meet, there is a cycle
            if (slow == fast) {
                return true;
            }
        }

        return false; // Fast reached nullptr, so no cycle
    }
};

// 6. Linked List Cycle II
// Return the node where the cycle begins, or nullptr if there is none.
struct Solution6 {
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // Phase 1: Determine if a cycle exists
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) {
                // Phase 2: Find the entrance of the cycle
                ListNode* ptr = head;
                while (ptr != slow) {
                    ptr = ptr->next;
                    slow = slow->next;
                }
                return ptr;
            }
        }

        return nullptr; // No cycle found
    }
};

// 7. Find the Duplicate Number
// Array of n+1 ints in [1,n]; find the duplicate without modifying array, O(1) extra space.
struct Solution7 {
    int findDuplicate(vector<int>& nums) {
        int slow = nums[0];
        int fast = nums[0];

        // Phase 1: Detect intersection using Floyd's Tortoise and Hare
        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        } while (slow != fast);

        // Phase 2: Find the entrance to the cycle (the duplicate number)
        slow = nums[0];
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }

        return slow;
    }
};

// 8. Remove Nth Node From End of List
// Remove the n-th node from the end and return the head.
struct Solution8 {
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;

        // Advance fast pointer by n steps to establish the n-node gap
        for (int i = 0; i < n; i++) {
            fast = fast->next;
        }

        // Move both pointers until fast is at the last node
        while (fast->next) {
            slow = slow->next;
            fast = fast->next;
        }

        // Delete the n-th node from the end
        slow->next = slow->next->next;

        return dummy.next;
    }
};

// 9. Reorder List
// Reorder list into L0 -> Ln -> L1 -> Ln-1 -> ... in-place.
struct Solution9 {
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;

        // Step 1: Find middle and reverse the second half using helpers
        ListNode* mid = getMid(head);
        ListNode* second = reverseList(mid->next);
        mid->next = nullptr;

        // Step 2: Interleave the first half and the reversed second half
        ListNode* first = head;
        while (second) {
            ListNode* next1 = first->next;
            ListNode* next2 = second->next;

            first->next = second;
            second->next = next1;

            first = next1;
            second = next2;
        }
    }
};

// 10. Palindrome Linked List
// Determine whether a singly linked list is a palindrome.
struct Solution10 {
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;

        // Step 1: Find mid and reverse the second half using helpers
        ListNode* mid = getMid(head);
        ListNode* second = reverseList(mid->next);
        ListNode* first = head;

        // Step 2: Compare values between first half and reversed second half
        while (second) {
            if (first->val != second->val) {
                return false;
            }
            first = first->next;
            second = second->next;
        }

        return true;
    }
};

// 11. Odd Even Linked List
// Group all odd-indexed nodes together followed by even-indexed nodes, O(1) space.
struct Solution11 {
    ListNode* oddEvenList(ListNode* head) {
        if (!head || !head->next) {
            return head;
        }

        ListNode* odd = head;
        ListNode* even = head->next;
        ListNode* evenHead = even; // Keep reference to start of even sublist

        // Unlink even and odd nodes into two parallel sublists
        while (even && even->next) {
            odd->next = even->next;
            odd = odd->next;

            even->next = odd->next;
            even = even->next;
        }

        // Attach even sublist after odd sublist
        odd->next = evenHead;
        return head;
    }
};

// 12. Rotate List
// Rotate the list to the right by k places.
struct Solution12 {
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) {
            return head;
        }

        // Step 1: Compute the length of the list and locate the tail node
        int len = 1;
        ListNode* tail = head;
        while (tail->next) {
            tail = tail->next;
            len++;
        }

        // Normalize k
        k %= len;
        if (k == 0) {
            return head;
        }

        // Step 2: Form a ring by connecting the tail to head
        tail->next = head;

        // Step 3: Find the new tail at position (len - k - 1)
        ListNode* newTail = head;
        for (int i = 0; i < len - k - 1; i++) {
            newTail = newTail->next;
        }

        // Step 4: Break the ring to establish the new head
        ListNode* newHead = newTail->next;
        newTail->next = nullptr;

        return newHead;
    }
};

// 13. Partition List
// Partition so nodes < x precede nodes >= x, preserving relative order.
struct Solution13 {
    ListNode* partition(ListNode* head, int x) {
        ListNode lessDummy(0);
        ListNode greaterDummy(0);
        ListNode* less = &lessDummy;
        ListNode* greater = &greaterDummy;

        // Partition nodes into less and greater/equal lists
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

        // Terminate the greater list and link the two lists together
        greater->next = nullptr;
        less->next = greaterDummy.next;

        return lessDummy.next;
    }
};

// 14. Remove Duplicates from Sorted List II
// Delete all nodes that have duplicate values, leaving only distinct numbers.
struct Solution14 {
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy; // Preceding node of distinct sublist

        while (head) {
            // Check if current node begins a run of duplicates
            if (head->next && head->val == head->next->val) {
                // Skip all subsequent nodes with the same value
                while (head->next && head->val == head->next->val) {
                    head = head->next;
                }
                // Bypass all duplicate nodes
                prev->next = head->next;
            } else {
                // Node is unique; move prev pointer forward
                prev = prev->next;
            }
            head = head->next;
        }

        return dummy.next;
    }
};

// 15. Add Two Numbers
// Add two numbers represented by linked lists, digits stored in reverse order.
struct Solution15 {
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        int carry = 0;

        // Traverse both lists and propagate carry
        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            int sum = carry;

            if (l1 != nullptr) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2 != nullptr) {
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

// 16. Add Two Numbers II
// Same as above but digits stored most-significant-first; inputs must not be modified.
struct Solution16 {
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        stack<int> s1;
        stack<int> s2;

        // Push digits into stacks to process least-significant digits first
        for (ListNode* p = l1; p != nullptr; p = p->next) {
            s1.push(p->val);
        }
        for (ListNode* p = l2; p != nullptr; p = p->next) {
            s2.push(p->val);
        }

        ListNode* head = nullptr;
        int carry = 0;

        // Pop digits, compute sum, and prepend nodes to form the result list
        while (!s1.empty() || !s2.empty() || carry != 0) {
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

            // Prepend new node with the digit
            ListNode* newNode = new ListNode(sum % 10, head);
            head = newNode;
        }

        return head;
    }
};

// 17. Intersection of Two Linked Lists
// Find the node at which two singly linked lists intersect.
struct Solution17 {
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        if (!headA || !headB) {
            return nullptr;
        }

        ListNode* pA = headA;
        ListNode* pB = headB;

        // Traversing both lists equalizes total distance traveled: (lenA + lenB)
        // If they intersect, they meet at the intersection node; otherwise at nullptr
        while (pA != pB) {
            pA = (pA == nullptr) ? headB : pA->next;
            pB = (pB == nullptr) ? headA : pB->next;
        }

        return pA;
    }
};

// 18. Copy List with Random Pointer
// Deep-copy a list with next and random pointers, O(1) auxiliary space.
struct Solution18 {
    Node* copyRandomList(Node* head) {
        if (!head) {
            return nullptr;
        }

        // Step 1: Create clone nodes and interleave them directly after original nodes
        for (Node* curr = head; curr != nullptr; curr = curr->next->next) {
            Node* clone = new Node(curr->val);
            clone->next = curr->next;
            curr->next = clone;
        }

        // Step 2: Assign cloned random pointers
        for (Node* curr = head; curr != nullptr; curr = curr->next->next) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
        }

        // Step 3: Separate cloned list from original list
        Node dummy(0);
        Node* cloneTail = &dummy;

        for (Node* curr = head; curr != nullptr; curr = curr->next) {
            Node* cloneNode = curr->next;
            cloneTail->next = cloneNode;
            cloneTail = cloneTail->next;

            // Restore original list pointer
            curr->next = cloneNode->next;
        }

        return dummy.next;
    }
};

// 19. Split Linked List in Parts
// Split list into k consecutive parts of balanced size (difference <= 1, earlier parts larger).
struct Solution19 {
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        int len = getLength(head);
        int baseSize = len / k;
        int remainder = len % k;

        vector<ListNode*> result(k, nullptr);
        ListNode* curr = head;

        // Step 2: Split the list into k chunks
        for (int i = 0; i < k && curr != nullptr; i++) {
            result[i] = curr;
            int partSize = baseSize + (i < remainder ? 1 : 0);

            // Move to the end of the current chunk
            for (int j = 1; j < partSize; j++) {
                curr = curr->next;
            }

            // Sever connection to the next part
            ListNode* nextPart = curr->next;
            curr->next = nullptr;
            curr = nextPart;
        }

        return result;
    }
};

// 20. Next Greater Node In Linked List
// For each node, find the value of the next strictly greater node (0 if none).
struct Solution20 {
    vector<int> nextLargerNodes(ListNode* head) {
        // Convert linked list to vector for random index access
        vector<int> values;
        for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
            values.push_back(curr->val);
        }

        vector<int> answer(values.size(), 0);
        stack<int> monoStack; // Monotonically decreasing stack of indices

        // Process elements and resolve next greater values
        for (int i = 0; i < (int)values.size(); i++) {
            while (!monoStack.empty() && values[monoStack.top()] < values[i]) {
                answer[monoStack.top()] = values[i];
                monoStack.pop();
            }
            monoStack.push(i);
        }

        return answer;
    }
};

// 21. Merge In Between Linked Lists
// Remove nodes from index a to b in list1 and splice list2 in their place.
struct Solution21 {
    ListNode* mergeInBetween(ListNode* list1, int a, int b, ListNode* list2) {
        // Step 1: Traverse to node at index (a - 1)
        ListNode* prevA = list1;
        for (int i = 0; i < a - 1; i++) {
            prevA = prevA->next;
        }

        // Step 2: Traverse to node at index (b + 1)
        ListNode* afterB = prevA;
        for (int i = 0; i < b - a + 2; i++) {
            afterB = afterB->next;
        }

        // Step 3: Splice list2 after prevA
        prevA->next = list2;

        // Step 4: Traverse to the tail of list2
        ListNode* tail2 = list2;
        while (tail2->next) {
            tail2 = tail2->next;
        }

        // Step 5: Connect tail of list2 to afterB
        tail2->next = afterB;

        return list1;
    }
};