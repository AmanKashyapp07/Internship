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
using vvi = vector<vector<int>>;
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



struct Centroid{
    int n;
    vvi g;
    vi subsize;

    Centroid(int n, const vvi& g) : n(n), g(g), subsize(n + 1, 0) {}

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
  vi z = z_array(s);
*/
vi z_array(string s) {
    int n = s.size();
    vi z(n, 0);
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

vi pi_array(string s) {
    vi z = z_array(s);
    int n = s.size();
    vi pi(n, 0);
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
  pair<vi, vi>: {d1, d2}
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
    vi p, sz;
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

/*
Kruskal's Algorithm (MST)
Time Complexity: O(E log E)
Usage:
  vector<Edge> edges, mst_edges;
  int total_weight = kruskal(n, edges, mst_edges);
*/
struct Edge {
    int u, v, w;
};

bool comparator(const Edge &a, const Edge &b) {
    return a.w < b.w;
}

int kruskal(int n, vector<Edge>& edges, vector<Edge>& mst_edges) {
    DSU dsu(n);
    sort(edges.begin(), edges.end(), comparator);
    int weight = 0;
    mst_edges.clear();
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) { // if u and v are not already connected
            weight += e.w;
            mst_edges.push_back(e);
        }
    }
    return weight;
}

int prim(int n, vector<vector<pii>>& g) {
    vector<bool> in_mst(n + 1, false);
    using T = pii; // {weight, vertex}
    priority_queue<T, vector<T>, greater<T>> pq;
    pq.push({0, 1});
    int weight = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true;
        weight += w;

        for (auto& [v, weight] : g[u]) {
            if (!in_mst[v]) {
                pq.push({weight, v});
            }
        }
    }
    return weight;
}

/*
Topological Sort (Kahn's and DFS)
Returns topological order of nodes. Returns empty vector if cycle exists.
*/
vi kahn(int n, vvi& g, int start_node = 1) {
    vi in_deg(g.size(), 0);
    for (int u = start_node; u < start_node + n; u++) {
        for (int v : g[u]) in_deg[v]++;
    }
    queue<int> q;
    for (int i = start_node; i < start_node + n; i++) {
        if (in_deg[i] == 0) q.push(i);
    }
    vi order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--in_deg[v] == 0) q.push(v);
        }
    }
    return order.size() == n ? order : vi{};
}

/*
DAG Solver (Shortest/Longest Paths, Path Counts)
Returns {dist, path_counts} in O(V + E) using Kahn's topological sort.
*/


pair<vi, vi> solve_dag(int n, vector<vector<pii>>& g, int src, int start_node = 1) {
    vi order = kahn(n, g, start_node);

    const int INF = 1e9;
    vi dist(g.size(), -INF); // dist[i] = longest distance from src to i
    vi paths(g.size(), 0); // paths[i] = number of longest paths from src to i
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
    const vvi& g;
    vi sz;
    vi dist;

public:
    TreeDistances(int nodes, const vvi& graph)
        : n(nodes), g(graph),
          sz(nodes + 1, 0),
          dist(nodes + 1, 0) {}

    void dfs(int u, int p, int depth) {
        sz[u] = 1;
        dist[1] += depth;

        for (int v : g[u]) {
            if (v == p) continue;
            dfs(v, u, depth + 1);
            sz[u] += sz[v];
        }
    }

    void reroot(int u, int p) {
        for (int v : g[u]) {
            if (v == p) continue;
            dist[v] = dist[u] + n - 2 * sz[v];
            reroot(v, u);
        }
    }

    vi solve(int root = 1) {
        dist[1] = 0;
        dfs(root, 0, 0);
        reroot(root, 0);
        return dist;
    }
};

void dfs(int u, int p, vvi& g, vi& dist) {
    for (int v : g[u]) {
        if (v == p) continue;
        dist[v] = dist[u] + 1;
        dfs(v, u, g, dist);
    }
} // dist[i]=distance from 1 to i

