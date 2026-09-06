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
 | 1  | Check Subsequence                           | Greedy Two-Pointer Linear Scan    | O(len(S))| O(1)     |
 | 2  | Precompute Palindromic Subarrays Table      | 2D Interval DP over Lengths       | O(N^2)   | O(N^2)   |
 | 3  | Minimum Excluded Value (MEX)                | Hash Set Presence Lookup          | O(N)     | O(N)     |
 | 4  | Matrix Multiplication & Exponentiation      | Binary Exponentiation on Matrices | O(N^3logB)| O(N^2)   |
 | 5  | Total Area Covered by Two 2D Rectangles     | Inclusion-Exclusion Geometry      | O(1)     | O(1)     |
 | 6  | Balanced Parentheses Count (Catalan Number) | Catalan Number Formula C_n        | O(N)     | O(N)     |
 | 7  | Exponentiation II (Power Tower — CSES 1712) | Euler's Totient + Fermat MOD-1    | O(log B) | O(1)     |
 | 8  | Divisor Analysis (CSES 1713 / Number Theory)| Prime Factorization Powers Modulo | O(N logM)| O(N)     |
 | 9  | Maximum GCD Pair in Range (CSES 1081)       | Multiples Sieve Frequency Scan    | O(M logM)| O(M)     |
 | 10 | Next Prime Search (CSES 3396)               | Deterministic Miller-Rabin Test   | O(k log3N)| O(1)     |
 | 11 | Derangements / Christmas Party (CSES 1717)  | Derangement Recurrence DP         | O(N)     | O(N)     |
 | 12 | Distributing Apples / Multinomial (CSES 1715)| Stars and Bars Formula nCr        | O(N) bld | O(N)     |
 | 13 | Creating Strings II / Stars & Bars (CSES 1716)| Multinomial Permutations Formula | O(N) bld | O(N)     |
 | 14 | Graph Paths II / Length K Walks (CSES 1723) | Min-Plus Matrix Exponentiation    | O(V^3logK)| O(V^2)   |
 | 15 | Permutation Inversions / Lehmer (CSES 3397) | Inversion Table + Fenwick Tree    | O(N logN)| O(N)     |
 | 16 | Permutation Rounds / Orbit Period (CSES 3398)| Permutation Orbit LCM Modulo     | O(N logM)| O(N)     |
 | 17 | Prime Multiples (CSES 2185 — PIE)           | Inclusion-Exclusion on Bitmasks   | O(K 2^K) | O(K)     |
 | 18 | Throwing Dice (CSES 1096 — Order-6 Exponent)| Order-6 Companion Exponentiation | O(6^3logN)| O(1)    |
 | 19 | Range Bitwise AND (LeetCode 201)            | Binary Prefix Bit-Shifts          | O(log R) | O(1)     |
 | 20 | Subarray Bitwise ORs (LeetCode 898)         | Set DP / Monotonic Frontier Values| O(N * 30)| O(N * 30)|
 | 21 | Count Total Set Bits (1 to N)               | Periodic Bit Position Math        | O(log N) | O(1)     |
 | 22 | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values    | O(N * 30)| O(30)    |
 | 23 | Multiply Two 2D Matrices                    | 3-Nested Loop Dot Products        | O(M N P) | O(M * P) |
 | 24 | N x N MEX Grid Construction                 | Bitwise XOR Matrix (i ^ j)        | O(N^2)   | O(N^2)   |
 ====================================================================================================
*/

// ============================================================
// 1. CHECK SUBSEQUENCE
// ============================================================

bool isSubsequence(string s, string t) {
    int i = 0, j = 0;
    int n = s.size(), m = t.size();
    while (i < n && j < m) {
        if (s[i] == t[j]) i++;
        j++;
    }
    return i == n;
}
// Interview Explanation:
// - Problem Statement: Check if string s is a subsequence of string t.
// - Approach: Greedy two-pointer linear scan.
// - Intuition:
//   * A subsequence preserves relative order without requiring contiguity.
//   * Greedy choice property holds: matching the earliest occurrence of s[i] in t leaves maximum slack for matching subsequent characters.
//   * Advance pointer i only when s[i] matches t[j]; pointer j advances unconditionally on every step.
//   * If i reaches s.size(), every character was successfully matched in order.
// - Complexity: Time: O(len(t)), Space: O(1).


