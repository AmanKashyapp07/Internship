/*
================================================================================
      ULTIMATE DATA STRUCTURE SELECTOR: SEGMENT TREE vs FENWICK vs SPARSE TABLE
================================================================================

1. QUICK COMPARISON TABLE
--------------------------------------------------------------------------------
Feature           | Sparse Table          | Fenwick Tree (BIT)    | Segment Tree
--------------------------------------------------------------------------------
Array State       | Static (No updates)   | Dynamic (Point upd)   | Dynamic (Range/Point upd)
Build Time        | O(N log N)            | O(N)                  | O(N)
Point Update      | N/A                   | O(log N)              | O(log N)
Range Update      | N/A                   | O(log N) (with diff)  | O(log N) (Lazy Prop)
Range Query       | O(1) (Idempotent)     | O(log N) (Invertible) | O(log N) (Associative)
Memory Complexity | O(N log N)            | O(N)                  | O(N) (typically 4N)
Operations        | Min, Max, GCD         | Sum, XOR, Mult        | Any merge operation
Code Complexity   | Very Low              | Low / Compact         | Medium to High
--------------------------------------------------------------------------------

2. DECISION FLOWCHART
--------------------------------------------------------------------------------
                  Is the array STATIC (no updates)?
                       /                     \
                     YES                      NO
                     /                         \
       Is query idempotent?            Do you need RANGE UPDATES?
       (e.g., Min, Max, GCD)            /                     \
             /       \                YES                      NO
           YES        NO              /                         \
           /           \        SEGMENT TREE           Is query invertible?
     SPARSE TABLE    FENWICK   (Lazy Propagation)      (e.g., Sum, XOR)
     [O(1) Query]   or SEGTREE                          /             \
                    [O(log N)]                        YES              NO
                                                      /                 \
                                                FENWICK TREE       SEGMENT TREE
                                                [Fast & Small]     (Point update)

3. DETAILED ARCHITECTURE & QUESTION TYPES

A. SPARSE TABLE (Static & Idempotent Range Queries)
   - When: Array has no updates. Query operations are idempotent: f(x, x) = x.
   - Query Time: O(1)
   - LeetCode / CSES Examples:
     * CSES Static Range Minimum Queries: O(1) Range Min.
     * CSES Distance Queries (LCA): Find LCA of two nodes using Euler Tour + RMQ on depth.
     * Range GCD queries on a static array.

B. FENWICK TREE / BIT (Dynamic Prefix/Range Queries on Invertible Operations)
   - When: Dynamic array with point updates. Operation MUST be invertible: f(a, b) = a + b 
     where inverse exists (e.g. subtraction for sum, XOR is self-inverse).
   - Code/Speed: Minimal code, extremely small constant factor, low memory footprint.
   - Special Trick: O(log N) binary lifting to find k-th active elements.
   - LeetCode / CSES Examples:
     * CSES Range Sum Queries II: Standard point update, range sum query.
     * CSES Forest Queries II: 2D Fenwick Tree for grid sum updates.
     * CSES List Removals: Track active elements, use binary lifting on BIT in O(log N).
     * LeetCode 307 (Range Sum Query - Mutable): Perfect BIT candidate.
     * LeetCode 315 (Count of Smaller Numbers After Self): Traverse right to left, update frequencies.

C. SEGMENT TREE (Dynamic, Range Updates, Non-Invertible Operations, Complex Merges)
   - When: Dynamic array. Need range updates (requires Lazy Propagation), or the operation is 
     non-invertible (e.g., Range Max/Min with updates, Range GCD with updates).
   - Dynamic Segment Tree: For online queries when coordinate range is huge (up to 10^9) 
     and coordinate compression is not feasible or online queries are required.
   - Persistent Segment Tree: Querying historical versions of the tree (e.g., range queries 
     after specific updates, k-th smallest element in range [L, R]).
   - LeetCode / CSES Examples:
     * CSES Range Minimum Queries II: Min is non-invertible, must use SegTree.
     * CSES Range Update Queries: Range update, point query (can use Fenwick with diff array, but SegTree is standard).
     * CSES Hotel Queries: Binary search on the SegTree to find first hotel with capacity >= X in O(log N).
     * CSES Subarray Sum Queries: Node stores sum, max prefix, max suffix, and max subarray sum.
     * CSES Polynomial Queries: Range updates with linear polynomial functions (Lazy Prop).
     * LeetCode 715 (Range Module): Lazy propagation + Dynamic Node allocation.
     * LeetCode 699 (Falling Squares): Range maximum query and range update.

================================================================================
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
            O.A. CHEAT SHEET: EULER TOUR / TREE FLATTENING REFERENCE
================================================================================
✔ IDENTIFICATION: Static tree (no edge add/remove) requiring Subtree/Path queries.

[VARIANT A] Subtree Queries & Updates (Size N Array)
  - Layout: Entire subtree of U is contiguous in range [tin[U], tout[U]].
  - Fenwick/SegTree Setup:
      Initialize: For each i ∈ [1, N]: `ft.upd(et.tin[i], val[i]);`
      Point Update node U to value x: `ft.upd(et.tin[U], x - val[U]); val[U] = x;`
      Subtree Query U: `ft.qry(et.tin[U], et.tout[U]);` (Range query)

[VARIANT B] Root-to-Node Path Queries (Size 2N Array)
  - Layout: `+val[U]` at `tin[U]`, `-val[U]` at `tout[U]`. Non-ancestors cancel out to 0.
  - Fenwick/SegTree Setup:
      Initialize: For each i ∈ [1, 2N]: `ft.upd(i, flat_val[i]);`
      Point Update node U to value x: 
          `ft.upd(et.tin[U], x - val[U]);` 
          `ft.upd(et.tout[U], val[U] - x); val[U] = x;`
      Root to U Path Query: `ft.qry(et.tin[U]);` (Prefix sum up to tin[U])
  - Path Query between U and V: 
      `PathSum(U, V) = Path(root, U) + Path(root, V) - 2 * Path(root, LCA) + val[LCA]`

[VARIANT C] LCA via RMQ (Size 2N-1 Array)
  - Layout: Record node index on every visit (entry, between children, exit).
  - Sparse Table Setup: 
      Build a minimum Sparse Table over the generated `depth` array values.
      `LCA(U, V)` is the node in `euler_lca` with the minimum depth in range 
      `[min(first_occ[U], first_occ[V]), max(first_occ[U], first_occ[V])]`.
================================================================================
*/

