
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

/*
================================================================================
          CHEAT SHEET: ITERATIVE VS. RECURSIVE SEGMENT TREE
================================================================================

Choose the right tool based on the operations your problem demands:

1. WHEN TO USE AN ITERATIVE SEGMENT TREE (The Speed Daemon)
   ------------------------------------------------------------
   ✔ Best For: Standard point updates and associative range queries 
                (Range Sum, Range Min/Max, Range GCD).
   ✔ Why: It is roughly 2x-3x faster than recursive implementations, uses half 
          the memory (2*N instead of 4*N), and has zero function call overhead.
   ✔ Implementation Note: The iterative traversal goes bottom-up. Because it 
     merges left and right fragments in arbitrary order during range queries, it
     works perfectly only if the operation is commutative (i.e., A + B == B + A).

2. WHEN TO USE A RECURSIVE SEGMENT TREE (The Structural Powerhouse)
   ------------------------------------------------------------
   ✔ Best For: Non-commutative merges, Lazy Propagation, and Binary Search 
                on the tree structure.
   ✔ Why: Traversal goes top-down. This gives you complete control over the 
          exact order in which sub-segments are explored and combined.
   
   ✔ Mandatory Scenarios:
     1. Non-Commutative Operations: Problems like "Maximum Subarray Sum in a Range" 
        or "Matrix Multiplication" where order matters immensely (Left child MUST 
        be merged before the Right child).
     2. Lazy Propagation: Range updates (e.g., add 'X' to everything in range [L, R]) 
        require pushing deferred updates down from parent to children, which is 
        only naturally possible top-down.
     3. Tree-Walking: Finding the first element in a range >= X (e.g., CSES Hotel Queries) 
        by routing left or right conditionally based on node values.

================================================================================
SUMMARY RULE of THUMB:
Default to Iterative for basic Point-Update / Range-Query tasks to maximize speed. 
Switch to Recursive the moment you see Range Updates (Lazy) or Non-Commutative merges.
================================================================================
*/

/*
Usage:
    SegTree st(a);          // Build from array
    st.update(idx, val);    // a[idx] = val
    st.query(l, r);         // Sum over [l, r]
*/

/*
==================== SEGMENT TREE VARIANTS ====================

Only TWO things change for different problems:

1. What each Node stores.
2. How two child Nodes are merged.

Everything else (build, update, query) remains identical.

---------------------------------------------------------------
1. Range Sum Query
---------------------------------------------------------------
Node:
    struct Node { long long sum; };

Leaf:
    {a[i]}

Merge:
    parent.sum = left.sum + right.sum;

Identity (No overlap):
    {0}

---------------------------------------------------------------
2. Range Minimum Query
---------------------------------------------------------------
Node:
    struct Node { int mn; };

Leaf:
    {a[i]}

Merge:
    parent.mn = min(left.mn, right.mn);

Identity:
    {INT_MAX}

---------------------------------------------------------------
3. Range Maximum Query
---------------------------------------------------------------
Node:
    struct Node { int mx; };

Leaf:
    {a[i]}

Merge:
    parent.mx = max(left.mx, right.mx);

Identity:
    {INT_MIN}

---------------------------------------------------------------
4. Range GCD
---------------------------------------------------------------
Node:
    struct Node { int g; };

Leaf:
    {a[i]}

Merge:
    parent.g = gcd(left.g, right.g);

Identity:
    {0}

---------------------------------------------------------------
5. Maximum Prefix Sum
---------------------------------------------------------------
Node:
    struct Node {
        long long sum;     // Total segment sum
        long long pref;    // Best prefix sum
    };

Leaf:
    {x, max(0LL, x)}

Merge:
    sum  = L.sum + R.sum;
    pref = max(L.pref, L.sum + R.pref);

Identity:
    {0, 0}

---------------------------------------------------------------
6. Maximum Suffix Sum
---------------------------------------------------------------
Node:
    struct Node {
        long long sum;
        long long suff;
    };

Leaf:
    {x, max(0LL, x)}

Merge:
    sum  = L.sum + R.sum;
    suff = max(R.suff, R.sum + L.suff);

Identity:
    {0, 0}

---------------------------------------------------------------
7. Maximum Subarray Sum
---------------------------------------------------------------
Node:
    struct Node {
        long long sum;     // Total segment sum
        long long pref;    // Best prefix
        long long suff;    // Best suffix
        long long ans;     // Best subarray
    };

Leaf:
    best = max(0LL, x);
    {x, best, best, best}

Merge:
    sum  = L.sum + R.sum;
    pref = max(L.pref, L.sum + R.pref);
    suff = max(R.suff, R.sum + L.suff);
    ans  = max({L.ans, R.ans, L.suff + R.pref});

Identity:
    {0, 0, 0, 0}

---------------------------------------------------------------
8. Range XOR
---------------------------------------------------------------
Node:
    struct Node { int xr; };

Leaf:
    {a[i]}

Merge:
    xr = L.xr ^ R.xr;

Identity:
    {0}

===============================================================
The recursive build(), update() and query() NEVER change.
Only Node, merge(), leaf initialization, and identity change.
===============================================================

*/
struct SegTree2 {
    int n;
    vector<int> tree;

    SegTree2(vector<int>& a) {
        n = a.size();
        tree.resize(4 * n);
        build(1, 0, n - 1, a);
    }

