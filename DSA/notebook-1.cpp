
#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cstring>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;


struct SparseTable {
    int n, K;
    vector<vector<int>> st;

    SparseTable(vector<int> &a) : n(a.size()), K((a.size() > 0 ? 31 - __builtin_clz(a.size()) : 0) + 1), st(K, vector<int>(a.size())) {
        st[0] = a;
        for (int j = 1; j < K; j++)
            for (int i = 0; i + (1<<j) <= n; i++)
                st[j][i] = min(st[j-1][i], st[j-1][i+(1<<(j-1))]);
    }

    int query(int l, int r) {
        int j = 31 - __builtin_clz(r - l + 1);
        return min(st[j][l], st[j][r - (1<<j) + 1]);
    }
};


struct Centroid{
    int n;
    vector<vector<int>> g;
    vector<int> subsize;

    Centroid(int n, const vector<vector<int>>& g) : n(n), g(g), subsize(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int dfs(int u, int p) {
        subsize[u] = 1;
        for (int v : g[u]) {
            if (v != p) {
                subsize[u] += dfs(v, u);
            }
        }
        return subsize[u];
    }

    int centroid(int u, int p, int total) {
        for (int v : g[u]) {
            if (v != p && subsize[v] > total / 2) {
                return centroid(v, u, total);
            }
        }
        return u;
    }

    int get(){
        dfs(1, 0);
        return centroid(1, 0, subsize[1]);
    }
};

// Returns the polynomial rolling hash of a single word/string 's' in O(|s|)
ll hashWord(string s) {
    ll h = 0;
    for (char c : s) h = (h * P + (c - 'a' + 1)) % MOD;
    return h;
}

/*
Usage:
  FastHash fh(s);          // Build prefix hashes for string 's' (0-indexed)
  fh.get(l, r);            // Returns hash value of substring s[l..r] in O(1)
Tips to change polynomial parameters or double hashing:
  1. Change base multiplier 'P' (default 31 for lowercase, 53 for mixed case) or 'MOD'.
  2. For double hashing to avoid collisions, compute hash using two different bases/mods.
*/
struct FastHash {
    vector<ll> p, h;
    FastHash(string s) : p(s.size() + 1, 1), h(s.size() + 1, 0) {
        int n = s.size();
        for (int i = 0; i < n; i++) {
            p[i + 1] = p[i] * P % MOD;
            h[i + 1] = (h[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    ll get(int l, int r) {
        return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD;
    }
};

/*
Z-array using FastHash (Z-Algo equivalent)
Time Complexity: O(N log N)
Usage:
  vector<int> z = z_array(s);
*/
vector<int> z_array(string s) {
    int n = s.size();
    vector<int> z(n, 0);
    if (n == 0) return z;
    FastHash fh(s);
    z[0] = n;
    for (int i = 1; i < n; i++) {
        int low = 1, high = n - i, ans = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (fh.get(0, mid - 1) == fh.get(i, i + mid - 1)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        z[i] = ans;
    }
    return z;
} // z[i] denotes the length of the longest substring starting from s[i] which is also a prefix of s

vector<int> pi_array(string s) {
    vector<int> z = z_array(s);
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++){
        for (int len = z[i] - 1; len >= 0; --len) {
            if (pi[i + len] != 0) break;
            pi[i + len] = len + 1;
        }
    }
    return pi;
} // pi[i] = length of longest proper prefix which is also suffix for s[0..i]


/*
Manacher's equivalent using FastHash (Palindromic substrings)
Time Complexity: O(N log N)
Returns:
  pair<vector<int>, vector<int>>: {d1, d2}
  radius means the length of the palindrome centered at that index (full length = 2*radius - 1 for odd, 2*radius for even)
  d1[i] = radius of palindrome centered at i (odd-length)
  d2[i] = radius of palindrome centered between i-1 and i (even-length)
*/
pair<vi, vi> manacherr(string s) {
    int n = s.size();
    vi d1(n, 1), d2(n, 0);
    if (n == 0) return {d1, d2};

    FastHash fh(s);
    string s_rev = s; reverse(s_rev.begin(), s_rev.end());
    FastHash fh_rev(s_rev);

    auto is_pal = [&](int l, int r) {
        return l >= 0 && r < n && l <= r && fh.get(l, r) == fh_rev.get(n - 1 - r, n - 1 - l);
    };

    for (int i = 0; i < n; i++) {
        int low = 2, high = min(i + 1, n - i);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid + 1, i + mid - 1)) d1[i] = mid, low = mid + 1;
            else high = mid - 1;
        }

        low = 1, high = min(i, n - i);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid, i + mid - 1)) d2[i] = mid, low = mid + 1;
            else high = mid - 1;
        }
    }
    return {d1, d2};
}

