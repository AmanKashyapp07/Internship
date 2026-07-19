// Link: https://cses.fi/problemset/task/1701

#include <bits/stdc++.h>
using namespace std;

// Shared global map and ID counter for tracking subtree structures
map<vector<int>, int> global_mp;
int global_nxt = 1;

// Returns the 1 or 2 centers of the tree using the leaf-peeling method
vector<int> find_centers(int n, const vector<vector<int>>& g) {
    if (n == 1) return {1};
    vector<int> deg(n + 1), leaves;
    
    // 1. Gather initial leaves
    for (int i = 1; i <= n; i++) {
        if ((deg[i] = g[i].size()) == 1) leaves.push_back(i);
    }

    // 2. Peel leaves layer by layer until 1 or 2 nodes remain
    while (n > 2) {
        n -= leaves.size();
        vector<int> next_leaves;
        
        for (int u : leaves) {
            for (int v : g[u]) {
                if (--deg[v] == 1) next_leaves.push_back(v);
            }
        }
        leaves = next_leaves; // Move inward to the next layer
    }
    
    return leaves;
}

struct TreeIsomorphism {
    int n;
    const vector<vector<int>>& g;
    vector<int> id;
    map<vector<int>, int>& mp; 
    int& nxt;

    // Connects internal references to the shared global tracking variables
    TreeIsomorphism(int nodes, const vector<vector<int>>& graph)
        : n(nodes), g(graph), id(nodes + 1), mp(global_mp), nxt(global_nxt) {}

    int dfs(int u, int p) {
        vector<int> child_ids;
        for (int v : g[u]) {
            if (v == p) continue;
            child_ids.push_back(dfs(v, u));
        }

        // Sort child IDs to ensure structural uniqueness
        sort(child_ids.begin(), child_ids.end());

        if (!mp.count(child_ids)) {
            mp[child_ids] = nxt++;
        }

        return id[u] = mp[child_ids];
    }

    int getID(int root) {
        return dfs(root, 0);
    } 
};

void solve() {
    int n; 
    cin >> n;
    
    // CRITICAL: Clear the shared dictionary at the start of every test case
    global_mp.clear();
    global_nxt = 1;

    vector<vector<int>> g1(n + 1), g2(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        g1[u].push_back(v); g1[v].push_back(u);
    }
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        g2[u].push_back(v); g2[v].push_back(u);
    }

    vector<int> centers1 = find_centers(n, g1);
    vector<int> centers2 = find_centers(n, g2);
    
    // Instantiate exactly ONCE outside the loops to protect the global map's state
    TreeIsomorphism T1(n, g1);
    TreeIsomorphism T2(n, g2);

    bool isIsomorphic = false;
    for (int c1 : centers1) {
        for (int c2 : centers2) {
            if (T1.getID(c1) == T2.getID(c2)) {
                isIsomorphic = true;
                break;
            }
        }
        if (isIsomorphic) break;
    }
    
    cout << (isIsomorphic ? "YES\n" : "NO\n");
}

int main() {
    // Fast I/O optimized for competitive programming
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; 
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}