pair<int, vi> tree_diameter(int n, vvi& g) {
    vi d1(n + 1), d2(n + 1);

    dfs(1, 0, g, d1);
    int a = max_element(d1.begin() + 1, d1.end()) - d1.begin(); // find the farthest node from node 1, which is one endpoint of the diameter

    fill(d1.begin(), d1.end(), 0);
    dfs(a, 0, g, d1);
    int b = max_element(d1.begin() + 1, d1.end()) - d1.begin(); // find the farthest node from node a, which is the other endpoint of the diameter

    int diameter = d1[b]; // the length of the longest path in the tree, which is the distance between nodes a and b

    dfs(b, 0, g, d2); // compute distances from node b to all other nodes in the tree

    vi ecc(n + 1);
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
    int t; vi tin, low; vector<pair<int,int>> res;
    void dfs(int u, int p, vvi& g) {
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
    vector<pair<int,int>> get(vvi& g) {
        int n = g.size(); t = 0; tin.assign(n, 0); low.resize(n); res.clear();
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, g);
        return res;
    }
};

struct Articulation {
    int t; vi tin, low; vi res;
    void dfs(int u, int p, vvi& g) {
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
    vi get(vvi& g) {
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

void removeDuplicates(vi&nums){
    sort(all(nums));
    nums.erase(unique(all(nums)), nums.end());
}
struct SCC {
    int n;
    vector<vector<int>> g, rg, sccs, dag;
    vector<int> comp, order;
    vector<bool> vis;

    SCC(int n) : n(n), g(n), rg(n), comp(n, -1), vis(n) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        rg[v].push_back(u);
    }

    void dfs1(int u) {
        vis[u] = true;
        for (int v : g[u]) if (!vis[v]) dfs1(v);
        order.push_back(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        sccs.back().push_back(u);
        for (int v : rg[u]) if (comp[v] == -1) dfs2(v, c);
    }

    // Run this first to identify all components
    void build() {
        for (int i = 0; i < n; i++) if (!vis[i]) dfs1(i);
        for (int i = n - 1; i >= 0; i--) {
            int u = order[i];
            if (comp[u] == -1) {
                sccs.emplace_back();
                dfs2(u, sccs.size() - 1);
            }
        }
    }

    // Optional: Builds the condensation DAG (call build() first)
    void build_dag() {
        dag.resize(sccs.size());
        for (int u = 0; u < n; u++) {
            for (int v : g[u]) {
                if (comp[u] != comp[v]) {
                    dag[comp[u]].push_back(comp[v]);
                }
            }
        }
        for (auto& adj : dag) {
            removeDuplicates(adj);
        }
    }
};



struct HierholzerUndirected {
    int n, m = 0;
    vvi g;
    vi used;
    vi deg, path;

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

    vi getEulerianCircuit(int start) {
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
    vi getEulerianPath(int start, int end) {
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
    vvi g;
    vi indeg, outdeg;
    vi path;

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

    vi getEulerianCircuit(int start) {
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

    vi getEulerianPath(int start, int end) {
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

// Returns empty vector if 'start' leads into an already processed component
vi getCycleFloyd(const vi &to, const vi &comp, int start) {
    int slow = start, fast = start;
    
    while(true){
        slow = to[slow];
        fast = to[to[fast]];
        if(comp[slow] != -1) return {}; // already processed component
        if (slow == fast) break;
    }

    // 2. Phase 2: Find cycle entry point
    slow = start;
    while (slow != fast) {
        slow = to[slow];
        fast = to[fast];
    }

    // 3. Phase 3: Collect nodes in the cycle
    vi cycle;
    int curr = slow;
    while(true){
        cycle.push_back(curr);
        curr = to[curr];
        if(curr == slow) break;
    }

    return cycle;
} // returns vector of nodes in the cycle, or empty vector if 'start' leads into an already processed component. already processed component means that the node is part of a cycle that has already been identified in a previous iteration, so we don't need to process it again.

struct FunctionalGraph
{
    int n, LOG = 20;
    vector<int> dist, cyc, comp, pos;
    vector<vector<int>> up;

    void dfs(int u, int p, const vector<int> &to, const vector<vector<int>> &radj)
    {
        for (int v : radj[u])
        {
            if (v == p) continue;
            dist[v] = dist[u] + 1;
            comp[v] = comp[u];
            dfs(v, u, to, radj);
        }
    }

    FunctionalGraph(const vector<int> &to)
    {
        n = to.size();
        dist.assign(n, -1); // dist[i] = distance from node i to the cycle (or -1 if not reachable)
        cyc.assign(n, -1); // cyc[i] = length of the cycle containing node i (or -1 if not in a cycle)
        comp.assign(n, -1); // comp[i] = component ID of the connected component containing node i
        pos.assign(n, -1); // pos[i] = position of node i in its cycle (0-indexed, or -1 if not in a cycle)
        up.assign(n, vector<int>(LOG));

        // 1. Binary Lifting table
        for (int i = 0; i < n; i++)
            up[i][0] = to[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i < n; i++)
                up[i][j] = up[up[i][j - 1]][j - 1];

        // 2. Cycle detection via Floyd's algorithm + Tree processing
        int comp_id = 0;
        vector<vector<int>> radj(n);
        for (int i = 0; i < n; i++)
            radj[to[i]].push_back(i);

        for (int i = 0; i < n; i++)
        {
            if (comp[i] != -1) continue;

            vi cycle = getCycleFloyd(to, comp, i);
            if (cycle.empty()) continue; // Leads to an already processed component

            int len = cycle.size();
            for (int j = 0; j < len; j++)
            {
                int u = cycle[j];
                cyc[u] = len;
                dist[u] = 0;
                comp[u] = comp_id;
                pos[u] = j;
            }

            dfs(i, -1, to, radj); // starting from a node in the cycle, process all nodes that lead into this cycle and assign their distances and component IDs
        }
    }

    int jump(int u, int k)
    {
        for (int j = 0; j < LOG; j++)
            if ((k >> j) & 1)
                u = up[u][j];
        return u;
    }

    int dis_cycle_nodes(int a, int b)
    {
        if (comp[a] != comp[b]) // if a and b are not in the same component, then they cannot be in the same cycle
            return -1;
        if (dist[a] != 0 || dist[b] != 0) // if either a or b is not in the cycle, then they are not both cycle nodes
            return -1;
        return (pos[b] - pos[a] + cyc[a]) % cyc[a]; // if both a and b are in the same cycle, then the distance from a to b is the difference in their positions in the cycle, modulo the cycle length
    }

    // Distance from a -> b (-1 if unreachable)
    int query(int a, int b)
    {
        if (comp[a] != comp[b]) return -1;

        bool a_cyc = (dist[a] == 0), b_cyc = (dist[b] == 0);

        if (a_cyc && b_cyc) return dis_cycle_nodes(a, b); 
        if (a_cyc && !b_cyc) return -1;
        if (!a_cyc && !b_cyc) // if both are in the tree leading to the cycle, then we can check if b is reachable from a by jumping up the tree
        {
            int d = dist[a] - dist[b];
            return (d >= 0 && jump(a, d) == b) ? d : -1;
        }
        // if a is in the tree and b is in the cycle, then we can jump from a to the cycle and then compute the distance to b
        // Tree node -> Cycle node
        int entry = jump(a, dist[a]);
        return dist[a] + dis_cycle_nodes(entry, b);
    }
};

const long long MOD1 = 1000000007;
const long long MOD2 = 1000000009;
const long long BASE1 = 313;
const long long BASE2 = 317;
const int MAX_LEN = 200005; 
long long pow1[MAX_LEN], pow2[MAX_LEN];

void precompute() {
    pow1[0] = pow2[0] = 1;
    for (int i = 1; i < MAX_LEN; i++) {
        pow1[i] = (pow1[i - 1] * BASE1) % MOD1;
        pow2[i] = (pow2[i - 1] * BASE2) % MOD2;
    }
}

vector<long long> dfs(int u, int p, const vvi& graph) {
    vector<vector<long long>> childs;

    for (int v : graph[u]) {
        if (v != p) {
            childs.push_back(dfs(v, u, graph));
        }
    }

    sort(childs.begin(), childs.end());

    long long h1 = 1, h2 = 1;
    int total_len = 1;

    for (auto &child : childs) {
        long long child_h1 = child[0];
        long long child_h2 = child[1];
        int len = child[2];

        h1 = (h1 * pow1[len] + child_h1) % MOD1;
        h2 = (h2 * pow2[len] + child_h2) % MOD2;
        total_len += len;
    }

    h1 = (h1 * BASE1 + 2) % MOD1;
    h2 = (h2 * BASE2 + 2) % MOD2;
    total_len++;

    return {h1, h2, total_len};
}

vi centers(int n, vvi& g) {
    if (n == 1) return {1};

    vi deg(n + 1), leaves;
    for (int i = 1; i <= n; i++)
        if ((deg[i] = g[i].size()) == 1) leaves.push_back(i);

    while (n > 2) {
        n -= leaves.size();
        vi nxt;
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
*/q