/*
Usage:
  DSU dsu(n);             // Init with n elements (1-indexed safe)
  dsu.unite(u, v);        // Connects u and v. Returns false if already connected.
  dsu.find(u) == dsu.find(v); // Check if u and v are in the same component
  dsu.size(u);            // Get the size of the component containing u
  dsu.comp;               // Total number of disconnected components left
*/
struct DSU {
    vector<int> p, sz;
    int comp;
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

int removeStones(vector<vector<int>>& stones) {
    const int OFFSET = 10001;
    DSU dsu(20005);
    
    unordered_set<int> used;
    
    for (auto &s : stones) {
        int row = s[0];
        int col = s[1] + OFFSET;
        
        dsu.unite(row, col);
        
        used.insert(row);
        used.insert(col);
    }
    int components = 0;
    for (int x : used) {
        if (dsu.find(x) == x) components++;
    }
    return stones.size() - components;
}

/*
Kruskal's Algorithm (MST)
Time Complexity: O(E log E)
Usage:
  vector<Edge> edges, mst_edges;
  long long total_weight = kruskal(n, edges, mst_edges);
*/
struct Edge {
    int u, v; long long w;
    bool operator<(const Edge& o) const { return w < o.w; }
};

long long kruskal(int n, vector<Edge>& edges, vector<Edge>& mst_edges) {
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    long long mst_weight = 0;
    mst_edges.clear();
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst_weight += e.w;
            mst_edges.push_back(e);
        }
    }
    return mst_weight;
}

long long prim(int n, vector<vector<pair<int, long long>>>& g) {
    vector<bool> in_mst(n + 1, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.push({0, 1});
    long long mst_weight = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true;
        mst_weight += w;

        for (auto& [v, weight] : g[u]) {
            if (!in_mst[v]) {
                pq.push({weight, v});
            }
        }
    }
    return mst_weight;
}

/*
Topological Sort (Kahn's and DFS)
Returns topological order of nodes. Returns empty vector if cycle exists.
*/
vector<int> kahn(int n, vector<vector<int>>& g, int start_node = 1) {
    vector<int> in_deg(g.size(), 0);
    for (int u = start_node; u < start_node + n; u++) {
        for (int v : g[u]) in_deg[v]++;
    }
    queue<int> q;
    for (int i = start_node; i < start_node + n; i++) {
        if (in_deg[i] == 0) q.push(i);
    }
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--in_deg[v] == 0) q.push(v);
        }
    }
    return order.size() == n ? order : vector<int>{};
}

/*
DAG Solver (Shortest/Longest Paths, Path Counts)
Returns {dist, path_counts} in O(V + E) using Kahn's topological sort.
*/
struct DAGEdge { int to; long long w; };
pair<vector<long long>, vector<long long>> solve_dag(int n, vector<vector<DAGEdge>>& g, int src, int start_node = 1, bool max_path = false) {
    vector<int> in_deg(g.size(), 0);
    for (int u = start_node; u < start_node + n; u++) {
        for (auto& e : g[u]) in_deg[e.to]++;
    }
    queue<int> q;
    for (int i = start_node; i < start_node + n; i++) {
        if (in_deg[i] == 0) q.push(i);
    }
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (auto& e : g[u]) {
            if (--in_deg[e.to] == 0) q.push(e.to);
        }
    }
    const long long INF_VAL = 1e18;
    vector<long long> dist(g.size(), max_path ? -INF_VAL : INF_VAL);
    vector<long long> paths(g.size(), 0);
    dist[src] = 0; paths[src] = 1;
    for (int u : order) {
        if (dist[u] == (max_path ? -INF_VAL : INF_VAL)) continue;
        for (auto& e : g[u]) {
            if (max_path) {
                dist[e.to] = max(dist[e.to], dist[u] + e.w);
            } else {
                dist[e.to] = min(dist[e.to], dist[u] + e.w);
            }
            paths[e.to] = (paths[e.to] + paths[u]) % MOD;
        }
    }
    return {dist, paths};
}

class TreeDistances {
    int n;
    const vector<vector<int>>& g;
    vector<int> subtree_sz;
    vector<long long> total_dist;

public:
    TreeDistances(int nodes, const vector<vector<int>>& graph)
        : n(nodes), g(graph),
          subtree_sz(nodes + 1, 0),
          total_dist(nodes + 1, 0) {}

    void dfs_size(int u, int p) {
        subtree_sz[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs_size(v, u);
            subtree_sz[u] += subtree_sz[v];
        }
    }

    void dfs_root(int u, int p, int depth) {
        total_dist[1] += depth;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs_root(v, u, depth + 1);
        }
    }

    void reroot(int u, int p) {
        for (int v : g[u]) {
            if (v == p) continue;
            total_dist[v] = total_dist[u] - subtree_sz[v] + (n - subtree_sz[v]);
            reroot(v, u);
        }
    }

    vector<long long> solve(int root = 1) {
        dfs_size(root, 0);
        dfs_root(root, 0, 0);
        reroot(root, 0);
        return total_dist;
    }
};

class TreeDiameter {
    int n; const vector<vector<int>>& g;
    vector<int> d1, d2;

