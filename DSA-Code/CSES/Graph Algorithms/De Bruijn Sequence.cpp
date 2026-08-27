// Link: https://cses.fi/problemset/task/1692
#include <bits/stdc++.h>
using namespace std;

struct Hierholzer {
    int n; vector<vector<int>> g; vector<int> path;
    Hierholzer(int n) : n(n), g(n) {}
    void dfs(int u) {
        while (!g[u].empty()) {
            int v = g[u].back(); g[u].pop_back(); dfs(v);
        }
        path.push_back(u);
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    if (n == 1) { cout << "01\n"; return 0; }

    int V = 1 << (n - 1), mask = V - 1;
    Hierholzer h(V);
    for (int u = 0; u < V; u++) {
        h.g[u].push_back((u << 1) & mask);
        h.g[u].push_back(((u << 1) & mask) | 1);
    }

    h.dfs(0);
    reverse(h.path.begin(), h.path.end());

    string ans(n - 1, '0');
    for (size_t i = 1; i < h.path.size(); i++) ans += char('0' + (h.path[i] & 1));
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Construct a shortest binary sequence containing all $2^n$ binary strings of length n as substrings (CSES 1692).
// - Approach: Eulerian Circuit on De Bruijn Graph via Hierholzer's Algorithm.
// - Intuition: Nodes are $(n-1)$-bit states; directed edges represent appending 0 or 1. Eulerian circuit visits every edge (n-bit string) once.
// - Complexity: Time: O(2^N), Space: O(2^N).