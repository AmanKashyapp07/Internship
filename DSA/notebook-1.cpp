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
using ll = int;
using ull = unsigned int;
using pii = pair<int, int>;
using vvi = vector<vector<int>>
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
  int total_weight = kruskal(n, edges, mst_edges);
*/
struct Edge {
    int u, v;
    int w;
};

bool comparator(const Edge &a, const Edge &b) {
    return a.w < b.w;
}

int kruskal(int n, vector<Edge>& edges, vector<Edge>& mst_edges) {
    DSU dsu(n);
    sort(edges.begin(), edges.end(), comparator);
    int mst_weight = 0;
    mst_edges.clear();
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) { // if u and v are not already connected
            mst_weight += e.w;
            mst_edges.push_back(e);
        }
    }
    return mst_weight;
}

int prim(int n, vector<vector<pair<int, int>>>& g) {
    vector<bool> in_mst(n + 1, false);
    using T = pair<int, int>; // {weight, vertex}
    priority_queue<T, vector<T>, greater<T>> pq;
    pq.push({0, 1});
    int mst_weight = 0;

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


pair<vector<int>, vector<int>> solve_dag(int n, vector<vector<pair<int, int>>>& g, int src, int start_node = 1) {
    vector<int> in_deg(g.size(), 0);
    for (int u = start_node; u < start_node + n; u++) {
        for (auto& e : g[u]) in_deg[e.first]++;
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
            if (--in_deg[e.first] == 0) q.push(e.first);
        }
    }

    const int INF = 1e9;
    vector<int> dist(g.size(), -INF);
    vector<int> paths(g.size(), 0);
    dist[src] = 0;
    paths[src] = 1;

    for (int u : order) {
        if (dist[u] == -INF) continue;
        for (auto& e : g[u]) {
            if (dist[e.first] < dist[u] + e.second) {
                dist[e.first] = dist[u] + e.second;
                paths[e.first] = paths[u];
            }
            else if (dist[e.first] == dist[u] + e.second) {
                paths[e.first] = (paths[e.first] + paths[u]) % MOD;
            }
        }
    }

    return {dist, paths};
}

class TreeDistances {
    int n;
    const vector<vector<int>>& g;
    vector<int> subtree_sz;
    vector<int> total_dist;

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

    vector<int> solve(int root = 1) {
        dfs_size(root, 0);
        dfs_root(root, 0, 0);
        reroot(root, 0);
        return total_dist;
    }
};

void dfs(int u, int p, vector<vector<int>>& g, vector<int>& dist) {
    for (int v : g[u]) {
        if (v == p) continue;
        dist[v] = dist[u] + 1;
        dfs(v, u, g, dist);
    }
}

pair<int, vector<int>> tree_diameter(int n, vector<vector<int>>& g) {
    vector<int> d1(n + 1), d2(n + 1);

    dfs(1, 0, g, d1);
    int a = max_element(d1.begin() + 1, d1.end()) - d1.begin();

    fill(d1.begin(), d1.end(), 0);
    dfs(a, 0, g, d1);
    int b = max_element(d1.begin() + 1, d1.end()) - d1.begin();

    int diameter = d1[b];

    dfs(b, 0, g, d2);

    vector<int> ecc(n + 1);
    for (int i = 1; i <= n; i++) {
        ecc[i] = max(d1[i], d2[i]);
    }

    return {diameter, ecc};
} // edd[i] = eccentricity of node i, which is the maximum distance from node i to any other node in the tree. The diameter of the tree is the maximum eccentricity among all nodes.


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
                sccs.emplace_back(); // create a new component
                dfs2(u, sccs.size() - 1);
            }
        }
        return sccs;
    }
    vector<vector<int>> get_dag() {
        dag.resize(sccs.size());
        for (int u = 1; u <= n; u++) 
            for (int v : g[u]) if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);

        for (auto& neighbors : dag) {
            sort(neighbors.begin(), neighbors.end());
            neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
        } // this removes duplicate edges in the DAG representation of the SCCs, ensuring that each edge between components is represented only once.
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
struct FunctionalGraph {
    int n;
    const int LOG = 20;
    vector<int> cyc, dist, indeg;
    vector<vector<int>> rev, up;
    // cyc[i] = length of the cycle for vertex i (or -1 if not in a cycle)
    // dist[i] = distance from vertex i to the cycle (0 if in a cycle)
    // indeg[i] = indegree of vertex i
    // up[i][j] = 2^j-th ancestor of vertex i
     void dfs(int u) {
        for (int v : rev[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                dfs(v);
            }
    }

    FunctionalGraph(vector<int>& to) {
        n = to.size();
        cyc.assign(n, -1);
        dist.assign(n, -1);
        indeg.assign(n, 0);
        rev.assign(n, {});
        up.assign(n, vector<int>(LOG));

        for (int i = 0; i < n; i++) {
            up[i][0] = to[i];
            rev[to[i]].push_back(i);
            indeg[to[i]]++;
        }

        for (int j = 1; j < LOG; j++)
            for (int i = 0; i < n; i++) {
                int p = up[i][j - 1];
                up[i][j] = up[p][j - 1];
            }

        vector<int> alive(n, 1), vis(n);
        queue<int> q;

        for (int i = 0; i < n; i++)
            if (!indeg[i]) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            alive[u] = 0;
            if (--indeg[to[u]] == 0) q.push(to[u]);
        }

        for (int i = 0; i < n; i++) {
            if (!alive[i] || vis[i]) continue;
            int u = i, len = 0;
            do vis[u] = 1, len++, u = to[u];
            while (u != i);

            u = i;
            do {
                cyc[u] = len;
                dist[u] = 0;
                u = to[u];
            } while (u != i);
        }

        for(int i = 0; i < n; i++) if (cyc[i] != -1) dfs(i);
    }

