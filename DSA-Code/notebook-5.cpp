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

// Modular exponentiation (a^b) % mod
// Time: O(log B), Space: O(1)

ll power(ll a, ll b, ll mod = MOD) {
    ll res = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute (a^b) % mod efficiently using modular exponentiation.
// - Approach: Binary Exponentiation (Divide and Conquer on power bits).
// - Intuition: Square the base at each step (`a = (a * a) % mod`); multiply into result whenever current bit is 1 (`b & 1`).
// - Complexity: Time: O(log B) bitwise shifts, Space: O(1) auxiliary space.

// Matrix multiplication modulo MOD
// Time: O(K^3), Space: O(K^2)

vvl multiply(const vvl &A, const vvl &B, int k) {
    vvl C(k, vl(k, 0));
    for (int i = 0; i < k; i++) {
        for (int m = 0; m < k; m++) {
            for (int j = 0; j < k; j++) {
                C[i][j] = (C[i][j] + A[i][m] * B[m][j]) % MOD;
            }
        }
    }
    return C;
}
// Interview Explanation:
// - Problem Statement: Compute matrix multiplication (A * B) % mod for k x k matrices.
// - Approach: Standard 3-nested loop matrix multiplication ($C = A \times B \pmod{MOD}$).
// - Intuition: Each cell $C[i][j]$ computes the dot product of row $i$ of $A$ and column $j$ of $B$ modulo MOD.
// - Complexity: Time: O(K^3) triple nested loop, Space: O(K^2) for the result matrix.

// Square matrix exponentiation (A^p) % MOD
// Time: O(K^3 log P), Space: O(K^2)

vvl power(vvl A, ll p, int k) {
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; i++)
        res[i][i] = 1;

    while (p > 0) {
        if (p & 1)
            res = multiply(res, A, k);
        A = multiply(A, A, k);
        p >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute (A^p) % mod for a square matrix of size k using binary exponentiation.
// - Approach: Repeated squaring on square matrices with identity matrix base case.
// - Intuition: Generalizes scalar binary exponentiation to matrices; squares $A$ at each bit shift, multiplying when $p & 1$.
// - Complexity: Time: O(K^3 \log P) matrix multiplications, Space: O(K^2) for intermediate products.

// Number of paths of length K from src to dest using matrix exponentiation
// Time: O(V^3 log K), Space: O(V^2)

ll countPathOfLengthK(vvl &adj, int src, int dest, ll k) {
    return power(adj, k, adj.size())[src][dest];
}
// Interview Explanation:
// - Problem Statement: Count the number of paths of length K from src to dest in a directed graph.
// - Approach: Graph adjacency matrix exponentiation.
// - Intuition: The entry $(Adj^K)[src][dest]$ in an adjacency matrix equals the exact count of walks of length $K$ between nodes.
// - Complexity: Time: O(V^3 \log K) via matrix exponentiation, Space: O(V^2) for adjacency matrix.

// N-th Fibonacci number modulo MOD via matrix exponentiation
// Time: O(log N), Space: O(1)

int calculateNthFiboacci(int n) {
    if (n <= 1)
        return n;
    vvl F = {{1, 1}, {1, 0}};
    return power(F, n - 1, 2)[0][0];
}
// Interview Explanation:
// - Problem Statement: Compute the N-th Fibonacci number modulo MOD in O(log N) time.
// - Approach: Matrix Exponentiation with Fibonacci transition matrix $\begin{pmatrix}1 & 1 \\ 1 & 0\end{pmatrix}$.
// - Intuition: Transition $\begin{pmatrix}F_n \\ F_{n-1}\end{pmatrix} = \begin{pmatrix}1 & 1 \\ 1 & 0\end{pmatrix}^{n-1} \begin{pmatrix}F_1 \\ F_0\end{pmatrix}$ evaluates $n$-th Fibonacci in $O(\log n)$ steps.
// - Complexity: Time: O(\log N) operations on a $2 \times 2$ matrix, Space: O(1) auxiliary space.

// Maximum subarray sum of length at most K using deque prefix sum
// Time: O(N), Space: O(N)

ll maxSubarraySumAtMostK(const vi &a, int k) {
    int n = a.size();
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + a[i];
    }

    deque<int> dq = {0};
    ll ans = -1e18;

    for (int i = 1; i <= n; i++) {
        while (!dq.empty() && dq.front() < i - k)
            dq.pop_front();
        ans = max(ans, pref[i] - pref[dq.front()]);
        while (!dq.empty() && pref[dq.back()] >= pref[i])
            dq.pop_back();
        dq.push_back(i);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum subarray sum of length at most K.
// - Approach: Monotonic Deque over prefix sums (Sliding Window Minimum).
// - Intuition: Max subarray sum ending at $i$ is $\text{pref}[i] - \min(\text{pref}[j])$ for $i - k \le j < i$; deque maintains indices of candidate minimum prefix sums.
// - Complexity: Time: O(N) since each index is pushed/popped at most once, Space: O(N) for prefix array and deque.

// K-th lexicographical permutation of numbers 1 to N (1-indexed K)
// Time: O(N^2), Space: O(N)

vi kthPermutation(int n, int k) {
    vi fact(n + 1, 1), nums(n), result;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i;
    iota(nums.begin(), nums.end(), 1);

    k--;
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
// - Intuition: Partition permutations into $N$ blocks of size $(N-1)!$; index $k / (N-1)!$ directly selects the next unused digit, updating $k = k \pmod{(N-1)!}$.
// - Complexity: Time: O(N^2) due to vector deletions, Space: O(N) for digits and factorial lookup.

// 1-based lexicographical rank of a permutation
// Time: O(N^2), Space: O(N)

ll permutationRank(const vi &p) {
    int n = p.size();
    vl fact(n + 1, 1);
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i;

    vi unused(n);
    iota(unused.begin(), unused.end(), 1);

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
// - Intuition: For each position $i$, count remaining unused elements strictly smaller than $p[i]$ and multiply by $(N - 1 - i)!$ permutations.
// - Complexity: Time: O(N^2) using vector search and erase (optimizable to $O(N \log N)$ with Fenwick tree), Space: O(N).

// Next greater permutation in-place
// Time: O(N), Space: O(1)

vi next_permutation(vi &p) {
    int n = p.size(), i = n - 2;
    while (i >= 0 && p[i] >= p[i + 1])
        i--;

    if (i >= 0) {
        int j = n - 1;
        while (p[j] <= p[i])
            j--;
        swap(p[i], p[j]);
    }
    reverse(p.begin() + i + 1, p.end());
    return p;
}
// Interview Explanation:
// - Problem Statement: Rearrange numbers into the lexicographically next greater permutation in-place.
// - Approach: Standard 3-step lexicographical successor algorithm.
// - Intuition: Find rightmost pivot where $p[i] < p[i+1]$, swap with smallest element to its right greater than $p[i]$, and reverse suffix $p[i+1\dots n-1]$.
// - Complexity: Time: O(N) single pass and reverse, Space: O(1) in-place.

// LCM of array elements modulo MOD via prime factorization
// Time: O(N * sqrt(MAX_VAL)), Space: O(U) distinct primes

ll getLCM(const vi &nums) {
    unordered_map<int, int> max_pwr;
    for (int x : nums) {
        int temp = x;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                max_pwr[p] = max(max_pwr[p], cnt);
            }
        }
        if (temp > 1)
            max_pwr[temp] = max(max_pwr[temp], 1);
    }

    ll lcm = 1;
    for (auto [prime, pwr] : max_pwr) {
        lcm = (lcm * power(prime, pwr, MOD)) % MOD;
    }
    return lcm;
}
// Interview Explanation:
// - Problem Statement: Compute the LCM of an array of numbers modulo MOD via prime factorization.
// - Approach: Prime Factorization with Maximum Exponent Tracking.
// - Intuition: LCM equals the product of each prime factor raised to its maximum multiplicity across all numbers in the array.
// - Complexity: Time: O(N \sqrt{\max(A)}) for trial division factorization, Space: O(U) where $U$ is the number of distinct prime factors.

// Minimum rounds to return permutation to original order (LCM of cycle lengths)
// Time: O(N * sqrt(N)), Space: O(N)

int permutationRounds(int n, const vi &a) {
    vector<bool> vis(n + 1, false);
    vi cycle_len;

    for (int i = 1; i <= n; i++) {
        if (vis[i])
            continue;
        int cur = i, len = 0;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur];
            len++;
        }
        cycle_len.push_back(len);
    }
    return getLCM(cycle_len);
}
// Interview Explanation:
// - Problem Statement: Find minimum rounds for a permutation to return to original order (LCM of cycle lengths).
// - Approach: Disjoint Cycle Decomposition + LCM of Cycle Lengths.
// - Intuition: Any permutation decomposes into disjoint cycles; each cycle returns to identity after its length, so the full array aligns at $\text{LCM}(\text{lengths})$.
// - Complexity: Time: O(N + \sum \sqrt{\text{len}}) for cycle discovery and LCM, Space: O(N) for visited boolean array.



// Minimum arbitrary swaps to sort 1-indexed permutation (N - cycles)
// Time: O(N), Space: O(N)

int minAnySwaps(const vi &a) {
    int n = a.size(), cycles = 0;
    vector<bool> vis(n, false);

    for (int i = 0; i < n; i++) {
        if (vis[i])
            continue;
        cycles++;
        int cur = i;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur] - 1;
        }
    }
    return n - cycles;
}
// Interview Explanation:
// - Problem Statement: Find the minimum arbitrary swaps to sort a 1-indexed permutation.
// - Approach: Permutation Cycle Decomposition.
// - Intuition: A cycle of length $L$ requires exactly $L - 1$ swaps to resolve; summing across $C$ disjoint cycles yields $\sum (L_i - 1) = N - C$ swaps.
// - Complexity: Time: O(N) traversing each element once, Space: O(N) visited boolean array.

