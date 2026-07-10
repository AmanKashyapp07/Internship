/**
 * LeetCode 210 - Course Schedule II
 *
 * Description:
 * Given n courses and their prerequisites, return the ordering of courses to take.
 * If there are multiple answers, return any of them. If impossible, return an empty array.
 *
 * Approach:
 * - Topological Sort using Kahn's algorithm or DFS.
 * - Store the result in a topological order list.
 *
 * Time Complexity: O(V + E)
 * Space Complexity: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m; cin >> n >> m;
    vector<vector<int>> adj(n + 1); 
    vector<int> indegree(n + 1, 0);
    for (int i = 0, u, v; i < m; i++) { cin >> u >> v; adj[v].push_back(u); indegree[u]++; } // Reverse edge insertion

    priority_queue<int> pq; // Max heap for reverse topological sort ordering

    for (int i = 1; i <= n; i++) if (indegree[i] == 0) pq.push(i);

    vector<int> order;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        order.push_back(u);
        for (int v : adj[u]) if (--indegree[v] == 0) pq.push(v);
    }

    if ((int)order.size() < n) { cout << "IMPOSSIBLE\n"; return 0; }

    reverse(order.begin(), order.end()); // Reversing the order gives the correct sorting

    for (int x : order) cout << x << ' ';
    
    cout << '\n';
}