    int jump(int u, int k) {
        for (int j = 0; j < LOG; j++)
            if (k & (1 << j)) u = up[u][j];
        return u;
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
        for (int v : g[u]) if(v!=p) child.push_back(dfs(v, u));
        sort(child.begin(), child.end());
        if (!mp.count(child)) mp[child] = nxt++;
 
        return id[u] = mp[child];
    }
 
    int getID(int root) {
        return dfs(root, 0);
    }
};

vector<int> find_centers(int n, vector<vector<int>>& g) {
    if (n == 1) return {1};

    vector<int> deg(n + 1), leaves;
    for (int i = 1; i <= n; i++)
        if ((deg[i] = g[i].size()) == 1) leaves.push_back(i);

    while (n > 2) {
        n -= leaves.size();
        vector<int> nxt;
        for (int u : leaves)
            for (int v : g[u])
                if (--deg[v] == 1) nxt.push_back(v);
        leaves = nxt;
    }

    return leaves;
}// returns the center(s) of the tree. A tree can have either one center (if it has an odd number of nodes) or two centers (if it has an even number of nodes). The center(s) are the node(s) that minimize the maximum distance to any other node in the tree.


/*
==================== GRAPH / TREE FORMULAE & MST CHEAT SHEET ====================

1. TREE & CYCLE FORMULAE:
   - Edges = N - 1 | Sum of degrees = 2(N - 1) | Leaves = 2 + Σ(deg(v) - 2) for deg(v) >= 2
   - Min edges to connect = components - 1 | Cyclomatic Number (independent cycles) = M - N + components
   - Simple graph max edges: Undirected = N(N - 1)/2 | Directed = N(N - 1)
   - Eulerian: Circuit <=> all degrees even | Path <=> 0 or 2 odd vertices | Subgraphs = 2^(M - N + comp)
   - Bipartite <=> No odd cycle | Complete Bipartite K(a,b) edges = a * b
   - Complete Graph K_N: Edges = N(N - 1)/2 | Degree = N - 1
   - Handshaking: Σ deg = 2M (number of odd-degree vertices is always even)
   - Tree Centre: 1 or 2 nodes | Tree Centroid: sizes of components after removal <= N/2
   - Sum(subtree sizes) = N + Sum(depth)

2. MINIMUM SPANNING TREE (MST) FUNDAMENTALS:
   - MST exists only if the graph is connected; otherwise, it's a Minimum Spanning Forest (MSF).
   - Adding any non-MST edge creates exactly one cycle; remove the heaviest edge on it to restore MST.
   - Removing an MST edge splits the tree; reconnect using the lightest edge crossing the cut.
   - Second Best MST: For each non-MST edge (u, v, w), add it, query max edge on MST path(u,v) (via HLD/Binary Lifting), and replace it. Minimum weight obtained is Second Best MST.
   - Shortest Path Tree (SPT) vs MST: SPT minimizes path from a single source; MST minimizes total weight.
   - MST works perfectly with negative weights. For directed graphs, use Minimum Spanning Arborescence (Edmonds').

3. DSU & RECOGNITION CLUES:
   - Always implement DSU with Path Compression and Union by Size/Rank -> O(alpha(N)) per operation.
   - Key clues to use MST: "connect all cities/islands", "minimum cable/road/infrastructure cost", "exactly N-1 edges", "remove redundant expensive edges".
   - Minimax Path: Build MST; the answer is the maximum weight edge on the path between u and v in the MST.
=========================================================================================
*/

/*
======================== MST EDGE THEORY ========================
1. Edge in EVERY MST: e = (u,v,w) is the UNIQUE minimum weight edge crossing some cut.
2. Edge in SOME MST: There is NO path from u to v using only edges with weight strictly smaller than w.
3. Edge in NO MST: There exists a cycle where e is the UNIQUE maximum weight edge (i.e. u-v path exists with edges < w).
4. Equal Weights: Create multiple valid MSTs unless uniqueness conditions hold.
5. Distinct Weights: If every edge weight is distinct, the MST is UNIQUE.
6. Non-MST Edge: Adding it creates exactly one cycle. To keep a spanning tree, remove the maximum edge on that cycle.
7. MST Edge: Removing it splits the tree. Reconnect using the minimum edge crossing the cut (useful in Dynamic MST / Second Best MST).
8. Second Best MST: For each non-MST edge, add it, find the max edge on the MST path, and replace it. Minimum resulting weight is Second MST.
9. Critical Edge: Removing it increases MST weight (or disconnects graph). Appears in EVERY MST.
10. Pseudo-Critical Edge: Can appear in SOME MST. If forced, the MST weight remains optimal.
11. Max Edge on MST Path: For any non-MST edge (u,v,w): maxEdge(path(u,v)) <= w. Strict inequality means the edge never enters MST.
12. Cut Property: For any cut, the minimum crossing edge belongs to some MST; unique minimum belongs to every MST.
13. Cycle Property: For any cycle, the maximum edge does not belong to any MST; unique maximum can never appear in an MST.
14. Kruskal: Sort edges. If u-v are already connected using edges < w, (u,v,w) is never needed; otherwise, it can be chosen.
=================================================================
*/