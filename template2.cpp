/**
 * CSES / OA Competitive Programming Template
 *
 * Sections:
 *  1. Headers & Aliases
 *  2. Macros & Utilities
 *  3. Math & Number Theory
 *  4. Binary Search Helpers
 *  5. Graph (BFS / DFS / Topo / SCC / Bridges)
 *  6. Shortest Paths (Dijkstra / Bellman-Ford / Floyd-Warshall)
 *  7. Minimum Spanning Tree (Kruskal / Prim)
 *  8. Disjoint Set Union (DSU)
 *  9. Fenwick Tree (BIT)
 * 10. Segment Tree (point-update, range-query + lazy)
 * 11. Sparse Table (RMQ)
 * 12. String Algorithms (KMP / Z / Hashing)
 * 13. DP Helpers (LIS / knapsack notes)
 * 14. Geometry Basics
 * 15. Miscellaneous
 * 16. Main
 */

// ─────────────────────────────────────────────
// 1. HEADERS & ALIASES
// ─────────────────────────────────────────────
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using ll   = long long;
using ull  = unsigned long long;
using ld   = long double;
using pii  = pair<int, int>;
using pll  = pair<ll, ll>;
using pil  = pair<int, ll>;
using pli  = pair<ll, int>;
using vi   = vector<int>;
using vll  = vector<ll>;
using vb   = vector<bool>;
using vvi  = vector<vi>;
using vvll = vector<vll>;

// ─────────────────────────────────────────────
// 2. MACROS & UTILITIES
// ─────────────────────────────────────────────
#define all(x)      (x).begin(), (x).end()
#define rall(x)     (x).rbegin(), (x).rend()
#define pb          push_back
#define eb          emplace_back
#define ff          first
#define ss          second
#define mp          make_pair
#define sz(x)       (int)(x).size()
#define YES         cout << "YES\n"
#define NO          cout << "NO\n"

// loop shorthands
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define per(i, a, b) for (int i = (b)-1; i >= (a); --i)
#define each(x, v)   for (auto& x : (v))

const int    INF  = INT_MAX / 2;
const ll     LINF = LLONG_MAX / 2;
const ll     MOD  = 1e9 + 7;
const ll     MOD2 = 998244353;
const double PI   = acos(-1.0);

// Safe modular add / multiply
inline ll addmod(ll a, ll b, ll m = MOD) { return (a + b) % m; }
inline ll mulmod(ll a, ll b, ll m = MOD) { return (__int128)a * b % m; }

// Useful lambdas
auto cmp_second = [](const pii& a, const pii& b){ return a.ss < b.ss; };

// ─────────────────────────────────────────────
// 3. MATH & NUMBER THEORY
// ─────────────────────────────────────────────

// Fast power  (a^b mod m)
ll pw(ll a, ll b, ll m = MOD) {
    ll res = 1; a %= m;
    for (; b > 0; b >>= 1) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
    }
    return res;
}

// Modular inverse (m must be prime)
ll inv(ll a, ll m = MOD) { return pw(a, m - 2, m); }

// Extended GCD  =>  ax + by = gcd(a,b)
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) { x = 1; y = 0; return a; }
    ll x1, y1, g = extgcd(b, a % b, x1, y1);
    x = y1; y = x1 - (a / b) * y1;
    return g;
}

// Sieve of Eratosthenes
const int MAXN = 1e6 + 5;
vector<bool> is_prime(MAXN, true);
vi primes;
void sieve() {
    is_prime[0] = is_prime[1] = false;
    rep(i, 2, MAXN) {
        if (is_prime[i]) {
            primes.pb(i);
            for (ll j = (ll)i * i; j < MAXN; j += i)
                is_prime[j] = false;
        }
    }
}

// Linear sieve + smallest prime factor
vi spf(MAXN, 0);
vi lprimes;
void linear_sieve() {
    rep(i, 2, MAXN) {
        if (!spf[i]) { spf[i] = i; lprimes.pb(i); }
        for (int p : lprimes) {
            if (p > spf[i] || (ll)i * p >= MAXN) break;
            spf[i * p] = p;
        }
    }
}

// Factorize using SPF
vi factorize(int n) {
    vi f;
    while (n > 1) { f.pb(spf[n]); n /= spf[n]; }
    return f;
}

// Factorial / Inverse-factorial precomputation
vector<ll> fact_arr(MAXN), inv_fact(MAXN);
void precompute_factorials(ll m = MOD) {
    fact_arr[0] = 1;
    rep(i, 1, MAXN) fact_arr[i] = fact_arr[i-1] * i % m;
    inv_fact[MAXN-1] = inv(fact_arr[MAXN-1], m);
    per(i, 0, MAXN-1) inv_fact[i] = inv_fact[i+1] * (i+1) % m;
}

// nCr mod prime
ll C(int n, int r, ll m = MOD) {
    if (r < 0 || r > n) return 0;
    return fact_arr[n] % m * inv_fact[r] % m * inv_fact[n-r] % m;
}