struct EulerTour {
    int n, timer;
    vector<int> tin, tout, depth, first_occ, euler_lca;

    EulerTour(int n) {
        this->n = n; timer = 0;
        tin.assign(n + 1, 0); tout.assign(n + 1, 0);
        depth.assign(n + 1, 0); first_occ.assign(n + 1, 0);
    }

    // 1. Subtree flattening (Size N) -> Range [tin[u], tout[u]]
    // Now constructs flat_val directly at 0-indexed positions matching your SegTree
    void dfs_subtree(int u, int p, const vector<vector<int>>& g, vector<int>& flat_val, const vector<int>& val) {
        tin[u] = ++timer; 
        flat_val[timer - 1] = val[u]; // Maps directly to 0-indexed SegTree array slots
        for (int v : g[u]) if (v != p) dfs_subtree(v, u, g, flat_val, val);
        tout[u] = timer;
    }

    // 2. Path flattening (Size 2N) -> Prefix sum to tin[u] = Path(root -> u)
    void dfs_path(int u, int p, const vector<vector<int>>& g, vector<int>& flat_val, const vector<int>& val) {
        tin[u] = ++timer; flat_val[timer] = val[u];
        for (int v : g[u]) if (v != p) dfs_path(v, u, g, flat_val, val);
        tout[u] = ++timer; flat_val[timer] = -val[u];
    }

    // 3. LCA flattening (Size 2N-1) -> RMQ on depth array over [first_occ[u], first_occ[v]]
    void dfs_lca(int u, int p, int d, const vector<vector<int>>& g) {
        first_occ[u] = euler_lca.size(); euler_lca.push_back(u); depth[u] = d;
        for (int v : g[u]) {
            if (v != p) { dfs_lca(v, u, d + 1, g); euler_lca.push_back(u); }
        }
    }
};