// ============================================================
// 2. PRECOMPUTE PALINDROMIC SUBARRAYS TABLE
// ============================================================

vector<vector<bool>> precomputePalindromicSubarrays(const string& s) {
    int n = s.size();
    vector<vector<bool>> dp(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++) dp[i][i] = true; // single character substrings are palindromes
    for (int i = 0; i < n - 1; i++) dp[i][i + 1] = (s[i] == s[i + 1]); // two character substrings are palindromes if both characters match
    for (int len = 3; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = (s[i] == s[j]) && dp[i + 1][j - 1];
        }
    }
    return dp; // dp[i][j] = true if s[i...j] is a palindrome
}
// Interview Explanation:
// - Problem Statement: Precompute a table where dp[i][j] indicates whether s[i...j] is a palindrome.
// - Approach: 2D Interval DP expanding over substring lengths.
// - Intuition:
//   * A string s[i...j] is a palindrome if and only if endpoint characters match (s[i] == s[j]) and the interior substring s[i+1...j-1] is also a palindrome.
//   * Base cases of length 1 are always palindromes (dp[i][i] = true); length 2 depends solely on character equality (s[i] == s[i+1]).
//   * Iterating length from 3 up to n ensures every subproblem dp[i+1][j-1] is evaluated before dp[i][j].
//   * Yields O(1) palindrome verification queries for downstream interval DP problems like palindrome partitioning.
// - Complexity: Time: O(N^2), Space: O(N^2).


// ============================================================
// 3. MINIMUM EXCLUDED VALUE (MEX)
// ============================================================

int findMEX(const vi& a) {
    unordered_set<int> st(a.begin(), a.end()); // unordered set for O(1) average lookup, if we used set, then it would be O(log N) per lookup
    int ans = 0;
    while (st.count(ans)) ans++;
    return ans;
}


// Interview Explanation:
// - Problem Statement: Find the smallest non-negative integer not present in the array.
// - Approach: Hash Set presence lookup.
// - Intuition:
//   * By Pigeonhole Principle, among N elements, the minimum excluded value must lie strictly within the range [0, N].
//   * Inserting all array elements into a hash set allows O(1) average lookup per candidate.
//   * Starting a test pointer from 0 and incrementing until the first absent integer is found guarantees finding the minimal missing value.
//   * Loop runs at most N + 1 times, bounding overall lookup operations.
// - Complexity: Time: O(N) average, Space: O(N).

vector<vector<int>> mexOfAllSubarrays(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> ans(n, vector<int>(n));
    // for each starting index l, maintain frequency of numbers in the current subarray [l, r]
    for (int l = 0; l < n; l++) {
        vector<int> freq(n + 1, 0); // for each starting index l, maintain frequency of numbers in the current subarray [l, r]
        int mex = 0; // the current minimum excluded value for the subarray starting at index l
        for (int r = l; r < n; r++) {
            if (nums[r] <= n && nums[r] >= 0) {
                freq[nums[r]]++;
            }
            // Move mex forward while it exists
            while (mex <= n && freq[mex] > 0) {
                mex++;
            }
            ans[l][r] = mex;
        }
    }
    return ans;
}

// Interview Explanation:
// - Problem Statement: Compute the MEX for all subarrays of a given array.
// - Approach: Sliding Window Frequency Count.
// - Intuition:
//   * For each starting index l, maintain a frequency array of elements in the current subarray [l, r].
//    * The MEX is the smallest index i such that freq[i] == 0.
//   * As r expands, update the frequency of nums[r] and adjust the MEX pointer accordingly.
//   * This avoids recomputing the MEX from scratch for each subarray, yielding O(N^2) time complexity instead of O(N^3).
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 4. MATRIX MULTIPLICATION & EXPONENTIATION
// ============================================================

using Matrix = vector<vector<ll>>;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
    return C;
}

