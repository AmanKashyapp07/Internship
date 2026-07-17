// Link: https://cses.fi/problemset/task/2177

/*
CSES Problem: Strongly Connected Edges

Given an undirected graph, orient every edge so that the resulting
directed graph is strongly connected.

If impossible, print "IMPOSSIBLE".

Idea:
- A bridge makes strong connectivity impossible.
- Run DFS from node 1.
- Orient tree edges from parent -> child.
- Orient back edges from descendant -> ancestor.
- If any bridge exists or graph is disconnected, answer is IMPOSSIBLE.
*/

#include <iostream>
#include <vector>

using namespace std;

int n, m;
vector<vector<pair<int, int>>> adj;
vector<int> tin, low, visited;
vector<pair<int, int>> answer;

int timer = 0;
bool hasBridge = false;

void dfs(int u, int parentEdge)
{
    visited[u] = 1;
    tin[u] = low[u] = ++timer; // Set discovery time and low-link value

    for (auto [v, edgeId] : adj[u])
    {
        if (edgeId == parentEdge)
        {
            continue;
        }

        if (!visited[v]) 
        {
            answer[edgeId] = {u, v}; // Orient tree edge from parent -> child

            dfs(v, edgeId); // Recur for the child node

            low[u] = min(low[u], low[v]); // Update low-link value of u based on child v

            if (low[v] > tin[u]) // If the lowest reachable vertex from v is after u's discovery time, then (u, v) is a bridge
            {
                hasBridge = true;
            }
        }
        else // back edge
        {
            low[u] = min(low[u], tin[v]);

            if (tin[v] < tin[u]) // only orient back edges from descendant -> ancestor
            {
                answer[edgeId] = {u, v};
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    adj.assign(n + 1, {});
    tin.assign(n + 1, 0);
    low.assign(n + 1, 0);
    visited.assign(n + 1, 0);
    answer.resize(m);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;

        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }

    dfs(1, -1);

    for (int i = 1; i <= n; i++)
    {
        if (!visited[i])
        {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    if (hasBridge)
    {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    for (auto [u, v] : answer)
    {
        cout << u << ' ' << v << '\n';
    }

    return 0;
}