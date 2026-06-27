/**
 * @file CSES_Tree_Algorithms.cpp
 * @brief Ultimate CSES Tree Algorithms Solutions Template (Flattened Functional Style)
 * Removes class wrappers to eliminate allocation overhead and improve execution speed.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <climits>
#include <memory>

using namespace std;

// =========================================================================
// 2. TREE MATCHING (CSES 1130)
// =========================================================================
namespace TreeMatching {
    vector<vector<int>> adj;
    vector<bool> matched;
    int max_matching_count = 0;

    // Greedily match leaves up to root via bottom-up DFS traversal
    void dfs(int u, int p) {
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u);
                // If neither current node nor its child is matched, pair them up
                if (!matched[u] && !matched[v]) {
                    matched[u] = true;
                    matched[v] = true;
                    max_matching_count++;
                }
            }
        }
    }

    void run() {
        int n;
        if (!(cin >> n)) return;

        adj.assign(n + 1, vector<int>());
        matched.assign(n + 1, false);
        max_matching_count = 0;

        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        dfs(1, 0);
        cout << max_matching_count << "\n";
    }
}

// =========================================================================
// 3. TREE DIAMETER (CSES 1131)
// =========================================================================
namespace TreeDiameter {

    vector<vector<int>> adj;

    // ---------------- Method 1: Tree DP ----------------

    int diameterDP = 0;

    int dfs(int u, int p) {
        int mx1 = 0, mx2 = 0;

        for (int v : adj[u]) {
            if (v == p) continue;

            int depth = dfs(v, u);

            if (depth >= mx1) {
                mx2 = mx1;
                mx1 = depth;
            } else if (depth > mx2) {
                mx2 = depth;
            }
        }

        diameterDP = max(diameterDP, mx1 + mx2);
        return mx1 + 1;
    }

    // ---------------- Method 2: Two DFS ----------------

    int farthestNode;
    int maxDist;

    void dfs2(int u, int p, int dist) {
        if (dist > maxDist) {
            maxDist = dist;
            farthestNode = u;
        }

        for (int v : adj[u]) {
            if (v != p)
                dfs2(v, u, dist + 1);
        }
    }

    void run() {
        int n;
        cin >> n;

        adj.assign(n + 1, {});

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Method 1: Tree DP
        diameterDP = 0;
        dfs(1, 0);

        // Method 2: Two DFS
        maxDist = -1;
        dfs2(1, 0, 0);

        int start = farthestNode;

        maxDist = -1;
        dfs2(start, 0, 0);

        int diameterTwoDFS = maxDist;

        cout << "DP Diameter      : " << diameterDP << '\n';
        cout << "Two DFS Diameter : " << diameterTwoDFS << '\n';
    }
}
// =========================================================================
// 4. TREE DISTANCES I (CSES 1132)
// =========================================================================
namespace TreeDistancesI {

    vector<vector<int>> adj;
    int farthestNode, maxDist;

    void dfs(int u, int p, int dist, vector<int> &d) {
        d[u] = dist;

        if (dist > maxDist) {
            maxDist = dist;
            farthestNode = u;
        }

        for (int v : adj[u]) {
            if (v != p)
                dfs(v, u, dist + 1, d);
        }
    }

    void run() {
        int n;
        cin >> n;

        adj.assign(n + 1, {});

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // First DFS: find one endpoint of the diameter
        vector<int> dist1(n + 1);

        maxDist = -1;
        dfs(1, 0, 0, dist1);

        int nodeA = farthestNode;

        // Second DFS: distances from nodeA and find the other endpoint
        vector<int> distA(n + 1);

        maxDist = -1;
        dfs(nodeA, 0, 0, distA);

        int nodeB = farthestNode;

        // Third DFS: distances from nodeB
        vector<int> distB(n + 1);

        maxDist = -1;
        dfs(nodeB, 0, 0, distB);

        for (int i = 1; i <= n; i++) {
            cout << max(distA[i], distB[i]) << " ";
        }
        cout << '\n';
    }
}

// =========================================================================
// 10. DISTINCT COLORS (CSES 1139)
// =========================================================================
namespace DistinctColors {

    vector<vector<int>> adj;
    vector<int> color, ans;
    vector<set<int>> st;

    void dfs(int u, int p) {
        st[u].insert(color[u]);

        for (int v : adj[u]) {
            if (v == p) continue;

            dfs(v, u);

            // Small-to-large merging
            if (st[u].size() < st[v].size())
                swap(st[u], st[v]);

            for (int x : st[v])
                st[u].insert(x);
        }

        ans[u] = st[u].size();
    }

    void run() {
        int n;
        cin >> n;

        adj.assign(n + 1, {});
        color.resize(n + 1);
        ans.resize(n + 1);
        st.assign(n + 1, {});

        for (int i = 1; i <= n; i++)
            cin >> color[i];

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        dfs(1, 0);

        for (int i = 1; i <= n; i++)
            cout << ans[i] << " ";
        cout << '\n';
    }
}