// Minimum move-to-anywhere operations to sort array (N - LIS)
// Time: O(N log N), Space: O(N)

int minMoveAnywhere(const vi &a) {
    vi lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end())
            lis.push_back(x);
        else
            *it = x;
    }
    return (int)a.size() - (int)lis.size();
}
// Interview Explanation:
// - Problem Statement: Find the minimum move-to-anywhere operations needed to sort an array.
// - Approach: Longest Increasing Subsequence (LIS) via Patience Sorting.
// - Intuition: Elements belonging to the LIS can stay in place while all other elements must be moved; minimum moves required is $N - \text{LIS}$.
// - Complexity: Time: O(N \log N) binary search with `lower_bound`, Space: O(N) for the LIS array.

// Minimum move-to-front operations to sort permutation of size N
// Time: O(N), Space: O(1)

int minMoveToFront(const vi &a) {
    int n = a.size(), need = n;
    for (int i = n - 1; i >= 0; i--) {
        if (a[i] == need)
            need--;
    }
    return need;
}
// Interview Explanation:
// - Problem Statement: Find the minimum move-to-front operations needed to sort a permutation of size N.
// - Approach: Greedy Backwards Suffix Matching from $N$ down to 1.
// - Intuition: Scan array from right to left tracking expected value ($N, N-1, \dots$); elements encountered in decreasing order never move, leaving `need` moves to front.
// - Complexity: Time: O(N) single backwards pass, Space: O(1) auxiliary space.

// Check if string is formed by repeating a substring
// Time: O(N), Space: O(N)

bool repeatedSubstringPattern(const string &s) {
    return (s + s).substr(1, 2 * s.size() - 2).find(s) != string::npos;
}
// Interview Explanation:
// - Problem Statement: Check if a given string can be constructed by repeating a substring.
// - Approach: String Doubling and Truncation Trick: $(s + s)[1\dots 2n-2]$.
// - Intuition: If string $s$ consists of repeating units of period $p$, cyclical shifts contain $s$; searching in $s + s$ with endpoints stripped verifies periodicity.
// - Complexity: Time: O(N) via standard substring search, Space: O(N) to store concatenated string.

// Minimum continuous subarrays needed with sum not exceeding K
// Time: O(N), Space: O(1)