    // Build the tree
    void build(int node, int lo, int hi, vector<int>& a) {
        if (lo == hi) {
            tree[node] = a[lo];
            return;
        }

        int mid = (lo + hi) / 2;

        build(2 * node, lo, mid, a);
        build(2 * node + 1, mid + 1, hi, a);

        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    // Point update
    void update(int idx, int val) {
        update(1, 0, n - 1, idx, val);
    }

    void update(int node, int lo, int hi, int idx, int val) {
        if (lo == hi) {
            tree[node] = val;
            return;
        }

        int mid = (lo + hi) / 2;

        if (idx <= mid)
            update(2 * node, lo, mid, idx, val);
        else
            update(2 * node + 1, mid + 1, hi, idx, val);

        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    // Range Query
    int query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    int query(int node, int lo, int hi, int l, int r) {
        // No overlap
        if (hi < l || r < lo)
            return 0;

        // Complete overlap
        if (l <= lo && hi <= r)
            return tree[node];

        // Partial overlap
        int mid = (lo + hi) / 2;

        return query(2 * node, lo, mid, l, r) +
               query(2 * node + 1, mid + 1, hi, l, r);
    }
};

/*
Coordinate Compressor Usage:
  CoordinateCompressor cc;
  cc.add(x); cc.build();
  int idx = cc.get(x);      // 0-based compressed index (ONLY valid if x was add()-ed before build())
  int orig = cc.vals[idx];   // Get original value back
  int sz = cc.size();       // Size of universe for Fenwick/SegTree sizing
  // WARNING: get(x) on a value NOT add()-ed before build() returns an arbitrary insertion-point index.

-----------------------------------------------------------
1. HOW TO USE WITH YOUR FENWICK TREE TEMPLATE:
   Your Fenwick tree takes 0-based arguments publicly. Since cc.get(x) 
   returns a 0-indexed integer, they map directly.

   // Size the BIT using cc.size()
   Fenwick ft(cc.size()); 
   
   // Point Update:
   ft.update(cc.get(val), 1); 
   
   // Range Query [L, R] Inclusive:
   int ans = ft.query(cc.get(l), cc.get(r));

-----------------------------------------------------------
2. HOW TO USE WITH YOUR SEGTREE TEMPLATE:
   Your SegTree is initialized using a base vector. Build a frequency 
   or value tracking vector matching the size of the compressed universe first.

   // Create base frequency mapping array
   vector<int> base_counts(cc.size(), 0);
   for (int x : arr) base_counts[cc.get(x)]++;
   
   // Initialize tree
   SegTree st(base_counts);
   
   // Range Query [L, R] Inclusive / Updates:
   st.update(cc.get(idx), new_val);
   int ans = st.query(cc.get(l), cc.get(r));
-----------------------------------------------------------
*/
struct CoordinateCompressor {
    vector<int> vals;
    void add(int x) { vals.push_back(x); }
    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }
    int get(int x) { return lower_bound(vals.begin(), vals.end(), x) - vals.begin(); }
    int size() { return vals.size(); }
};


/*
Usage:
  Fenwick ft(n);           // Initialize 0-indexed BIT for 'n' elements
  ft.update(idx, val);     // Adds 'val' to index 'idx' (0-based)
  ft.query(idx);           // Returns prefix sum from range [0, idx]
  ft.query(l, r);          // Returns range sum from [l, r] inclusive
*/
struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1) {}
    
    void update(int i, int val) {
        for (++i; i <= n; i += i & -i) bit[i] += val;
    }
    int query(int i) {
        int s = 0;
        for (++i; i > 0; i -= i & -i) s += bit[i];
        return s;
    }
    int query(int l, int r) { 
        return query(r) - (l > 0 ? query(l - 1) : 0); // guard: avoids query(-1) when l==0
    }
};

/* 
Usage: 
  SegTree st(a);           // Build tree from vector 'a' (0-indexed)
  st.update(idx, val);     // Sets a[idx] = val
  st.query(l, r);          // Returns sum of range [l, r] inclusive

Tips to change from Sum to Min/Max or custom structures (like Non-Commutative operations):
  1. In constructor: change '+' to 'min' (or 'max') in:
     t[i] = t[i << 1] + t[i << 1 | 1];
  2. In update: change '+' to 'min' (or 'max') in:
     t[p >> 1] = t[p & ~1] + t[p | 1]; // Node order matters: left is p & ~1, right is p | 1
  3. In query: change initial values 'ra = 0, rb = 0' to appropriate identities,
     and change '+' to 'min' (or 'max') in:
     if (l & 1) ra = ra + t[l++];
     if (r & 1) rb = t[--r] + rb;
     return ra + rb;
  4. Note: This iterative template uses power-of-two padding, which guarantees correct left-to-right 
     merging order. This makes it safe for non-commutative operations (like matrix multiplication, 
     max subarray sum, etc.) as long as you preserve the merge operand order.
*/
struct SegTree {
    int n; vector<int> t;
    SegTree(vector<int>& a) {
        n = 1;
        while (n < a.size()) n *= 2;
        t.assign(2 * n, 0);
        for (int i = 0; i < a.size(); i++) t[n + i] = a[i];
        for (int i = n - 1; i > 0; --i) t[i] = t[i << 1] + t[i << 1 | 1];
    }
    void update(int p, int val) {
        for (t[p += n] = val; p > 1; p >>= 1) t[p >> 1] = t[p & ~1] + t[p | 1];
    }
    int query(int l, int r) {
        int ra = 0, rb = 0;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ra = ra + t[l++];
            if (r & 1) rb = t[--r] + rb;
        }
        return ra + rb;
    }
};

/*
Usage:
  SparseTable st(a);       // Build sparse table from vector 'a' (0-indexed)
  st.query(l, r);          // Returns range minimum from [l, r] inclusive in O(1)
  l and r are 0-based indices.
Tips to change from Min to Max:
  1. In constructor: change 'min' to 'max' in:
     st[j][i] = min(st[j-1][i], st[j-1][i+(1<<(j-1))]);
  2. In query: change 'min' to 'max' in:
     return min(st[j][l], st[j][r - (1<<j) + 1]);
*/
struct SparseTable {
   int n, K;
   vector<vector<int>> st;

