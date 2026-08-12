#include <bits/stdc++.h>
using namespace std;

using ll = int;
using ull = unsigned int;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;
const ll P = 31;

// Finds the centroid of a tree (a vertex whose removal splits the tree into components of size <= N/2).
// Time Complexity: O(N), Space Complexity: O(N).
struct Centroid {
    int n; vvi g; vi subsize;
    Centroid(int n, const vvi& g) : n(n), g(g), subsize(n + 1, 0) {}
    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    int dfs(int u, int p) {
        subsize[u] = 1;
        for (int v : g[u]) if (v != p) subsize[u] += dfs(v, u);
        return subsize[u];
    }
    int centroid(int u, int p, int total) {
        for (int v : g[u]) if (v != p && subsize[v] > total / 2) return centroid(v, u, total);
        return u;
    }
    int get() { dfs(1, 0); return centroid(1, 0, subsize[1]); }
};

// Returns the polynomial rolling hash value of a single word in O(|s|).
// Time Complexity: O(|s|), Space Complexity: O(1).
ll hashWord(string s) {
    ll h = 0; for (char c : s) h = (h * P + (c - 'a' + 1)) % MOD;
    return h;
}

// Polynomial rolling hash structure for fast 0-indexed substring hash queries in O(1).
// Time Complexity: O(N) build, O(1) query, Space Complexity: O(N).
struct FastHash {
    vector<ll> p, h;
    FastHash(string s) : p(s.size() + 1, 1), h(s.size() + 1, 0) {
        int n = s.size();
        for (int i = 0; i < n; i++) {
            p[i + 1] = p[i] * P % MOD;
            h[i + 1] = (h[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    ll get(int l, int r) { return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD; }
};

// Computes Z-array of string s using binary search + FastHash (longest common prefix of s and s[i..]).
// Time Complexity: O(N log N), Space Complexity: O(N).
vi z_array(string s) {
    int n = s.size(); vi z(n, 0); if (n == 0) return z;
    FastHash fh(s); z[0] = n;
    for (int i = 1; i < n; i++) {
        int low = 1, high = n - i, ans = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (fh.get(0, mid - 1) == fh.get(i, i + mid - 1)) { ans = mid; low = mid + 1; }
            else high = mid - 1;
        }
        z[i] = ans;
    }
    return z;
}

// Computes KMP Prefix function (pi-array) using the Z-array values.
// Time Complexity: O(N), Space Complexity: O(N).
vi pi_array(string s) {
    vi z = z_array(s); int n = s.size(); vi pi(n, 0);
    for (int i = 1; i < n; i++) {
        for (int len = z[i] - 1; len >= 0; --len) {
            if (pi[i + len] != 0) break;
            pi[i + len] = len + 1;
        }
    }
    return pi;
}

// Computes odd-length and even-length palindrome radii for all centers using FastHash.
// Time Complexity: O(N log N), Space Complexity: O(N).
pair<vi, vi> manacherr(string s) {
    int n = s.size(); vi d1(n, 1), d2(n, 0); if (n == 0) return {d1, d2};
    FastHash fh(s); string s_rev = s; reverse(s_rev.begin(), s_rev.end()); FastHash fh_rev(s_rev);
    auto is_pal = [&](int l, int r) { return l >= 0 && r < n && l <= r && fh.get(l, r) == fh_rev.get(n - 1 - r, n - 1 - l); };
    for (int i = 0; i < n; i++) {
        int low = 2, high = min(i + 1, n - i);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid + 1, i + mid - 1)) { d1[i] = mid; low = mid + 1; } else high = mid - 1;
        }
        low = 1; high = min(i, n - i);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid, i + mid - 1)) { d2[i] = mid; low = mid + 1; } else high = mid - 1;
        }
    }
    return {d1, d2};
}

// Disjoint Set Union (DSU) structure with path compression and union-by-size.
// Time Complexity: O(α(N)) per operation, Space Complexity: O(N).
struct DSU {
    vi p, sz; int comp;
    DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        if ((a = find(a)) == (b = find(b))) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    int size(int x) { return sz[find(x)]; }
};

