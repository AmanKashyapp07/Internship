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

    while (i <= mid)
        temp[k++] = nums[i++];

    while (j <= right)
        temp[k++] = nums[j++];

    for (int p = 0; p < k; p++)
        nums[left + p] = temp[p];

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
========================================
SOS DP (Sum Over Subsets DP)
========================================

Preprocessing:
    Time   : O(B * 2^B)
    Memory : O(2^B)

Use when:
    - Masks have <= 20~22 bits.
    - Need counts/info over ALL subsets or ALL supersets.
    - Many queries on bitmasks.

----------------------------------------
Usage
----------------------------------------

SOS sos;

for (int x : a)
    sos.add(x);      // Insert every mask

sos.build();         // Call once after all insertions

Queries:

sos.countSubsets(x);
    # y such that (y | x) == x
    => y is a subset of x.

sos.countSupersets(x);
    # y such that (y & x) == x
    => y is a superset of x.

sos.countIntersect(x, n);
    # y such that (y & x) != 0
    where n = total inserted masks.

----------------------------------------
Useful Bitmask Patterns
----------------------------------------

Need: y ⊆ x
Condition:
    (y | x) == x
Use:
    countSubsets(x)

------------------------

Need: x ⊆ y
Condition:
    (y & x) == x
Use:
    countSupersets(x)

------------------------

Need: (y & x) != 0
Answer:
    total - countSubsets(~x)

Implemented as:
    countIntersect(x, total)

------------------------

Need: (y & x) == 0
Answer:
    countSubsets((N - 1) ^ x)

------------------------

Need: mask contains all bits of x
Condition:
    (mask & x) == x
Use:
    countSupersets(x)

------------------------

Need: mask is contained inside x
Condition:
    (mask | x) == x
Use:
    countSubsets(x)

----------------------------------------
Recognizing SOS DP
----------------------------------------

Look for:

    (a | b) == b    -> subset queries
    (a & b) == b    -> superset queries
    (a & b) != 0    -> intersect
    (a & b) == 0    -> disjoint
    "contains all bits"
    "is subset/superset"
    many bitmask queries

----------------------------------------
Common Problems
----------------------------------------

CSES:
    - Bit Problem

Codeforces:
    - 165E Compatible Numbers
    - 449D Jzzhu and Numbers
    - 383E Vowels

LeetCode:
    - 982 Triples with Bitwise AND Equal To Zero
    - 1125 Smallest Sufficient Team
    - 1494 Parallel Courses II
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