/*
=== DATA STRUCTURE DECISION MATRIX ===
1. Array is STATIC & queries are IDEMPOTENT (min, max, gcd)     -> SPARSE TABLE  [O(1) query, O(NlogN) build]
2. Array is STATIC but queries are NOT idempotent (sum, xor)    -> FENWICK TREE  [O(N) build, simpler than Segment]
3. Array is DYNAMIC & query is point update, prefix/range sum   -> FENWICK TREE  [Fastest, low memory, easy code]
4. Array is DYNAMIC & query involves range updates (Lazy Prop)  -> SEGMENT TREE  [Flexible, standard for range updates]
5. Array is DYNAMIC & range query is non-invertible (min, max)  -> SEGMENT TREE  [Fenwick cannot cleanly do range min/max]
6. Coordinate range is huge (e.g., up to 10^9) & memory tight   -> FENWICK/SEG   [Combine with Coordinate Compression]
7. Indices are generated on the fly (Online & Large Range)      -> DYNAMIC SEG   [Pointers/Implicit Segment Tree]
8. Need to query historical states of the array (Past updates) -> PERSISTENT SEG [Preserves history of versions]
9. Tree structure requires binary lifting to find k-th element -> FENWICK TREE  [O(log N) lifting, faster than Segment]
10. Default Rule of Thumb: If Fenwick *can* do it, use Fenwick. Otherwise, use a Segment Tree.
*/


/*
================================================================================
          O.A. CHEAT SHEET: SEGMENT TREE VARIANTS (MERGE & NODE LOGIC)
================================================================================
✔ IDENTIFICATION: Tree structures or arrays requiring dynamic range operations where 
  only the Node properties, child merges, leaf setups, and identity states change.

VARIANT       | NODE STRUCT      | LEAF INITIALIZATION   | MERGE LOGIC (P = parent, L = left, R = right) | IDENTITY (No Overlap)
--------------------------------------------------------------------------------------------------------------------------------
1. SUM        | ll sum;          | {a[i]}                | P.sum = L.sum + R.sum;                       | {0}
2. MIN        | int mn;          | {a[i]}                | P.mn = min(L.mn, R.mn);                      | {INT_MAX}
3. MAX        | int mx;          | {a[i]}                | P.mx = max(L.mx, R.mx);                      | {INT_MIN}
4. GCD        | int g;           | {a[i]}                | P.g = std::gcd(L.g, R.g);                     | {0}
5. XOR        | int xr;          | {a[i]}                | P.xr = L.xr ^ R.xr;                          | {0}
--------------------------------------------------------------------------------------------------------------------------------
6. MAX PREFIX | ll sum, pref;    | val = max(0LL, x);    | P.sum = L.sum + R.sum;                       | {0, 0}
              |                  | {x, val}              | P.pref = max(L.pref, L.sum + R.pref);        | 
--------------------------------------------------------------------------------------------------------------------------------
7. MAX SUFFIX | ll sum, suff;    | val = max(0LL, x);    | P.sum = L.sum + R.sum;                       | {0, 0}
              |                  | {x, val}              | P.suff = max(R.suff, R.sum + L.suff);        | 
--------------------------------------------------------------------------------------------------------------------------------
8. MAX SUBARR | ll sum, pref,    | v = max(0LL, x);      | P.sum = L.sum + R.sum;                       | {0, 0, 0, 0}
   (GSS1/5)   |    suff, ans;    | {x, v, v, v}          | P.pref = max(L.pref, L.sum + R.pref);        |
              |                  |                       | P.suff = max(R.suff, R.sum + L.suff);        |
              |                  |                       | P.ans = max({L.ans, R.ans, L.suff + R.pref});|
================================================================================
*/