// Edge representation and Kruskal's algorithm for Minimum Spanning Tree (MST).
// Time Complexity: O(E log E), Space Complexity: O(V + E).
struct Edge { int u, v, w; };
bool comparator(const Edge &a, const Edge &b) { return a.w < b.w; }
int kruskal(int n, vector<Edge>& edges, vector<Edge>& mst_edges) {
    DSU dsu(n); sort(edges.begin(), edges.end(), comparator);
    int weight = 0; mst_edges.clear();
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) { weight += e.w; mst_edges.push_back(e); }
    }
    return weight;
}

// Prim's algorithm for Minimum Spanning Tree (MST) on an adjacency list graph.
// Time Complexity: O(E log V), Space Complexity: O(V + E).
int prim(int n, vector<vector<pii>>& g) {
    vector<bool> in_mst(n + 1, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq; pq.push({0, 1}); int weight = 0;
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true; weight += w;
        for (auto& [v, wt] : g[u]) if (!in_mst[v]) pq.push({wt, v});
    }
    return weight;
}

// Topological sort using Kahn's algorithm (indegree queue traversal).
// Time Complexity: O(V + E), Space Complexity: O(V + E).
vi kahn(int n, vvi& g, int start_node = 1) {
    vi in_deg(g.size(), 0);
    for (int u = start_node; u < start_node + n; u++) for (int v : g[u]) in_deg[v]++;
    queue<int> q; for (int i = start_node; i < start_node + n; i++) if (in_deg[i] == 0) q.push(i);
    vi order;
    while (!q.empty()) {
        int u = q.front(); q.pop(); order.push_back(u);
        for (int v : g[u]) if (--in_deg[v] == 0) q.push(v);
    }
    return order.size() == n ? order : vi{};
}

// Finds longest paths and number of longest paths from src on a DAG using topological order.
// Time Complexity: O(V + E), Space Complexity: O(V + E).
pair<vi, vi> dag_(int n, vector<vector<pii>>& g, int src, int start_node = 1) {
    vi order = kahn(n, g, start_node); const int INF = 1e9;
    vi dist(g.size(), -INF), paths(g.size(), 0); dist[src] = 0; paths[src] = 1;
    for (int u : order) {
        if (dist[u] == -INF) continue;
        for (auto& e : g[u]) {
            if (dist[e.first] < dist[u] + e.second) {
                dist[e.first] = dist[u] + e.second; paths[e.first] = paths[u];
            } else if (dist[e.first] == dist[u] + e.second) {
                paths[e.first] = (paths[e.first] + paths[u]) % MOD;
            }
        }
    }
    return {dist, paths};
}

// Computes sum of distances from every node to all other nodes using tree rerooting DP.
// Time Complexity: O(N), Space Complexity: O(N).
class TreeDistances {
    int n; const vvi& g; vi sz, dist;
public:
    TreeDistances(int nodes, const vvi& graph) : n(nodes), g(graph), sz(nodes + 1, 0), dist(nodes + 1, 0) {}
    void dfs(int u, int p, int depth) {
        sz[u] = 1; dist[1] += depth;
        for (int v : g[u]) { if (v == p) continue; dfs(v, u, depth + 1); sz[u] += sz[v]; }
    }
    void reroot(int u, int p) {
        for (int v : g[u]) { if (v == p) continue; dist[v] = dist[u] + n - 2 * sz[v]; reroot(v, u); }
    }
    vi solve(int root = 1) { dist[1] = 0; dfs(root, 0, 0); reroot(root, 0); return dist; }
};

// Calculates tree diameter length and eccentricity of every node using double DFS traversal.
// Time Complexity: O(N), Space Complexity: O(N).
void dfsTreeDist(int u, int p, vvi& g, vi& dist) {
    for (int v : g[u]) { if (v == p) continue; dist[v] = dist[u] + 1; dfsTreeDist(v, u, g, dist); }
}
pair<int, vi> tree_diameter(int n, vvi& g) {
    vi d1(n + 1), d2(n + 1);
    dfsTreeDist(1, 0, g, d1); int a = max_element(d1.begin() + 1, d1.end()) - d1.begin();
    fill(d1.begin(), d1.end(), 0); dfsTreeDist(a, 0, g, d1); int b = max_element(d1.begin() + 1, d1.end()) - d1.begin();
    int diameter = d1[b]; dfsTreeDist(b, 0, g, d2);
    vi ecc(n + 1); for (int i = 1; i <= n; i++) ecc[i] = max(d1[i], d2[i]);
    return {diameter, ecc};
}

