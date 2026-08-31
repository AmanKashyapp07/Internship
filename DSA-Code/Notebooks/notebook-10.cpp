#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
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
#include <random>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | In-Place Merge Sort (Gap Method)            | Shell-like Gap Shelling           | O(N log^2N)| O(1)   |
 | 2  | Radix Sort MSD (Recursive Bucket Sort)      | Trie-like MSD Prefix Partitioning | O(D(N+B))| O(N + B) |
 | 3  | Pigeonhole Sort (Direct Range Slots)        | Range-Sized Bucket Placement      | O(N + R) | O(N + R) |
 | 4  | Pancake Sorting (LeetCode 969)              | Greedy Suffix Placement (2-Flips) | O(N^2)   | O(N)     |
 | 5  | Patience Sorting & LIS (Greene's Theorem)   | Pile Insertion via lower_bound    | O(N logN)| O(N)     |
 | 6  | Sort Matrix Diagonally (LeetCode 1329)      | Hash Map Diagonal Bucketing       | O(MN logM)| O(M * N)|
 | 7  | Sort Transformed Array (LeetCode 360)       | Parabola Concavity Two-Pointers   | O(N)     | O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. IN-PLACE MERGE SORT (GAP METHOD)
// ============================================================

void inPlaceMerge(vector<int>& arr, int l, int m, int r) {
    int len = r - l + 1;
    int gap = (len + 1) / 2;
    while (gap > 0) {
        int i = l;
        while (i + gap <= r) {
            int j = i + gap;
            if (arr[i] > arr[j]) {
                swap(arr[i], arr[j]);
            }
            i++;
        }
        gap = (gap > 1) ? (gap + 1) / 2 : 0;
    }
}

void inPlaceMergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    inPlaceMergeSort(arr, l, mid);
    inPlaceMergeSort(arr, mid + 1, r);
    inPlaceMerge(arr, l, mid, r);
}
// Interview Explanation:
// - Problem Statement: Merge two sorted adjacent subarrays in-place without allocating O(N) extra buffer memory.
// - Approach: Shell's Gap Reduction Method across boundary.
// - Intuition: Treat the two halves as a single contiguous array and compare elements separated by `gap = ceil(len / 2)`. Halving the gap successively sorts the combined range in O(N log N) merge time.
// - Complexity: Time: O(N log^2 N), Space: O(1) auxiliary.


// ============================================================
// 2. RADIX SORT MSD (RECURSIVE BUCKET SORT)
// ============================================================

