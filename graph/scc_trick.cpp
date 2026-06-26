/**
 * CSES - Component Cycle Trick
 *
 * Description:
 * Given a directed graph, partition it into weakly connected components.
 * Check if components contain cycles to compute the reachable vertices score.
 *
 * Approach:
 * - Find weakly connected components (treating edges as undirected).
 * - Run DFS on each component on the directed graph to detect any cycles.
 * - Sum component sizes: add `size` if it has a cycle, else `size - 1`.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
using namespace std;

void findComponent(int u, int compId, const vector<vector<int>>& undirected, vector<int>& component) {
    component[u] = compId;
    for (int v : undirected[u]) if (component[v] == -1) findComponent(v, compId, undirected, component);
}

void detectCycle(int u, const vector<vector<int>>& directed, vector<int>& state, vector<int>& component, vector<bool>& hasCycle) {
    state[u] = 1; // 1 = visiting
    for (int v : directed[u]) {
        if (state[v] == 0) detectCycle(v, directed, state, component, hasCycle);
        else if (state[v] == 1) hasCycle[component[u]] = true; // Back edge indicates cycle in component
    }
    state[u] = 2; // 2 = visited
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> directed(n + 1), undirected(n + 1);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v; directed[u].push_back(v); undirected[u].push_back(v); undirected[v].push_back(u);
    }
    vector<int> component(n + 1, -1), compSize; int compCount = 0;
    // Find weakly connected components
    for (int i = 1; i <= n; i++) if (component[i] == -1) {
        compSize.push_back(0); findComponent(i, compCount++, undirected, component);
    }
    for (int i = 1; i <= n; i++) compSize[component[i]]++;
    vector<int> vis(n + 1, 0); vector<bool> hasCycle(compCount, false);
    for (int i = 1; i <= n; i++) if (vis[i] == 0) detectCycle(i, directed, vis, component, hasCycle);
    long long answer = 0;
    for (int c = 0; c < compCount; c++) answer += hasCycle[c] ? compSize[c] : compSize[c] - 1;
    cout << answer << '\n';
}