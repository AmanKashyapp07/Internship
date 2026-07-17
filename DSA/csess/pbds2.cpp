#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct CoordinateCompressor {
    vector<int> vals;
    void add(int x) { vals.push_back(x); }

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

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n + 1) {}

    void update(int i, int val) {
        i++;
        while (i <= n) {
            bit[i] += val;
            i += i & -i;
        }
    }

    int query(int i) {
        int s = 0;
        i++;
        while (i > 0) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }

    int query(int l, int r) {
        if (l > r) return 0;
        return query(r) - (l ? query(l - 1) : 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    CoordinateCompressor cc;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        cc.add(a[i]);
    }

    cc.build();

    vector<int> id(n);
    for (int i = 0; i < n; i++)
        id[i] = cc.get(a[i]);

    Fenwick bit(cc.size());

    ll inv = 0;

    // Build first window
    for (int i = 0; i < k; i++) {
        // previous elements greater than current
        inv += bit.query(id[i] + 1, cc.size() - 1);
        bit.update(id[i], 1);
    }

    cout << inv;

    for (int i = k; i < n; i++) {

        int out = i - k;

        // Remove contribution of outgoing element
        bit.update(id[out], -1);
        inv -= bit.query(0, id[out] - 1);

        // Add contribution of incoming element
        inv += bit.query(id[i] + 1, cc.size() - 1);
        bit.update(id[i], 1);

        cout << " " << inv;
    }

    cout << '\n';
}