    void dfs(int u, int p, vector<int>& d) {
        for (int v : g[u]) if (v != p)
            d[v] = d[u] + 1, dfs(v, u, d);
    }

    int farthest(vector<int>& d) {
        return max_element(d.begin() + 1, d.end()) - d.begin();
    }

public:
    TreeDiameter(int n, const vector<vector<int>>& g)
        : n(n), g(g), d1(n + 1), d2(n + 1) {}

    int diameter() {
        fill(d1.begin(), d1.end(), 0);
        dfs(1, 0, d1);
        int a = farthest(d1);

        fill(d1.begin(), d1.end(), 0);
        dfs(a, 0, d1);
        int b = farthest(d1);

        return d1[b];
    }

    vector<int> eccentricity() {
        fill(d1.begin(), d1.end(), 0);
        dfs(1, 0, d1);
        int a = farthest(d1);

        fill(d1.begin(), d1.end(), 0);
        dfs(a, 0, d1);
        int b = farthest(d1);

        fill(d2.begin(), d2.end(), 0);
        dfs(b, 0, d2);

        vector<int> ans(n + 1);
        for (int i = 1; i <= n; i++) ans[i] = max(d1[i], d2[i]);
        return ans;
    }
};

/*
Usage:
  Bridge B;
  auto ans = B.get(g); // Returns vector<pair<int,int>> of bridges
*/
struct Bridge {
    int t; vector<int> tin, low; vector<pair<int,int>> res;
    void dfs(int u, int p, vector<vector<int>>& g) {
        tin[u] = low[u] = ++t;
        for (int v : g[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u, g);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) res.push_back({u, v});
            }
        }
    }
    vector<pair<int,int>> get(vector<vector<int>>& g) {
        int n = g.size(); t = 0; tin.assign(n, 0); low.resize(n); res.clear();
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, g);
        return res;
    }
};

struct Articulation {
    int t; vector<int> tin, low; vector<int> res;
    void dfs(int u, int p, vector<vector<int>>& g) {
        tin[u] = low[u] = ++t; int children = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u, g); low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u] && p != -1) res.push_back(u);
                children++;
            }
        }
        if (p == -1 && children > 1) res.push_back(u);
    }
    vector<int> get(vector<vector<int>>& g) {
        int n = g.size(); t = 0; tin.assign(n, 0); low.resize(n); res.clear();
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, g);
        return res;
    }
};


/*
Usage:
  SCC graph(n);
  graph.add(u, v);
  auto components = graph.build();
  int cid = graph.comp[u];
  auto dag = graph.get_dag();
*/
struct SCC {
    int n; vector<vector<int>> g, rg, sccs, dag; stack<int> order; vector<int> comp; vector<bool> vis;
    SCC(int n) : n(n), g(n + 1), rg(n + 1), comp(n + 1, -1), vis(n + 1) {}

    void add(int u, int v) { g[u].push_back(v); rg[v].push_back(u); }

    void dfs1(int u) {
        vis[u] = 1;
        for (int v : g[u]) if (!vis[v]) dfs1(v);
        order.push(u);
    }
    void dfs2(int u, int c) {
        comp[u] = c; sccs[c].push_back(u);
        for (int v : rg[u]) if (comp[v] == -1) dfs2(v, c);
    }
    vector<vector<int>> build() {
        for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);
        while (!order.empty()) {
            int u = order.top(); order.pop();
            if (comp[u] == -1) {
                sccs.emplace_back();
                dfs2(u, sccs.size() - 1);
            }
        }
        return sccs;
    }
    vector<vector<int>> get_dag() {
        dag.resize(sccs.size());
        for (int u = 1; u <= n; u++) for (int v : g[u])
            if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
        for (auto& neighbors : dag) {
            sort(neighbors.begin(), neighbors.end());
            neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
        }
        return dag;
    }
};

class DirectedGraphToDAG{
    // first build the SCC of the directed graph, then build the DAG of the SCCs
    SCC scc;
public:
    DirectedGraphToDAG(int n) : scc(n) {}
    vector<vector<int>> buildDAG(const vector<pair<int,int>>& edges) {
        for (auto& e : edges) scc.add(e.first, e.second);
        scc.build();
        return scc.get_dag();
    } // it returned the DAG of the SCCs, where each node in the DAG represents a strongly connected component of the original graph.

}; // this helps in many problem solving, especially when we need to analyze the structure of a directed graph and its strongly connected components. normally it will be tough, but after conversion to DAG, it becomes easier to analyze and solve problems related to reachability, cycles, and other properties of the graph.



struct HierholzerUndirected {
    int n, m = 0;
    vector<vector<pair<int,int>>> g;
    vector<bool> used;
    vector<int> deg, path;

    HierholzerUndirected(int n) : n(n), g(n + 1), deg(n + 1, 0) {}

    // Add an undirected edge
    void addEdge(int u, int v) {
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        used.push_back(false);
        deg[u]++;
        deg[v]++;
        m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, id] = g[u].back();
            g[u].pop_back();

