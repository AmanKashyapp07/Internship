#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
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
 | 1  | K-th Element & Median of 2 Sorted Arrays    | Binary Search on Partition Cut    | O(log min| O(1)     |
 | 2  | Next Greater Permutation                    | 3-Step Suffix Inversion Reversal   | O(N)     | O(1)     |
 | 3  | Longest Substring Without Repeating Chars   | Sliding Window + Last Seen Map    | O(N)     | O(min N) |
 | 4  | Min Moves to Gather K Consecutive Ones      | Shifted Index + Median Prefix Sum | O(N)     | O(N)     |
 | 5  | 2D Prefix Sum Matrix                        | 2D Inclusion-Exclusion Prefix DP   | O(M * N) | O(M * N) |
 | 6  | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values    | O(N * 30)| O(N)     |
 | 7  | Multiply Two 2D Matrices                    | 3-Nested Loop Dot Products         | O(M*N*P) | O(M * P) |
 | 8  | Count Subsequences with Sum Equal to K      | 1D 0/1 Knapsack DP (Backwards)    | O(N * K) | O(K)     |
 | 9  | Row with Maximum 1s in Binary Matrix        | Top-Right Corner Staircase Scan   | O(N + M) | O(1)     |
 | 10 | Max Components Tree Split with Equal Sum    | Divisors + Subtree DFS Cuts       | O(N*div) | O(N)     |
 | 11 | Wildcard Pattern Matching                   | 2D DP Memoization ('?' and '*')   | O(\|S\|\|P\|) | O(\|S\|\|P\|)|
 | 12 | Next Greater Element                        | Monotonic Decreasing Stack        | O(N)     | O(N)     |
 | 13 | Next Smaller Element                        | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 14 | Previous Greater Element                    | Monotonic Decreasing Stack        | O(N)     | O(N)     |
 | 15 | Previous Smaller Element                    | Monotonic Increasing Stack        | O(N)     | O(N)     |
 | 16 | Stock Span Problem                          | Monotonic Stack of Indices        | O(N)     | O(N)     |
 | 17 | Next Greater Element in Circular Array      | Monotonic Stack on Doubled (2N)   | O(N)     | O(N)     |
 | 18 | Sum of Subarray Minimums                    | Monotonic Stack Contribution      | O(N)     | O(N)     |
 | 19 | Maximum Score of Good Subarray              | Monotonic Stack Range Bounds (K)  | O(N)     | O(N)     |
 | 20 | Sum of Subarray Maximums                    | Monotonic Stack Contribution      | O(N)     | O(N)     |
 | 21 | Max of Minimums for Every Window Size       | Monotonic Stack + Suffix Max      | O(N)     | O(N)     |
 | 22 | Array Median using Quickselect              | Randomized QuickSelect (Lomuto)   | O(N) avg | O(1)     |
 | 23 | N x N MEX Grid Construction                 | Bitwise XOR Matrix (i ^ j)        | O(N^2)   | O(N^2)   |
 | 24 | Binary Lifting (LCA, K-th Ancestor)         | Ancestor Doubling Table up[u][j]  | O(N logN)| O(N logN)|
 | 25 | Digit DP Template                           | Digit Memo (pos,start,tight,state)| O(dig*st)| O(dig*st)|
 | 26 | Cycle Detection & Reconstruction            | DFS Parent Trace / Bellman-Ford   | O(V+E)/VE| O(V)     |
 | 27 | Make Array Non-Decreasing (Slope Trick)     | Greedy Max-Heap Slope Inflection  | O(N logN)| O(N)     |
 ====================================================================================================
*/


// =========================================================
// 1. K-TH ELEMENT & MEDIAN OF TWO SORTED ARRAYS
// =========================================================

int kthElement(const vi &nums1, const vi &nums2, int k) {
    if (nums1.size() > nums2.size()) return kthElement(nums2, nums1, k);
    int m = nums1.size(), n = nums2.size();
    int l = max(0, k - n);
    int r = min(k, m);

    while (l <= r) {
        int i = l + (r - l) / 2;
        int j = k - i;

        int l1 = (i == 0) ? INT_MIN : nums1[i - 1];
        int r1 = (i == m) ? INT_MAX : nums1[i];
        int l2 = (j == 0) ? INT_MIN : nums2[j - 1];
        int r2 = (j == n) ? INT_MAX : nums2[j];

        if (l1 <= r2 && l2 <= r1) return max(l1, l2);
        if (l1 > r2) r = i - 1;
        else l = i + 1;
    }
    return -1;
}

int median(const vi &nums1, const vi &nums2) {
    int total = nums1.size() + nums2.size();
    if (total % 2 == 1) {
        return kthElement(nums1, nums2, total / 2 + 1);
    } else {
        int left = kthElement(nums1, nums2, total / 2);
        int right = kthElement(nums1, nums2, total / 2 + 1);
        return (left + right) / 2;
    }
}
// Interview Explanation:
// - Problem Statement: Find the k-th smallest element and median of two sorted arrays in logarithmic time.
// - Approach: Binary search on partition cut in the smaller array.
// - Intuition: Partition both arrays into left and right halves with total size k; valid partition satisfies l1 <= r2 and l2 <= r1, giving answer max(l1, l2).
// - Complexity: Time: O(log(min(M, N))) binary search, Space: O(1) auxiliary space.


// =========================================================
// 2. NEXT GREATER PERMUTATION
// =========================================================

void nextPermutation(vi &nums) {
    int n = nums.size(), i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;

    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}
// Interview Explanation:
// - Problem Statement: Rearrange numbers into the lexicographically next greater permutation in-place.
// - Approach: Standard 3-step lexicographical successor algorithm.
// - Intuition: Find rightmost index i where nums[i] < nums[i+1], swap with smallest element to its right greater than nums[i], and reverse suffix nums[i+1...n-1].
// - Complexity: Time: O(N) single pass and reverse, Space: O(1) in-place.


// =========================================================
// 3. LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS
// =========================================================

int lengthOfLongestSubstring(const string &s) {
    unordered_map<char, int> last_seen;
    int l = 0, max_len = 0;

    for (int r = 0; r < (int)s.size(); r++) {
        if (last_seen.count(s[r]) && last_seen[s[r]] >= l) {
            l = last_seen[s[r]] + 1;
        }
        last_seen[s[r]] = r;
        max_len = max(max_len, r - l + 1);
    }
    return max_len;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest substring without repeating characters.
// - Approach: Sliding window with hash map storing last seen index of each character.
// - Intuition: If character at right pointer was seen inside current window, jump left pointer to last_seen[c] + 1; update max window length r - l + 1.
// - Complexity: Time: O(N) linear scan, Space: O(min(N, alphabet)) for character map.


// =========================================================
// 4. MINIMUM MOVES TO GATHER K CONSECUTIVE ONES
// =========================================================

ll minMoves(const vi &nums, int k) {
    vl pos;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (nums[i]) pos.push_back(i);
    }

    int m = pos.size();
    vl shifted(m), pref(m + 1, 0);
    for (int i = 0; i < m; i++) {
        shifted[i] = pos[i] - i;
        pref[i + 1] = pref[i] + shifted[i];
    }

    ll ans = 1e18;
    for (int l = 0; l + k <= m; l++) {
        int r = l + k - 1, mid = (l + r) / 2;
        ll median = shifted[mid];
        ll cost = median * (mid - l) - (pref[mid] - pref[l]) + (pref[r + 1] - pref[mid + 1]) - median * (r - mid);
        ans = min(ans, cost);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the minimum moves to group any K consecutive 1s together in a binary array.
// - Approach: Shifted indices transformation + Prefix sum median cost evaluation.
// - Intuition: For 1s at positions p_0, p_1, ..., shifting by p_i - i transforms grouping adjacent elements into gathering coordinates to their median, solvable via prefix sums.
// - Complexity: Time: O(N) single pass and sliding window, Space: O(N) for shifted coordinates and prefix array.


// =========================================================
// 5. 2D PREFIX SUM MATRIX
// =========================================================

vvi prefixSum2D(const vvi &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vvi pref(m, vi(n));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            pref[i][j] = matrix[i][j];
            if (i > 0) pref[i][j] += pref[i - 1][j];
            if (j > 0) pref[i][j] += pref[i][j - 1];
            if (i > 0 && j > 0) pref[i][j] -= pref[i - 1][j - 1];
        }
    }
    return pref;
}
// Interview Explanation:
// - Problem Statement: Construct a 2D prefix sum table supporting O(1) submatrix sum queries.
// - Approach: Inclusion-Exclusion 2D Dynamic Programming.
// - Intuition: Each cell sum is matrix[i][j] + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1], adding top and left sums and removing double-counted diagonal.
// - Complexity: Time: O(M * N) filling 2D grid, Space: O(M * N) for prefix sum matrix.


// =========================================================
// 6. COUNT SUBARRAYS WITH BITWISE AND EQUAL TO K
// =========================================================

ll countSubarraysWithAND(const vi &nums, int k) {
    ll ans = 0;
    unordered_map<int, ll> prev, cur;

    for (int x : nums) {
        cur.clear();
        cur[x]++;
        for (auto [v, cnt] : prev) {
            cur[v & x] += cnt;
        }
        ans += cur[k];
        prev = std::move(cur);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count the number of subarrays whose bitwise AND equals K.
// - Approach: Hash Map DP aggregating unique prefix AND results.
// - Intuition: As elements are added, cumulative bitwise AND can change value at most 30 times; maintain counts of distinct AND values ending at previous index and transition.
// - Complexity: Time: O(N * 30) = O(N log(max_val)), Space: O(N) for hash maps.


// =========================================================
// 7. MULTIPLY TWO 2D MATRICES
// =========================================================

vvi multiplyMatrices2D(const vvi &A, const vvi &B) {
    int m = A.size(), n = A[0].size(), p = B[0].size();
    vvi C(m, vi(p, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}
// Interview Explanation:
// - Problem Statement: Multiply two 2D matrices A of dimensions M x N and B of dimensions N x P.
// - Approach: Standard 3-nested loop matrix multiplication.
// - Intuition: Each entry C[i][j] = sum(A[i][k] * B[k][j]) computes the dot product of row i of A and column j of B.
// - Complexity: Time: O(M * N * P) triple loop, Space: O(M * P) for result matrix.


// =========================================================
// 8. COUNT SUBSEQUENCES WITH SUM EQUAL TO K
// =========================================================

ll countSubsequences(const vi &nums, int k) {
    vl dp(k + 1, 0);
    dp[0] = 1;
    for (int x : nums) {
        for (int sum = k; sum >= x; sum--) {
            dp[sum] += dp[sum - x];
        }
    }
    return dp[k];
}
// Interview Explanation:
// - Problem Statement: Count the number of subsequences whose sum equals K.
// - Approach: 0/1 Knapsack 1D Dynamic Programming.
// - Intuition: dp[sum] += dp[sum - x] scanning backwards from K down to x to prevent using the same element multiple times in the same subset.
// - Complexity: Time: O(N * K) nested loop, Space: O(K) rolling DP vector.


// =========================================================
// 9. ROW WITH MAXIMUM 1s IN BINARY MATRIX
// =========================================================

struct Node {
    int value, height;
    Node *left, *right;
    Node(int x) : value(x), height(1), left(nullptr), right(nullptr) {}
};

int rowWithMax1s(const vvi &mat) {
    int n = mat.size(), m = mat[0].size();
    int row = 0, col = m - 1, ans = -1;

    while (row < n && col >= 0) {
        if (mat[row][col] == 1) {
            ans = row;
            col--;
        } else {
            row++;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the 0-based index of the row with the maximum number of 1s in a row-sorted binary matrix.
// - Approach: Top-Right corner staircase elimination.
// - Intuition: Start at top-right (0, m-1); if cell is 1, update best row and move left (col--); else move down (row++); each step eliminates one row or column.
// - Complexity: Time: O(N + M) at most N down and M left steps, Space: O(1) auxiliary space.


// =========================================================
// 10. MAXIMUM COMPONENTS TREE SPLIT WITH EQUAL SUM
// =========================================================

static bool ok_comp;

ll dfsMaxComp(int u, int p, const vvi &g, const vi &val, ll target) {
    ll sum = val[u];
    for (int v : g[u]) {
        if (v == p) continue;
        sum += dfsMaxComp(v, u, g, val, target);
    }
    if (sum == target) return 0;
    if (sum > target) ok_comp = false;
    return sum;
}

int maxComponents(const vi &val, const vvi &g) {
    ll total = accumulate(val.begin(), val.end(), 0LL);
    vl divisors;

    for (ll d = 1; d * d <= total; d++) {
        if (total % d == 0) {
            divisors.push_back(d);
            if (d * d != total) divisors.push_back(total / d);
        }
    }
    sort(divisors.begin(), divisors.end());

    for (ll target : divisors) {
        ok_comp = true;
        if (dfsMaxComp(0, -1, g, val, target) == 0 && ok_comp) {
            return total / target - 1;
        }
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Find the maximum number of connected components a tree can be partitioned into such that each component has equal node sum.
// - Approach: Factorization of total tree sum + Subtree sum DFS verification.
// - Intuition: Number of components must divide total sum; test divisors in ascending order; DFS returns 0 when subtree reaches target sum, effectively cutting the component.
// - Complexity: Time: O(N * divisors(total_sum)), Space: O(N) for recursion stack.


// =========================================================
// 11. WILDCARD PATTERN MATCHING
// =========================================================

bool wildcardHelper(const string &s, const string &p, int i, int j, vvi &dp) {
    int n = s.size(), m = p.size();
    if (i == n && j == m) return true;
    if (j == m) return false;
    if (i == n) {
        for (int k = j; k < m; k++) {
            if (p[k] != '*') return false;
        }
        return true;
    }
    if (dp[i][j] != -1) return dp[i][j];

    if (p[j] == s[i] || p[j] == '?') {
        dp[i][j] = wildcardHelper(s, p, i + 1, j + 1, dp);
    } else if (p[j] == '*') {
        dp[i][j] = wildcardHelper(s, p, i + 1, j, dp) || wildcardHelper(s, p, i, j + 1, dp);
    } else {
        dp[i][j] = false;
    }
    return dp[i][j];
}

bool wildcardMatch(const string &s, const string &p) {
    vvi dp(s.size(), vi(p.size(), -1));
    return wildcardHelper(s, p, 0, 0, dp);
}
// Interview Explanation:
// - Problem Statement: Determine if a string matches a wildcard pattern containing '?' and '*'.
// - Approach: Top-down 2D Dynamic Programming with Memoization.
// - Intuition: '?' matches any single character; '*' branches into matching 0 characters (solve(i, j+1)) or 1+ characters (solve(i+1, j)).
// - Complexity: Time: O(|S| * |P|) unique state pairs, Space: O(|S| * |P|) for DP memoization table.


// =========================================================
// 12. NEXT GREATER ELEMENT
// =========================================================

vi nextGreaterElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the next strictly greater element for every item in an array (or -1 if none exists).
// - Approach: Monotonic Decreasing Stack storing indices.
// - Intuition: Current element resolves all smaller elements pending on top of stack; pop them, set ans[st.top()] = nums[i], then push current index.
// - Complexity: Time: O(N) each element pushed and popped at most once, Space: O(N) for stack and result array.


// =========================================================
// 13. NEXT SMALLER ELEMENT
// =========================================================

vi nextSmallerElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the next strictly smaller element for every item in an array.
// - Approach: Monotonic Increasing Stack storing indices.
// - Intuition: Current element resolves all strictly larger elements pending on top of stack; pop them and record answer.
// - Complexity: Time: O(N) linear stack pass, Space: O(N) for stack and result vector.


// =========================================================
// 14. PREVIOUS GREATER ELEMENT
// =========================================================

vi previousGreaterElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] <= nums[i]) st.pop();
        if (!st.empty()) ans[i] = nums[st.top()];
        st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the previous strictly greater element for each item in an array.
// - Approach: Monotonic Decreasing Stack.
// - Intuition: Pop all elements smaller than or equal to nums[i]; if stack is non-empty, top is the nearest previous greater element.
// - Complexity: Time: O(N) single forward pass, Space: O(N) for stack.


// =========================================================
// 15. PREVIOUS SMALLER ELEMENT
// =========================================================

vi previousSmallerElement(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] >= nums[i]) st.pop();
        if (!st.empty()) ans[i] = nums[st.top()];
        st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the previous strictly smaller element for each item in an array.
// - Approach: Monotonic Increasing Stack.
// - Intuition: Pop all elements greater than or equal to nums[i]; remaining top of stack is the nearest previous smaller element.
// - Complexity: Time: O(N) single forward pass, Space: O(N) for stack.


// =========================================================
// 16. STOCK SPAN PROBLEM
// =========================================================

vi stockSpan(const vi &prices) {
    int n = prices.size();
    vi span(n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && prices[st.top()] <= prices[i]) st.pop();
        span[i] = st.empty() ? i + 1 : i - st.top();
        st.push(i);
    }
    return span;
}
// Interview Explanation:
// - Problem Statement: Calculate the span of stock prices (maximum consecutive days leading up to today where price <= today's price).
// - Approach: Monotonic Stack storing indices of previous greater prices.
// - Intuition: Pop all prices smaller than or equal to today's; span is i - st.top() (or i + 1 if stack becomes empty).
// - Complexity: Time: O(N) linear time, Space: O(N) for stack.


// =========================================================
// 17. NEXT GREATER ELEMENT IN CIRCULAR ARRAY
// =========================================================

vi nextGreaterCircular(const vi &nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < 2 * n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) {
            ans[st.top()] = nums[i % n];
            st.pop();
        }
        if (i < n) st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the next greater element for every item in a circular array.
// - Approach: Monotonic Stack over doubled virtual array [0...2N-1].
// - Intuition: Traversing 2N indices with modulo index i % n simulates circular wrap-around while maintaining a standard monotonic decreasing stack.
// - Complexity: Time: O(N) two passes through array, Space: O(N) for stack.


// =========================================================
// 18. SUM OF SUBARRAY MINIMUMS
// =========================================================

ll sumSubarrayMins(const vi &arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            ans = (ans + 1LL * arr[mid] * (mid - l) % MOD * (r - mid)) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Compute the sum of minimum values of all subarrays modulo 1e9 + 7.
// - Approach: Contribution Technique using Monotonic Stack.
// - Intuition: Each element a[i] is the minimum for (i - l) * (r - i) subarrays where l is previous strictly smaller and r is next smaller or equal index.
// - Complexity: Time: O(N) single pass with monotonic stack, Space: O(N) for stack.


// =========================================================
// 19. MAXIMUM SCORE OF GOOD SUBARRAY
// =========================================================

int maximumScore(const vi &nums, int k) {
    int n = nums.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || nums[st.top()] >= nums[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            if (l < k && k < r) {
                ans = max(ans, 1LL * nums[mid] * (r - l - 1));
            }
        }
        if (i < n) st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum score of a good subarray containing index K, where score is min(subarray) * length.
// - Approach: Monotonic Stack identifying valid range bounds [l+1, r-1] containing k.
// - Intuition: For each element treated as the minimum, its validity domain is (l, r); check if index k in (l, r) and maximize nums[mid] * (r - l - 1).
// - Complexity: Time: O(N) single pass with monotonic stack, Space: O(N) for stack.


// =========================================================
// 20. SUM OF SUBARRAY MAXIMUMS
// =========================================================

ll sumSubarrayMaxs(const vi &arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] <= arr[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            ans = (ans + 1LL * arr[mid] * (mid - l) % MOD * (r - mid)) % MOD;
        }
        if (i < n) st.push(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Compute the sum of maximum values of all subarrays modulo 1e9 + 7.
// - Approach: Contribution Technique using Monotonic Stack.
// - Intuition: Each element a[i] is the maximum for (i - l) * (r - i) subarrays where l is previous strictly greater and r is next greater or equal index.
// - Complexity: Time: O(N) linear monotonic stack pass, Space: O(N) for stack.


// =========================================================
// 21. MAXIMUM OF MINIMUMS FOR EVERY WINDOW SIZE
// =========================================================

vi maxOfMins(const vi &arr) {
    int n = arr.size();
    vi ans(n + 1, INT_MIN);
    stack<int> st;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();
            int l = st.empty() ? -1 : st.top();
            int r = i;
            ans[r - l - 1] = max(ans[r - l - 1], arr[mid]);
        }
        if (i < n) st.push(i);
    }

    for (int len = n - 1; len >= 1; len--) {
        ans[len] = max(ans[len], ans[len + 1]);
    }

    vi res;
    for (int len = 1; len <= n; len++) {
        res.push_back(ans[len]);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find the maximum of minimums for every window size from 1 to N.
// - Approach: Monotonic Stack range computation + Suffix maximum propagation.
// - Intuition: For each element a[i] with maximal window length L = r - l - 1 where it is the minimum, set ans[L] = max(ans[L], a[i]); then compute suffix maximums backwards.
// - Complexity: Time: O(N) two linear passes, Space: O(N) for stack and result vectors.


// =========================================================
// 22. ARRAY MEDIAN USING QUICKSELECT
// =========================================================

int quickPartition(vi &arr, int l, int r) {
    int rand_idx = l + rand() % (r - l + 1);
    swap(arr[rand_idx], arr[r]);
    int pivot = arr[r], i = l;

    for (int j = l; j < r; j++) {
        if (arr[j] <= pivot) swap(arr[i++], arr[j]);
    }
    swap(arr[i], arr[r]);
    return i;
}

int quickSelect(vi &arr, int l, int r, int k) {
    if (l == r) return arr[l];
    int p_idx = quickPartition(arr, l, r);
    if (k == p_idx) return arr[k];
    return k < p_idx ? quickSelect(arr, l, p_idx - 1, k) : quickSelect(arr, p_idx + 1, r, k);
}

double findMedian(vi &arr) {
    int n = arr.size();
    if (n % 2 == 1) return quickSelect(arr, 0, n - 1, n / 2);
    return (quickSelect(arr, 0, n - 1, n / 2 - 1) + quickSelect(arr, 0, n - 1, n / 2)) / 2.0;
}
// Interview Explanation:
// - Problem Statement: Find the median element of an unsorted array in linear average time.
// - Approach: Randomized QuickSelect with Lomuto Partitioning.
// - Intuition: Randomly partition array; discard the irrelevant half to find the element that lands at N/2 (and N/2 - 1 for even lengths).
// - Complexity: Time: O(N) average time (O(N^2) worst case), Space: O(1) auxiliary in-place.


// =========================================================
// 23. N x N MEX GRID CONSTRUCTION
// =========================================================

vvi mexGridConstruction(int n) {
    vvi grid(n, vi(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = i ^ j;
        }
    }
    return grid;
}
// Interview Explanation:
// - Problem Statement: Construct an N x N grid where cell (i, j) contains i ^ j (Nim-sum matrix).
// - Approach: Bitwise XOR Matrix Construction.
// - Intuition: XOR addition ensures every row and column contains a permutation of values, satisfying MEX (minimum excluded) grid properties.
// - Complexity: Time: O(N^2) filling grid, Space: O(N^2) for grid matrix.


// =========================================================
// 24. BINARY LIFTING (LCA, K-TH ANCESTOR, PATH QUERIES)
// =========================================================

struct BinaryLifting {
    static const int LOG = 21;

    int n, root;
    vector<array<int, LOG>> up;
    vector<int> depth;

    BinaryLifting(int n, int root, const vector<vector<int>>& g)
        : n(n), root(root),
          up(n + 1),
          depth(n + 1, 0) {

        // Build parent + depth
        vector<int> q = {root};
        up[root][0] = 0;

        int head = 0;

        while (head < (int)q.size()) {
            int u = q[head++];

            for (int v : g[u]) {
                if (v == up[u][0])
                    continue;

                up[v][0] = u;
                depth[v] = depth[u] + 1;

                q.push_back(v);
            }
        }

        // Binary lifting table
        for (int j = 1; j < LOG; j++) {
            for (int u = 1; u <= n; u++) {
                up[u][j] = up[up[u][j - 1]][j - 1];
            }
        }
    }

    // Move u up by k levels
    int kthAncestor(int u, int k) {
        for (int j = 0; j < LOG && u != 0; j++) {
            if (k & (1 << j))
                u = up[u][j];
        }

        return u == 0 ? -1 : u;
    }

    // Lowest Common Ancestor
    int lca(int a, int b) {

        if (depth[a] < depth[b])
            swap(a, b);

        // Same depth
        a = kthAncestor(a, depth[a] - depth[b]);

        if (a == b)
            return a;

        // Jump both nodes upwards
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[a][j] != up[b][j]) {
                a = up[a][j];
                b = up[b][j];
            }
        }

        return up[a][0];
    }

    // Distance between two nodes
    int dist(int a, int b) {
        int l = lca(a, b);

        return depth[a] + depth[b] - 2 * depth[l];
    }
};
// Interview Explanation:
// - Problem Statement: Perform efficient tree queries including LCA, distance, K-th ancestor, and path aggregates.
// - Approach: Binary Lifting table up[node][k] doubling ancestor jumps (2^k).
// - Intuition: Precompute 2^k-th ancestors via up[u][j] = up[up[u][j-1]][j-1]; any ancestor jump can be decomposed into binary powers of 2 in O(log N) time.
// - Complexity: Time: O(N \log N) preprocessing, O(\log N) per query, Space: O(N \log N) table storage.


// =========================================================
// 25. DIGIT DP TEMPLATE
// =========================================================

struct DigitDP {
    string s;
    ll memo[20][2][2][100];

    ll dp(int pos, bool started, bool tight, int state) {
        if (pos == (int)s.size()) return state;
        if (memo[pos][started][tight][state] != -1) return memo[pos][started][tight][state];

        int limit = tight ? s[pos] - '0' : 9;
        ll ans = 0;

        if (!started) {
            ans += dp(pos + 1, false, tight && (0 == limit), state);
        }

        for (int d = (started ? 0 : 1); d <= limit; d++) {
            ans += dp(pos + 1, true, tight && (d == limit), state + d);
        }

        return memo[pos][started][tight][state] = ans;
    }

    ll solve(ll n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};
// Interview Explanation:
// - Problem Statement: Count numbers <= N satisfying specific digit properties (e.g. digit sum).
// - Approach: Digit Dynamic Programming with memoization memo[pos][started][tight][state].
// - Intuition: Construct numbers digit by digit from most to least significant; tight flag indicates whether choice is restricted by digits of N.
// - Complexity: Time: O(digits * states) = O(18 * 2 * 2 * 100), Space: O(digits * states) memo table.


// =========================================================
// 26. CYCLE DETECTION & RECONSTRUCTION (UNDIRECTED / DIRECTED / NEGATIVE)
// =========================================================

vi buildCycle(int s, int e, const vi &par) {
    vi cyc;
    cyc.push_back(e);
    while (true) {
        cyc.push_back(s);
        if (s == e) break;
        s = par[s];
    }
    reverse(cyc.begin(), cyc.end());
    return cyc;
}

struct UndirectedCycle {
    int n, s = -1, e = -1;
    vvi g;
    vi vis, par;

    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v, u)) return true;
            } else {
                s = v; e = u;
                return true;
            }
        }
        return false;
    }

    vi getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i, -1)) return buildCycle(s, e, par);
        }
        return {};
    }
};

