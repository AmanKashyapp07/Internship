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

// Centroid of a tree
// Time: O(N), Space: O(N)

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

// Polynomial rolling hash of a string
// Time: O(|S|), Space: O(1)

ll hashWord(const string &s) {
    ll h = 0;
    for (char c : s) {
        h = (h * P + (c - 'a' + 1)) % MOD;
    }
    return h;
}

// Substring polynomial rolling hash
// Time: O(N) build, O(1) query, Space: O(N)

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

// Disjoint Set Union (DSU) with path compression and union-by-size
// Time: O(alpha(N)), Space: O(N)

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

// Kruskal's algorithm for Minimum Spanning Tree (MST)
// Time: O(E log E), Space: O(V + E)

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

// Prim's algorithm for Minimum Spanning Tree (MST)
// Time: O(E log V), Space: O(V + E)

int prim(int n, const vector<vector<pii>> &g) {
    vector<bool> vis(n + 1, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, 1});
    int total_w = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (vis[u]) continue;
        vis[u] = true;
        total_w += w;

        for (auto &[v, wt] : g[u]) {
            if (!vis[v]) pq.push({wt, v});
        }
    }

    return total_w;
}

// Kahn's algorithm for Topological Sort
// Time: O(V + E), Space: O(V + E)

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
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (int v : g[u]) {
            if (--in_deg[v] == 0) q.push(v);
        }
    }

    return (int)order.size() == n ? order : vi{};
}

// Longest paths and path counts on a DAG
// Time: O(V + E), Space: O(V + E)

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
        int u = q.front();
        q.pop();
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

// Tree rerooting DP for sum of distances from every node
// Time: O(N), Space: O(N)

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

// Tree diameter and node eccentricities
// Time: O(N), Space: O(N)

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

// Tarjan's algorithm for finding bridges
// Time: O(V + E), Space: O(V + E)

// Kosaraju's algorithm for Strongly Connected Components (SCC) and DAG condensation
// Time: O(V + E), Space: O(V + E)

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
        order.push_back(u); // push u after traversing all its descendants
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
        for (int i = n - 1; i >= 0; i--) { // starting from last element
            int u = order[i]; // order[i] is the node with the highest finishing time
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
                if (comp[u] != comp[v]) { // if u and v are in different components, add an edge in the DAG
                    dag[comp[u]].push_back(comp[v]); // edge from component of u to component of v
                }
            }
        } // first run buildDag() after build() to get the DAG of SCCs
        for (auto &v : dag) {
            sort(v.begin(), v.end()); // sort the vacency list of each component in the DAG
            v.erase(unique(v.begin(), v.end()), v.end()); // remove duplicate edges
        } // for removing duplicate edges
    }
};

// Functional Graph decomposition (outdegree = 1) with binary lifting
// Time: O(N log N) build, O(1) jump/query, Space: O(N log N)

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

// Double-hashing tree isomorphism check
// Time: O(N log N), Space: O(N)

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

// Tree centers by leaf trimming
// Time: O(N), Space: O(N)

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
