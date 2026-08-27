#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const ll P = 31;

// =========================================================
// 1. TREE CENTROID
// =========================================================

struct Centroid {
    int n;
    vvi g;
    vi sz;

    Centroid(int n, const vvi &g) : n(n), g(g), sz(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int dfs(int u, int p) {
        sz[u] = 1;
        for (int v : g[u]) {
            if (v != p) sz[u] += dfs(v, u);
        }
        return sz[u];
    }

    int centroid(int u, int p, int total) {
        for (int v : g[u]) {
            if (v != p && sz[v] > total / 2) return centroid(v, u, total);
        }
        return u;
    }

    int get() {
        dfs(1, 0);
        return centroid(1, 0, sz[1]);
    }
};
// Interview Explanation:
// - Problem Statement: Find a centroid node of a tree whose removal splits the tree into components each of size at most N / 2.
// - Approach: Subtree size computation via DFS + greedy descent to heavy child.
// - Intuition: At every node, if any child subtree has size > N / 2, the centroid must lie in that child's branch; otherwise the current node is the centroid.
// - Complexity: Time: O(N) two linear tree traversals, Space: O(N) for tree adjacency and subtree size array.


// =========================================================
// 2. POLYNOMIAL ROLLING HASH OF A STRING
// =========================================================

ll hashWord(const string &s) {
    ll h = 0;
    for (char c : s) {
        h = (h * P + (c - 'a' + 1)) % MOD;
    }
    return h;
}
// Interview Explanation:
// - Problem Statement: Compute a 64-bit polynomial rolling hash of a string modulo 1e9 + 7.
// - Approach: Horner's polynomial evaluation with base prime P = 31.
// - Intuition: Interprets string as a base-P number; accumulating characters from left to right converts strings to compact hash integers for O(1) equality comparisons.
// - Complexity: Time: O(|S|) single pass through characters, Space: O(1) auxiliary space.


// =========================================================
// 3. SUBSTRING POLYNOMIAL ROLLING HASH
// =========================================================

struct FastHash {
    vl p, h;

    FastHash(const string &s) {
        int n = s.size();
        p.assign(n + 1, 1);
        h.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            p[i + 1] = p[i] * P % MOD;
            h[i + 1] = (h[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    ll get(int l, int r) {
        return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD;
    }
};
// Interview Explanation:
// - Problem Statement: Precompute polynomial rolling hashes to query the hash of any substring s[l...r] in O(1) time.
// - Approach: Prefix hash array + base power array precomputation.
// - Intuition: hash(s[l...r]) = (h[r+1] - h[l] * P^(r - l + 1)) % MOD, shifting out prefix hash and aligning powers via subtraction and modular arithmetic.
// - Complexity: Time: O(N) build, O(1) per query, Space: O(N) for powers and prefix hash arrays.


// =========================================================
// 4. DISJOINT SET UNION (DSU / UNION-FIND)
// =========================================================

struct DSU {
    vi p, sz;
    int comp;

    DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) {
        iota(p.begin(), p.end(), 0);
    }

    int find(int u) {
        return p[u] == u ? u : p[u] = find(p[u]);
    }

    bool unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        p[v] = u;
        sz[u] += sz[v];
        comp--;
        return true;
    }

    int size(int u) {
        return sz[find(u)];
    }
};
// Interview Explanation:
// - Problem Statement: Maintain disjoint sets supporting near-constant time component union and representative find queries.
// - Approach: Disjoint Set Union (Union-Find) with Path Compression and Union by Size.
// - Intuition: Path compression flattens tree depth to root on find; union by size attaches smaller trees beneath larger ones, bounding tree depth.
// - Complexity: Time: O(alpha(N)) ~ O(1) amortized per operation, Space: O(N) for parent and size vectors.


// =========================================================
// 5. KRUSKAL'S MINIMUM SPANNING TREE (MST)
// =========================================================

struct Edge {
    int u, v, w;
    bool operator<(const Edge &other) const {
        return w < other.w;
    }
};

int kruskal(int n, vector<Edge> &edges, vector<Edge> &mst) {
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    int total_w = 0;
    mst.clear();

    for (auto &e : edges) {
        if (dsu.unite(e.u, e.v)) {
            total_w += e.w;
            mst.push_back(e);
        }
    }
    return total_w;
}
// Interview Explanation:
// - Problem Statement: Find the Minimum Spanning Tree (MST) weight and edge set in an edge-weighted undirected graph.
// - Approach: Greedy edge selection via sorting + DSU cycle prevention.
// - Intuition: Sort all edges by weight; iteratively connect endpoints using DSU if they reside in different components until N - 1 edges are included.
// - Complexity: Time: O(E \log E) edge sorting, Space: O(V + E) for DSU and MST edge storage.


// =========================================================
// 6. PRIM'S MINIMUM SPANNING TREE (MST)
// =========================================================

int prim(int n, const vector<vector<pii>> &g) {
    vector<bool> vis(n + 1, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, 1});
    int total_w = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        total_w += w;