struct DirectedCycle {
    int n, s = -1, e = -1;
    vvi g;
    vi vis, in_path, par;

    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), in_path(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    bool dfs(int u) {
        vis[u] = in_path[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v)) return true;
            } else if (in_path[v]) {
                s = v; e = u;
                return true;
            }
        }
        in_path[u] = 0;
        return false;
    }

    vi getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i)) return buildCycle(s, e, par);
        }
        return {};
    }
};

struct EdgeItem {
    int u, v;
    ll w;
};

vi findNegativeCycle(int n, const vector<EdgeItem> &edges) {
    vl dist(n + 1, 0);
    vi parent(n + 1, -1);
    int last_relaxed = -1;

    for (int i = 1; i <= n; i++) {
        last_relaxed = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                last_relaxed = e.v;
            }
        }
    }

    if (last_relaxed == -1) return {};
    for (int i = 0; i < n; i++) last_relaxed = parent[last_relaxed];
    return buildCycle(last_relaxed, parent[last_relaxed], parent);
}
// Interview Explanation:
// - Problem Statement: Detect and reconstruct cycles in undirected, directed, and edge-weighted graphs.
// - Approach: DFS with parent / path arrays (Undirected/Directed) and Bellman-Ford (Negative cycles).
// - Intuition: Back-edge collision records start and end vertices; backtracking through parent pointers recovers the exact cyclic path sequence.
// - Complexity: Time: O(V + E) for DFS, O(V * E) for Bellman-Ford, Space: O(V) for visited and parent arrays.


// =========================================================
// 27. MAKE ARRAY NON-DECREASING (SLOPE TRICK)
// =========================================================

ll makeArrayNonDecreasing(const vi &nums) {
    ll total_cost = 0;
    priority_queue<int> max_heap;

    for (int x : nums) {
        max_heap.push(x);
        if (max_heap.top() > x) {
            total_cost += max_heap.top() - x;
            max_heap.pop();
            max_heap.push(x);
        }
    }
    return total_cost;
}
// Interview Explanation:
// - Problem Statement: Find minimum operations to make an array non-decreasing where each step can increment or decrement an element by 1.
// - Approach: Greedy Slope Trick using a Max-Heap.
// - Intuition: If current element x < heap.top(), moving top down to x minimizes cost; push x twice (once for slope inflection point, once for value adjustment).
// - Complexity: Time: O(N \log N) heap operations, Space: O(N) for priority queue.

