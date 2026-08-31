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
 | 22 | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values    | O(N * 30)| O(N)     |
 | 23 | Multiply Two 2D Matrices                    | 3-Nested Loop Dot Products        | O(M N P) | O(M * P) |
 | 24 | N x N MEX Grid Construction                 | Bitwise XOR Matrix (i ^ j)        | O(N^2)   | O(N^2)   |
 ====================================================================================================
*/

// ============================================================
// 1. CHECK SUBSEQUENCE
// ============================================================

bool isSubsequence(string s, string t) {
    int i = 0, j = 0;
    while (i < s.size() && j < t.size()) {
        if (s[i] == t[j]) i++;
        j++;
    }
    return i == s.size();
}
// Interview Explanation:
// - Problem Statement: Check if string s is a subsequence of string t.
// - Approach: Greedy two-pointer scan.
// - Intuition: Advance pointer in s only on match; advance in t always.
// - Complexity: Time: O(len(T)), Space: O(1).


// ============================================================
// 2. PRECOMPUTE PALINDROMIC SUBARRAYS TABLE
// ============================================================

vector<vector<bool>> precomputePalindromicSubarrays(const string& s) {
    int n = s.size();
    vector<vector<bool>> isPal(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++) isPal[i][i] = true;
    for (int i = 0; i < n - 1; i++) isPal[i][i + 1] = (s[i] == s[i + 1]);
    for (int len = 3; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            isPal[i][j] = (s[i] == s[j]) && isPal[i + 1][j - 1];
        }
    }
    return isPal;
}
// Interview Explanation:
// - Problem Statement: Precompute a table where isPal[i][j] indicates whether s[i...j] is a palindrome.
// - Approach: 2D Interval DP expanding over substring lengths.
// - Intuition: s[i...j] is a palindrome iff outer characters match and interior is a palindrome.
// - Complexity: Time: O(N^2), Space: O(N^2).


// ============================================================
// 3. MINIMUM EXCLUDED VALUE (MEX)
// ============================================================

int findMEX(const vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int mex = 0;
    while (s.count(mex)) mex++;
    return mex;
}
// Interview Explanation:
// - Problem Statement: Find the smallest non-negative integer not present in the array.
// - Approach: Hash Set presence lookup.
// - Intuition: MEX is bounded in [0, N]; incrementing from 0 finds the first missing element.
// - Complexity: Time: O(N), Space: O(N).


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
// - Intuition: Repeated matrix squaring solves linear recurrences in logarithmic steps.
// - Complexity: Time: O(N^3 log p), Space: O(N^2).


// ============================================================
// 5. TOTAL AREA COVERED BY TWO 2D RECTANGLES
// ============================================================

int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
    int areaA = (ax2 - ax1) * (ay2 - ay1);
    int areaB = (bx2 - bx1) * (by2 - by1);
    int overlapW = max(0, min(ax2, bx2) - max(ax1, bx1));
    int overlapH = max(0, min(ay2, by2) - max(ay1, by1));
    int overlapArea = overlapW * overlapH;
    return areaA + areaB - overlapArea;
}
// Interview Explanation:
// - Problem Statement: Compute total 2D area covered by two rectilinear rectangles.
// - Approach: Principle of Inclusion-Exclusion.
// - Intuition: Total Area = Area(A) + Area(B) - Area(A ∩ B).
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 6. BALANCED PARENTHESES COUNT (CATALAN NUMBER)
// ============================================================

ll catalanNumber(int n) {
    vl C(n + 1, 0);
    C[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            C[i] = (C[i] + C[j] * C[i - 1 - j]) % MOD;
        }
    }
    return C[n];
}
// Interview Explanation:
// - Problem Statement: Count number of valid parentheses sequences of length 2N.
// - Approach: Catalan recurrence DP C_n = Sum(C_i * C_{n-1-i}).
// - Intuition: Any valid sequence can be uniquely split as `(A)B` where A and B are valid sub-sequences.
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
// - Approach: Fermat's Little Theorem: a^(MOD-1) ≡ 1 (mod MOD).
// - Intuition: Reduce exponent (b^c) modulo (MOD - 1) before computing power of a.
// - Complexity: Time: O(log C + log B), Space: O(1).


// ============================================================
// 8. DIVISOR ANALYSIS (CSES 1713 / NUMBER THEORY)
// ============================================================

struct DivisorAnalysis {
    static ll countDivisors(const vector<pair<ll, ll>>& factors) {
        ll count = 1;
        for (auto& [p, k] : factors) count = (count * (k + 1)) % MOD;
        return count;
    }

