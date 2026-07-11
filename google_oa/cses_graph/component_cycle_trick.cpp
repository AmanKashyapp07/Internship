/**
 * CSES - Component Cycle Trick
 *
 * Problem Statement:
 * You are given a directed graph with n vertices and m edges.
 * Treat the graph as undirected to divide it into weakly connected components.
 * For each component:
 *   - If it contains a directed cycle, contribute its full size.
 *   - Otherwise, contribute (size - 1).
 * Output the sum of these values over all components.
 *
 * Approach:
 * - Find weakly connected components using DFS on the undirected graph.
 * - Run DFS on the directed graph to detect cycles using recursion states.
 * - Compute the answer based on whether each component contains a cycle.
 *
 * Time Complexity: O(n + m)
 * Space Complexity: O(n + m)
 */

#include <iostream>
#include <vector>

using namespace std;

void dfsComp(int u, int id, vector<vector<int>> &ug, vector<int> &comp) {
    comp[u] = id;
    for (int v : ug[u]) if (comp[v] == -1) dfsComp(v, id, ug, comp);
}

void dfsCycle(int u, vector<vector<int>> &g, vector<bool> &vis,vector<bool> &inStack, vector<int> &comp,vector<bool> &cycle) {
    vis[u] = true;
    inStack[u] = true;

    for (int v : g[u]) {
        if (!vis[v])
            dfsCycle(v, g, vis, inStack, comp, cycle);
        else if (inStack[v])
            cycle[comp[u]] = true;
    }

    inStack[u] = false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n + 1), ug(n + 1);

    while (m--) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        ug[u].push_back(v);
        ug[v].push_back(u);
    }

    vector<int> comp(n + 1, -1), sz; // comp[i] = component id of vertex i, sz[id] = size of component id , component here means weakly connected component, as we are treating the graph as undirected to find weakly connected components, so we are using undirected graph to find weakly connected components, and then we are using directed graph to find cycles in each component, so we are using directed graph to find cycles in each component, and then we are using the size of each component to calculate the answer
    int cnt = 0;

    for (int i = 1; i <= n; i++) {
        if (comp[i] != -1)
            continue;
        sz.push_back(0);
        dfsComp(i, cnt, ug, comp);
        cnt++;
    }

    for (int i = 1; i <= n; i++) sz[comp[i]]++;

    vector<bool> vis(n + 1, false);
    vector<bool> inStack(n + 1, false);
    vector<bool> cycle(cnt, false);

    for (int i = 1; i <= n; i++)if (!vis[i]) dfsCycle(i, g, vis, inStack, comp, cycle);
    long long ans = 0;
    for (int i = 0; i < cnt; i++)
        ans += cycle[i] ? sz[i] : sz[i] - 1;

    cout << ans << '\n';
}