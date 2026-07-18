#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cstring>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

/*
Usage:
  DigitDP dp;
  long long count = dp.solve(n); // Computes for range [0, n]
*/
class DigitDP {
    string s;
    long long memo[20][2][2][100]; // [pos][started][tight][custom_state]

    long long dp(int pos, bool started, bool tight, int state) {
        if (pos == s.size()) return state; // Evaluate state here
        if (memo[pos][started][tight][state] != -1) return memo[pos][started][tight][state];

        int limit = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) ans += dp(pos + 1, false, tight && (0 == limit), state);

        for (int d = (started ? 0 : 1); d <= limit; d++) 
            ans += dp(pos + 1, true, tight && (d == limit), state + d); // Custom logic

        return memo[pos][started][tight][state] = ans;
    }

public:
    long long solve(long long n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

// Binary Exponentiation
long long power(long long a, long long b, int MOD) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

// a^b^c = power(a, power(b, c, MOD - 1), MOD) (Fermat's Little Theorem)
// Modular Inverse (MOD must be prime)
long long inv(long long x, int MOD) {
    return power(x, MOD - 2, MOD);
}

// Factorials + Inverse Factorials
vector<long long> fac, ifac;

void init_nCr(int n, int MOD) {
    fac.resize(n + 1);
    ifac.resize(n + 1);

    fac[0] = 1;
    for (int i = 1; i <= n; i++)
        fac[i] = fac[i - 1] * i % MOD;

    ifac[n] = power(fac[n], MOD - 2, MOD);
    for (int i = n; i > 0; i--)
        ifac[i - 1] = ifac[i] * i % MOD;
}

long long nCr(int n, int r, int MOD) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[r] % MOD * ifac[n - r] % MOD;
}

long long nPr(int n, int r, int MOD) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[n - r] % MOD;
}

// Sieve (minimum prime factor)
vector<int> min_pf;

void sieve(int n) {
    min_pf.resize(n + 1);
    iota(min_pf.begin(), min_pf.end(), 0);

    for (int i = 2; i * i <= n; i++)
        if (min_pf[i] == i)
            for (int j = i * i; j <= n; j += i)
                if (min_pf[j] == j)
                    min_pf[j] = i;
}

vector<pair<int,int>> prime_factorize(int n) {
    vector<pair<int,int>> pf;

    for (int p = 2; p * p <= n; p++) {
        if (n % p) continue;

        int cnt = 0;
        while (n % p == 0) {
            n /= p;
            cnt++;
        }

        pf.push_back({p, cnt});
    }

    if (n > 1) pf.push_back({n, 1});
    return pf;
}

/*
Bit Operations
1 << i              // 2^i
x & (1 << i)        // check if i-th bit is set
x |= (1 << i)       // set i-th bit
x &= ~(1 << i)      // clear i-th bit
x ^= (1 << i)       // toggle i-th bit
x & (x - 1)         // removes the lowest set bit
x & -x              // value of the lowest set bit
(x & (x - 1)) == 0  // check if x is a power of 2 (x > 0)
__builtin_popcount(x)      // number of set bits (int)
__builtin_popcountll(x)    // number of set bits (long long)
__builtin_clz(x)           // leading zeros (int)
__builtin_clzll(x)         // leading zeros (long long)
__builtin_ctz(x)           // trailing zeros (int)
__builtin_ctzll(x)         // trailing zeros (long long)
__builtin_parity(x)        // 1 if odd number of set bits, else 0
msb = 31 - __builtin_clz(x)        // index of most significant set bit
msb = 63 - __builtin_clzll(x)      // (long long)
lsb = __builtin_ctz(x)             // index of least significant set bit
lsb = __builtin_ctzll(x)           // (long long)
*/
int XORupto(int n) {
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}

// ------------------------------------------------------------
// Sliding Window Maximum
// Deque: decreasing
// Front = maximum of current window
// Time: O(N)
// ------------------------------------------------------------
vi maxSlidingWindow(const vi& nums, int k) {
    deque<int> dq;
    vi ans;
    int n = nums.size();
    for (int R = 0; R < n; R++) {
        int L = R - k + 1;

        while (!dq.empty() && dq.front() < L) dq.pop_front();

        while (!dq.empty() && nums[dq.back()] <= nums[R]) dq.pop_back();

        dq.push_back(R);

        if (L >= 0) ans.push_back(nums[dq.front()]);
    }

    return ans;
}

