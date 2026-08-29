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
 | 1  | Binary Exponentiation (Modular Power)       | Bitwise Repeated Squaring         | O(log B) | O(1)     |
 | 2  | Modular Multiplicative Inverse              | Fermat's Little Theorem (MOD - 2) | O(log MOD| O(1)     |
 | 3  | Factorials & Combinatorics (nCr, nPr)       | Factorial & Inverse Fac Arrays    | O(N)+O(1)| O(N)     |
 | 4  | Sieve of Eratosthenes                       | Multiples Cross-Off               | O(Nloglog| O(N)     |
 | 5  | Prime Factorization                         | Trial Division up to sqrt(N)      | O(sqrt N)| O(log N) |
 | 6  | Prefix XOR (1 to N)                         | Periodicity of 4 in Binary XOR    | O(1)     | O(1)     |
 | 7  | Sliding Window Maximum & Minimum            | Monotonic Double-Ended Queue (DQ) | O(N)     | O(K)     |
 | 8  | Max Subarray Sum of Length at Most K        | Prefix Sums + Monotonic Deque     | O(N)     | O(N)     |
 | 9  | Longest Increasing Subsequence (LIS)        | Patience Sorting (lower_bound)    | O(N logN)| O(N)     |
 | 10 | Longest Common Subsequence (Reconstruction) | 2D Dynamic Programming Table      | O(N * M) | O(N * M) |
 | 11 | Interval DP Template                        | DP over Subproblem Lengths        | O(N^3)   | O(N^2)   |
 | 12 | Subset Enumeration & Sum of All Subsets     | Bitmask Enumeration [0...2^N - 1] | O(N * 2^N| O(N * 2^N|
 | 13 | Kadane's Max Subarray Sum with Indices      | Running Prefix Sum Reset          | O(N)     | O(1)     |
 | 14 | Permutation Generation                      | Bitmask Backtracking              | O(N! * N)| O(N! * N)|
 | 15 | Longest Palindromic Subsequence (LPS)       | Interval DP / LCS with Reverse    | O(N^2)   | O(N^2)   |
 | 16 | Check Subsequence                           | Greedy Two-Pointer Linear Scan    | O(|S|)   | O(1)     |
 | 17 | Precompute Palindromic Subarrays Table      | 2D Interval DP over Lengths       | O(N^2)   | O(N^2)   |
 | 18 | Booth's Algorithm (Min String Rotation)     | Two-Pointer Mismatch on (S + S)   | O(N)     | O(N)     |
 | 19 | Minimum Excluded Value (MEX)                | Hash Set Presence Lookup          | O(N)     | O(N)     |
 | 20 | Inversion Count (a[i] > x * a[j])           | Divide & Conquer Merge Sort Count | O(N logN)| O(N)     |
 | 21 | Matrix Multiplication & Exponentiation      | Binary Exponentiation on Matrices | O(N^3logB| O(N^2)   |
 | 22 | N-th Fibonacci (Matrix Exponentiation)      | [[1, 1], [1, 0]] Matrix Power     | O(log N) | O(1)     |
 | 23 | Total Area Covered by Two 2D Rectangles    | Inclusion-Exclusion Geometry      | O(1)     | O(1)     |
 | 24 | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n        | O(N)     | O(N)     |
 ====================================================================================================
*/


// =========================================================
// 1. BINARY EXPONENTIATION (MODULAR POWER)
// =========================================================

ll power(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    while (b > 0) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute (a^b) % MOD in logarithmic time using binary exponentiation.
// - Approach: Exponentiation by squaring (Bitwise power decomposition).
// - Intuition: If current power bit is set (b & 1), multiply accumulator by base; square base at each bit shift.
// - Complexity: Time: O(log B), Space: O(1) auxiliary space.


// =========================================================
// 2. MODULAR MULTIPLICATIVE INVERSE
// =========================================================

ll inv(ll x) {
    return power(x, MOD - 2);
}
// Interview Explanation:
// - Problem Statement: Compute the modular multiplicative inverse of x modulo prime MOD.
// - Approach: Fermat's Little Theorem (x^(MOD - 2) % MOD).
// - Intuition: For prime MOD, x^(MOD - 1) = 1 (mod MOD), so x * x^(MOD - 2) = 1 (mod MOD); computing power(x, MOD - 2) yields the inverse.
// - Complexity: Time: O(log MOD), Space: O(1) auxiliary space.


// =========================================================
// 3. FACTORIALS & COMBINATORICS (nCr, nPr)
// =========================================================

vl fac, ifac;

void init_nCr(int n) {
    fac.resize(n + 1);
    ifac.resize(n + 1);
    fac[0] = 1;
    for (int i = 1; i <= n; i++) {
        fac[i] = (fac[i - 1] * i) % MOD;
    }
    ifac[n] = inv(fac[n]);
    for (int i = n; i > 0; i--) {
        ifac[i - 1] = (ifac[i] * i) % MOD;
    }
}

ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[r] % MOD * ifac[n - r] % MOD;
}

ll nPr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[n - r] % MOD;
}
// Interview Explanation:
// - Problem Statement: Compute combinations nCr and permutations nPr modulo 1e9 + 7 in O(1) query time.
// - Approach: Precomputing factorials and inverse factorials in O(N).
// - Intuition: Precompute factorials up to N; compute ifac[N] = inv(fac[N]), then iteratively populate backwards via ifac[i-1] = (ifac[i] * i) % MOD.
// - Complexity: Time: O(N) precomputation, O(1) per query, Space: O(N) for fac and ifac vectors.


// =========================================================
// 4. SIEVE OF ERATOSTHENES
// =========================================================

vi sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    vi primes;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) primes.push_back(i);
    }
    return primes;
}
// Interview Explanation:
// - Problem Statement: Generate all prime numbers up to N.
// - Approach: Classical Sieve of Eratosthenes.
// - Intuition: Cross off multiples of each prime starting at i * i; numbers that remain unmarked are primes.
// - Complexity: Time: O(N \log \log N), Space: O(N) boolean array.


