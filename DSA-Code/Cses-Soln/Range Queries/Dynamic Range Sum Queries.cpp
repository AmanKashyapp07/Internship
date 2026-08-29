// Link: https://cses.fi/problemset/task/1648
#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n; vector<long long> b;
    BIT(int n) : n(n), b(n + 1, 0) {}
    void upd(int i, long long v) { for (++i; i <= n; i += i & -i) b[i] += v; }
    long long qry(int i) { long long s = 0; for (++i; i > 0; i -= i & -i) s += b[i]; return s; }
    long long qry(int l, int r) { return qry(r) - (l ? qry(l - 1) : 0); }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    BIT bit(n);
    for (int i = 0; i < n; i++) bit.upd(i, a[i]);
    while (q--) {
        int t; cin >> t;
        if (t == 1) { int k, u; cin >> k >> u; bit.upd(k - 1, u - a[k-1]); a[k-1] = u; }
        else        { int l, r; cin >> l >> r; cout << bit.qry(l-1, r-1) << '\n'; }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Support point updates and range sum queries on an array (CSES 1648).
// - Approach: Fenwick Tree (Binary Indexed Tree) for O(log N) update + prefix sum.
// - Intuition: BIT stores cumulative sums using lowest-set-bit trick; update propagates up, prefix query walks down in O(log N).
// - Complexity: Time: O((N + Q) log N), Space: O(N).