/*
--------------------------------------------------------------------------------
SEGMENT TREE VARIATIONS (CSES & LEETCODE) & HOW TO MODIFY TEMPLATE:
--------------------------------------------------------------------------------
1. Range Min/Max/GCD Queries with Point Updates
   - Problems: CSES Range Minimum Queries II, LeetCode 307
   - Template Modification:
     * In build(), update(), query(): change the combining function (e.g. max to min or std::gcd).
     * In query(): change the out-of-bounds return value (Identity element).
       - Sum/XOR: return 0
       - Min: return INT_MAX
       - Max: return INT_MIN
       - GCD: return 0 (since gcd(0, x) = x)

2. Range Updates (Add/Set val in [L, R]) & Range Queries (Lazy Propagation)
   - Problems: CSES Range Update Queries, CSES Polynomial Queries
   - Template Modification:
     * Add `vector<int> lazy` of size 4N initialized to 0 (or -1/flag for set updates).
     * Add `void push(int node, int lo, int hi)` function:
       ```cpp
       void push(int node, int lo, int hi) {
           if (!lazy[node]) return;
           tree[node] += lazy[node] * (hi - lo + 1); // For Range Sum
           if (lo != hi) {
               lazy[2*node] += lazy[node];
               lazy[2*node+1] += lazy[node];
           }
           lazy[node] = 0;
       }
       ```
     * Call `push(node, lo, hi)` at the beginning of `update(node, ...)` and `query(node, ...)`.
     * Update function will recurse both sides and merge similar to query.

3. Tree-based Binary Search (Find first index in [L, R] with value >= X)
   - Problems: CSES Hotel Queries
   - Template Modification:
     * Add a traversal method that checks child nodes directly instead of standard range query:
       ```cpp
       int query_first(int node, int lo, int hi, int val) {
           if (tree[node] < val) return -1;
           if (lo == hi) return lo;
           int mid = (lo + hi) / 2;
           if (tree[2*node] >= val) return query_first(2*node, lo, mid, val);
           return query_first(2*node+1, mid+1, hi, val);
       }
       ```

4. Dynamic / Implicit Segment Tree (Large Coordinate Range e.g. [0, 10^9])
   - Problems: LeetCode 715 (Range Module), LeetCode 699 (Falling Squares)
   - Template Modification:
     * Replace `vector<int> tree` with a node structure:
       `struct Node { int val; Node *left = nullptr, *right = nullptr; };`
     * Instantiate root `Node* root = new Node();`
     * In update/query, dynamically create child nodes on demand:
       `if (!node->left) node->left = new Node();`

5. Persistent Segment Tree (History queries / K-th smallest in range)
   - Problems: CSES Range Queries and Copies
   - Template Modification:
     * When updating a value, instead of modifying in-place, copy the path from the root.
     * Return a pointer to the new root node for that version.
--------------------------------------------------------------------------------
*/
struct SegTree {
    int n; vector<int> tree;
    SegTree() : n(0) {}

    SegTree(vector<int>& a) { 
        n = a.size(); 
        tree.resize(4 * n); 
        build(1, 0, n - 1, a); 
    }

    void build(int node, int lo, int hi, vector<int>& a) {
        if (lo == hi) { tree[node] = a[lo]; return; }
        int mid = (lo + hi) / 2;
        build(2 * node, lo, mid, a); build(2 * node + 1, mid + 1, hi, a);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int idx, int val) { if (n > 0) update(1, 0, n - 1, idx, val); }