   SparseTable(vector<int> &a) {
      n = a.size(); 
      K = (n > 0 ? 31 - __builtin_clz(n) : 0) + 1;
      st.assign(K, vector<int>(n));
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
    FastHash(string s) {
        int n = s.size(); p.assign(n + 1, 1); h.assign(n + 1, 0);
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
KMP equivalent (Pattern Matching / Rabin-Karp) using FastHash
Time Complexity: O(N + M)
Usage:
  vi occurrences = hash_kmp(text, pattern);
  Returns 0-based starting indices of all occurrences of pattern in text.
*/
vi hash_kmp(string text, string pattern) {
    vi occurrences;
    int n = text.size();
    int m = pattern.size();
    if (m == 0 || n < m) return occurrences;

    FastHash fh_text(text);
    ll pat_hash = hashWord(pattern);

    for (int i = 0; i <= n - m; i++) {
        if (fh_text.get(i, i + m - 1) == pat_hash) {
            occurrences.push_back(i);
        }
    }
    return occurrences;
}

/*
Z-array using FastHash (Z-Algo equivalent)
Time Complexity: O(N log N)
Usage:
  vector<int> z = hash_z_algo(s);
*/
vector<int> hash_z_algo(string s) {
    int n = s.size();
    vector<int> z(n, 0);
    if (n == 0) return z;
    FastHash fh(s);
    z[0] = n;
    for (int i = 1; i < n; i++) {
        int low = 1, high = n - i, ans = 0; // why n-i because the substring starting at i can be at most n-i long
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
} // z[i] stores the length of the longest substring starting from s[i] that is also a prefix of s

/*
Manacher's equivalent using FastHash (Palindromic substrings)
Time Complexity: O(N log N)
Returns:
  pair<vector<int>, vector<int>>: {d1, d2}
  radius means the length of the palindrome centered at that index (full length = 2*radius - 1 for odd, 2*radius for even)
  d1[i] = radius of palindrome centered at i (odd-length)
  d2[i] = radius of palindrome centered between i-1 and i (even-length)
*/
pair<vi, vi> hash_manacher(string s) {
    int n = s.size();
    vi d1(n, 1), d2(n, 0); // d1 defaults to radius 1 (single char), d2 to 0
    if (n == 0) return {d1, d2};

    FastHash fh(s);
    string s_rev = s; reverse(s_rev.begin(), s_rev.end());
    FastHash fh_rev(s_rev);

    // O(1) palindrome check by matching forward hash with reversed string hash
    auto is_pal = [&](int l, int r) {
        return l >= 0 && r < n && l <= r && fh.get(l, r) == fh_rev.get(n - 1 - r, n - 1 - l);
    };

    for (int i = 0; i < n; i++) {
        // 1. Odd-length palindromes centered at i (Range: [i - ans + 1, i + ans - 1])
        int low = 2, high = min(i + 1, n - i); // low starts at 2 since radius 1 is always true
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid + 1, i + mid - 1)) d1[i] = mid, low = mid + 1;
            else high = mid - 1;
        }

        // 2. Even-length palindromes between i-1 and i (Range: [i - ans, i + ans - 1])
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

/*
Kruskal's Algorithm (MST)
Time Complexity: O(E log E)
Usage:
  vector<Edge> edges, mst_edges;
  long long total_weight = kruskal(n, edges, mst_edges);
*/
struct Edge {
    int u, v; long long w;
    bool operator<(const Edge& o) const { return w < o.w; } // Sort edges by weight for Kruskal's algorithm
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

/*
Topological Sort (Kahn's and DFS)
Returns topological order of nodes. Returns empty vector if cycle exists.
*/
// Kahn's (BFS-based). Handles 0-indexed and 1-indexed.
vector<int> kahn(int n, vector<vector<int>>& adj, int start_node = 1) {
    vector<int> in_deg(adj.size(), 0);
    for (int u = start_node; u < start_node + n; u++) {
        for (int v : adj[u]) in_deg[v]++;
    }
    queue<int> q;
    for (int i = start_node; i < start_node + n; i++) {
        if (in_deg[i] == 0) q.push(i);
    }
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
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

pair<vector<long long>, vector<long long>> solve_dag(int n, vector<vector<DAGEdge>>& adj, int src, int start_node = 1, bool max_path = false) {
    vector<int> in_deg(adj.size(), 0);
    for (int u = start_node; u < start_node + n; u++) {
        for (auto& e : adj[u]) in_deg[e.to]++;
    }
    queue<int> q;
    for (int i = start_node; i < start_node + n; i++) {
        if (in_deg[i] == 0) q.push(i);
    }
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (auto& e : adj[u]) {
            if (--in_deg[e.to] == 0) q.push(e.to);
        }
    }
    const long long INF_VAL = 1e18;
    vector<long long> dist(adj.size(), max_path ? -INF_VAL : INF_VAL); // dist[i] = shortest/longest distance from src to i
    vector<long long> paths(adj.size(), 0); // paths[i] = number of shortest/longest paths from src to i
    dist[src] = 0; paths[src] = 1;
    for (int u : order) {
        if (dist[u] == (max_path ? -INF_VAL : INF_VAL)) continue;
        for (auto& e : adj[u]) {
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

// calculates the sum of distances from each node to all other nodes in a tree using rerooting technique
class TreeDistances {
    int n;
    const vector<vector<int>>& adj;

    vector<int> subtree_sz;
    vector<long long> total_dist;

public:
    TreeDistances(int nodes, const vector<vector<int>>& graph)
        : n(nodes), adj(graph),
          subtree_sz(nodes + 1, 0),
          total_dist(nodes + 1, 0) {}

    // Compute subtree sizes
    void dfs_size(int u, int p) {
        subtree_sz[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_size(v, u);
            subtree_sz[u] += subtree_sz[v];
        }
    }

    // Compute sum of distances from the initial root
    void dfs_root(int u, int p, int depth) {
        total_dist[1] += depth;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_root(v, u, depth + 1);
        }
    }

    // Reroot DP
    void reroot(int u, int p) {
        for (int v : adj[u]) {
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
    int n; const vector<vector<int>>& adj;
    vector<int> d1,d2;

    void dfs(int u,int p,vector<int>& d){
        for(int v:adj[u]) if(v!=p)
            d[v]=d[u]+1, dfs(v,u,d);
    }

    int farthest(vector<int>& d){
        return max_element(d.begin()+1,d.end())-d.begin();
    }

public:
    TreeDiameter(int n,const vector<vector<int>>& g)
        : n(n),adj(g),d1(n+1),d2(n+1){}

    int diameter(){
        fill(d1.begin(),d1.end(),0);
        dfs(1,0,d1);
        int a=farthest(d1);

        fill(d1.begin(),d1.end(),0);
        dfs(a,0,d1);
        int b=farthest(d1);

        return d1[b];
    } // returns the length of the diameter of the tree

    vector<int> eccentricity(){
        fill(d1.begin(),d1.end(),0);
        dfs(1,0,d1);
        int a=farthest(d1);

        fill(d1.begin(),d1.end(),0);
        dfs(a,0,d1);
        int b=farthest(d1);

        fill(d2.begin(),d2.end(),0);
        dfs(b,0,d2);

        vector<int> ans(n+1);
        for(int i=1;i<=n;i++) ans[i]=max(d1[i],d2[i]);
        return ans;
    } // returns the eccentricity of each node in the tree (1-indexed), which means the maximum distance from that node to any other node in the tree, which is actually max (d1[i], d2[i]) where d1[i] is the distance from one end of the diameter and d2[i] is the distance from the other end of the diameter.
};


/*
Usage:

  Bridge B;
  auto ans = B.get(adj); // Returns vector<pair<int,int>> of bridges
*/
struct Bridge {
    int t; vector<int> tin, low; vector<pair<int,int>> res;
    void dfs(int u, int p, vector<vector<int>>& adj) {
        tin[u] = low[u] = ++t;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u, adj);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) res.push_back({u, v});
            }
        }
    }
    vector<pair<int,int>> get(vector<vector<int>>& adj) {
        int n = adj.size(); t = 0; tin.assign(n, 0); low.resize(n); res.clear();
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, adj);
        return res;
    }
};

struct Articulation {
    int t; vector<int> tin, low; vector<int> res;
    void dfs(int u, int p, vector<vector<int>>& adj) {
        tin[u] = low[u] = ++t; int children = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u, adj); low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u] && p != -1) res.push_back(u);
                children++;
            }
        }
        if (p == -1 && children > 1) res.push_back(u); // root is an articulation point if it has more than one child
    }
    vector<int> get(vector<vector<int>>& adj) {
        int n = adj.size(); t = 0; tin.assign(n, 0); low.resize(n); res.clear();
        for (int i = 0; i < n; i++) if (!tin[i]) dfs(i, -1, adj);
        return res;
    }
};
/*
Usage:
  BinaryLifting bl(n, root, adj);
  int node_lca = bl.lca(u, v);
  int ancestor = bl.lift(u, k);
  int dist = bl.depth[u] + bl.depth[v] - 2 * bl.depth[node_lca];
  This is 1-indexed safe. If your tree is 0-indexed, adjust accordingly.
*/
struct BinaryLifting {
    int LOG;
    vector<vector<int>> up;
    vector<int> depth;

