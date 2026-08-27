// Link: https://cses.fi/problemset/task/1734
#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n; vector<int> b;
    BIT(int n) : n(n), b(n + 1, 0) {}
    void upd(int i, int v) { for (++i; i <= n; i += i & -i) b[i] += v; }
    int qry(int l, int r) {
        int s = 0;
        for (int i = r + 1; i > 0; i -= i & -i) s += b[i];
        if (l) for (int i = l; i > 0; i -= i & -i) s -= b[i];
        return s;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;

    // Offline: group queries by right endpoint
    struct Q { int l, r, id; };
    vector<vector<Q>> endAt(n);
    for (int i = 0; i < q; i++) {
        int l, r; cin >> l >> r;
        endAt[r-1].push_back({l-1, r-1, i});
    }

    BIT bit(n);
    unordered_map<int,int> last;
    vector<int> ans(q);
    for (int i = 0; i < n; i++) {
        if (last.count(a[i])) bit.upd(last[a[i]], -1);
        bit.upd(i, 1);
        last[a[i]] = i;
        for (auto& [l, r, id] : endAt[i])
            ans[id] = bit.qry(l, r);
    }
    for (int x : ans) cout << x << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each query [l, r], count the number of distinct values in the subarray (CSES 1734).
// - Approach: Offline BIT + Last-Occurrence tracking (sweep right endpoint).
// - Intuition: For each element, remove its previous occurrence from BIT and mark the current; sweep left-to-right answering all queries ending at current index; counts distinct elements in [l, r].
// - Complexity: Time: O((N + Q) log N), Space: O(N + Q).