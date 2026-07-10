/**
 * CSES - Functional Graph Cycle Detection
 *
 * Description:
 * Given a functional graph where every node has exactly one outgoing edge,
 * find any cycle and print its vertices.
 *
 * Approach:
 * - Run DFS from every unvisited node.
 * - Maintain:
 *      vis[i]     = node has been visited.
 *      inStack[i] = node is currently in the DFS recursion stack.
 * - If DFS reaches a node already in the recursion stack,
 *   a cycle is found.
 * - Reconstruct the cycle by following outgoing edges until
 *   the starting node repeats.
 *
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> nxt;
vector<bool> vis, inStack;
int start = -1;

bool dfs(int u)
{
    vis[u] = true;
    inStack[u] = true;

    int v = nxt[u];
    if(!vis[v]) if(dfs(v)) return true;
    else if (inStack[v])
    {
        start = v;
        return true;
    }

    inStack[u] = false;
    return false;
}

vector<int> buildCycle(int start)
{
    vector<int> cycle;
    int cur = start;
    do
    {
        cycle.push_back(cur);
        cur = nxt[cur];
    }
    while (cur != start);

    return cycle;
}

vector<int> findCycle(int n)
{
    for (int i = 1; i <= n; i++)
    {
        if (!vis[i] && dfs(i))
        {
            return buildCycle(start);
        }
    }

    return {};
}   

int main()
{
    int n;
    cin >> n;

    nxt.resize(n + 1);
    vis.assign(n + 1, false);
    inStack.assign(n + 1, false);

    for (int i = 1; i <= n; i++)
        cin >> nxt[i];

    vector<int> cycle = findCycle(n);

    for (int x : cycle)
        cout << x << " ";
}