    void update(int node, int lo, int hi, int idx, int val) {
        if (lo == hi) { tree[node] = val; return; }
        int mid = (lo + hi) / 2;
        if (idx <= mid) update(2 * node, lo, mid, idx, val);
        else update(2 * node + 1, mid + 1, hi, idx, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    int query(int l, int r) { return n <= 0 ? INT_MIN : query(1, 0, n - 1, l, r); }

    int query(int node, int lo, int hi, int l, int r) {
        if (hi < l || r < lo) return INT_MIN;
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return max(query(2 * node, lo, mid, l, r), query(2 * node + 1, mid + 1, hi, l, r));
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

--------------------------------------------------------------------------------
FENWICK TREE / BIT VARIATIONS (CSES & LEETCODE) & HOW TO MODIFY TEMPLATE:
--------------------------------------------------------------------------------
1. Range Sum / XOR Queries with Point Updates
   - Problems: CSES Range Sum Queries II, LeetCode 307
   - Template Modification:
     * Standard code works for Sum.
     * For XOR: replace `+=` with `^=` in `update()`.
     * In `query(l, r)`: replace `-` subtraction with `^` (XOR is self-inverse).

2. Range Updates (Add V to [L, R]) & Point Queries
   - Problems: CSES Range Update Queries (when only point query is needed)
   - Template Modification:
     * Maintain Fenwick tree over the difference array D[i] = A[i] - A[i-1].
     * To update range [L, R] by V: call `update(L, V)` and `update(R + 1, -V)`.
     * To query point index P: call `query(P)` (this returns the prefix sum of difference array, which is the value).

3. Range Updates & Range Queries
   - Problems: Dynamic Range Add & Range Sum
   - Template Modification:
     * Maintain two BITs: BIT1 and BIT2.
     * To update [L, R] with V:
       `BIT1.update(L, V); BIT1.update(R + 1, -V);`
       `BIT2.update(L, V * (L - 1)); BIT2.update(R + 1, -V * R);`
     * Prefix sum up to index P is computed as: `BIT1.query(P) * P - BIT2.query(P)`.

4. 2D Fenwick Tree (Grid sum / updates)
   - Problems: CSES Forest Queries II
   - Template Modification:
     * Change `vector<int> bit` to `vector<vector<int>> bit`.
     * Nest loops in both `update` and `query` to traverse grid in O(log N * log M):
       ```cpp
       void update(int x, int y, int val) {
           for (++x; x <= n; x += x & -x)
               for (int y_t = y + 1; y_t <= m; y_t += y_t & -y_t)
                   bit[x][y_t] += val;
       }
       ```

5. O(log N) Binary Lifting / Binary Search on Prefix Sum
   - Problems: CSES List Removals, LeetCode 315
   - Template Modification:
     * Find the index where prefix sum is exactly k in O(log N) instead of O(log^2 N) binary search:
       ```cpp
       int find_kth(int k) {
           int idx = 0;
           for (int i = 1 << 20; i > 0; i >>= 1) { // 1<<20 covers N up to 10^6
               if (idx + i <= n && bit[idx + i] < k) {
                   idx += i;
                   k -= bit[idx];
               }
           }
           return idx; // Returns 0-based index
       }
       ```

6. Nested Ranges Count (Contains and Contained By)
   - Problems: CSES Nested Ranges Count
   - Template Modification:
     * Given ranges [a_i, b_i]. Sort them primarily by start a_i ascending, secondarily by end b_i descending.
     * To compute "how many other ranges contain range i" (Contained By):
       - Iterate left to right on sorted ranges. Maintain a Fenwick tree of active end-points (coordinate compressed).
       - For range i: count how many previously processed ranges have end >= b_i.
       - Query range sum in `[cc.get(b_i), cc.size() - 1]`, then update/add the end-point `update(cc.get(b_i), 1)`.
     * To compute "how many other ranges are contained in range i" (Contains):
       - Iterate right to left on sorted ranges.
       - For range i: count how many previously processed ranges have end <= b_i.
       - Query prefix sum up to `cc.get(b_i)` (i.e. `query(cc.get(b_i))`), then update/add the end-point `update(cc.get(b_i), 1)`.

7. Josephus Problem II (Circle Game skip-k-remove-one)
   - Problems: CSES Josephus Problem II
   - Template Modification:
     * Initialize a Fenwick tree of size N with 1s: `for (int i=0; i<N; i++) ft.update(i, 1);`
     * Maintain the size of active elements `active = N` and current position index `pos = 0`.
     * At each step, compute `next_pos = (pos + k) % active`.
     * Find the original array index of the `(next_pos + 1)`-th active child using the binary lifting method `int idx = ft.find_kth(next_pos + 1);` in O(log N) time.
     * Remove the child with `ft.update(idx, -1)`, set `pos = next_pos`, and decrement `active`.
--------------------------------------------------------------------------------
*/
struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1) {}
    
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
        while (i > 0) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }
    int query(int l, int r) { 
        return query(r) - (l > 0 ? query(l - 1) : 0);
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

--------------------------------------------------------------------------------
SPARSE TABLE VARIATIONS (CSES & LEETCODE) & HOW TO MODIFY TEMPLATE:
--------------------------------------------------------------------------------
1. Range Min/Max/GCD Queries (Idempotent Operations)
   - Problems: CSES Static Range Minimum Queries
   - Template Modification:
     * Change `min` inside the double loops of constructor and the `query` function to `max` or `std::gcd`.

2. Range Sum / XOR Queries (Non-idempotent Operations)
   - Problems: Static range queries where operations are not idempotent but O(log N) query is acceptable.
   - Template Modification:
     * Cannot use standard two-interval overlap O(1) query.
     * Decompose the interval length into powers of 2:
       ```cpp
       long long query_sum(int l, int r) {
           long long sum = 0;
           for (int j = K - 1; j >= 0; j--) {
               if ((1 << j) <= r - l + 1) {
                   sum += st[j][l];
                   l += (1 << j);
               }
           }
           return sum;
       }
       ```

3. Lowest Common Ancestor (LCA) via Euler Tour RMQ
   - Problems: CSES Distance Queries, CSES Company Queries II
   - Template Modification:
     * Flatten the tree using Euler Tour DFS (record nodes and depths).
     * Build Sparse Table on the `depth` array (storing `{depth, node}`).
     * LCA query returns the node index with the minimum depth in the range `[first_occurrence[u], first_occurrence[v]]`.
--------------------------------------------------------------------------------
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

// Change the '+' operations in your SegTree struct to max() or min() to support this
// problem statement - finding the maximum value in the subtree of a given node, and also supporting point updates to change the value of a node. The Fenwick tree is not suitable for this because it can only support sum queries, and max queries are non-commutative, so we need to use a segment tree instead of a Fenwick tree for max queries.
struct SubtreeMaxEngine {
    EulerTour et;
    SegTree seg;

    SubtreeMaxEngine(int n, vector<int>& val, const vector<vector<int>>& g)
        : et(n) {

        vector<int> flat(n);
        et.dfs_subtree(1, 0, g, flat, val);
        seg = SegTree(flat);
    }

    void update_node(int u, int new_val) {
        seg.update(et.tin[u] - 1, new_val);
    }

    int query_subtree_max(int u) {
        return seg.query(et.tin[u] - 1, et.tout[u] - 1);
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
  BinaryLifting bl(n, root, g);
  int noddelca = bl.lca(u, v);
  int ancestor = bl.lift(u, k);
  int dist = bl.depth[u] + bl.depth[v] - 2 * bl.depth[noddelca];
*/
struct BinaryLifting {
    int LOG;
    vector<vector<int>> up;
    vector<int> depth;

    BinaryLifting(int n, int root, vector<vector<int>>& g) {
        LOG = 32 - __builtin_clz(n);
        up.assign(n + 1, vector<int>(LOG, -1));
        depth.assign(n + 1, 0);
        dfs(root, -1, 0, g);
    }

    void dfs(int u, int p, int d, vector<vector<int>>& g) {
        depth[u] = d; up[u][0] = p;
        for (int j = 1; j < LOG; j++) 
            if (up[u][j - 1] != -1) up[u][j] = up[up[u][j - 1]][j - 1];
        for (int v : g[u]) if (v != p) dfs(v, u, d + 1, g);
    }

    int lift(int u, int k) {
        for (int j = 0; j < LOG && u != -1; j++) if ((k >> j) & 1) u = up[u][j];
        return u;
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        a = lift(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int j = LOG - 1; j >= 0; j--) if (up[a][j] != up[b][j]) a = up[a][j], b = up[b][j];
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
        return -1;
    }
    void dfsCount(int u, int p, vector<int>& cnt, vector<vector<int>>& g) {
        for (int v : g[u]) {
            if (v == p) continue;
            dfsCount(v, u, cnt, g);
            cnt[u] += cnt[v];
        }
    }

    vector<int> countingPaths(int n, vector<pair<int,int>>& paths, vector<vector<int>>& g) {
        vector<int> cnt(n + 1, 0);

        for (auto &[a, b] : paths) {
            int l = lca(a, b);
            cnt[a]++;
            cnt[b]++;
            cnt[l]--;
            if (up[l][0] != -1)
                cnt[up[l][0]]--;
        }

        dfsCount(1, -1, cnt, g);

        return cnt;
    }
};

/*
Usage:
  XorTrie trie;
  trie.insert(x);
  trie.remove(x);
  trie.maxXor(x);
  trie.countLess(x, k);
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
    }
    void remove(int x) { insert(x, -1); }

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
  StringTrie trie;
  trie.insert(s);
  trie.remove(s);
  trie.search(s);
  trie.startsWith(s);
  trie.countEqual(s);
  trie.countPrefix(s);
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
    void remove(string s) { insert(s, -1); }

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

// A directed graph consists of n nodes and m edges. The edges are numbered 1,2,\dots,n.
//Your task is to answer q queries of the form "can you reach node b from node a?"
// for this question we can use the DirectedGraphToDAG class to convert the directed graph into a DAG of its strongly connected components (SCCs). Once we have the DAG, we can perform a reachability analysis on it.


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
    vector<vector<int>> rev; // rev[u] = stores reversed adjacency list (to walk away from cycle roots)
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