#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    int unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) return sz[a];

        if (sz[a] < sz[b]) swap(a, b);

        parent[b] = a;
        sz[a] += sz[b];

        return sz[a];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DSU dsu(n);

    int components = n;
    int largest = 1;

    while (m--) {
        int a, b;
        cin >> a >> b;

        if (dsu.find(a) != dsu.find(b)) {
            largest = max(largest, dsu.unite(a, b));
            components--;
        }

        cout << components << ' ' << largest << '\n';
    }
}