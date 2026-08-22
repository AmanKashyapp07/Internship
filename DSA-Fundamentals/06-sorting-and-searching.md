# Section 06: Sorting & Searching

> **Focus:** Comparison vs. Non-comparison sorting algorithms, the $\Omega(n \log n)$ decision tree lower bound, algorithmic stability and in-place trade-offs, advanced binary search variants, and Binary Search on Answer.

---

# Table of Contents
1. [The $\Omega(n \log n)$ Theoretical Lower Bound for Comparison Sorts](#1-the-theoretical-lower-bound)
2. [Comparison-Based Sorting Algorithms](#2-comparison-based-sorting-algorithms)
3. [Non-Comparison Linear Sorting Algorithms](#3-non-comparison-linear-sorting-algorithms)
4. [Master Sorting Trade-Off Matrix](#4-master-sorting-trade-off-matrix)
5. [Binary Search & Advanced Variants](#5-binary-search--advanced-variants)
6. [Binary Search on Answer (Monotonic Predicates)](#6-binary-search-on-answer)
7. [Quick Recap](#7-quick-recap)

---

# 1. The $\Omega(n \log n)$ Theoretical Lower Bound

A fundamental theorem in computer science states that **no comparison-based sorting algorithm can achieve a worst-case time complexity faster than $\Omega(n \log n)$**.

```
Decision Tree for Sorting 3 Elements [ a, b, c ] (Total Permutations = 3! = 6):

                               [ a < b ? ]
                              /           \
                     (YES)  /               \  (NO)
                     [ b < c ? ]         [ a < c ? ]
                    /          \         /          \
            (YES) /      (NO)    \     (YES) /  (NO)  \
             [a,b,c]    [ a < c ? ]   [b,a,c]   [b,c,a]
                        /         \
                      (YES)       (NO)
                     [a,c,b]     [c,a,b]
```

### Mathematical Proof via Decision Trees:
1. An array of $n$ distinct elements has **$n!$ (n factorial)** possible permutations.
2. A comparison sort can be modeled as a binary decision tree where each internal node is a comparison ($a_i \le a_j$) and each leaf represents a unique sorted permutation.
3. To correctly sort all inputs, the decision tree must have at least $n!$ leaves:
   $$\text{Leaves } L \ge n!$$
4. A binary tree of height $h$ has at most $2^h$ leaves:
   $$2^h \ge L \ge n! \implies h \ge \log_2(n!)$$
5. Using **Stirling's Approximation** ($\ln(n!) \approx n \ln n - n$):
   $$h \ge \log_2(n!) = \sum_{i=1}^n \log_2 i \ge \sum_{i=n/2}^n \log_2(n/2) = \frac{n}{2} \log_2\left(\frac{n}{2}\right) = \mathbf{\Omega(n \log n)}$$
6. Because the height $h$ represents the worst-case number of comparisons, the lower bound is strictly $\mathbf{\Omega(n \log n)}$.

---

# 2. Comparison-Based Sorting Algorithms

### A. Merge Sort (Divide and Conquer)
- **Mechanics:** Recursively divides array into two halves until single elements remain, then merges sorted halves using an auxiliary buffer.
- **Complexity:** **$\Theta(n \log n)$ in Best, Average, and Worst cases**.
- **Space:** **$O(n)$ Auxiliary Space** for the temporary merge buffer + $O(\log n)$ call stack frames.
- **Stability:** **Strictly Stable** (by using `<=` during the merge step, preserving left array elements first).

```cpp
// Merge Step in C++: O(N) Time, O(N) Space
void merge(vector<int>& arr, int l, int mid, int r) {
    vector<int> temp;
    int i = l, j = mid + 1;
    
    while (i <= mid && j <= r) {
        if (arr[i] <= arr[j]) { // '<=' ensures stability!
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);
    
    for (int k = 0; k < temp.size(); k++) {
        arr[l + k] = temp[k];
    }
}
```

---

### B. Quick Sort (Partitioning & Pivot Strategy)
- **Mechanics:** Selects a **Pivot element**, partitions the array such that all elements $\le \text{pivot}$ are on the left and all elements $> \text{pivot}$ are on the right, then recurses on both partitions.
- **Complexity:** **Average: $\Theta(n \log n)$**, **Worst-Case: $O(n^2)$** (when pivots split array into $0$ and $n-1$ elements).
- **Space:** $O(\log n)$ call stack space on average ($O(n)$ worst-case).
- **Stability:** **Unstable** (long-distance swaps disrupt equal key order).

```cpp
// Hoare's Partitioning Scheme (More efficient than Lomuto: 3x fewer swaps)
int hoarePartition(vector<int>& arr, int low, int high) {
    int pivot = arr[low + (high - low) / 2]; // Mid element as pivot
    int i = low - 1, j = high + 1;
    
    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}
```

---

### C. Heap Sort (In-Place Tree Selection)
- **Mechanics:**
  1. Build a Max-Heap from the array in **$O(n)$ linear time** using bottom-up `buildHeap`.
  2. Repeatedly swap root `arr[0]` (maximum element) with the last element `arr[size-1]`, decrement heap size, and call `heapifyDown(0)` ($O(\log n)$).
- **Complexity:** **$\Theta(n \log n)$ in Best, Average, and Worst cases**.
- **Space:** **$O(1)$ Auxiliary Space (Strictly In-Place)**.
- **Stability:** **Unstable** (heap child-parent index mappings alter original positions).

---

### D. Production Hybrid Sorts: Introsort & Timsort
1. **Introsort (`std::sort` in C++ STL):**
   - Starts with **QuickSort** for blazing speed.
   - Monitors recursion depth: if depth exceeds $2 \log_2 n$, it switches automatically to **HeapSort** (preventing the $O(n^2)$ worst-case).
   - Switches to **InsertionSort** for small subarrays ($n \le 16$) to maximize CPU cache locality.
2. **Timsort (Default in Python `sorted()` and Java `Arrays.sort(Object[])`):**
   - Hybrid of **MergeSort** and **InsertionSort**.
   - Identifies natural consecutive sorted runs in data, using insertion sort on small chunks and merging runs with minimal copies.
   - Achieves **$O(n)$ linear time on already-sorted data** and is **strictly Stable**.

---

### Interview Follow-Up Questions
1. **"Why is QuickSort practically faster than MergeSort despite having the same $O(n \log n)$ average complexity?"**
   - *Answer:*
     1. QuickSort sorts in-place with $O(1)$ memory allocation, whereas MergeSort requires allocating and copying an auxiliary $O(n)$ buffer on every merge.
     2. QuickSort displays superior **CPU spatial cache locality** because it iterates sequentially over contiguous array bounds, triggering fewer cache misses.

---

# 3. Non-Comparison Linear Sorting Algorithms

By making structural assumptions about the input data (such as keys being integers within a bounded range), non-comparison algorithms bypass the $\Omega(n \log n)$ decision tree barrier to sort in **$O(n)$ linear time**.

```
+---------------------------------------------------------------------------------------------------+
| ALGORITHM      | ASSUMPTION / RANGE              | TIME COMPLEXITY       | SPACE COMPLEXITY       |
+---------------------------------------------------------------------------------------------------+
| Counting Sort  | Keys are integers in range [0, K]| O(n + K)              | O(n + K)               |
| Radix Sort     | d-digit numbers in base b       | O(d * (n + b))        | O(n + b)               |
| Bucket Sort    | Numbers uniformly in [0.0, 1.0) | Average O(n + K)      | O(n + K)               |
+---------------------------------------------------------------------------------------------------+
```

---

### A. Counting Sort (Frequency Array + Prefix Sums)
Counts the frequency of each distinct value, computes running prefix sums to determine the exact final output index for each key, and iterates backwards over the input array to preserve **Stability**.

```cpp
// Stable Counting Sort for non-negative integers
void countingSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    int maxVal = *max_element(arr.begin(), arr.end());
    
    vector<int> count(maxVal + 1, 0);
    for (int x : arr) count[x]++;
    
    // Prefix sum running accumulation
    for (int i = 1; i <= maxVal; i++) count[i] += count[i - 1];
    
    vector<int> output(n);
    // Iterate BACKWARDS to preserve stability!
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    arr = output;
}
```

---

### B. Radix Sort (LSD vs. MSD)
- **Mechanics:** Sorts multi-digit numbers digit-by-digit from **Least Significant Digit (LSD)** to Most Significant Digit (MSD) using a **Stable Counting Sort** as an intermediate subroutine for each digit.
- **Why Stability is Mandatory:** If sorting by tens place, stability ensures that numbers with equal tens digits retain their previous ones-place relative ordering.

```cpp
// Radix Sort LSD: O(d * (n + 10)) Time
void radixSort(vector<int>& arr) {
    int maxVal = *max_element(arr.begin(), arr.end());
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        // Run Counting Sort on digit: (arr[i] / exp) % 10
    }
}
```

---

### Interview Follow-Up Questions
1. **"When does Counting Sort become worse than MergeSort/QuickSort?"**
   - *Answer:* When the value range $K$ is significantly larger than $N$ (e.g. sorting 10 integers where values range from $1$ to $10^9$). Counting Sort takes $O(n + 10^9)$ time and allocates 4GB of RAM for the count array, while MergeSort sorts it in $10 \log 10 \approx 33$ operations.

---

# 4. Master Sorting Trade-Off Matrix

```
+---------------------------------------------------------------------------------------------------+
| ALGORITHM       | BEST TIME       | AVERAGE TIME    | WORST TIME      | SPACE     | STABLE? | IN-PLACE?|
+---------------------------------------------------------------------------------------------------+
| Insertion Sort  | O(n)            | O(n^2)          | O(n^2)          | O(1)      | YES     | YES      |
| Selection Sort  | O(n^2)          | O(n^2)          | O(n^2)          | O(1)      | NO      | YES      |
| Bubble Sort     | O(n)            | O(n^2)          | O(n^2)          | O(1)      | YES     | YES      |
| Merge Sort      | O(n log n)      | O(n log n)      | O(n log n)      | O(n)      | YES     | NO       |
| Quick Sort      | O(n log n)      | O(n log n)      | O(n^2)          | O(log n)  | NO      | YES      |
| Heap Sort       | O(n log n)      | O(n log n)      | O(n log n)      | O(1)      | NO      | YES      |
| Counting Sort   | O(n + k)        | O(n + k)        | O(n + k)        | O(n + k)  | YES     | NO       |
| Radix Sort      | O(d*(n + b))    | O(d*(n + b))    | O(d*(n + b))    | O(n + b)  | YES     | NO       |
| Timsort         | O(n)            | O(n log n)      | O(n log n)      | O(n)      | YES     | NO       |
+---------------------------------------------------------------------------------------------------+
```

---

# 5. Binary Search & Advanced Variants

**Binary Search** is a divide-and-conquer search algorithm operating on **Monotonically Sorted Arrays** in **$O(\log n)$ Time and $O(1)$ Space**.

```
Avoiding Integer Overflow Bug:
WRONG: int mid = (low + high) / 2;      // If low + high > 2^31 - 1 -> Overflows to negative integer!
RIGHT: int mid = low + (high - low) / 2; // Mathematically equivalent, 100% overflow-safe!
```

---

### A. Lower Bound vs. Upper Bound

```
Array: [ 10, 20, 20, 20, 30, 40 ]
Target = 20

- lower_bound (First element >= target): Points to index 1 (First 20)
- upper_bound (First element > target):  Points to index 4 (Value 30)

Count of target occurrences = upper_bound(20) - lower_bound(20) = 4 - 1 = 3!
```

```cpp
// Lower Bound Implementation (First index where arr[i] >= target)
int lowerBound(const vector<int>& arr, int target) {
    int l = 0, r = arr.size(); // Search space [0, n]
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] >= target) {
            r = mid; // Narrow right bound
        } else {
            l = mid + 1; // Narrow left bound
        }
    }
    return l; // First index >= target
}
```

---

### B. Search in Rotated Sorted Array ($O(\log n)$ Time)
- **Key Insight:** In any rotated sorted array (`[4, 5, 6, 7, 0, 1, 2]`), dividing at `mid` guarantees that **at least one half is always strictly sorted**:
  - If `arr[low] <= arr[mid]`: Left half is sorted $\implies$ Check if target lies within `[arr[low], arr[mid]]`.
  - Otherwise: Right half is sorted $\implies$ Check if target lies within `[arr[mid], arr[high]]`.

```cpp
// Search in Rotated Sorted Array: O(log N) Time
int searchRotated(vector<int>& nums, int target) {
    int l = 0, r = nums.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target) return mid;
        
        // Check if left half is sorted
        if (nums[l] <= nums[mid]) {
            if (target >= nums[l] && target < nums[mid]) r = mid - 1;
            else l = mid + 1;
        } else { // Right half is sorted
            if (target > nums[mid] && target <= nums[r]) l = mid + 1;
            else r = mid - 1;
        }
    }
    return -1;
}
```

---

# 6. Binary Search on Answer

Whenever an optimization problem asks for the **"Minimum maximum"** or **"Maximum minimum"** value such that a condition is satisfied, and the feasibility function $P(x)$ is **Monotonic**:

$$\text{Search Space: } [x_{\min}, x_{\max}] \quad \text{Condition: } [ \text{False, False, False, } \mathbf{True, True, True} ]$$

Binary search on the candidate answer values rather than array indices in **$O(\log(\text{Range}) \times \text{Validation Cost})$ Time**.

```
Classic Pattern Problems:
1. Koko Eating Bananas (Find minimum eating speed K to eat all piles within H hours).
2. Capacity To Ship Packages Within D Days.
3. Aggressive Cows / Allocate Minimum Pages.
```

```cpp
// Classic Template: Koko Eating Bananas
bool canEatInTime(const vector<int>& piles, int speed, int h) {
    long long totalHours = 0;
    for (int p : piles) {
        totalHours += (p + speed - 1) / speed; // ceil(p / speed)
    }
    return totalHours <= h;
}

int minEatingSpeed(vector<int>& piles, int h) {
    int l = 1, r = *max_element(piles.begin(), piles.end());
    int ans = r;
    
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (canEatInTime(piles, mid, h)) {
            ans = mid;     // Feasible! Try to find a smaller valid speed
            r = mid - 1;
        } else {
            l = mid + 1;   // Infeasible! Must increase speed
        }
    }
    return ans;
}
```

---

# 7. Quick Recap

- **Comparison Lower Bound:** $\Omega(n \log n)$ is proven via the binary decision tree height $h \ge \log_2(n!)$.
- **MergeSort vs. QuickSort:** MergeSort is stable and guaranteed $O(n \log n)$ with $O(n)$ space; QuickSort is in-place and faster in practice due to CPU cache lines.
- **Introsort:** C++ `std::sort` starts with QuickSort, switches to HeapSort if recursion depth exceeds $2 \log n$, and uses InsertionSort for small chunks ($n \le 16$).
- **Linear Sort Limits:** Counting, Radix, and Bucket sort achieve $O(n)$ by making integer/digit assumptions, bypassing comparison bounds.
- **Overflow-Safe Midpoint:** Always write `mid = low + (high - low) / 2`.
- **Binary Search on Answer:** Applied to monotonic feasibility predicates ($P(x)$) for min-max problems in $O(\log(\text{Range}) \cdot N)$ time.
