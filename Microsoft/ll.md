# Linked List — Core Patterns & Microsoft OA Survival Sheet

> **The Core Realization:** Linked lists feel tricky mainly because there are only **6 real patterns** hiding behind 35 different problem names. Learn the patterns and their invariant movements, not individual problems.

---

## 0. The Golden Rules & Invariants

### 1. The #1 Habit That Fixes 80% of Bugs
**Always draw 3–4 boxes and arrows on scratchpad before typing a single line of code.**  
Linked list bugs are almost never logic bugs — they are **pointer-order bugs**. If you can trace the pointer reassignment order by hand, your code will work on the first run.

### 2. The Golden Question: "Do I Need a Dummy Node?"
> **Rule:** Whenever the **head itself might change, get deleted, or when constructing a new list from scratch**, ALWAYS use a dummy node.

```cpp
// Standard struct used across all C++ solutions:
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

// Stack-allocated dummy head (avoids heap allocation / memory leak)
ListNode dummy(0, head);
ListNode* prev = &dummy;

// ... perform manipulations on dummy.next ...

return dummy.next; // Returns the real, potentially new head
```

### 3. The Cardinal Pointer Rule
```cpp
ListNode* nextTemp = curr->next; // 1. SAVE the next pointer first
curr->next = prev;               // 2. REWIRE current pointer
prev = curr;                      // 3. ADVANCE previous
curr = nextTemp;                  // 4. ADVANCE current
```
> **Warning:** Never overwrite `curr->next` before saving it in a temporary variable if you still need to reach the remainder of the list!

---

## 1. Pattern: Iterative Pointer Reversal

**Problems Using It:** Reverse Linked List (LC 206), Reverse Linked List II (LC 92), Reverse Nodes in k-Group (LC 25), Swap Nodes in Pairs (LC 24), Reorder List (LC 143), Palindrome Linked List (LC 234).

**Core Idea:** Walk the list once, flipping `next` pointers backwards using 3 rolling pointers: `prev`, `curr`, `nextTemp`.

### Master Template: Reverse Entire List
```cpp
ListNode* reverse(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr) {
        ListNode* nextTemp = curr->next; // 1. Save
        curr->next = prev;               // 2. Reverse
        prev = curr;                      // 3. Move prev
        curr = nextTemp;                  // 4. Move curr
    }
    return prev; // New head of reversed list
}
```

### In-Place Sublist Reversal (Between Position $m$ and $n$)
```cpp
ListNode* reverseBetween(ListNode* head, int left, int right) {
    if (!head || left == right) return head;
    
    ListNode dummy(0, head);
    ListNode* prev = &dummy;
    
    // 1. Advance prev to node immediately before sublist (position left - 1)
    for (int i = 1; i < left; ++i) {
        prev = prev->next;
    }
    
    // 2. Incrementally move each subsequent node to the front of the sublist
    ListNode* curr = prev->next;
    for (int i = 0; i < right - left; ++i) {
        ListNode* nextTemp = curr->next;
        curr->next = nextTemp->next;
        nextTemp->next = prev->next;
        prev->next = nextTemp;
    }
    
    return dummy.next;
}
```

### Reverse Nodes in k-Group (LC 25)
```cpp
ListNode* reverseKGroup(ListNode* head, int k) {
    if (!head || k <= 1) return head;
    
    ListNode dummy(0, head);
    ListNode* groupPrev = &dummy;
    
    while (true) {
        // 1. Check if k nodes exist ahead
        ListNode* kth = groupPrev;
        for (int i = 0; i < k && kth; ++i) {
            kth = kth->next;
        }
        if (!kth) break; // Fewer than k nodes remain: leave unchanged
        
        ListNode* groupNext = kth->next;
        ListNode* prev = groupNext;
        ListNode* curr = groupPrev->next;
        
        // 2. Reverse k nodes in-place
        while (curr != groupNext) {
            ListNode* temp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = temp;
        }
        
        // 3. Connect previous group to new group head
        ListNode* oldGroupHead = groupPrev->next;
        groupPrev->next = kth;
        groupPrev = oldGroupHead;
    }
    return dummy.next;
}
```