// Tarjan's algorithm for finding bridges (critical edges) in an undirected graph.
// Time Complexity: O(V + E), Space Complexity: O(V + E).
struct Bridge {
    int timer; vi tin, tout, low, parent; vector<pair<int, int>> bridges;
    void dfs(int u, int p, vvi &g) {
        parent[u] = p; tin[u] = low[u] = ++timer;
        for (int v : g[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else { dfs(v, u, g); low[u] = min(low[u], low[v]); if (low[v] > tin[u]) bridges.push_back({u, v}); }
        }
        tout[u] = timer;
    }
    void build(vvi &g) {
        int n = g.size(); timer = 0; tin.assign(n, 0); tout.assign(n, 0); low.assign(n, 0); parent.assign(n, -1); bridges.clear();
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, g);
    }
    bool isAncestor(int u, int v) { return tin[u] <= tin[v] && tout[v] <= tout[u]; }
    bool separated(int u, int v, int a, int b) {
        if (parent[u] == v) swap(u, v);
        return isAncestor(v, a) != isAncestor(v, b);
    }
};

// Tarjan's algorithm for finding articulation points (cut vertices) and component separations.
// Time Complexity: O(V + E), Space Complexity: O(V + E).
struct Articulation {
    int timer; vi tin, tout, low, par; vvi child;
    void dfs(int u, int p, vvi& g) {
        par[u] = p; tin[u] = low[u] = ++timer;
        for (int v : g[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else { child[u].push_back(v); dfs(v, u, g); low[u] = min(low[u], low[v]); }
        }
        tout[u] = timer;
    }
    void build(vvi& g) {
        int n = g.size(); timer = 0; tin.assign(n, 0); tout.assign(n, 0); low.assign(n, 0); par.assign(n, -1); child.assign(n, {});
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, g);
    }
    bool isAncestor(int u, int v) { return tin[u] <= tin[v] && tout[v] <= tout[u]; }
    int findChild(int u, int x) {
        int l = 0, r = child[u].size() - 1;
        while (l <= r) {
            int mid = (l + r) / 2, v = child[u][mid];
            if (tin[x] < tin[v]) r = mid - 1;
            else if (isAncestor(v, x)) return v;
            else l = mid + 1;
        }
        return -1;
    }
    int component(int cut, int x) {
        if (cut == x) return -1;
        if (par[cut] == x) return cut;
        if (!isAncestor(cut, x)) return par[cut];
        return findChild(cut, x);
    }
    bool separated(int cut, int a, int b) {
        if (a == cut || b == cut) return true;
        return component(cut, a) != component(cut, b);
    }
};

// Kosaraju's algorithm for finding Strongly Connected Components (SCC) and building condensation DAG.
// Time Complexity: O(V + E), Space Complexity: O(V + E).
void removeDuplicates(vi& nums) { sort(all(nums)); nums.erase(unique(all(nums)), nums.end()); }
struct SCC {
    int n; vvi g, rg, sccs, dag; vi comp, order; vector<bool> vis;
    SCC(int n) : n(n), g(n), rg(n), comp(n, -1), vis(n) {}
    void add_edge(int u, int v) { g[u].push_back(v); rg[v].push_back(u); }
    void dfs1(int u) {
        vis[u] = true; for (int v : g[u]) if (!vis[v]) dfs1(v); order.push_back(u);
    }
    void dfs2(int u, int c) {
        comp[u] = c; sccs.back().push_back(u);
        for (int v : rg[u]) if (comp[v] == -1) dfs2(v, c);
    }
    void build() {
        for (int i = 0; i < n; i++) if (!vis[i]) dfs1(i);
        for (int i = n - 1; i >= 0; i--) { int u = order[i]; if (comp[u] == -1) { sccs.emplace_back(); dfs2(u, sccs.size() - 1); } }
    }
    void build_dag() {
        dag.resize(sccs.size());
        for (int u = 0; u < n; u++) for (int v : g[u]) if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
        for (auto& adj : dag) removeDuplicates(adj);
    }
};

