// Link: https://cses.fi/problemset/task/1686

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
#define int long long
using namespace std;

vector<vector<int>> g, rg;
vector<bool> vis;
vector<int> comp, order;

void dfs1(int u)
{
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs1(v);
    order.push_back(u);
}

void dfs2(int u, int id)
{
    comp[u] = id;
    for (int v : rg[u]) if (!comp[v]) dfs2(v, id);
}

int dfs(int u, vector<long long>& weight, vector<vector<int>>& dag, vector<long long>& dp)
{
    if (dp[u] != -1) return dp[u];
    dp[u] = weight[u];
    for (int v : dag[u])
    {
        dfs(v, weight, dag, dp);
        dp[u] = max(dp[u], weight[u] + dp[v]);
    }
    return dp[u];
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<long long> coins(n + 1);

    for (int i = 1; i <= n; i++)
        cin >> coins[i];

    g.resize(n + 1);
    rg.resize(n + 1);

    while (m--)
    {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        rg[b].push_back(a);
    }

    vis.assign(n + 1, false);

    for (int i = 1; i <= n; i++)
        if (!vis[i])
            dfs1(i);

    comp.assign(n + 1, 0);

    reverse(order.begin(), order.end());

    int scc = 0;

    for (int u : order)
    {
        if (!comp[u])
            dfs2(u, ++scc);
    } // this will fill the comp vector with the component id for each node, and scc will be the total number of strongly connected components

    vector<long long> weight(scc + 1);

    for (int i = 1; i <= n; i++)  weight[comp[i]] += coins[i]; // this will give us the total weight of each strongly connected component, which is the sum of coins in that component

    vector<vector<int>> dag(scc + 1); // this will be the directed acyclic graph of strongly connected components, where each node is a strongly connected component and there is an edge from component A to component B if there is an edge from any node in A to any node in B in the original graph

    for (int u = 1; u <= n; u++)
    {
        for (int v : g[u])
        {
            if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]); // this will add an edge from component of u to component of v in the DAG if they are different components
        }
    }

    vector<long long> dp(scc + 1, -1);

    int ans = 0;
    for (int i = 1; i <= scc; i++)
    {
        ans = max(ans, dfs(i, weight, dag, dp)); // this will find the maximum weight path in the DAG of strongly connected components
    } // we are starting from each component and finding the maximum weight path starting from that component, and taking the maximum of all such paths, because the maximum weight path can start from any component, it's not only to start from the first component, because the graph is not necessarily connected, and there can be multiple components, so we need to consider all components as starting points for the maximum weight path
    cout << ans << '\n';
}