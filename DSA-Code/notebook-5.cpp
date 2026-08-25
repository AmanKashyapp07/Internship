#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// Modular exponentiation (a^b) % mod
// Time: O(log B), Space: O(1)

ll power(ll a, ll b, ll mod = MOD) {
    ll res = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1)
            res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

// Matrix multiplication modulo MOD
// Time: O(K^3), Space: O(K^2)

vvl multiply(const vvl &A, const vvl &B, int k) {
    vvl C(k, vl(k, 0));
    for (int i = 0; i < k; i++) {
        for (int m = 0; m < k; m++) {
            for (int j = 0; j < k; j++) {
                C[i][j] = (C[i][j] + A[i][m] * B[m][j]) % MOD;
            }
        }
    }
    return C;
}

// Square matrix exponentiation (A^p) % MOD
// Time: O(K^3 log P), Space: O(K^2)

vvl power(vvl A, ll p, int k) {
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; i++)
        res[i][i] = 1;

    while (p > 0) {
        if (p & 1)
            res = multiply(res, A, k);
        A = multiply(A, A, k);
        p >>= 1;
    }
    return res;
}

// Number of paths of length K from src to dest using matrix exponentiation
// Time: O(V^3 log K), Space: O(V^2)

ll countPathOfLengthK(vvl &adj, int src, int dest, ll k) {
    return power(adj, k, adj.size())[src][dest];
}

// N-th Fibonacci number modulo MOD via matrix exponentiation
// Time: O(log N), Space: O(1)

int calculateNthFiboacci(int n) {
    if (n <= 1)
        return n;
    vvl F = {{1, 1}, {1, 0}};
    return power(F, n - 1, 2)[0][0];
}

// Maximum subarray sum of length at most K using deque prefix sum
// Time: O(N), Space: O(N)

ll maxSubarraySumAtMostK(const vi &a, int k) {
    int n = a.size();
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + a[i];
    }

    deque<int> dq = {0};
    ll ans = -1e18;

    for (int i = 1; i <= n; i++) {
        while (!dq.empty() && dq.front() < i - k)
            dq.pop_front();
        ans = max(ans, pref[i] - pref[dq.front()]);
        while (!dq.empty() && pref[dq.back()] >= pref[i])
            dq.pop_back();
        dq.push_back(i);
    }

    return ans;
}

// K-th lexicographical permutation of numbers 1 to N (1-indexed K)
// Time: O(N^2), Space: O(N)

vi kthPermutation(int n, int k) {
    vi fact(n + 1, 1), nums(n), result;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i;
    iota(nums.begin(), nums.end(), 1);

    k--;
    for (int i = n; i >= 1; i--) {
        int idx = k / fact[i - 1];
        result.push_back(nums[idx]);
        nums.erase(nums.begin() + idx);
        k %= fact[i - 1];
    }

    return result;
}

// 1-based lexicographical rank of a permutation
// Time: O(N^2), Space: O(N)

ll permutationRank(const vi &p) {
    int n = p.size();
    vl fact(n + 1, 1);
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i;

    vi unused(n);
    iota(unused.begin(), unused.end(), 1);

    ll rank = 0;
    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();
        rank += idx * fact[n - i - 1];
        unused.erase(unused.begin() + idx);
    }

    return rank + 1;
}

// Next greater permutation in-place
// Time: O(N), Space: O(1)

vi next_permutation(vi &p) {
    int n = p.size(), i = n - 2;
    while (i >= 0 && p[i] >= p[i + 1])
        i--;

    if (i >= 0) {
        int j = n - 1;
        while (p[j] <= p[i])
            j--;
        swap(p[i], p[j]);
    }
    reverse(p.begin() + i + 1, p.end());
    return p;
}

// LCM of array elements modulo MOD via prime factorization
// Time: O(N * sqrt(MAX_VAL)), Space: O(U) distinct primes

ll getLCM(const vi &nums) {
    unordered_map<int, int> max_pwr;
    for (int x : nums) {
        int temp = x;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                max_pwr[p] = max(max_pwr[p], cnt);
            }
        }
        if (temp > 1)
            max_pwr[temp] = max(max_pwr[temp], 1);
    }

    ll lcm = 1;
    for (auto [prime, pwr] : max_pwr) {
        lcm = (lcm * power(prime, pwr, MOD)) % MOD;
    }
    return lcm;
}

// Minimum rounds to return permutation to original order (LCM of cycle lengths)
// Time: O(N * sqrt(N)), Space: O(N)

int permutationRounds(int n, const vi &a) {
    vector<bool> vis(n + 1, false);
    vi cycle_len;

    for (int i = 1; i <= n; i++) {
        if (vis[i])
            continue;
        int cur = i, len = 0;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur];
            len++;
        }
        cycle_len.push_back(len);
    }
    return getLCM(cycle_len);
}

// Fenwick Tree (Binary Indexed Tree) for point updates and prefix sums
// Time: O(log N) update/query, Space: O(N)

struct Fenwick {
    int n;
    vi bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }

    int sum(int idx) {
        int res = 0;
        for (; idx > 0; idx -= idx & -idx)
            res += bit[idx];
        return res;
    }
};

