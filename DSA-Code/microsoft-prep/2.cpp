#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <climits>
#endif
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// =========================================================
// 18. BINARY SEARCH
// =========================================================

int binarySearch(vector<int>& nums, int target) {
    int left = 0, right = (int)nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find target in sorted array in O(log N) time.
// - Approach: Standard Binary Search.
// - Intuition: Compare target with middle element and eliminate half of remaining search space.
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 19. SEARCH IN ROTATED SORTED ARRAY
// =========================================================

int searchRotated(vector<int>& nums, int target) {
    int left = 0, right = (int)nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;

        if (nums[left] <= nums[mid]) { // Left half sorted
            if (target >= nums[left] && target < nums[mid]) right = mid - 1;
            else left = mid + 1;
        } else { // Right half sorted
            if (target > nums[mid] && target <= nums[right]) left = mid + 1;
            else right = mid - 1;
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find target in rotated sorted array in O(log N) time.
// - Approach: Modified Binary Search (Identify Sorted Half).
// - Intuition: At least one half is strictly sorted; check if target lies within sorted half boundaries.
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 20. FIND MINIMUM IN ROTATED SORTED ARRAY
// =========================================================

int findMin(vector<int>& nums) {
    int left = 0, right = (int)nums.size() - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > nums[right]) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return nums[left];
}
// Interview Explanation:
// - Problem Statement: Find minimum element in rotated sorted array.
// - Approach: Binary Search on Pivot Invariant (nums[mid] vs nums[right]).
// - Intuition: If nums[mid] > nums[right], minimum must lie in the right half.
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 21. FIND PEAK ELEMENT
// =========================================================

int findPeakElement(vector<int>& nums) {
    int left = 0, right = (int)nums.size() - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > nums[mid + 1]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}
// Interview Explanation:
// - Problem Statement: Find any peak element strictly greater than its neighbors.
// - Approach: Binary Search on Local Gradient.
// - Intuition: Ascending slope guarantees a peak to the right; descending slope guarantees a peak to the left.
// - Complexity: Time: O(log N), Space: O(1).


// =========================================================
// 22. KOKO EATING BANANAS
// =========================================================

bool canEatAll(const vector<int>& piles, int speed, int h) {
    int hours = 0;
    for (int p : piles) {
        hours += (p + speed - 1) / speed;
        if (hours > h) return false;
    }
    return hours <= h;
}

int minEatingSpeed(vector<int>& piles, int h) {
    int left = 1, right = *max_element(piles.begin(), piles.end());
    int ans = right;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (canEatAll(piles, mid, h)) {
            ans = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum eating speed k to eat all bananas within h hours.
// - Approach: Binary Search on Monotonic Answer Range [1, max(piles)].
// - Intuition: Feasibility function is monotonic; binary search for the smallest valid speed.
// - Complexity: Time: O(N log(max_pile)), Space: O(1).


// =========================================================
// 23. CAPACITY TO SHIP PACKAGES WITHIN D DAYS
// =========================================================

bool canShip(const vector<int>& weights, int cap, int days) {
    int neededDays = 1, currWeight = 0;
    for (int w : weights) {
        if (currWeight + w > cap) {
            neededDays++;
            currWeight = 0;
        }
        currWeight += w;
    }
    return neededDays <= days;
}

int shipWithinDays(vector<int>& weights, int days) {
    int left = *max_element(weights.begin(), weights.end());
    int right = 0;
    for (int w : weights) right += w;
    int ans = right;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (canShip(weights, mid, days)) {
            ans = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find least ship capacity to ship all packages within given days.
// - Approach: Binary Search on Monotonic Answer Range [max_weight, sum_weights].
// - Intuition: Greedy packaging checks feasibility in O(N); binary search narrows capacity in O(log(sum)).
// - Complexity: Time: O(N log(Sum)), Space: O(1).


// =========================================================
// 24. REVERSE LINKED LIST
// =========================================================

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}
// Interview Explanation:
// - Problem Statement: Reverse singly linked list in-place.
// - Approach: 3-Pointer In-Place Redirection (prev, curr, nxt).
// - Intuition: Save forward pointer before redirecting backward.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 25. LINKED LIST CYCLE
// =========================================================

bool hasCycle(ListNode *head) {
    if (!head || !head->next) return false;
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
// - Problem Statement: Detect if linked list has a cycle.
// - Approach: Floyd's Tortoise and Hare (Slow & Fast Pointers).
// - Intuition: Fast closes the relative distance to slow by 1 each step, guaranteeing collision if a loop exists.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 26. MERGE TWO SORTED LISTS
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
// - Problem Statement: Merge two sorted linked lists into one sorted list.
// - Approach: Two Pointers with Sentinel Dummy Head.
// - Intuition: Splice smaller node to tail and advance pointer; attach remaining non-null list at end.
// - Complexity: Time: O(N + M), Space: O(1).


// =========================================================
// 27. REMOVE NTH NODE FROM END OF LIST
// =========================================================

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;
    for (int i = 0; i <= n; ++i) fast = fast->next;

    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    ListNode* toDelete = slow->next;
    slow->next = slow->next->next;
    delete toDelete;
    return dummy.next;
}
// Interview Explanation:
// - Problem Statement: Remove nth node from end of linked list in a single pass.
// - Approach: Fast & Slow Pointers with (N+1) Gap Invariant.
// - Intuition: Advance fast (N+1) steps ahead so slow lands at predecessor of target node.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 28. REORDER LIST
// =========================================================

void reorderList(ListNode* head) {
    if (!head || !head->next || !head->next->next) return;

    // 1. Find middle
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. Reverse second half
    ListNode* second = slow->next;
    slow->next = nullptr;
    ListNode* prev = nullptr;
    while (second) {
        ListNode* nxt = second->next;
        second->next = prev;
        prev = second;
        second = nxt;
    }

    // 3. Interleave merge
    ListNode* first = head;
    second = prev;
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
// - Problem Statement: Reorder list to L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2...
// - Approach: 3-Stage Pipeline (Find Middle + Reverse 2nd Half + Alternating Merge).
// - Intuition: Splitting and reversing turns problem into standard two-list interleaving in O(1) space.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 29. INTERSECTION OF TWO LINKED LISTS
// =========================================================

ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (!headA || !headB) return nullptr;
    ListNode* a = headA;
    ListNode* b = headB;
    while (a != b) {
        a = a ? a->next : headB;
        b = b ? b->next : headA;
    }
    return a;
}
// Interview Explanation:
// - Problem Statement: Find node where two singly linked lists intersect.
// - Approach: Two-Pointer Traversal with Pointer Switch.
// - Intuition: Switching heads equalizes total path length (lenA + lenB), ensuring pointers meet at intersection node or nullptr.
// - Complexity: Time: O(N + M), Space: O(1).


// =========================================================
// 30. LRU CACHE
// =========================================================

class LRUCache {
    struct DNode {
        int key, val;
        DNode* prev;
        DNode* next;
        DNode(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    unordered_map<int, DNode*> cache;
    DNode* head;
    DNode* tail;

    void removeNode(DNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insertAtHead(DNode* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void moveToHead(DNode* node) {
        removeNode(node);
        insertAtHead(node);
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new DNode(-1, -1);
        tail = new DNode(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key)) return -1;
        DNode* node = cache[key];
        moveToHead(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            DNode* node = cache[key];
            node->val = value;
            moveToHead(node);
        } else {
            if ((int)cache.size() == capacity) {
                DNode* lru = tail->prev;
                cache.erase(lru->key);
                removeNode(lru);
                delete lru;
            }
            DNode* newNode = new DNode(key, value);
            cache[key] = newNode;
            insertAtHead(newNode);
        }
    }
};
// Interview Explanation:
// - Problem Statement: Design LRU Cache with O(1) get and put.
// - Approach: Hash Map + Doubly Linked List with Sentinel Head & Tail.
// - Intuition: Map provides O(1) key lookup; Doubly linked list allows O(1) node removal and head insertion.
// - Complexity: Time: O(1) all operations, Space: O(Capacity).


// =========================================================
// 31. MERGE K SORTED LISTS
// =========================================================

ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

    for (ListNode* head : lists) {
        if (head) pq.push(head);
    }

    ListNode dummy(0);
    ListNode* curr = &dummy;

    while (!pq.empty()) {
        ListNode* node = pq.top();
        pq.pop();

        curr->next = node;
        curr = curr->next;

        if (node->next) pq.push(node->next);
    }
    return dummy.next;
}
// Interview Explanation:
// - Problem Statement: Merge k sorted linked lists into one sorted list.
// - Approach: Min-Heap Priority Queue of list heads.
// - Intuition: Heap of size k maintains current minimum element across all k lists.
// - Complexity: Time: O(N log K) where N is total nodes, Space: O(K).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 18 | Binary Search                               | Standard Binary Search            | O(log N) | O(1)     |
 | 19 | Search in Rotated Sorted Array              | Modified Binary Search (Sorted)   | O(log N) | O(1)     |
 | 20 | Find Minimum in Rotated Sorted Array        | Binary Search (Pivot Invariant)   | O(log N) | O(1)     |
 | 21 | Find Peak Element                           | Binary Search on Gradient         | O(log N) | O(1)     |
 | 22 | Koko Eating Bananas                         | Binary Search on Monotonic Answer | O(N logM)| O(1)     |
 | 23 | Capacity To Ship Packages Within D Days     | Binary Search on Monotonic Answer | O(N logS)| O(1)     |
 | 24 | Reverse Linked List                         | 3-Pointer In-Place Redirection    | O(N)     | O(1)     |
 | 25 | Linked List Cycle                           | Floyd's Fast & Slow Pointers      | O(N)     | O(1)     |
 | 26 | Merge Two Sorted Lists                      | Two Pointers Sentinel Splicing    | O(N + M) | O(1)     |
 | 27 | Remove Nth Node From End of List           | Fast & Slow Pointers (Gap N+1)    | O(N)     | O(1)     |
 | 28 | Reorder List                                | Mid + Reverse 2nd + Alternating   | O(N)     | O(1)     |
 | 29 | Intersection of Two Linked Lists            | Two Pointers Pointer Switch       | O(N + M) | O(1)     |
 | 30 | LRU Cache                                   | Hash Map + Doubly Linked List     | O(1)     | O(Cap)   |
 | 31 | Merge K Sorted Lists                        | Min-Heap Priority Queue of Heads  | O(N logK)| O(K)     |
 ====================================================================================================
*/