// Integer square root
ll isqrt(ll n) { ll s = sqrtl(n); while (s*s > n) --s; while ((s+1)*(s+1) <= n) ++s; return s; }

// GCD / LCM
ll gcd(ll a, ll b) { return b ? gcd(b, a%b) : a; }
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

// ─────────────────────────────────────────────
// 4. BINARY SEARCH HELPERS
// ─────────────────────────────────────────────
// lower_bound / upper_bound are in STL; use these wrappers for clarity.

// First index where arr[i] >= val
int bs_lower(const vi& arr, int val) {
    return (int)(lower_bound(all(arr), val) - arr.begin());
}
// First index where arr[i] > val
int bs_upper(const vi& arr, int val) {
    return (int)(upper_bound(all(arr), val) - arr.begin());
}

// Binary search on answer (integer)
//   lo = smallest possible, hi = largest possible
//   check(mid) returns true when mid is feasible
// ll binary_search_ans(ll lo, ll hi, auto check) {
//     while (lo < hi) {
//         ll mid = lo + (hi - lo) / 2;
//         if (check(mid)) hi = mid; else lo = mid + 1;
//     }
//     return lo;
// }

// ─────────────────────────────────────────────
// 5. GRAPH
// ─────────────────────────────────────────────
const int MAXV = 2e5 + 5;
vector<pii> adj[MAXV];      // weighted: adj[u] = {v, w}
vi          radj[MAXV];     // reverse graph (unweighted)
vi          order_topo;
bool        visited[MAXV];
int         comp[MAXV];     // SCC / component id

// BFS (unweighted shortest path)
vi bfs(int src, int n) {
    vi dist(n+1, -1);
    queue<int> q;
    dist[src] = 0; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : adj[u]) {
            if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
        }
    }
    return dist;
}

// DFS iterative
void dfs(int src) {
    stack<int> st;
    st.push(src); visited[src] = true;
    while (!st.empty()) {
        int u = st.top(); st.pop();
        for (auto [v, w] : adj[u]) {
            if (!visited[v]) { visited[v] = true; st.push(v); }
        }
    }
}

// Topological sort (Kahn's BFS) — returns empty if cycle exists
vi topo_sort(int n) {
    vi indeg(n+1, 0);
    rep(u, 1, n+1) for (auto [v, w] : adj[u]) indeg[v]++;
    queue<int> q;
    rep(u, 1, n+1) if (!indeg[u]) q.push(u);
    vi res;
    while (!q.empty()) {
        int u = q.front(); q.pop(); res.pb(u);
        for (auto [v, w] : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return (sz(res) == n) ? res : vi{};  // empty => cycle
}

// Cycle detection in undirected graph (DFS coloring)
// 0=unvisited 1=in-stack 2=done
vi color_g(MAXV, 0);
bool has_cycle_directed(int u, int par = -1) {
    color_g[u] = 1;
    for (auto [v, w] : adj[u]) {
        if (color_g[v] == 1) return true;
        if (!color_g[v] && has_cycle_directed(v, u)) return true;
    }
    color_g[u] = 2;
    return false;
}

// Bipartite check (2-coloring)
vi bip_color(MAXV, -1);
bool bipartite(int src) {
    queue<int> q; q.push(src); bip_color[src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : adj[u]) {
            if (bip_color[v] == -1) { bip_color[v] = 1 - bip_color[u]; q.push(v); }
            else if (bip_color[v] == bip_color[u]) return false;
        }
    }
    return true;
}

// Bridges (Tarjan)
int disc[MAXV], low_val[MAXV], timer_g = 0;
vector<pii> bridges;
void dfs_bridge(int u, int par) {
    disc[u] = low_val[u] = ++timer_g;
    for (auto [v, w] : adj[u]) {
        if (!disc[v]) {
            dfs_bridge(v, u);
            low_val[u] = min(low_val[u], low_val[v]);
            if (low_val[v] > disc[u]) bridges.pb({u, v});
        } else if (v != par) {
            low_val[u] = min(low_val[u], disc[v]);
        }
    }
}

// Articulation points (Tarjan)
bool ap[MAXV];
void dfs_ap(int u, int par, int &cnt) {
    disc[u] = low_val[u] = ++timer_g;
    int children = 0;
    for (auto [v, w] : adj[u]) {
        if (!disc[v]) {
            ++children; dfs_ap(v, u, cnt);
            low_val[u] = min(low_val[u], low_val[v]);
            if (par == -1 && children > 1) ap[u] = true;
            if (par != -1 && low_val[v] >= disc[u]) ap[u] = true;
        } else if (v != par) {
            low_val[u] = min(low_val[u], disc[v]);
        }
    }
}

// Kosaraju SCC (two-pass)
void dfs1(int u) { visited[u]=true; for (int v:radj[u]) if(!visited[v]) dfs1(v); order_topo.pb(u); }
void dfs2(int u, int c) { comp[u]=c; for(auto [v,w]:adj[u]) if(!comp[v]) dfs2(v,c); }
// call: fill radj, run dfs1 for all unvisited, then dfs2 in reverse order_topo

// ─────────────────────────────────────────────
// 6. SHORTEST PATHS
// ─────────────────────────────────────────────

// Dijkstra (non-negative weights)
vll dijkstra(int src, int n) {
    vll dist(n+1, LINF);
    priority_queue<pll, vector<pll>, greater<pll>> pq;
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// Bellman-Ford (handles negative edges; detects negative cycle)
// edges: {u, v, w}
vll bellman_ford(int src, int n, vector<tuple<int,int,ll>>& edges) {
    vll dist(n+1, LINF); dist[src] = 0;
    rep(i, 0, n-1) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != LINF && dist[u]+w < dist[v])
                dist[v] = dist[u]+w;
        }
    }
    // Check negative cycle
    for (auto [u, v, w] : edges) {
        if (dist[u] != LINF && dist[u]+w < dist[v])
            dist[v] = -LINF;  // mark reachable via negative cycle
    }
    return dist;
}

