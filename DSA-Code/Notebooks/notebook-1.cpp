#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const ll P = 31;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Tree Centroid                               | Subtree Size DFS + Heavy Descent  | O(N)     | O(N)     |
 | 2  | Longest Paths & Path Counts on a DAG        | Topological Sort + DP Relaxation  | O(V + E) | O(V + E) |
 | 3  | Tree Rerooting DP (All-Nodes Distance Sum)  | 2-Pass Tree Rerooting DP          | O(N)     | O(N)     |
 | 4  | Tree Diameter & Node Eccentricities         | 2-Pass Tree BFS/DFS               | O(N)     | O(N)     |
 | 5  | Functional Graph Decomposition & Queries    | Floyd's Cycle + Binary Lifting    | O(N logN)| O(N logN)|
 | 6  | Tree Isomorphism (Double-Hashing)           | Canonical Subtree Hashing (AHU)   | O(N logN)| O(N)     |
 | 7  | Tree Centers by Leaf Trimming               | Topological Leaf Peeling (Deg 1)  | O(N)     | O(N)     |
 ====================================================================================================
*/

// =========================================================
// 1. TREE CENTROID
// =========================================================

struct Centroid {
    int n; vvi g; vi sz;
    Centroid(int n, const vvi &g) : n(n), g(g), sz(n + 1, 0) {}
    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    int dfs(int u, int p) {
        sz[u] = 1;
        for (int v : g[u]) if (v != p) sz[u] += dfs(v, u);
        return sz[u];
    }
    int centroid(int u, int p, int total) {
        for (int v : g[u]) if (v != p && sz[v] > total / 2) return centroid(v, u, total);
        return u;
    }
    int get() { dfs(1, 0); return centroid(1, 0, sz[1]); }
};
// Interview Explanation:
// - Problem Statement: Find a centroid node of a tree whose removal splits the tree into components each of size at most N / 2.
// - Approach: Subtree size computation via DFS + greedy descent to heavy child.
// - Intuition: At every node, if any child subtree has size > N / 2, the centroid must lie in that child's branch; otherwise current node is centroid.
// - Complexity: Time: O(N) two linear traversals, Space: O(N).


// =========================================================
// 2. LONGEST PATHS & PATH COUNTS ON A DAG
// =========================================================

pair<vi, vi> dag_(int n, const vector<vector<pii>> &g, int src, int start = 1) {
    vi in_deg(g.size(), 0);
    for (int u = start; u < start + n; u++) for (auto &[v, w] : g[u]) in_deg[v]++;
    queue<int> q;
    for (int u = start; u < start + n; u++) if (in_deg[u] == 0) q.push(u);
    vi order;
    while (!q.empty()) {
        int u = q.front(); q.pop(); order.push_back(u);
        for (auto &[v, w] : g[u]) if (--in_deg[v] == 0) q.push(v);
    }
    const int INF = 1e9; vi dist(g.size(), -INF), paths(g.size(), 0);
    dist[src] = 0; paths[src] = 1;
    for (int u : order) {
        if (dist[u] == -INF) continue;
        for (auto &[v, w] : g[u]) {
            if (dist[v] < dist[u] + w) { dist[v] = dist[u] + w; paths[v] = paths[u]; }
            else if (dist[v] == dist[u] + w) paths[v] = (paths[v] + paths[u]) % MOD;
        }
    }
    return {dist, paths};
}
// Interview Explanation:
// - Problem Statement: Find longest path distance and distinct longest paths count from a source in a weighted DAG.
// - Approach: Topological Sort + Dynamic Programming edge relaxation.
// - Intuition: Relaxing edges in topological order guarantees optimal path distances and multiplicity without revisiting.
// - Complexity: Time: O(V + E), Space: O(V + E).


// =========================================================
// 3. TREE REROOTING DP (ALL-NODES DISTANCE SUM)
// =========================================================

