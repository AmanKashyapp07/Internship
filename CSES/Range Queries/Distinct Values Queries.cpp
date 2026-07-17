// Link: https://cses.fi/problemset/task/1734

#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int idx, int val) {
        for (++idx; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    int query(int idx) {
        int sum = 0;
        for (++idx; idx > 0; idx -= idx & -idx) sum += bit[idx];
        return sum;
    }

    int query(int l, int r) {
        return l > r ? 0 : query(r) - query(l - 1);
    }
};

struct Query { int l, r, id; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<int> a(n);
    for (int &x : a) cin >> x;

    vector<vector<Query>> endingAt(n);
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        endingAt[r - 1].push_back({l - 1, r - 1, i});
    }

    Fenwick bit(n);
    map<int, int> last; // value -> latest index seen
    vector<int> ans(q);

    for (int i = 0; i < n; i++) {
        // If value seen before, remove its previous instance from BIT
        if (last.count(a[i])) bit.update(last[a[i]], -1);
        
        bit.update(i, 1);
        last[a[i]] = i;

        // Answer all queries ending at the current index
        for (const auto &qu : endingAt[i]) {
            ans[qu.id] = bit.query(qu.l, qu.r);
        }
    }

    for (int x : ans) cout << x << '\n';
}