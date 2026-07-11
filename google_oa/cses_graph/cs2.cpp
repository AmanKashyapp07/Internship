/**
 * CSES 1757 - Course Schedule II
 *
 * Problem:
 * Find a topological ordering of the courses.
 * If multiple valid orderings exist, output the lexicographically smallest one.
 * If impossible, print "IMPOSSIBLE".
 *
 * Approach:
 * - Reverse every edge.
 * - Perform Kahn's algorithm on the reversed graph using a max-heap.
 * - Reverse the resulting order.
 *
 * Time:  O((V + E) log V)
 * Space: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1);

    while (m--) {
        int u, v;
        cin >> u >> v;

        adj[v].push_back(u);   // reverse edge
        indegree[u]++;
    }

    priority_queue<int> pq;

    for (int i = 1; i <= n; i++)
        if (indegree[i] == 0)
            pq.push(i);

    vector<int> order;

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        order.push_back(u);
        for (int v : adj[u]) if(--indegree[v] == 0) pq.push(v);
    }

    if (order.size() != n) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    reverse(order.begin(), order.end());

    for (int x : order)
        cout << x << " ";
}