// ------------------------------------------------------------
// Sliding Window Minimum
// Deque: increasing
// Front = minimum of current window
// Time: O(N)
// ------------------------------------------------------------
vi minSlidingWindow(const vi& nums, int k) {
    deque<int> dq;
    vi ans;
    int n = nums.size();

    for (int R = 0; R < n; R++) {
        int L = R - k + 1;

        while (!dq.empty() && dq.front() < L) dq.pop_front();

        while (!dq.empty() && nums[dq.back()] >= nums[R]) dq.pop_back();

        dq.push_back(R);

        if (L >= 0) ans.push_back(nums[dq.front()]);
    }

    return ans;
}

int lis_length(const vi& a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x);
        else *it = x;
    }
    return dp.size();
}

vector<int> reconstruct_lis(const vi& a) {
    int n = a.size();
    vi dp, idx(n), prev(n, -1);

    for (int i = 0; i < n; i++) {
        auto it = lower_bound(dp.begin(), dp.end(), a[i]);
        int pos = it - dp.begin();

        if (it == dp.end()) dp.push_back(a[i]);
        else *it = a[i];

        idx[pos] = i;
        if (pos > 0) prev[i] = idx[pos - 1];
    }
    int lis_length = dp.size();
    vector<int> lis;
    while(idx[lis_length - 1] != -1) {
        lis.push_back(a[idx[lis_length - 1]]);
        lis_length--;
    }
    reverse(lis.begin(), lis.end());
    return lis;
}

string lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    // Reconstruct the LCS
    string lcs_str;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            lcs_str += a[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) { 
            i--;
        } else {
            j--;
        }
    }
    reverse(lcs_str.begin(), lcs_str.end());
    return lcs_str;
}

/*
=========================================================
INTERVAL DP TEMPLATE (Bottom-Up)
=========================================================
State:
    dp[l][r] = answer for interval [l, r]
Transition:
    dp[l][r] = combine(dp[l][k], dp[k+1][r], l, k, r)
Order:
    Increasing interval length.
Time:
    O(n^3)
=========================================================
Common Variants
1. Matrix Chain Multiplication
--------------------------------
State:
    dp[l][r] = minimum cost to multiply matrices l...r
Base:
    dp[i][i] = 0
Transition:
    dp[l][r] = min(
        dp[l][k] +
        dp[k+1][r] +
        arr[l] * arr[k+1] * arr[r+1]
    )
---------------------------------------------------------
2. Merge Stones / Slimes
--------------------------------
State:
    dp[l][r] = minimum cost to merge interval
Extra:
    prefix sums required
Transition:
    dp[l][r] = min(
        dp[l][k] +
        dp[k+1][r]
    ) + sum(l,r)
---------------------------------------------------------
3. Optimal BST
--------------------------------
Transition:
    dp[l][r] = min(
        dp[l][k-1] +
        dp[k+1][r]
    ) + freqSum(l,r)
---------------------------------------------------------
4. Polygon Triangulation
--------------------------------
State:
    dp[l][r] = minimum triangulation cost
Transition:
    for k = l+1 ... r-1
    dp[l][r] = min(
        dp[l][k] +
        dp[k][r] +
        cost(l,k,r)
    )
---------------------------------------------------------
5. Burst Balloons
--------------------------------
State:
    dp[l][r] = maximum coins from balloons l...r
Pad array with 1 at both ends.
Transition:
    Choose LAST balloon k.
    dp[l][r] = max(
        dp[l][k-1] +
        dp[k+1][r] +
        a[l-1] * a[k] * a[r+1]
    )
---------------------------------------------------------
6. Rod Cutting / Stick Cutting
--------------------------------
State:
    dp[l][r] = minimum cost to cut segment
Transition:
    dp[l][r] = min(
        dp[l][k] +
        dp[k][r] +
        cuts[r] - cuts[l]
    )
=========================================================
*/
long long solveIntervalDPBottomUp(int n) {
    vector<vector<long long>> dp(n, vector<long long>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            dp[l][r] = LLONG_MAX;

            for (int k = l; k < r; k++) {

                long long cur =
                    dp[l][k] +
                    dp[k + 1][r]
                    /* + problem-specific cost(l,k,r) */;

                dp[l][r] = min(dp[l][r], cur);
            }
        }
    }

    return dp[0][n - 1];
}

