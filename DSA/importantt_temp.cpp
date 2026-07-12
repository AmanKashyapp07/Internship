/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
 */

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
      K = __lg(n) + 1;
      st.assign(K, vector<int>(n));
      st[0] = a;
      for (int j = 1; j < K; j++)
         for (int i = 0; i + (1<<j) <= n; i++)
            st[j][i] = min(st[j-1][i], st[j-1][i+(1<<(j-1))]);
    }

    int query(int l, int r) {
        int j = __lg(r - l + 1);
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

/*
Usage:
  BinaryLifting bl(n, root, adj);
  int node_lca = bl.lca(u, v);
  int ancestor = bl.lift(u, k);
  int dist = bl.depth[u] + bl.depth[v] - 2 * bl.depth[node_lca];
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
};


/*
Usage:
  Trie trie;
  trie.insert(x);
  trie.remove(x);          // Safely removes x from the trie
  trie.maxXor(x);          // Returns maximum value of (x ^ y)
  trie.countLess(x, k);    // Returns count of elements where (x ^ y) < k
*/
struct Nodee {
    Nodee *c[2] = {};
    int cnt = 0;
};

struct Trie {
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
  Trie trie;               // Initialize character String Trie (lowercase 'a'-'z')
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

struct Trie {
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
    int n; vector<vector<int>> adj, radj, sccs, dag; vector<int> order, comp; vector<bool> vis;
    SCC(int n) : n(n), adj(n + 1), radj(n + 1), comp(n + 1, -1), vis(n + 1) {}
    
    void add(int u, int v) { adj[u].push_back(v); radj[v].push_back(u); }
    
    void dfs1(int u) {
        vis[u] = 1;
        for (int v : adj[u]) if (!vis[v]) dfs1(v);
        order.push_back(u);
    }
    void dfs2(int u, int c) {
        comp[u] = c; sccs[c].push_back(u);
        for (int v : radj[u]) if (comp[v] == -1) dfs2(v, c);
    }
    vector<vector<int>> build() {
        for (int i = 1; i <= n; i++) if (!vis[i]) dfs1(i);
        for (int i = n - 1; i >= 0; i--) if (comp[order[i]] == -1) {
            sccs.emplace_back(); dfs2(order[i], sccs.size() - 1);
        }
        return sccs;
    }
    vector<vector<int>> get_dag() {
        dag.resize(sccs.size());
        for (int u = 1; u <= n; u++) for (int v : adj[u])
            if (comp[u] != comp[v]) dag[comp[u]].push_back(comp[v]);
        for (auto& neighbors : dag) { // De-duplicate edges
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


