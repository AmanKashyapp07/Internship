# Binary Search
## Master Interview & OA Guide (C++)

---

## How to Use This Guide

Binary search is not just "search a sorted array." The real skill is
recognizing when to apply it and which of the five patterns fits.
Most candidates know Pattern 1. Patterns 3 and 4 (binary search on
the answer) are what separate good candidates from great ones.

---

## Core Idea

Eliminate half the search space at every step.
Requires: a **monotone predicate** — some condition that is false for
a prefix of the search space and true for the rest (or vice versa).

```
Search space:  [F F F F T T T T T]
                         ^
                   first True = answer
```

If you can define such a predicate, you can binary search.

---

## Master Decision Tree

```
Is the array / search space sorted (or can it be sorted)?
  YES ->
    Looking for an exact value?           -> Pattern 1: Classic Binary Search
    Looking for first/last occurrence?    -> Pattern 2: Lower / Upper Bound
    Array rotated at some unknown pivot?  -> Pattern 5: Rotated Array

Can you define: "is X a feasible answer?"
  YES ->
    Minimizing something?  -> Binary search on answer (minimize)
    Maximizing something?  -> Binary search on answer (maximize)
    -> Pattern 3 or 4

Does the problem say "find in O(log n)" or involve a sorted structure?
  YES -> Almost certainly binary search

Does the problem have the phrase "minimum maximum" or "maximum minimum"?
  YES -> Pattern 3: Binary search on answer (minimize the maximum)
```

---

## The Unified Style: Style A (Closed Interval [lo, hi]) with `ans` Variable

Every binary search bug comes from mixing invariant styles.
Instead of memorizing multiple templates, this guide uses a single unified style:
**Style A (closed interval `[lo, hi]`)** with the loop condition `lo <= hi` and an **`ans`** variable to store the best answer found so far.

This approach:
1. Works for classic search, lower/upper bounds, and binary search on answer (both minimize and maximize).
2. Uses the same `mid` formula: `mid = lo + (hi - lo) / 2`.
3. Eliminates all off-by-one errors and infinite loops by always shrinking the search space (`lo = mid + 1` or `hi = mid - 1`).

---

# PATTERN 1: Classic Binary Search

Find an exact target value in a sorted array.

```cpp
// LC 704. Binary Search
int search(vector<int>& nums, int target) {
    int lo = 0, hi = nums.size() - 1;
    int ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2; // avoids integer overflow vs (lo+hi)/2
        if (nums[mid] == target) {
            ans = mid;
            break; // found target, stop searching
        } else if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
```

**Why `mid = lo + (hi - lo) / 2`?**
`(lo + hi)` can overflow when both are large ints. The subtraction form
is always safe.

---

# PATTERN 2: Lower Bound / Upper Bound

Find the **first** position where a condition becomes true.
This is the foundation of almost every non-trivial binary search.

```
Lower bound: first index where nums[i] >= target
Upper bound: first index where nums[i] >  target
```

```cpp
// LC 35. Search Insert Position  (= lower bound)
// First index where nums[i] >= target
int searchInsert(vector<int>& nums, int target) {
    int lo = 0, hi = nums.size() - 1;
    int ans = nums.size(); // default value if target is greater than all elements
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] >= target) {
            ans = mid;       // candidate answer
            hi = mid - 1;    // search left for a smaller index
        } else {
            lo = mid + 1;    // search right
        }
    }
    return ans;
}
```

```cpp
// Upper bound: first index where nums[i] > target
int upperBound(vector<int>& nums, int target) {
    int lo = 0, hi = nums.size() - 1;
    int ans = nums.size(); // default value if no element > target
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > target) {
            ans = mid;       // candidate answer
            hi = mid - 1;    // search left
        } else {
            lo = mid + 1;    // search right
        }
    }
    return ans;
}
```

```cpp
// LC 34. Find First and Last Position
// Count of target = upperBound(target) - lowerBound(target)
vector<int> searchRange(vector<int>& nums, int target) {
    int lo = searchInsert(nums, target);          // lower bound
    if (lo == nums.size() || nums[lo] != target)
        return {-1, -1};
    int hi = upperBound(nums, target) - 1;        // upper bound - 1
    return {lo, hi};
}
```

```cpp
// STL equivalents (use these in contests)
auto it1 = lower_bound(nums.begin(), nums.end(), target); // first >= target
auto it2 = upper_bound(nums.begin(), nums.end(), target); // first >  target
int count = it2 - it1; // number of occurrences
int index = it1 - nums.begin(); // index of lower bound
```

