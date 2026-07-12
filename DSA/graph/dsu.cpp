/**
 * CSES 1676 - Road Construction
 *
 * Description:
 * Track the number of connected components and the size of the largest component
 * as edges are added dynamically to the graph.
 *
 * Approach:
 * - Use the Disjoint Set Union (DSU) data structure.
 * - Implement path compression and union-by-size.
 * - Initialize n components of size 1.
 * - For each added edge, merge sets, decrement the component count if they were disjoint, and update the maximum size.
 *
 * Time Complexity: O(E * alpha(V)) where alpha is the inverse Ackermann function.
 * Space Complexity: O(V)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    int comp;
    DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) { iota(p.begin(), p.end(), 0); }

    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }

    bool unite(int a, int b) {
        if ((a = find(a)) == (b = find(b))) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    int size(int x) { return sz[find(x)]; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DSU dsu(n);
    int mx = 1;

    while (m--) {
        int a, b;
        cin >> a >> b;
        dsu.unite(a, b);
        mx = max(mx, dsu.size(a));
        cout << dsu.comp << " " << mx << "\n";
    }

    return 0;
}