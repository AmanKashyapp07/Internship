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
 | 1  | Modular Exponentiation                      | Binary Exponentiation (Squaring)  | O(log B) | O(1)     |
 | 2  | Matrix Multiplication Modulo MOD           | 3-Nested Loop Dot Product (K x K) | O(K^3)   | O(K^2)   |
 | 3  | Square Matrix Exponentiation                | Binary Exponentiation on Matrices | O(K^3logP| O(K^2)   |
 | 4  | Number of Paths of Length K on Graph        | Adjacency Matrix Exponentiation   | O(V^3logK| O(V^2)   |
 | 5  | N-th Fibonacci via Matrix Exponentiation    | [[1, 1], [1, 0]] Matrix Power     | O(log N) | O(1)     |
 | 6  | Max Subarray Sum of Length at Most K        | Monotonic Deque on Prefix Sums    | O(N)     | O(N)     |
 | 7  | K-th Lexicographical Permutation            | Factorial Number System (Lehmer)  | O(N^2)   | O(N)     |
 | 8  | Lexicographical Rank of a Permutation       | Factorial Positional Weighting    | O(N^2)   | O(N)     |
 | 9  | Next Greater Permutation                    | 3-Step Suffix Inversion Reversal   | O(N)     | O(1)     |
 | 10 | LCM of Array Elements Modulo MOD            | Prime Factorization + Max Powers  | O(NsqrtV)| O(U)     |
 | 11 | Permutation Rounds (LCM of Cycles)          | Disjoint Cycle Decomposition + LCM| O(N+sqrt)| O(N)     |
 | 12 | Min Arbitrary Swaps to Sort Permutation     | Permutation Cycles (N - cycles)   | O(N)     | O(N)     |
 | 13 | Min Move-to-Anywhere Operations             | Longest Increasing Subseq (N-LIS) | O(N logN)| O(N)     |
 | 14 | Min Move-to-Front Operations                | Greedy Backwards Suffix Matching  | O(N)     | O(1)     |
 | 15 | Repeated Substring Pattern                  | String Doubling: (S+S)[1...2N-2]   | O(N)     | O(N)     |
 | 16 | Min Subarrays with Sum <= K                 | Greedy Running Sum Partitioning   | O(N)     | O(1)     |
 | 17 | K-th Smallest via QuickSelect               | Lomuto Partition QuickSelect      | O(N) avg | O(1)     |
 | 18 | Bidirectional BFS                           | Two-Ended BFS (Smaller Frontier)  | O(B^(D/2)| O(B^(D/2)|
 | 19 | Valid Parentheses with Wildcard '*'         | Greedy Min-Max Bounds (cmin, cmax)| O(N)     | O(1)     |
 | 20 | Count Reverse Pairs (a[i] > x * a[j])       | Modified Merge Sort Counting      | O(N logN)| O(N)     |
 | 21 | Find Median via std::nth_element            | QuickSelect Median via STL        | O(N) avg | O(1)     |
 | 22 | Sort 0s, 1s, and 2s (Dutch National Flag)   | 3-Way In-Place Partitioning       | O(N)     | O(1)     |
 | 23 | Merge Two Sorted Arrays                     | Two-Pointer Merge Step            | O(N + M) | O(N + M) |
 | 24 | Merge Sort                                  | Divide-and-Conquer Merge Sort     | O(N logN)| O(N)     |
 | 25 | Longest Substring with At Least K Frequency | Sliding Window per Unique Target  | O(26 * N)| O(1)     |
 | 26 | 3-Way QuickSort (DNF Partition)             | Dutch National Flag QuickSort     | O(N logN)| O(log N) |
 | 27 | Cyclic Right and Left Rotation              | 3-Step Range Reversal Algorithm   | O(N)     | O(1)     |
 | 28 | Cyclic Sort [1 to N]                        | In-Place Index Mapping Swap       | O(N)     | O(1)     |
 | 29 | Largest Number from Concatenation           | Custom String Comparator Sorting  | O(N logN)| O(N)     |
 | 30 | Running K-th Element Stream                 | Two Heaps (Max-Heap + Min-Heap)   | O(log K) | O(N)     |
 | 31 | Wiggle Sort II                              | Median + DNF 3-Way + Interleaving | O(N)     | O(N)     |
 | 32 | LRU Cache                                   | Hash Map + Doubly Linked List     | O(1)     | O(Cap)   |
 | 33 | LFU Cache                                   | Hash Map + Freq-to-List + minFreq | O(1)     | O(Cap)   |
 | 34 | Gas Station Circuit                         | Greedy Net Surplus & Tank Reset   | O(N)     | O(1)     |
 ====================================================================================================
*/

// =========================================================
// 1. MODULAR EXPONENTIATION
// =========================================================

ll power(ll a, ll b, ll mod = MOD) {
    ll res = 1; a %= mod;
    while (b > 0) { if (b & 1) res = (res * a) % mod; a = (a * a) % mod; b >>= 1; }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute (a^b) % mod efficiently using modular exponentiation.
// - Approach: Binary Exponentiation (Divide and Conquer on power bits).
// - Intuition: Square the base at each step (`a = (a * a) % mod`); multiply into result whenever current bit is 1 (`b & 1`).
// - Complexity: Time: O(log B) bitwise shifts, Space: O(1) auxiliary space.

// =========================================================
// 2. MATRIX MULTIPLICATION MODULO MOD
// =========================================================

vvl multiply(const vvl &A, const vvl &B, int k) {
    vvl C(k, vl(k, 0));
    for (int i = 0; i < k; i++)
        for (int m = 0; m < k; m++)
            for (int j = 0; j < k; j++) C[i][j] = (C[i][j] + A[i][m] * B[m][j]) % MOD;
    return C;
}
// Interview Explanation:
// - Problem Statement: Compute matrix multiplication (A * B) % mod for k x k matrices.
// - Approach: Standard 3-nested loop matrix multiplication ($C = A \times B \pmod{MOD}$).
// - Intuition: Each cell $C[i][j]$ computes the dot product of row $i$ of $A$ and column $j$ of $B$ modulo MOD.
// - Complexity: Time: O(K^3) triple nested loop, Space: O(K^2) for the result matrix.

// =========================================================
// 3. SQUARE MATRIX EXPONENTIATION
// =========================================================

vvl power(vvl A, ll p, int k) {
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; i++) res[i][i] = 1;
    while (p > 0) {
        if (p & 1) res = multiply(res, A, k);
        A = multiply(A, A, k); p >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute (A^p) % mod for a square matrix of size k using binary exponentiation.
// - Approach: Repeated squaring on square matrices with identity matrix base case.
// - Intuition: Generalizes scalar binary exponentiation to matrices; squares $A$ at each bit shift, multiplying when $p & 1$.
// - Complexity: Time: O(K^3 \log P) matrix multiplications, Space: O(K^2) for intermediate products.

// =========================================================
// 4. NUMBER OF PATHS OF LENGTH K ON GRAPH
// =========================================================

ll countPathOfLengthK(vvl &adj, int src, int dest, ll k) {
    return power(adj, k, adj.size())[src][dest];
}
// Interview Explanation:
// - Problem Statement: Count the number of paths of length K from src to dest in a directed graph.
// - Approach: Graph adjacency matrix exponentiation.
// - Intuition: The entry $(Adj^K)[src][dest]$ in an adjacency matrix equals the exact count of walks of length $K$ between nodes.
// - Complexity: Time: O(V^3 \log K) via matrix exponentiation, Space: O(V^2) for adjacency matrix.

// =========================================================
// 5. N-TH FIBONACCI VIA MATRIX EXPONENTIATION
// =========================================================

int calculateNthFiboacci(int n) {
    if (n <= 1) return n;
    vvl F = {{1, 1}, {1, 0}};
    return power(F, n - 1, 2)[0][0];
}
// Interview Explanation:
// - Problem Statement: Compute the N-th Fibonacci number modulo MOD in O(log N) time.
// - Approach: Matrix Exponentiation with Fibonacci transition matrix [[1, 1], [1, 0]].
// - Intuition: Transition evaluates $n$-th Fibonacci in $O(\log n)$ steps via matrix power.
// - Complexity: Time: O(\log N) operations on a 2x2 matrix, Space: O(1) auxiliary space.

// =========================================================
// 6. MAX SUBARRAY SUM OF LENGTH AT MOST K
// =========================================================

ll maxSubarraySumAtMostK(const vi &a, int k) {
    int n = a.size(); vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];
    deque<int> dq = {0}; ll ans = -1e18;
    for (int i = 1; i <= n; i++) {
        while (!dq.empty() && dq.front() < i - k) dq.pop_front();
        ans = max(ans, pref[i] - pref[dq.front()]);
        while (!dq.empty() && pref[dq.back()] >= pref[i]) dq.pop_back();
        dq.push_back(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum subarray sum of length at most K.
// - Approach: Monotonic Deque over prefix sums (Sliding Window Minimum).
// - Intuition: Max subarray sum ending at $i$ is pref[i] - min(pref[j]) for $i - k <= j < i$; deque maintains candidate minimum prefix sums.
// - Complexity: Time: O(N) each index pushed/popped at most once, Space: O(N) for prefix array and deque.

// =========================================================
// 7. K-TH LEXICOGRAPHICAL PERMUTATION
// =========================================================

vi kthPermutation(int n, int k) {
    vi fact(n + 1, 1), nums(n), result;
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    iota(nums.begin(), nums.end(), 1); k--;
    for (int i = n; i >= 1; i--) {
        int idx = k / fact[i - 1];
        result.push_back(nums[idx]);
        nums.erase(nums.begin() + idx);
        k %= fact[i - 1];
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Find the K-th lexicographical permutation of numbers from 1 to N.
// - Approach: Factorial Number System / Greedy Digit Selection (Lehmer code).
// - Intuition: Index $k / (N-1)!$ directly selects the next unused digit, updating $k %= (N-1)!$.
// - Complexity: Time: O(N^2) due to vector deletions, Space: O(N) for digits and factorial lookup.

// =========================================================
// 8. LEXICOGRAPHICAL RANK OF A PERMUTATION
// =========================================================

ll permutationRank(const vi &p) {
    int n = p.size(); vl fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    vi unused(n); iota(unused.begin(), unused.end(), 1);
    ll rank = 0;
    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();
        rank += idx * fact[n - i - 1];
        unused.erase(unused.begin() + idx);
    }
    return rank + 1;
}
// Interview Explanation:
// - Problem Statement: Find the 1-based lexicographical rank of a given permutation.
// - Approach: Factorial Positional Weighting (Lexicographical Rank).
// - Intuition: For each position $i$, count remaining unused elements strictly smaller than $p[i]$ and multiply by $(N - 1 - i)!$.
// - Complexity: Time: O(N^2) using vector search and erase, Space: O(N).

// =========================================================
// 9. NEXT GREATER PERMUTATION
// =========================================================

vi next_permutation(vi &p) {
    int n = p.size(), i = n - 2;
    while (i >= 0 && p[i] >= p[i + 1]) i--;
    if (i >= 0) {
        int j = n - 1;
        while (p[j] <= p[i]) j--;
        swap(p[i], p[j]);
    }
    reverse(p.begin() + i + 1, p.end());
    return p;
}
// Interview Explanation:
// - Problem Statement: Rearrange numbers into the lexicographically next greater permutation in-place.
// - Approach: Standard 3-step lexicographical successor algorithm.
// - Intuition: Find rightmost pivot where $p[i] < p[i+1]$, swap with smallest element to its right greater than $p[i]$, and reverse suffix.
// - Complexity: Time: O(N) single pass and reverse, Space: O(1) in-place.

// =========================================================
// 10. LCM OF ARRAY ELEMENTS MODULO MOD
// =========================================================

ll getLCM(const vi &nums) {
    unordered_map<int, int> max_pwr;
    for (int x : nums) {
        int temp = x;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) { temp /= p; cnt++; }
                max_pwr[p] = max(max_pwr[p], cnt);
            }
        }
        if (temp > 1) max_pwr[temp] = max(max_pwr[temp], 1);
    }
    ll lcm = 1;
    for (auto [prime, pwr] : max_pwr) lcm = (lcm * power(prime, pwr, MOD)) % MOD;
    return lcm;
}
// Interview Explanation:
// - Problem Statement: Compute the LCM of an array of numbers modulo MOD via prime factorization.
// - Approach: Prime Factorization with Maximum Exponent Tracking.
// - Intuition: LCM equals the product of each prime factor raised to its maximum multiplicity across all numbers.
// - Complexity: Time: O(N \sqrt{\max(A)}), Space: O(U) distinct prime factors.

// =========================================================
// 11. PERMUTATION ROUNDS (LCM OF CYCLES)
// =========================================================

int permutationRounds(int n, const vi &a) {
    vector<bool> vis(n + 1, false); vi cycle_len;
    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;
        int cur = i, len = 0;
        while (!vis[cur]) { vis[cur] = true; cur = a[cur]; len++; }
        cycle_len.push_back(len);
    }
    return getLCM(cycle_len);
}
// Interview Explanation:
// - Problem Statement: Find minimum rounds for a permutation to return to original order (LCM of cycle lengths).
// - Approach: Disjoint Cycle Decomposition + LCM of Cycle Lengths.
// - Intuition: Any permutation decomposes into disjoint cycles; full array aligns at LCM of cycle lengths.
// - Complexity: Time: O(N + \sum \sqrt{\text{len}}), Space: O(N).

// =========================================================
// 12. MIN ARBITRARY SWAPS TO SORT PERMUTATION
// =========================================================

int minAnySwaps(const vi &a) {
    int n = a.size(), cycles = 0; vector<bool> vis(n, false);
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        cycles++; int cur = i;
        while (!vis[cur]) { vis[cur] = true; cur = a[cur] - 1; }
    }
    return n - cycles;
}
// Interview Explanation:
// - Problem Statement: Find the minimum arbitrary swaps to sort a 1-indexed permutation.
// - Approach: Permutation Cycle Decomposition.
// - Intuition: Cycle of length $L$ requires $L - 1$ swaps; summing over $C$ cycles yields $N - C$ swaps.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 13. MIN MOVE-TO-ANYWHERE OPERATIONS
// =========================================================

int minMoveAnywhere(const vi &a) {
    vi lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x); else *it = x;
    }
    return (int)a.size() - (int)lis.size();
}
// Interview Explanation:
// - Problem Statement: Find the minimum move-to-anywhere operations needed to sort an array.
// - Approach: Longest Increasing Subsequence (LIS) via Patience Sorting.
// - Intuition: Elements in LIS stay in place; minimum moves required is $N - \text{LIS}$.
// - Complexity: Time: O(N \log N), Space: O(N).

// =========================================================
// 14. MIN MOVE-TO-FRONT OPERATIONS
// =========================================================

int minMoveToFront(const vi &a) {
    int n = a.size(), need = n;
    for (int i = n - 1; i >= 0; i--) if (a[i] == need) need--;
    return need;
}
// Interview Explanation:
// - Problem Statement: Find the minimum move-to-front operations needed to sort a permutation of size N.
// - Approach: Greedy Backwards Suffix Matching from $N$ down to 1.
// - Intuition: Scan right to left tracking expected value ($N, N-1, \dots$); suffix elements never move.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 15. REPEATED SUBSTRING PATTERN
// =========================================================

bool repeatedSubstringPattern(const string &s) {
    return (s + s).substr(1, 2 * s.size() - 2).find(s) != string::npos;
}
// Interview Explanation:
// - Problem Statement: Check if a given string can be constructed by repeating a substring.
// - Approach: String Doubling and Truncation Trick: $(s + s)[1\dots 2n-2]$.
// - Intuition: Searching in $s + s$ with endpoints stripped verifies periodicity.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 16. MIN SUBARRAYS WITH SUM <= K
// =========================================================

int countSubarrays(const vi &nums, int k) {
    int sum = 0, cnt = 1;
    for (int x : nums) {
        sum += x;
        if (sum > k) { sum = x; cnt++; }
        else if (sum == k) { sum = 0; cnt++; }
    }
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Find the minimum continuous subarrays needed with sum not exceeding K.
// - Approach: Greedy Running Sum Partitioning.
// - Intuition: Greedily extend subarray while sum $\le K$; split when sum exceeds $K$.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 17. K-TH SMALLEST VIA QUICKSELECT
// =========================================================

int quickSelect(vi &nums, int k) {
    int l = 0, r = (int)nums.size() - 1;
    while (true) {
        int pivot = nums[r], p = l;
        for (int i = l; i < r; i++) if (nums[i] < pivot) swap(nums[p++], nums[i]);
        swap(nums[p], nums[r]);
        if (p == k) return nums[p];
        if (p < k) l = p + 1; else r = p - 1;
    }
}
// Interview Explanation:
// - Problem Statement: Find the K-th smallest element in an unsorted array using QuickSelect.
// - Approach: QuickSelect with Lomuto Partition Scheme.
// - Intuition: Partition array around pivot; discard irrelevant half iteratively.
// - Complexity: Time: O(N) average, Space: O(1) in-place.

// =========================================================
// 18. BIDIRECTIONAL BFS
// =========================================================

vector<string> getNeighbors(const string &cur) { return {}; }

int bidirectionalBFS(const string &start, const string &target) {
    if (start == target) return 0;
    unordered_set<string> fwd = {start}, bwd = {target}, vis = {start, target};
    int steps = 0;
    while (!fwd.empty() && !bwd.empty()) {
        if (fwd.size() > bwd.size()) swap(fwd, bwd);
        unordered_set<string> nxt_set; steps++;
        for (const string &cur : fwd) {
            for (const string &nxt : getNeighbors(cur)) {
                if (bwd.count(nxt)) return steps;
                if (!vis.count(nxt)) { vis.insert(nxt); nxt_set.insert(nxt); }
            }
        }
        fwd = std::move(nxt_set);
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the shortest transformation path between two strings using Bidirectional BFS.
// - Approach: Bidirectional Breadth-First Search (Two-Ended BFS).
// - Intuition: Expand simultaneously from start and target; always expand the smaller set to reduce search space to $O(B^{D/2})$.
// - Complexity: Time: O(B^{D/2}), Space: O(B^{D/2}).

// =========================================================
// 19. VALID PARENTHESES WITH WILDCARD '*'
// =========================================================

bool checkValidString(const string &s) {
    int cmin = 0, cmax = 0;
    for (char c : s) {
        if (c == '(') { cmin++; cmax++; }
        else if (c == ')') { cmin--; cmax--; }
        else { cmin--; cmax++; }
        if (cmax < 0) return false;
        if (cmin < 0) cmin = 0;
    }
    return cmin == 0;
}
// Interview Explanation:
// - Problem Statement: Check whether a parentheses string containing '(', ')', and '*' is valid.
// - Approach: Range Tracking / Greedy Min-Max Open Parentheses Bounds.
// - Intuition: `cmin` tracks min open parens and `cmax` tracks max; invalid if `cmax < 0`, valid if `cmin == 0` at end.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 20. COUNT REVERSE PAIRS (a[i] > x * a[j])
// =========================================================

int countPairs(vector<int>& a, int x) {
    if (a.size() <= 1) return 0;
    int mid = a.size() / 2;
    vector<int> l(a.begin(), a.begin() + mid), r(a.begin() + mid, a.end());
    int cnt = countPairs(l, x) + countPairs(r, x), j = 0;
    for (int i = 0; i < l.size(); i++) {
        while (j < r.size() && l[i] > 1LL * x * r[j]) j++;
        cnt += j;
    }
    merge(l.begin(), l.end(), r.begin(), r.end(), a.begin());
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Count the number of reverse pairs (i < j with a[i] > x * a[j]) in an array.
// - Approach: Divide and Conquer / Modified Merge Sort Counting (Reverse Pairs).
// - Intuition: For two sorted halves, use two pointers to count pairs where $l[i] > x \cdot r[j]$ in $O(N)$ before merge.
// - Complexity: Time: O(N \log N), Space: O(N).

// =========================================================
// 21. FIND MEDIAN VIA STD::NTH_ELEMENT
// =========================================================

int findMedian(vi &arr) {
    int n = arr.size();
    if (n & 1) {
        nth_element(arr.begin(), arr.begin() + n / 2, arr.end());
        return arr[n / 2];
    }
    nth_element(arr.begin(), arr.begin() + n / 2 - 1, arr.end()); int m1 = arr[n / 2 - 1];
    nth_element(arr.begin(), arr.begin() + n / 2, arr.end()); int m2 = arr[n / 2];
    return (m1 + m2) / 2;
}
// Interview Explanation:
// - Problem Statement: Find the median of an unsorted array in O(N) average time using nth_element.
// - Approach: Quickselect Median Finding via `std::nth_element`.
// - Intuition: `std::nth_element` places elements at target index in linear average time.
// - Complexity: Time: O(N) average, Space: O(1).

// =========================================================
// 22. SORT 0s, 1s, AND 2s (DUTCH NATIONAL FLAG)
// =========================================================

void sort012(vi &arr) {
    int low = 0, mid = 0, high = arr.size() - 1;
    while (mid <= high) {
        if (arr[mid] == 0) swap(arr[low++], arr[mid++]);
        else if (arr[mid] == 1) mid++;
        else swap(arr[mid], arr[high--]);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array of 0s, 1s, and 2s in-place in linear time (Dutch National Flag).
// - Approach: Dutch National Flag 3-Way Partitioning with 3 pointers: `low`, `mid`, `high`.
// - Intuition: Invariant maintains [0...low-1] as 0s, [low...mid-1] as 1s, and [high+1...n-1] as 2s.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 23. MERGE TWO SORTED ARRAYS
// =========================================================

vi merge2sortedarrays(vi& a, vi& b) {
    int n = a.size(), m = b.size(), i = 0, j = 0, k = 0;
    vi merged(n + m);
    while (i < n && j < m) merged[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < n) merged[k++] = a[i++];
    while (j < m) merged[k++] = b[j++];
    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge two sorted arrays into a single sorted array.
// - Approach: Two-Pointer Merge Step (standard Merge Sort subroutine).
// - Intuition: Advance pointers across both sorted arrays, appending smaller head.
// - Complexity: Time: O(N + M), Space: O(N + M).

// =========================================================
// 24. MERGE SORT
// =========================================================

void mergeSort(vi& a, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSort(a, l, mid); mergeSort(a, mid + 1, r);
    vi left(a.begin() + l, a.begin() + mid + 1), right(a.begin() + mid + 1, a.begin() + r + 1);
    vi temp = merge2sortedarrays(left, right);
    copy(temp.begin(), temp.end(), a.begin() + l);
}
// Interview Explanation:
// - Problem Statement: Sort an array using divide-and-conquer Merge Sort.
// - Approach: Classic Divide-and-Conquer Recursive Merge Sort.
// - Intuition: Recursively divide array into halves, then merge sorted halves back using two pointers.
// - Complexity: Time: O(N \log N), Space: O(N).

// =========================================================
// 25. LONGEST SUBSTRING WITH AT LEAST K FREQUENCY
// =========================================================

int longestSubstring(string s, int k) {
    int maxLength = 0, n = s.length();
    for (int uniqueTarget = 1; uniqueTarget <= 26; ++uniqueTarget) {
        vi freq(26, 0);
        int left = 0, right = 0, uniqueCount = 0, countAtLeastK = 0;
        while (right < n) {
            if (uniqueCount <= uniqueTarget) {
                int idx = s[right++] - 'a';
                if (freq[idx]++ == 0) uniqueCount++;
                if (freq[idx] == k) countAtLeastK++;
            } else {
                int idx = s[left++] - 'a';
                if (freq[idx] == k) countAtLeastK--;
                if (--freq[idx] == 0) uniqueCount--;
            }
            if (uniqueCount == uniqueTarget && uniqueCount == countAtLeastK) maxLength = max(maxLength, right - left);
        }
    }
    return maxLength;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest substring where every character frequency is at least k.
// - Approach: Sliding Window constrained by Target Unique Character Count (1 to 26).
// - Intuition: Fixing target unique count creates window monotonicity; expand right, shrink left when count exceeds target.
// - Complexity: Time: O(26 \cdot N) = O(N), Space: O(1).

// =========================================================
// 26. 3-WAY QUICKSORT (DNF PARTITION)
// =========================================================

void dnfPartition(vector<int>& a, int l, int r, int pivot, int& low, int& high) {
    low = l; int mid = l; high = r;
    while (mid <= high) {
        if (a[mid] < pivot) swap(a[low++], a[mid++]);
        else if (a[mid] == pivot) mid++;
        else swap(a[mid], a[high--]);
    }
}

void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int low, high;
    dnfPartition(a, l, r, a[r], low, high);
    quickSort(a, l, low - 1); quickSort(a, high + 1, r);
}
// Interview Explanation:
// - Problem Statement: Sort an array in-place using 3-way QuickSort (efficient for duplicate elements).
// - Approach: 3-Way QuickSort using Dutch National Flag partitioning.
// - Intuition: Partition into regions `< pivot`, `== pivot`, and `> pivot`; duplicate handling avoids quadratic degradation.
// - Complexity: Time: O(N \log N) average, Space: O(\log N) recursion stack.

// =========================================================
// 27. CYCLIC RIGHT AND LEFT ROTATION
// =========================================================

void reverseSubarray(vi& a, int l, int r) {
    while (l < r) swap(a[l++], a[r--]);
}

void RightRotate(vi& a, int l, int r, int k) {
    int n = r - l + 1; k %= n; if (k == 0) return;
    reverseSubarray(a, l, r); reverseSubarray(a, l, l + k - 1); reverseSubarray(a, l + k, r);
}

void LeftRotate(vi& a, int l, int r, int k) {
    int n = r - l + 1; k %= n; if (k == 0) return;
    reverseSubarray(a, l, l + k - 1); reverseSubarray(a, l + k, r); reverseSubarray(a, l, r);
}
// Interview Explanation:
// - Problem Statement: Right/left rotate an array segment from index l to r by k positions in-place.
// - Approach: 3-step reversal algorithm for cyclic rotation.
// - Intuition: Symmetrical 3-pass reverse operations shift target prefix/suffix directly in-place.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 28. CYCLIC SORT [1 TO N]
// =========================================================

void cyclicSort(vi &a) {
    int n = a.size();
    for (int i = 0; i < n; ) {
        if (a[i] != i + 1) swap(a[i], a[a[i] - 1]);
        else i++;
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array containing numbers from 1 to N in O(N) time and O(1) extra space.
// - Approach: Cyclic Sort for numbers in range $[1, N]$.
// - Intuition: Swap element $a[i]$ with $a[a[i] - 1]$ until $a[i] == i + 1$.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 29. LARGEST NUMBER FROM CONCATENATION
// =========================================================

int ConcatenationSort(vi &a) {
    sort(a.begin(), a.end(), [](int x, int y) {
        return to_string(x) + to_string(y) > to_string(y) + to_string(x);
    });
    string s;
    for (int x : a) s += to_string(x);
    return stoi(s);
}
// Interview Explanation:
// - Problem Statement: Arrange non-negative integers such that their concatenation forms the largest number.
// - Approach: Custom comparator sorting based on string concatenation (Largest Number).
// - Intuition: Sort with predicate $(to\_string(x) + to\_string(y)) > (to\_string(y) + to\_string(x))$.
// - Complexity: Time: O(N \log N), Space: O(N).

// =========================================================
// 30. RUNNING K-TH ELEMENT STREAM
// =========================================================

int kthSmallest(vector<int>& a, int k) {
    nth_element(a.begin(), a.begin() + k - 1, a.end()); return a[k - 1];
}

int kthLargest(vector<int>& a, int k) {
    nth_element(a.begin(), a.end() - k, a.end()); return a[a.size() - k];
}

int median(vi &a) {
    int n = a.size();
    if (n % 2 == 1) return kthSmallest(a, n / 2 + 1);
    return (kthSmallest(a, n / 2) + kthSmallest(a, n / 2 + 1)) / 2;
}

int kthElement(const vector<int>& a, const vector<int>& b, int k) {
    if (a.size() > b.size()) return kthElement(b, a, k);
    int n = a.size(), m = b.size(), lo = max(0, k - m), hi = min(k, n);
    while (lo <= hi) {
        int cutA = (lo + hi) / 2, cutB = k - cutA;
        int lA = (cutA == 0) ? INT_MIN : a[cutA - 1], rA = (cutA == n) ? INT_MAX : a[cutA];
        int lB = (cutB == 0) ? INT_MIN : b[cutB - 1], rB = (cutB == m) ? INT_MAX : b[cutB];
        if (lA <= rB && lB <= rA) return max(lA, lB);
        if (lA > rB) hi = cutA - 1; else lo = cutA + 1;
    }
    return -1;
}

double medianOfTwoSortedArrays(const vector<int>& a, const vector<int>& b) {
    int total = a.size() + b.size();
    if (total % 2 == 1) return kthElement(a, b, total / 2 + 1);
    return (kthElement(a, b, total / 2) + kthElement(a, b, total / 2 + 1)) / 2.0;
}

class RunningKth {
    priority_queue<int> left;
    priority_queue<int, vector<int>, greater<int>> right;
    int k;
public:
    RunningKth(int rank) : k(rank) {}
    void add(int x) {
        if (left.empty() || x <= left.top()) left.push(x); else right.push(x);
        while (left.size() > k) { right.push(left.top()); left.pop(); }
        while (left.size() < k && !right.empty()) { left.push(right.top()); right.pop(); }
    }
    int getKth() const { return left.top(); }
};
// Interview Explanation:
// - Problem Statement: Design a data structure that dynamically returns the k-th smallest element added so far.
// - Approach: Two Heaps (Max-Heap `left` holding $k$ smallest, Min-Heap `right` holding the rest).
// - Intuition: Invariant maintains `left` size at $k$; top of `left` is always the $k$-th smallest element.
// - Complexity: Time: O(\log K) for `add`, O(1) for `getKth`, Space: O(N).

// =========================================================
// 31. WIGGLE SORT II
// =========================================================

void wiggleSort(std::vector<int>& nums) {
    int n = nums.size(), mid_idx = (n - 1) / 2;
    std::vector<int> sorted_arr = nums;
    nth_element(sorted_arr.begin(), sorted_arr.begin() + mid_idx, sorted_arr.end());
    int median = sorted_arr[mid_idx], i = 0, j = 0, k = n - 1;
    while (j <= k) {
        if (sorted_arr[j] < median) swap(sorted_arr[i++], sorted_arr[j++]);
        else if (sorted_arr[j] > median) swap(sorted_arr[j], sorted_arr[k--]);
        else j++;
    }
    int left = mid_idx, right = n - 1;
    for (int x = 0; x < n; ++x) nums[x] = (x % 2 == 0) ? sorted_arr[left--] : sorted_arr[right--];
}
// Interview Explanation:
// - Problem Statement: Reorder array into wiggle sort order: nums[0] < nums[1] > nums[2] < nums[3]... (Wiggle Sort II).
// - Approach: Quickselect Median Finding + Dutch National Flag Partition + Interleaving.
// - Intuition: 3-way partition around median, then interleave into even/odd indices backwards.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 32. LRU CACHE & MERGE INTERVALS
// =========================================================

class LRUCache {
    int cap;
    list<pair<int, int>> l;
    unordered_map<int, list<pair<int, int>>::iterator> mp;
public:
    LRUCache(int capacity) : cap(capacity) {}
    int get(int key) {
        if (!mp.count(key)) return -1;
        l.splice(l.begin(), l, mp[key]); return mp[key]->second;
    }
    void put(int key, int value) {
        if (mp.count(key)) {
            mp[key]->second = value; l.splice(l.begin(), l, mp[key]);
        } else {
            if (l.size() == cap) { mp.erase(l.back().first); l.pop_back(); }
            l.emplace_front(key, value); mp[key] = l.begin();
        }
    }
};

vvi mergeIntervals(vvi &intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vvi merged = {intervals[0]};
    for (const auto &interval : intervals) {
        if (merged.back()[1] >= interval[0]) merged.back()[1] = max(merged.back()[1], interval[1]);
        else merged.push_back(interval);
    }
    return merged;
}
// Interview Explanation:
// - Problem Statement: Implement LRU cache with O(1) operations; and merge overlapping intervals.
// - Approach: Hash Map + Doubly Linked List for LRU; Sort + Greedy Linear Scan for intervals.
// - Intuition: Splicing list iterators provides O(1) recency updates; sorting by start time clusters overlaps.
// - Complexity: Time: O(1) LRU ops / O(N log N) intervals, Space: O(Cap) / O(N).

// =========================================================
// 33. LFU CACHE
// =========================================================

class LFUCache {
    int cap, minFreq;
    unordered_map<int, pair<int, int>> kv;
    unordered_map<int, list<int>> freqList;
    unordered_map<int, list<int>::iterator> pos;

    void increaseFreq(int key) {
        int freq = kv[key].second;
        freqList[freq].erase(pos[key]);
        if (freq == minFreq && freqList[freq].empty()) minFreq++;
        kv[key].second++;
        freqList[freq + 1].push_front(key);
        pos[key] = freqList[freq + 1].begin();
    }
public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}
    int get(int key) {
        if (!kv.count(key)) return -1;
        increaseFreq(key); return kv[key].first;
    }
    void put(int key, int value) {
        if (cap == 0) return;
        if (kv.count(key)) { kv[key].first = value; increaseFreq(key); return; }
        if (kv.size() == cap) {
            int keyToRemove = freqList[minFreq].back();
            freqList[minFreq].pop_back(); pos.erase(keyToRemove); kv.erase(keyToRemove);
        }
        kv[key] = {value, 1}; freqList[1].push_front(key); pos[key] = freqList[1].begin(); minFreq = 1;
    }
};
// Interview Explanation:
// - Problem Statement: Design and implement a Least Frequently Used (LFU) Cache with O(1) operations.
// - Approach: Hash Map + Frequency-to-Doubly-Linked-List map + `minFreq` tracker.
// - Intuition: Separate lists per frequency maintain LRU tie-breaking; evict from back of `freqList[minFreq]`.
// - Complexity: Time: O(1) for both `get` and `put`, Space: O(Cap).

// =========================================================
// 34. TOP K, TOP K FREQUENT, STRING UTILS & GAS STATION
// =========================================================

class TopK {
    int k; priority_queue<int, vector<int>, greater<int>> pq;
public:
    TopK(int k) : k(k) {}
    void add(int x) {
        if (pq.size() < k) pq.push(x);
        else if (x > pq.top()) { pq.pop(); pq.push(x); }
    }
    vector<int> getTopK() {
        vector<int> ans; auto temp = pq;
        while (!temp.empty()) { ans.push_back(temp.top()); temp.pop(); }
        reverse(ans.begin(), ans.end()); return ans;
    }
};

class TopKFrequent {
    int k; unordered_map<int, int> freq;
public:
    TopKFrequent(int k) : k(k) {}
    void add(int x) { freq[x]++; }
    vector<int> getTopK() {
        using P = pair<int, int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        for (auto &[x, f] : freq) { pq.push({f, x}); if (pq.size() > k) pq.pop(); }
        vector<int> ans;
        while (!pq.empty()) { ans.push_back(pq.top().second); pq.pop(); }
        reverse(ans.begin(), ans.end()); return ans;
    }
};

int compress(vector<char>& chars) {
    int write = 0;
    for (int i = 0; i < chars.size(); ) {
        char ch = chars[i]; int j = i;
        while (j < chars.size() && chars[j] == ch) j++;
        int count = j - i; chars[write++] = ch;
        if (count > 1) for (char c : to_string(count)) chars[write++] = c;
        i = j;
    }
    return write;
}

int removeDuplicates(vector<int>& nums) {
    int i = 1, n = nums.size();
    for (int j = 1; j < n; j++) if (nums[i - 1] != nums[j]) nums[i++] = nums[j];
    return i;
}

int longestValidParentheses(string s) {
    stack<int> st; st.push(-1); int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') st.push(i);
        else {
            st.pop();
            if (st.empty()) st.push(i);
            else ans = max(ans, i - st.top());
        }
    }
    return ans;
}

class WildcardMatching {
    bool helper(string &s, string &p, int i, int j, vector<vector<int>> &dp) {
        if (i == s.size() && j == p.size()) return true;
        if (j == p.size()) return false;
        if (i == s.size()) {
            for (int k = j; k < p.size(); ++k) if (p[k] != '*') return false;
            return true;
        }
        if (dp[i][j] != -1) return dp[i][j];
        if (p[j] == s[i] || p[j] == '?') return dp[i][j] = helper(s, p, i + 1, j + 1, dp);
        if (p[j] == '*') return dp[i][j] = helper(s, p, i + 1, j, dp) || helper(s, p, i, j + 1, dp);
        return dp[i][j] = false;
    }
public:
    bool isMatch(string s, string p) {
        vector<vector<int>> dp(s.size(), vector<int>(p.size(), -1));
        return helper(s, p, 0, 0, dp);
    }
};

class RegularExpressionMatching {
    vector<vector<int>> dp; string s, p;
    bool dfs(int i, int j) {
        if (j == p.size() && i == s.size()) return true;
        if (j == p.size()) return false;
        if (dp[i][j] != -1) return dp[i][j];
        bool match = i < s.size() && (s[i] == p[j] || p[j] == '.');
        if (j + 1 < p.size() && p[j + 1] == '*') return dp[i][j] = dfs(i, j + 2) || (match && dfs(i + 1, j));
        return dp[i][j] = match && dfs(i + 1, j + 1);
    }
public:
    bool isMatch(string str, string pat) {
        s = str; p = pat; dp.assign(s.size() + 1, vector<int>(p.size() + 1, -1));
        return dfs(0, 0);
    }
};

class GasStationCircuit {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total = 0, tank = 0, start = 0;
        for (int i = 0; i < gas.size(); i++) {
            int gain = gas[i] - cost[i]; total += gain; tank += gain;
            if (tank < 0) { start = i + 1; tank = 0; }
        }
        return total >= 0 ? start : -1;
    }
};
// Interview Explanation:
// - Problem Statement: Top K streaming elements, run-length compression, deduplication, valid parentheses, regex/wildcard matching, and circular gas circuit.
// - Approach: Heaps, two-pointer scans, stack indexing, 2D memoized DFS, and greedy surplus tracking.
// - Intuition: Standard optimal competitive programming patterns for each classic category.
// - Complexity: Time: O(N) to O(N log K), Space: O(1) to O(N).