---

# PATTERN 3: Binary Search on Answer (Minimize)

Use when: you cannot directly compute the answer, but you can check
"is X a valid / feasible answer?" and the feasibility is monotone.

**Template:**
```
lo = minimum possible answer
hi = maximum possible answer
Find the SMALLEST x such that feasible(x) is true
```

```cpp
// Generic template
int lo = min_possible, hi = max_possible;
int ans = -1; // store the best feasible value found
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (feasible(mid)) {
        ans = mid;      // mid works; record it
        hi = mid - 1;   // try to find an even smaller feasible answer
    } else {
        lo = mid + 1;   // mid doesn't work; we need a larger value
    }
}
return ans; // smallest feasible value
```

---

```cpp
// LC 875. Koko Eating Bananas
// Minimize eating speed k such that she finishes within h hours
// feasible(k): can Koko eat all piles at speed k in <= h hours?
bool canEat(vector<int>& piles, int k, int h) {
    long long hours = 0;
    for (int p : piles)
        hours += (p + k - 1) / k; // ceil(p / k)
    return hours <= h;
}

int minEatingSpeed(vector<int>& piles, int h) {
    int lo = 1, hi = *max_element(piles.begin(), piles.end());
    int ans = hi;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canEat(piles, mid, h)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
```

```cpp
// LC 1011. Capacity to Ship Packages Within D Days
// Minimize ship capacity such that all packages ship in <= D days
bool canShip(vector<int>& weights, int cap, int days) {
    int d = 1, curr = 0;
    for (int w : weights) {
        if (curr + w > cap) { d++; curr = 0; }
        curr += w;
    }
    return d <= days;
}

int shipWithinDays(vector<int>& weights, int days) {
    int lo = *max_element(weights.begin(), weights.end()); // must fit heaviest
    int hi = 0;
    for (int w : weights) hi += w; // worst case: ship all in one day
    int ans = hi;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canShip(weights, mid, days)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
```

```cpp
// LC 410. Split Array Largest Sum
// Minimize the largest sum among m subarrays
// Same feasibility as shipping: "can we split into <= m parts with max sum <= mid?"
bool canSplit(vector<int>& nums, int m, int maxSum) {
    int parts = 1, curr = 0;
    for (int x : nums) {
        if (curr + x > maxSum) { parts++; curr = 0; }
        curr += x;
    }
    return parts <= m;
}

int splitArray(vector<int>& nums, int m) {
    int lo = *max_element(nums.begin(), nums.end());
    int hi = 0; for (int x : nums) hi += x;
    int ans = hi;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canSplit(nums, m, mid)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
```

```cpp
// LC 2064. Minimized Maximum of Products Distributed to Any Store
// Minimize maximum products per store
bool canDistribute(vector<int>& quantities, int n, int maxQ) {
    int stores = 0;
    for (int q : quantities)
        stores += (q + maxQ - 1) / maxQ; // ceil(q / maxQ) stores needed
    return stores <= n;
}

int minimizedMaximum(int n, vector<int>& quantities) {
    int lo = 1, hi = *max_element(quantities.begin(), quantities.end());
    int ans = hi;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canDistribute(quantities, n, mid)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
```

```cpp
// LC 1283. Find the Smallest Divisor Given a Threshold
bool feasible(vector<int>& nums, int divisor, int threshold) {
    int sum = 0;
    for (int x : nums)
        sum += (x + divisor - 1) / divisor;
    return sum <= threshold;
}

int smallestDivisor(vector<int>& nums, int threshold) {
    int lo = 1, hi = *max_element(nums.begin(), nums.end());
    int ans = hi;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasible(nums, mid, threshold)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
```

---

# PATTERN 4: Binary Search on Answer (Maximize)

Find the **largest** x such that feasible(x) is true.

```cpp
// Generic template (maximize)
int lo = min_possible, hi = max_possible;
int ans = -1; // store the best feasible value found
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2; // no rounding-up (+1) needed here!
    if (feasible(mid)) {
        ans = mid;      // mid works; record it
        lo = mid + 1;   // try to find an even larger feasible answer
    } else {
        hi = mid - 1;   // mid doesn't work; go smaller
    }
}
return ans; // largest feasible value
```

**Note on Simplicity:**
In the traditional `lo < hi` template, we had to use `mid = lo + (hi - lo + 1) / 2` because when `lo + 1 == hi`, if `feasible(lo)` is true, setting `lo = mid` causes an infinite loop.
By using the **`ans` variable and Style A (`lo <= hi`)**, we always increment/decrement (`lo = mid + 1` or `hi = mid - 1`), which guarantees loop termination. Thus, we can use the exact same `lo + (hi - lo) / 2` formula for both minimization and maximization!