    BinaryLifting(int n, int root, vector<vector<int>>& adj) {
        LOG = 32 - __builtin_clz(n);
        up.assign(n + 1, vector<int>(LOG, -1));
        depth.assign(n + 1, 0);
        dfs(root, -1, 0, adj);
    }

    void dfs(int u, int p, int d, vector<vector<int>>& adj) {
        depth[u] = d; up[u][0] = p;
        for (int j = 1; j < LOG; j++) 
            if (up[u][j - 1] != -1) up[u][j] = up[up[u][j - 1]][j - 1];
        for (int v : adj[u]) if (v != p) dfs(v, u, d + 1, adj);
    }

    int lift(int u, int k) {
        for (int j = 0; j < LOG && u != -1; j++) if ((k >> j) & 1) u = up[u][j];
        return u;
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        a = lift(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int j = LOG - 1; j >= 0; j--) 
            if (up[a][j] != up[b][j]) a = up[a][j], b = up[b][j];
        return up[a][0];
    }

    int distance(int a, int b) {
        return depth[a] + depth[b] - 2 * depth[lca(a, b)];
    }

    int isAncestor(int a, int b) {
        return lca(a, b) == a;
    }

    int kthNodeOnPath(int a, int b, int k) {
        int l = lca(a, b);
        int d1 = depth[a] - depth[l];
        if (k <= d1) return lift(a, k);
        k -= d1;
        int d2 = depth[b] - depth[l];
        if (k <= d2) return lift(b, d2 - k);
        return -1; // k is larger than the distance between a and b
    } // returns the k-th node on the path from a to b (0-indexed, a is 0-th)
    void dfsCount(int u, int p, vector<int>& cnt, vector<vector<int>>& adj) {
        for (int v : adj[u]) {
            if (v == p) continue;
            dfsCount(v, u, cnt, adj);
            cnt[u] += cnt[v];
        }
    }

    vector<int> countingPaths(int n, vector<pair<int,int>>& paths,vector<vector<int>>& adj) {
        vector<int> cnt(n + 1, 0);

        for (auto &[a, b] : paths) {
            int l = lca(a, b);
            cnt[a]++;
            cnt[b]++;
            cnt[l]--;
            if (up[l][0] != -1)
                cnt[up[l][0]]--;
        }

        dfsCount(1, -1, cnt, adj);   // root of the tree

        return cnt;
    }
};

/*
Usage:
  XorTrie trie;
  trie.insert(x);
  trie.remove(x);          // Safely removes x from the trie
  trie.maxXor(x);          // Returns maximum value of (x ^ y)
  trie.countLess(x, k);    // Returns count of elements where (x ^ y) < k
*/
struct Nodee {
    Nodee *c[2] = {};
    int cnt = 0;
};

struct XorTrie {
    Nodee *root = new Nodee();

    void insert(int x, int d = 1) {
        Nodee *cur = root;
        for (int b = 30; b >= 0; b--) {
            int i = (x >> b) & 1;
            if (!cur->c[i]) cur->c[i] = new Nodee();
            cur = cur->c[i]; cur->cnt += d;
        }
    } // d is +1 for insert, -1 for remove
    void remove(int x) { insert(x, -1); } // Crucial for dynamic windows/sets

    int maxXor(int x) {
        Nodee *cur = root; int ans = 0;
        for (int b = 30; b >= 0; b--) {
            int i = (x >> b) & 1;
            if (cur->c[i ^ 1] && cur->c[i ^ 1]->cnt > 0) ans |= 1 << b, cur = cur->c[i ^ 1];
            else cur = cur->c[i];
        }
        return ans;
    }

    int countLess(int x, int k) {
        Nodee *cur = root; int ans = 0;
        for (int b = 30; b >= 0 && cur; b--) {
            int xb = (x >> b) & 1, kb = (k >> b) & 1;
            if (kb) {
                if (cur->c[xb]) ans += cur->c[xb]->cnt;
                cur = cur->c[xb ^ 1];
            } else cur = cur->c[xb];
        }
        return ans;
    }
};

/*
Usage (String Trie):
  StringTrie trie;         // Initialize character String Trie (lowercase 'a'-'z')
  trie.insert(s);          // Inserts string 's' into the Trie
  trie.remove(s);          // Safely deletes string 's' from the Trie (subtracts counts)
  trie.search(s);          // Returns true if string 's' exists in the Trie
  trie.startsWith(s);      // Returns true if there is any string with prefix 's'
  trie.countEqual(s);      // Returns count of strings exactly equal to 's'
  trie.countPrefix(s);     // Returns count of strings starting with prefix 's'
*/
struct Node {
    Node *c[26] = {};
    int endCnt = 0, preCnt = 0;
};