vector<vector<int>> generateSubsets(vector<int>& nums) {
    vector<vector<int>> subsets;
    int n = nums.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) subset.push_back(nums[i]);
        }
        subsets.push_back(subset);
    }
    return subsets;
}

int sumOfAllSubsets(vector<int>& nums) {
    int n = nums.size();
    int total_sum = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int subset_sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) subset_sum += nums[i];
        }
        total_sum += subset_sum;
    }
    return total_sum;
}

vector<int> kadaneWithLandR(const vector<int>& nums) {
    int n = nums.size();
    int max_sum = INT_MIN, current_sum = 0;
    int start = 0, end = 0, temp_start = 0;

    for (int i = 0; i < n; i++) {
        current_sum += nums[i];

        if (current_sum > max_sum) {
            max_sum = current_sum;
            start = temp_start;
            end = i;
        }

        if (current_sum < 0) {
            current_sum = 0;
            temp_start = i + 1;
        }
    }

    return {max_sum, start, end};
}

void solve_(vector<int>& nums, vector<int>& curr, vector<vector<int>>& ans, int mask) {
    if (curr.size() == nums.size()) {
        ans.push_back(curr);
        return;
    }

    for (int i = 0; i < nums.size(); i++) {
        if (!(mask & (1 << i))) {
            curr.push_back(nums[i]);
            solve_(nums, curr, ans, mask | (1 << i));
            curr.pop_back();
        }
    }
}

vector<vector<int>> generatePermutations(vector<int>& nums) {
    vector<vector<int>> ans;
    vector<int> curr;

    solve_(nums, curr, ans, 0);

    return ans;
}

int lps(string s) {
    int n = s.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) dp[i][i] = 1;

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            if (s[l] == s[r]) {
                dp[l][r] = 2 + (len > 2 ? dp[l + 1][r - 1] : 0);
            } else {
                dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
            }
        }
    }

    return dp[0][n - 1];
}

bool checkSubsequence(const string& s, const string& t) {
    int n = s.size(), m = t.size();
    int j = 0;
    for (int i = 0; i < n && j < m; i++) {
        if (s[i] == t[j]) j++;
    }
    return j == m;
}

vector<vector<int>> palindromeTable(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> isPalindrome(n, vector<int>(n, 0));
    for(int i=0; i<n; i++) isPalindrome[i][i] = 1;
    for(int len=2; len<=n; len++) {
        for(int l=0; l+len-1<n; l++) {
            int r = l + len - 1;
            if(nums[l] == nums[r]) {
                isPalindrome[l][r] = (len == 2) ? 1 : isPalindrome[l+1][r-1];
            } else {
                isPalindrome[l][r] = 0;
            }
        }
    }
    return isPalindrome;
}

vector<int> buildCycle(int start, const vector<int>& parent) {
    vector<int> cycle;
    int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];

        if (cur == start)
            break;
    }

    return cycle;
}


string boothAlgorithm(string s){
    string t = s + s;
    int n = s.size();

    int i = 0, j = 1, k = 0;
    // i is the starting index of the current candidate for the smallest rotation
    // j is the starting index of the next candidate for the smallest rotation
    // k is the offset from i and j for comparison, basically, k is the length of the current matching prefix between the two candidates.
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) {
            k++;
        } // If the characters at the current offset k are equal, we increment k to check the next character in the next iteration.
        else if (t[i + k] < t[j + k]) {
            j += k + 1; // Move j to the next candidate
            if (i == j) j++; // Ensure i and j are not the same
            k = 0; // Reset k for the new comparison
        } else {
            i += k + 1; // Move i to the next candidate
            if (i == j) i++; // Ensure i and j are not the same
            k = 0; // Reset k for the new comparison
        }
    }

    int start = min(i, j);
    return t.substr(start, n);
} // it returns the lexicographically smallest rotation of the string s using Booth's algorithm.

int mex(const vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int m = 0;
    while (s.count(m)) m++;
    return m;
} // time complexity O(n) and space complexity O(n)
// mex of k numbers is always between 0 and k, inclusive.