            if (used[id]) continue;
            used[id] = true;

            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getEulerianCircuit(int start) {
        // Every vertex must have even degree.
        for (int i = 1; i <= n; i++)
            if (deg[i] & 1)
                return {};

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        // Every edge must have been used.
        if ((int)path.size() != m + 1)
            return {};

        return path;
    }
    vector<int> getEulerianPath(int start, int end) {
        if (start == end) return getEulerianCircuit(start);
        // Exactly 2 vertices must have odd degree (start and end).
        for (int i = 1; i <= n; i++) {
            if (i == start || i == end) {
                if (!(deg[i] & 1)) return {};
            } else {
                if (deg[i] & 1) return {};
            }
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        // Every edge must have been used.
        if ((int)path.size() != m + 1)
            return {};

        return path;
    } // eulerian path means that it visits every edge exactly once, but it does not necessarily start and end at the same vertex. An Eulerian circuit is a special case of an Eulerian path that starts and ends at the same vertex.
};

/*
Returns:
- Eulerian circuit if it exists.
- Empty vector otherwise.

Checks performed:
1. Every vertex has even degree.
2. All edges are reachable from the start vertex
   (verified by path.size() == edges + 1).

/*
Hierholzer's Algorithm
----------------------
Used to find an Eulerian Path/Circuit in O(V + E).

Definitions
-----------
Eulerian Path    : Visits every edge exactly once.
Eulerian Circuit : Eulerian Path that starts and ends at the same vertex.

Undirected Graph
----------------
Eulerian Circuit:
- Every vertex has even degree.
- All vertices having degree > 0 belong to one connected component.

Eulerian Path:
- Exactly 0 or 2 vertices have odd degree.
- If 2 exist, they are the start and end.
- All vertices having degree > 0 belong to one connected component.

Directed Graph
--------------
Eulerian Circuit:
- indegree(v) == outdegree(v) for every vertex.
- All vertices with edges are connected.

Eulerian Path:
- One vertex: outdegree = indegree + 1 (start)
- One vertex: indegree = outdegree + 1 (end)
- All others: indegree = outdegree

Why edge IDs?
-------------
In an undirected graph each edge appears twice in gacency lists.
A unique edge ID ensures every edge is traversed exactly once.

Algorithm
---------
dfs(u):
    while(u has unused edge)
        mark edge used
        dfs(next)
    path.push_back(u)

Reverse the path at the end.

Verification
------------
Let E be the number of edges.
A valid Eulerian traversal must contain exactly E + 1 vertices.

if(path.size() != E + 1)
    => Graph is disconnected or some edges were not visited.

Complexity
----------
Time  : O(V + E)
Space : O(V + E)

*/

struct HierholzerDirected {
    int n, m = 0;
    vector<vector<int>> g;
    vector<int> indeg, outdeg;
    vector<int> path;

    HierholzerDirected(int n)
        : n(n), g(n + 1), indeg(n + 1, 0), outdeg(n + 1, 0) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        outdeg[u]++;
        indeg[v]++;
        m++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            int v = g[u].back();
            g[u].pop_back();
            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getEulerianCircuit(int start) {
        // Every vertex must satisfy indegree == outdegree.
        for (int i = 1; i <= n; i++)
            if (indeg[i] != outdeg[i])
                return {};

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        // Every edge must have been used.
        if ((int)path.size() != m + 1)
            return {};

        return path;
    }

    vector<int> getEulerianPath(int start, int end) {
        if (start == end) return getEulerianCircuit(start);
        // Validation: Start and End vertices should satisfy the indegree/outdegree conditions and rest of the vertices should have equal indegree and outdegree.
        for (int i = 1; i <= n; i++) {
            if (i == start) {
                if (outdeg[i] != indeg[i] + 1)
                    return {};
            } else if (i == end) {
                if (indeg[i] != outdeg[i] + 1)
                    return {};
            } else {
                if (indeg[i] != outdeg[i])
                    return {};
            }
        }

        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());

        if ((int)path.size() != m + 1)
            return {};

        return path;
    }
}; 

/*
Returns:
- Eulerian circuit if it exists, starting from the given vertex and ending on the same vertex.
- Empty vector otherwise.

Checks performed:
1. indegree(v) == outdegree(v) for every vertex.
2. All edges are reachable from the start vertex
   (verified by path.size() == edges + 1).

To obtain an Eulerian Path instead of a Circuit:
- Exactly one vertex should satisfy outdegree = indegree + 1 (start).
- Exactly one vertex should satisfy indegree = outdegree + 1 (end).
- Every other vertex must satisfy indegree == outdegree.
*/