struct StringTrie {
    Node *root = new Node();

    void insert(string s, int d = 1) {
        Node *cur = root;
        for (char ch : s) {
            int i = ch - 'a';
            if (!cur->c[i]) cur->c[i] = new Node();
            cur = cur->c[i]; cur->preCnt += d;
        }
        cur->endCnt += d;
    }
    void remove(string s) { insert(s, -1); } // Crucial for moving windows/sets

private:
    Node* find(string s) {
        Node *cur = root;
        for (char ch : s) if (!(cur = cur->c[ch - 'a'])) return nullptr;
        return cur;
    }

public:
    bool search(string s) { Node *n = find(s); return n && n->endCnt > 0; }
    bool startsWith(string s) { Node *n = find(s); return n && n->preCnt > 0; }
    int countEqual(string s) { Node *n = find(s); return n ? n->endCnt : 0; }
    int countPrefix(string s) { Node *n = find(s); return n ? n->preCnt : 0; }
};

/*
Usage:
  SCC graph(n);
  graph.add(u, v);
  auto components = graph.build();  // Returns vector<vector<int>> of SCC groups
  int cid = graph.comp[u];          // Component ID of node u
  auto dag = graph.get_dag();       // Condenses SCCs into a condensed DAG
*/
struct SCC {
    int n; vector<vector<int>> adj, radj, sccs, dag; stack<int> order; vector<int> comp; vector<bool> vis;
    SCC(int n) : n(n), adj(n + 1), radj(n + 1), comp(n + 1, -1), vis(n + 1) {}

    void add(int u, int v) { adj[u].push_back(v); radj[v].push_back(u); }

    void dfs1(int u) {
        vis[u] = 1;
        for (int v : adj[u]) if (!vis[v]) dfs1(v);
        order.push(u);
    }
    void dfs2(int u, int c) {
        comp[u] = c; sccs[c].push_back(u);
        for (int v : radj[u]) if (comp[v] == -1) dfs2(v, c);
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
        for (int u = 1; u <= n; u++) for (int v : adj[u])
            if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
        for (auto& neighbors : dag) {
            sort(neighbors.begin(), neighbors.end());
            neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
        }
        return dag;
    }
};
/*
Usage:
  DigitDP dp;
  long long count = dp.solve(n); // Computes for range [0, n]
*/
class DigitDP {
    string s;
    long long memo[20][2][2][100]; // [pos][started][tight][custom_state]

    long long dp(int pos, bool started, bool tight, int state) {
        if (pos == s.size()) return state; // Evaluate state here
        if (memo[pos][started][tight][state] != -1) return memo[pos][started][tight][state];

        int limit = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) ans += dp(pos + 1, false, tight && (0 == limit), state);

        for (int d = (started ? 0 : 1); d <= limit; d++) 
            ans += dp(pos + 1, true, tight && (d == limit), state + d); // Custom logic

        return memo[pos][started][tight][state] = ans;
    }

public:
    long long solve(long long n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

// Binary Exponentiation
long long power(long long a, long long b, int MOD) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}
// a^b^c = power(a, power(b, c, MOD - 1), MOD) (Fermat's Little Theorem)
// Modular Inverse (MOD must be prime)
long long inv(long long x, int MOD) {
    return power(x, MOD - 2, MOD);
}

// Factorials + Inverse Factorials
vector<long long> fac, ifac;

void init_nCr(int n, int MOD) {
    fac.resize(n + 1);
    ifac.resize(n + 1);

    fac[0] = 1;
    for (int i = 1; i <= n; i++)
        fac[i] = fac[i - 1] * i % MOD;

    ifac[n] = power(fac[n], MOD - 2, MOD);
    for (int i = n; i > 0; i--)
        ifac[i - 1] = ifac[i] * i % MOD;
}

long long nCr(int n, int r, int MOD) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[r] % MOD * ifac[n - r] % MOD;
}

long long nPr(int n, int r, int MOD) {
    if (r < 0 || r > n) return 0;
    return fac[n] * ifac[n - r] % MOD;
}

// Sieve (minimum prime factor)
vector<int> min_pf;

void sieve(int n) {
    min_pf.resize(n + 1);
    iota(min_pf.begin(), min_pf.end(), 0);

    for (int i = 2; i * i <= n; i++)
        if (min_pf[i] == i)
            for (int j = i * i; j <= n; j += i)
                if (min_pf[j] == j)
                    min_pf[j] = i;
}

vector<pair<int,int>> prime_factorize(int n) {
    vector<pair<int,int>> pf;

    for (int p = 2; p * p <= n; p++) {
        if (n % p) continue;

        int cnt = 0;
        while (n % p == 0) {
            n /= p;
            cnt++;
        }

        pf.push_back({p, cnt});
    }

    if (n > 1) pf.push_back({n, 1});
    return pf;
}

/*
Bit Operations

1 << i              // 2^i

x & (1 << i)        // check if i-th bit is set
x |= (1 << i)       // set i-th bit
x &= ~(1 << i)      // clear i-th bit
x ^= (1 << i)       // toggle i-th bit

x & (x - 1)         // removes the lowest set bit
x & -x              // value of the lowest set bit

(x & (x - 1)) == 0  // check if x is a power of 2 (x > 0)

__builtin_popcount(x)      // number of set bits (int)
__builtin_popcountll(x)    // number of set bits (long long)

__builtin_clz(x)           // leading zeros (int)
__builtin_clzll(x)         // leading zeros (long long)

__builtin_ctz(x)           // trailing zeros (int)
__builtin_ctzll(x)         // trailing zeros (long long)

__builtin_parity(x)        // 1 if odd number of set bits, else 0

msb = 31 - __builtin_clz(x)        // index of most significant set bit
msb = 63 - __builtin_clzll(x)      // (long long)

lsb = __builtin_ctz(x)             // index of least significant set bit
lsb = __builtin_ctzll(x)           // (long long)
*/

int XORupto(int n) {
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}


// ------------------------------------------------------------
// Sliding Window Maximum
//
// Deque: decreasing
// Front = maximum of current window
//
// Time: O(N)
// ------------------------------------------------------------
vi maxSlidingWindow(const vi& nums, int k) {
    deque<int> dq;
    vi ans;
    int n = nums.size();
    for (int R = 0; R < n; R++) {
        int L = R - k + 1;

        while (!dq.empty() && dq.front() < L) dq.pop_front();

        while (!dq.empty() && nums[dq.back()] <= nums[R]) dq.pop_back();

        dq.push_back(R);

        if (L >= 0) ans.push_back(nums[dq.front()]);
    }

    return ans;
}