// =========================================================
// 5. PRIME FACTORIZATION
// =========================================================

vector<pii> prime_factorize(int n) {
    vector<pii> pf;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                cnt++;
            }
            pf.push_back({p, cnt});
        }
    }
    if (n > 1) pf.push_back({n, 1});
    return pf;
}
// Interview Explanation:
// - Problem Statement: Find all distinct prime factors and their multiplicities for an integer N.
// - Approach: Trial division up to sqrt(N).
// - Intuition: Any composite number N must have at least one prime factor <= sqrt(N); dividing out factors reduces N, leaving at most one prime > sqrt(N).
// - Complexity: Time: O(sqrt(N)), Space: O(log N) for factor pairs.


// =========================================================
// 6. PREFIX XOR (1 TO N)
// =========================================================

int XORupto(int n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        case 3: return 0;
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Compute the cumulative XOR sum of all integers from 1 to N in O(1) time.
// - Approach: Periodicity of 4 in binary XOR sums.
// - Intuition: XORing four consecutive numbers (4k, 4k+1, 4k+2, 4k+3) always yields 0; answer depends only on N % 4.
// - Complexity: Time: O(1) constant time, Space: O(1) auxiliary space.


// =========================================================
// 7. SLIDING WINDOW MAXIMUM & MINIMUM
// =========================================================

vi maxSlidingWindow(const vi &nums, int k) {
    int n = nums.size();
    deque<int> dq;
    vi ans;

    for (int r = 0; r < n; r++) {
        int l = r - k + 1;
        while (!dq.empty() && dq.front() < l) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[r]) dq.pop_back();
        dq.push_back(r);
        if (l >= 0) ans.push_back(nums[dq.front()]);
    }
    return ans;
}

