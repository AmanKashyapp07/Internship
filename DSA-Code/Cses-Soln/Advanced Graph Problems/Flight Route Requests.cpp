// Link: https://cses.fi/problemset/task/1699
#include <bits/stdc++.h>
using namespace std;

void findComp(int u, int compId, const vector<vector<int>>& uG, vector<int>& comp) {
    comp[u] = compId;
    for (int v : uG[u]) if (comp[v] == -1) findComp(v, compId, uG, comp);
}

void detectCycle(int u, const vector<vector<int>>& dG, vector<int>& state, const vector<int>& comp, vector<bool>& hasCycle) {
    state[u] = 1;
    for (int v : dG[u]) {
        if (state[v] == 0) detectCycle(v, dG, state, comp, hasCycle);
        else if (state[v] == 1) hasCycle[comp[u]] = true;
    }
    state[u] = 2;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> dG(n + 1), uG(n + 1);
    while (m--) {
        int u, v; cin >> u >> v; dG[u].push_back(v); uG[u].push_back(v); uG[v].push_back(u);
    }

    vector<int> comp(n + 1, -1), compSize;
    int compCount = 0;
    for (int i = 1; i <= n; i++) {
        if (comp[i] == -1) {
            compSize.push_back(0);
            findComp(i, compCount++, uG, comp);
        }
    }
    for (int i = 1; i <= n; i++) compSize[comp[i]]++;

    vector<int> state(n + 1, 0);
    vector<bool> hasCycle(compCount, false);
    for (int i = 1; i <= n; i++) if (state[i] == 0) detectCycle(i, dG, state, comp, hasCycle);

    long long ans = 0;
    for (int c = 0; c < compCount; c++) ans += hasCycle[c] ? compSize[c] : compSize[c] - 1;
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum flight routes required to satisfy all reachability requirements across n cities (CSES 1699).
// - Approach: Weakly Connected Components + DFS Cycle Detection.
// - Intuition: For each weakly connected component of size C: if it contains a directed cycle, it requires C edges; otherwise $C-1$ edges suffice.
// - Complexity: Time: O(V + E), Space: O(V + E).