// ------------------------------------------------------------
// Sliding Window Minimum
//
// Deque: increasing
// Front = minimum of current window
//
// Time: O(N)
// ------------------------------------------------------------
vi minSlidingWindow(const vi& nums, int k) {
    deque<int> dq;
    vi ans;
    int n = nums.size();

    for (int R = 0; R < n; R++) {
        int L = R - k + 1;

        while (!dq.empty() && dq.front() < L) dq.pop_front();

        while (!dq.empty() && nums[dq.back()] >= nums[R]) dq.pop_back();

        dq.push_back(R);

        if (L >= 0) ans.push_back(nums[dq.front()]);
    }

    return ans;
}

int lis_length(const vi& a) {
    vi dp;
    for (int x : a) {
        auto it = lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x);
        else *it = x;
    }
    return dp.size();
} // time complexity: O(n log n), returns the length of the longest increasing subsequence

vector<int> reconstruct_lis(const vi& a) {
    int n = a.size();
    vi dp, idx(n), prev(n, -1);

    for (int i = 0; i < n; i++) {
        auto it = lower_bound(dp.begin(), dp.end(), a[i]);
        int pos = it - dp.begin();

        if (it == dp.end()) dp.push_back(a[i]);
        else *it = a[i]; // replace with a[i] to reconstruct the actual sequence

        idx[pos] = i; // store the index of the last element of the subsequence of length pos + 1
        if (pos > 0) prev[i] = idx[pos - 1]; // store the index of the previous element in the subsequence
    }
    int lis_length = dp.size();
    vector<int> lis;
    for (int i = idx[lis_length - 1]; i >= 0; i = prev[i]) lis.push_back(a[i]);
    reverse(lis.begin(), lis.end());
    return lis;
} // time complexity: O(n log n), returns the longest increasing subsequence itself

string lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    // Reconstruct the LCS
    string lcs_str;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            lcs_str += a[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) { 
            i--;
        } else {
            j--;
        }
    }
    reverse(lcs_str.begin(), lcs_str.end());
    return lcs_str;
} // time complexity: O(n * m), returns the longest common subsequence of strings a and b

/*
=========================================================
INTERVAL DP TEMPLATE (Bottom-Up)
=========================================================

State:
    dp[l][r] = answer for interval [l, r]

Transition:
    dp[l][r] = combine(dp[l][k], dp[k+1][r], l, k, r)

Order:
    Increasing interval length.

Time:
    O(n^3)

=========================================================
Common Variants

1. Matrix Chain Multiplication
--------------------------------
State:
    dp[l][r] = minimum cost to multiply matrices l...r

Base:
    dp[i][i] = 0

Transition:
    dp[l][r] = min(
        dp[l][k] +
        dp[k+1][r] +
        arr[l] * arr[k+1] * arr[r+1]
    )

---------------------------------------------------------
2. Merge Stones / Slimes
--------------------------------
State:
    dp[l][r] = minimum cost to merge interval

Extra:
    prefix sums required

Transition:
    dp[l][r] = min(
        dp[l][k] +
        dp[k+1][r]
    ) + sum(l,r)

---------------------------------------------------------
3. Optimal BST
--------------------------------
Transition:
    dp[l][r] = min(
        dp[l][k-1] +
        dp[k+1][r]
    ) + freqSum(l,r)

---------------------------------------------------------
4. Polygon Triangulation
--------------------------------
State:
    dp[l][r] = minimum triangulation cost

Transition:
    for k = l+1 ... r-1

    dp[l][r] = min(
        dp[l][k] +
        dp[k][r] +
        cost(l,k,r)
    )

---------------------------------------------------------
5. Burst Balloons
--------------------------------
State:
    dp[l][r] = maximum coins from balloons l...r

Pad array with 1 at both ends.

Transition:
    Choose LAST balloon k.

    dp[l][r] = max(
        dp[l][k-1] +
        dp[k+1][r] +
        a[l-1] * a[k] * a[r+1]
    )

---------------------------------------------------------
6. Rod Cutting / Stick Cutting
--------------------------------
State:
    dp[l][r] = minimum cost to cut segment

Transition:
    dp[l][r] = min(
        dp[l][k] +
        dp[k][r] +
        cuts[r] - cuts[l]
    )

=========================================================
*/
long long solveIntervalDPBottomUp(int n) {
    vector<vector<long long>> dp(n, vector<long long>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            dp[l][r] = LLONG_MAX;      // LLONG_MIN for maximization

            for (int k = l; k < r; k++) {

                long long cur =
                    dp[l][k] +
                    dp[k + 1][r]
                    /* + problem-specific cost(l,k,r) */;

                dp[l][r] = min(dp[l][r], cur);
                // dp[l][r] = max(dp[l][r], cur);
            }
        }
    }

    return dp[0][n - 1];
}

vector<vector<int>> generateSubsets(vector<int>& nums) {
    vector<vector<int>> subsets;
    int n = nums.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) subset.push_back(nums[i]);
        }
        subsets.push_back(subset);
    }
    return subsets;
}

int sumOfAllSubsets(vector<int>& nums) {
    int n = nums.size();
    int total_sum = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int subset_sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) subset_sum += nums[i];
        }
        total_sum += subset_sum;
    }
    return total_sum;
}

vector<int> kadaneWithLandR(const vector<int>& nums) {
    int n = nums.size();
    int max_sum = INT_MIN, current_sum = 0;
    int start = 0, end = 0, temp_start = 0;

    for (int i = 0; i < n; i++) {
        current_sum += nums[i];

        if (current_sum > max_sum) {
            max_sum = current_sum;
            start = temp_start;
            end = i;
        }

        if (current_sum < 0) {
            current_sum = 0;
            temp_start = i + 1;
        }
    }

    return {max_sum, start, end}; // Returns maximum sum and the indices of the subarray
}


void solve_(vector<int>& nums, vector<int>& curr, vector<vector<int>>& ans, int mask) {
    if (curr.size() == nums.size()) {
        ans.push_back(curr);
        return;
    }

    for (int i = 0; i < nums.size(); i++) {
        if (!(mask & (1 << i))) {   // if ith element is not used
            curr.push_back(nums[i]);
            solve_(nums, curr, ans, mask | (1 << i));
            curr.pop_back();
        }
    }
}