    static ll sumDivisors(const vector<pair<ll, ll>>& factors) {
        ll sum = 1;
        for (auto& [p, k] : factors) {
            ll num = (powerMod(p, k + 1, MOD) - 1 + MOD) % MOD;
            ll den = powerMod(p - 1, MOD - 2, MOD);
            ll term = (num * den) % MOD;
            sum = (sum * term) % MOD;
        }
        return sum;
    }
};
// Interview Explanation:
// - Problem Statement: Compute count and sum of all divisors from prime factorization.
// - Approach: Geometric series sum and multiplicative arithmetic functions.
// - Intuition: Multiplicative properties of divisor count and sum formulas.
// - Complexity: Time: O(K log MOD), Space: O(1).


// ============================================================
// 9. MAXIMUM GCD PAIR IN RANGE (CSES 1081)
// ============================================================

int maxGCDPair(const vector<int>& arr) {
    int maxVal = 0;
    for (int x : arr) maxVal = max(maxVal, x);
    vector<int> freq(maxVal + 1, 0);
    for (int x : arr) freq[x]++;

    for (int gcd = maxVal; gcd >= 1; gcd--) {
        int count = 0;
        for (int multiple = gcd; multiple <= maxVal; multiple += gcd) {
            count += freq[multiple];
            if (count >= 2) return gcd;
        }
    }
    return 1;
}
// Interview Explanation:
// - Problem Statement: Find maximum GCD among any pair of elements in array (CSES 1081).
// - Approach: Sieve-like multiple frequency counting backwards from maxVal.
// - Intuition: Harmonic series iteration: the first candidate GCD with >= 2 multiples is optimal.
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
// - Intuition: Prime gaps are small (~O(log N)); Miller-Rabin tests each candidate deterministically.
// - Complexity: Time: O(log^3 N), Space: O(1).


// ============================================================
// 11. DERANGEMENTS / CHRISTMAS PARTY (CSES 1717)
// ============================================================

ll countDerangements(int n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    ll prev2 = 0, prev1 = 1;
    for (int i = 3; i <= n; i++) {
        ll cur = ((i - 1) * (prev1 + prev2)) % MOD;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Count permutations where no element appears in its original position (CSES 1717).
// - Approach: Derangement Recurrence: D(n) = (n - 1) * (D(n - 1) + D(n - 2)).
// - Intuition: Element 1 has (n - 1) choices to swap; case analysis on whether the other element takes pos 1 yields the recurrence.
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
// - Intuition: Placing n - 1 dividers among m stars.
// - Complexity: Time: O(N + M) build, O(1) query, Space: O(N + M).


// ============================================================
// 13. CREATING STRINGS II / STARS & BARS (CSES 1716)
// ============================================================

ll creatingStringsII(string s) {
    int n = s.size();
    Combinatorics comb(n);
    vector<int> freq(26, 0);
    for (char c : s) freq[c - 'a']++;
    ll ans = comb.fact[n];
    for (int f : freq) ans = (ans * comb.invFact[f]) % MOD;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count distinct permutations of a string with repeating characters (CSES 1716).
// - Approach: Multinomial Coefficient: N! / (f_a! * f_b! * ... * f_z!).
// - Intuition: Factorial of total length divided by factorials of identical character frequencies.
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

ll shortestPathKWalks(int n, int k, Matrix& adjMatrix) {
    Matrix res = adjMatrix;
    k--;
    Matrix base = adjMatrix;
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
// - Intuition: Replace standard (+, *) matrix multiplication with (min, +) to propagate shortest path lengths.
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

vl lehmerCode(const vi& perm) {
    int n = perm.size();
    Fenwick bit(n);
    vl code(n);
    for (int i = n - 1; i >= 0; i--) {
        code[i] = bit.query(perm[i]);
        bit.add(perm[i], 1);
    }
    return code;
}
// Interview Explanation:
// - Problem Statement: Compute Lehmer code / inversion table of a permutation (CSES 3397).
// - Approach: Fenwick tree (Binary Indexed Tree) scanning backwards.
// - Intuition: Counting elements smaller than perm[i] to its right.
// - Complexity: Time: O(N log N), Space: O(N).


// ============================================================
// 16. PERMUTATION ROUNDS / ORBIT PERIOD (CSES 3398)
// ============================================================

ll permutationOrbitLCM(const vi& perm) {
    int n = perm.size();
    vector<bool> vis(n, false);
    map<int, int> primeMaxPower;

    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            int len = 0, cur = i;
            while (!vis[cur]) { vis[cur] = true; cur = perm[cur] - 1; len++; }
            int temp = len;
            for (int p = 2; p * p <= temp; p++) {
                if (temp % p == 0) {
                    int count = 0;
                    while (temp % p == 0) { count++; temp /= p; }
                    primeMaxPower[p] = max(primeMaxPower[p], count);
                }
            }
            if (temp > 1) primeMaxPower[temp] = max(primeMaxPower[temp], 1);
        }
    }
    ll ans = 1;
    for (auto& [p, power] : primeMaxPower) {
        ans = (ans * powerMod(p, power, MOD)) % MOD;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find period of permutation orbit: LCM of all cycle lengths modulo 1e9+7 (CSES 3398).
// - Approach: Disjoint cycle decomposition + prime-factor max power tracking.
// - Intuition: Modular LCM requires tracking global max prime exponent across all cycle lengths.
// - Complexity: Time: O(N log MOD), Space: O(N).


// ============================================================
// 17. PRIME MULTIPLES (CSES 2185 — PIE)
// ============================================================

ll countPrimeMultiples(ll n, int k, const vl& primes) {
    ll total = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        ll prod = 1;
        bool overflow = false;
        int bitCount = 0;
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1) {
                bitCount++;
                if (prod > n / primes[i]) { overflow = true; break; }
                prod *= primes[i];
            }
        }
        if (overflow) continue;
        if (bitCount % 2 == 1) total += n / prod;
        else total -= n / prod;
    }
    return total;
}
// Interview Explanation:
// - Problem Statement: Count numbers <= n divisible by at least one of k primes (CSES 2185).
// - Approach: Principle of Inclusion-Exclusion over 2^K bitmasks with overflow guards.
// - Intuition: Add odd-sized intersections, subtract even-sized intersections.
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
// - Intuition: Recurrence dp[n] = dp[n-1] + ... + dp[n-6] represented in 6x6 transition matrix.
// - Complexity: Time: O(6^3 log N), Space: O(1).


// ============================================================
// 19. RANGE BITWISE AND — LeetCode 201
// ============================================================

int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left < right) {
        left >>= 1;
        right >>= 1;
        shift++;
    }
    return left << shift;
}
// Interview Explanation:
// - Problem Statement: Bitwise AND of all numbers in range [left, right] (LeetCode 201).
// - Approach: Find Common Binary Prefix.
// - Intuition: Bit positions that change between left and right will evaluate to 0 in bitwise AND.
// - Complexity: Time: O(log R), Space: O(1).