int countSubarrays(const vi &nums, int k) {
    int sum = 0, cnt = 1;
    for (int x : nums) {
        sum += x;
        if (sum > k) {
            sum = x;
            cnt++;
        } else if (sum == k) {
            sum = 0;
            cnt++;
        }
    }
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Find the minimum continuous subarrays needed with sum not exceeding K.
// - Approach: Greedy Running Sum Partitioning.
// - Intuition: Greedily extend the current subarray as long as sum $\le K$; split and start a new subarray whenever adding an element would exceed $K$.
// - Complexity: Time: O(N) single linear pass, Space: O(1) auxiliary space.

// K-th smallest element via Lomuto QuickSelect
// Time: O(N) average, Space: O(1)

int quickSelect(vi &nums, int k) {
    int l = 0, r = (int)nums.size() - 1;
    while (true) {
        int pivot = nums[r], p = l;
        for (int i = l; i < r; i++) {
            if (nums[i] < pivot)
                swap(nums[p++], nums[i]);
        }
        swap(nums[p], nums[r]);

        if (p == k)
            return nums[p];
        if (p < k)
            l = p + 1;
        else
            r = p - 1;
    }
}
// Interview Explanation:
// - Problem Statement: Find the K-th smallest element in an unsorted array using QuickSelect.
// - Approach: QuickSelect with Lomuto Partition Scheme.
// - Intuition: Partition array around pivot; if pivot lands at index $k$, target is found, otherwise discard the non-target half and recurse iteratively.
// - Complexity: Time: O(N) average, O(N^2) worst case, Space: O(1) in-place iterative.

// Generate adjacent state transitions for string BFS
// Time: Problem dependent, Space: O(branching factor)

vector<string> getNeighbors(const string &cur) { return {}; }

// Bidirectional BFS for shortest path between start and target strings
// Time: O(B^(D/2)), Space: O(B^(D/2))

int bidirectionalBFS(const string &start, const string &target) {
    if (start == target)
        return 0;
    unordered_set<string> fwd = {start}, bwd = {target}, vis = {start, target};
    int steps = 0;

    while (!fwd.empty() && !bwd.empty()) {
        if (fwd.size() > bwd.size())
            swap(fwd, bwd);
        unordered_set<string> nxt_set;
        steps++;

        for (const string &cur : fwd) {
            for (const string &nxt : getNeighbors(cur)) {
                if (bwd.count(nxt))
                    return steps;
                if (!vis.count(nxt)) {
                    vis.insert(nxt);
                    nxt_set.insert(nxt);
                }
            }
        }
        fwd = move(nxt_set);
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the shortest transformation path between two strings using Bidirectional BFS.
// - Approach: Bidirectional Breadth-First Search (Two-Ended BFS).
// - Intuition: Expand simultaneously from start and target; always expand the smaller set to drastically reduce the search space from $O(B^D)$ to $O(B^{D/2})$.
// - Complexity: Time: O(B^{D/2}) where $B$ is branching factor and $D$ is distance, Space: O(B^{D/2}) for frontier sets.

bool checkValidString(const string &s) {
    int cmin = 0, cmax = 0;
    // cmin represents the minimum number of open parentheses that must be closed
    // cmax represents the maximum number of open parentheses that can be closed
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            cmin++;
            cmax++;
        } else if (s[i] == ')') {
            cmin--;
            cmax--;
        } else {
            cmin--;
            cmax++;
        }
        if (cmax < 0) // If at any point the maximum number of open parentheses is negative, it means there are more closing parentheses than opening ones, which is invalid.
            return false;
        if (cmin < 0) // If the minimum number of open parentheses is negative, we reset it to 0 because we can't have a negative count of open parentheses.
            cmin = 0;
    }
    return cmin == 0;
}
// Interview Explanation:
// - Problem Statement: Check whether a parentheses string containing '(', ')', and '*' is valid.
// - Approach: Range Tracking / Greedy Min-Max Open Parentheses Bounds.
// - Intuition: `cmin` tracks min open parens (treating '*' as ')') and `cmax` tracks max (treating '*' as '('); invalid if `cmax < 0`, valid if `cmin == 0` at end.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

int countPairs(vector<int>& a, int x) {
    if (a.size() <= 1) return 0;

    int mid = a.size() / 2;
    vector<int> l(a.begin(), a.begin() + mid);
    vector<int> r(a.begin() + mid, a.end());

    int cnt = countPairs(l, x) + countPairs(r, x);

    int j = 0;
    for (int i = 0; i < l.size(); i++) {
        while (j < r.size() && l[i] > 1LL * x * r[j])j++;
        cnt += j;
    }

    merge(l.begin(), l.end(), r.begin(), r.end(), a.begin()); // Merge the two halves back into the original array to maintain sorted order for further recursive calls.
    // time complexity: O(n log n) due to the merge sort approach, space complexity: O(n) for the temporary arrays l and r.
    // merge is built in function in C++ STL that merges two sorted ranges into a single sorted range.

    return cnt;
}
// Interview Explanation:
// - Problem Statement: Count the number of reverse pairs (i < j with a[i] > x * a[j]) in an array.
// - Approach: Divide and Conquer / Modified Merge Sort Counting (Reverse Pairs).
// - Intuition: For two sorted halves, use two pointers to count pairs where $l[i] > x \cdot r[j]$ in $O(N)$ before standard merge step.
// - Complexity: Time: O(N \log N) recurrence $T(N) = 2T(N/2) + O(N)$, Space: O(N) for temporary merge buffers.

// Rearranges the array so arr[mid_idx] is in its final sorted position, where mid_idx = arr.size() / 2.
// Elements before it <= arr[mid_idx]; elements after it >= arr[mid_idx].
// The two partitions themselves are not sorted.
// Average: O(n)
// std::nth_element(arr.begin(), arr.begin() + mid_idx, arr.end()); this is in-place and uses the QuickSelect algorithm to find the k-th smallest element in an unordered list. It partially sorts the array such that the element at the specified position is the one that would be in that position in a fully sorted array, and all elements before it are less than or equal to it, and all elements after it are greater than or equal to it.

//int median = arr[mid_idx];

int findMedian(vi &arr) {
    int n = arr.size();
    if(n&1) { // odd length
        int mid_idx = n / 2;
        nth_element(arr.begin(), arr.begin() + mid_idx, arr.end());
        return arr[mid_idx];
    } else { // even length
        int mid_idx1 = n / 2 - 1;
        int mid_idx2 = n / 2;
        nth_element(arr.begin(), arr.begin() + mid_idx1, arr.end());
        int median1 = arr[mid_idx1];
        nth_element(arr.begin(), arr.begin() + mid_idx2, arr.end());
        int median2 = arr[mid_idx2];
        return (median1 + median2) / 2; // or return both medians if needed
    }
}
// Interview Explanation:
// - Problem Statement: Find the median of an unsorted array in O(N) average time using nth_element.
// - Approach: Quickselect Median Finding via `std::nth_element`.
// - Intuition: `std::nth_element` places elements at target middle index $n/2$ in sorted position in linear average time; averages two medians for even lengths.
// - Complexity: Time: O(N) average time, Space: O(1) auxiliary space in-place.

void sort012(vi &arr) {
    int low = 0, mid = 0, high = arr.size() - 1;
    while (mid <= high) {
        if (arr[mid] == 0) {
            swap(arr[low++], arr[mid++]);
        } else if (arr[mid] == 1) {
            mid++;
        } else { // arr[mid] == 2
            swap(arr[mid], arr[high--]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array of 0s, 1s, and 2s in-place in linear time (Dutch National Flag).
// - Approach: Dutch National Flag 3-Way Partitioning with 3 pointers: `low`, `mid`, `high`.
// - Intuition: Invariant maintains [0...low-1] as 0s, [low...mid-1] as 1s, and [high+1...n-1] as 2s; element at `mid` is swapped to its corresponding section.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space in-place.


vi merge2sortedarrays(vi& a, vi& b) {
    int n = a.size(), m = b.size();
    vi merged(n + m);
    int i = 0, j = 0, k = 0;

    while (i < n && j < m) {
        if (a[i] <= b[j]) {
            merged[k++] = a[i++];
        } else {
            merged[k++] = b[j++];
        }
    }

    while (i < n) {
        merged[k++] = a[i++];
    }

    while (j < m) {
        merged[k++] = b[j++];
    }

    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge two sorted arrays into a single sorted array.
// - Approach: Two-Pointer Merge Step (standard Merge Sort subroutine).
// - Intuition: Simultaneously advance pointers across both sorted arrays, appending the smaller head to `merged` and flushing leftovers.
// - Complexity: Time: O(N + M) linear scan, Space: O(N + M) for the merged array.

void mergeSort(vi& a, int l, int r){
    if(l >= r) return;

    int mid = l + (r - l) / 2;
    mergeSort(a, l, mid);
    mergeSort(a, mid + 1, r);

    vi left(a.begin() + l, a.begin() + mid + 1);
    vi right(a.begin() + mid + 1, a.begin() + r + 1);
    vi temp = merge2sortedarrays(left, right);
    copy(temp.begin(), temp.end(), a.begin() + l);
}
// Interview Explanation:
// - Problem Statement: Sort an array using divide-and-conquer Merge Sort.
// - Approach: Classic Divide-and-Conquer Recursive Merge Sort.
// - Intuition: Recursively divide array into halves until base case of size $\le 1$, then merge sorted halves back using two pointers.
// - Complexity: Time: O(N \log N) in all cases, Space: O(N) for temporary auxiliary merge vectors.

int longestSubstring(string s, int k) {
    int maxLength = 0;
    int n = s.length();

    // Try for every possible number of unique characters (1 to 26)
    for (int uniqueTarget = 1; uniqueTarget <= 26; ++uniqueTarget) {
        vi freq(26, 0);
        int left = 0, right = 0;
        int uniqueCount = 0;   // Count of distinct characters in current window
        int countAtLeastK = 0; // Count of distinct characters with freq >= k

        while (right < n) {
            // Expand window
            if (uniqueCount <= uniqueTarget) {
                int idx = s[right] - 'a';
                if (freq[idx] == 0) uniqueCount++;
                freq[idx]++;
                if (freq[idx] == k) countAtLeastK++;
                right++;
            }
            // Shrink window
            else {
                int idx = s[left] - 'a';
                if (freq[idx] == k) countAtLeastK--;
                freq[idx]--;
                if (freq[idx] == 0) uniqueCount--;
                left++;
            }

            // If window has target unique chars and ALL of them appear >= k times
            if (uniqueCount == uniqueTarget && uniqueCount == countAtLeastK) {
                maxLength = max(maxLength, right - left); // because we incremented right after adding the character, so the length is right - left
            }
        }
    }

    return maxLength;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest substring where every character frequency is at least k.
// - Approach: Sliding Window constrained by Target Unique Character Count (1 to 26).
// - Intuition: Fixing target unique count $U \in [1, 26]$ creates window monotonicity; expand right, shrink left when unique count exceeds $U$, and check $\text{freq} \ge k$.
// - Complexity: Time: O(26 \cdot N) = O(N) linear time, Space: O(1) fixed-size frequency array of 26.

void dnfPartition(vector<int>& a, int l, int r, int pivot,int& low, int& high) {
    low = l;
    int mid = l;
    high = r;

    while (mid <= high) {
        if (a[mid] < pivot) {
            swap(a[low++], a[mid++]);
        }
        else if (a[mid] == pivot) {
            mid++;
        }
        else {
            swap(a[mid], a[high--]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Partition an array into three sections: < pivot, == pivot, and > pivot around a pivot.
// - Approach: Dutch National Flag 3-Way Partition around a given pivot value.
// - Intuition: Partitions array into `< pivot`, `== pivot`, and `> pivot`; returns boundary references `low` and `high` around elements equal to pivot.
// - Complexity: Time: O(N) single pass over subarray, Space: O(1) auxiliary space in-place.

void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int pivot = a[r];

    int low, high;
    dnfPartition(a, l, r, pivot, low, high);
    // low and high are the indices of the first and last elements equal to the pivot after partitioning. The elements in a[l..low-1] are less than pivot, a[low..high] are equal to pivot, and a[high+1..r] are greater than pivot.
    quickSort(a, l, low - 1);
    quickSort(a, high + 1, r);
}
// Interview Explanation:
// - Problem Statement: Sort an array in-place using 3-way QuickSort (efficient for duplicate elements).
// - Approach: 3-Way QuickSort using Dutch National Flag partitioning.
// - Intuition: Partition into regions `< pivot`, `== pivot`, and `> pivot`; elements equal to pivot are in final position, preventing $O(N^2)$ on duplicate-heavy inputs.
// - Complexity: Time: O(N \log N) average, Space: O(\log N) recursion stack space.


void reverseSubarray(vi& a, int l, int r) {
    while (l < r) swap(a[l++], a[r--]);
}
// Interview Explanation:
// - Problem Statement: Reverse elements of an array between indices l and r in-place.
// - Approach: Two-pointer symmetrical element swap.
// - Intuition: Swap elements at left and right pointers while moving inward until pointers meet or cross.
// - Complexity: Time: O(R - L + 1) linear time, Space: O(1) auxiliary space in-place.

void RightRotate(vi& a, int l, int r, int k) {
    int n = r - l + 1;
    k %= n; // In case k is larger than the segment length
    if (k == 0) return;

    reverseSubarray(a, l, r); // first, rotate the entire segment
    reverseSubarray(a, l, l + k - 1); // then, reverse the first k elements to bring them to the front
    reverseSubarray(a, l + k, r); // finally, reverse the remaining elements to restore their order
}
// Interview Explanation:
// - Problem Statement: Right rotate an array segment from index l to r by k positions in-place.
// - Approach: 3-step reversal algorithm for cyclic right rotation.
// - Intuition: Reverse whole range, reverse first $k$ elements, then reverse remaining $n - k$ elements to shift rotated suffix to the front.
// - Complexity: Time: O(N) where $N = r - l + 1$, Space: O(1) auxiliary space in-place.

void LeftRotate(vi& a, int l, int r, int k) {
    int n = r - l + 1;
    k %= n; // In case k is larger than the segment length
    if (k == 0) return;

    reverseSubarray(a, l, l + k - 1); // first, reverse the first k elements
    reverseSubarray(a, l + k, r); // then, reverse the remaining elements
    reverseSubarray(a, l, r); // finally, reverse the entire segment to complete the left rotation
}
// Interview Explanation:
// - Problem Statement: Left rotate an array segment from index l to r by k positions in-place.
// - Approach: 3-step reversal algorithm for cyclic left rotation.
// - Intuition: Reverse first $k$ elements, reverse remaining $n - k$ elements, then reverse the entire range to cycle elements to the left.
// - Complexity: Time: O(N) where $N = r - l + 1$, Space: O(1) auxiliary space in-place.

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
// - Intuition: Swap element $a[i]$ with $a[a[i] - 1]$ until $a[i] == i + 1$; advance index only when current slot holds its correct element.
// - Complexity: Time: O(N) since each swap places at least one element into its final position, Space: O(1) in-place.

int ConcatenationSort(vi &a) {
    sort(a.begin(), a.end(), [](int x, int y) {
        return to_string(x) + to_string(y) >
               to_string(y) + to_string(x);
    });

    string s;
    for (int x : a)
        s += to_string(x);

    return stoi(s);
}
// Interview Explanation:
// - Problem Statement: Arrange non-negative integers such that their concatenation forms the largest number.
// - Approach: Custom comparator sorting based on string concatenation (Largest Number).
// - Intuition: Sort with predicate $(to\_string(x) + to\_string(y)) > (to\_string(y) + to\_string(x))$; strict weak ordering guarantees maximal concatenated value.
// - Complexity: Time: O(N \log N) sorting comparisons, Space: O(N) for string conversions.

int kthSmallest(vector<int>& a, int k) {
    nth_element(a.begin(), a.begin() + k - 1, a.end()); 
    return a[k - 1];
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest element in an unsorted array using Quickselect.
// - Approach: Quickselect order statistic via `std::nth_element`.
// - Intuition: Partially sorts array so that index $k-1$ holds the element that would appear there in full sort.
// - Complexity: Time: O(N) average, Space: O(1) in-place.

int kthLargest(vector<int>& a, int k) {
    nth_element(a.begin(), a.end() - k, a.end());
    return a[a.size() - k];
}
// Interview Explanation:
// - Problem Statement: Find the k-th largest element in an unsorted array using Quickselect.
// - Approach: Quickselect order statistic on offset `end() - k`.
// - Intuition: The $k$-th largest element corresponds to index $N - k$ in 0-indexed ascending sorted order.
// - Complexity: Time: O(N) average, Space: O(1) in-place.

int median(vi &a) {
    int n = a.size();
    if (n % 2 == 1) {
        return kthSmallest(a, n / 2 + 1);
    } else {
        int left = kthSmallest(a, n / 2);
        int right = kthSmallest(a, n / 2 + 1);
        return (left + right) / 2;
    }
}
// Interview Explanation:
// - Problem Statement: Find the median of an array using order statistics.
// - Approach: Median extraction via Quickselect order statistics.
// - Intuition: Query middle element directly if odd length; average the two central elements ($N/2$ and $N/2 + 1$) if even.
// - Complexity: Time: O(N) average time, Space: O(1) auxiliary space.

int kthElement(const vector<int>& a, const vector<int>& b, int k) {
    if (a.size() > b.size()) return kthElement(b, a, k);

    int n = a.size(), m = b.size();

    int lo = max(0, k - m); // max(0,k-m) because we cannot take more than m elements from b, so we need to take at least k-m elements from a. If k <= m, then we can take 0 elements from a.
    int hi = min(k, n); // min(k,n) because we cannot take more than n elements from a, so we can take at most min(k,n) elements from a.

    while (lo <= hi) {
        int cutA = (lo + hi) / 2;
        int cutB = k - cutA;

        int lA = (cutA == 0) ? INT_MIN : a[cutA - 1];
        int rA = (cutA == n) ? INT_MAX : a[cutA];

        int lB = (cutB == 0) ? INT_MIN : b[cutB - 1];
        int rB = (cutB == m) ? INT_MAX : b[cutB];

        if (lA <= rB && lB <= rA) return max(lA, lB);

        if (lA > rB) hi = cutA - 1;
        else lo = cutA + 1;
    }

    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest element in the union of two sorted arrays in logarithmic time.
// - Approach: Binary search on partition cut in the smaller array.
// - Intuition: Partition both arrays so $\text{cutA} + \text{cutB} = k$; valid partition occurs when $lA \le rB$ and $lB \le rA$, giving answer $\max(lA, lB)$.
// - Complexity: Time: O(\log(\min(N, M))) binary search, Space: O(1) auxiliary space.


double medianOfTwoSortedArrays(const vector<int>& a, const vector<int>& b) {
    int n = a.size(), m = b.size();
    int total = n + m;

    if (total % 2 == 1) {
        return kthElement(a, b, total / 2 + 1);
    } else {
        int left = kthElement(a, b, total / 2);
        int right = kthElement(a, b, total / 2 + 1);
        return (left + right) / 2.0;
    }
}
// Interview Explanation:
// - Problem Statement: Find the median of two sorted arrays in O(log(min(N, M))) time.
// - Approach: Reduction to `kthElement` of two sorted arrays.
// - Intuition: Use binary search partition helper to find element at $(N+M)/2 + 1$ (if odd) or average of $(N+M)/2$ and $(N+M)/2 + 1$ (if even).
// - Complexity: Time: O(\log(\min(N, M))), Space: O(1) auxiliary space.

class RunningKth {
    priority_queue<int> left;
    priority_queue<int, vector<int>, greater<int>> right;

    int k;

public:
    RunningKth(int rank) : k(rank) {}

    void add(int x) {
        if (left.empty() || x <= left.top()) left.push(x);
        else right.push(x);

        // Keep exactly k elements in left
        while (left.size() > k) {
            right.push(left.top());
            left.pop();
        }

        // Keep left with exactly k elements if possible
        while (left.size() < k && !right.empty()) {
            left.push(right.top());
            right.pop();
        }
    }

    int getKth() const {
        return left.top();
    }
};
// Interview Explanation:
// - Problem Statement: Design a data structure that dynamically returns the k-th smallest element added so far.
// - Approach: Two Heaps (Max-Heap `left` holding $k$ smallest, Min-Heap `right` holding the rest).
// - Intuition: Maintain invariant where `left` holds exactly $k$ elements; top of `left` is always the $k$-th smallest element in the stream.
// - Complexity: Time: O(\log K) for `add`, O(1) for `getKth`, Space: O(N) total elements stored.

 void wiggleSort(std::vector<int>& nums) {
        int n = nums.size();
        
        // Step 1: Create a copy of the array
        std::vector<int> sorted_arr = nums;
        
        // Step 2: Find the median in O(n) time
        int mid_idx = (n - 1) / 2;
        nth_element(sorted_arr.begin(), sorted_arr.begin() + mid_idx, sorted_arr.end());
        int median = sorted_arr[mid_idx];
        
        // Step 3: Strict 3-way partition (Dutch National Flag)
        // This ensures all elements equal to the median are clustered in the middle
        int i = 0, j = 0, k = n - 1;
        while (j <= k) {
            if (sorted_arr[j] < median) {
                swap(sorted_arr[i++], sorted_arr[j++]);
            } else if (sorted_arr[j] > median) {
                swap(sorted_arr[j], sorted_arr[k--]);
            } else {
                j++;
            }
        }
        
        // Step 4: Interleave elements back into the original array
        int left = mid_idx;   // Pointer to the end of the smaller/equal half
        int right = n - 1;    // Pointer to the end of the larger half
        
        for (int x = 0; x < n; ++x) {
            if (x % 2 == 0) {
                nums[x] = sorted_arr[left--];
            } else {
                nums[x] = sorted_arr[right--];
            }
        }
    }
// Interview Explanation:
// - Problem Statement: Reorder array into wiggle sort order: nums[0] < nums[1] > nums[2] < nums[3]... (Wiggle Sort II).
// - Approach: Quickselect Median Finding + Dutch National Flag Partition + Interleaving (Wiggle Sort II).
// - Intuition: Find median in $O(N)$, 3-way partition around median, then interleave elements into even/odd indices backwards to prevent adjacent equal elements.
// - Complexity: Time: O(N) total time, Space: O(N) for temporary copy during partitioning.

class LRUCache {
public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        if (mp.find(key) == mp.end())
            return -1;

        l.splice(l.begin(), l, mp[key]); // move the accessed node to the front of the list to mark it as recently used, time complexity O(1)
        return mp[key]->second;
    }

    void put(int key, int value) {
        if (mp.find(key) != mp.end()) {
            mp[key]->second = value;
            l.splice(l.begin(), l, mp[key]); // move the updated node to the front, time complexity O(1)
        } else {
            if (l.size() == cap) {
                int old_key = l.back().first; // get the key of the least recently used item, which is at the back of the list
                l.pop_back();
                mp.erase(old_key);
            }

            l.emplace_front(key, value);
            mp[key] = l.begin();
        }
    }

private:
    int cap;
    list<pair<int, int>> l; // list of pairs (key, value) to maintain the order of usage
    // list is double linked list, so we can move elements to the front in O(1) time.
    // unordered_map maps keys to iterators of the list, allowing O(1) access to the elements in the list.
    unordered_map<int, list<pair<int, int>>::iterator> mp;
};
// Interview Explanation:
// - Problem Statement: Design and implement a Least Recently Used (LRU) Cache with O(1) get and put operations.
// - Approach: Hash Map + Doubly Linked List (`std::list`).
// - Intuition: Hash map gives $O(1)$ key-to-node lookup; doubly linked list maintains access recency where accessed nodes splice to head and LRU evicts from tail.
// - Complexity: Time: O(1) for both `get` and `put`, Space: O(\text{capacity}) for stored nodes.

vvi mergeIntervals(vvi &intervals) {
    if (intervals.empty()) return {};

    sort(intervals.begin(), intervals.end());
    vvi merged;
    merged.push_back(intervals[0]);

    for (const auto &interval : intervals) {
        if (merged.back()[1] >= interval[0]) {
            merged.back()[1] = max(merged.back()[1], interval[1]);
        } else {
            merged.push_back(interval);
        }
    }

    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge all overlapping intervals into a list of non-overlapping intervals.
// - Approach: Sort by Start Times + Greedy Linear Merge.
// - Intuition: After sorting by start times, overlapping intervals are adjacent; merge when $\text{curr.start} \le \text{last.end}$ by taking $\max(\text{last.end}, \text{curr.end})$.
// - Complexity: Time: O(N \log N) sorting, Space: O(N) for merged intervals output.



class LFUCache {
    int cap, minFreq;

    // key -> {value, frequency}
    unordered_map<int, pair<int, int>> kv;

    // frequency -> list of keys
    // Front = most recently used, Back = least recently used
    unordered_map<int, list<int>> freqList;

    // key -> iterator in its frequency list
    unordered_map<int, list<int>::iterator> pos;

public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        if (!kv.count(key))
            return -1;

        int value = kv[key].first;
        increaseFreq(key);
        return value;
    }

    void put(int key, int value) {
        if (cap == 0)
            return;

        // Key already exists
        if (kv.count(key)) {
            kv[key].first = value;
            increaseFreq(key);
            return;
        }

        // Evict LFU key
        if (kv.size() == cap) {
            int keyToRemove = freqList[minFreq].back();

            freqList[minFreq].pop_back();
            pos.erase(keyToRemove);
            kv.erase(keyToRemove);
        }

        // Insert new key with frequency 1
        kv[key] = {value, 1};
        freqList[1].push_front(key);
        pos[key] = freqList[1].begin();

        minFreq = 1;
    }

private:
    void increaseFreq(int key) {
        int freq = kv[key].second;

        // Remove from old frequency list
        freqList[freq].erase(pos[key]);

        // If this was the minimum frequency and list became empty
        if (freq == minFreq && freqList[freq].empty())
            minFreq++;

        // Increase frequency
        kv[key].second++;

        // Add to new frequency list
        freqList[freq + 1].push_front(key);
        pos[key] = freqList[freq + 1].begin();
    }
};
// Interview Explanation:
// - Problem Statement: Design and implement a Least Frequently Used (LFU) Cache with O(1) operations.
// - Approach: Hash Map + Frequency-to-Doubly-Linked-List map + `minFreq` tracker.
// - Intuition: Separate lists for each frequency maintain LRU tie-breaking; on eviction, pop the back of `freqList[minFreq]` in $O(1)$ time.
// - Complexity: Time: O(1) for both `get` and `put`, Space: O(\text{capacity}) for hash maps and lists.

// LRU (Least Recently Used) Cache and LFU (Least Frequently Used) Cache are both caching strategies, but they differ in how they determine which items to evict when the cache reaches its capacity. LRU evicts the least recently accessed item, while LFU evicts the least frequently accessed item. LRU is simpler and often used in scenarios where recent access patterns are more relevant, while LFU is better suited for scenarios where frequency of access is more important.



class TopK {
    int k;
    priority_queue<int, vector<int>, greater<int>> pq;

public:
    TopK(int k) : k(k) {}

    void add(int x) {
        if (pq.size() < k) {
            pq.push(x);
        }
        else if (x > pq.top()) {
            pq.pop();
            pq.push(x);
        }
    }

    vector<int> getTopK() {
        vector<int> ans;

        auto temp = pq;
        while (!temp.empty()) {
            ans.push_back(temp.top());
            temp.pop();
        }

        reverse(ans.begin(), ans.end());
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain and retrieve the top K largest elements from a stream of numbers.
// - Approach: Min-Heap bounded to size $K$.
// - Intuition: Heap stores top $k$ largest elements seen so far; incoming element replaces heap root if larger, keeping size bounded at $k$.
// - Complexity: Time: O(\log K) for `add`, O(K \log K) to extract and sort top $K$, Space: O(K) in heap.

class TopKFrequent {
    int k;
    unordered_map<int, int> freq;

public:
    TopKFrequent(int k) : k(k) {}

    void add(int x) {
        freq[x]++;
    }

    vector<int> getTopK() {
        using P = pair<int, int>; // {frequency, element}

        priority_queue<P, vector<P>, greater<P>> pq;

        for (auto &[x, f] : freq) {
            pq.push({f, x});

            if (pq.size() > k)
                pq.pop();
        }

        vector<int> ans;
        while (!pq.empty()) {
            ans.push_back(pq.top().second);
            pq.pop();
        }

        reverse(ans.begin(), ans.end());
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find the K most frequent elements in an array.
// - Approach: Hash Map Frequency Counter + Min-Heap of size $K$ on `(freq, elem)` pairs.
// - Intuition: Tally frequencies in hash map; stream pairs through a min-heap of capacity $k$, evicting lowest frequency roots when size exceeds $k$.
// - Complexity: Time: O(N \log K) where $N$ is distinct elements, Space: O(N + K) for map and heap.

int compress(vector<char>& chars) {
    int write = 0;

    for (int i = 0; i < chars.size(); ) {
        char ch = chars[i]; // current character to be processed
        int j = i;

        while (j < chars.size() && chars[j] == ch) j++; // find the end of the current character sequence
        int count = j - i;
        chars[write++] = ch; // write the character to the compressed array

        if (count > 1) {
            string s = to_string(count);
            for (char c : s) chars[write++] = c;
        }

        i = j;
    }

    return write;
}
// Interview Explanation:
// - Problem Statement: Compress an array of characters in-place using run-length encoding.
// - Approach: Two-pointer in-place read/write scan (Run-Length Encoding).
// - Intuition: Pointer `j` measures the run of identical characters starting at `i`; write the character and its digit length (if $> 1$) at `write` pointer.
// - Complexity: Time: O(N) single pass through characters, Space: O(1) auxiliary space in-place.

int removeDuplicates(vector<int>& nums) {
        int i=1;
        int n = nums.size();
        for(int j=1;j<n;j++){
            if(nums[i]!=nums[j]){
                nums[i]=nums[j];
                i++;
            }
        }
        return i;
}
// Interview Explanation:
// - Problem Statement: Remove duplicates from a sorted array in-place and return the count of unique elements.
// - Approach: Two-pointer slow/fast scan (`i` write pointer, `j` read pointer).
// - Intuition: In a sorted array duplicates are contiguous; whenever `nums[j] != nums[i]`, advance `i` and copy `nums[j]` to keep unique prefix.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary in-place.

int longestValidParentheses(string s) {
    stack<int> st; // stack stores the indices of characters in the string, it stores the index of the last unmatched '(' or the base index for valid substrings
    st.push(-1);  // base index

    int ans = 0;

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop(); // if s[i] == ')', pop the last index, because we are trying to find a matching '(' for this ')'
            if (st.empty()) st.push(i); // if stack becomes empty, push the current index as a new base for future valid substrings
            else ans = max(ans, i - st.top());
        }
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest valid (well-formed) parentheses substring.
// - Approach: Stack of indices initialized with base boundary `-1`.
// - Intuition: Push index on '('; on ')', pop match and record length `i - stack.top()`; if stack becomes empty, push `i` as new base boundary.
// - Complexity: Time: O(N) single pass, Space: O(N) auxiliary space for stack.

class WildcardMatching {
public:
    bool helper(string &s, string &p, int i, int j, vector<vector<int>> &dp) {
        // Base cases
        if (i == s.size() && j == p.size()) return true; // Both strings are exhausted
        if (j == p.size()) return false; // Pattern is exhausted but string is not
        if (i == s.size()) { // String is exhausted
            // Check if remaining pattern only contains '*'
            for (int k = j; k < p.size(); ++k)
                if (p[k] != '*') return false;
            return true;
        }

        // If already computed
        if (dp[i][j] != -1) return dp[i][j];

        // Match conditions
        if (p[j] == s[i] || p[j] == '?') {
            // Characters match or '?' matches any single character
            dp[i][j] = helper(s, p, i + 1, j + 1, dp);
        } else if (p[j] == '*') {
            // '*' matches zero or more characters
            dp[i][j] = helper(s, p, i + 1, j, dp) || helper(s, p, i, j + 1, dp);
        } else {
            // Characters do not match
            dp[i][j] = false;
        }

        return dp[i][j];
    }

    bool isMatch(string s, string p) {
        // Memoization table
        vector<vector<int>> dp(s.size(), vector<int>(p.size(), -1));
        return helper(s, p, 0, 0, dp);
    }
};
// Interview Explanation:
// - Problem Statement: Implement wildcard pattern matching with support for '?' and '*'.
// - Approach: Top-down 2D Dynamic Programming with Memoization.
// - Intuition: '?' matches any single char; '*' branches into matching 0 characters (`helper(i, j+1)`) or 1+ characters (`helper(i+1, j)`).
// - Complexity: Time: O(M \cdot N) distinct subproblems, Space: O(M \cdot N) for memoization table and call stack.


class RegularExpressionMatching {
    vector<vector<int>> dp;
    string s, p;

    bool dfs(int i, int j) {
        if(j==p.size() && i==s.size()) return true; // both string and pattern are exhausted
        if(j==p.size()) return false; // pattern is exhausted but string is not exhausted
        if(i==s.size()) return false; // string is exhausted but pattern is not
        if (dp[i][j] != -1) return dp[i][j];

        bool match = i < s.size() && (s[i] == p[j] || p[j] == '.'); // check if the current characters match or if the pattern has a '.' which can match any character

        if (j + 1 < p.size() && p[j + 1] == '*') { // if the next character in the pattern is '*', we have two choices: either skip the '*' and the preceding character in the pattern (which represents 0 occurrences of that character), or use the '*' to match the current character in the string (which represents 1 or more occurrences of that character).
            // 0 occurrences OR 1+ occurrences
            return dp[i][j] = dfs(i, j + 2) || (match && dfs(i + 1, j));
        }
        // dp(i,j+2) means skipping *
        // dp(i+1,j) means using * to match current character and move to next character in string
        return dp[i][j] = match && dfs(i + 1, j + 1);
    }

public:
    bool isMatch(string str, string pat) {
        s = str;
        p = pat;
        dp.assign(s.size() + 1, vector<int>(p.size() + 1, -1));

        return dfs(0, 0);
    }
};
// Interview Explanation:
// - Problem Statement: Implement regular expression matching with support for '.' and '*'.
// - Approach: Top-down 2D Dynamic Programming / DFS with Memoization.
// - Intuition: When next char is '*', branch into skipping 0 occurrences (`dfs(i, j+2)`) or matching 1+ occurrences (`match && dfs(i+1, j)`); otherwise direct match.
// - Complexity: Time: O(M \cdot N) distinct states, Space: O(M \cdot N) for memoization table and call stack.

// diff between wildcard and regex problem is that wildcard uses '?' and '*' for matching, while regex uses '.' and '*' for matching. * in wildcard means any sequence of characters, while * in regex means zero or more of the preceding element.

class GasStationCircuit {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total = 0; // stores the total net gain/loss of gas after completing the circuit
        int tank = 0; // stores the current amount of gas in the tank while traversing the circuit
        int start = 0; // stores the index of the starting gas station from which we can complete the circuit

        for (int i = 0; i < gas.size(); i++) {
            int gain = gas[i] - cost[i]; // calculate the net gain/loss of gas at station i

            total += gain; // update the total net gain/loss of gas after completing the circuit
            tank += gain; // update the current amount of gas in the tank while traversing the circuit

            // Cannot reach i+1 from current start
            if (tank < 0) {
                start = i + 1;
                tank = 0;
            }
        }

        return total >= 0 ? start : -1;
    }
};
// Interview Explanation:
// - Problem Statement: Find the starting gas station index to complete a circular circuit, or -1 if impossible.
// - Approach: Greedy single-pass tracking total surplus and running tank balance.
// - Intuition: If overall gas $\ge$ cost, a unique valid start exists; if running `tank < 0`, no station in current window can start, so reset `start = i + 1`.
// - Complexity: Time: O(N) single linear pass, Space: O(1) auxiliary space.