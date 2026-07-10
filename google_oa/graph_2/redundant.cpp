/**
 * LeetCode 684 - Redundant Connection
 *
 * Description:
 * Given a graph that started as a tree with n nodes but one additional edge added,
 * return the edge that can be removed so that the resulting graph is a tree.
 *
 * Approach:
 * - Use Disjoint Set Union (DSU) / Union-Find.
 * - Process edges one by one.
 * - For each edge `(u, v)`, check if they are already in the same component.
 * - If yes, this edge forms a cycle and is the redundant connection.
 * - If not, union the components of `u` and `v`.
 *
 * Time Complexity: O(N * alpha(N))
 * Space Complexity: O(N)
 */
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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

class DSU {
    vector<int> p, sz;

public:
    DSU(int n) {
        p.resize(n + 1);
        sz.assign(n + 1, 1);
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

class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        DSU dsu(edges.size());

        for (auto &e : edges)
            if (!dsu.unite(e[0], e[1]))
                return e;

        return {};
    }
};