int merge(vector<int>& nums, int left, int mid, int right) {
    int inversions = 0;
    vector<int> temp(right - left + 1);

    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
            inversions += (mid - i + 1);   // Count inversions
        }
    }

    while (i <= mid) temp[k++] = nums[i++];

    while (j <= right) temp[k++] = nums[j++];

    for (int p = 0; p < k; p++) nums[left + p] = temp[p];

    return inversions;
}

int mergeSort(vector<int>& nums, int left, int right) {
    if (left >= right)
        return 0;

    int mid = left + (right - left) / 2;

    int inversions = 0;

    inversions += mergeSort(nums, left, mid);
    inversions += mergeSort(nums, mid + 1, right);
    inversions += merge(nums, left, mid, right);

    return inversions;
}

int countInversions(vector<int>& nums) {
    return mergeSort(nums, 0, nums.size() - 1);
}


struct SOS {
    static constexpr int B = 20, N = 1 << B;
    vector<int> f, sub, sup;

    SOS() : f(N), sub(N), sup(N) {}
    // subsets mean all masks y such that (y | x) == x
    // supersets mean all masks y such that (y & x) == x
    void add(int x) { f[x]++; }

    // Build SOS DP
    void build() {
        sub = sup = f;
        for (int b = 0; b < B; b++)
            for (int m = 0; m < N; m++)
                if (m >> b & 1) sub[m] += sub[m ^ (1 << b)];
                else            sup[m] += sup[m | (1 << b)];
    }

    // (y | x) == x
    int subsets(int x) { return sub[x]; }

    // (y & x) == x
    int supersets(int x) { return sup[x]; }

    // (y & x) != 0
    int intersect(int x, int n) { return n - sub[(N - 1) ^ x]; } // because (y & x) == 0 <=> (y | ~x) == ~x <=> y is a subset of ~x, so we can use the subset count of ~x to find the count of y such that (y & x) == 0, and then subtract from total n to get the count of y such that (y & x) != 0, to find ~x , we are using (N - 1) ^ x because N - 1 is a bitmask with all bits set to 1, and XORing it with x will flip the bits of x, giving us the complement of x.
};

/*
SOS DP (O(B * 2^B), Mem: O(2^B))

Use:
- Masks <= 20~22 bits
- Many subset/superset/intersection queries

Usage:
SOS sos;
for (int x : a) sos.add(x);
sos.build();

countSubsets(x)    -> y ⊆ x      ((y | x) == x)
countSupersets(x)  -> y ⊇ x      ((y & x) == x)
countIntersect(x,n)-> (y & x) != 0

Common Patterns:
(y | x) == x  -> subset
(y & x) == x  -> superset
(y & x) != 0  -> intersect
(y & x) == 0  -> disjoint = countSubsets((N-1)^x)
contains bits -> superset
contained in  -> subset

Recognize:
- subset/superset of mask
- contains all bits
- disjoint/intersect
- many bitmask queries

Problems:
CSES: Bit Problem
CF: 165E, 449D, 383E
LC: 982, 1125, 1494
*/


struct XorBasis {
    static const int LOG = 31;
    vector<int> basis;

    XorBasis() : basis(LOG, 0) {}

    void add(int x) {
        for (int i = LOG - 1; i >= 0; i--) {
            if ((x & (1 << i)) == 0) continue;

            if (basis[i] == 0) {
                basis[i] = x;
                return;
            }

            x ^= basis[i];
        }
    }

    int rank() {
        int cnt = 0;
        for (int x : basis)
            if (x) cnt++;
        return cnt;
    }

    int maxXor() {
        int res = 0;
        for (int i = LOG - 1; i >= 0; i--) {
            if ((res ^ basis[i]) > res) {
                res ^= basis[i];
            }
        }
        return res;
    }

    long long countDistinctXors() {
        return 1LL << rank();
    }
};