void msdRadixSortHelper(vector<string>& arr, int lo, int hi, int d, vector<string>& aux) {
    if (hi <= lo || d >= 100) return;
    const int R = 256;
    vector<int> count(R + 2, 0);

    for (int i = lo; i <= hi; i++) {
        int c = (d < (int)arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
        count[c + 1]++;
    }
    for (int r = 0; r < R + 1; r++) count[r + 1] += count[r];
    for (int i = lo; i <= hi; i++) {
        int c = (d < (int)arr[i].size()) ? (unsigned char)arr[i][d] + 1 : 0;
        aux[lo + count[c]++] = arr[i];
    }
    for (int i = lo; i <= hi; i++) arr[i] = aux[i];

    for (int r = 0; r < R; r++) {
        msdRadixSortHelper(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
    }
}
// Interview Explanation:
// - Problem Statement: Sort variable-length strings or high-precision integers lexicographically using Most Significant Digit (MSD) partitioning.
// - Approach: Recursive Trie-like MSD Bucket Partitioning.
// - Intuition: Partition into 256 character buckets based on the d-th character, then recursively sort each bucket independently.
// - Complexity: Time: O(Total Characters) = O(N * L), Space: O(N + R * Depth).


// ============================================================
// 3. PIGEONHOLE SORT (DIRECT RANGE SLOTS)
// ============================================================

void pigeonholeSort(vector<int>& arr) {
    if (arr.empty()) return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    vector<vector<int>> holes(range);
    for (int x : arr) holes[x - minVal].push_back(x);

    int idx = 0;
    for (int i = 0; i < range; i++) {
        for (int x : holes[i]) arr[idx++] = x;
    }
}
// Interview Explanation:
// - Problem Statement: Sort integer array where range of values R is close to the number of elements N.
// - Approach: Direct Pigeonhole / Slot Mapping.
// - Intuition: Direct array mapping from key to bucket index `val - minVal`, collecting items in order.
// - Complexity: Time: O(N + Range), Space: O(N + Range).


// ============================================================
// 4. PANCAKE SORTING — LeetCode 969
// ============================================================

vector<int> pancakeSort(vector<int>& arr) {
    vector<int> ans;
    int n = arr.size();
    for (int j = n; j > 1; j--) {
        int maxIdx = max_element(arr.begin(), arr.begin() + j) - arr.begin();
        if (maxIdx != j - 1) {
            if (maxIdx != 0) {
                ans.push_back(maxIdx + 1);
                reverse(arr.begin(), arr.begin() + maxIdx + 1);
            }
            ans.push_back(j);
            reverse(arr.begin(), arr.begin() + j);
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Sort array using prefix reversals (pancake flips) with <= 2*N flips (LeetCode 969).
// - Approach: Greedy Suffix Placement (At most 2 flips per element).
// - Intuition: For unsorted prefix of size `sz`, find max element. Flip to index 0, then flip whole range `sz` so max lands at its final position `sz - 1`.
// - Complexity: Time: O(N^2), Space: O(N) flips output.


// ============================================================
// 5. PATIENCE SORTING & LIS (GREENE'S THEOREM)
// ============================================================

int patienceSortLISLength(const vector<int>& arr) {
    vector<int> piles;
    for (int x : arr) {
        auto it = lower_bound(piles.begin(), piles.end(), x);
        if (it == piles.end()) piles.push_back(x);
        else *it = x;
    }
    return piles.size();
}
// Interview Explanation:
// - Problem Statement: Determine the Longest Increasing Subsequence length using Patience Sorting card pile game.
// - Approach: Greedy Card Pile Placement + Binary Search (`std::lower_bound`).
// - Intuition: Place incoming card on the leftmost pile whose top card is >= current card. By Greene's Theorem, the minimum number of piles formed equals the LIS length.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 6. SORT MATRIX DIAGONALLY — LeetCode 1329
// ============================================================

vector<vector<int>> diagonalSort(vector<vector<int>>& mat) {
    int m = mat.size(), n = mat[0].size();
    unordered_map<int, vector<int>> diagonals; // key = i - j

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            diagonals[i - j].push_back(mat[i][j]);
        }
    }
    for (auto& [key, vec] : diagonals) {
        sort(vec.rbegin(), vec.rend()); // Sort descending so we can pop_back in O(1)
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = diagonals[i - j].back();
            diagonals[i - j].pop_back();
        }
    }
    return mat;
}
// Interview Explanation:
// - Problem Statement: Sort each diagonal of an m x n matrix ascending from top-left to bottom-right (LeetCode 1329).
// - Approach: Hash Map Diagonal Grouping with Key `i - j`.
// - Intuition: All cells on the same diagonal share invariant `i - j`. Collect elements per diagonal, sort, and write back in order.
// - Complexity: Time: O(M * N * log(min(M, N))), Space: O(M * N).


// ============================================================
// 7. SORT TRANSFORMED ARRAY — LeetCode 360
// ============================================================

vector<int> sortTransformedArray(vector<int>& nums, int a, int b, int c) {
    auto f = [&](int x) { return a * x * x + b * x + c; };
    int n = nums.size();
    vector<int> res(n);
    int i = 0, j = n - 1;

    if (a >= 0) {
        // Parabola opens upwards: maximums are at extremities
        int k = n - 1;
        while (i <= j) {
            int v1 = f(nums[i]), v2 = f(nums[j]);
            if (v1 >= v2) { res[k--] = v1; i++; }
            else { res[k--] = v2; j--; }
        }
    } else {
        // Parabola opens downwards: minimums are at extremities
        int k = 0;
        while (i <= j) {
            int v1 = f(nums[i]), v2 = f(nums[j]);
            if (v1 <= v2) { res[k++] = v1; i++; }
            else { res[k++] = v2; j--; }
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Given sorted array and coefficients a, b, c, apply f(x) = ax^2 + bx + c and return sorted array in O(N) time (LeetCode 360).
// - Approach: Parabola Convexity / Concavity + Two Pointers.
// - Intuition: If a >= 0 (convex U-shape), maximum values lie at outer endpoints; fill from back. If a < 0 (concave inverted U-shape), minimum values lie at outer endpoints; fill from front.
// - Complexity: Time: O(N), Space: O(N) output array.