```cpp
// LC 1552. Magnetic Force Between Two Balls
// Maximize the minimum distance between any two balls
bool canPlace(vector<int>& pos, int m, int minDist) {
    int count = 1, last = pos[0];
    for (int i = 1; i < pos.size(); i++) {
        if (pos[i] - last >= minDist) {
            count++;
            last = pos[i];
        }
    }
    return count >= m;
}

int maxDistance(vector<int>& position, int m) {
    sort(position.begin(), position.end());
    int lo = 1, hi = position.back() - position.front();
    int ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canPlace(position, m, mid)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
```

```cpp
// LC 2517. Maximum Tastiness of Candy Basket
// Same pattern: maximize minimum difference
bool feasible(vector<int>& price, int k, int minDiff) {
    int count = 1, prev = price[0];
    for (int i = 1; i < price.size(); i++) {
        if (price[i] - prev >= minDiff) {
            count++;
            prev = price[i];
            if (count == k) return true;
        }
    }
    return count >= k;
}

int maximumTastiness(vector<int>& price, int k) {
    sort(price.begin(), price.end());
    int lo = 0, hi = price.back() - price.front();
    int ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasible(price, k, mid)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
```

```cpp
// LC 774. Minimize Max Distance to Gas Station (real-valued binary search)
bool feasible(vector<int>& stations, int k, double maxDist) {
    int added = 0;
    for (int i = 1; i < stations.size(); i++) {
        double gap = stations[i] - stations[i-1];
        added += (int)(gap / maxDist); // stations needed in this gap
    }
    return added <= k;
}

double minmaxGasDist(vector<int>& stations, int k) {
    double lo = 0, hi = stations.back() - stations.front();
    double ans = hi;
    for (int iter = 0; iter < 100; iter++) { // 100 iterations = ~1e-30 precision
        double mid = (lo + hi) / 2;
        if (feasible(stations, k, mid)) {
            ans = mid;
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return ans;
}
```

---

# PATTERN 5: Binary Search in Rotated / Modified Arrays

```cpp
// LC 33. Search in Rotated Sorted Array
// One half is always sorted; use that to decide which half to go into
int search(vector<int>& nums, int target) {
    int lo = 0, hi = nums.size() - 1;
    int ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) {
            ans = mid;
            break;
        }

        if (nums[lo] <= nums[mid]) { // left half is sorted
            if (nums[lo] <= target && target < nums[mid])
                hi = mid - 1;
            else
                lo = mid + 1;
        } else { // right half is sorted
            if (nums[mid] < target && target <= nums[hi])
                lo = mid + 1;
            else
                hi = mid - 1;
        }
    }
    return ans;
}
```

```cpp
// LC 81. Search in Rotated Array II (with duplicates)
// Same idea but handle nums[lo] == nums[mid] by shrinking lo
bool search(vector<int>& nums, int target) {
    int lo = 0, hi = nums.size() - 1;
    bool ans = false;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) {
            ans = true;
            break;
        }
        if (nums[lo] == nums[mid]) { lo++; continue; } // can't determine sorted half

        if (nums[lo] < nums[mid]) {
            if (nums[lo] <= target && target < nums[mid]) hi = mid - 1;
            else lo = mid + 1;
        } else {
            if (nums[mid] < target && target <= nums[hi]) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return ans;
}
```

```cpp
// LC 153. Find Minimum in Rotated Sorted Array
// The minimum is where the rotation point is
int findMin(vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    int ans = nums[0];
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] <= nums[hi]) {
            ans = nums[mid];
            hi = mid - 1; // look for smaller min to the left
        } else {
            lo = mid + 1; // min is in right half
        }
    }
    return ans;
}
```

```cpp
// LC 162. Find Peak Element
// Peak: nums[i] > nums[i-1] and nums[i] > nums[i+1]
// Always move toward the higher side
int findPeakElement(vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    int ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid == nums.size() - 1 || nums[mid] > nums[mid + 1]) {
            ans = mid;
            hi = mid - 1;   // peak is at mid or left
        } else {
            lo = mid + 1;   // peak is to the right
        }
    }
    return ans;
}
```

