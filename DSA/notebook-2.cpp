#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// Binary exponentiation (modular power)
// Time: O(log B), Space: O(1)

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

// Modular inverse using Fermat's Little Theorem
// Time: O(log MOD), Space: O(1)

ll inv(ll x) {
    return power(x, MOD - 2);
}

// Factorials and Combinatorics (nCr, nPr)
// Time: O(N) init, O(1) query, Space: O(N)

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

// Sieve of Eratosthenes prime generation
// Time: O(N log log N), Space: O(N)

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

// Prime factorization of an integer
// Time: O(sqrt(N)), Space: O(log N)

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

// Prefix XOR from 1 to N
// Time: O(1), Space: O(1)

int XORupto(int n) {
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}

// Sliding window maximum
// Time: O(N), Space: O(K)

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

// Sliding window minimum
// Time: O(N), Space: O(K)

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

// Maximum subarray sum with length at most K
// Time: O(N), Space: O(N)

ll maxSubarraySumAtMostK(const vi &nums, int k) {
    int n = nums.size();
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + nums[i];
    }

    deque<int> dq;
    ll ans = -1e18;

    for (int r = 1; r <= n; r++) {
        int l = r - k;
        while (!dq.empty() && dq.front() < l) dq.pop_front();
        while (!dq.empty() && pref[dq.back()] >= pref[r - 1]) dq.pop_back();
        dq.push_back(r - 1);
        ans = max(ans, pref[r] - pref[dq.front()]);
    }

    return ans;
}

// Length of Longest Increasing Subsequence (LIS)
// Time: O(N log N), Space: O(N)

int lis(const vi &a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x);
        else *it = x;
    }
    return dp.size();
}

// Reconstruction of Longest Increasing Subsequence (LIS)
// Time: O(N log N), Space: O(N)

vi longestIncreasingSubsequence(const vi &nums) {
    if (nums.empty()) return {};
    int n = nums.size();
    vi tail_idx, par(n, -1);

    for (int i = 0; i < n; i++) {
        int l = 0, r = (int)tail_idx.size() - 1, pos = tail_idx.size();
        while (l <= r) {
            int mid = (l + r) / 2;
            if (nums[tail_idx[mid]] >= nums[i]) {
                pos = mid;
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }

        if (pos > 0) par[i] = tail_idx[pos - 1];

        if (pos == (int)tail_idx.size()) tail_idx.push_back(i);
        else tail_idx[pos] = i;
    }

    vi ans;
    int cur = tail_idx.back();
    while (cur != -1) {
        ans.push_back(nums[cur]);
        cur = par[cur];
    }
    reverse(ans.begin(), ans.end());

    return ans;
}

// Longest Common Subsequence (LCS) string reconstruction
// Time: O(N * M), Space: O(N * M)

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

// Interval DP template (bottom-up matrix chain multiplication pattern)
// Time: O(N^3), Space: O(N^2)

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

// Generate all subsets using bitmask
// Time: O(N * 2^N), Space: O(N * 2^N)

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

// Sum of all element sums over all subsets
// Time: O(N * 2^N), Space: O(1)

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

// Kadane's algorithm for Maximum Subarray Sum with indices
// Time: O(N), Space: O(1)

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

// Generate all permutations using bitmask backtracking
// Time: O(N! * N), Space: O(N! * N)

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

// Longest Palindromic Subsequence (LPS) length
// Time: O(N^2), Space: O(N^2)

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

// Check if t is a subsequence of s
// Time: O(|S|), Space: O(1)

bool checkSubsequence(const string &s, const string &t) {
    int n = s.size(), m = t.size();
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (s[i] == t[j]) j++;
        i++;
    }
    return j == m;
}

// Precompute 2D table of all palindromic subarrays
// Time: O(N^2), Space: O(N^2)

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

// Booth's algorithm for lexicographically smallest string rotation
// Time: O(N), Space: O(N)

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

// Minimum Excluded value (MEX) of an array
// Time: O(N), Space: O(N)

int mex(const vi &nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int val = 0;
    while (st.count(val)) val++;
    return val;
}

// Inversion count using Merge Sort
// Time: O(N log N), Space: O(N)

int mergeInversions(vi &nums, int l, int mid, int r) {
    int invs = 0;
    vi temp(r - l + 1);
    int i = l, j = mid + 1, k = 0;

    while (i <= mid && j <= r) {
        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
            invs += (mid - i + 1);
        }
    }
    while (i <= mid) temp[k++] = nums[i++];
    while (j <= r) temp[k++] = nums[j++];

    for (int p = 0; p < k; p++) nums[l + p] = temp[p];
    return invs;
}

int mergeSortInversions(vi &nums, int l, int r) {
    if (l >= r) return 0;
    int mid = l + (r - l) / 2;
    int invs = 0;
    invs += mergeSortInversions(nums, l, mid);
    invs += mergeSortInversions(nums, mid + 1, r);
    invs += mergeInversions(nums, l, mid, r);
    return invs;
}

int countInversions(vi nums) {
    return mergeSortInversions(nums, 0, (int)nums.size() - 1);
}

// Matrix multiplication modulo MOD
// Time: O(N^3), Space: O(N^2)

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

// Matrix exponentiation modulo MOD
// Time: O(N^3 log B), Space: O(N^2)

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

// N-th Fibonacci number using matrix exponentiation
// Time: O(log N), Space: O(1)

ll nthFibonacci(ll n) {
    if (n == 1) return 1 % MOD;
    if (n == 2) return 2 % MOD;

    vvi F = {{1, 1}, {1, 0}};
    vvi res = powerMatrices(F, n - 2);

    return (2LL * res[0][0] + res[0][1]) % MOD;
}

// Total area covered by two 2D rectangles
// Time: O(1), Space: O(1)

int computeArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int area1 = (x2 - x1) * (y2 - y1);
    int area2 = (x4 - x3) * (y4 - y3);
    int w = max(0, min(x2, x4) - max(x1, x3));
    int h = max(0, min(y2, y4) - max(y1, y3));
    return area1 + area2 - (w * h);
}

// Count balanced parentheses strings of given length (Catalan number)
// Time: O(LEN), Space: O(LEN)

ll countOfBalancedParentheses(int len) {
    if (len % 2 != 0) return 0;
    int n = len / 2;
    init_nCr(2 * n);
    return nCr(2 * n, n) * inv(n + 1) % MOD;
}
