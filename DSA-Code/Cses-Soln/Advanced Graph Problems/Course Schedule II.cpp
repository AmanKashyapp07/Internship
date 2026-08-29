// Link: https://cses.fi/problemset/task/1757
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> revGraph(n + 1);
    vector<int> inDegree(n + 1, 0);

    while (m--) {
        int u, v; cin >> u >> v;
        revGraph[v].push_back(u); // Reverse edges
        inDegree[u]++;
    }

    priority_queue<int> pq; // Max-heap
    for (int i = 1; i <= n; i++) if (inDegree[i] == 0) pq.push(i);

    vector<int> order;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        order.push_back(u);
        for (int v : revGraph[u]) {
            if (--inDegree[v] == 0) pq.push(v);
        }
    }
    reverse(order.begin(), order.end());

    for (int i = 0; i < n; i++) cout << order[i] << (i + 1 == n ? '\n' : ' ');
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find lexicographically smallest topological sort prioritizing small course numbers as early as possible (CSES 1757).
// - Approach: Reverse Graph + Max-Heap Reverse Topological Sort.
// - Intuition: Finding the lexicographically largest topological sort on the reversed graph and reversing the final order yields the optimal schedule.
// - Complexity: Time: O((V + E) \log V), Space: O(V + E).