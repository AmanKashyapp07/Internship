// Link: https://cses.fi/problemset/task/1676
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, sz;
    DSU(int n) : parent(n + 1), sz(n + 1, 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    int unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return sz[a];
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a; sz[a] += sz[b];
        return sz[a];
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    DSU dsu(n);
    int components = n, largest = 1;

    while (m--) {
        int u, v; cin >> u >> v;
        if (dsu.find(u) != dsu.find(v)) {
            largest = max(largest, dsu.unite(u, v));
            components--;
        }
        cout << components << ' ' << largest << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process added roads and track total connected components and max component size (CSES 1676).
// - Approach: Disjoint Set Union (DSU) with Path Compression & Union by Size.
// - Intuition: Uniting two distinct components reduces component count by 1 and updates the maximum size in $O(\alpha(N))$ per query.
// - Complexity: Time: O(M \cdot \alpha(N)), Space: O(N).