struct TreeDistances {
    int n; vvi g; vi sz, dist;
    TreeDistances(int n, const vvi &g) : n(n), g(g), sz(n + 1, 0), dist(n + 1, 0) {}
    void dfs(int u, int p, int d) {
        sz[u] = 1; dist[1] += d;
        for (int v : g[u]) if (v != p) { dfs(v, u, d + 1); sz[u] += sz[v]; }
    }
    void reroot(int u, int p) {
        for (int v : g[u]) if (v != p) { dist[v] = dist[u] - sz[v] + (n - sz[v]); reroot(v, u); }
    }
    vi solve(int root = 1) { dfs(root, 0, 0); reroot(root, 0); return dist; }
};
// Interview Explanation:
// - Problem Statement: Compute sum of distances from every node to all other nodes in a tree (All-Nodes Distance Sum).
// - Approach: Two-pass Tree Rerooting Dynamic Programming.
// - Intuition: First DFS calculates sum from root; rerooting DFS updates in O(1): dist[v] = dist[u] - sz[v] + (N - sz[v]).
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 4. TREE DIAMETER & NODE ECCENTRICITIES
// =========================================================

void dfsDist(int u, int p, const vvi &g, vi &dist) {
    for (int v : g[u]) if (v != p) { dist[v] = dist[u] + 1; dfsDist(v, u, g, dist); }
}

pair<int, vi> treeDiameter(int n, const vvi &g) {
    vi dist1(n + 1, 0), dist2(n + 1, 0);
    dfsDist(1, 0, g, dist1);
    int a = 1; for (int i = 1; i <= n; i++) if (dist1[i] > dist1[a]) a = i;
    fill(dist1.begin(), dist1.end(), 0); dfsDist(a, 0, g, dist1);
    int b = 1; for (int i = 1; i <= n; i++) if (dist1[i] > dist1[b]) b = i;
    dfsDist(b, 0, g, dist2);
    vi eccentricity(n + 1);
    for (int i = 1; i <= n; i++) eccentricity[i] = max(dist1[i], dist2[i]);
    return {dist1[b], eccentricity};
}
// Interview Explanation:
// - Problem Statement: Find tree diameter and eccentricity of all vertices in a tree.
// - Approach: Two-pass / Three-pass BFS/DFS tree diameter algorithm.
// - Intuition: Farthest node from node 1 is diameter endpoint a; farthest from a is endpoint b; distance a-b is diameter.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 5. FUNCTIONAL GRAPH DECOMPOSITION & QUERIES
// =========================================================

vi getCycleFloyd(const vi &to, const vi &comp, int start) {
    int slow = start, fast = start;
    while (true) {
        slow = to[slow]; fast = to[to[fast]];
        if (comp[slow] != -1) return {};
        if (slow == fast) break;
    }
    slow = start;
    while (slow != fast) { slow = to[slow]; fast = to[fast]; }
    vi cycle; int curr = slow;
    do { cycle.push_back(curr); curr = to[curr]; } while (curr != slow);
    return cycle;
}

