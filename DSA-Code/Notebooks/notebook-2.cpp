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

// ============================================================
// HELPER UTILITIES FOR COMBINATORICS & MATRIX POWERS
// ============================================================

ll power(ll a, ll b, ll mod = MOD) {
    ll res = 1; a %= mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

ll inv(ll x, ll mod = MOD) { return power(x, mod - 2, mod); }

vl fac, ifac;
void init_nCr(int n) {
    fac.resize(n + 1); ifac.resize(n + 1); fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = (fac[i - 1] * i) % MOD;
    ifac[n] = inv(fac[n]);
    for (int i = n; i > 0; i--) ifac[i - 1] = (ifac[i] * i) % MOD;
}

ll nCr(int n, int r) { return (r < 0 || r > n) ? 0 : fac[n] * ifac[r] % MOD * ifac[n - r] % MOD; }

vvi multiplyMatrices(const vvi &A, const vvi &B) {
    int n = A.size(), m = B[0].size(), p = B.size();
    vvi C(n, vi(m, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < p; k++)
            for (int j = 0; j < m; j++)
                C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % MOD;
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

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Check Subsequence                           | Greedy Two-Pointer Linear Scan    | O(|S|)   | O(1)     |
 | 2  | Precompute Palindromic Subarrays Table      | 2D Interval DP over Lengths       | O(N^2)   | O(N^2)   |
 | 3  | Minimum Excluded Value (MEX)                | Hash Set Presence Lookup          | O(N) avg | O(N)     |
 | 4  | Matrix Multiplication & Exponentiation      | Binary Exponentiation on Matrices | O(N^3logB| O(N^2)   |
 | 5  | Total Area Covered by Two 2D Rectangles     | Inclusion-Exclusion Geometry      | O(1)     | O(1)     |
 | 6  | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n        | O(N)     | O(N)     |
 | 7  | Double Modular Exponentiation (A^(B^C))     | Fermat's Little Theorem Power Twr | O(log C) | O(1)     |
 | 8  | Counting Divisors for Multiple Queries      | Sieve Divisor Count Array         | O(MAX)   | O(MAX)   |
 | 9  | Maximum GCD of Array Pair (Common Divisors) | Multiples Frequency Bucket Sweep  | O(MAX lg)| O(MAX)   |
 | 10 | Next Prime & Deterministic Primality        | 6k +/- 1 Trial Division Primality | O(sqrt P)| O(1)     |
 | 11 | Derangements Count (Subfactorial !N)        | Recurrence D_n = (n-1)(D_n-1+D_n-2| O(N)     | O(1)     |
 | 12 | Multinomial Permutations Count              | N! / (c1! * c2! * ... * ck!)      | O(N)     | O(N)     |
 | 13 | Stars and Bars Distribution                 | Combinatorics C(N + K - 1, K)     | O(N + K) | O(N + K) |
 | 14 | Paths of Length K in Directed Graph         | Adjacency Matrix Exponentiation   | O(N^3 lg)| O(N^2)   |
 | 15 | Lehmer Code / K-th Permutation & Rank       | Factorial Base Positional Number  | O(N^2)   | O(N)     |
 | 16 | Permutation Orbit Period & LCM Modulo M     | Cycle Decomp + Max Prime Powers   | O(N rt N)| O(N)     |
 | 17 | Inclusion-Exclusion on Prime Multiples      | Bitmask Subset Product PIE        | O(K 2^K) | O(K)     |
 | 18 | General Recurrence Matrix Exponentiation    | Linear Transition State Matrix    | O(K^3 lg)| O(K^2)   |
 ====================================================================================================
*/

// ============================================================
// 1. CHECK SUBSEQUENCE
// ============================================================

bool checkSubsequence(const string &s, const string &t) {
    int n = s.size(), m = t.size(), i = 0, j = 0;
    while (i < n && j < m) {
        if (s[i] == t[j]) j++;
        i++;
    }
    return j == m;
}
// Interview Explanation:
// - Problem Statement: Determine whether string t is a subsequence of string s.
// - Approach: Greedy Two-Pointer linear scan.
// - Intuition: Advance pointer in s searching for next required character of t.
// - Complexity: Time: O(|S|), Space: O(1).


// ============================================================
// 2. PRECOMPUTE PALINDROMIC SUBARRAYS TABLE
// ============================================================

vvi palindromeTable(const vi &nums) {
    int n = nums.size();
    vvi is_pal(n, vi(n, 0));
    for (int i = 0; i < n; i++) is_pal[i][i] = 1;
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len <= n; l++) {
            int r = l + len - 1;
            is_pal[l][r] = (nums[l] == nums[r]) ? ((len == 2) ? 1 : is_pal[l + 1][r - 1]) : 0;
        }
    }
    return is_pal;
}
// Interview Explanation:
// - Problem Statement: Precompute 2D table indicating whether substring s[l...r] is a palindrome.
// - Approach: Interval Dynamic Programming over substring lengths.
// - Intuition: is_pal[l][r] = (nums[l] == nums[r]) && is_pal[l+1][r-1].
// - Complexity: Time: O(N^2), Space: O(N^2).


// ============================================================
// 3. MINIMUM EXCLUDED VALUE (MEX)
// ============================================================

int mex(const vi &nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int val = 0;
    while (st.count(val)) val++;
    return val;
}
// Interview Explanation:
// - Problem Statement: Find smallest non-negative integer absent from array.
// - Approach: Hash Set presence lookup starting from 0.
// - Intuition: Increment counter from 0 until first missing integer in set.
// - Complexity: Time: O(N) average, Space: O(N).


// ============================================================
// 4. MATRIX MULTIPLICATION & MATRIX EXPONENTIATION
// ============================================================
// multiplyMatrices and powerMatrices are defined in the helper utilities above.
// Time Complexity: O(N^3 log B), Space Complexity: O(N^2).


// ============================================================
// 5. TOTAL AREA COVERED BY TWO 2D RECTANGLES
// ============================================================

int computeArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int area1 = (x2 - x1) * (y2 - y1), area2 = (x4 - x3) * (y4 - y3);
    int w = max(0, min(x2, x4) - max(x1, x3));
    int h = max(0, min(y2, y4) - max(y1, y3));
    return area1 + area2 - (w * h);
}
// Interview Explanation:
// - Problem Statement: Find total 2D area covered by two rectilinear rectangles (LC 223).
// - Approach: 2D Geometry Inclusion-Exclusion Principle.
// - Intuition: Total area = Area(A) + Area(B) - Overlap(A, B).
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 6. BALANCED PARENTHESES STRINGS COUNT (CATALAN NUMBER)
// ============================================================

ll countOfBalancedParentheses(int len) {
    if (len % 2 != 0) return 0;
    int n = len / 2;
    init_nCr(2 * n);
    return nCr(2 * n, n) * inv(n + 1) % MOD;
}
// Interview Explanation:
// - Problem Statement: Count valid balanced parentheses strings of length LEN.
// - Approach: Catalan Number formula C_n = (1 / (n + 1)) * (2n choose n) where n = LEN / 2.
// - Intuition: Balanced string corresponds to Dyck path enumerated by Catalan numbers.
// - Complexity: Time: O(N) precomputation, Space: O(N).


// ============================================================
// 7. DOUBLE MODULAR EXPONENTIATION (A^(B^C) MOD M)
// ============================================================

ll powerTower(ll a, ll b, ll c) {
    // By Fermat's Little Theorem: a^(b^c) % MOD = a^( (b^c) % (MOD - 1) ) % MOD
    ll exp = power(b, c, MOD - 1);
    return power(a, exp, MOD);
}
// Interview Explanation:
// - Problem Statement: Compute (a^(b^c)) % (10^9 + 7) (CSES 1712).
// - Approach: Fermat's Little Theorem power reduction: exponent modulo (MOD - 1).
// - Complexity: Time: O(log C + log B), Space: O(1).


// ============================================================
// 8. COUNTING DIVISORS FOR MULTIPLE QUERIES
// ============================================================

vi precomputeDivisorCounts(int maxVal) {
    vi divs(maxVal + 1, 0);
    for (int i = 1; i <= maxVal; i++) {
        for (int j = i; j <= maxVal; j += i) {
            divs[j]++;
        }
    }
    return divs;
}
// Interview Explanation:
// - Problem Statement: Compute number of divisors for multiple queries up to maxVal (CSES 1713).
// - Approach: Harmonic series sieve precomputation.
// - Complexity: Time: O(MAX log MAX) precomputation, O(1) query, Space: O(MAX).


// ============================================================
// 9. MAXIMUM GCD OF ARRAY PAIR (COMMON DIVISORS)
// ============================================================

int maxGCDPair(const vi& nums, int maxVal) {
    vi freq(maxVal + 1, 0);
    for (int x : nums) freq[x]++;
    for (int d = maxVal; d >= 1; d--) {
        int multiples = 0;
        for (int m = d; m <= maxVal; m += d) {
            multiples += freq[m];
        }
        if (multiples >= 2) return d;
    }
    return 1;
}
// Interview Explanation:
// - Problem Statement: Find maximum GCD shared by at least two elements in array (CSES 1081).
// - Approach: Multiples bucket frequency sweep from maxVal down to 1.
// - Complexity: Time: O(MAX log MAX + N), Space: O(MAX).


// ============================================================
// 10. NEXT PRIME & DETERMINISTIC PRIMALITY
// ============================================================

bool isPrimeCheck(ll n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll d = 5; d * d <= n; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return false;
    }
    return true;
}

ll getNextPrime(ll n) {
    ll x = n + 1;
    if (x <= 2) return 2;
    if (x % 2 == 0) x++;
    while (!isPrimeCheck(x)) x += 2;
    return x;
}
// Interview Explanation:
// - Problem Statement: Find smallest prime strictly greater than n (CSES 3396).
// - Approach: 6k +/- 1 trial division primality testing + linear odd-step progression.
// - Complexity: Time: O(sqrt P) per query, Space: O(1).


// ============================================================
// 11. DERANGEMENTS COUNT (SUBFACTORIAL !N)
// ============================================================

ll countDerangements(int n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    ll d0 = 0, d1 = 1;
    for (int i = 3; i <= n; i++) {
        ll cur = (1LL * (i - 1) * ((d0 + d1) % MOD)) % MOD;
        d0 = d1;
        d1 = cur;
    }
    return d1;
}
// Interview Explanation:
// - Problem Statement: Count permutations where no element appears in its original position (CSES 1717).
// - Approach: Subfactorial DP recurrence D_n = (n - 1) * (D_{n-1} + D_{n-2}) mod MOD.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 12. MULTINOMIAL PERMUTATIONS COUNT
// ============================================================

ll multinomialPermutations(const string& s) {
    int n = s.size();
    init_nCr(n);
    vi count(26, 0);
    for (char c : s) count[c - 'a']++;
    ll ans = fac[n];
    for (int i = 0; i < 26; i++) {
        if (count[i] > 1) {
            ans = (ans * ifac[count[i]]) % MOD;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count distinct permutations of string with character duplicates (CSES 1715).
// - Approach: Multinomial formula N! / prod(count[c]!).
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 13. STARS AND BARS COMBINATORIAL DISTRIBUTION
// ============================================================

ll starsAndBars(int n_children, int m_apples) {
    // Distribute m identical objects among n distinct bins: C(n + m - 1, m)
    init_nCr(n_children + m_apples);
    return nCr(n_children + m_apples - 1, m_apples);
}
// Interview Explanation:
// - Problem Statement: Count ways to distribute m identical items to n distinct groups (CSES 1716).
// - Approach: Stars and Bars Theorem: C(n + m - 1, m).
// - Complexity: Time: O(N + M), Space: O(N + M).


// ============================================================
// 14. PATHS OF LENGTH K IN DIRECTED GRAPH
// ============================================================

ll countGraphPathsLengthK(int n, const vvi& adj, ll k, int src, int dest) {
    vvi A = adj;
    vvi P = powerMatrices(A, k);
    return P[src - 1][dest - 1];
}
// Interview Explanation:
// - Problem Statement: Count number of paths of exact length K from node 1 to node N (CSES 1723).
// - Approach: Binary Matrix Exponentiation on Graph Adjacency Matrix (A^K).
// - Complexity: Time: O(N^3 log K), Space: O(N^2).


// ============================================================
// 15. LEHMER CODE / K-TH PERMUTATION & RANK
// ============================================================

vi getKthPermutation(int n, ll k) {
    vl factorials(n + 1, 1);
    for (int i = 1; i <= n; i++) factorials[i] = factorials[i - 1] * i;
    k--; // 0-based
    vi unused(n), res;
    iota(unused.begin(), unused.end(), 1);
    for (int rem = n; rem >= 1; rem--) {
        ll block = factorials[rem - 1];
        int idx = k / block;
        res.push_back(unused[idx]);
        unused.erase(unused.begin() + idx);
        k %= block;
    }
    return res;
}

ll getPermutationRank(const vi& p) {
    int n = p.size();
    vl factorials(n + 1, 1);
    for (int i = 1; i <= n; i++) factorials[i] = factorials[i - 1] * i;
    vi unused(n);
    iota(unused.begin(), unused.end(), 1);
    ll rank = 0;
    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();
        rank += idx * factorials[n - i - 1];
        unused.erase(unused.begin() + idx);
    }
    return rank + 1; // 1-based
}
// Interview Explanation:
// - Problem Statement: Convert between 1-based lexicographical rank and Nth permutation (CSES 3397 / LC 60).
// - Approach: Factorial Number System (Lehmer Code / Positional Division).
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 16. PERMUTATION ORBIT PERIOD & LCM MODULO M
// ============================================================

ll permutationOrbitPeriodLCM(int n, const vi& p) {
    vector<bool> vis(n + 1, false);
    unordered_map<int, int> maxPrimePower;

    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;
        int cur = i, len = 0;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = p[cur];
            len++;
        }
        int temp = len;
        for (int d = 2; d * d <= temp; d++) {
            if (temp % d == 0) {
                int cnt = 0;
                while (temp % d == 0) { temp /= d; cnt++; }
                maxPrimePower[d] = max(maxPrimePower[d], cnt);
            }
        }
        if (temp > 1) maxPrimePower[temp] = max(maxPrimePower[temp], 1);
    }

    ll lcm = 1;
    for (auto [prime, powerVal] : maxPrimePower) {
        lcm = (lcm * power(prime, powerVal)) % MOD;
    }
    return lcm;
}
// Interview Explanation:
// - Problem Statement: Find number of rounds for permutation to return to original state mod 10^9+7 (CSES 3398).
// - Approach: Disjoint Cycle Decomposition + LCM calculated via maximum prime powers mod MOD.
// - Complexity: Time: O(N + sum sqrt(L_i)), Space: O(N).


// ============================================================
// 17. INCLUSION-EXCLUSION PRINCIPLE ON PRIME MULTIPLES
// ============================================================

ll countPrimeMultiples(ll n, int k, const vl& primes) {
    ll ans = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        ll prod = 1;
        bool ok = true;
        for (int i = 0; i < k; i++) {
            if (mask & (1 << i)) {
                if (prod > n / primes[i]) { ok = false; break; }
                prod *= primes[i];
            }
        }
        if (ok) {
            int bits = __builtin_popcount(mask);
            if (bits % 2 == 1) ans += n / prod;
            else ans -= n / prod;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count integers in 1..N divisible by at least one of K primes (K <= 20) (CSES 2185).
// - Approach: Principle of Inclusion-Exclusion (PIE) with overflow-safe product checks over bitmask subsets.
// - Complexity: Time: O(K * 2^K), Space: O(K).


// ============================================================
// 18. GENERAL RECURRENCE MATRIX EXPONENTIATION (THROWING DICE)
// ============================================================

ll throwingDiceSumN(ll n) {
    if (n <= 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;
    if (n == 4) return 8;
    if (n == 5) return 16;
    if (n == 6) return 32;

    vvi T = {
        {1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0}
    };
    vvi P = powerMatrices(T, n - 6);
    vl base = {32, 16, 8, 4, 2, 1};
    ll ans = 0;
    for (int j = 0; j < 6; j++) {
        ans = (ans + 1LL * P[0][j] * base[j]) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count ways to reach sum N by throwing a 6-sided die modulo 10^9+7 (CSES 1096).
// - Approach: 6 x 6 Companion Transition Matrix Exponentiation for order-6 linear recurrence.
// - Complexity: Time: O(6^3 log N), Space: O(6^2).