// Minimum adjacent swaps to sort array (inversion count)
// Time: O(N log N), Space: O(N)

ll minAdjacentSwaps(const vi &a) {
    int n = a.size();
    Fenwick ft(n);
    ll inv = 0;

    for (int i = n - 1; i >= 0; i--) {
        inv += ft.sum(a[i] - 1);
        ft.add(a[i], 1);
    }
    return inv;
}

// Minimum arbitrary swaps to sort 1-indexed permutation (N - cycles)
// Time: O(N), Space: O(N)

int minAnySwaps(const vi &a) {
    int n = a.size(), cycles = 0;
    vector<bool> vis(n, false);

    for (int i = 0; i < n; i++) {
        if (vis[i])
            continue;
        cycles++;
        int cur = i;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur] - 1;
        }
    }
    return n - cycles;
}

// Minimum move-to-anywhere operations to sort array (N - LIS)
// Time: O(N log N), Space: O(N)

int minMoveAnywhere(const vi &a) {
    vi lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end())
            lis.push_back(x);
        else
            *it = x;
    }
    return (int)a.size() - (int)lis.size();
}

// Minimum move-to-front operations to sort permutation of size N
// Time: O(N), Space: O(1)

int minMoveToFront(const vi &a) {
    int n = a.size(), need = n;
    for (int i = n - 1; i >= 0; i--) {
        if (a[i] == need)
            need--;
    }
    return need;
}

// Check if string is formed by repeating a substring
// Time: O(N), Space: O(N)

bool repeatedSubstringPattern(const string &s) {
    return (s + s).substr(1, 2 * s.size() - 2).find(s) != string::npos;
}

// Minimum continuous subarrays needed with sum not exceeding K
// Time: O(N), Space: O(1)

int countSubarrays(const vi &nums, int k) {
    int sum = 0, cnt = 1;
    for (int x : nums) {
        sum += x;
        if (sum > k) {
            sum = x;
            cnt++;
        } else if (sum == k) {
            sum = 0;
            cnt++;
        }
    }
    return cnt;
}

// K-th smallest element via Lomuto QuickSelect
// Time: O(N) average, Space: O(1)

int quickSelect(vi &nums, int k) {
    int l = 0, r = (int)nums.size() - 1;
    while (true) {
        int pivot = nums[r], p = l;
        for (int i = l; i < r; i++) {
            if (nums[i] < pivot)
                swap(nums[p++], nums[i]);
        }
        swap(nums[p], nums[r]);

        if (p == k)
            return nums[p];
        if (p < k)
            l = p + 1;
        else
            r = p - 1;
    }
}

// Generate adjacent state transitions for string BFS
// Time: Problem dependent, Space: O(branching factor)

vector<string> getNeighbors(const string &cur) { return {}; }

// Bidirectional BFS for shortest path between start and target strings
// Time: O(B^(D/2)), Space: O(B^(D/2))

int bidirectionalBFS(const string &start, const string &target) {
    if (start == target)
        return 0;
    unordered_set<string> fwd = {start}, bwd = {target}, vis = {start, target};
    int steps = 0;

    while (!fwd.empty() && !bwd.empty()) {
        if (fwd.size() > bwd.size())
            swap(fwd, bwd);
        unordered_set<string> nxt_set;
        steps++;

        for (const string &cur : fwd) {
            for (const string &nxt : getNeighbors(cur)) {
                if (bwd.count(nxt))
                    return steps;
                if (!vis.count(nxt)) {
                    vis.insert(nxt);
                    nxt_set.insert(nxt);
                }
            }
        }
        fwd = move(nxt_set);
    }
    return -1;
}

bool checkValidString(const string &s) {
    int cmin = 0, cmax = 0;
    // cmin represents the minimum number of open parentheses that must be closed
    // cmax represents the maximum number of open parentheses that can be closed
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            cmin++;
            cmax++;
        } else if (s[i] == ')') {
            cmin--;
            cmax--;
        } else {
            cmin--;
            cmax++;
        }
        if (cmax < 0) // If at any point the maximum number of open parentheses is negative, it means there are more closing parentheses than opening ones, which is invalid.
            return false;
        if (cmin < 0) // If the minimum number of open parentheses is negative, we reset it to 0 because we can't have a negative count of open parentheses.
            cmin = 0;
    }
    return cmin == 0;
}

int countPairs(vector<int>& a, int x) {
    if (a.size() <= 1) return 0;

    int mid = a.size() / 2;
    vector<int> l(a.begin(), a.begin() + mid);
    vector<int> r(a.begin() + mid, a.end());

    int cnt = countPairs(l, x) + countPairs(r, x);

    int j = 0;
    for (int i = 0; i < l.size(); i++) {
        while (j < r.size() && l[i] > 1LL * x * r[j])j++;
        cnt += j;
    }

    merge(l.begin(), l.end(), r.begin(), r.end(), a.begin()); // Merge the two halves back into the original array to maintain sorted order for further recursive calls.
    // time complexity: O(n log n) due to the merge sort approach, space complexity: O(n) for the temporary arrays l and r.
    // merge is built in function in C++ STL that merges two sorted ranges into a single sorted range.

    return cnt;
} // finds the number of pairs (i, j) such that i < j and a[i] > x * a[j] 