vi minSlidingWindow(const vi &nums, int k) {
    int n = nums.size();
    deque<int> dq;
    vi ans;

    for (int r = 0; r < n; r++) {
        int l = r - k + 1;
        while (!dq.empty() && dq.front() < l) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] >= nums[r]) dq.pop_back();
        dq.push_back(r);
        if (l >= 0) ans.push_back(nums[dq.front()]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum (or minimum) element in every sliding window of size K.
// - Approach: Monotonic Double-Ended Queue (Deque).
// - Intuition: Maintain indices in decreasing (or increasing) order of values; remove expired indices from front and worse elements from back before pushing.
// - Complexity: Time: O(N) each index pushed/popped at most once, Space: O(K) for deque.


// =========================================================
// 8. MAXIMUM SUBARRAY SUM OF LENGTH AT MOST K
// =========================================================

ll maxSubarraySumAtMostK(const vi &nums, int k) {
    int n = nums.size();
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + nums[i];
    }
    deque<int> dq;
    ll ans = -1e18;
    for (int r = 0; r < n; r++) {
        int l = r - k + 1;
        while (!dq.empty() && dq.front() < l) dq.pop_front();
        while (!dq.empty() && pref[dq.back()] >= pref[r]) dq.pop_back();
        dq.push_back(r);
        ans = max(ans, pref[r + 1] - pref[dq.front()]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum subarray sum among all contiguous subarrays of length at most K.
// - Approach: Prefix sums + Monotonic Deque.
// - Intuition: Subarray sum is pref[r+1] - pref[l]; to maximize for a fixed r, minimize pref[l] where l in [r - k + 1, r] using a sliding window min deque.
// - Complexity: Time: O(N) linear time, Space: O(N) for prefix array and deque.


// =========================================================
// 9. LONGEST INCREASING SUBSEQUENCE (LENGTH & RECONSTRUCTION)
// =========================================================

int lisLength(const vi &a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x);
        else *it = x;
    }
    return dp.size();
}

vi lisReconstruct(const vi &a) {
    int n = a.size();
    vi dp, pos, parent(n, -1);

    for (int i = 0; i < n; i++) {
        int x = a[i];
        auto it = lower_bound(dp.begin(), dp.end(), x);
        int j = it - dp.begin();

        if (it == dp.end()) {
            dp.push_back(x);
            pos.push_back(i);
        } else {
            *it = x;
            pos[j] = i;
        }
        if (j > 0) parent[i] = pos[j - 1];
    }

    vi ans;
    int cur = pos.back();
    while (cur != -1) {
        ans.push_back(a[cur]);
        cur = parent[cur];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the length and lexicographical path of the Longest Increasing Subsequence (LIS).
// - Approach: Patience Sorting (std::lower_bound) + Predecessor Parent Array.
// - Intuition: Maintain smallest tail values of all increasing subsequences; record predecessor index pos[j - 1] at each replacement to reconstruct path backwards.
// - Complexity: Time: O(N \log N) binary search, Space: O(N) for tail and parent arrays.


// =========================================================
// 10. LONGEST COMMON SUBSEQUENCE (RECONSTRUCTION)
// =========================================================

string lcs(const string &a, const string &b) {
    int n = a.size(), m = b.size();
    vvi dp(n + 1, vi(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    string res;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            res += a[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    reverse(res.begin(), res.end());
    return res;
}
// Interview Explanation:
// - Problem Statement: Find and reconstruct the Longest Common Subsequence string of two strings.
// - Approach: 2D Dynamic Programming table + Diagonal/Backtrack path reconstruction.
// - Intuition: Compute 2D table; backtrack from (N, M) following match diagonals or max transitions, then reverse accumulated result.
// - Complexity: Time: O(N * M) table fill and backtrack, Space: O(N * M) for DP matrix.


// =========================================================
// 11. INTERVAL DP TEMPLATE
// =========================================================

ll solveIntervalDPBottomUp(int n) {
    vvl dp(n, vl(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            dp[l][r] = 1e18;
            for (int k = l; k < r; k++) {
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
            }
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Compute minimum cost to merge/partition an interval from 0 to N-1 (Matrix Chain / Optimal BST pattern).
// - Approach: Interval Dynamic Programming ordered by subproblem length.
// - Intuition: Solve subproblems of length 2 to N; for range [l, r], iterate all partition points k in [l, r-1]: dp[l][r] = min(dp[l][k] + dp[k+1][r]).
// - Complexity: Time: O(N^3) triple nested loops, Space: O(N^2) for DP table.


// =========================================================
// 12. SUBSET ENUMERATION & SUM OF ALL SUBSETS
// =========================================================

vvi generateSubsets(const vi &nums) {
    int n = nums.size();
    vvi subsets;

    for (int mask = 0; mask < (1 << n); mask++) {
        vi sub;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) sub.push_back(nums[i]);
        }
        subsets.push_back(sub);
    }
    return subsets;
}

int sumOfAllSubsets(const vi &nums) {
    int n = nums.size();
    int total = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        int sub_sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) sub_sum += nums[i];
        }
        total += sub_sum;
    }
    return total;
}
// Interview Explanation:
// - Problem Statement: Generate all 2^N subsets of an array and compute the sum of all subset sums.
// - Approach: Bitmask Enumeration [0...2^N - 1].
// - Intuition: Bitmask integer i represents a subset where the j-th bit indicates whether nums[j] is included.
// - Complexity: Time: O(N * 2^N), Space: O(N * 2^N) for subset storage.


// =========================================================
// 13. KADANE'S MAXIMUM SUBARRAY SUM WITH INDICES
// =========================================================

vi kadane(const vi &nums) {
    int n = nums.size();
    int max_sum = -2e9, cur_sum = 0;
    int start = 0, end = 0, temp_start = 0;

    for (int i = 0; i < n; i++) {
        cur_sum += nums[i];
        if (cur_sum > max_sum) {
            max_sum = cur_sum;
            start = temp_start;
            end = i;
        }
        if (cur_sum < 0) {
            cur_sum = 0;
            temp_start = i + 1;
        }
    }
    return {max_sum, start, end};
}
// Interview Explanation:
// - Problem Statement: Find the maximum sum contiguous subarray along with its start and end indices.
// - Approach: Kadane's Algorithm with running prefix reset.
// - Intuition: Accumulate current sum; if it exceeds global max, update indices; if current sum drops below 0, reset start pointer to next index.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.


// =========================================================
// 14. PERMUTATION GENERATION (BITMASK BACKTRACKING)
// =========================================================

void solvePerm(const vi &nums, vi &cur, vvi &ans, int mask) {
    if (cur.size() == nums.size()) {
        ans.push_back(cur);
        return;
    }
    for (int i = 0; i < (int)nums.size(); i++) {
        if (!(mask & (1 << i))) {
            cur.push_back(nums[i]);
            solvePerm(nums, cur, ans, mask | (1 << i));
            cur.pop_back();
        }
    }
}

vvi generatePermutations(const vi &nums) {
    vvi ans;
    vi cur;
    solvePerm(nums, cur, ans, 0);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Generate all N! permutations of an array.
// - Approach: Backtracking with a bitmask of used elements.
// - Intuition: Try placing every unused element at the current position, recurse with updated bitmask, and backtrack.
// - Complexity: Time: O(N! * N), Space: O(N! * N) for permutation storage.


// =========================================================
// 15. LONGEST PALINDROMIC SUBSEQUENCE (LPS)
// =========================================================

int lps(const string &s) {
    int n = s.size();
    vvi dp(n, vi(n, 0));

    for (int i = 0; i < n; i++) dp[i][i] = 1;

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            if (s[l] == s[r]) dp[l][r] = 2 + (len > 2 ? dp[l + 1][r - 1] : 0);
            else dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find the length of the Longest Palindromic Subsequence of a string.
// - Approach: Interval DP / LCS of string with its reverse.
// - Intuition: If s[l] == s[r], dp[l][r] = 2 + dp[l+1][r-1]; otherwise take max(dp[l+1][r], dp[l][r-1]).
// - Complexity: Time: O(N^2) interval DP, Space: O(N^2) for DP table.


// =========================================================
// 16. CHECK SUBSEQUENCE
// =========================================================

bool checkSubsequence(const string &s, const string &t) {
    int n = s.size(), m = t.size();
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (s[i] == t[j]) j++;
        i++;
    }
    return j == m;
}
// Interview Explanation:
// - Problem Statement: Determine whether string t is a subsequence of string s.
// - Approach: Greedy Two-Pointer linear scan.
// - Intuition: Advance pointer in s searching for next required character of t; valid if all characters of t are matched before s is exhausted.
// - Complexity: Time: O(|S|) single pass, Space: O(1) auxiliary space.


// =========================================================
// 17. PRECOMPUTE PALINDROMIC SUBARRAYS TABLE
// =========================================================

vvi palindromeTable(const vi &nums) {
    int n = nums.size();
    vvi is_pal(n, vi(n, 0));

    for (int i = 0; i < n; i++) is_pal[i][i] = 1;

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len <= n; l++) {
            int r = l + len - 1;
            if (nums[l] == nums[r]) is_pal[l][r] = (len == 2) ? 1 : is_pal[l + 1][r - 1];
            else is_pal[l][r] = 0;
        }
    }
    return is_pal;
}
// Interview Explanation:
// - Problem Statement: Precompute a 2D boolean table indicating whether substring s[l...r] is a palindrome in O(1) query time.
// - Approach: Interval Dynamic Programming over substring lengths.
// - Intuition: is_pal[l][r] = (nums[l] == nums[r]) && is_pal[l+1][r-1], building from length 1 and 2 upwards.
// - Complexity: Time: O(N^2) preprocessing, Space: O(N^2) for lookup table.


// =========================================================
// 18. BOOTH'S ALGORITHM (MINIMUM STRING ROTATION)
// =========================================================

string boothAlgorithm(const string &s) {
    string t = s + s;
    int n = s.size();
    int i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) {
            k++;
        } else if (t[i + k] < t[j + k]) {
            j += k + 1;
            if (i == j) j++;
            k = 0;
        } else {
            i += k + 1;
            if (i == j) i++;
            k = 0;
        }
    }
    return t.substr(min(i, j), n);
}
// Interview Explanation:
// - Problem Statement: Find the lexicographically smallest circular rotation of a string.
// - Approach: Booth's Algorithm (Two-pointer rotation comparison on doubled string).
// - Intuition: Compare candidate rotations i and j on s + s; advance the lexicographically larger candidate past the mismatch offset k in linear time.
// - Complexity: Time: O(N) linear time, Space: O(N) for doubled string.


// =========================================================
// 19. MINIMUM EXCLUDED VALUE (MEX)
// =========================================================

int mex(const vi &nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int val = 0;
    while (st.count(val)) val++;
    return val;
}
// Interview Explanation:
// - Problem Statement: Find the Minimum Excluded value (smallest non-negative integer absent from array).
// - Approach: Hash Set presence lookup starting from 0.
// - Intuition: Insert all elements into an unordered set; increment a counter from 0 until encountering the first missing value.
// - Complexity: Time: O(N) average time, Space: O(N) for hash set.


// =========================================================
// 20. INVERSION COUNT (a[i] > x * a[j])
// =========================================================

int countInversions(vi &a, int l, int r, int x) {
    if (l >= r) return 0;
    int m = l + (r - l) / 2;
    int cnt = countInversions(a, l, m, x) + countInversions(a, m + 1, r, x);

    int j = m + 1;
    for (int i = l; i <= m; i++) {
        while (j <= r && a[i] > 1LL * x * a[j]) {
            j++;
        }
        cnt += (j - (m + 1));
    }

    vi temp;
    int i = l;
    j = m + 1;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) temp.push_back(a[i++]);
        else temp.push_back(a[j++]);
    }
    while (i <= m) temp.push_back(a[i++]);
    while (j <= r) temp.push_back(a[j++]);
    copy(temp.begin(), temp.end(), a.begin() + l);
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Count pairs (i, j) such that i < j and a[i] > x * a[j] (Generalized Inversion Count / Reverse Pairs).
// - Approach: Divide-and-Conquer Merge Sort with cross-inversion two-pointer scan.
// - Intuition: Recursively sort halves; before merging, use two pointers across sorted subarrays to count valid cross-pairs in linear time.
// - Complexity: Time: O(N \log N) merge sort, Space: O(N) temporary buffer.


