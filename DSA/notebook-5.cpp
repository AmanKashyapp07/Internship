#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

// Calculates (a^b) % mod using binary exponentiation.
// Time Complexity: O(log b), Space Complexity: O(1).
ll power(ll a, ll b, ll mod = MOD) {
    ll res = 1; a %= mod;
    while (b > 0) { if (b & 1) res = (res * a) % mod; a = (a * a) % mod; b >>= 1; }
    return res;
}

// Multiplies two k x k matrices A and B modulo MOD.
// Time Complexity: O(k^3), Space Complexity: O(k^2).
vvl multiply(const vvl& A, const vvl& B, int k) {
    vvl C(k, vl(k, 0));
    for (int i = 0; i < k; ++i)
        for (int j = 0; j < k; ++j)
            for (int m = 0; m < k; ++m)
                C[i][j] = (C[i][j] + A[i][m] * B[m][j]) % MOD;
    return C;
}

// Computes matrix exponentiation (A^p) % MOD for a square matrix of size k.
// Time Complexity: O(k^3 log p), Space Complexity: O(k^2).
vvl power(vvl A, long long p, int k) {
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; ++i) res[i][i] = 1;
    while (p > 0) { if (p & 1) res = multiply(res, A, k); A = multiply(A, A, k); p >>= 1; }
    return res;
}

// Counts the number of paths of length k from src to dest using matrix exponentiation.
// Time Complexity: O(V^3 log k), Space Complexity: O(V^2).
ll countPathOfLengthK(vvl& adj, int src, int dest, long long k) {
    return power(adj, k, adj.size())[src][dest];
}

// Computes the N-th Fibonacci number modulo MOD using 2x2 matrix exponentiation.
// Time Complexity: O(log n), Space Complexity: O(1).
int calculateNthFiboacci(int n) {
    if (n <= 1) return n;
    vvl F = {{1, 1}, {1, 0}};
    return power(F, n - 1, 2)[0][0];
}

// Finds maximum subarray sum with length at most k using monotonic deque and prefix sums.
// Time Complexity: O(n), Space Complexity: O(n).
long long maxSubarraySumAtMostK(vector<int>& a, int k) {
    int n = a.size();
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + a[i];
    deque<int> dq = {0};
    long long ans = LLONG_MIN;
    for (int i = 1; i <= n; i++) {
        while (!dq.empty() && dq.front() < i - k) dq.pop_front();
        ans = max(ans, prefix[i] - prefix[dq.front()]);
        while (!dq.empty() && prefix[dq.back()] >= prefix[i]) dq.pop_back();
        dq.push_back(i);
    }
    return ans;
}

// Generates the k-th lexicographical permutation of numbers from 1 to n (1-indexed k).
// Time Complexity: O(n^2), Space Complexity: O(n).
vi kthPermutation(int n, int k) {
    vector<int> fact(n + 1, 1), nums(n), result;
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
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

// Calculates the 1-based lexicographical rank of a given permutation.
// Time Complexity: O(n^2), Space Complexity: O(n).
ll permutationRank(vector<int>& p) {
    int n = p.size();
    vector<ll> fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    vector<int> unused(n);
    iota(unused.begin(), unused.end(), 1);
    ll rank = 0;
    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();
        rank += idx * fact[n - i - 1];
        unused.erase(unused.begin() + idx);
    }
    return rank + 1;
}

// Rearranges the vector into the lexicographically next greater permutation in-place.
// Time Complexity: O(n), Space Complexity: O(1).
vi next_permutation(vi& p) {
    int n = p.size(), i = n - 2;
    while (i >= 0 && p[i] >= p[i + 1]) i--;
    if (i >= 0) {
        int j = n - 1;
        while (p[j] <= p[i]) j--;
        swap(p[i], p[j]);
    }
    reverse(p.begin() + i + 1, p.end());
    return p;
}

// Computes the LCM of an array of numbers modulo MOD via prime factorization.
// Time Complexity: O(N * sqrt(MAX_VAL)), Space Complexity: O(U) distinct primes.
ll getLCM(vi& nums) {
    unordered_map<int, int> maxPower;
    for (int x : nums) {
        int temp = x;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) { temp /= p; cnt++; }
                maxPower[p] = max(maxPower[p], cnt);
            }
        }
        if (temp > 1) maxPower[temp] = max(maxPower[temp], 1);
    }
    ll lcm = 1;
    for (auto [prime, pwr] : maxPower) lcm = lcm * power(prime, pwr, MOD) % MOD;
    return lcm;
}