// ============================================================
// 20. SUBARRAY BITWISE ORS — LeetCode 898
// ============================================================

int subarrayBitwiseORs(vector<int>& arr) {
    unordered_set<int> allOrs, curOrs;
    for (int x : arr) {
        unordered_set<int> nextOrs = {x};
        for (int y : curOrs) nextOrs.insert(x | y);
        curOrs = std::move(nextOrs);
        for (int v : curOrs) allOrs.insert(v);
    }
    return allOrs.size();
}
// Interview Explanation:
// - Problem Statement: Find number of distinct bitwise ORs of all non-empty subarrays (LeetCode 898).
// - Approach: Set Dynamic Programming on Frontier Values.
// - Intuition: Bitwise OR is monotonically non-decreasing and can change at most 30 times.
// - Complexity: Time: O(30 * N), Space: O(30 * N).


// ============================================================
// 21. COUNT TOTAL SET BITS (1 TO N)
// ============================================================

int countTotalSetBits(int n) {
    int total = 0;
    n++;
    for (int i = 0; (1 << i) <= n; i++) {
        int cycle = 1 << (i + 1);
        int fullCycles = n / cycle;
        total += fullCycles * (1 << i);
        int rem = n % cycle;
        total += max(0, rem - (1 << i));
    }
    return total;
}
// Interview Explanation:
// - Problem Statement: Count total number of set bits (1s) across all integers from 1 to n.
// - Approach: Periodic Bit Position Contribution Math.
// - Intuition: For bit position i, bits alternate in cycles of length 2^(i+1).
// - Complexity: Time: O(log N), Space: O(1).


// ============================================================
// 22. COUNT SUBARRAYS WITH BITWISE AND EQUAL TO K
// ============================================================

long long countSubarraysWithAndK(vector<int>& nums, int k) {
    long long count = 0;
    unordered_map<int, int> prevAnds;
    for (int x : nums) {
        unordered_map<int, int> currAnds;
        if ((x & k) == k) {
            currAnds[x] = 1;
            for (auto& [val, freq] : prevAnds) {
                currAnds[val & x] += freq;
            }
            for (auto& [val, freq] : currAnds) {
                if (val == k) count += freq;
            }
        }
        prevAnds = currAnds;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total number of subarrays whose bitwise AND equals k.
// - Approach: Hash Map DP on Frontier Bitwise AND Values.
// - Intuition: Frontier values are bounded by <= 30 distinct values at each index.
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
// - Intuition: `C[i][j] = Sum(A[i][p] * B[p][j])`.
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
// - Approach: Bitwise XOR matrix `grid[i][j] = i ^ j` (Nim addition table).
// - Intuition: XOR grid forms a Latin square / group table where each row/column is a permutation of [0, 2^k - 1].
// - Complexity: Time: O(N^2), Space: O(N^2).