---

## 2. Pattern: Slow & Fast Pointers (Floyd's Technique)

**Problems Using It:** Middle of Linked List (LC 876), Linked List Cycle (LC 141), Linked List Cycle II (LC 142), Palindrome Linked List (LC 234), Reorder List (LC 143).

**Core Idea:** `slow` advances 1 step, `fast` advances 2 steps. When `fast` reaches the end, `slow` is at the midpoint. If a cycle exists, they inevitably collide inside the cycle.

### 1. Middle of Linked List
```cpp
// Returns 2nd middle for even length (e.g., [1, 2, 3, 4] -> 3)
ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Returns 1st middle for even length (Best for splitting list in MergeSort/Reorder)
ListNode* firstMiddleNode(ListNode* head) {
    if (!head) return nullptr;
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
```

### 2. Cycle Detection (LC 141)
```cpp
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
```

### 3. Cycle Start Point (LC 142)
```cpp
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    
    // Phase 1: Detect meeting point inside cycle
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // Phase 2: Find cycle start
            // Distance from head to cycle start == distance from meeting point to cycle start
            ListNode* ptr = head;
            while (ptr != slow) {
                ptr = ptr->next;
                slow = slow->next;
            }
            return ptr; // Both meet at cycle entry
        }
    }
    return nullptr;
}
```

---

## 3. Pattern: Two-Pointer Gap ($N$-th Node Tricks)

**Problems Using It:** Remove Nth Node From End of List (LC 19), Rotate List (LC 61).

**Core Idea:** Advance one pointer $n$ steps ahead first. Then advance both pointers together at the same speed. The fixed distance gap between them does the counting in a single pass.

```text
fast (advanced n steps)
  |
  |---- n ----|
  |           |
 slow        fast (move both until fast hits end)
```

### Remove Nth Node From End (LC 19)
```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;
    
    // Move fast n + 1 steps ahead so slow lands right BEFORE target node
    for (int i = 0; i <= n; ++i) {
        fast = fast->next;
    }
    
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    
    ListNode* toDelete = slow->next;
    slow->next = slow->next->next;
    delete toDelete; // Memory cleanup in C++
    
    return dummy.next;
}
```

### Rotate List (LC 61)
```cpp
ListNode* rotateRight(ListNode* head, int k) {
    if (!head || !head->next || k == 0) return head;
    
    // 1. Calculate length and find tail
    int len = 1;
    ListNode* tail = head;
    while (tail->next) {
        tail = tail->next;
        len++;
    }
    
    k %= len;
    if (k == 0) return head;
    
    // 2. Make circular ring
    tail->next = head;
    
    // 3. Find new tail at (len - k) steps from head
    int steps = len - k;
    ListNode* newTail = tail;
    while (steps--) {
        newTail = newTail->next;
    }
    
    // 4. Sever ring
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;
    
    return newHead;
}
```

---

## 4. Pattern: Dummy Head + Tail (Splice & Merge)

**Problems Using It:** Merge Two Sorted Lists (LC 21), Merge k Sorted Lists (LC 23), Add Two Numbers (LC 2), Partition List (LC 86).

**Core Idea:** Always build a new list using a stack-allocated `dummy` sentinel node plus a roving `tail` pointer. Never manually track the head of a newly constructed list.

```text
dummy -> [built list nodes]
             ^
            tail (keep pushing tail->next = node, tail = tail->next)
```

### Merge Two Sorted Lists (LC 21)
```cpp
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
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
    
    // Splice remainder in O(1) time
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}
```

### Merge k Sorted Lists via Min-Heap (LC 23)
```cpp
struct Compare {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, Compare> pq;
    for (ListNode* head : lists) {
        if (head) pq.push(head);
    }
    
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (!pq.empty()) {
        ListNode* curr = pq.top();
        pq.pop();
        
        tail->next = curr;
        tail = curr;
        
        if (curr->next) {
            pq.push(curr->next);
        }
    }
    return dummy.next;
}
```

