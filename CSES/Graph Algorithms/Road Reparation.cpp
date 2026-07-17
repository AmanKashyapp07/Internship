#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

struct DSU {
    vector<int> p, sz;

    DSU(int n) : p(n + 1), sz(n + 1, 1) {
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) return false;

        if (sz[a] < sz[b]) swap(a, b);

        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

struct Edge {
    int u, v;
    long long w;

    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);

    for (auto &[u, v, w] : edges)
        cin >> u >> v >> w;

    sort(edges.begin(), edges.end()); // sorting edges by weight in non-decreasing order, by using operator< in Edge struct

    DSU dsu(n);
    long long cost = 0;
    int used = 0;

    for (auto &[u, v, w] : edges) {
        if (dsu.unite(u, v)) {
            cost += w;
            used++;
        }
    }

    if (used != n - 1)
        cout << "IMPOSSIBLE\n";
    else
        cout << cost << '\n';
}