        for (auto &[v, wt] : g[u]) {
            if (!vis[v]) pq.push({wt, v});
        }
    }
    return total_w;
}
// Interview Explanation:
// - Problem Statement: Find the Minimum Spanning Tree (MST) total weight using Prim's algorithm.
// - Approach: Greedy vertex expansion using a Min-Heap Priority Queue.
// - Intuition: Start with node 1; iteratively grow the MST cut by extracting the minimum-weight boundary edge connecting an unvisited vertex to the growing tree.
// - Complexity: Time: O(E \log V) priority queue operations, Space: O(V + E) for visited array and adjacency list.


// =========================================================
// 7. KAHN'S TOPOLOGICAL SORT
// =========================================================

vi kahn(int n, const vvi &g, int start = 1) {
    vi in_deg(g.size(), 0);
    for (int u = start; u < start + n; u++) {
        for (int v : g[u]) in_deg[v]++;
    }

    queue<int> q;
    for (int u = start; u < start + n; u++) {
        if (in_deg[u] == 0) q.push(u);
    }

    vi order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);

        for (int v : g[u]) {
            if (--in_deg[v] == 0) q.push(v);
        }
    }
    return (int)order.size() == n ? order : vi{};
}
// Interview Explanation:
// - Problem Statement: Compute a valid topological ordering of a directed acyclic graph (DAG), or detect cycles.
// - Approach: Kahn's Algorithm using in-degrees and a FIFO Queue.
// - Intuition: Vertices with in-degree 0 have all dependencies satisfied; process them and decrement neighbor in-degrees, queuing neighbors that become 0.
// - Complexity: Time: O(V + E) linear queue traversal, Space: O(V + E) for in-degree array and queue.


// =========================================================
// 8. LONGEST PATHS & PATH COUNTS ON A DAG
// =========================================================

pair<vi, vi> dag_(int n, const vector<vector<pii>> &g, int src, int start = 1) {
    vi in_deg(g.size(), 0);
    for (int u = start; u < start + n; u++) {
        for (auto &[v, w] : g[u]) in_deg[v]++;
    }

    queue<int> q;
    for (int u = start; u < start + n; u++) {
        if (in_deg[u] == 0) q.push(u);
    }

    vi order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);

        for (auto &[v, w] : g[u]) {
            if (--in_deg[v] == 0) q.push(v);
        }
    }

    const int INF = 1e9;
    vi dist(g.size(), -INF), paths(g.size(), 0);
    dist[src] = 0;
    paths[src] = 1;

    for (int u : order) {
        if (dist[u] == -INF) continue;
        for (auto &[v, w] : g[u]) {
            if (dist[v] < dist[u] + w) {
                dist[v] = dist[u] + w;
                paths[v] = paths[u];
            } else if (dist[v] == dist[u] + w) {
                paths[v] = (paths[v] + paths[u]) % MOD;
            }
        }
    }
    return {dist, paths};
}
// Interview Explanation:
// - Problem Statement: Find the longest path distance and number of distinct longest paths from a source in a weighted DAG.
// - Approach: Topological Sort + Dynamic Programming edge relaxation.
// - Intuition: Relaxing edges in topological order guarantees optimal path distances and path multiplicity counts without revisiting nodes.
// - Complexity: Time: O(V + E) linear DAG DP, Space: O(V + E) for distance, paths, and topological order arrays.