### Add Two Numbers (LC 2)
```cpp
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    int carry = 0;
    
    while (l1 || l2 || carry) {
        int sum = carry;
        if (l1) { sum += l1->val; l1 = l1->next; }
        if (l2) { sum += l2->val; l2 = l2->next; }
        
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
        carry = sum / 10;
    }
    return dummy.next;
}
```

---

## 5. Pattern: Dummy + Prev/Curr (Filter & Delete)

**Problems Using It:** Remove Linked List Elements (LC 203), Remove Duplicates from Sorted List I & II (LC 83 / 82), Odd Even List (LC 328), Partition List (LC 86).

**Core Idea:** Rebuild the list by deciding node-by-node whether it stays or goes. Always keep a `prev` pointer to relink around removed nodes.

> **CRITICAL RULE:**
> - When **deleting** a node: `prev` does **NOT** move (`prev->next = curr->next`).
> - When **keeping** a node: `prev` moves (`prev = curr`).
> *(Failing this is the #1 bug in Microsoft OAs!)*

### Remove Elements Matching Value (LC 203)
```cpp
ListNode* removeElements(ListNode* head, int val) {
    ListNode dummy(0, head);
    ListNode* prev = &dummy;
    ListNode* curr = head;
    
    while (curr) {
        if (curr->val == val) {
            prev->next = curr->next; // Skip curr (prev does NOT advance)
            delete curr;             // Memory cleanup
        } else {
            prev = curr;             // Advance prev only when keeping
        }
        curr = prev->next;
    }
    return dummy.next;
}
```

### Remove Duplicates II — Delete ALL Duplicate Nodes (LC 82)
```cpp
ListNode* deleteDuplicates(ListNode* head) {
    ListNode dummy(0, head);
    ListNode* prev = &dummy;
    ListNode* curr = head;
    
    while (curr) {
        // Detect streak of duplicates
        if (curr->next && curr->val == curr->next->val) {
            int dupVal = curr->val;
            while (curr && curr->val == dupVal) {
                ListNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
            prev->next = curr; // Relink around all duplicates
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return dummy.next;
}
```

---

## 6. Pattern: Partition / Odd-Even = Two Lists

**Problems Using It:** Partition List (LC 86), Odd Even Linked List (LC 328).

**Core Idea:** When asked to bucket or reorder nodes while preserving relative order, **build two separate dummy-headed lists in a single pass**, then join them at the end.

```text
dummy1 -> [group 1 nodes] -> tail1
dummy2 -> [group 2 nodes] -> tail2
Connection: tail1->next = dummy2.next; tail2->next = nullptr;
```

### Odd Even Linked List (LC 328)
```cpp
ListNode* oddEvenList(ListNode* head) {
    if (!head || !head->next) return head;
    
    ListNode* odd = head;
    ListNode* even = head->next;
    ListNode* evenHead = even; // Anchor start of even sublist
    
    while (even && even->next) {
        odd->next = even->next;
        odd = odd->next;
        
        even->next = odd->next;
        even = even->next;
    }
    
    odd->next = evenHead; // Connect odd sublist to even sublist
    return head;
}
```

---

## 7. Deep Copy & Cache Architectures

### Copy List with Random Pointer (LC 138)
**$O(1)$ Auxiliary Space Recipe (Interleave $\to$ Set Randoms $\to$ Detach):**

```cpp
class Node {
public:
    int val;
    Node* next;
    Node* random;
    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

Node* copyRandomList(Node* head) {
    if (!head) return nullptr;
    
    // Step 1: Interleave cloned nodes: A -> A' -> B -> B'
    Node* curr = head;
    while (curr) {
        Node* clone = new Node(curr->val);
        clone->next = curr->next;
        curr->next = clone;
        curr = clone->next;
    }
    
    // Step 2: Set random pointers for cloned nodes
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }
    
    // Step 3: Detach lists
    curr = head;
    Node* cloneHead = head->next;
    Node* cloneCurr = cloneHead;
    
    while (curr) {
        curr->next = curr->next->next;
        cloneCurr->next = cloneCurr->next ? cloneCurr->next->next : nullptr;
        
        curr = curr->next;
        cloneCurr = cloneCurr->next;
    }
    return cloneHead;
}
```

### LRU Cache (LC 146)
**Core Idea:** Hash Map (`key -> Node*`) for $O(1)$ lookup + Doubly Linked List with dummy sentinels (`head` and `tail`) for $O(1)$ node removal and insertion.

```cpp
class LRUCache {
    struct DNode {
        int key, val;
        DNode *prev, *next;
        DNode(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    unordered_map<int, DNode*> cache;
    DNode *head, *tail;

    void removeNode(DNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addToHead(DNode* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void moveToHead(DNode* node) {
        removeNode(node);
        addToHead(node);
    }

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
            DNode* newNode = new DNode(key, value);
            cache[key] = newNode;
            addToHead(newNode);
            
            if ((int)cache.size() > capacity) {
                DNode* lru = popTail();
                cache.erase(lru->key);
                delete lru;
            }
        }
    }
};
```

---

## 8. Quick-Glance Pattern Spotting Table

| If the problem description says... | Reach immediately for... | Example Problems |
| :--- | :--- | :--- |
| "reverse", "between $m$ and $n$", "in groups of $k$" | **Iterative Pointer Reversal** (`prev, curr, next`) | LC 206, LC 92, LC 25 |
| "middle", "cycle exists", "cycle entry point" | **Slow/Fast Pointers** (Floyd's technique) | LC 876, LC 141, LC 142 |
| "palindrome", "reorder list alternate" | **Midpoint + Reversal + Interleave** | LC 234, LC 143 |
| "$n$-th node from end", "rotate by $k$" | **Two-Pointer Fixed Gap** | LC 19, LC 61 |
| "merge", "add numbers", "combine sorted" | **Dummy Node + Tail Splice** | LC 21, LC 23, LC 2 |
| "remove nodes", "delete duplicates" | **Dummy Node + Prev/Curr Filtering** | LC 203, LC 83, LC 82 |
| "partition", "odd-even", "separate by condition" | **Two Dummy Lists** (group into 2 sublists) | LC 86, LC 328 |
| "deep copy with random pointers" | **Interleave-Clone-Detach** ($O(1)$ space) | LC 138 |
| "LRU Cache" / "LFU Cache" | **Hash Map + Doubly Linked List** | LC 146, LC 460 |

---

## 9. Microsoft OA Decision Flowchart

```text
                         LINKED LIST PROBLEM
                                 |
         +-----------------------+-----------------------+
         |                                               |
    Need to reverse?                               Need to find?
         |                                               |
       [YES]                                       +-----+-----+
         |                                         |           |
  prev / curr / nextTemp                        Middle      N-th from end
  (LC 206, LC 92, LC 25)                           |           |
                                               Slow/Fast   Two-Pointer Gap
                                               (LC 876)       (LC 19)
         [NO]
         |
    Need to delete / filter?
         |
       [YES] ---> Dummy + Prev/Curr (LC 203, LC 82)
         |
       [NO]
         |
    Need to build / merge lists?
         |
       [YES] ---> Dummy + Tail pointer (LC 21, LC 2)
         |
       [NO]
         |
    Need to bucket / separate?
         |
       [YES] ---> Two Dummy Sublists + Splice (LC 86, LC 328)
         |
       [NO]
         |
    Palindrome / Reorder?
         |
       [YES] ---> 1. Find Mid -> 2. Reverse Second Half -> 3. Compare / Interleave
```

---

## 10. Pre-Submission Edge Case Checklist

Before submitting code in an interview or OA, trace these mentally:
- [ ] **Empty list (`head == nullptr`)**
- [ ] **Single node (`head->next == nullptr`)**
- [ ] **Two nodes (`[1, 2]`)**
- [ ] **The head gets deleted** (does `dummy.next` capture the new head?)
- [ ] **The tail gets deleted**
- [ ] **All nodes are deleted / duplicate** (e.g. `[1, 1, 1]` in LC 82)
- [ ] **$k = 1$ or $k = \text{length}$**
- [ ] **Odd length vs Even length lists**
- [ ] **Memory cleanup:** Did you call `delete` on unlinked nodes in C++?
- [ ] **Did you terminate lists?** (`tail->next = nullptr;` — forgetting this creates accidental cycles!).