/*
================================================================================
FUNCTIONAL GRAPH DECOMPOSITION
================================================================================
A Functional Graph is a directed graph where every vertex has an outdegree of exactly 1.
Every connected component of such a graph contains exactly one directed cycle, with
trees rooted on the cycle nodes and edges directed towards the cycle.

Visual Example:
      [Tree Leaf] -> [Tree Node] 
                           \
                            v
   [Cycle Node 1] ----> [Cycle Node 2 (Root of Tree)]
         ^                     |
         |                     v
   [Cycle Node 4] <---- [Cycle Node 3]

This decomposition structure computes:
1. Identifying all cycle vertices.
2. The length of the cycle for each cycle vertex (non-cycle vertices have cyc[u] = -1).
3. The distance of each non-cycle vertex to the cycle (cycle vertices have dist[u] = 0).

Time Complexity : O(N)
Space Complexity: O(N)
================================================================================
*/
struct FunctionalGraphDecomposition {
    int n;
    vector<int> cyc;   // cyc[i] = length of the cycle node `i` belongs to (or -1 if not on a cycle)
    vector<int> dist;  // dist[i] = distance of node `i` to its cycle root (0 if on a cycle)
    vector<int> indeg; // indeg[i] = in-degree of node `i`
    vector<vector<int>> rev; // rev[u] = stores reversed graphacency list (to walk away from cycle roots)
    const int LOG = 20;
    vector<vector<int>> up; // up[u][j] = 2^j-th ancestor of node `u` in the reversed graph

    // `to[i]` represents the single outgoing edge from vertex `i` (0-indexed)
    FunctionalGraphDecomposition(vector<int> &to) {
        n = to.size();
        cyc.assign(n, -1);
        dist.assign(n, -1);
        indeg.assign(n, 0);
        rev.assign(n, {});
        up.assign(n, vector<int>(LOG, -1));
        for(int i = 0; i < n; i++) up[i][0] = to[i];

        for(int k=1; k < LOG; k++){
            for(int i = 0; i < n; i++){
                up[i][k] = up[up[i][k-1]][k-1];}}

        // Build reversed graph and calculate in-degrees
        for (int i = 0; i < n; i++)
            rev[to[i]].push_back(i), indeg[to[i]]++;

        findCycles(to);
        buildDist();
        
    }

    int jump(int u, int k) {
        for(int j = 0; j < LOG; j++){
            if(k & (1 << j)) u = up[u][j];}
        return u;
    }
    // Identifies cycles and computes cycle lengths
    void findCycles(vector<int> &to) {
        vector<int> alive(n, 1), vis(n);
        queue<int> q;

        // Leaf-peeling (Kahn's Topological Sort algorithm style):
        // Repeatedly remove vertices with in-degree 0.
        // The vertices that are left "alive" at the end are exactly the cycle vertices.
        for (int i = 0; i < n; i++)
            if (!indeg[i]) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            alive[u] = 0; // Mark tree nodes as dead (not part of any cycle)
            if (!--indeg[to[u]]) q.push(to[u]);
        }

        // For all alive (cycle) nodes, traverse and compute their cycle length
        for (int i = 0; i < n; i++) {
            if (!alive[i] || vis[i]) continue;

            // 1. Calculate the cycle length
            int u = i, len = 0;
            do vis[u] = 1, len++, u = to[u];
            while (u != i);

            // 2. Set cycle length and initialize distance to 0 for all nodes in this cycle
            u = i;
            do {
                cyc[u] = len;
                dist[u] = 0;
                u = to[u];
            } while (u != i);
        }
    }

    // DFS on the reversed graph to propagate the distance to cycle roots
    void dfs(int u) {
        for (int v : rev[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                dfs(v);
            }
    }

    // Propagates distances from all cycle vertices outward to tree leaves
    void buildDist() {
        for (int i = 0; i < n; i++)
            if (cyc[i] != -1) // If `i` is on a cycle, start DFS to find distances of its tree branches
                dfs(i);
    }
};

    
struct TreeIsomorphism {
 
    int n;
    const vector<vector<int>>& g;
    vector<int> id;
 
    map<vector<int>, int>& mp;
    int& nxt;
 
    TreeIsomorphism(int nodes, const vector<vector<int>>& graph,
                    map<vector<int>, int>& mp,
                    int& nxt)
        : n(nodes), g(graph), id(nodes + 1), mp(mp), nxt(nxt) {}
 
    int dfs(int u, int p) {
        vector<int> child;
 
        for (int v : g[u]) {
            if (v == p) continue;
            child.push_back(dfs(v, u));
        }
 
        sort(child.begin(), child.end());
 
        if (!mp.count(child))
            mp[child] = nxt++;
 
        return id[u] = mp[child];
    }
 
    int getID(int root = 1) {
        return dfs(root, 0);
    }
};

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
                if (--deg[v] == 1) next_leaves.push_back(v); // if v becomes a leaf, add it to the next layer
            }
        }
        leaves = next_leaves; // Move to the inner layer
    }
    
    return leaves; // The last remaining leaves are the centers
} // returns the center(s) of the tree. A tree can have either one center (if it has an odd number of nodes) or two centers (if it has an even number of nodes). The center(s) are the node(s) that minimize the maximum distance to any other node in the tree.


