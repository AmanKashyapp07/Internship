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

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using vvi = vector<vector<int>>;
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
const ll MOD = 1e9 + 7; // this is a prime number, which is useful for modular arithmetic



// Binary Exponentiation
int power(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

// Modular Inverse (MOD must be prime)
int inv(int x) { return power(x, MOD - 2); }

// Factorials & combinations
vi fac, ifac;
void init_nCr(int n) {
    fac.resize(n + 1); ifac.resize(n + 1);
    fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % MOD;
    ifac[n] = power(fac[n], MOD - 2);
    for (int i = n; i > 0; i--) ifac[i - 1] = ifac[i] * i % MOD;
}
int nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[r] % MOD * ifac[n - r] % MOD;
}
int nPr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[n - r] % MOD;
}

// Primes & Factorization
vi sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) is_prime[j] = false;
        }
    }
    vi primes;
    for (int i = 2; i <= n; i++) if (is_prime[i]) primes.push_back(i);
    return primes;
}

vector<pii> prime_factorize(int n) {
    vector<pii> pf;
    for (int p = 2; p * p <= n; p++) {
        if (n % p) continue;
        int cnt = 0;
        while (n % p == 0) { n /= p; cnt++; }
        pf.push_back({p, cnt});
    }
    if (n > 1) pf.push_back({n, 1});
    return pf;
}

/*
Bitwise Operations:
- Check set bit: x & (1 << i)        | Toggle bit: x ^= (1 << i)
- Set bit: x |= (1 << i)            | Clear bit: x &= ~(1 << i)
- Remove lowest set bit: x & (x - 1)| Value of lowest set bit: x & -x
- Power of 2: (x & (x - 1)) == 0
- __builtin_popcount(x)             | __builtin_popcountll(x) -> Count set bits
- __builtin_clz(x)                  | __builtin_clzll(x) -> Leading zeros
- __builtin_ctz(x)                  | __builtin_ctzll(x) -> Trailing zeros
*/

int XORupto(int n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        case 3: return 0;
    }
}

// Sliding Window Extreme Queries
vi maxSlidingWindow(const vi& nums, int k) {
    deque<int> dq; vi ans; int n = nums.size();
    for (int R = 0; R < n; R++) {
        int L = R - k + 1;
        while (!dq.empty() && dq.front() < L) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[R]) dq.pop_back();
        dq.push_back(R);
        if (L >= 0) ans.push_back(nums[dq.front()]);
    }
    return ans;
}

vi minSlidingWindow(const vi& nums, int k) {
    deque<int> dq; vi ans; int n = nums.size();
    for (int R = 0; R < n; R++) {
        int L = R - k + 1;
        while (!dq.empty() && dq.front() < L) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] >= nums[R]) dq.pop_back();
        dq.push_back(R);
        if (L >= 0) ans.push_back(nums[dq.front()]);
    }
    return ans;
}

// LIS Queries
int lis(const vi& a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x);
        else *it = x;
    }
    return dp.size();
}

vector<int> longestIncreasingSubsequence(vector<int>& nums) {
    if (nums.empty()) return {};

    int n = nums.size();
    vector<int> tailIndex;
    vector<int> parent(n, -1);

    for (int i = 0; i < n; i++) {
        // Binary search for position in tailIndex
        auto it = lower_bound(
            tailIndex.begin(),
            tailIndex.end(),
            nums[i],
            [&](int idx, int val) {
                return nums[idx] < val;
            }); // finds the first index in tailIndex where nums[idx] >= nums[i]

        int pos = it - tailIndex.begin();

        // 1. Link parent FIRST (predecessor is the top of the previous pile)
        if (pos > 0) {
            parent[i] = tailIndex[pos - 1];
        }

        // 2. Update pile top AFTER linking
        if (it == tailIndex.end()) {
            tailIndex.push_back(i);
        } else {
            tailIndex[pos] = i;
        }
    }

    // Reconstruct LIS starting from the end of the longest pile chain
    vector<int> lis;
    int cur = tailIndex.back();

    while (cur != -1) {
        lis.push_back(nums[cur]);
        cur = parent[cur];
    }

    reverse(lis.begin(), lis.end());

    return lis;
}

// LCS Queries
string lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size(); // dp[i][j] = length of LCS of a[0..i-1] and b[0..j-1]
    vvi dp(n + 1, vi(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    string result; int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) { result += a[i - 1]; i--; j--; }
        else if (dp[i - 1][j] > dp[i][j - 1]) i--;
        else j--;
    }
    reverse(result.begin(), result.end());
    return result;
}

/*
Interval DP Bottom-Up transition: dp[l][r] = min_{k}(dp[l][k] + dp[k+1][r] + cost)
For matrices (MCM): cost = arr[l] * arr[k+1] * arr[r+1]
For burst balloons: cost = arr[l-1] * arr[k] * arr[r+1]
For stick cutting: cost = stick[r] - stick[l]
*/
int solveIntervalDPBottomUp(int n) {
    vvi dp(n, vi(n, 0));
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1; dp[l][r] = LLONG_MAX;
            for (int k = l; k < r; k++) {
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
            }
        }
    }
    return dp[0][n - 1];
}

// Combinatorics / Subsets / Permutations
vvi generateSubsets(vi& nums) {
    vvi subsets; int n = nums.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        vi subset;
        for (int i = 0; i < n; i++) if (mask & (1 << i)) subset.push_back(nums[i]);
        subsets.push_back(subset);
    }
    return subsets;
}