struct FunctionalGraph {
    int n, LOG = 20; vi dist, cyc, comp, pos; vvi up, rv;
    void dfs(int u) {
        for (int v : rv[u]) if (dist[v] == -1) {
            dist[v] = dist[u] + 1; cyc[v] = cyc[u]; comp[v] = comp[u]; dfs(v);
        }
    }
    FunctionalGraph(const vi &to) : n(to.size()), dist(n, -1), cyc(n, -1), comp(n, -1), pos(n, -1), up(n, vi(LOG)), rv(n) {
        for (int i = 0; i < n; i++) up[i][0] = to[i];
        for (int j = 1; j < LOG; j++) for (int i = 0; i < n; i++) up[i][j] = up[up[i][j - 1]][j - 1];
        for (int i = 0; i < n; i++) rv[to[i]].push_back(i);
        int cid = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] != -1) continue;
            vi cycle = getCycleFloyd(to, comp, i);
            if (cycle.empty()) continue;
            int len = cycle.size();
            for (int j = 0; j < len; j++) {
                int u = cycle[j]; dist[u] = 0; cyc[u] = len; comp[u] = cid; pos[u] = j;
            }
            for (int u : cycle) dfs(u);
            cid++;
        }
    }
    int jump(int u, int k) {
        for (int j = 0; j < LOG; j++) if (k >> j & 1) u = up[u][j];
        return u;
    }
    int distanceInCycle(int a, int b) {
        if (comp[a] != comp[b] || dist[a] || dist[b]) return -1;
        return (pos[b] - pos[a] + cyc[a]) % cyc[a];
    }
    int query(int a, int b) {
        if (comp[a] != comp[b]) return -1;
        bool ac = !dist[a], bc = !dist[b];
        if (ac && bc) return distanceInCycle(a, b);
        if (ac) return -1;
        if (!bc) return dist[a] >= dist[b] && jump(a, dist[a] - dist[b]) == b ? dist[a] - dist[b] : -1;
        return dist[a] + distanceInCycle(jump(a, dist[a]), b);
    }
};
// Interview Explanation:
// - Problem Statement: Decompose functional graph into trees on cycles, answering k-step jumps and path reachability.
// - Approach: Floyd's Cycle Detection + Binary Lifting + Component cycle offset indexing.
// - Intuition: Every component has one directed cycle with trees feeding in; binary lifting and modulo arithmetic resolve paths.
// - Complexity: Time: O(N \log N) build, O(\log N) query, Space: O(N \log N).


// =========================================================
// 6. TREE ISOMORPHISM (DOUBLE-HASHING)
// =========================================================

const ll MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, BASE1 = 313, BASE2 = 317;
const int MAX_N = 200005;
ll pow1[MAX_N], pow2[MAX_N];

void precomputeTreeHash() {
    pow1[0] = pow2[0] = 1;
    for (int i = 1; i < MAX_N; i++) {
        pow1[i] = (pow1[i - 1] * BASE1) % MOD1; pow2[i] = (pow2[i - 1] * BASE2) % MOD2;
    }
}

vl dfsTreeHash(int u, int p, const vvi &g) {
    vvl children;
    for (int v : g[u]) if (v != p) children.push_back(dfsTreeHash(v, u, g));
    sort(children.begin(), children.end());
    ll h1 = 1, h2 = 1; int total_len = 1;
    for (auto &child : children) {
        h1 = (h1 * pow1[child[2]] + child[0]) % MOD1;
        h2 = (h2 * pow2[child[2]] + child[1]) % MOD2;
        total_len += child[2];
    }
    return {(h1 * BASE1 + 2) % MOD1, (h2 * BASE2 + 2) % MOD2, (ll)total_len + 1};
}
// Interview Explanation:
// - Problem Statement: Check if two unrooted trees are isomorphic.
// - Approach: AHU / Double-Hashing tree isomorphism algorithm.
// - Intuition: Canonical sorted tuple of children hashes produces unique structural hash independent of labels.
// - Complexity: Time: O(N \log N), Space: O(N).


// =========================================================
// 7. TREE CENTERS BY LEAF TRIMMING
// =========================================================

vi centers(int n, const vvi &g) {
    if (n == 1) return {1};
    vi deg(n + 1), leaves;
    for (int i = 1; i <= n; i++) { deg[i] = g[i].size(); if (deg[i] == 1) leaves.push_back(i); }
    while (n > 2) {
        n -= leaves.size(); vi nxt;
        for (int u : leaves) for (int v : g[u]) if (--deg[v] == 1) nxt.push_back(v);
        leaves = nxt;
    }
    return leaves;
}
// Interview Explanation:
// - Problem Statement: Find center(s) of a tree (nodes minimizing max distance to any leaf).
// - Approach: Topological Leaf Trimming (peeling degree-1 leaves layer-by-layer).
// - Intuition: Iteratively prune degree-1 leaves until 1 or 2 nodes remain; these are tree centers.
// - Complexity: Time: O(N), Space: O(N).
