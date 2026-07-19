// Link: https://cses.fi/problemset/task/1748

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n + 1) {}

    void update(int i, int val) {
        for (++i; i <= n; i += i & -i)
            bit[i] = (bit[i] + val) % MOD;
    }

    int query(int i) {
        int s = 0;
        for (++i; i > 0; i -= i & -i)
            s = (s + bit[i]) % MOD;
        return s;
    }

    int query(int l, int r) {
        return (query(r) - (l > 0 ? query(l - 1) : 0) + MOD) % MOD;
    }
};

struct CoordinateCompressor {
    vector<int> vals;

    void add(int x) {
        vals.push_back(x);
    }

    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }

    int get(int x) {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    }

    int size() {
        return vals.size();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    CoordinateCompressor cc;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        cc.add(a[i]);
    }

    cc.build();

    Fenwick ft(cc.size());

    int ans = 0;

    for (int x : a) {
        int idx = cc.get(x);

        int dp = (1 + (idx > 0 ? ft.query(idx - 1) : 0)) % MOD; // ft.query(idx - 1) gives the count of increasing subsequences ending with elements less than x, basically prefix sum 

        ft.update(idx, dp);

        ans = (ans + dp) % MOD;
    }

    cout << ans << '\n';
}