int sumOfAllSubsets(vi& nums) {
    int n = nums.size(); int total_sum = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int subset_sum = 0;
        for (int i = 0; i < n; i++) if (mask & (1 << i)) subset_sum += nums[i];
        total_sum += subset_sum;
    }
    return total_sum;
}

vi kadane(const vi& nums) {
    int n = nums.size(); int maxi = INT_MIN, sum = 0;
    int start = 0, end = 0, temp_start = 0;
    for (int i = 0; i < n; i++) {
        sum += nums[i];
        if (sum > maxi) { maxi = sum; start = temp_start; end = i; }
        if (sum < 0) { sum = 0; temp_start = i + 1; } // if current sum < 0, reset the start index for the next potential subarray, as there is no advantage in keeping a negative sum when looking for the maximum sum subarray
    }
    return {maxi, start, end};
}

void solve_perm(vi& nums, vi& curr, vvi& ans, int mask) {
    if (curr.size() == nums.size()) { ans.push_back(curr); return; }
    for (int i = 0; i < nums.size(); i++) {
        if (!(mask & (1 << i))) {
            curr.push_back(nums[i]);
            solve_perm(nums, curr, ans, mask | (1 << i));
            curr.pop_back();
        }
    }
}
vvi generatePermutations(vi& nums) {
    vvi ans; vi curr;
    solve_perm(nums, curr, ans, 0);
    return ans;
}

// Palindromes
int lps(string s) {
    int n = s.size(); vvi dp(n, vi(n, 0));
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

bool checkSubsequence(const string& s, const string& t) {
    int n = s.size(), m = t.size(); int j = 0;
    int i=0;
    while(i<n && j<m) {
        if(s[i] == t[j]) j++;
        i++;
    }
    return j == m;
}

vvi palindromeTable(vi& nums) {
    int n = nums.size(); vvi isPal(n, vi(n, 0));
    for(int i=0; i<n; i++) isPal[i][i] = 1;
    for(int len=2; len<=n; len++) {
        for(int l=0; l+len<=n; l++) {
            int r = l + len - 1;
            if(nums[l] == nums[r]) isPal[l][r] = (len == 2) ? 1 : isPal[l+1][r-1];
            else isPal[l][r] = 0;
        }
    }
    return isPal;
}

// String rotation / Booth Algorithm
string boothAlgorithm(string s) {
    string t = s + s; int n = s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) k++;
        else if (t[i + k] < t[j + k]) { j += k + 1; if (i == j) j++; k = 0; }
        else { i += k + 1; if (i == j) i++; k = 0; }
    }
    return t.substr(min(i, j), n);
} // it returns the lexicographically smallest rotation of the string s

// MEX helper
int mex(const vi& nums) {
    unordered_set<int> s(nums.begin(), nums.end()); int m = 0;
    while (s.count(m)) m++;
    return m;
}

// Inversion Count via Merge Sort
int mergeInversions(vi& nums, int left, int mid, int right) {
    int inversions = 0; vi temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) temp[k++] = nums[i++];
        else { temp[k++] = nums[j++]; inversions += (mid - i + 1); }
    }
    while (i <= mid) temp[k++] = nums[i++];
    while (j <= right) temp[k++] = nums[j++];
    for (int p = 0; p < k; p++) nums[left + p] = temp[p];
    return inversions;
}
int mergeSortInversions(vi& nums, int left, int right) {
    if (left >= right) return 0;
    int mid = left + (right - left) / 2;
    int inversions = 0;
    inversions += mergeSortInversions(nums, left, mid);
    inversions += mergeSortInversions(nums, mid + 1, right);
    inversions += mergeInversions(nums, left, mid, right);
    return inversions;
}
int countInversions(vi& nums) {
    return mergeSortInversions(nums, 0, nums.size() - 1);
}

vvi multiplyMatrices(const vvi& A, const vvi& B) {
    int n = A.size(), m = B[0].size(), p = B.size();
    vvi C(n, vi(m, 0));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < p; k++) {       // Swapped k and j
            for (int j = 0; j < m; j++) {   // Accesses B[k][j] contiguously in cache
                C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % MOD;
            }
        }
    }
    return C;
}

vvi powerMatrices(vvi A, long long b) { // Use long long for exponent
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


int nthFibonacci(long long n) {
    // Base cases (1-indexed)
    if (n == 1) return 1 % MOD;
    if (n == 2) return 2 % MOD;

    vvi F = {{1, 1}, {1, 0}};
    vvi res = powerMatrices(F, n - 2);

    // F_n = 2 * res[0][0] + res[0][1]
    long long ans = (2LL * res[0][0] + res[0][1]) % MOD;
    return ans;
}   

// Area calculation
int computeArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int area1 = (x2 - x1) * (y2 - y1);
    int area2 = (x4 - x3) * (y4 - y3);
    int overlapWidth = max(0, min(x2, x4) - max(x1, x3));
    int overlapHeight = max(0, min(y2, y4) - max(y1, y3));
    return area1 + area2 - (overlapWidth * overlapHeight);
}

int countOfBalancedParentheses(int len) {
    // we have to output catalan number of n pairs of balanced parentheses
    // C(n) = (2n)! / ((n + 1)! * n!) = C(2n, n) / (n + 1)
    if(len % 2 != 0) return 0; // odd length cannot form balanced parentheses
    int n = len / 2;
    init_nCr(2 * n);
    return nCr(2 * n, n) * inv(n + 1) % MOD;
}

