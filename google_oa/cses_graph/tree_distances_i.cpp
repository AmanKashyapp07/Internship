#include <iostream>
#include <vector>
using namespace std;

void dfs(int node, int parent, int depth,
         const vector<vector<int>> &adj,
         vector<int> &dist) {

    dist[node] = depth;

    for (int next : adj[node]) {
        if (next == parent)
            continue;

        dfs(next, node, depth + 1, adj, dist);
    }
}

// Returns {farthest_node, distance_array}
pair<int, vector<int>> getDistances(int start,
                                    const vector<vector<int>> &adj) {

    int n = adj.size();
    vector<int> dist(n, -1);

    dfs(start, -1, 0, adj, dist);

    int farthest = start;

    for (int i = 0; i < n; i++) {
        if (dist[i] > dist[farthest])
            farthest = i;
    }

    return {farthest, dist};
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        u--;
        v--;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Find one endpoint of the diameter.
    auto [end1, temp] = getDistances(0, adj);

    // Find the other endpoint and distances from end1.
    auto [end2, dist1] = getDistances(end1, adj);

    // Distances from the second endpoint.
    auto [dummy, dist2] = getDistances(end2, adj);

    // Answer for each node.
    for (int i = 0; i < n; i++)
        cout << max(dist1[i], dist2[i]) << " ";

    cout << '\n';
}