/*
=========================================================
BITSET TEMPLATE & CP/OA TRICKS
=========================================================
std::bitset is a class template that represents a fixed-size sequence of N bits.
It provides a space-optimized representation of booleans (1 bit per element)
and optimizes bitwise operations by a factor of 32 or 64 (using word-level parallelism).

Complexity Improvement: O(N) -> O(N / 64)

1. Initialization & Basics:
--------------------------------
bitset<1000> b;           // Init all 0s
bitset<1000> b(val);      // Init from unsigned long long
bitset<1000> b("0101");   // Init from binary string (right-to-left mapping)

b.set(i)                  // Sets i-th bit to 1 (0-indexed)
b.reset(i)                // Sets i-th bit to 0
b.flip(i)                 // Toggles i-th bit
b.test(i)                 // Returns true if i-th bit is 1 (throws if out of bounds)
b[i]                      // Returns reference to i-th bit (no bounds check)

b.set()                   // Sets all bits to 1
b.reset()                 // Sets all bits to 0
b.flip()                  // Toggles all bits

b.count()                 // Number of set bits (popcount)
b.any()                   // Returns true if any bit is 1
b.none()                  // Returns true if no bit is 1
b.all()                   // Returns true if all bits are 1
b.size()                  // Returns N

b.to_ulong()              // Converts to unsigned long (throws if overflows)
b.to_ullong()             // Converts to unsigned long long (throws if overflows)
b.to_string()             // Converts to std::string

2. GCC Specific Extensions (Game Changers for CP):
--------------------------------
Since CP contests (Codeforces, AtCoder, Codechef) and most OAs use GCC,
you can use these non-standard but extremely fast helper functions:

b._Find_first()           // Returns the index of the first set bit (LSB).
                          // Returns b.size() if no bits are set.
b._Find_next(idx)         // Returns the index of the next set bit after `idx`.
                          // Returns b.size() if no more bits are set.

*Usage: Iterating over all set bits of a bitset in O(count/64):
for (size_t i = b._Find_first(); i < b.size(); i = b._Find_next(i)) {
    // Process index `i`
}

3. CP/OA Tricks & Patterns:
--------------------------------
A. Subset Sum / Knapsack DP (Reachability):
   If we want to find all reachable subset sums given array `A`:
   Standard DP: O(N * Sum) -> DP with bitset: O(N * Sum / 64)
   
   bitset<MAX_SUM> dp;
   dp[0] = 1;
   for (int x : A) {
       dp |= (dp << x); // or dp |= dp.operator<<((size_t)x) to avoid compiler issues
   }
   // Now dp[i] is 1 if sum `i` is reachable.

B. Bitset for Graph Reachability / Transitive Closure:
   Find all reachable nodes in a DAG (or any graph via Floyd-Warshall).
   Optimized to O(N^3 / 64) or O(N * (N + M) / 64):
   
   vector<bitset<MAXN>> reach(N);
   // Initialize reach[i][i] = 1 and reach[i][j] = 1 if edge i->j exists
   // Process DAG in reverse topological order:
   for (int u : reverse_topo_order) {
       for (int v : adj[u]) {
           reach[u] |= reach[v];
       }
   }

C. Set Intersection / Common Elements:
   Given two sets of numbers, find count of common elements:
   (bitset1 & bitset2).count() // O(N / 64)

D. Bitset for Grid/Matrix Path:
   Checking if there's a path in a grid where we only move down and right.

E. Sieve of Eratosthenes Space Optimization:
   bitset<MAX_VAL> is_prime;
   is_prime.set();
   is_prime[0] = is_prime[1] = 0;
   for (int p = 2; p * p < MAX_VAL; p++) {
       if (is_prime[p]) {
           for (int i = p * p; i < MAX_VAL; i += p) {
               is_prime.reset(i);
           }
       }
   }

4. Crucial Gotchas & Warnings:
--------------------------------
- Compile-time Constant Size: The size N must be a compile-time constant.
  If you need dynamic size, use `std::vector<bool>` (space-optimized but lacks bitwise shifts)
  or `boost::dynamic_bitset` (not always available in OAs).
- Pass by Reference: Always pass bitsets to functions by reference (e.g., `const bitset<N>& b`)
  to avoid O(N/64) copy overhead.
- Stack Overflow: Large bitsets (e.g., N >= 10^7) should NOT be declared inside functions
  (on the stack) as it will cause a Stack Overflow. Declare them globally or dynamically:
  `auto dp = make_unique<bitset<10000000>>();`
- Implicit Conversion Warning: Shift operators `<<` and `>>` expect `size_t`. 
  To avoid compiler or VS Code IntelliSense overload warnings, cast shift amounts:
  `dp << (size_t)sz` or `dp << static_cast<size_t>(sz)`.
=========================================================
*/

struct Bitset {
    // 1. Subset Sum
    template<size_t S>
    static bitset<S> subset(const vector<int>& a) {
        bitset<S> dp;
        dp[0] = 1;
        for (int x : a) dp |= (dp << x);
        return dp;
    }

