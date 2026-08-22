# Section 02: Linear Data Structures

> **Focus:** Core linear memory models, contiguous vs. node-based structures, pointer manipulation, and foundational patterns across Arrays, Strings, Linked Lists, Stacks, and Queues.

---

# Table of Contents
1. [Arrays (Static & Dynamic)](#1-arrays-static--dynamic)
2. [Strings & Memory Models](#2-strings--memory-models)
3. [Linked Lists (Singly, Doubly, Circular)](#3-linked-lists-singly-doubly-circular)
4. [Stacks (LIFO & Monotonic Patterns)](#4-stacks-lifo--monotonic-patterns)
5. [Queues (FIFO, Circular, Deque & Priority Queue)](#5-queues-fifo-circular-deque--priority-queue)
6. [Quick Recap](#6-quick-recap)

---

# 1. Arrays (Static & Dynamic)

An **Array** is a contiguous block of homogeneous memory elements stored sequentially in RAM. Because elements are stored in adjacent memory addresses, any element can be accessed in $O(1)$ constant time using its zero-based index.

```
Memory Address:  0x1000   0x1004   0x1008   0x100C   0x1010
Array Index:      [ 0 ]    [ 1 ]    [ 2 ]    [ 3 ]    [ 4 ]
Value:             10       25       40       55       70
Size per element:  4 bytes (32-bit int)

Address Calculation Formula:
Address(arr[i]) = Base_Address + (i * sizeof(DataType))
Example: Address(arr[3]) = 0x1000 + (3 * 4) = 0x100C  --> Exact O(1) pointer jump!
```

---

### Static vs. Dynamic Arrays

| Feature | Static Array (`int arr[N]`) | Dynamic Array (`std::vector` / ArrayList) |
| :--- | :--- | :--- |
| **Size Allocation** | Fixed at compile-time on the Stack. | Dynamically allocated on the Heap; resizes at runtime. |
| **Resizing Behavior** | Cannot resize; fixed capacity. | Doubles capacity ($N \to 2N$) when full. |
| **Insertion at End** | $O(1)$ (as long as index $< N$). | **Amortized $O(1)$** (Worst-case $O(N)$ during resize). |
| **Insertion/Deletion at Mid** | $O(N)$ (requires shifting elements). | $O(N)$ (requires shifting elements). |
| **Memory Overhead** | Zero overhead (raw contiguous bytes). | Stores pointer, size, and capacity fields (24 bytes in 64-bit). |

---

### Hardware Reality: CPU Cache Locality & Spatial Prefetching
Arrays vastly outperform Linked Lists for iteration due to **Spatial Locality**:
- Modern CPUs do not fetch individual 4-byte integers from RAM; they fetch **64-byte CPU Cache Lines** into L1/L2 caches.
- Loading `arr[0]` automatically pulls `arr[1]` through `arr[15]` into the blazing-fast L1 cache (~1ns latency), enabling hardware prefetchers to anticipate sequential reads with zero cache misses.

#### 2D Array Memory Layout: Row-Major vs. Column-Major
- **C / C++ / Java / Python** use **Row-Major Order**: entire rows are stored contiguously in RAM (`matrix[0][0], matrix[0][1], matrix[0][2]...`).
- **Iterating row-by-row** is up to **10x faster** than iterating column-by-column because column iteration causes a CPU cache miss on every single access!

```cpp
// Fast (Cache-Friendly Row-Major Traversal):
for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
        sum += matrix[r][c]; // Sequential memory access -> 100% cache line hits
    }
}

// Slow (Cache-Hostile Column-Major Traversal):
for (int c = 0; c < cols; c++) {
    for (int r = 0; r < rows; r++) {
        sum += matrix[r][c]; // Stride of (cols * 4 bytes) -> CPU cache miss every read!
    }
}
```

---

### Core Array Algorithmic Patterns
1. **Two Pointers (Opposite Direction):** Squeezing from `left = 0` and `right = n - 1` on sorted arrays (e.g. Two Sum II, Valid Palindrome, Container With Most Water).
2. **Sliding Window:** Expanding and contracting a window `[left, right]` in $O(N)$ time (e.g. Longest Substring Without Repeating Characters, Minimum Size Subarray Sum).
3. **Prefix Sum Array:** Precomputing running sums `prefix[i] = prefix[i-1] + arr[i]` to answer range sum queries $Q(L, R) = \text{prefix}[R] - \text{prefix}[L-1]$ in **$O(1)$ time**.

```cpp
// Prefix Sum Pattern: O(N) precomputation, O(1) range queries
vector<int> buildPrefix(const vector<int>& arr) {
    int n = arr.size();
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + arr[i];
    }
    return prefix;
}
// Sum in range [L, R] (0-indexed inclusive) = prefix[R + 1] - prefix[L]
```

---

### Interview Follow-Up Questions
1. **"Why does deleting an element from an array take $O(N)$ time?"**
   - *Answer:* Because memory must remain contiguous with no gaps. Deleting `arr[i]` requires shifting all subsequent $N - 1 - i$ elements one position to the left.
2. **"What is the difference between Array `size` and `capacity`?"**
   - *Answer:* `size` is the number of active elements currently stored; `capacity` is the total number of elements the allocated heap buffer can hold before another memory reallocation and copy is triggered.
3. **"Can an array hold elements of different data types in C++?"**
   - *Answer:* No, standard C++ arrays are homogeneous because $O(1)$ indexing relies on every element occupying an identical byte size (`Base + i * sizeof(T)`). Heterogeneous storage requires wrappers like `std::variant` or pointer arrays (`void*` / `std::any`).

---

# 2. Strings & Memory Models

A **String** is fundamentally an array of characters (`char`), terminated by a null byte `'\0'` in C/C++ or stored with an explicit length header in modern languages.

```
C-Style String in Memory: "HELLO"
Address: 0x2000  0x2001  0x2002  0x2003  0x2004  0x2005
Byte:    ['H']   ['E']   ['L']   ['L']   ['O']   ['\0']  (6 bytes total)
```

---

### String Immutability vs. Mutability Across Languages

| Language | Mutability | Internal Memory Mechanism |
| :--- | :--- | :--- |
| **C++ (`std::string`)** | **Mutable** | Modifying `s[0] = 'h'` alters the byte buffer directly in-place in $O(1)$ time. |
| **Java (`String`)** | **Immutable** | Stored in the **String Constant Pool**. Any concatenation (`s += "a"`) allocates a brand-new object on the Heap and copies all previous characters ($O(N)$ per concatenation). Use `StringBuilder` for $O(1)$ amortized appends. |
| **Python (`str`)** | **Immutable** | Strings cannot be mutated in place; string concatenation in loops produces $O(N^2)$ runtime unless using `''.join(list)`. |
| **JavaScript (`string`)** | **Immutable** | String primitives are immutable. Substring methods return new string primitives or internal sliced string pointers in V8. |

---

### C++ Small String Optimization (SSO)
- Modern C++ standard libraries (libc++, libstdc++) implement **Small String Optimization (SSO)**:
  - If a string has **15 or fewer characters** (22 bytes in some implementations), `std::string` does not allocate memory on the Heap!
  - Instead, it stores the characters directly inside the 24-byte `std::string` stack object itself (using a union overlapping the capacity and pointer fields), completely eliminating expensive `malloc`/`free` heap allocations for short strings.

```cpp
// Demonstrating SSO:
std::string shortStr = "Hello"; // Stored entirely on the Stack (0 Heap allocations)
std::string longStr = "This is a very long string exceeding 15 characters"; // Allocates on Heap
```

---

### Key String Interview Algorithms & Complexity

| Algorithm / Pattern | Primary Use Case | Time Complexity | Space Complexity |
| :--- | :--- | :---: | :---: |
| **Two Pointers Reversal** | Reverse string, palindrome verification | $O(N)$ | $O(1)$ |
| **Sliding Window (Frequency Map)**| Longest substring with K distinct chars | $O(N)$ | $O(\Sigma) = O(1)$ for 26/128 chars |
| **KMP (Knuth-Morris-Pratt)** | Substring search with LPS (Longest Proper Prefix which is Suffix) table | $O(N + M)$ | $O(M)$ |
| **Rabin-Karp** | Substring search via Rolling Polynomial Hashing | Average $O(N + M)$, Worst $O(NM)$ | $O(1)$ |

```cpp
// Standard Sliding Window on Strings (Longest Substring Without Repeating Characters)
int lengthOfLongestSubstring(string s) {
    vector<int> lastIndex(256, -1);
    int maxLen = 0, left = 0;
    
    for (int right = 0; right < s.length(); right++) {
        char c = s[right];
        if (lastIndex[c] >= left) {
            left = lastIndex[c] + 1; // Shrink window past duplicate
        }
        lastIndex[c] = right;
        maxLen = max(maxLen, right - left + 1); // Expand window
    }
    return maxLen;
}
```

---

### Interview Follow-Up Questions
1. **"Why does repeatedly appending characters in a loop take $O(N^2)$ time in Java/Python?"**
   - *Answer:* Because strings are immutable. Appending character $i$ requires allocating a new string buffer of size $i$ and copying all $i-1$ previous characters. Summing $1 + 2 + 3 + \dots + N = \frac{N(N+1)}{2} = O(N^2)$.
2. **"How does Rabin-Karp's Rolling Hash achieve $O(1)$ hash updates?"**
   - *Answer:* By subtracting the outgoing character's high-order hash contribution, multiplying the remaining hash by base $B$, and adding the new incoming character modulo prime $P$:
     $$\text{Hash}_{\text{new}} = \left( (\text{Hash}_{\text{old}} - c_{\text{out}} \cdot B^{M-1}) \cdot B + c_{\text{in}} \right) \pmod P$$

---

# 3. Linked Lists (Singly, Doubly, Circular)

A **Linked List** is a linear data structure where elements (nodes) are stored non-contiguously in heap memory. Each node contains a data payload and one or more pointers referencing adjacent nodes.

```
1. Singly Linked List:
[ Head: 10 | next ] ---> [ 20 | next ] ---> [ 30 | next ] ---> NULL

2. Doubly Linked List:
NULL <--- [ prev | 10 | next ] <---> [ prev | 20 | next ] <---> [ prev | 30 | next ] ---> NULL

3. Circular Linked List:
+---> [ 10 | next ] ---> [ 20 | next ] ---> [ 30 | next ] ---+
|                                                             |
+-------------------------------------------------------------+
```

---

### Linked List vs. Array Trade-Off Comparison

| Operation | Array | Linked List | Mechanical Reasoning |
| :--- | :---: | :---: | :--- |
| **Access by Index `[i]`** | **$O(1)$** | **$O(N)$** | Array does instant pointer math; List must traverse $i$ nodes sequentially. |
| **Insert / Delete at Head**| $O(N)$ | **$O(1)$** | Array must shift all elements; List updates 1 pointer (`newNode->next = head`). |
| **Insert / Delete at Known Node**| $O(N)$ | **$O(1)$** | List merely rewires pointers ($O(1)$ in Doubly Linked List). |
| **Cache Friendliness** | **High** | **Low** | List nodes are scattered across Heap pages, causing CPU cache misses on every hop. |
| **Memory Overhead** | **Low** | **High** | List nodes require 8 bytes (Singly) or 16 bytes (Doubly) per node just for pointer metadata. |

---

### The Power of Sentinel (Dummy) Nodes
In pointer-heavy coding interviews, initializing a **Dummy Head Node** (`ListNode dummy(0); dummy.next = head;`) eliminates 90% of edge-case bugs:
- No special conditional checks for deleting or inserting at the true head.
- No need to update the `head` pointer if the first element is removed.

```cpp
// Reversing a Singly Linked List in-place (O(N) Time, O(1) Space)
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextTemp = curr->next; // 1. Save next node
        curr->next = prev;               // 2. Reverse pointer
        prev = curr;                     // 3. Move prev forward
        curr = nextTemp;                 // 4. Move curr forward
    }
    return prev; // New head of reversed list
}
```

---

### Fast & Slow Pointers (Floyd's Cycle-Finding Algorithm)
Using two pointers moving at different speeds (`slow` moves 1 step, `fast` moves 2 steps):
1. **Finding Middle of List:** When `fast` reaches the end (`NULL`), `slow` is guaranteed to be at the exact midpoint $\lfloor N/2 \rfloor$.
2. **Cycle Detection:** If a cycle exists, `fast` will inevitably lap `slow` and they will meet inside the loop in $O(N)$ time.
3. **Finding Cycle Start Node:** Once `slow` and `fast` meet, reset `slow = head`. Move both `slow` and `fast` at **1 step per turn**; the exact node where they collide is the cycle entry point.

```
Floyd's Cycle Entry Proof:
Let L = distance from Head to Cycle Start.
Let C = Cycle Length.
Let d = distance from Cycle Start to Meeting Point.

Distance traveled by Slow = L + d
Distance traveled by Fast = L + d + k*C
Since Fast is twice as fast: 2(L + d) = L + d + k*C  ==>  L + d = k*C  ==>  L = k*C - d

Therefore: Starting one pointer at Head and one at Meeting Point, moving both at 1 step/sec,
they will meet at the Cycle Start after exactly L steps!
```

```cpp
// Floyd's Cycle Detection & Cycle Entry Point
ListNode* detectCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { // Cycle detected!
            ListNode* entry = head;
            while (entry != slow) {
                entry = entry->next;
                slow = slow->next;
            }
            return entry; // Cycle start node
        }
    }
    return nullptr; // No cycle
}
```

---

### Interview Follow-Up Questions
1. **"How is an LRU Cache implemented using a Doubly Linked List and Hash Map?"**
   - *Answer:* The Hash Map stores `key -> Node*` for $O(1)$ lookups. The Doubly Linked List maintains access recency: newly accessed nodes are moved to the Head in $O(1)$ pointer swaps, and the least recently used node at the Tail is evicted in $O(1)$ time when capacity is exceeded.
2. **"Can you delete a node in a Singly Linked List in $O(1)$ time given only a pointer to that node (no head pointer)?"**
   - *Answer:* Yes, copy the value of `node->next` into `node`, then delete `node->next` by updating pointers (`node->next = node->next->next`). *Caveat:* Fails if the target node is the final tail node.

---

# 4. Stacks (LIFO & Monotonic Patterns)

A **Stack** is an abstract linear data structure operating under the **LIFO (Last-In, First-Out)** principle: the most recently added element is the first one removed.

```
Stack Operations (Top of Stack):
Push(40) ---> |  40  |  <--- TOP (Index: 3)
              |  30  |
              |  20  |
Pop()    <--- |  10  |  (Index: 0)
              +------+
```

---

### Implementation Backing: Dynamic Array vs. Linked List

| Feature | Dynamic Array Backed (`std::vector`) | Linked List Backed (`std::list`) |
| :--- | :--- | :--- |
| **`push()` Time** | **Amortized $O(1)$** | **Strict $O(1)$** |
| **`pop()` Time** | **$O(1)$** | **$O(1)$** |
| **Cache Locality** | **High** (Contiguous array memory). | **Low** (Pointer chasing on Heap). |
| **Memory Overhead** | Low (only unused capacity). | High (8-byte pointer per stack item). |

---

### The Monotonic Stack Pattern
A **Monotonic Stack** is a stack whose elements are strictly monotonically increasing or monotonically decreasing.
- **When to Use:** Whenever a problem asks for the **Next Greater Element**, **Previous Greater Element**, **Stock Span**, or **Largest Rectangle in Histogram**.
- **Time Complexity:** Every element is pushed onto the stack once and popped from the stack at most once $\implies$ **Strictly $O(N)$ Total Time Complexity** across the entire loop!

```cpp
// Next Greater Element to the Right (O(N) Time, O(N) Space)
vector<int> nextGreaterElement(const vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n, -1);
    stack<int> st; // Stores indices of elements
    
    for (int i = 0; i < n; i++) {
        // While current element is greater than element at stack top
        while (!st.empty() && nums[i] > nums[st.top()]) {
            int topIdx = st.top();
            st.pop();
            result[topIdx] = nums[i]; // Found next greater element!
        }
        st.push(i);
    }
    return result;
}
```

---

### Expression Evaluation & Bracket Matching
Stacks are the native engine for compiler syntax parsing, balancing parentheses, and evaluating arithmetic expressions:
- **Infix to Postfix Conversion (Shunting-Yard Algorithm)**: Uses an operator stack to enforce mathematical precedence (`*`, `/` over `+`, `-`).
- **Evaluating Postfix (Reverse Polish Notation)**: Push operands to stack; when an operator is encountered, pop 2 operands, apply operation, and push result back.

---

### Interview Follow-Up Questions
1. **"How do you design a Stack that supports `push()`, `pop()`, and `getMin()` in $O(1)$ time and $O(1)$ auxiliary space?"**
   - *Answer:* Store modified encoded values. When pushing a value $x < \text{currentMin}$, push `2*x - currentMin` to the stack and update `currentMin = x`. When popping a value $y < \text{currentMin}$, restore the previous minimum via `currentMin = 2*currentMin - y`.
2. **"How do you implement a Stack using 2 Queues?"**
   - *Answer:* On `push(x)`: Enqueue $x$ into $Q_2$, dequeue all elements from $Q_1$ into $Q_2$, then swap names of $Q_1$ and $Q_2$. `pop()` and `top()` become direct $O(1)$ dequeues from $Q_1$.

---

# 5. Queues (FIFO, Circular, Deque & Priority Queue)

A **Queue** is an abstract linear data structure operating under the **FIFO (First-In, First-Out)** principle: elements are inserted at the Rear (Tail) and removed from the Front (Head).

```
Simple Queue:
Enqueue(40) ---> [ Rear: 40 | 30 | 20 | Front: 10 ] ---> Dequeue() returns 10
```

---

### Queue Variations Comparison

```
+-----------------------------------------------------------------------------------------+
| 1. SIMPLE QUEUE                                                                         |
| Backed by Array or Linked List. Enqueue at Tail, Dequeue at Head.                       |
+-----------------------------------------------------------------------------------------+
| 2. CIRCULAR QUEUE                                                                       |
| Solves Array Queue space waste. Uses modulo arithmetic: nextIndex = (i + 1) % Capacity. |
+-----------------------------------------------------------------------------------------+
| 3. DOUBLE-ENDED QUEUE (DEQUE)                                                           |
| Supports O(1) push_front, push_back, pop_front, and pop_back.                           |
+-----------------------------------------------------------------------------------------+
| 4. PRIORITY QUEUE                                                                       |
| Elements have priority scores. Highest priority element is dequeued first via Heap O(logN)|
+-----------------------------------------------------------------------------------------+
```

---

### Circular Queue Array Implementation
In a naive array-backed queue, dequeuing leaves empty dead space at the front of the array. A **Circular Queue** wraps pointers around using modulo arithmetic:

```cpp
class MyCircularQueue {
    vector<int> data;
    int head, tail, size, capacity;
public:
    MyCircularQueue(int k) : capacity(k), head(0), tail(-1), size(0), data(k) {}
    
    bool enQueue(int value) {
        if (isFull()) return false;
        tail = (tail + 1) % capacity; // Wrap-around pointer math
        data[tail] = value;
        size++;
        return true;
    }
    
    bool deQueue() {
        if (isEmpty()) return false;
        head = (head + 1) % capacity; // Advance head wrap-around
        size--;
        return true;
    }
    
    int Front() { return isEmpty() ? -1 : data[head]; }
    int Rear() { return isEmpty() ? -1 : data[tail]; }
    bool isEmpty() { return size == 0; }
    bool isFull() { return size == capacity; }
};
```

---

### Monotonic Deque Pattern: Sliding Window Maximum in $O(N)$ Time
Finding the maximum element in every sliding window of size $K$ naively takes $O(N \cdot K)$ time. A **Monotonic Decreasing Deque** solves this in **$O(N)$ linear time**:
- Store array indices in the deque such that values are kept in strictly decreasing order (`deque.front()` always holds the maximum of the current window).
- Before inserting `nums[i]`, pop all elements from the back of the deque smaller than `nums[i]`.
- Remove indices from the front that fall outside the current window (`idx <= i - k`).

```cpp
// Sliding Window Maximum in O(N) Time using Monotonic Deque
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; // Stores indices
    vector<int> result;
    
    for (int i = 0; i < nums.size(); i++) {
        // 1. Remove indices out of current window
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // 2. Maintain decreasing monotonicity: remove smaller elements from back
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        // 3. Add current element index
        dq.push_back(i);
        
        // 4. Record maximum once first window of size k is formed
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}
```

---

### Interview Follow-Up Questions
1. **"How do you implement a Queue using 2 Stacks with amortized $O(1)$ operations?"**
   - *Answer:* Maintain `inputStack` and `outputStack`. `push(x)` pushes directly to `inputStack` in $O(1)$. `pop()` pops from `outputStack`; if `outputStack` is empty, it drains all elements from `inputStack` into `outputStack` (reversing their order to FIFO). Every element moves between stacks exactly once $\implies$ **Amortized $O(1)$ per operation**.
2. **"Why does a BFS (Breadth-First Search) traversal require a Queue rather than a Stack?"**
   - *Answer:* A Queue's FIFO property guarantees that all nodes at depth $D$ are visited and processed before any node at depth $D + 1$ is explored, ensuring the shortest path is found in unweighted graphs.

---

# 6. Quick Recap

- **Array Indexing:** $O(1)$ random access is achieved via continuous pointer arithmetic (`Base + i * sizeof(T)`).
- **CPU Cache Locality:** Contiguous arrays take full advantage of 64-byte CPU cache lines; linked lists cause cache misses due to scattered heap node allocations.
- **Row-Major Traversal:** Iterating 2D arrays row-by-row matches contiguous RAM layout, running up to 10x faster than column-by-column iteration.
- **Floyd's Cycle Finding:** Fast and slow pointers detect linked list cycles in $O(N)$ time and find the exact cycle entry node with $O(1)$ memory.
- **Monotonic Stack / Deque:** Guarantees $O(N)$ linear time for Next Greater Element and Sliding Window Maximum problems because each index is pushed/popped at most once.
- **Queue mod math:** Circular queues prevent memory drift in fixed arrays using `(index + 1) % capacity`.
