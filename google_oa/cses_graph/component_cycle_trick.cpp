/**
 * CSES - Component Cycle Trick
 *
 * Problem:
 * Given a directed graph, treat it as undirected to form weakly connected
 * components.
 *
 * For each component:
 * - If it contains a directed cycle, contribute its size.
 * - Otherwise, contribute (size - 1).
 *
 * Output the total contribution.
 *
 * Approach:
 * - Find weakly connected components using DFS on the undirected graph.
 * - Detect directed cycles using DFS with a recursion stack.
 * - For each component:
 *      cycle   -> add size
 *      acyclic -> add (size - 1)
 *
 * Time:  O(n + m)
 * Space: O(n + m)
 */

#include <iostream>
#include <vector>
using namespace std;

int n, m;

vector<vector<int>> graph, undirected;
vector<int> comp, sz;
vector<bool> vis, inStack, hasCycle;

void findComponent(int u, int id) {
    comp[u] = id;

    for (int v : undirected[u]) {
        if (comp[v] == -1)
            findComponent(v, id);
    }
}

void findCycle(int u) {
    vis[u] = true;
    inStack[u] = true;

    for (int v : graph[u]) {
        if (!vis[v]) {
            findCycle(v);
        }
        else if (inStack[v]) {
            hasCycle[comp[u]] = true;
        }
    }

    inStack[u] = false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    graph.assign(n + 1, {});
    undirected.assign(n + 1, {});

    while (m--) {
        int u, v;
        cin >> u >> v;

        graph[u].push_back(v);
        undirected[u].push_back(v);
        undirected[v].push_back(u);
    }

    comp.assign(n + 1, -1);

    int components = 0;

    for (int i = 1; i <= n; i++) {
        if (comp[i] == -1) {
            sz.push_back(0);
            findComponent(i, components);
            components++;
        }
    }

    for (int i = 1; i <= n; i++)
        sz[comp[i]]++;

    vis.assign(n + 1, false);
    inStack.assign(n + 1, false);
    hasCycle.assign(components, false);

    for (int i = 1; i <= n; i++) {
        if (!vis[i])
            findCycle(i);
    }

    long long ans = 0;

    for (int i = 0; i < components; i++) {
        if (hasCycle[i])
            ans += sz[i];
        else
            ans += sz[i] - 1;
    }

    cout << ans << '\n';
}