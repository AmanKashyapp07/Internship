/**
 * CSES 1667 - Message Route
 *
 * Approach:
 * - Run BFS from node 1.
 * - Store the parent of each visited node.
 * - If node n is reachable, backtrack using the parent array.
 *
 * Time: O(V + E)
 * Space: O(V + E)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);

    while (m--) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> par(n + 1, -1);

    queue<int> q;
    q.push(1);
    par[1] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if(par[v] == -1) { // otherwise we already have found a shorter path to v
                par[v] = u;
                q.push(v);
            }
        }
    }

    if (par[n] == -1) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    vector<int> path;
    int curr =n;
    while(curr!= 0) {
        path.push_back(curr);
        curr = par[curr];
    }
    reverse(path.begin(), path.end());

    cout << path.size() << '\n';

    for (int u : path)
        cout << u << ' ';

    cout << '\n';
}