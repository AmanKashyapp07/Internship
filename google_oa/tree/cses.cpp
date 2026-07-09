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

using namespace std;

// =========================================================================
// 2. TREE MATCHING (CSES 1130)
// =========================================================================
namespace TreeMatching {
    vector<vector<int>> adj;
    vector<bool> matched;
    int max_matching_count = 0;

    /**
     * @brief Bottom-up DFS traversal to greedily pair adjacent unmatched vertices.
     * @note Leaves can never be a optimal parent matching choice, so we prioritize 
     * matching nodes from the bottom up.
     */
    void dfs(int u, int p) {
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u); // Run bottom-up first
                
                // If neither node u nor its child v has been taken yet, match them
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
    int diameterDP = 0;
    int farthestNode, maxDist;

    // ---------------- METHOD 1: TREE DP ----------------
    
    /**
     * @brief Computes maximum subtree depths to calculate diameter in a single pass.
     * @return Max depth extending downward from node u.
     */
    int dfsDP(int u, int p) {
        int mx1 = 0, mx2 = 0; // Tracks the top 2 deepest child branches

        for (int v : adj[u]) {
            if (v == p) continue;

            int depth = dfsDP(v, u);

            // Update the top two longest branches downward
            if (depth >= mx1) {
                mx2 = mx1;
                mx1 = depth;
            } else if (depth > mx2) {
                mx2 = depth;
            }
        }

        // The longest path passing through u as the highest peak node
        diameterDP = max(diameterDP, mx1 + mx2);
        return mx1 + 1;
    }

    // ---------------- METHOD 2: TWO DFS ----------------
    
    /**
     * @brief Simple path tracer to track the absolute furthest vertex distance.
     */
    void dfs2(int u, int p, int dist) {
        if (dist > maxDist) {
            maxDist = dist;
            farthestNode = u;
        }

        for (int v : adj[u]) {
            if (v != p) {
                dfs2(v, u, dist + 1);
            }
        }
    }

    void run() {
        int n;
        if (!(cin >> n)) return;
        if (n <= 1) {
            cout << 0 << "\n";
            return;
        }

        adj.assign(n + 1, {});

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Method 1 Execution: Tree Dynamic Programming
        diameterDP = 0;
        dfsDP(1, 0);

        // Method 2 Execution: Two-Pass Distant DFS Scaling
        // Pass A: Find one definitive endpoint of the tree diameter
        maxDist = -1;
        dfs2(1, 0, 0);
        int startNode = farthestNode;

        // Pass B: From that endpoint, measure distance to the other furthest side
        maxDist = -1;
        dfs2(startNode, 0, 0);
        int diameterTwoDFS = maxDist;

        // Output results natively based on preference
        cout << diameterTwoDFS << "\n";
    }
}

// =========================================================================
// 4. TREE DISTANCES I (CSES 1132)
// =========================================================================
namespace TreeDistancesI {
    vector<vector<int>> adj;
    int farthestNode, maxDist;

    /**
     * @brief Utility scan that registers distances from a root node into an array map.
     */
    void dfs(int u, int p, int dist, vector<int> &d) {
        d[u] = dist;

        if (dist > maxDist) {
            maxDist = dist;
            farthestNode = u;
        }

        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, dist + 1, d);
            }
        }
    }

    void run() {
        int n;
        if (!(cin >> n)) return;

        adj.assign(n + 1, {});

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<int> dist1(n + 1, 0);
        vector<int> distA(n + 1, 0);
        vector<int> distB(n + 1, 0);

        // Step 1: Execute arbitrary search from Node 1 to capture Diameter Endpoint A
        maxDist = -1;
        dfs(1, 0, 0, dist1);
        int nodeA = farthestNode;

        // Step 2: Run a precise search from NodeA to map all distances and locate Endpoint B
        maxDist = -1;
        dfs(nodeA, 0, 0, distA);
        int nodeB = farthestNode;

        // Step 3: Run a precise search from NodeB to map opposite distances across diameter boundary
        maxDist = -1;
        dfs(nodeB, 0, 0, distB);

        // KEY INSIGHT: The absolute maximum distance from any arbitrary node to another node
        // inside a tree structure must always terminate at one of the two diameter endpoints.
        for (int i = 1; i <= n; i++) {
            cout << max(distA[i], distB[i]) << (i == n ? "" : " ");
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
    // Optimized: Using unordered_set ensures O(1) average hash lookups instead of O(log N) trees
    vector<unordered_set<int>> st;

    /**
     * @brief Aggregates sets bottom-up using the Small-to-Large merging technique.
     * Time Complexity: O(N log N) average execution runtime
     */
    void dfs(int u, int p) {
        st[u].insert(color[u]); // Seed node's internal native color properties

        for (int v : adj[u]) {
            if (v == p) continue;

            dfs(v, u); // Process children subtrees completely first

            // Optimization Rule: Swap set buffers if child container outgrows parent container.
            // This guarantees each node's element is re-inserted at most O(log N) times.
            if (st[u].size() < st[v].size()) {
                swap(st[u], st[v]);
            }

            // Merge items out of the smaller child bucket into the dominant parent bucket
            for (int x : st[v]) {
                st[u].insert(x);
            }
            
            // Memory Optimization: Flush child records early once merged to drop peak memory footprint
            st[v].clear();
        }

        ans[u] = st[u].size(); // Set tracking capacity yields accurate unique colors metric
    }

    void run() {
        int n;
        if (!(cin >> n)) return;

        adj.assign(n + 1, {});
        color.resize(n + 1);
        ans.resize(n + 1);
        st.assign(n + 1, {});

        for (int i = 1; i <= n; i++) cin >> color[i];

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        dfs(1, 0);

        for (int i = 1; i <= n; i++) {
            cout << ans[i] << (i == n ? "" : " ");
        }
        cout << '\n';
    }
}

// =========================================================================
// EXECUTIVE ENVIRONMENT
// =========================================================================
int main() {
    // Speed optimization profiles for standard C++ I/O pipelines
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Uncomment target procedure block pipeline execution:
    // TreeMatching::run();
    // TreeDiameter::run();
    // TreeDistancesI::run();
    // DistinctColors::run();

    return 0;
}