// =========================================================
// 9. TREE REROOTING DP (ALL-NODES DISTANCE SUM)
// =========================================================

struct TreeDistances {
    int n;
    vvi g;
    vi sz, dist;

    TreeDistances(int n, const vvi &g) : n(n), g(g), sz(n + 1, 0), dist(n + 1, 0) {}

    void dfs(int u, int p, int d) {
        sz[u] = 1;
        dist[1] += d;
        for (int v : g[u]) {
            if (v != p) {
                dfs(v, u, d + 1);
                sz[u] += sz[v];
            }
        }
    }

    void reroot(int u, int p) {
        for (int v : g[u]) {
            if (v != p) {
                dist[v] = dist[u] - sz[v] + (n - sz[v]);
                reroot(v, u);
            }
        }
    }

    vi solve(int root = 1) {
        dfs(root, 0, 0);
        reroot(root, 0);
        return dist;
    }
};
// Interview Explanation:
// - Problem Statement: Compute the sum of distances from every node to all other nodes in a tree (All-Nodes Distance Sum).
// - Approach: Two-pass Tree Rerooting Dynamic Programming.
// - Intuition: First DFS calculates subtree sizes and sum of distances from root; rerooting DFS updates child distance in O(1): dist[v] = dist[u] - sz[v] + (N - sz[v]).
// - Complexity: Time: O(N) two DFS traversals, Space: O(N) for tree adjacency and distance vectors.


// =========================================================
// 10. TREE DIAMETER & NODE ECCENTRICITIES
// =========================================================

void dfsDist(int u, int p, const vvi &g, vi &dist) {
    for (int v : g[u]) {
        if (v == p) continue;
        dist[v] = dist[u] + 1;
        dfsDist(v, u, g, dist);
    }
}

pair<int, vi> treeDiameter(int n, const vvi &g) {
    vi dist1(n + 1, 0), dist2(n + 1, 0);

    dfsDist(1, 0, g, dist1);
    int a = 1;
    for (int i = 1; i <= n; i++) {
        if (dist1[i] > dist1[a]) a = i;
    }

    fill(dist1.begin(), dist1.end(), 0);
    dfsDist(a, 0, g, dist1);
    int b = 1;
    for (int i = 1; i <= n; i++) {
        if (dist1[i] > dist1[b]) b = i;
    }

    int diameter = dist1[b];
    dfsDist(b, 0, g, dist2);

    vi eccentricity(n + 1);
    for (int i = 1; i <= n; i++) {
        eccentricity[i] = max(dist1[i], dist2[i]);
    }
    return {diameter, eccentricity};
}
// Interview Explanation:
// - Problem Statement: Find the diameter (longest path between any two nodes) and eccentricity of all vertices in a tree.
// - Approach: Two-pass (or Three-pass) BFS/DFS tree diameter algorithm.
// - Intuition: The farthest node from an arbitrary node 1 is an endpoint a of the diameter; the farthest node from a is the other endpoint b; distance from a to b is the diameter.
// - Complexity: Time: O(N) linear tree traversals, Space: O(N) for distance arrays.


// =========================================================
// 11. STRONGLY CONNECTED COMPONENTS (KOSARAJU'S ALGORITHM)
// =========================================================

struct SCC {
    int n;
    vvi g, rg, sccs, dag;
    vi comp, order;
    vector<bool> vis;

    SCC(int n) : n(n), g(n), rg(n), comp(n, -1), vis(n, false) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        rg[v].push_back(u);
    }

    void dfs1(int u) {
        vis[u] = true;
        for (int v : g[u]) {
            if (!vis[v]) dfs1(v);
        }
        order.push_back(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        sccs.back().push_back(u);
        for (int v : rg[u]) {
            if (comp[v] == -1) dfs2(v, c);
        }
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (!vis[i]) dfs1(i);
        }
        for (int i = n - 1; i >= 0; i--) {
            int u = order[i];
            if (comp[u] == -1) {
                sccs.emplace_back();
                dfs2(u, (int)sccs.size() - 1);
            }
        }
    }

    void buildDag() {
        dag.resize(sccs.size());
        for (int u = 0; u < n; u++) {
            for (int v : g[u]) {
                if (comp[u] != comp[v]) {
                    dag[comp[u]].push_back(comp[v]);
                }
            }
        }
        for (auto &v : dag) {
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
        }
    }
};
// Interview Explanation:
// - Problem Statement: Decompose a directed graph into Strongly Connected Components (SCCs) and construct its condensed DAG.
// - Approach: Kosaraju's Algorithm (Forward DFS + Reverse Graph DFS).
// - Intuition: Post-order finishing times on original graph order vertices such that DFS on reversed graph isolates sink components one by one into SCCs.
// - Complexity: Time: O(V + E) two DFS passes, Space: O(V + E) for graph, reverse graph, and component mappings.