// Finds minimum rounds to return a permutation to original order (LCM of cycle lengths).
// Time Complexity: O(n * sqrt(n)), Space Complexity: O(n).
int permutationRounds(int n, vi& a) {
    vector<bool> vis(n + 1, false);
    vi cycleLengths;
    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;
        int cur = i, len = 0;
        while (!vis[cur]) { vis[cur] = true; cur = a[cur]; len++; }
        cycleLengths.push_back(len);
    }
    return getLCM(cycleLengths);
}

// Fenwick Tree (Binary Indexed Tree) for 1-indexed point updates and prefix sum queries.
// Time Complexity: O(log n) update/query, Space Complexity: O(n).
struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int idx, int val) { for (; idx <= n; idx += idx & -idx) bit[idx] += val; }
    int sum(int idx) { int res = 0; for (; idx > 0; idx -= idx & -idx) res += bit[idx]; return res; }
};

// Calculates minimum adjacent swaps to sort an array (equal to number of inversions).
// Time Complexity: O(n log n), Space Complexity: O(n).
ll minAdjacentSwaps(vector<int> &a) {
    int n = a.size();
    Fenwick ft(n);
    ll inv = 0;
    for (int i = n - 1; i >= 0; i--) { inv += ft.sum(a[i] - 1); ft.add(a[i], 1); }
    return inv;
}

// Calculates minimum arbitrary swaps to sort a 1-indexed permutation array (n - cycles).
// Time Complexity: O(n), Space Complexity: O(n).
int minAnySwaps(vector<int> &a) {
    int n = a.size(), cycles = 0;
    vector<bool> vis(n, false);
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        cycles++;
        int cur = i;
        while (!vis[cur]) { vis[cur] = true; cur = a[cur] - 1; }
    }
    return n - cycles;
}

// Calculates minimum move-to-any-position operations to sort array (n - LIS length).
// Time Complexity: O(n log n), Space Complexity: O(n).
int minMoveAnywhere(vector<int> &a) {
    vector<int> lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return (int)a.size() - (int)lis.size();
}

// Calculates minimum move-to-front operations to sort a permutation of size n.
// Time Complexity: O(n), Space Complexity: O(1).
int minMoveToFront(vector<int> &a) {
    int n = a.size(), need = n;
    for (int i = n - 1; i >= 0; i--) if (a[i] == need) need--;
    return need;
}

// Checks if string can be constructed by repeating a substring.
// Time Complexity: O(n), Space Complexity: O(n).
bool repeatedSubstringPattern(string s) {
    return (s + s).substr(1, 2 * s.size() - 2).find(s) != string::npos;
}

// Counts minimum continuous subarrays needed such that no subarray sum exceeds k.
// Time Complexity: O(n), Space Complexity: O(1).
int countSubarrays(vi nums, int k) {
    int sum = 0, cnt = 1;
    for (int x : nums) {
        sum += x;
        if (sum > k) { sum = x; cnt++; }
        else if (sum == k) { sum = 0; cnt++; }
    }
    return cnt;
}

// Finds the k-th smallest element using Lomuto partition QuickSelect algorithm.
// Time Complexity: O(n) average, O(n^2) worst case, Space Complexity: O(1).
int quickSelect(vector<int>& nums, int k) {
    int l = 0, r = nums.size() - 1;
    while (true) {
        int pivot = nums[r], p = l;
        for (int i = l; i < r; i++) if (nums[i] < pivot) swap(nums[p++], nums[i]);
        swap(nums[p], nums[r]);
        if (p == k) return nums[p];
        if (p < k) l = p + 1; else r = p - 1;
    }
}

// Generates all valid next adjacent state strings from the current state.
// Time Complexity: Problem dependent, Space Complexity: O(branching factor).
vector<string> get_neighbors(const string& cur) {
    return {};
}

// Finds shortest path between start and target strings using Bidirectional BFS.
// Time Complexity: O(b^(d/2)), Space Complexity: O(b^(d/2)).
int bidirectional_bfs(const string& start, const string& target) {
    if (start == target) return 0;
    unordered_set<string> forward_set = {start}, backward_set = {target}, visited = {start, target};
    int steps = 0;
    while (!forward_set.empty() && !backward_set.empty()) {
        if (forward_set.size() > backward_set.size()) swap(forward_set, backward_set);
        unordered_set<string> next_set;
        steps++;
        for (const string& cur : forward_set) {
            for (const string& nxt : get_neighbors(cur)) {
                if (backward_set.count(nxt)) return steps;
                if (!visited.count(nxt)) { visited.insert(nxt); next_set.insert(nxt); }
            }
        }
        forward_set = move(next_set);
    }
    return -1;
}

