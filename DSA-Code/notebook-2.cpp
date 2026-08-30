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
 | 2  | Modular Multiplicative Inverse              | Fermat's Little Theorem (MOD - 2) | O(log MOD | O(1)     |
 | 3  | Factorials & Combinatorics (nCr, nPr)       | Factorial & Inverse Fac Arrays    | O(N)+O(1) | O(N)     |
 | 4  | Sieve of Eratosthenes                       | Multiples Cross-Off               | O(Nloglog | O(N)     |
 | 5  | Prime Factorization                         | Trial Division up to sqrt(N)      | O(sqrt N) | O(log N) |
 | 6  | Prefix XOR (1 to N)                         | Periodicity of 4 in Binary XOR    | O(1)     | O(1)     |
 | 7  | Sliding Window Maximum & Minimum            | Monotonic Double-Ended Queue (DQ) | O(N)     | O(K)     |
 | 8  | Longest Increasing Subsequence (LIS)        | Patience Sorting (lower_bound)    | O(N logN) | O(N)     |
 | 9  | Longest Common Subsequence (Reconstruction) | 2D Dynamic Programming Table      | O(N * M) | O(N * M) |
 | 10 | Interval DP Template                        | DP over Subproblem Lengths        | O(N^3)   | O(N^2)   |
 | 11 | Subset Enumeration & Sum of All Subsets     | Bitmask Enumeration [0...2^N - 1] | O(N * 2^N | O(N * 2^N |
 | 12 | Kadane's Max Subarray Sum with Indices      | Running Prefix Sum Reset          | O(N)     | O(1)     |
 | 13 | Permutation Generation                      | Bitmask Backtracking              | O(N! * N) | O(N! * N) |
 | 14 | Longest Palindromic Subsequence (LPS)       | Interval DP / LCS with Reverse    | O(N^2)   | O(N^2)   |
 | 15 | Check Subsequence                           | Greedy Two-Pointer Linear Scan    | O(       | S        |
 | 16 | Precompute Palindromic Subarrays Table      | 2D Interval DP over Lengths       | O(N^2)   | O(N^2)   |
 | 17 | Booth's Algorithm (Min String Rotation)     | Two-Pointer Mismatch on (S + S)   | O(N)     | O(N)     |
 | 18 | Minimum Excluded Value (MEX)                | Hash Set Presence Lookup          | O(N)     | O(N)     |
 | 19 | Matrix Multiplication & Exponentiation      | Binary Exponentiation on Matrices | O(N^3logB | O(N^2)   |
 | 20 | N-th Fibonacci (Matrix Exponentiation)      | [[1, 1], [1, 0]] Matrix Power     | O(log N) | O(1)     |
 | 21 | Total Area Covered by Two 2D Rectangles     | Inclusion-Exclusion Geometry      | O(1)     | O(1)     |
 | 22 | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n        | O(N)     | O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. BINARY EXPONENTIATION (MODULAR POWER)
// ============================================================

ll power(ll a, ll b) {
    ll res = 1; a %= MOD;
    while (b > 0) { if (b & 1) res = (res * a) % MOD; a = (a * a) % MOD; b >>= 1; }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute (a^b) % MOD in logarithmic time using binary exponentiation.
// - Approach: Exponentiation by squaring (Bitwise power decomposition).
// - Intuition: If current bit is set (b & 1), multiply accumulator; square base at each bit shift.
// - Complexity: Time: O(log B), Space: O(1).

// ============================================================
// 2. MODULAR MULTIPLICATIVE INVERSE
// ============================================================

ll inv(ll x) { return power(x, MOD - 2); }
// Interview Explanation:
// - Problem Statement: Compute the modular multiplicative inverse of x modulo prime MOD.
// - Approach: Fermat's Little Theorem (x^(MOD - 2) % MOD).
// - Intuition: For prime MOD, x^(MOD - 1) = 1 (mod MOD), so x * x^(MOD - 2) = 1 (mod MOD).
// - Complexity: Time: O(log MOD), Space: O(1).

// ============================================================
// 3. FACTORIALS & COMBINATORICS (nCr, nPr)
// ============================================================

vl fac, ifac;

void init_nCr(int n) {
    fac.resize(n + 1); ifac.resize(n + 1); fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = (fac[i - 1] * i) % MOD;
    ifac[n] = inv(fac[n]);
    for (int i = n; i > 0; i--) ifac[i - 1] = (ifac[i] * i) % MOD;
}

ll nCr(int n, int r) { return (r < 0 || r > n) ? 0 : fac[n] * ifac[r] % MOD * ifac[n - r] % MOD; }
ll nPr(int n, int r) { return (r < 0 || r > n) ? 0 : fac[n] * ifac[n - r] % MOD; }
// Interview Explanation:
// - Problem Statement: Compute combinations nCr and permutations nPr modulo 1e9 + 7 in O(1) query time.
// - Approach: Precomputing factorials and inverse factorials in O(N).
// - Intuition: Precompute factorials up to N; compute ifac[N] = inv(fac[N]), then iteratively populate backwards.
// - Complexity: Time: O(N) build, O(1) query, Space: O(N).

// ============================================================
// 4. SIEVE OF ERATOSTHENES
// ============================================================

vi sieve(int n) {
    vector<bool> is_prime(n + 1, true); is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; i++) if (is_prime[i]) for (int j = i * i; j <= n; j += i) is_prime[j] = false;
    vi primes;
    for (int i = 2; i <= n; i++) if (is_prime[i]) primes.push_back(i);
    return primes;
}
// Interview Explanation:
// - Problem Statement: Generate all prime numbers up to N.
// - Approach: Classical Sieve of Eratosthenes.
// - Intuition: Cross off multiples of each prime starting at i * i; unmarked numbers are primes.
// - Complexity: Time: O(N \log \log N), Space: O(N).

// ============================================================
// 5. PRIME FACTORIZATION
// ============================================================

vector<pii> prime_factorize(int n) {
    vector<pii> pf;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) { n /= p; cnt++; }
            pf.push_back({p, cnt});
        }
    }
    if (n > 1) pf.push_back({n, 1});
    return pf;
}
// Interview Explanation:
// - Problem Statement: Find all distinct prime factors and their multiplicities for an integer N.
// - Approach: Trial division up to sqrt(N).
// - Intuition: Composite number N must have at least one prime factor <= sqrt(N); divide out factors iteratively.
// - Complexity: Time: O(sqrt(N)), Space: O(log N).

// ============================================================
// 6. PREFIX XOR (1 TO N)
// ============================================================

int XORupto(int n) {
    int rem = n % 4;
    return rem == 0 ? n : (rem == 1 ? 1 : (rem == 2 ? n + 1 : 0));
}
// Interview Explanation:
// - Problem Statement: Compute cumulative XOR sum from 1 to N in O(1) time.
// - Approach: Periodicity of 4 in binary XOR sums.
// - Intuition: XORing 4 consecutive numbers (4k..4k+3) yields 0; answer depends solely on N % 4.
// - Complexity: Time: O(1), Space: O(1).

// ============================================================
// 7. SLIDING WINDOW MAXIMUM & MINIMUM
// ============================================================

vi maxSlidingWindow(const vi &nums, int k) {
    int n = nums.size(); deque<int> dq; vi ans;
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
    int n = nums.size(); deque<int> dq; vi ans;
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
// - Problem Statement: Find maximum and minimum element in every sliding window of size K.
// - Approach: Monotonic Double-Ended Queue (Deque).
// - Intuition: Maintain indices in decreasing/increasing order; remove expired from front and suboptimal from back.
// - Complexity: Time: O(N), Space: O(K).

// ============================================================
// 8. LONGEST INCREASING SUBSEQUENCE (LENGTH & RECONSTRUCTION)
// ============================================================

int lisLength(const vi &a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x); else *it = x;
    }
    return dp.size();
}

vi lisReconstruct(const vi &a) {
    int n = a.size(); vi dp, pos, parent(n, -1);
    for (int i = 0; i < n; i++) {
        int x = a[i]; auto it = lower_bound(dp.begin(), dp.end(), x);
        int j = it - dp.begin();
        if (it == dp.end()) { dp.push_back(x); pos.push_back(i); } else { *it = x; pos[j] = i; }
        if (j > 0) parent[i] = pos[j - 1];
    }
    vi ans; int cur = pos.back();
    while (cur != -1) { ans.push_back(a[cur]); cur = parent[cur]; }
    reverse(ans.begin(), ans.end()); return ans;
}
// Interview Explanation:
// - Problem Statement: Find length and reconstruct Longest Increasing Subsequence path.
// - Approach: Patience Sorting (std::lower_bound) + Predecessor Parent Array.
// - Intuition: Maintain smallest tail values; record predecessor index pos[j - 1] at each step.
// - Complexity: Time: O(N \log N), Space: O(N).

// ============================================================
// 9. LONGEST COMMON SUBSEQUENCE (RECONSTRUCTION)
// ============================================================

string lcs(const string &a, const string &b) {
    int n = a.size(), m = b.size(); vvi dp(n + 1, vi(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = (a[i - 1] == b[j - 1]) ? dp[i - 1][j - 1] + 1 : max(dp[i - 1][j], dp[i][j - 1]);
    string res; int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) { res += a[i - 1]; i--; j--; }
        else if (dp[i - 1][j] > dp[i][j - 1]) i--;
        else j--;
    }
    reverse(res.begin(), res.end()); return res;
}
// Interview Explanation:
// - Problem Statement: Find and reconstruct Longest Common Subsequence of two strings.
// - Approach: 2D Dynamic Programming table + Backtrack path reconstruction.
// - Intuition: Backtrack from (N, M) following match diagonals or max transitions.
// - Complexity: Time: O(N * M), Space: O(N * M).

// ============================================================
// 10. INTERVAL DP TEMPLATE
// ============================================================

ll solveIntervalDPBottomUp(int n) {
    vvl dp(n, vl(n, 0));
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1; dp[l][r] = 1e18;
            for (int k = l; k < r; k++) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Compute minimum cost to merge/partition an interval from 0 to N-1.
// - Approach: Interval Dynamic Programming ordered by subproblem length.
// - Intuition: Solve subproblems of length 2 to N; partition at k in [l, r-1]: dp[l][r] = min(dp[l][k] + dp[k+1][r]).
// - Complexity: Time: O(N^3), Space: O(N^2).

// ============================================================
// 11. SUBSET ENUMERATION & SUM OF ALL SUBSETS
// ============================================================

vvi generateSubsets(const vi &nums) {
    int n = nums.size(); vvi subsets;
    for (int mask = 0; mask < (1 << n); mask++) {
        vi sub; for (int i = 0; i < n; i++) if (mask & (1 << i)) sub.push_back(nums[i]);
        subsets.push_back(sub);
    }
    return subsets;
}

int sumOfAllSubsets(const vi &nums) {
    int n = nums.size(), total = 0;
    for (int mask = 0; mask < (1 << n); mask++)
        for (int i = 0; i < n; i++) if (mask & (1 << i)) total += nums[i];
    return total;
}
// Interview Explanation:
// - Problem Statement: Generate all 2^N subsets of an array and compute sum of all subset sums.
// - Approach: Bitmask Enumeration [0...2^N - 1].
// - Intuition: Bitmask integer i represents subset where j-th bit indicates inclusion of nums[j].
// - Complexity: Time: O(N * 2^N), Space: O(N * 2^N).

// ============================================================
// 12. KADANE'S MAXIMUM SUBARRAY SUM WITH INDICES
// ============================================================

vi kadane(const vi &nums) {
    int n = nums.size(), max_sum = -2e9, cur_sum = 0, start = 0, end = 0, temp_start = 0;
    for (int i = 0; i < n; i++) {
        cur_sum += nums[i];
        if (cur_sum > max_sum) { max_sum = cur_sum; start = temp_start; end = i; }
        if (cur_sum < 0) { cur_sum = 0; temp_start = i + 1; }
    }
    return {max_sum, start, end};
}
// Interview Explanation:
// - Problem Statement: Find maximum sum contiguous subarray with start and end indices.
// - Approach: Kadane's Algorithm with running prefix reset.
// - Intuition: Accumulate sum; if current sum drops below 0, reset start pointer to next index.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 13. PERMUTATION GENERATION (BITMASK BACKTRACKING)
// ============================================================

void solvePerm(const vi &nums, vi &cur, vvi &ans, int mask) {
    if (cur.size() == nums.size()) { ans.push_back(cur); return; }
    for (int i = 0; i < (int)nums.size(); i++) {
        if (!(mask & (1 << i))) {
            cur.push_back(nums[i]); solvePerm(nums, cur, ans, mask | (1 << i)); cur.pop_back();
        }
    }
}

vvi generatePermutations(const vi &nums) {
    vvi ans; vi cur; solvePerm(nums, cur, ans, 0); return ans;
}
// Interview Explanation:
// - Problem Statement: Generate all N! permutations of an array.
// - Approach: Backtracking with a bitmask of used elements.
// - Intuition: Place every unused element at current position, recurse, and backtrack.
// - Complexity: Time: O(N! * N), Space: O(N! * N).

// ============================================================
// 14. LONGEST PALINDROMIC SUBSEQUENCE (LPS)
// ============================================================

int lps(const string &s) {
    int n = s.size(); vvi dp(n, vi(n, 0));
    for (int i = 0; i < n; i++) dp[i][i] = 1;
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            dp[l][r] = (s[l] == s[r]) ? 2 + (len > 2 ? dp[l + 1][r - 1] : 0) : max(dp[l + 1][r], dp[l][r - 1]);
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find length of Longest Palindromic Subsequence of a string.
// - Approach: Interval DP / LCS of string with reverse.
// - Intuition: If s[l] == s[r], dp[l][r] = 2 + dp[l+1][r-1]; otherwise max(dp[l+1][r], dp[l][r-1]).
// - Complexity: Time: O(N^2), Space: O(N^2).

// ============================================================
// 15. CHECK SUBSEQUENCE
// ============================================================

bool checkSubsequence(const string &s, const string &t) {
    int n = s.size(), m = t.size(), i = 0, j = 0;
    while (i < n && j < m) { if (s[i] == t[j]) j++; i++; }
    return j == m;
}
// Interview Explanation:
// - Problem Statement: Determine whether string t is a subsequence of string s.
// - Approach: Greedy Two-Pointer linear scan.
// - Intuition: Advance pointer in s searching for next required character of t.
// - Complexity: Time: O(|S|), Space: O(1).

// ============================================================
// 16. PRECOMPUTE PALINDROMIC SUBARRAYS TABLE
// ============================================================

vvi palindromeTable(const vi &nums) {
    int n = nums.size(); vvi is_pal(n, vi(n, 0));
    for (int i = 0; i < n; i++) is_pal[i][i] = 1;
    for (int len = 2; len <= n; len++)
        for (int l = 0; l + len <= n; l++) {
            int r = l + len - 1;
            is_pal[l][r] = (nums[l] == nums[r]) ? ((len == 2) ? 1 : is_pal[l + 1][r - 1]) : 0;
        }
    return is_pal;
}
// Interview Explanation:
// - Problem Statement: Precompute 2D table indicating whether substring s[l...r] is a palindrome.
// - Approach: Interval Dynamic Programming over substring lengths.
// - Intuition: is_pal[l][r] = (nums[l] == nums[r]) && is_pal[l+1][r-1].
// - Complexity: Time: O(N^2), Space: O(N^2).

// ============================================================
// 17. BOOTH'S ALGORITHM (MINIMUM STRING ROTATION)
// ============================================================

string boothAlgorithm(const string &s) {
    string t = s + s; int n = s.size(), i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) k++;
        else if (t[i + k] < t[j + k]) { j += k + 1; if (i == j) j++; k = 0; }
        else { i += k + 1; if (i == j) i++; k = 0; }
    }
    return t.substr(min(i, j), n);
}
// Interview Explanation:
// - Problem Statement: Find lexicographically smallest circular rotation of a string.
// - Approach: Booth's Algorithm (Two-pointer rotation comparison on doubled string).
// - Intuition: Compare candidate rotations i and j on s + s; skip past mismatch offset k.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 18. MINIMUM EXCLUDED VALUE (MEX)
// ============================================================

int mex(const vi &nums) {
    unordered_set<int> st(nums.begin(), nums.end()); int val = 0;
    while (st.count(val)) val++;
    return val;
}
// Interview Explanation:
// - Problem Statement: Find smallest non-negative integer absent from array.
// - Approach: Hash Set presence lookup starting from 0.
// - Intuition: Increment counter from 0 until first missing integer in set.
// - Complexity: Time: O(N) average, Space: O(N).

// ============================================================
// 19. MATRIX MULTIPLICATION & MATRIX EXPONENTIATION
// ============================================================

vvi multiplyMatrices(const vvi &A, const vvi &B) {
    int n = A.size(), m = B[0].size(), p = B.size(); vvi C(n, vi(m, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < p; k++)
            for (int j = 0; j < m; j++) C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % MOD;
    return C;
}

vvi powerMatrices(vvi A, ll b) {
    int n = A.size(); vvi res(n, vi(n, 0));
    for (int i = 0; i < n; i++) res[i][i] = 1;
    while (b > 0) {
        if (b & 1) res = multiplyMatrices(res, A);
        A = multiplyMatrices(A, A); b >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Multiply and compute the power of N x N matrices modulo 1e9 + 7.
// - Approach: Binary Exponentiation on Matrices.
// - Intuition: Repeatedly square transformation matrix with identity base.
// - Complexity: Time: O(N^3 \log B), Space: O(N^2).

// ============================================================
// 20. N-TH FIBONACCI (MATRIX EXPONENTIATION)
// ============================================================

ll nthFibonacci(ll n) {
    if (n == 1) return 1 % MOD;
    if (n == 2) return 2 % MOD;
    vvi F = {{1, 1}, {1, 0}};
    vvi res = powerMatrices(F, n - 2);
    return (2LL * res[0][0] + res[0][1]) % MOD;
}
// Interview Explanation:
// - Problem Statement: Compute N-th Fibonacci number modulo 1e9 + 7 in logarithmic time.
// - Approach: Matrix Exponentiation on [[1, 1], [1, 0]].
// - Intuition: Recurrence [F(n), F(n-1)]^T = [[1, 1], [1, 0]]^(n-2) * [F(2), F(1)]^T.
// - Complexity: Time: O(\log N), Space: O(1).

// ============================================================
// 21. TOTAL AREA COVERED BY TWO 2D RECTANGLES
// ============================================================

int computeArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int area1 = (x2 - x1) * (y2 - y1), area2 = (x4 - x3) * (y4 - y3);
    int w = max(0, min(x2, x4) - max(x1, x3)), h = max(0, min(y2, y4) - max(y1, y3));
    return area1 + area2 - (w * h);
}
// Interview Explanation:
// - Problem Statement: Find total 2D area covered by two rectilinear rectangles.
// - Approach: 2D Geometry Inclusion-Exclusion Principle.
// - Intuition: Total area = Area(A) + Area(B) - Overlap(A, B).
// - Complexity: Time: O(1), Space: O(1).

// ============================================================
// 22. BALANCED PARENTHESES STRINGS COUNT (CATALAN NUMBER)
// ============================================================

ll countOfBalancedParentheses(int len) {
    if (len % 2 != 0) return 0;
    int n = len / 2; init_nCr(2 * n);
    return nCr(2 * n, n) * inv(n + 1) % MOD;
}
// Interview Explanation:
// - Problem Statement: Count valid balanced parentheses strings of length LEN.
// - Approach: Catalan Number formula C_n = (1 / (n + 1)) * (2n choose n) where n = LEN / 2.
// - Intuition: Balanced string corresponds to Dyck path enumerated by Catalan numbers.
// - Complexity: Time: O(N) precomputation, Space: O(N).