// =========================================================
// 12. FUNCTIONAL GRAPH DECOMPOSITION & QUERIES
// =========================================================

vi getCycleFloyd(const vi &to, const vi &comp, int start) {
    int slow = start, fast = start;
    while (true) {
        slow = to[slow];
        fast = to[to[fast]];
        if (comp[slow] != -1) return {};
        if (slow == fast) break;
    }

    slow = start;
    while (slow != fast) {
        slow = to[slow];
        fast = to[fast];
    }

    vi cycle;
    int curr = slow;
    do {
        cycle.push_back(curr);
        curr = to[curr];
    } while (curr != slow);
    return cycle;
}

struct FunctionalGraph {
    int n, LOG = 20;
    vi dist, cyc, comp, pos;
    vvi up, rv;

    void dfs(int u) {
        for (int v : rv[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                cyc[v] = cyc[u];
                comp[v] = comp[u];
                dfs(v);
            }
        }
    }

    FunctionalGraph(const vi &to) : n(to.size()), dist(n, -1), cyc(n, -1), comp(n, -1), pos(n, -1), up(n, vi(LOG)), rv(n) {
        for (int i = 0; i < n; i++) up[i][0] = to[i];
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) up[i][j] = up[up[i][j - 1]][j - 1];
        }

        for (int i = 0; i < n; i++) rv[to[i]].push_back(i);

        int cid = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] != -1) continue;
            vi cycle = getCycleFloyd(to, comp, i);
            if (cycle.empty()) continue;

            int len = cycle.size();
            for (int j = 0; j < len; j++) {
                int u = cycle[j];
                dist[u] = 0;
                cyc[u] = len;
                comp[u] = cid;
                pos[u] = j;
            }
            for (int u : cycle) dfs(u);
            cid++;
        }
    }

    int jump(int u, int k) {
        for (int j = 0; j < LOG; j++) {
            if (k >> j & 1) u = up[u][j];
        }
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
// - Problem Statement: Decompose a functional graph (where every vertex has out-degree 1) into trees rooted on directed cycles, answering k-step jumps and path reachability.
// - Approach: Floyd's Cycle Detection + Binary Lifting table + Component cycle offset indexing.
// - Intuition: Every component contains exactly one directed cycle with directed trees feeding into it; binary lifting navigates tree branches, and modular arithmetic resolves cycle jumps.
// - Complexity: Time: O(N \log N) build, O(\log N) per jump query, Space: O(N \log N) table storage.


// =========================================================
// 13. TREE ISOMORPHISM (DOUBLE-HASHING)
// =========================================================

const ll MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, BASE1 = 313, BASE2 = 317;
const int MAX_N = 200005;
ll pow1[MAX_N], pow2[MAX_N];

void precomputeTreeHash() {
    pow1[0] = pow2[0] = 1;
    for (int i = 1; i < MAX_N; i++) {
        pow1[i] = (pow1[i - 1] * BASE1) % MOD1;
        pow2[i] = (pow2[i - 1] * BASE2) % MOD2;
    }
}

vl dfsTreeHash(int u, int p, const vvi &g) {
    vvl children;
    for (int v : g[u]) {
        if (v != p) children.push_back(dfsTreeHash(v, u, g));
    }
    sort(children.begin(), children.end());

    ll h1 = 1, h2 = 1;
    int total_len = 1;

    for (auto &child : children) {
        ll ch1 = child[0], ch2 = child[1];
        int len = child[2];
        h1 = (h1 * pow1[len] + ch1) % MOD1;
        h2 = (h2 * pow2[len] + ch2) % MOD2;
        total_len += len;
    }

    h1 = (h1 * BASE1 + 2) % MOD1;
    h2 = (h2 * BASE2 + 2) % MOD2;
    total_len++;
    return {h1, h2, total_len};
}
// Interview Explanation:
// - Problem Statement: Check if two unrooted trees are isomorphic (structurally identical under renaming of vertices).
// - Approach: AHU / Double-Hashing tree isomorphism algorithm.
// - Intuition: Represent each subtree as a canonical sorted tuple of its children's hashes; identical subtree structures produce identical polynomial hashes independent of node labels.
// - Complexity: Time: O(N \log N) sorting children hashes, Space: O(N) for tree representation and hash powers.


// =========================================================
// 14. TREE CENTERS BY LEAF TRIMMING
// =========================================================

vi centers(int n, const vvi &g) {
    if (n == 1) return {1};
    vi deg(n + 1), leaves;

    for (int i = 1; i <= n; i++) {
        deg[i] = g[i].size();
        if (deg[i] == 1) leaves.push_back(i);
    }

    while (n > 2) {
        n -= leaves.size();
        vi nxt;
        for (int u : leaves) {
            for (int v : g[u]) {
                if (--deg[v] == 1) nxt.push_back(v);
            }
        }
        leaves = nxt;
    }
    return leaves;
}
// Interview Explanation:
// - Problem Statement: Find the center(s) of a tree (nodes minimizing maximum distance to any leaf).
// - Approach: Topological Leaf Trimming (peeling leaves layer-by-layer).
// - Intuition: Iteratively prune all degree-1 leaves until 1 or 2 nodes remain; these surviving nodes constitute the tree's topological center(s).
// - Complexity: Time: O(N) each vertex and edge removed once, Space: O(N) for degree array and leaves queue.

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: ADVANCED TREES & GRAPHS
 ====================================================================================================

 1. PATTERN IDENTIFICATION MATRIX:
    | Problem Type / Clue                         | Technique / Data Structure          | Core Template / Invariant             |
    |:--------------------------------------------|:------------------------------------|:--------------------------------------|
    | Divide & Conquer on Trees / Path queries    | Centroid Decomposition              | Subtree size <= N/2                   |
    | Sum of Distances / All Roots DP             | Tree Rerooting (2 passes)           | dist[v] = dist[u] - sz[v] + (N-sz[v]) |
    | Farthest Node / Max Distance in Tree        | Tree Diameter (2 DFS passes)        | Farthest from arbitrary node is endpt |
    | Center of Tree / Minimum Height Tree Root   | Leaf Trimming (Degree 1 peeling)   | Prune leaves until 1 or 2 nodes remain|
    | Tree Isomorphism (Are two trees same shape?)| Tree Hashing (Canonical sort)      | Sort child hashes + double hashing    |
    | Successor Graph / Next pointer (outdeg = 1) | Functional Graph (Cycles + Trees)   | Binary Lifting up[u][k] + Floyd cycle |
    | Directed Cycles / Condensed Graph           | Kosaraju's SCC (Forward + Reverse)  | Post-order finish on RG yields SCCs   |
    | Substring Hash in O(1)                      | Polynomial Rolling Hash (Prefix)   | (h[R+1] - h[L]*P^(len)) % MOD         |
    | Minimum Spanning Tree                       | Kruskal (Sparse) / Prim (Dense)     | Sort edges + DSU / Priority Queue     |

 2. TOP LIVE INTERVIEW & OA GOTCHAS:
    • Tree Centers: An unweighted tree has either exactly 1 or 2 centers; never 0, never >= 3.
    • Tree Diameter: 2-DFS trick only works on trees with NON-NEGATIVE edge weights; fails if negative edges exist.
    • Rolling Hash collisions: Single hash with MOD = 1e9 + 7 has birthday paradox collision risk around N = 10^5;
      always mention or implement double hashing (MOD1 = 1e9+7, MOD2 = 1e9+9) in competitive environments.
    • DSU component count: Start with N components; decrement count every time unite() successfully returns true.
    • Functional Graph component reachability: Node a can reach node b only if comp[a] == comp[b] AND either b is on
      the cycle or b is an ancestor of a in the tree branch feeding into the cycle.
 ====================================================================================================
*/
