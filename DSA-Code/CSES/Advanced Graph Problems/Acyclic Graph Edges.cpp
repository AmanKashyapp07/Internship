// Link: https://cses.fi/problemset/task/1756
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    while (m--) {
        int u, v; cin >> u >> v;
        if (u < v) cout << u << ' ' << v << '\n';
        else cout << v << ' ' << u << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Direct all edges in an undirected graph such that the resulting graph is acyclic (CSES 1756).
// - Approach: Index-Based Ordering (`u -> v` if $u < v$).
// - Intuition: Directing every edge from the smaller vertex index to the larger vertex index guarantees a valid topological ordering ($1 < 2 < \dots < N$), which inherently prevents any directed cycle.
// - Complexity: Time: O(M), Space: O(1).