// Functional Graph Decomposition (every vertex outdegree = 1) for cycles, tree branches, & teleports.
// Time Complexity: O(N log N) build, O(1) jump/query, Space Complexity: O(N log N).
vi getCycleFloyd(const vi &to, const vi &comp, int start) {
    int slow = start, fast = start;
    while (true) {
        slow = to[slow]; fast = to[to[fast]];
        if (comp[slow] != -1) return {};
        if (slow == fast) break;
    }
    slow = start; while (slow != fast) slow = to[slow], fast = to[fast];
    vi cycle; int curr = slow;
    do { cycle.pb(curr); curr = to[curr]; } while (curr != slow);
    return cycle;
}
struct FunctionalGraph {
    int n, LOG = 20; vi dist, cyc, comp, pos; vvi up, radj;
    void dfs(int u) {
        for (int v : radj[u]) {
            if (dist[v] == -1) { dist[v] = dist[u] + 1; cyc[v] = cyc[u]; comp[v] = comp[u]; dfs(v); }
        }
    }
    FunctionalGraph(const vi &to) : n(to.size()), dist(n, -1), cyc(n, -1), comp(n, -1), pos(n, -1), up(n, vi(LOG)), radj(n) {
        for (int i = 0; i < n; i++) up[i][0] = to[i];
        for (int j = 1; j < LOG; j++) for (int i = 0; i < n; i++) up[i][j] = up[up[i][j - 1]][j - 1];
        for (int i = 0; i < n; i++) radj[to[i]].pb(i);
        int cid = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] != -1) continue;
            vi cycle = getCycleFloyd(to, comp, i); if (cycle.empty()) continue;
            int len = cycle.size();
            for (int j = 0; j < len; j++) { int u = cycle[j]; dist[u] = 0; cyc[u] = len; comp[u] = cid; pos[u] = j; }
            for (int u : cycle) dfs(u); cid++;
        }
    }
    int jump(int u, int k) {
        for (int j = 0; j < LOG; j++) if (k >> j & 1) u = up[u][j];
        return u;
    }
    int distance_in_cycle(int a, int b) {
        if (comp[a] != comp[b] || dist[a] || dist[b]) return -1;
        return (pos[b] - pos[a] + cyc[a]) % cyc[a];
    }
    int query(int a, int b) {
        if (comp[a] != comp[b]) return -1;
        bool ac = !dist[a], bc = !dist[b];
        if (ac && bc) return distance_in_cycle(a, b);
        if (ac) return -1;
        if (!bc) return dist[a] >= dist[b] && jump(a, dist[a] - dist[b]) == b ? dist[a] - dist[b] : -1;
        return dist[a] + distance_in_cycle(jump(a, dist[a]), b);
    }
};

// Precomputes powers for double-hashing tree isomorphism checks.
// Time Complexity: O(MAX_LEN), Space Complexity: O(MAX_LEN).
const long long MOD1 = 1000000007, MOD2 = 1000000009, BASE1 = 313, BASE2 = 317;
const int MAX_LEN = 200005;
long long pow1[MAX_LEN], pow2[MAX_LEN];
void precompute() {
    pow1[0] = pow2[0] = 1;
    for (int i = 1; i < MAX_LEN; i++) {
        pow1[i] = (pow1[i - 1] * BASE1) % MOD1; pow2[i] = (pow2[i - 1] * BASE2) % MOD2;
    }
}
vector<long long> dfsTreeHash(int u, int p, const vvi& graph) {
    vector<vector<long long>> childs;
    for (int v : graph[u]) if (v != p) childs.push_back(dfsTreeHash(v, u, graph));
    sort(childs.begin(), childs.end());
    long long h1 = 1, h2 = 1; int total_len = 1;
    for (auto &child : childs) {
        long long child_h1 = child[0], child_h2 = child[1]; int len = child[2];
        h1 = (h1 * pow1[len] + child_h1) % MOD1; h2 = (h2 * pow2[len] + child_h2) % MOD2;
        total_len += len;
    }
    h1 = (h1 * BASE1 + 2) % MOD1; h2 = (h2 * BASE2 + 2) % MOD2; total_len++;
    return {h1, h2, total_len};
}

// Finds tree center(s) by iteratively trimming leaf nodes.
// Time Complexity: O(N), Space Complexity: O(N).
vi centers(int n, vvi& g) {
    if (n == 1) return {1};
    vi deg(n + 1), leaves;
    for (int i = 1; i <= n; i++) if ((deg[i] = g[i].size()) == 1) leaves.push_back(i);
    while (n > 2) {
        n -= leaves.size(); vi nxt;
        for (int u : leaves) for (int v : g[u]) if (--deg[v] == 1) nxt.push_back(v);
        leaves = nxt;
    }
    return leaves;
}
