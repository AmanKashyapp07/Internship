/**
 * CSES 1704 - Network Renovation
 *
 * Description:
 * Add the minimum number of edges to a tree to make it 2-edge-connected (bridge-free).
 *
 * Approach:
 * - Find all leaves in the tree.
 * - Pair the leaves appropriately: pair leaf `i` with leaf `i + L/2` where `L` is the total number of leaves.
 * - Adding edges between these pairs ensures that all tree edges are covered by at least one cycle.
 *
 * Time Complexity: O(V)
 * Space Complexity: O(V)
 */

#include <iostream>
#include <vector>
using namespace std;

void dfs(int u, int p, const vector<vector<int>>& adj, vector<int>& leaves) {
    if (adj[u].size() == 1) leaves.push_back(u); // Leaf detected
    for (int v : adj[u]) if (v != p) dfs(v, u, adj, leaves);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    if (n == 1) { cout << 0 << "\n"; return 0; }
    vector<vector<int>> adj(n + 1);
    for (int i = 0, u, v; i < n - 1; i++) { cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    vector<int> leaves;
    dfs(1, -1, adj, leaves);
    int L = leaves.size(), k = (L + 1) / 2;
    cout << k << "\n"; // logic: to make the tree 2-edge-connected, we need to pair up leaves. If there are L leaves, we can pair them up in L/2 pairs. If L is odd, one leaf will remain unpaired, so we need (L + 1) / 2 edges to connect all leaves.
    // Pair leaf i with leaf (i + L/2) % L to make the tree 2-edge-connected
    
    for (int i = 0; i < k; i++) {
        cout << leaves[i] << " " << leaves[(i + L / 2) % L] << "\n";
    }
}