struct CentroidDecomposition {
    int n, k;
    long long ans = 0;
    vector<vector<int>> &graph;
    vector<int> sz, cnt;
    vector<bool> removed;
    // cnt[i] means the number of nodes at depth `i` from the current centroid. This is used to count pairs of nodes whose distances sum to `k`.
    CentroidDecomposition(int n, vector<vector<int>> &graph, int k)
        : n(n), k(k), graph(graph), sz(n + 1), removed(n + 1), cnt(k + 1) {}

    int getSize(int u, int p) {
        sz[u] = 1;
        for (int v : graph[u])
            if (v != p && !removed[v]) sz[u] += getSize(v, u);
        return sz[u];
    }

    int getCentroid(int u, int p, int total) {
        for (int v : graph[u])
            if (v != p && !removed[v] && sz[v] > total / 2)
                return getCentroid(v, u, total);
        return u;
    }

    void collectDepths(int u, int p, int d, vector<int> &depths) {
        depths.push_back(d);
        for (int v : graph[u])
            if (v != p && !removed[v])
                collectDepths(v, u, d + 1, depths);
    } // This function collects the depths of all nodes in the subtree rooted at node `u`, excluding the parent node `p`. It recursively traverses the tree, incrementing the depth `d` for each child node. The collected depths are stored in the `depths` vector, which can later be used to count pairs of nodes whose distances sum to a specific value `k`.

    void process(int c) {
        cnt[0] = 1;
        int mxDepth = 0;
        for (int child : graph[c]) {
            if (removed[child]) continue;
            vector<int> depths;
            collectDepths(child, c, 1, depths); // for each child of the centroid, collect the depths of its subtree, then count how many pairs can be formed with previously counted depths that sum to `k`. After processing all children, reset the counts for the next iteration.
            for (int d : depths)
                if (d <= k) ans += cnt[k - d];
            for (int d : depths)
                if (d <= k) cnt[d]++, mxDepth = max(mxDepth, d);
        }
        for (int i = 0; i <= mxDepth; i++) cnt[i] = 0;
    } // this function processes the centroid `c` by counting pairs of nodes whose distances sum to `k`. It initializes the count of nodes at depth 0 (the centroid itself) and iterates through each child of the centroid. For each child, it collects the depths of its subtree and counts how many pairs can be formed with previously counted depths that sum to `k`. After processing all children, it resets the counts for the next iteration.

    Fenwick bit(n);

    void process2(int c) {

        bit.update(0, 1);          // centroid at depth 0

        vector<int> usedDepths;
        usedDepths.push_back(0);

        for (int child : graph[c]) {

            if (removed[child]) continue;

            vector<int> depths;
            collectDepths(child, c, 1, depths);

            // Query
            for (int d : depths) {

                int l = max(0, k1 - d); // why max(0, k1 - d)? because we want to find pairs of nodes whose distances sum to a value within the range [k1, k2]. For a node at depth `d`, the other node in the pair must be at a depth that satisfies the condition: `k1 <= d + other_depth <= k2`. Rearranging this gives us the range for `other_depth`: `k1 - d <= other_depth <= k2 - d`. Since depths cannot be negative, we take the maximum of 0 and `k1 - d` to ensure we only consider valid depths.
                int r = min(n, k2 - d);

                if (l <= r)
                    ans += bit.query(l, r);
            }

            // Insert
            for (int d : depths) {
                bit.update(d, 1);
                usedDepths.push_back(d);
            }
        }

        // Clear BIT
        for (int d : usedDepths)
            bit.update(d, -1);
    } // this function processes the centroid `c` by counting pairs of nodes whose distances fall within a specified range `[k1, k2]`. It uses a Fenwick Tree (Binary Indexed Tree) to efficiently query and update counts of nodes at various depths. The function initializes the count for the centroid at depth 0, collects depths for each child subtree, queries the BIT for valid pairs, and updates the BIT with new depths. Finally, it clears the BIT to reset counts for the next iteration.

    void decompose(int entry) {
        int centroid = getCentroid(entry, -1, getSize(entry, -1));
        process(centroid);
        removed[centroid] = 1;
        for (int v : graph[centroid])
            if (!removed[v]) decompose(v);
    }

    void build() { decompose(1); }
};