vector<vector<int>> generatePermutations(vector<int>& nums) {
    vector<vector<int>> ans;
    vector<int> curr;

    solve_(nums, curr, ans, 0);

    return ans;
}

int lps(string s) {
    int n = s.size();
    vector<vector<int>> dp(n, vector<int>(n, 0)); // dp[i][j] = length of longest palindromic subsequence in s[i..j]

    for (int i = 0; i < n; i++) dp[i][i] = 1;

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            if (s[l] == s[r]) {
                dp[l][r] = 2 + (len > 2 ? dp[l + 1][r - 1] : 0); // we will add 2 to the length of the longest palindromic subsequence in s[l+1..r-1]
            } else {
                dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
            }
        }
    }

    return dp[0][n - 1];
} // time complexity: O(n^2), returns the length of the longest palindromic subsequence
// min insertions to make a string palindrome = n - lps(s)


bool checkSubsequence(const string& s, const string& t) {
    int n = s.size(), m = t.size();
    int j = 0; // Pointer for t
    for (int i = 0; i < n && j < m; i++) {
        if (s[i] == t[j]) j++;
    }
    return j == m; // If we have matched all characters of t
} // time complexity: O(n), returns true if t is a subsequence of s

vector<vector<int>> palindromeTable(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> isPalindrome(n, vector<int>(n, 0));
    for(int i=0; i<n; i++) isPalindrome[i][i] = 1;
    for(int len=2; len<=n; len++) {
        for(int l=0; l+len-1<n; l++) {
            int r = l + len - 1;
            if(nums[l] == nums[r]) {
                isPalindrome[l][r] = (len == 2) ? 1 : isPalindrome[l+1][r-1];
            } else {
                isPalindrome[l][r] = 0;
            }
        }
    }
    return isPalindrome;
} // time complexity: O(n^2), returns a table where isPalindrome[l][r] is true if the subarray nums[l..r] is a palindrome

vector<int> buildCycle(int start, const vector<int>& parent) {
    vector<int> cycle;
    int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];

        if (cur == start)
            break;
    }

    return cycle;
}
/*=============================================================================
    1. UNDIRECTED GRAPH CYCLE RECONSTRUCTION (1-indexed)
=============================================================================*/
class UndirectedCycle {
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<int> vis, parent;

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue; 
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v, u)) return true;
            } else { 
                startNode = v; endNode = u;
                return true;
            }
        }
        return false;
    }

public:
    UndirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, 0), parent(n + 1, -1) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v); adj[v].push_back(u);
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i, -1)) {
                return buildCycle(startNode, parent);
            }
        }
        return {};
    }
};

/*=============================================================================
    2. DIRECTED GRAPH CYCLE RECONSTRUCTION (1-indexed)
=============================================================================*/
class DirectedCycle {
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<bool> vis, inPath;
    vector<int> parent;

    bool dfs(int u) {
        vis[u] = inPath[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v)) return true;
            } else if (inPath[v]) {   
                startNode = v; endNode = u;
                return true;
            }
        }
        inPath[u] = false;
        return false;
    }

public:
    DirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, false), inPath(n + 1, false), parent(n + 1, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i)) {
                return buildCycle(startNode, parent);
            }
        }
        return {};
    }
};

/*=============================================================================
    3. NEGATIVE WEIGHT CYCLE FINDING (Bellman-Ford, 1-indexed)
=============================================================================*/
struct Edge { 
    int u, v; 
    ll w; 
};

vector<int> findNegativeCycle(int n, const vector<Edge>& edges) {
    vector<ll> dist(n + 1, 0); 
    vector<int> parent(n + 1, -1);
    int lastRelaxedNode = -1;
    
    for (int i = 1; i <= n; i++) {
        lastRelaxedNode = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w; 
                parent[e.v] = e.u; 
                lastRelaxedNode = e.v; 
            }
        }
    }
    if (lastRelaxedNode == -1) return {}; 
    
    // Backtrack N times to guarantee entry into the cycle structure
    for (int i = 0; i < n; i++) {
        lastRelaxedNode = parent[lastRelaxedNode];
    }
    return buildCycle(lastRelaxedNode, parent);
}

/*=============================================================================
    4. SHORTEST CYCLE LENGTH / GIRT (BFS, 0-indexed)
=============================================================================*/
int findShortestCycle(int n, const vector<vector<int>>& adj) {
    int minCycleLen = INT_MAX;
    
    for (int src = 0; src < n; src++) {
        vector<int> dist(n, -1), parent(n, -1); 
        queue<int> q;
        dist[src] = 0; 
        q.push(src);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1; 
                    parent[v] = u; 
                    q.push(v);
                } else if (parent[u] != v) { 
                    minCycleLen = min(minCycleLen, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    return minCycleLen == INT_MAX ? -1 : minCycleLen;
}

/*=============================================================================
    5. IDENTIFY ALL CYCLIC DEPENDENCY NODES (Kahn's Peeling, 0-indexed)
=============================================================================*/
vector<int> getNodesInCycles(int n, const vector<vector<int>>& adj, vector<int>& indegree) {
    queue<int> q; 
    vector<bool> isAcyclic(n, false); 
    vector<int> cyclicNodes;
    
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    while (!q.empty()) {
        int u = q.front(); q.pop(); 
        isAcyclic[u] = true;
        for (int v : adj[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (!isAcyclic[i]) cyclicNodes.push_back(i);
    }
    return cyclicNodes;
}

bool dfs_bipartite(int u, int color, vector<int>& colors, const vector<vector<int>>& adj) {
    colors[u] = color;
    for (int v : adj[u]) {
        if (colors[v] == -1) {
            if (!dfs_bipartite(v, 1 - color, colors, adj)) return false;
        } else if (colors[v] == color) {
            return false; // Found same color on adjacent nodes
        }
    }
    return true;
}


vector<int> shortestPathDAG(int n, vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> indeg(n);

    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u])
            indeg[v]++;
    }

    vector<int> dist(n, INT_MAX);
    dist[src] = 0;

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indeg[i] == 0)
            q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto [v, wt] : adj[u]) {
            if (dist[u] != INT_MAX && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
            indeg[v]--;
            if (indeg[v] == 0)
                q.push(v);
        }
    }

    return dist;
}


