// CSES Problem: Tree Distances II
// https://cses.fi/problemset/task/1133
//
// You are given a tree consisting of n nodes.
// For each node, determine the sum of distances from that node
// to all other nodes.
//
// Input:
// n
// followed by n-1 edges
//
// Output:
// n integers where the i-th integer is the sum of distances
// from node i to all other nodes.
//
// Approach:
// 1. Root the tree at node 1.
// 2. First DFS:
//    - Compute subtree sizes.
//    - Compute answer for node 1 (sum of depths).
// 3. Second DFS (rerooting):
//    - If moving root from u to child v:
//      ans[v] = ans[u] + n - 2 * subtreeSize[v]
// 4. Complexity: O(n)

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int n;
vector<vector<int>> adj;
vector<ll> subtreeSize;
vector<ll> ans;

void dfs1(int node, int parent, int depth)
{
    ans[1] += depth;
    subtreeSize[node] = 1;

    for (int child : adj[node])
    {
        if (child == parent)
            continue;

        dfs1(child, node, depth + 1);
        subtreeSize[node] += subtreeSize[child];
    }
}

void dfs2(int node, int parent)
{
    for (int child : adj[node])
    {
        if (child == parent)
            continue;

        ans[child] = ans[node] + n - 2 * subtreeSize[child];
        dfs2(child, node);
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    adj.resize(n + 1);
    subtreeSize.resize(n + 1);
    ans.resize(n + 1, 0);

    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs1(1, 0, 0);
    dfs2(1, 0);

    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << " ";
    }
    cout << '\n';

    return 0;
}