/*
==================== Graph / Tree Formula Cheat Sheet ====================

1. Tree
- Edges = n - 1
- Sum of degrees = 2(n - 1)
- Leaves = 2 + Σ(deg(v) - 2), for deg(v) >= 2

2. Connected Components
- Minimum edges to connect graph = components - 1
- Independent cycles (Cyclomatic Number) = m - n + components

3. Eulerian Graph
- Euler Circuit  <=> all degrees even
- Euler Path     <=> exactly 0 or 2 odd-degree vertices
- #Eulerian subgraphs = 2^(m - n + components)

4. Bipartite Graph
- Graph is bipartite <=> no odd cycle
- Complete bipartite K(a,b) has a*b edges

5. Complete Graph K_n
- Edges = n(n - 1) / 2
- Degree of every vertex = n - 1

6. Handshaking Lemma
- Σ degree = 2m
- Number of odd-degree vertices is always even

7. Incidence Matrix
- rank = n - components

8. Binary Tree
- Max nodes at level i = 2^i
- Max nodes of height h = 2^(h + 1) - 1
- Full binary tree: leaves = internal nodes + 1

9. Diameter of Tree
- BFS/DFS from any node -> farthest u
- BFS/DFS from u -> diameter

10. LCA
- Binary Lifting: preprocess O(n log n), query O(log n)

11. MST
- Every MST contains exactly n - 1 edges

14. SCC
- Condensation graph is always a DAG

17. Maximum Edges
- Undirected simple graph = n(n - 1) / 2
- Directed simple graph   = n(n - 1)

18. Complete Binary Tree
- Height = floor(log2(n))

19. Useful Tree DP Identity
- Σ(subtree sizes) = n + Σ(depth)

20. Centre of Tree
- 1 or 2 nodes
- nodes with minimum maximum distance to all other nodes

21. Centroid of Tree
- nodes which after removal leave all components with size <= n/2
=======================================================================
*/


/*
======================== MINIMUM SPANNING TREE (MST) CHEAT SHEET ========================

Definition
----------
- A spanning tree connects all vertices using exactly (n - 1) edges.
- An MST is a spanning tree with minimum total edge weight.
- MST exists only if the graph is connected.
- If graph is disconnected -> Minimum Spanning Forest (MSF).
----------------------------------------------------------------
What is a Cut?
----------------------------------------------------------------
A cut is ANY partition of the vertices into two non-empty sets.

Example:

        {1,2,3} | {4,5,6}

Every edge having one endpoint in each set is called a
"CROSSING EDGE" of the cut.

Example:

1 ----- 4
2 ----- 5
3 ----- 6

All three edges cross this cut.

There are exponentially many possible cuts.

----------------------------------------------------------------
Cut Property (Most Important)
----------------------------------------------------------------
For ANY cut:

The minimum weight edge crossing that cut
belongs to SOME MST.

If the minimum edge is UNIQUE,
it belongs to EVERY MST.

Use this theorem to prove an edge MUST be chosen.

Properties
----------
- Number of edges in MST = n - 1.
- MST contains no cycles.
- There may be multiple MSTs if equal edge weights exist.
- If all edge weights are distinct -> MST is unique.
- Adding any extra edge to an MST creates exactly one cycle.
- Removing any edge from an MST disconnects the tree.

Algorithms
----------
Kruskal
- Sort edges by weight.
- Add edge if it connects two different components.
- Use DSU (Union Find).
- Complexity: O(E log E).

Prim
- Similar to Dijkstra.
- Grow one connected component.
- Use priority queue.
- Complexity:
    O(E log V) with heap.
    O(V^2) with graphacency matrix.

Boruvka
- Each component picks its cheapest outgoing edge.
- Mostly theoretical/parallel.

Cut Property (Most Important)
-----------------------------
For ANY cut of the graph:
- The minimum weight edge crossing the cut ALWAYS belongs to SOME MST.

Used to prove Kruskal.

Cycle Property
--------------
For ANY cycle:
- The maximum weight edge in that cycle can NEVER belong to an MST
  (if it is strictly heavier than the others).

Useful for eliminating edges.

Edge Classification
-------------------
Given edge (u,v,w):

Definitely in every MST:
- It is the UNIQUE minimum edge across some cut.

Never in any MST:
- It is the UNIQUE maximum edge in some cycle.

Otherwise:
- It may or may not appear depending on equal weights.

Maximum Edge Trick
------------------
If edge (u,v,w) is NOT in MST:

Maximum edge on path(u,v) in MST >= w

Reason:
Otherwise replacing the heavier edge with this one improves MST.

Second Best MST
---------------
Typical approach:
1. Build MST.
2. For every non-MST edge:
       replace maximum edge on MST path(u,v)
3. Minimum valid replacement gives second MST.

Requires:
- Binary Lifting + Max Edge on Path
or
- Heavy Light Decomposition.

Updating MST
------------
Add one edge:
- Forms a cycle.
- Remove maximum edge on that cycle.

Delete one MST edge:
- Need minimum edge reconnecting the two components.

Negative Weights
----------------
MST works perfectly with negative edges.

Directed Graph
--------------
No MST exists.
Equivalent concept:
- Minimum Spanning Arborescence (Edmonds' Algorithm).

Difference from Shortest Path Tree
----------------------------------
MST:
- Minimizes TOTAL tree weight.

Shortest Path Tree:
- Minimizes distance from ONE source.

They are usually different.

What Changes MST?
-----------------
Changing ONE edge weight:
- May or may not change MST.
- Increasing an MST edge can remove it.
- Decreasing a non-MST edge can insert it.

Common OA / Contest Patterns
----------------------------
1. Build MST (Kruskal)
2. Maximum spanning tree
3. Minimum bottleneck spanning tree
4. Count number of MSTs
5. Critical / Pseudo-critical edges
6. Second Best MST
7. MST after adding/removing edges
8. Offline connectivity using DSU
9. MST on grid
10. XOR/AND/OR weighted MST variants

DSU Template
------------
find(x)
union(a,b)

Always:
- Path Compression
- Union by Size / Rank

Complexity:
O(alpha(N)) per operation.

Recognition Clues
-----------------
Think MST when problem says:
- Connect all cities
- Minimum total cable cost
- Road construction
- Network design
- Electric wiring
- Internet connections
- Water pipelines
- Connect islands
- Minimum infrastructure cost
- Exactly n-1 chosen edges
- Remove expensive redundant roads

Common Tricks
-------------
- Maximum Spanning Tree:
      Sort edges descending.

- Minimax Path:
      Build MST.
      Answer = maximum edge on MST path.

- Maximum Bottleneck:
      Build Maximum Spanning Tree.

- Offline Queries:
      Sort edges and queries together by weight.

Remember
--------
Kruskal:
    Sort edges.

Prim:
    Grow tree.

Cut Property:
    Minimum crossing edge is safe.

Cycle Property:
    Maximum edge in a cycle is unsafe.

=========================================================================================
*/