Matrix matrixPower(Matrix A, ll p) {
    int n = A.size();
    Matrix res(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) res[i][i] = 1;
    while (p > 0) {
        if (p & 1) res = multiply(res, A);
        A = multiply(A, A);
        p >>= 1;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute A^p for an N x N matrix modulo MOD.
// - Approach: Binary exponentiation with O(N^3) matrix multiplication.
// - Intuition:
//   * Linear recurrences can be represented in state-transition matrix form: S_{t+1} = T * S_t.
//   * By associativity of matrix multiplication, T^p can be computed via repeated squaring in O(log p) matrix operations instead of linear multiplications.
//   * Identity matrix I serves as the multiplicative neutral element (res = I).
//   * Reordering loops (i, k, j) improves CPU cache locality during row-major matrix multiplications.
// - Complexity: Time: O(N^3 log p), Space: O(N^2).


// ============================================================
// 5. TOTAL AREA COVERED BY TWO 2D RECTANGLES
// ============================================================

int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
    int areaA = (ax2 - ax1) * (ay2 - ay1);
    int areaB = (bx2 - bx1) * (by2 - by1);
    int ow = max(0, min(ax2, bx2) - max(ax1, bx1));
    int oh = max(0, min(ay2, by2) - max(ay1, by1));
    return areaA + areaB - (ow * oh);
}
// Interview Explanation:
// - Problem Statement: Compute total 2D area covered by two rectilinear rectangles.
// - Approach: Principle of Inclusion-Exclusion (PIE).
// - Intuition:
//   * Summing individual areas overcounts any overlapping region twice: Total = Area(A) + Area(B) - Area(A ∩ B).
//   * The intersection of two axis-aligned rectangles is also an axis-aligned rectangle.
//   * Overlap width is max(0, min(ax2, bx2) - max(ax1, bx1)), and height is max(0, min(ay2, by2) - max(ay1, by1)).
//   * If rectangles do not intersect, width or height clamps to 0 via max(0, ...), correctly yielding zero overlap.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 6. BALANCED PARENTHESES COUNT (CATALAN NUMBER)
// ============================================================

ll catalanNumber(int n) {
    vl c(n + 1, 0);
    c[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            c[i] = (c[i] + c[j] * c[i - 1 - j]) % MOD;
        }
    }
    return c[n];
}
// Interview Explanation:
// - Problem Statement: Count number of valid parentheses sequences of length 2N.
// - Approach: Catalan recurrence DP: C_n = Sum(C_j * C_{n - 1 - j}) for j = 0 to n - 1.
// - Intuition:
//   * Any valid parentheses sequence can be uniquely decomposed as `(A)B` where the first `(` matches a specific `)` closing bracket.
//   * In this canonical decomposition, interior sequence `A` contains j pairs and exterior tail `B` contains n - 1 - j pairs.
//   * Because every valid string matches a unique first pair, partitions across all values of j are mutually exclusive and exhaustive.
//   * Dynamic programming sums products of independent subproblem solutions.
// - Complexity: Time: O(N^2), Space: O(N).


// ============================================================
// 7. EXPONENTIATION II (POWER TOWER — CSES 1712)
// ============================================================

