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
// 1. SEGMENT TREE (RANGE MAX & BINARY SEARCH)
// =========================================================

struct SegNode {
    int val;
};

struct SegTree {
    int n;
    vector<SegNode> tree;

    SegTree(const vi &a) : n(a.size()), tree(4 * n) {
        build(1, 0, n - 1, a);
    }

    SegNode identity() { return {INT_MIN}; }
    SegNode make_node(int x) { return {x}; }
    SegNode merge(SegNode L, SegNode R) { return {max(L.val, R.val)}; }

    void build(int u, int lo, int hi, const vi &a) {
        if (lo == hi) {
            tree[u] = make_node(a[lo]);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        build(2 * u, lo, mid, a);
        build(2 * u + 1, mid + 1, hi, a);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1]);
    }

    void update(int idx, int val) { update(1, 0, n - 1, idx, val); }

    void update(int u, int lo, int hi, int idx, int val) {
        if (lo == hi) {
            tree[u] = make_node(val);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        if (idx <= mid) update(2 * u, lo, mid, idx, val);
        else update(2 * u + 1, mid + 1, hi, idx, val);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1]);
    }

    int query(int L, int R) { return query(1, 0, n - 1, L, R).val; }

    SegNode query(int u, int lo, int hi, int L, int R) {
        if (hi < L || R < lo) return identity();
        if (L <= lo && hi <= R) return tree[u];
        int mid = lo + (hi - lo) / 2;
        return merge(query(2 * u, lo, mid, L, R), query(2 * u + 1, mid + 1, hi, L, R));
    }

    int find_first(int val) { return find_first(1, 0, n - 1, val); }

    int find_first(int u, int lo, int hi, int val) {
        if (tree[u].val < val) return -1;
        if (lo == hi) return lo;
        int mid = lo + (hi - lo) / 2;
        if (tree[2 * u].val >= val) return find_first(2 * u, lo, mid, val);
        return find_first(2 * u + 1, mid + 1, hi, val);
    }
};
// Interview Explanation:
// - Problem Statement: Support range maximum queries, point updates, and binary searching for the first index with value >= X in an array.
// - Approach: Segment Tree with tree binary search (find_first).
// - Intuition: Divide array into halves recursively; range queries merge child maxima in O(log N); find_first descends left subtree if tree[2*u].val >= val, else right subtree.
// - Complexity: Time: O(N) build, O(log N) point update, range query, and binary search, Space: O(4N) tree node storage.


// =========================================================
// 2. FENWICK TREE (BINARY INDEXED TREE / BIT)
// =========================================================

struct Fenwick {
    int n;
    vi bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int i, int val) {
        i++;
        while (i <= n) {
            bit[i] += val;
            i += i & -i;
        }
    }

    int query(int i) {
        int s = 0;
        i++;
        while (i >= 1) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }

    int query(int l, int r) {
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }

    int find(int k) {
        int lo = 0, hi = n - 1, ans = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (query(mid) >= k) {
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain an array supporting point additions, prefix/range sum queries, and lower bound binary search in O(log N).
// - Approach: Fenwick Tree (Binary Indexed Tree) with lowest set bit navigation (i & -i).
// - Intuition: Each index i stores partial sums for a range of length i & (-i); binary search finds the smallest index achieving prefix sum >= k.
// - Complexity: Time: O(log N) update and query, Space: O(N) table storage.


// =========================================================
// 3. COORDINATE COMPRESSION
// =========================================================

struct CoordinateCompressor {
    vi vals;

    void add(int x) { vals.push_back(x); }

    void build() {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }

    int get(int x) {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    }

    int size() { return vals.size(); }
};
// Interview Explanation:
// - Problem Statement: Map a set of arbitrary large/sparse integer coordinates to a dense contiguous rank range [0, K-1].
// - Approach: Sorting + Duplicate removal (std::unique) + Binary Search (std::lower_bound).
// - Intuition: Preserves relative ordering between coordinates while shrinking the values into [0, K-1], enabling Fenwick/Segment Trees on sparse coordinates.
// - Complexity: Time: O(N \log N) sorting, O(\log K) rank lookup, Space: O(N) storage.



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
// - Problem Statement: Compute the radii of all odd-length and even-length palindromes centered at every position in a string.
// - Approach: Binary Search on Rolling Hashes (Forward vs. Reversed String Hashes).
// - Intuition: Substring s[l...r] is a palindrome iff its forward hash equals its reverse hash; binary searching the radius finds maximal palindromic expansions in O(log N).
// - Complexity: Time: O(N \log N) binary search over hashes, Space: O(N) for forward and reverse hash tables.


// =========================================================
// 15. TARJAN'S BRIDGE DETECTION & COMPONENT SEPARATION
// =========================================================

struct Bridge {
    int timer;
    vi tin, tout, low, parent;
    vector<pair<int, int>> bridges;

    void dfs(int u, int p, vvi &g) {
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

    void build(vvi &g) {
        int n = g.size();
        timer = 0;
        tin.assign(n, 0);
        tout.assign(n, 0);
        low.assign(n, 0);
        parent.assign(n, -1);
        bridges.clear();

        for (int i = 0; i < n; i++) {
            if (!tin[i]) dfs(i, -1, g);
        }
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    bool separated(int u, int v, int a, int b) {
        if (parent[u] == v) swap(u, v);
        return isAncestor(v, a) != isAncestor(v, b);
    }
};
// Interview Explanation:
// - Problem Statement: Find all bridges (critical cut-edges whose removal increases the number of connected components) and query edge-separation in O(1).
// - Approach: Tarjan's Bridge-Finding Algorithm via DFS discovery times (tin) and low-link values (low).
// - Intuition: Edge (u, v) is a bridge iff low[v] > tin[u], meaning subtree at v has no back-edge reaching an ancestor of u.
// - Complexity: Time: O(V + E) single DFS pass, Space: O(V + E) for tree and low-link arrays.


// =========================================================
// 16. TARJAN'S ARTICULATION POINTS & CUT-VERTEX SEPARATION
// =========================================================

struct Articulation {
    int timer;
    vi tin, tout, low, par;
    vvi child;

    void dfs(int u, int p, vvi &g) {
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

    void build(vvi &g) {
        int n = g.size();
        timer = 0;
        tin.assign(n, 0);
        tout.assign(n, 0);
        low.assign(n, 0);
        par.assign(n, -1);
        child.assign(n, {});

        for (int i = 0; i < n; i++) {
            if (!tin[i]) dfs(i, -1, g);
        }
    }

    bool isAncestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    int findChild(int u, int x) {
        int l = 0, r = (int)child[u].size() - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            int v = child[u][mid];
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
// Interview Explanation:
// - Problem Statement: Find all articulation points (cut-vertices whose removal disconnects the graph) and determine if cutting vertex C separates vertices A and B.
// - Approach: Tarjan's Articulation Algorithm + DFS Tree Ancestor Binary Search.
// - Intuition: Non-root vertex u is an articulation point if it has a child v with low[v] >= tin[u]; ancestor intervals determine component separation after cut.
// - Complexity: Time: O(V + E) build, O(\log \text{deg}) per separation query, Space: O(V + E) for tree representation.