/*
======================== MST EDGE THEORY ========================

Let edge e = (u, v, w)

----------------------------------------------------------------
1. Edge in EVERY MST
----------------------------------------------------------------
Edge e appears in every MST if:

-> e is the UNIQUE minimum weight edge crossing some cut.

Reason:
By Cut Property, the lightest edge across a cut is always chosen.
If it is unique, there is no alternative.

---------------------------------------------------------------
2. Edge in AT LEAST ONE MST
---------------------------------------------------------------
Edge e appears in some MST iff

There is NO path from u to v consisting entirely of edges
with weight strictly smaller than w.

Equivalent:
While running Kruskal, u and v are NOT already connected
using only edges with weight < w.

Otherwise e can never be chosen.

---------------------------------------------------------------
3. Edge in NO MST
---------------------------------------------------------------
Edge e cannot belong to any MST if

There exists a cycle where e is the UNIQUE maximum weight edge.

Reason:
Cycle Property:
The heaviest edge in a cycle is never useful.

Equivalent:
There already exists another path between u and v using only
edges with weight strictly smaller than w.

---------------------------------------------------------------
4. Equal Weight Edges
---------------------------------------------------------------
Equal weights create multiple valid MSTs.

An edge may
- appear in some MSTs
- disappear in others

unless uniqueness conditions hold.

---------------------------------------------------------------
5. Distinct Edge Weights
---------------------------------------------------------------
If every edge weight is distinct

-> MST is UNIQUE.

Every chosen edge is forced.

---------------------------------------------------------------
6. Non-MST Edge
---------------------------------------------------------------
Suppose edge e is NOT chosen.

Adding e creates exactly one cycle.

To keep a spanning tree,
remove the maximum edge on that cycle.

If
    w(e) == maxEdge
then another MST may exist.

If
    w(e) > maxEdge
then e can never improve MST.

---------------------------------------------------------------
7. MST Edge
---------------------------------------------------------------
Removing an MST edge splits the tree into two components.

To reconnect them,
choose the minimum edge crossing that cut.

Useful in:
- Dynamic MST
- Second Best MST

---------------------------------------------------------------
8. Second Best MST
---------------------------------------------------------------
For every non-MST edge:

    Add edge.
    Find maximum edge on MST path.
    Replace it.

Minimum larger answer = Second MST.

---------------------------------------------------------------
9. Critical Edge
---------------------------------------------------------------
Critical edge:

Removing it increases MST weight
or disconnects the graph.

Appears in EVERY MST.

---------------------------------------------------------------
10. Pseudo-Critical Edge
---------------------------------------------------------------
Pseudo-critical edge:

Can appear in SOME MST
but not necessarily all.

Test:
Force include the edge.
If MST weight stays optimal,
it is pseudo-critical.

---------------------------------------------------------------
11. Maximum Edge on MST Path
---------------------------------------------------------------
For any non-MST edge (u,v,w):

maxEdge(path(u,v)) <= w

Otherwise replacing the heavier edge
would produce a cheaper spanning tree.

Equality
    => multiple MSTs possible.

Strict inequality
    => edge never enters MST.

---------------------------------------------------------------
12. Cut Property
---------------------------------------------------------------
For ANY cut:

Minimum crossing edge
belongs to SOME MST.

Unique minimum
belongs to EVERY MST.

---------------------------------------------------------------
13. Cycle Property
---------------------------------------------------------------
For ANY cycle:

Maximum edge
does NOT belong to ANY MST.

Unique maximum
can never appear in an MST.

---------------------------------------------------------------
14. Kruskal Interpretation
---------------------------------------------------------------
Sort edges by weight.

Before processing weight w,

All vertices connected using edges < w
already behave as one component.

Edge (u,v,w):

Different components
    -> can be chosen.

Same component
    -> never needed.

===============================================================
*/