```cpp
// LC 540. Single Element in a Sorted Array
// Every pair broken after the single element
int singleNonDuplicate(vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    int ans = nums[0];
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        bool partner_matched = false;
        if (mid % 2 == 1) {
            if (mid - 1 >= 0 && nums[mid] == nums[mid - 1]) partner_matched = true;
        } else {
            if (mid + 1 < nums.size() && nums[mid] == nums[mid + 1]) partner_matched = true;
        }
        
        if (partner_matched) {
            lo = mid + 1;
        } else {
            ans = nums[mid];
            hi = mid - 1;
        }
    }
    return ans;
}
```

---

# PATTERN 6: Binary Search on Result in 2D / Matrix

```cpp
// LC 74. Search a 2D Matrix
// Treat the matrix as a flat sorted array
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int rows = matrix.size(), cols = matrix[0].size();
    int lo = 0, hi = rows * cols - 1;
    bool ans = false;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int val = matrix[mid / cols][mid % cols];
        if (val == target) {
            ans = true;
            break;
        } else if (val < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
```

```cpp
// LC 240. Search a 2D Matrix II
// Sorted rows and columns but not fully sorted overall
// Staircase search: start top-right, move left or down
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int r = 0, c = matrix[0].size() - 1;
    bool ans = false;
    while (r < matrix.size() && c >= 0) {
        if (matrix[r][c] == target) {
            ans = true;
            break;
        } else if (matrix[r][c] < target) {
            r++;
        } else {
            c--;
        }
    }
    return ans;
}
```

```cpp
// LC 668. Kth Smallest Number in Multiplication Table
// Binary search on the value; count how many values <= mid
int findKthNumber(int m, int n, int k) {
    int lo = 1, hi = m * n;
    int ans = hi;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int count = 0;
        for (int i = 1; i <= m; i++)
            count += min(mid / i, n); // how many multiples of i are <= mid
        if (count >= k) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
```

---

# PATTERN 7: Binary Search with Auxiliary Data Structure

```cpp
// LC 300. Longest Increasing Subsequence — O(n log n)
// Binary search to find where to place each element in the patience sort piles
int lengthOfLIS(vector<int>& nums) {
    vector<int> tails; // tails[i] = smallest tail of all IS of length i+1
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}
```

```cpp
// LC 354. Russian Doll Envelopes
// Sort by width asc, height desc; then LIS on height
int maxEnvelopes(vector<vector<int>>& envelopes) {
    sort(envelopes.begin(), envelopes.end(), [](auto& a, auto& b) {
        return a[0] == b[0] ? a[1] > b[1] : a[0] < b[0];
    });
    vector<int> tails;
    for (auto& e : envelopes) {
        int h = e[1];
        auto it = lower_bound(tails.begin(), tails.end(), h);
        if (it == tails.end()) tails.push_back(h);
        else *it = h;
    }
    return tails.size();
}
```

---

# Off-By-One: Solved via the `ans` Variable

By using **Style A (closed interval [lo, hi])** with loop condition `lo <= hi` and an `ans` variable:
1. We always use the same `mid` formula: `mid = lo + (hi - lo) / 2` (rounds down).
2. We always update either `lo = mid + 1` or `hi = mid - 1`.
3. Because the search space is strictly reduced in each step, there is **zero risk of infinite loops** or off-by-one errors, even for maximizing answers.

### Why the `ans` Style Avoids Infinite Loops for Maximize:
Without `ans`, in a `lo < hi` loop:
- `lo = 2`, `hi = 3` -> `mid = 2` (rounds down).
- If `feasible(mid)` is true, setting `lo = mid` results in `lo = 2`, which causes an infinite loop.
- To fix this, you would have to round up `mid` using `lo + (hi - lo + 1) / 2`.

With `ans`, in a `lo <= hi` loop:
- `lo = 2`, `hi = 3` -> `mid = 2`.
- If `feasible(mid)` is true, we record `ans = 2` and set `lo = mid + 1 = 3`.
- In the next iteration, `lo = 3`, `hi = 3` -> `mid = 3`.
- If `feasible(3)` is true, we record `ans = 3` and set `lo = mid + 1 = 4`.
- The loop terminates (`lo > hi`), returning `ans = 3`.
- No infinite loops, no complex rounding rules!

---

# Feasibility Function Design Guide

This is the hardest part of binary search on answer problems.

**Step 1:** Identify what you are searching over.
Usually: a capacity, a speed, a distance, a value, a count.

**Step 2:** Define `feasible(x)`:
"Given this value of x, can we satisfy the problem constraints?"