int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left < right) {
        left >>= 1; // both are shifted right until they are equal, which means we are finding the common prefix
        right >>= 1;
        shift++; // count how many bits we have shifted, which will be the number of zeros in the result
    }
    return left << shift;
}

int totalHammingDistance(const vector<int>& nums) {
    int total = 0, n = nums.size();
    for (int bit = 0; bit < 32; bit++) {
        int countOnes = 0;
        for (int num : nums) {
            if (num & (1 << bit)) countOnes++;
        }
        total += countOnes * (n - countOnes); // pairs of differing bits
    }
    return total;
}

int longestNiceSubarray(const vector<int>& nums) {
    int n = nums.size();
    int left = 0, maxLength = 0;
    int mask = 0;
    for(int i=0; i<n; i++) {
        while((mask & nums[i]) != 0) { // While the current number shares any set bits with the accumulated mask, we need to shrink the window from the left to maintain the "nice" property (no overlapping bits).
            mask ^= nums[left];
            left++;
        }
        mask |= nums[i];
        maxLength = max(maxLength, i - left + 1);
    }
    return maxLength;
} // finds the length of the longest subarray such that the bitwise AND of any two elements is 0 (i.e., no two elements share a set bit).

int subarrayBitwiseORs(vector<int>& arr) {
        unordered_set<int>s1;
        unordered_set<int>s2;
        for(auto c:arr){
            unordered_set<int>s3;
            s3.insert(c);
            for(auto v:s2) s3.insert(c|v);
            for(auto x:s3) s1.insert(x);
            s2=s3;
        }
        return s1.size();
} // finds the number of distinct values that can be obtained by taking the bitwise OR of all possible contiguous subarrays of the given array.

int countTotalSetBits(int n) {
   int total_ones = 0;

    // Iterate through all possible bit positions (0 to 60)
    for (int i = 0; i < 60; i++) {
        // lengthCycle = 2^(i+1)
        int lengthCycle = 1LL << (i + 1);
        int totalCycles = (n + 1) / lengthCycle;
        int one = 1LL << i;
        int zeroes = 1LL << i; 
        total_ones += totalCycles * one;
        int remainder = (n + 1) % lengthCycle;
        // in the last block 
        if (remainder > zeroes) {
            total_ones += (remainder - zeroes);
        }
    }

    cout << total_ones << "\n";
} // counts the total number of set bits in the binary representations of all numbers from 1 to n.


int CountSubsetsWithSumK(const vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> dp(k + 1, 0);
    dp[0] = 1; // There's one way to make sum 0: choose no elements.

    for (int num : nums) {
        for (int j = k; j >= num; j--) {
            dp[j] += dp[j - num];
        }
    }

    return dp[k];
} // counts the number of subsets of nums that sum up to k.

int minSubsetSumDifference(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    int n = nums.size();
    vector<bool> dp(totalSum / 2 + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int j = totalSum / 2; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }

    for (int j = totalSum / 2; j >= 0; j--) {
        if (dp[j]) {
            return totalSum - 2 * j;
        }
    }

    return totalSum; // This line should never be reached.
} // finds the minimum difference between the sums of two subsets of nums.

vector<int> getMoneySums(vector<int>& coins) {
    int n = coins.size();
    if (n == 0) return {};
    
    int total = 0;
    for (int c : coins) {
        total += c;
    }
    
    vector<bool> dp(total + 1, false);
    dp[0] = true;
    
    // Update DP table backwards to use each coin at most once
    for (int coin : coins) {
        for (int sum = total; sum >= coin; sum--) {
            if (dp[sum - coin]) {
                dp[sum] = true;
            }
        }
    }
        
    vector<int> possibleSums;
    for (int s = 1; s <= total; s++) {
        if (dp[s]) {
            possibleSums.push_back(s);
        }
    }
        
    return possibleSums;
    }

int CountOfLIS(const vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;

    vector<int> length(n, 1), count(n, 1);
    int maxLength = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (length[j] + 1 > length[i]) {
                    length[i] = length[j] + 1;
                    count[i] = count[j];
                } else if (length[j] + 1 == length[i]) {
                    count[i] += count[j];
                }
            }
        }
        maxLength = max(maxLength, length[i]);
    }

    int totalCount = 0;
    for (int i = 0; i < n; i++) {
        if (length[i] == maxLength) {
            totalCount += count[i];
        }
    }

    return totalCount;
} // counts the number of longest increasing subsequences in nums.

vector<int> dijkstra(int n, const vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue; // Skip if we have already found a better path
        for (auto [v, wt] : adj[u]) {
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
} // computes the shortest path from src to all other nodes in a weighted graph using Dijkstra's algorithm.

vector<int> bellman_ford(int n, const vector<Edge>& edges, int src) {
    vector<long long> dist(n + 1, LLONG_MAX);
    dist[src] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (const auto& e : edges) {
            if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
            }
        }
    }

    // Check for negative weight cycles
    for (const auto& e : edges) {
        if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) {
            throw runtime_error("Graph contains a negative weight cycle");
        }
    }

    return vector<int>(dist.begin() + 1, dist.end()); // Return distances excluding the 0th index
} // computes the shortest path from src to all other nodes in a weighted graph using the Bellman-Ford algorithm, and detects negative weight cycles.

vector<vector<int>> floyd_warshall(int n, const vector<vector<int>>& graph) {
    vector<vector<int>> dist = graph; // Initialize distance matrix with the input graph
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
} // computes the shortest paths between all pairs of vertices in a weighted graph using the Floyd-Warshall algorithm.

vector<int> lexicographical_topo_sort(int n,const vector<vector<int>>& adj,vector<int> indegree) {

    priority_queue<int> pq;   // max heap

    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0)
            pq.push(i);
    }

    vector<int> order;

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();

        order.push_back(u);

        for (int v : adj[u]) {
            if (--indegree[v] == 0)
                pq.push(v);
        }
    }

    if ((int)order.size() != n)
        return {};   // cycle exists

    reverse(order.begin(), order.end());

    return order;
} // make sure edges are reversed before calling this function. This will give you the lexicographically largest topological order. If you want the lexicographically smallest topological order, use a min-heap instead of a max-heap.
//    min-heap → lexicographically smallest topological order.
// max-heap → lexicographically largest topological order.