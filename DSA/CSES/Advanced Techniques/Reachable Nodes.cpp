// Link: https://cses.fi/problemset/task/2138

#include <algorithm>
#include <array>
#include <bitset>
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

using namespace std;

// IMPORTANT: Allocate bitset array globally. 
// A local variable inside main() would cause a Stack Overflow because it's ~312 MB.
const int MAXN = 50005;
bitset<MAXN> reachable[MAXN];

signed main()
{
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n);
    vector<int> indegree(n, 0);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        graph[u].push_back(v);
        indegree[v]++;
    }
    
    queue<int> q;
    for (int i = 0; i < n; i++)
    {
        if (indegree[i] == 0)
        {
            q.push(i);
        }
    }

    vector<int> topo_order;
    
    // 1. Finish Kahn's algorithm to generate the Topological Order
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        for (int v : graph[u]) {
            indegree[v]--;
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    } // after this loop, topo_order contains the nodes in topological order, through which we can process the DAG for reachability by iterating in reverse, because we want to ensure that when we process a node, all its children have already been processed and their reachable sets are ready to be merged into the current node's reachable set.

    // 2. Process nodes in REVERSE Topological Order
    // This ensures children are fully calculated before their parents query them.
    for (int i = n - 1; i >= 0; i--)
    {
        int u = topo_order[i]; // current node in reverse topological order
        
        // A node can always reach itself
        reachable[u].set(u);  // Set the bit corresponding to node u to 1, indicating that u can reach itself.
        
        for (int v : graph[u]) {
            // Fast Set Union: Reachable nodes of u include all reachable nodes of v, where v is a child of u in the DAG. This is done using bitwise OR operation on the bitsets, if u is last node in topo order then it will not have any child and hence it will only be able to reach itself, otherwise it will be able to reach all the nodes that its children can reach as well as itself.
            reachable[u] |= reachable[v]; 
        }
    }

    // 3. Output the result
    for (int i = 0; i < n; i++)
    {
        cout << reachable[i].count() << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}