ll powerMod(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

ll solvePowerTower(ll a, ll b, ll c) {
    ll exp = powerMod(b, c, MOD - 1);
    return powerMod(a, exp, MOD);
}
// Interview Explanation:
// - Problem Statement: Compute a^(b^c) % (1e9 + 7) (CSES 1712).
// - Approach: Fermat's Little Theorem / Euler's Totient Theorem.
// - Intuition:
//   * Since MOD = 1e9 + 7 is prime, Fermat's Little Theorem asserts a^(MOD - 1) ≡ 1 (mod MOD) for gcd(a, MOD) = 1.
//   * Exponents in the power tower can therefore be evaluated modulo (MOD - 1), reducing b^c modulo (MOD - 1).
//   * Two-stage exponentiation: first evaluate exp = (b^c) % (MOD - 1), then compute ans = (a^exp) % MOD.
//   * Handles special exponent cases (e.g., 0^0 = 1) consistently via binary exponentiation.
// - Complexity: Time: O(log C + log B), Space: O(1).


// ============================================================
// 8. DIVISOR ANALYSIS (CSES 1713 / NUMBER THEORY)
// ============================================================

struct DivisorAnalysis {
    static ll countDivisors(const vector<pair<ll, ll>>& f) {
        ll ans = 1;
        for (auto& [p, k] : f) ans = (ans * (k + 1)) % MOD;
        return ans;
    }

    static ll sumDivisors(const vector<pair<ll, ll>>& f) {
        ll ans = 1;
        for (auto& [p, k] : f) {
            ll num = (powerMod(p, k + 1, MOD) - 1 + MOD) % MOD;
            ll den = powerMod(p - 1, MOD - 2, MOD);
            ll term = (num * den) % MOD;
            ans = (ans * term) % MOD;
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Compute count and sum of all divisors from prime factorization p_i^k_i.
// - Approach: Geometric series sum and multiplicative arithmetic functions.
// - Intuition:
//   * Any divisor is formed by choosing prime powers p_i^c_i where 0 <= c_i <= k_i (giving k_i + 1 choices per prime).
//   * Divisor count d(n) is the product of (k_i + 1) across all distinct prime factors.
//   * Divisor sum sigma(n) expands as the product of geometric series: (1 + p_i + ... + p_i^k_i) = (p_i^{k_i+1} - 1) / (p_i - 1).
//   * Modular division by (p_i - 1) is computed via modular inverse using Fermat's Little Theorem.
// - Complexity: Time: O(K log MOD), Space: O(1).


// ============================================================
// 9. MAXIMUM GCD PAIR IN RANGE (CSES 1081)
// ============================================================

int maxGCDPair(const vi& a) {
    int mx = 0;
    for (int x : a) mx = max(mx, x);
    vi freq(mx + 1, 0);
    for (int x : a) freq[x]++;

    for (int g = mx; g >= 1; g--) {
        int cnt = 0;
        for (int m = g; m <= mx; m += g) {
            cnt += freq[m];
            if (cnt >= 2) return g;
        }
    }
    return 1;
}
// Interview Explanation:
// - Problem Statement: Find maximum GCD among any pair of elements in array (CSES 1081).
// - Approach: Sieve-like multiple frequency counting backwards from maxVal.
// - Intuition:
//   * Brute-force pair testing takes O(N^2 log M), which times out for N = 2e5.
//   * Instead of iterating pairs, iterate possible GCD candidate values g from maxVal down to 1.
//   * Candidate g is valid if the array contains at least 2 multiples of g.
//   * Sum of harmonic series M/1 + M/2 + ... + M/M = O(M log M). The first candidate found from top down is guaranteed to be maximal.
// - Complexity: Time: O(M log M + N), Space: O(M).


// ============================================================
// 10. NEXT PRIME SEARCH (CSES 3396)
// ============================================================

bool millerRabinPrime(ll n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    ll d = n - 1; int s = 0;
    while (d % 2 == 0) { d /= 2; s++; }
    static const vector<ll> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (ll a : bases) {
        if (n <= a) break;
        ll x = powerMod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = ((__int128)x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

ll nextPrime(ll n) {
    ll cur = n;
    while (!millerRabinPrime(cur)) cur++;
    return cur;
}
// Interview Explanation:
// - Problem Statement: Find the smallest prime >= n (CSES 3396).
// - Approach: Deterministic Miller-Rabin Primality Test for 64-bit integers.
// - Intuition:
//   * By prime gap theorems, distance between consecutive primes near n is roughly O(log n), typically under a few hundred steps.
//   * Factor n - 1 as d * 2^s where d is odd.
//   * Miller-Rabin tests whether a^d ≡ 1 (mod n) or a^(d * 2^r) ≡ -1 (mod n).
//   * Testing against the first 12 primes provides 100% deterministic correctness for all 64-bit integers up to 2^64.
// - Complexity: Time: O(gap * log^3 N), Space: O(1).


// ============================================================
// 11. DERANGEMENTS / CHRISTMAS PARTY (CSES 1717)
// ============================================================

ll countDerangements(int n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    ll p2 = 0, p1 = 1;
    for (int i = 3; i <= n; i++) {
        ll cur = ((i - 1) * (p1 + p2)) % MOD;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
// Interview Explanation:
// - Problem Statement: Count permutations where no element appears in its original position (CSES 1717).
// - Approach: Derangement Recurrence: D(n) = (n - 1) * (D(n - 1) + D(n - 2)).
// - Intuition:
//   * Person 1 can receive gifts from any of the (n - 1) other people, say person k.
//   * Case 1: Person k also receives Person 1's gift. Both are satisfied, leaving D(n - 2) subproblems for remaining n - 2 people.
//   * Case 2: Person k does not receive Person 1's gift. Person 1 is temporarily mapped to Person k's prohibited slot, yielding D(n - 1) subproblems.
//   * State depends only on preceding two values, allowing O(1) rolling variables.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 12. DISTRIBUTING APPLES / MULTINOMIAL (CSES 1715)
// ============================================================

struct Combinatorics {
    vl fact, invFact;
    Combinatorics(int n) : fact(n + 1), invFact(n + 1) {
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = (fact[i - 1] * i) % MOD;
        invFact[n] = powerMod(fact[n], MOD - 2, MOD);
        for (int i = n - 1; i >= 0; i--) invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
    }
    ll nCr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
    }
};

ll distributeApples(int n, int m) {
    Combinatorics comb(n + m);
    return comb.nCr(n + m - 1, n - 1);
}
// Interview Explanation:
// - Problem Statement: Distribute m identical apples to n children (CSES 1715).
// - Approach: Stars and Bars theorem: C(m + n - 1, n - 1).
// - Intuition:
//   * Distributing m identical items among n distinct buckets is equivalent to arranging m stars and n - 1 dividers.
//   * Total positions in the sequence equals m + (n - 1).
//   * Any choice of n - 1 divider positions uniquely determines the allocation for each child.
//   * Precomputing factorials and modular inverse factorials allows O(1) combinations query.
// - Complexity: Time: O(N + M) build, O(1) query, Space: O(N + M).


// ============================================================
// 13. CREATING STRINGS II / STARS & BARS (CSES 1716)
// ============================================================

ll creatingStringsII(string s) {
    int n = s.size();
    Combinatorics comb(n);
    vi freq(26, 0);
    for (char c : s) freq[c - 'a']++;
    ll ans = comb.fact[n];
    for (int f : freq) ans = (ans * comb.invFact[f]) % MOD;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count distinct permutations of a string with repeating characters (CSES 1716).
// - Approach: Multinomial Coefficient: N! / (f_1! * f_2! * ... * f_k!).
// - Intuition:
//   * If all characters were distinct, there would be N! total permutations.
//   * Identical characters can be permuted among themselves in f_c! ways without creating a new distinct string.
//   * By the multiplication principle, dividing N! by the product of f_c! for all distinct characters cancels overcounted permutations.
//   * Division modulo MOD is implemented using precomputed inverse factorials.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 14. GRAPH PATHS II / LENGTH K WALKS (CSES 1723)
// ============================================================

Matrix minPlusMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 2e18));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                if (A[i][k] < 2e18 && B[k][j] < 2e18)
                    C[i][j] = min(C[i][j], A[i][k] + B[k][j]);
    return C;
}

ll shortestPathKWalks(int n, int k, Matrix& adj) {
    Matrix res = adj;
    k--;
    Matrix base = adj;
    while (k > 0) {
        if (k & 1) res = minPlusMultiply(res, base);
        base = minPlusMultiply(base, base);
        k >>= 1;
    }
    return res[0][n - 1] >= 2e18 ? -1 : res[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find shortest path of EXACTLY K edges between node 1 and N (CSES 1723).
// - Approach: (Min, +) Tropical Semiring Matrix Exponentiation.
// - Intuition:
//   * Standard matrix multiplication computes path counts via (Sum, Prod).
//   * Replacing addition with min and multiplication with + forms the tropical semiring, where C[i][j] = min_k(A[i][k] + B[k][j]).
//   * A^k under this algebra computes the shortest walk of exactly k edges between all pairs.
//   * Binary exponentiation accelerates computing A^k to logarithmic matrix operations.
// - Complexity: Time: O(V^3 log K), Space: O(V^2).


// ============================================================
// 15. PERMUTATION INVERSIONS / LEHMER (CSES 3397)
// ============================================================

struct Fenwick {
    int n; vi tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) { for (; i <= n; i += i & -i) tree[i] += delta; }
    int query(int i) { int sum = 0; for (; i > 0; i -= i & -i) sum += tree[i]; return sum; }
};

vl lehmerCode(const vi& a) {
    int n = a.size();
    Fenwick bit(n);
    vl code(n);
    for (int i = n - 1; i >= 0; i--) {
        code[i] = bit.query(a[i]);
        bit.add(a[i], 1);
    }
    return code;
} // code[i] = number of elements to the right of index i that are strictly smaller than a[i], we can solve this using merge sort or fenwick tree, here we use fenwick tree

// Interview Explanation:
// - Problem Statement: Compute Lehmer code / inversion table of a permutation (CSES 3397).
// - Approach: Fenwick tree (Binary Indexed Tree) scanning backwards.
// - Intuition:
//   * The i-th term of the Lehmer code counts how many elements to the right of index i are strictly smaller than a[i].
//   * Iterating from right to left (i = n - 1 down to 0) ensures that only elements to the right of index i have been processed.
//   * Querying the Fenwick tree up to a[i] returns the count of already visited smaller values in O(log N).
//   * Adding a[i] into the Fenwick tree registers it for subsequent elements to the left.
// - Complexity: Time: O(N log N), Space: O(N) (fenwick tree)

// ============================================================
// 16. PERMUTATION ROUNDS / ORBIT PERIOD (CSES 3398)
// ============================================================

ll permutationOrbitLCM(const vi& a) {
    int n = a.size();
    vector<bool> vis(n, false);
    map<int, int> mxP;

    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            int len = 0, cur = i;
            while (!vis[cur]) { vis[cur] = true; cur = a[cur] - 1; len++; }
            int tmp = len;
            for (int p = 2; p * p <= tmp; p++) {
                if (tmp % p == 0) {
                    int cnt = 0;
                    while (tmp % p == 0) { cnt++; tmp /= p; }
                    mxP[p] = max(mxP[p], cnt);
                }
            }
            if (tmp > 1) mxP[tmp] = max(mxP[tmp], 1);
        }
    }
    ll ans = 1;
    for (auto& [p, pw] : mxP) {
        ans = (ans * powerMod(p, pw, MOD)) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find period of permutation orbit: LCM of all cycle lengths modulo 1e9+7 (CSES 3398).
// - Approach: Disjoint cycle decomposition + prime-factor max power tracking.
// - Intuition:
//   * Any permutation decomposes into a set of disjoint cyclic orbits. Elements in a cycle of length L return to original position every L steps.
//   * The global period is the Least Common Multiple (LCM) of all individual cycle lengths.
//   * LCM modulo MOD cannot be calculated by pairwise division because division is not straightforward in modular arithmetic.
//   * Instead, find the maximum power p^k of each prime dividing any cycle length, and multiply p^{max_k} modulo MOD.
// - Complexity: Time: O(N log MOD), Space: O(N).


// ============================================================
// 17. PRIME MULTIPLES (CSES 2185 — PIE)
// ============================================================

ll countPrimeMultiples(ll n, int k, const vl& a) {
    ll ans = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        ll prod = 1;
        bool overflow = false;
        int bits = 0;
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1) {
                bits++;
                if (prod > n / a[i]) { overflow = true; break; }
                prod *= a[i];
            }
        }
        if (overflow) continue;
        if (bits % 2 == 1) ans += n / prod;
        else ans -= n / prod;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count numbers <= n divisible by at least one of k primes (CSES 2185).
// - Approach: Principle of Inclusion-Exclusion over 2^K bitmasks with overflow guards.
// - Intuition:
//   * The union of sets of multiples is computed via PIE: add sizes of odd-sized subsets, subtract sizes of even-sized subsets.
//   * For a chosen subset of primes, the number of multiples <= n is floor(n / prod(primes)).
//   * The product of primes can easily exceed 64-bit integer limits (e.g., 20 primes > 10^18).
//   * An overflow check `prod > n / a[i]` detects if the product exceeds n, in which case floor(n / prod) is 0 and can be skipped safely.
// - Complexity: Time: O(K * 2^K), Space: O(K).


// ============================================================
// 18. THROWING DICE (CSES 1096 — ORDER-6 EXPONENTIATION)
// ============================================================

ll throwingDiceWays(ll n) {
    if (n < 0) return 0;
    Matrix T(6, vector<ll>(6, 0));
    for (int j = 0; j < 6; j++) T[0][j] = 1;
    for (int i = 1; i < 6; i++) T[i][i - 1] = 1;

    Matrix Tn = matrixPower(T, n);
    return Tn[0][0];
}
// Interview Explanation:
// - Problem Statement: Count ways to get sum n by throwing dice with outcomes 1 to 6 (CSES 1096).
// - Approach: Order-6 Companion Matrix Exponentiation.
// - Intuition:
//   * Recurrence is linear: dp[n] = dp[n-1] + dp[n-2] + dp[n-3] + dp[n-4] + dp[n-5] + dp[n-6].
//   * State vector [dp[n], dp[n-1], ..., dp[n-5]]^T transitions via a 6x6 companion matrix T.
//   * First row of T is all 1s (summing past 6 values); subdiagonal has 1s to shift states forward.
//   * With n up to 10^18, O(6^3 log n) matrix exponentiation solves the problem in milliseconds.
// - Complexity: Time: O(6^3 log N), Space: O(1).


// ============================================================
// 19. RANGE BITWISE AND — LeetCode 201
// ============================================================

int rangeBitwiseAnd(int l, int r) {
    int shift = 0;
    while (l < r) {
        l >>= 1;
        r >>= 1;
        shift++;
    }
    return l << shift;
}
// Interview Explanation:
// - Problem Statement: Bitwise AND of all numbers in range [l, r] (LeetCode 201).
// - Approach: Find Common Binary Prefix.
// - Intuition:
//   * Bitwise AND produces 1 at a bit position if and only if every number in the range [l, r] has a 1 at that position.
//   * If l != r, at least one odd and one even number exist in the range, turning the lowest bit to 0.
//   * Any bit position that flips between l and r will encounter both 0 and 1, forcing its AND result to 0.
//   * The result is simply the common binary prefix of l and r, with all trailing differing bits cleared to 0.
// - Complexity: Time: O(log R), Space: O(1).


// ============================================================
// 20. SUBARRAY BITWISE ORS — LeetCode 898
// ============================================================

int subarrayBitwiseORs(vi& a) {
    unordered_set<int> allOrs, curOrs;
    // allOrs stores all distinct OR results across all subarrays
    // curOrs stores the distinct OR results of subarrays ending at the current index
    for (int x : a) {
        unordered_set<int> nxtOrs = {x};
        for (int y : curOrs) nxtOrs.insert(x | y);
        curOrs = std::move(nxtOrs); // std::move avoids unnecessary copy, transferring ownership of the set to curOrs, which is efficient for large sets
        for (int v : curOrs) allOrs.insert(v);
    }
    return allOrs.size();
}
// Interview Explanation:
// - Problem Statement: Find number of distinct bitwise ORs of all non-empty subarrays (LeetCode 898).
// - Approach: Set Dynamic Programming on Frontier Values.
// - Intuition:
//   * As a subarray extends leftward from current element x, bitwise OR can only add 1-bits (monotonically non-decreasing).
//   * An integer has at most 30 bits, so a running OR ending at index i can take at most 30 distinct values.
//   * Maintaining a small set of frontier OR values ending at the current position keeps each step bounded by O(30).
//   * Moving results into a global set collects all unique values across all possible subarrays.
// - Complexity: Time: O(30 * N), Space: O(30 * N).


// ============================================================
// 21. COUNT TOTAL SET BITS (1 TO N)
// ============================================================

int countTotalSetBits(int n) {
    int ans = 0;
    n++;
    for (int i = 0; (1 << i) <= n; i++) {
        int cyc = 1 << (i + 1);
        int full = n / cyc;
        ans += full * (1 << i);
        int rem = n % cyc;
        ans += max(0, rem - (1 << i));
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count total number of set bits (1s) across all integers from 1 to n.
// - Approach: Periodic Bit Position Contribution Math.
// - Intuition:
//   * For each bit position i (value 2^i), bits follow a strict periodic cycle of length 2^{i+1}: 2^i zeros followed by 2^i ones.
//   * In n + 1 numbers (0 to n), there are (n + 1) / 2^{i+1} full cycles, each contributing 2^i set bits.
//   * The remaining partial cycle has length (n + 1) % 2^{i+1}, contributing max(0, rem - 2^i) additional set bits.
//   * Summing across all bit positions 0 <= i <= log2(n) yields exact count in logarithmic time.
// - Complexity: Time: O(log N), Space: O(1).


// ============================================================
// 22. COUNT SUBARRAYS WITH BITWISE AND EQUAL TO K
// ============================================================

long long countSubarraysWithAndK(vi& a, int k) {
    long long ans = 0;
    unordered_map<int, int> prv;
    for (int x : a) {
        unordered_map<int, int> cur;
        if ((x & k) == k) { // if x has all bits of k set, it can contribute to subarrays with AND equal to k
            cur[x] = 1;
            for (auto& [val, f] : prv) {
                cur[val & x] += f;
            }
            for (auto& [val, f] : cur) {
                if (val == k) ans += f;
            }
        }
        prv = cur;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count total number of subarrays whose bitwise AND equals k.
// - Approach: Hash Map DP on Frontier Bitwise AND Values.
// - Intuition:
//   * As a subarray extends, bitwise AND can only turn 1-bits into 0-bits (monotonically non-increasing).
//   * Therefore, the number of distinct bitwise AND values for subarrays ending at any index i is at most 30.
//   * If x does not have all bits of k set ((x & k) != k), no subarray including x can possibly have AND equal to k, resetting the frontier.
//   * Transition computes (prev_and & x) with frequencies, updating the count whenever the result equals k.
// - Complexity: Time: O(N * 30), Space: O(30).


// ============================================================
// 23. MULTIPLY TWO 2D MATRICES
// ============================================================

vector<vector<int>> multiplyMatrices(vector<vector<int>>& A, vector<vector<int>>& B) {
    int m = A.size(), k = A[0].size(), n = B[0].size();
    vector<vector<int>> C(m, vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        for (int p = 0; p < k; p++) {
            if (A[i][p] == 0) continue;
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][p] * B[p][j];
            }
        }
    }
    return C;
}
// Interview Explanation:
// - Problem Statement: Multiply matrix A (m x k) by matrix B (k x n).
// - Approach: Cache-friendly row-major 3-nested loop with zero skipping.
// - Intuition:
//   * Matrix entry C[i][j] equals dot product of row i of A with column j of B: Sum_{p} A[i][p] * B[p][j].
//   * Reordering loop nesting order from (i, j, p) to (i, p, j) ensures inner loop accesses B[p][j] and C[i][j] contiguously in memory.
//   * Checking if A[i][p] == 0 skips an entire inner row scan, speeding up sparse matrix operations significantly.
//   * Eliminates cache misses and utilizes spatial locality.
// - Complexity: Time: O(M * K * N), Space: O(M * N).


// ============================================================
// 24. N X N MEX GRID CONSTRUCTION
// ============================================================

vector<vector<int>> constructMEXGrid(int n) {
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = (i ^ j);
        }
    }
    return grid;
}
// Interview Explanation:
// - Problem Statement: Construct an n x n grid where every row and column has distinct values with minimum MEX properties.
// - Approach: Bitwise XOR matrix grid[i][j] = i ^ j (Nim addition table).
// - Intuition:
//   * In group theory, bitwise XOR is the group operation of (Z_2)^k, making the XOR table an abelian Cayley table / Latin square.
//   * For any fixed row i, as j varies over 0 to 2^k - 1, the value i ^ j is an exact permutation of 0 to 2^k - 1.
//   * No two elements in the same row or column can be identical because (i ^ j = i ^ k) implies j = k.
//   * This directly minimizes the MEX of every prefix subarray and guarantees optimal distribution of values.
// - Complexity: Time: O(N^2), Space: O(N^2).
