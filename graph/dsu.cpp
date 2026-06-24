/**
 * Problem: Road Construction (https://cses.fi/problemset/task/1676)
 * Track the number of components and the size of the largest component as edges are added.
 * Time: O(E * alpha(V)) time, O(V) space.
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

struct DSU {
    vector<int> parent, sz;
    DSU(int n) {
        parent.resize(n + 1); sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]); // Path compression
    }
    int unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return sz[a];
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a; sz[a] += sz[b]; // Union by size
        return sz[a];
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    DSU dsu(n);
    int components = n, largest = 1; // always initialise with components = n and largest = 1 because initially, each node is its own component of size 1
    while (m--) {
        int u, v; cin >> u >> v;
        if (dsu.find(u) != dsu.find(v)) { // only unite if u and v are in different components
            largest = max(largest, dsu.unite(u, v)); // update largest component size after uniting u and v
            components--; // decrease component count by 1 after uniting u and v
        }
        cout << components << ' ' << largest << '\n';
    }
}
