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


pair<vi, vi> dag_(int n, vector<vector<pii>>& g, int src, int start_node = 1) {
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
    int timer;
    vector<int> tin, tout, low, parent;
    vector<pair<int, int>> bridges;

    void dfs(int u, int p, vector<vector<int>> &g) {
        parent[u] = p;
        tin[u] = low[u] = ++timer;

        for (int v : g[u]) {
            if (v == p) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u, g);
                low[u] = min(low[u], low[v]);

                if (low[v] > tin[u])
                    bridges.push_back({u, v});
            }
        }

        tout[u] = timer;
    }

    void build(vector<vector<int>> &g) {
        int n = g.size();

        timer = 0;
        tin.assign(n, 0);
        tout.assign(n, 0);
        low.assign(n, 0);
        parent.assign(n, -1);
        bridges.clear();

        for (int i = 0; i < n; i++)
            if (!tin[i])
                dfs(i, -1, g);
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    // Returns true if removing bridge (u,v) disconnects a and b.
    // Assumes (u,v) is a bridge.
    bool separated(int u, int v, int a, int b) {
        // Make v the DFS child.
        if (parent[u] == v)
            swap(u, v);

        bool sideA = isAncestor(v, a);
        bool sideB = isAncestor(v, b);

        return sideA != sideB;
    };
};

struct Articulation {
    int timer;
    vector<int> tin, tout, low, par;
    vector<vector<int>> child;

    void dfs(int u, int p, vector<vector<int>>& g) {
        par[u] = p;
        tin[u] = low[u] = ++timer;

        for (int v : g[u]) {
            if (v == p) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                child[u].push_back(v);
                dfs(v, u, g);
                low[u] = min(low[u], low[v]);
            }
        }

        tout[u] = timer;
    }

    void build(vector<vector<int>>& g) {
        int n = g.size();
        timer = 0;
        tin.assign(n, 0);
        tout.assign(n, 0);
        low.assign(n, 0);
        par.assign(n, -1);
        child.assign(n, {});

        for (int i = 0; i < n; i++)
            if (!tin[i])
                dfs(i, -1, g);
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    // returns:
    // -1 : parent side
    // child vertex : separating child subtree
    int findChild(int u, int x) {
        int l = 0, r = child[u].size() - 1;

        while (l <= r) {
            int mid = (l + r) / 2;
            int v = child[u][mid];

            if (tin[x] < tin[v]) {
                r = mid - 1;
            } else if (isAncestor(v, x)) {
                return v;
            } else {
                l = mid + 1;
            }
        }

        return -1;
    }

    int component(int cut, int x) {
        if (cut == x) return -1;

        if (par[cut] == x) return cut;

        if (!isAncestor(cut, x)) return par[cut];

        int child = findChild(cut, x);
        return child;
    }

    // true => disconnected after removing cut
    bool separated(int cut, int a, int b) {

        if (a == cut || b == cut)
            return true;

        int ca = component(cut, a);
        int cb = component(cut, b);

        return ca != cb;
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
    while (true) {
        slow = to[slow]; fast = to[to[fast]];
        if (comp[slow] != -1) return {};
        if (slow == fast) break;
    }
    slow = start;
    while (slow != fast) slow = to[slow], fast = to[fast];
    vi cycle; int curr = slow;
    do { cycle.pb(curr); curr = to[curr]; } while (curr != slow);
    return cycle;
}

struct FunctionalGraph {
    int n, LOG = 20;
    vi dist, cyc, comp, pos;
    vvi up, radj;

    void dfs(int u) {
        for (int v : radj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                cyc[v] = cyc[u];
                comp[v] = comp[u];
                dfs(v);
            }
        }
    }

    FunctionalGraph(const vi &to) : n(to.size()), dist(n, -1), cyc(n, -1), comp(n, -1), pos(n, -1), up(n, vi(LOG)), radj(n) {
        for (int i = 0; i < n; i++) up[i][0] = to[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i < n; i++) up[i][j] = up[up[i][j - 1]][j - 1];

        for (int i = 0; i < n; i++) radj[to[i]].pb(i);

        int cid = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] != -1) continue;
            vi cycle = getCycleFloyd(to, comp, i);
            if (cycle.empty()) continue;
            int len = cycle.size();
            for (int j = 0; j < len; j++) {
                int u = cycle[j];
                dist[u] = 0; cyc[u] = len; comp[u] = cid; pos[u] = j;
            }
            for (int u : cycle) dfs(u);
            cid++;
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
// dist[u] → distance from u to its cycle (0 if on cycle).
// cyc[u] → length of the cycle containing u (works for every node, not just cycle nodes).
// comp[u] → connected component ID.
// pos[u] → index of u inside its cycle (-1 for tree nodes).
// jump(u, k) → node reached after k teleports.
// query(a, b) → minimum teleports from a to b, or -1.
// distance_in_cycle(a, b) → distance along the cycle if both are cycle nodes.

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