// =========================================================
// 21. MATRIX MULTIPLICATION & MATRIX EXPONENTIATION
// =========================================================

vvi multiplyMatrices(const vvi &A, const vvi &B) {
    int n = A.size(), m = B[0].size(), p = B.size();
    vvi C(n, vi(m, 0));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < p; k++) {
            for (int j = 0; j < m; j++) {
                C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % MOD;
            }
        }
    }
    return C;
}

vvi powerMatrices(vvi A, ll b) {
    int n = A.size();
    vvi res(n, vi(n, 0));
    for (int i = 0; i < n; i++) res[i][i] = 1;

    while (b > 0) {
        if (b & 1) res = multiplyMatrices(res, A);
        A = multiplyMatrices(A, A);
        b >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Multiply and compute the power of N x N matrices modulo 1e9 + 7.
// - Approach: Binary Exponentiation on Matrices.
// - Intuition: Repeatedly square the transformation matrix; identity matrix acts as base multiplier (res[i][i] = 1).
// - Complexity: Time: O(N^3 \log B), Space: O(N^2) for matrix representation.


// =========================================================
// 22. N-TH FIBONACCI (MATRIX EXPONENTIATION)
// =========================================================

ll nthFibonacci(ll n) {
    if (n == 1) return 1 % MOD;
    if (n == 2) return 2 % MOD;

    vvi F = {{1, 1}, {1, 0}};
    vvi res = powerMatrices(F, n - 2);
    return (2LL * res[0][0] + res[0][1]) % MOD;
}
// Interview Explanation:
// - Problem Statement: Compute the N-th Fibonacci number modulo 1e9 + 7 in logarithmic time.
// - Approach: Matrix Exponentiation on [[1, 1], [1, 0]].
// - Intuition: The linear recurrence [F(n), F(n-1)]^T = [[1, 1], [1, 0]] * [F(n-1), F(n-2)]^T computes F(N) in O(log N) matrix powers.
// - Complexity: Time: O(\log N) matrix exponentiation, Space: O(1) auxiliary space.


// =========================================================
// 23. TOTAL AREA COVERED BY TWO 2D RECTANGLES
// =========================================================

int computeArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int area1 = (x2 - x1) * (y2 - y1);
    int area2 = (x4 - x3) * (y4 - y3);
    int w = max(0, min(x2, x4) - max(x1, x3));
    int h = max(0, min(y2, y4) - max(y1, y3));
    return area1 + area2 - (w * h);
}
// Interview Explanation:
// - Problem Statement: Find the total 2D area covered by two rectilinear rectangles that may overlap.
// - Approach: 2D Geometry Inclusion-Exclusion Principle.
// - Intuition: Total area = Area(A) + Area(B) - Overlap(A, B); overlap width and height are clamped intersection lengths.
// - Complexity: Time: O(1) constant time, Space: O(1) auxiliary space.


// =========================================================
// 24. BALANCED PARENTHESES STRINGS COUNT (CATALAN NUMBER)
// =========================================================

ll countOfBalancedParentheses(int len) {
    if (len % 2 != 0) return 0;
    int n = len / 2;
    init_nCr(2 * n);
    return nCr(2 * n, n) * inv(n + 1) % MOD;
}
// Interview Explanation:
// - Problem Statement: Count the number of valid balanced parentheses strings of length LEN.
// - Approach: Catalan Number formula C_n = (1 / (n + 1)) * (2n choose n) where n = LEN / 2.
// - Intuition: A balanced string of length 2n corresponds to a Dyck path that never crosses below the diagonal, enumerated by the n-th Catalan number.
// - Complexity: Time: O(N) precomputing factorials, Space: O(N) for combinatorics arrays.