// Floyd-Warshall  (all-pairs shortest path)
// d[i][j] = direct edge weight or INF; d[i][i]=0
void floyd_warshall(vector<vll>& d, int n) {
    rep(k, 0, n) rep(i, 0, n) rep(j, 0, n) {
        if (d[i][k] < LINF && d[k][j] < LINF)
            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    }
}

// ─────────────────────────────────────────────
// 7. MINIMUM SPANNING TREE
// ─────────────────────────────────────────────

// Kruskal (uses DSU below; edges sorted by weight)
struct Edge { int u, v; ll w; };

// Prim (dense graphs — use adj matrix; sparse — use priority queue)
ll prim(int n) {   // returns MST weight; uses global adj[]
    vll key(n+1, LINF); vb in_mst(n+1, false);
    priority_queue<pll, vector<pll>, greater<pll>> pq;
    key[1] = 0; pq.push({0, 1});
    ll total = 0;
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true; total += d;
        for (auto [v, w] : adj[u])
            if (!in_mst[v] && w < key[v]) { key[v] = w; pq.push({w, v}); }
    }
    return total;
}

// ─────────────────────────────────────────────
// 8. DISJOINT SET UNION (DSU)
// ─────────────────────────────────────────────
struct DSU {
    vi par, rnk, sz_d;
    int components;
    DSU(int n) : par(n+1), rnk(n+1, 0), sz_d(n+1, 1), components(n) {
        iota(all(par), 0);
    }
    int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        --components;
        if (rnk[a] < rnk[b]) swap(a, b);
        par[b] = a; sz_d[a] += sz_d[b];
        if (rnk[a] == rnk[b]) ++rnk[a];
        return true;
    }
    bool connected(int a, int b) { return find(a) == find(b); }
    int  size(int a)             { return sz_d[find(a)]; }
};



// ─────────────────────────────────────────────
// 13. DP HELPERS
// ─────────────────────────────────────────────

// LIS — O(n log n), returns length
int lis_length(const vi& a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(all(dp), x);
        if (it == dp.end()) dp.pb(x);
        else *it = x;
    }
    return sz(dp);
}

// ─────────────────────────────────────────────
// 15. MISCELLANEOUS
// ─────────────────────────────────────────────

// Next permutation loop  (iterate all perms of v)
// do { ... } while (next_permutation(all(v)));

// __builtin shortcuts
// __builtin_popcount(x)   — number of set bits (int)
// __builtin_popcountll(x) — number of set bits (long long)
// __builtin_ctz(x)        — count trailing zeros
// __builtin_clz(x)        — count leading zeros

// Prefix sums (1-indexed)
vll prefix(const vi& a) {
    int n = sz(a); vll pre(n+1, 0);
    rep(i, 0, n) pre[i+1] = pre[i] + a[i];
    return pre;
}

// 2D prefix sums (1-indexed)
vector<vll> prefix2d(vector<vi>& g) {
    int n = sz(g), m = sz(g[0]);
    vector<vll> pre(n+1, vll(m+1, 0));
    rep(i, 1, n+1) rep(j, 1, m+1)
        pre[i][j] = g[i-1][j-1] + pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1];
    return pre;
}
// query rect [r1,c1]x[r2,c2] (1-indexed):
// pre[r2][c2] - pre[r1-1][c2] - pre[r2][c1-1] + pre[r1-1][c1-1]

// Two pointers / sliding window skeleton
// int l = 0; for (int r = 0; r < n; ++r) { /* add a[r] */ while (/* invalid */) { /* remove a[l]*/ ++l; } /* update answer */ }

// ─────────────────────────────────────────────
// 16. MAIN
// ─────────────────────────────────────────────
void solve() {
    // --- write solution here ---
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 1;
    // cin >> t;      // uncomment for multi-test
    while (t--) solve();

    return 0;
}