**Step 3:** Verify monotonicity:
If `feasible(x)` is true, is `feasible(x+1)` also true (for minimize)?
If `feasible(x)` is true, is `feasible(x-1)` also true (for maximize)?

**Step 4:** Set boundaries:
`lo` = smallest x where feasible could ever be true
`hi` = largest x that makes sense (often a sum, max element, or given bound)

**Common feasibility patterns:**

```cpp
// "Can we complete the task in at most T time/days/slots?"
// -> Greedily simulate: accumulate until limit reached, count resets
bool canFinish(... , int limit) {
    int groups = 1, curr = 0;
    for (int x : arr) {
        if (curr + x > limit) { groups++; curr = 0; }
        curr += x;
    }
    return groups <= maxGroups;
}

// "Can we place k items with at least minGap between consecutive?"
// -> Greedy: place as early as possible, count placements
bool canPlace(vector<int>& pos, int k, int minGap) {
    int count = 1, last = pos[0];
    for (int i = 1; i < pos.size(); i++) {
        if (pos[i] - last >= minGap) { count++; last = pos[i]; }
    }
    return count >= k;
}

// "Does total cost / rounded-up division stay within threshold?"
bool withinThreshold(vector<int>& arr, int divisor, int threshold) {
    int total = 0;
    for (int x : arr) total += (x + divisor - 1) / divisor;
    return total <= threshold;
}
```

---

# Pattern Recognition Quick-Reference

| Signal in Problem | Pattern | Template (with `ans` variable) |
|---|---|---|
| Sorted array, find exact value | Classic | `lo <= hi`, `ans = mid; break;` |
| First / last occurrence | Lower / Upper Bound | `lo <= hi`, `ans = mid; hi = mid - 1;` |
| "Search insert position" | Lower Bound | `lo <= hi`, `ans = mid; hi = mid - 1;` |
| Count occurrences in sorted array | Both bounds | upper_bound - lower_bound |
| "Minimum speed / capacity / divisor" | BS on Answer (minimize) | `lo <= hi`, if feasible `ans = mid; hi = mid - 1;` |
| "Minimum days / groups / rooms" | BS on Answer (minimize) | `lo <= hi`, if feasible `ans = mid; hi = mid - 1;` |
| "Maximum minimum distance" | BS on Answer (maximize) | `lo <= hi`, if feasible `ans = mid; lo = mid + 1;` |
| "Minimum maximum value" | BS on Answer (minimize) | `lo <= hi`, if feasible `ans = mid; hi = mid - 1;` |
| Array is rotated | Rotated BS | Check which half is sorted |
| Find peak element | Modified BS | if peak candidate, `ans = mid; hi = mid - 1;` |
| Single non-duplicate in sorted | Modified BS | check partner: `lo = mid + 1` or `ans = mid; hi = mid - 1;` |
| 2D matrix fully sorted | Flatten to 1D | `mid/cols`, `mid%cols` with `ans` |
| 2D matrix row+col sorted | Staircase | Start top-right |
| Kth in multiplication table | BS on value + count | count >= k -> `ans = mid; hi = mid - 1;` |
| LIS in O(n log n) | BS + patience sort | lower_bound on tails array |

---

# Complexity Cheat Sheet

| Algorithm | Time | Space |
|---|---|---|
| Classic binary search | O(log n) | O(1) |
| Lower / upper bound | O(log n) | O(1) |
| BS on answer (feasibility O(n)) | O(n log(hi-lo)) | O(1) |
| BS on answer (feasibility O(n log n)) | O(n log n log(hi-lo)) | O(1) |
| LIS with binary search | O(n log n) | O(n) |
| Search in 2D matrix (flat) | O(log(rows*cols)) | O(1) |
| Search in 2D matrix (staircase) | O(rows + cols) | O(1) |
| Kth in multiplication table | O(m log(mn)) | O(1) |

---

# Complete Problem List

### Classic Binary Search
704, 374, 69 (sqrt), 367 (perfect square)

### Lower / Upper Bound
35, 34, 278 (first bad version), 744, 1385

### Binary Search on Answer — Minimize
875, 1011, 410, 1283, 2064, 1870, 1552 (also maximize), 1482,
2187, 1760, 2226, 2576

### Binary Search on Answer — Maximize
1552, 2517, 2439, 774, 1631 (also Dijkstra), 2498

### Rotated / Modified Arrays
33, 81, 153, 154, 162, 540, 852

### 2D Binary Search
74, 240, 668, 378 (also heap), 719

### Binary Search + Auxiliary Structure
300, 354, 315, 493, 1649