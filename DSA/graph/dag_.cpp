/**
 * CSES Problem Set
 *
 * Problem: Reachable Nodes
 * Link: https://cses.fi/problemset/task/2138
 * Category: Graph Algorithms
 *
 * Problem Statement:
 * Given a Directed Acyclic Graph (DAG), determine for every node
 * how many nodes (including itself) are reachable from it.
 *
 * Approach:
 * 1. Perform a Topological Sort using Kahn's Algorithm.
 * 2. Process the nodes in reverse topological order.
 * 3. Maintain a bitset for each node representing all nodes reachable from it.
 * 4. Initially, every node can reach itself.
 * 5. For every outgoing edge u -> v, merge the reachable set of v into u:
 *        reachable[u] |= reachable[v]
 * 6. The number of reachable nodes is simply reachable[u].count().
 *
 * Why Reverse Topological Order?
 * When processing a node, all of its children have already been processed,
 * so their reachable sets are complete and can be merged directly.
 *
 * Time Complexity:
 * O((N * (N / 64)) + (M * (N / 64)))
 * ≈ O((N + M) * N / 64)
 *
 * Space Complexity:
 * O(N² / 8) bits for storing the bitsets.
 *
 * Note:
 * The bitset array is declared globally because its size is roughly
 * 300 MB for the maximum constraints. Declaring it locally would
 * cause a stack overflow.
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50005;
bitset<MAXN> reachable[MAXN];

vector<int> topologicalSort(const vector<vector<int>>& graph,
                            vector<int> indegree)
{
    int n = graph.size();

    queue<int> q;

    for (int i = 0; i < n; i++)
    {
        if (indegree[i] == 0)
            q.push(i);
    }

    vector<int> topo;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        topo.push_back(u);

        for (int v : graph[u])
        {
            indegree[v]--;

            if (indegree[v] == 0)
                q.push(v);
        }
    }

    return topo;
}


//---------------------------------------------------------------
// Computes reachable nodes using DP on Reverse Topological Order
//---------------------------------------------------------------
void computeReachability(const vector<vector<int>>& graph,
                         const vector<int>& topo)
{
    int n = graph.size();

    for (int i = n - 1; i >= 0; i--)
    {
        int u = topo[i];

        // Every node can reach itself
        reachable[u].set(u);

        // Merge children's reachable sets
        for (int v : graph[u])
        {
            reachable[u] |= reachable[v];
        }
    }
}


//---------------------------------------------------------------
// Prints answer
//---------------------------------------------------------------
void printAnswer(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << reachable[i].count();

        if (i + 1 != n)
            cout << " ";
    }

    cout << "\n";
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n);
    vector<int> indegree(n);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        u--;
        v--;

        graph[u].push_back(v);
        indegree[v]++;
    }

    vector<int> topo = topologicalSort(graph, indegree);

    computeReachability(graph, topo);

    printAnswer(n);

    return 0;
}