    // 2. Transitive Closure
    template<size_t N>
    static vector<bitset<N>> reach(int n, vector<vector<int>>& g) {
        vector<bitset<N>> r(n);
        for (int i = 0; i < n; i++) {
            r[i][i] = 1;
            for (int j : g[i]) r[i][j] = 1;
        }

        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                if (r[i][k]) r[i] |= r[k];

        return r;
    }

};

struct MatrixExponentiation {
    static vector<vector<long long>> multiply(const vector<vector<long long>>& A, const vector<vector<long long>>& B, long long MOD) {
        int n = A.size();
        int m = B[0].size();
        int p = B.size();
        vector<vector<long long>> C(n, vector<long long>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < p; k++) {
                    C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
                }
            }
        }
        return C;
    } // multiplies two matrices A and B under modulo MOD, returns the resulting matrix C.
    // Here, each matric is represented as a vector of vectors, where the outer vector represents rows and the inner vector represents columns. The function iterates through each cell of the resulting matrix C and computes its value by summing the products of corresponding elements from A and B, taking care to apply the modulo operation to prevent overflow.

    static vector<vector<long long>> power(vector<vector<long long>> A, long long b, long long MOD) {
        int n = A.size();
        vector<vector<long long>> res(n, vector<long long>(n, 0));
        for (int i = 0; i < n; i++) res[i][i] = 1; // Identity matrix
        while (b) {
            if (b & 1) res = multiply(res, A, MOD);
            A = multiply(A, A, MOD);
            b >>= 1;
        }
        return res;
    } // computes the power of a square matrix A raised to the exponent b under modulo MOD using binary exponentiation. It initializes the result matrix res as the identity matrix and iteratively squares the matrix A while halving the exponent b. If the current bit of b is set (i.e., b is odd), it multiplies the result matrix res by A. This process continues until b becomes zero, and the final result is returned.
};

int computeNthFibo(int n, int MOD) {
    // use matrix exponentiation to compute Fibonacci numbers efficiently
    // recurrence relation is {F(n), F(n-1)} = {{1, 1}, {1, 0}}^(n-1) * {F(1), F(0)}
    vector<vector<long long>> base = {{1, 1}, {1, 0}};
    vector<vector<long long>> result = MatrixExponentiation::power(base, n, MOD);
    return result[0][1]; // the nth Fibonacci number is located at position (0, 1) in the resulting matrix after exponentiation.
} // computes the nth Fibonacci number modulo MOD using matrix exponentiation. It defines the base matrix for the Fibonacci recurrence relation and uses the power function to compute its nth power. The result is extracted from the appropriate position in the resulting matrix.

const int MOD = 1e9 + 7;

struct CountOfEulerianSubgraphs {
    int n;
    vector<vector<int>> adj;
    vector<bool> vis;

    CountOfEulerianSubgraphs(int n) : n(n), adj(n), vis(n, false) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u) {
        vis[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) dfs(v);
        }
    }

    int count(int m) {
        int components = 0;

        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                dfs(i);
                components++;
            }
        }

        return power(2, m - n + components, MOD);
    }
}; /*
Proof:
Let xe = 1 if edge e is included, otherwise 0.
For every vertex, the sum of incident xe must be even, giving one linear equation over GF(2).
Thus Eulerian subgraphs are exactly the solutions of Ax = 0, where A is the incidence matrix.
The number of solutions is 2^(m - rank(A)).
For an undirected graph, rank(A) = n - c, where c is the number of connected components.
Hence the answer is 2^(m - n + c).
*/


int computeArea(int x1, int y1, int x2, int y2,
                int x3, int y3, int x4, int y4) {

    int area1 = (x2 - x1) * (y2 - y1);
    int area2 = (x4 - x3) * (y4 - y3);

    int overlapWidth = max(0, min(x2, x4) - max(x1, x3));
    int overlapHeight = max(0, min(y2, y4) - max(y1, y3));

    int overlapArea = overlapWidth * overlapHeight;

    return area1 + area2 - overlapArea;
} // finds the total area covered by two rectangles defined by their bottom-left and top-right coordinates. It calculates the area of each rectangle, determines the overlapping region (if any), and subtracts the overlap from the sum of the individual areas to avoid double counting.