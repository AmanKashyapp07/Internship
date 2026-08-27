// Link: https://cses.fi/problemset/task/1748
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, int val) { for (i++; i <= n; i += i & -i) bit[i] = (bit[i] + val) % MOD; }
    int query(int i) { int s = 0; for (i++; i > 0; i -= i & -i) s = (s + bit[i]) % MOD; return s; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n), vals;
    for (int i = 0; i < n; i++) { cin >> a[i]; vals.push_back(a[i]); }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto getRank = [&](int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); };

    Fenwick ft(vals.size());
    long long ans = 0;
    for (int x : a) {
        int idx = getRank(x);
        long long dp = (1 + (idx > 0 ? ft.query(idx - 1) : 0)) % MOD;
        ft.add(idx, dp);
        ans = (ans + dp) % MOD;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count total increasing subsequences modulo 10^9+7 (CSES 1748).
// - Approach: Coordinate Compression + Fenwick Tree DP ($O(N \log N)$).
// - Intuition: `dp[x] = 1 + query(rank(x) - 1)` counts all valid increasing subsequences ending at element x in logarithmic time.
// - Complexity: Time: O(N \log N), Space: O(N).