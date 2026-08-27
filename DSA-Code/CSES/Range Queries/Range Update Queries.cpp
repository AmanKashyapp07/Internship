// Link: https://cses.fi/problemset/task/1651
#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n; vector<long long> b;
    BIT(int n) : n(n), b(n + 2, 0) {}
    void upd(int i, long long v) { for (++i; i <= n; i += i & -i) b[i] += v; }
    long long qry(int i) { long long s = 0; for (++i; i > 0; i -= i & -i) s += b[i]; return s; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    BIT bit(n);
    // Build difference array in BIT
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        bit.upd(i, a[i] - (i > 0 ? a[i-1] : 0));
    }
    while (q--) {
        int t; cin >> t;
        if (t == 1) { int l, r, u; cin >> l >> r >> u; l--; r--;
                      bit.upd(l, u); if (r + 1 < n) bit.upd(r + 1, -u); }
        else        { int k; cin >> k; cout << bit.qry(k - 1) << '\n'; }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Support range add updates and point queries (CSES 1651).
// - Approach: Fenwick Tree on Difference Array — store diffs in BIT; range add = two point updates; point query = prefix sum.
// - Intuition: If `d[i] = a[i] - a[i-1]`, then `a[k] = Σ d[0..k]`; range add `[l,r]+u` ⟹ `d[l]+=u`, `d[r+1]-=u`.
// - Complexity: Time: O((N + Q) log N), Space: O(N).
