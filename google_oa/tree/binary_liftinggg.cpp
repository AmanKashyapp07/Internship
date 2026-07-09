/**
 * @file CSES_Binary_Lifting_Compilation.cpp
 * @brief Ultimate Monolithic Blueprint for CSES Tree & Lowest Common Ancestor Variations.
 * Core Concept: Doubling pointers up[u][j] = parent at 2^j levels above node u.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// Maximum height bound ($2^{20} > 10^6$) to accommodate large paths
const int LOG = 20;

// =========================================================================
// SECTION 1: COMMON BINARY LIFTING & PATH AGGREGATION UTILITIES
// =========================================================================

vector<vector<int>> adj;
vector<vector<int>> up;
vector<int> depth;
vector<long long> cnt;
vector<long long> ans;

/**
 * @brief Reallocates and flushes the standard global buffers for a size of N nodes.
 */
void init_structures(int n) {
    adj.assign(n + 1, vector<int>());
    up.assign(n + 1, vector<int>(LOG, -1));
    depth.assign(n + 1, 0);
    cnt.assign(n + 1, 0);
    ans.assign(n + 1, 0);
}

/**
 * @brief DFS Precomputation to initialize depth maps and standard binary ancestor tables.
 * Time Complexity: O(N log N) | Space Complexity: O(N log N)
 */
void dfs_precompute(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p; // Direct 2^0 parent

    // Derive ancestor coordinates iteratively
    for (int j = 1; j < LOG; j++) {
        int p = up[u][j - 1];
        if (p != -1) {
            up[u][j] = up[p][j - 1];
        }
        else {
            up[u][j] = -1; // No ancestor exists at this level
        }
    }

    for (int v : adj[u]) {
        if (v != p) {
            dfs_precompute(v, u, d + 1);
        }
    }
}

/**
 * @brief Lifts a target node x up by k total structural levels.
 * Time Complexity: O(log K)
 */
int lift_node(int x, int k) {
    for (int j = 0; j < LOG; j++) {
        if ((k >> j) & 1) { // Examine bit flag representations
            x = up[x][j];
            if (x == -1) return -1;
        }
    }
    return x;
}

/**
 * @brief Extracts the Lowest Common Ancestor (LCA) using binary jumping.
 * Time Complexity: O(log N)
 */
int find_lca(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);

    // Phase 1: Shift both nodes up until depth alignment is matching
    a = lift_node(a, depth[a] - depth[b]);
    if (a == b) return a;

    // Phase 2: Climb up synchronously without crossing thresholds
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }

    // Nodes are parked right below their LCA, return parent node
    return up[a][0];
}

/**
 * @brief Aggregates the cumulative node metrics via a bottom-up subtree sum traversal.
 * Time Complexity: O(N)
 */
void dfs_sum(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) {
            dfs_sum(v, u);
            cnt[u] += cnt[v]; // Propagate child variations upward
        }
    }
    ans[u] = cnt[u];
}

// =========================================================================
// SECTION 2: STANDALONE CSES VARIATION PIEPLINES
// =========================================================================

/**
 * @brief 1. Company Queries I (CSES 1687)
 * Problem Statement: Find the k-th boss (ancestor) of employee x.
 */
void solve_company_queries_1() {
    int n, q;
    if (!(cin >> n >> q)) return;

    init_structures(n);
    for (int i = 2; i <= n; i++) {
        int boss; cin >> boss;
        adj[boss].push_back(i);
        adj[i].push_back(boss);
    }

    dfs_precompute(1, -1, 0); // Root node 1, default parent -1

    while (q--) {
        int x, k;
        cin >> x >> k;
        cout << lift_node(x, k) << "\n";
    }
}

/**
 * @brief 2. Company Queries II (CSES 1688)
 * Problem Statement: Find the LCA of two employees a and b.
 */
void solve_company_queries_2() {
    int n, q;
    if (!(cin >> n >> q)) return;

    init_structures(n);
    for (int i = 2; i <= n; i++) {
        int boss; cin >> boss;
        adj[boss].push_back(i);
        adj[i].push_back(boss);
    }

    dfs_precompute(1, -1, 0);

    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << find_lca(a, b) << "\n";
    }
}

/**
 * @brief 3. Distance Queries (CSES 1135)
 * Problem Statement: Calculate total edges traversing path between node a and node b.
 * Formula: $$Dist(a, b) = Depth[a] + Depth[b] - 2 \cdot Depth[LCA(a, b)]$$
 */
void solve_distance_queries() {
    int n, q;
    if (!(cin >> n >> q)) return;

    init_structures(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs_precompute(1, -1, 0);

    while (q--) {
        int a, b;
        cin >> a >> b;
        int lca = find_lca(a, b);
        cout << (depth[a] + depth[b] - 2 * depth[lca]) << "\n";
    }
}

/**
 * @brief 4. Counting Paths (CSES 1136)
 * Problem Statement: Given M paths, track how many paths cross each node.
 * Technique: Tree Difference Array (Prefix sums over paths)
 */
void solve_counting_paths() {
    int n, m;
    if (!(cin >> n >> m)) return;

    init_structures(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs_precompute(1, -1, 0);

    while (m--) {
        int a, b;
        cin >> a >> b;
        int lca = find_lca(a, b);
        
        // Tree difference array update rules to mark path increments over nodes:
        cnt[a]++;
        cnt[b]++;
        cnt[lca]--; // Negate double counting at intersection vertex
        
        int p = up[lca][0];
        if (p != -1) cnt[p]--; // Negate parent flow bounds
    }

    dfs_sum(1, -1); // Compress difference aggregates via bottom-up summation

    for (int i = 1; i <= n; i++) {
        cout << ans[i] << (i == n ? "" : " ");
    }
    cout << "\n";
}

// =========================================================================
// EXECUTIVE ENVIRONMENT
// =========================================================================
int main() {
    // Fast I/O optimize
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Uncomment the target problem pipeline to run:
    // solve_company_queries_1();
    // solve_company_queries_2();
    // solve_distance_queries();
    // solve_counting_paths();

    return 0;
}