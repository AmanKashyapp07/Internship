# Linked List — Core Patterns Cheat Sheet (Microsoft OA Prep)

Linked lists feel hard mainly because there are only ~6 real patterns hiding
behind 35 different problem names. Learn the pattern, not the problem.

---

## 0. The One Habit That Fixes 80% of Bugs

**Always draw 3–4 boxes and arrows on paper/scratchpad before typing code.**
Linked list bugs are pointer-order bugs, not logic bugs. If you can trace it
by hand, you can code it.

**Always ask yourself:** "Do I need a dummy node?" — Yes, whenever the head
itself might change or get deleted.

```cpp
// struct used throughout the C++ snippets below:
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

ListNode dummy(0, head);
// work using dummy.next as the "real" head
return dummy.next;
```

Use `dummy` whenever: deleting nodes, merging lists, partitioning, or any
problem where the new head is not necessarily the old head.

---

## 1. Pattern: Iterative Pointer Reversal

**Used in:** Reverse Linked List, Reverse Linked List II, Reverse Nodes in
k-Group, Swap Nodes in Pairs, Reorder List, Palindrome Linked List.

**Core idea:** walk the list once, flipping `next` pointers as you go, using
3 rolling pointers: `prev`, `curr`, `nextTemp`.

```cpp
ListNode* reverse(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr) {
        ListNode* nextTemp = curr->next;  // save before we overwrite it
        curr->next = prev;                // flip the pointer
        prev = curr;                       // advance prev
        curr = nextTemp;                   // advance curr
    }
    return prev;  // new head
}
```

**Reversing a sublist (positions m to n):** reverse normally *inside* the
range, then reconnect the boundary nodes on both sides.

**k-Group variant:** first check if `k` nodes exist ahead (else stop),
reverse that group, then recursively/iteratively attach to the next group.

**Common mistake:** forgetting to save `curr->next` before overwriting it —
you lose the rest of the list permanently.

---

## 2. Pattern: Slow/Fast Pointer (Floyd's Technique)

**Used in:** Middle of Linked List, Linked List Cycle, Linked List Cycle II,
Palindrome Linked List, Reorder List, Find the Duplicate Number.

**Core idea:** `slow` moves 1 step, `fast` moves 2 steps. When `fast` reaches
the end, `slow` is at the middle. If there's a cycle, they eventually meet
inside it.

```cpp
ListNode* middle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;  // middle node (2nd middle if even length)
}

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

**Finding the cycle START (Cycle II):** after slow/fast meet, reset one
pointer to `head`, then move both one step at a time — they meet exactly at
the cycle's start. This is the part people forget; memorize it as a fixed
recipe, don't re-derive it under pressure.

```cpp
ListNode* cycleStart(ListNode* head) {
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
```

---

## 3. Pattern: Two-Pointer Gap (N-th Node Tricks)

**Used in:** Remove Nth Node From End, Rotate List.

**Core idea:** move one pointer `n` steps ahead first, then move both
together — the gap between them does the counting for you, in one pass.

```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;
    for (int i = 0; i < n; i++) {
        fast = fast->next;
    }
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }
    slow->next = slow->next->next;
    return dummy.next;
}
```

**Rotate List:** find length, connect tail to head (make it circular), then
break the ring at the correct new point (`length - k % length`).

---

## 4. Pattern: Merge / Splice Two Lists

**Used in:** Merge Two Sorted Lists, Merge k Sorted Lists, Add Two Numbers,
Merge In Between Lists, Partition List.

**Core idea:** always use a dummy node + a "tail" pointer you keep pushing
forward. Never touch the original head pointers directly.

```cpp
ListNode* mergeTwo(ListNode* l1, ListNode* l2) {
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
```

**Merge k Lists:** don't merge one-by-one (slow). Use a min-heap of the k
current heads, pop the smallest, push its `.next`. `O(N log K)`.

**Add Two Numbers:** same dummy+tail template, plus a running `carry`
variable updated each step: `total = v1 + v2 + carry`.

---

## 5. Pattern: Dummy Node + Filtering/Skipping

**Used in:** Remove Linked List Elements, Remove Duplicates (I & II),
Odd Even List, Partition List.

**Core idea:** you're rebuilding the list by deciding, node by node, whether
it stays or goes. Always keep a `prev` pointer to relink around removed
nodes.

```cpp
ListNode* removeElements(ListNode* head, int val) {
    ListNode dummy(0, head);
    ListNode* prev = &dummy;
    ListNode* curr = head;
    while (curr) {
        if (curr->val == val) {
            prev->next = curr->next;  // skip curr
        } else {
            prev = curr;               // advance prev only when kept
        }
        curr = curr->next;
    }
    return dummy.next;
}
```

**Remove Duplicates II (delete ALL nodes with duplicate values):** you need
to detect a *run* of duplicates before deciding to cut the whole run out —
don't just compare `curr` to `curr->next`.

**Odd Even / Partition:** build two separate dummy-headed sublists in one
pass, then join them at the end. This is the go-to trick whenever you need
to "bucket" nodes without extra space.

---

## 6. Quick Pattern-Spotting Table

| If the problem says...                          | Reach for...                        |
|--------------------------------------------------|--------------------------------------|
| "reverse", "in groups of k", "between position m and n" | Iterative pointer reversal |
| "middle", "cycle", "palindrome", "duplicate number" | Slow/fast pointer |
| "nth from end", "rotate by k"                    | Two-pointer gap |
| "merge", "add two numbers", "k sorted lists"     | Dummy + tail splice |
| "remove", "partition", "odd/even", "duplicates"  | Dummy + prev/curr filtering |
| "clone", "random pointer"                        | Interleave-clone-detach (see below) |
| "LRU / LFU cache"                                | Hash map + doubly-linked list |

---

## 7. Two Extra Recipes Worth Memorizing Cold

**Copy List with Random Pointer** (no hash map, O(1) space):
1. Interleave: clone each node right after its original (`A→A'→B→B'`).
2. Set randoms: `A'.random = A.random.next` (since the clone sits right after).
3. Detach: split the interleaved list back into two separate lists.

**LRU Cache:**
- Hash map: `key -> node` for O(1) lookup.
- Doubly-linked list: keeps usage order; move-to-front on access, evict from
  the tail on overflow.
- The doubly-linked list is what makes removal O(1) — you don't need to
  search for the node to unlink it.

---

## 8. Pre-OA Mental Checklist

- [ ] Do I need a **dummy node**? (head might change/get deleted → yes)
- [ ] Am I about to lose a pointer? (save `next` **before** overwriting it)
- [ ] Singly or doubly linked? (doubly lets you go backward — don't
      overcomplicate with extra passes if `.prev` already exists)
- [ ] Any **edge cases**: empty list, single node, list shorter than `k`/`n`?
- [ ] Can I solve this in **O(1) space**? (most of these patterns can —
      interviewers notice if you reach for a hash set/array by default)
- [ ] After coding, **trace on a 3–4 node example by hand** before saying "done".

---

*Companion to your Linked List problem-pattern reference table — this file
is the "how to actually write it" layer underneath that table.*