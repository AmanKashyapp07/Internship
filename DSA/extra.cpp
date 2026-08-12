
/*
===============================================================================
    ULTIMATE DECISION CHEAT SHEET: SEGMENT TREE VS. FENWICK TREE (BIT)
===============================================================================

| Feature                | Segment Tree                        | Fenwick Tree (BIT)                |
|------------------------|-------------------------------------|-----------------------------------|
| Space Complexity       | O(4N) (Memory Heavy)                | O(N) (4x more memory efficient)   |
| Time Complexity        | Build: O(N), Query/Update: O(log N) | Build: O(N), Query/Update: O(log N)|
| Constant Factor / Speed| Slower (Recursion overhead)         | Blazing Fast (Bitwise iterations)  |
| Code Length            | Long & Verbose (~80 lines)          | Short & Compact (~20 lines)       |
| Invertible Ops (+, ^)  | Yes                                 | Yes (Fastest choice)              |
| Non-Invertible (Min/Max| Yes (Through custom nodes/merge)    | No (Cannot do range min/max/GCD)  |
| Range Updates (Lazy)   | Yes (Supports Set, Add, Flip, AP)   | Limited (Only via difference array)|
| Tree Binary Search     | Yes (Find first index in O(log N))  | Yes (Binary lifting in O(log N))  |

DECISION FLOWCHART:
1. Is it a Range Min / Max / GCD query?
   👉 Use SEGMENT TREE.
2. Does it require binary search on the tree (e.g., "find leftmost index >= X")?
   👉 Use SEGMENT TREE.
3. Does it require complex range updates (e.g. Set value, linear addition, flip)?
   👉 Use SEGMENT TREE (with Lazy Propagation).
4. Is it a Range Sum / XOR query with point updates?
   👉 Use FENWICK TREE (faster to write, smaller constant factor).
5. Does it require offline query processing for counting/distinct?
   👉 Use FENWICK TREE (lighter, runs faster).
===============================================================================
*/

// ============================================================================
// THEME 1: SEGMENT TREE
// ============================================================================
/*
===============================================================================
SEGMENT TREE MERGE CHEAT SHEET (IMPORTANT ONLY)
===============================================================================

1. RANGE MINIMUM
------------------------------------------------------------------------------
Node:       { int val; }
Identity:   { INT_MAX }
Make:       { x }
Merge:      { min(L.val, R.val) }

2. RANGE MAXIMUM
------------------------------------------------------------------------------
Node:       { int val; }
Identity:   { INT_MIN }
Make:       { x }
Merge:      { max(L.val, R.val) }

3. RANGE SUM
------------------------------------------------------------------------------
Node:       { long long sum; }
Identity:   { 0 }
Make:       { x }
Merge:      { L.sum + R.sum }

4. RANGE GCD
------------------------------------------------------------------------------
Node:       { int val; }
Identity:   { 0 }
Make:       { x }
Merge:      { gcd(L.val, R.val) }

5. RANGE XOR
------------------------------------------------------------------------------
Node:       { int val; }
Identity:   { 0 }
Make:       { x }
Merge:      { L.val ^ R.val }

6. MINIMUM + FREQUENCY
------------------------------------------------------------------------------
Node:       { int mn, cnt; }
Identity:   { INT_MAX, 0 }
Make:       { x, 1 }

Merge:
    if (L.mn < R.mn) return L;
    if (R.mn < L.mn) return R;
    return {L.mn, L.cnt + R.cnt};

7. MAXIMUM PREFIX SUM
------------------------------------------------------------------------------
Node:       { ll sum, pref; }
Identity:   { 0, 0 }
Make:       { x, max(0LL, x) }

Merge:
{
    L.sum + R.sum,
    max(L.pref, L.sum + R.pref)
}

8. MAXIMUM SUFFIX SUM
------------------------------------------------------------------------------
Node:       { ll sum, suff; }
Identity:   { 0, 0 }
Make:       { x, max(0LL, x) }

Merge:
{
    L.sum + R.sum,
    max(R.suff, R.sum + L.suff)
}

9. MAXIMUM SUBARRAY SUM
------------------------------------------------------------------------------
Node:       { ll sum, pref, suff, ans; }
Identity:   { 0, 0, 0, 0 }
Make:       { x, max(0LL,x), max(0LL,x), max(0LL,x) }

Merge:
sum  = L.sum + R.sum;
pref = max(L.pref, L.sum + R.pref);
suff = max(R.suff, R.sum + L.suff);
ans  = max({L.ans, R.ans, L.suff + R.pref});

10. MAXIMUM SUBARRAY (NON-EMPTY)
------------------------------------------------------------------------------
Identity:   { -INF, -INF, -INF, -INF }
Make:       { x, x, x, x }

Same merge as above.

11. BALANCED PARENTHESES
------------------------------------------------------------------------------
Node:       { int open, close, match; }
Identity:   { 0, 0, 0 }

Make:
'(' -> {1,0,0}
')' -> {0,1,0}

Merge:
t = min(L.open, R.close);

match = L.match + R.match + t;
open  = L.open  + R.open  - t;
close = L.close + R.close - t;

Answer = 2 * match.

12. DISTINCT CHARACTERS (BITMASK)
------------------------------------------------------------------------------
Node:       { int mask; }
Identity:   { 0 }
Make:       { 1 << (c-'a') }
Merge:      { L.mask | R.mask }

Answer:
__builtin_popcount(mask);
} */


struct Node {
    int val; 
};

struct SegTree {
    int n;
    vector<Node> tree;

    SegTree(const vector<int>& a) : n(a.size()), tree(4 * n) {
        build(1, 0, n - 1, a);
    }

    Node identity() { return { INT_MIN }; }

    Node make_node(int x) { return { x }; }

    Node merge(Node L, Node R) {
        return { max(L.val, R.val) };
    }

    // Build the tree from the initial array
    void build(int u, int lo, int hi, const vector<int>& a) {
        if (lo == hi) {
            tree[u] = make_node(a[lo]);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        
        build(LC, lo, mid, a); 
        build(RC, mid + 1, hi, a);
        tree[u] = merge(tree[LC], tree[RC]);
    }

    // Point update: set array[idx] = val
    void update(int idx, int val) { update(1, 0, n - 1, idx, val); }

    void update(int u, int lo, int hi, int idx, int val) {
        if (lo == hi) { 
            tree[u] = make_node(val); 
            return; 
        }
        int mid = lo + (hi - lo) / 2;
        
        if (idx <= mid) update(LC, lo, mid, idx, val);
        else update(RC, mid + 1, hi, idx, val);
        tree[u] = merge(tree[LC], tree[RC]);
    }

    // Range maximum query for interval [L, R]
    int query(int L, int R) { return query(1, 0, n - 1, L, R).val; }

    Node query(int u, int lo, int hi, int L, int R) {
        if (hi < L || R < lo) return identity(); // out of bounds
        if (L <= lo && hi <= R) return tree[u]; // fully contained
        int mid = lo + (hi - lo) / 2; 
        
        return merge(query(LC, lo, mid, L, R), query(RC, mid + 1, hi, L, R)); // combine results
    }

    // Binary search on tree: Finds the first index in [0, n-1] with value >= val
    int find_first(int val) { return find_first(1, 0, n - 1, val); }

    int find_first(int u, int lo, int hi, int val) {
        if (tree[u].val < val) return -1;
        if (lo == hi) return lo; // found the index
        int mid = lo + (hi - lo) / 2;
        
        if (tree[LC].val >= val) return find_first(LC, lo, mid, val);
        return find_first(RC, mid + 1, hi, val);
    }
};


/*
--------------------------------------------------------------------------------
SEGMENT TREE SUMMARY & OA TRICKS:
--------------------------------------------------------------------------------
- When to Use: Dynamic range queries with range updates (Lazy Prop) or non-invertible
  operations (Range Max, Min, GCD, Max Subarray Sum).
- Build: O(N), Update: O(log N), Query: O(log N). Space: O(4N).
  ```

*/

// ============================================================================
// THEME 2: FENWICK TREE (BIT)
// ============================================================================

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1) {}
    
    void update(int i, int val) {
        i++; // because Fenwick is 1-indexed
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
    int find(int k){
        int lo = 0, hi = n - 1, ans = -1;
        while(lo <= hi){
            int mid = (lo + hi) / 2;
            if(query(mid) >= k){
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        return ans;
    } // find left_child index with prefix sum >= k in O(log N) time
};

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
--------------------------------------------------------------------------------
FENWICK TREE (BIT) SUMMARY & OA TRICKS:
--------------------------------------------------------------------------------
- When to Use: Prefix/range queries on invertible operations (Sum, XOR) with point updates.
- Space Complexity: O(N) (extremely memory-efficient vs Segment Tree's O(4N)).
- Key Advantages: Shorter code, faster execution, small constant factor.
- Range Updates & Point Queries: Maintain Fenwick over diff array. Update [L, R] by +V:
  `update(L, V)` and `update(R + 1, -V)`. Query P: `query(P)`.
  ```
--------------------------------------------------------------------------------
*/

// ============================================================================
// THEME 3: EULER TOUR / TREE FLATTENING
// ============================================================================


struct EulerTour {
    int n, timer;
    vector<int> tin, tout;
    
    EulerTour(int n) : n(n), timer(0), tin(n + 1, 0), tout(n + 1, 0) {}

    // Subtree flattening (Size N) -> Range [tin[u], tout[u]]
    // Maps node values directly to a 0-indexed array for a Segment Tree
    void dfs_subtree(int u, int p, const vector<vector<int>>& g, vector<int>& flat, const vector<int>& val) {
        tin[u] = ++timer; 
        flat[timer - 1] = val[u];
        
        for (int v : g[u]) {
            if (v != p) dfs_subtree(v, u, g, flat, val);
        }
        tout[u] = timer;
    }

    // Path flattening (Size 2N) -> Prefix sum to tin[u] gives Path(root -> u)
    void dfs_path(int u, int p, const vector<vector<int>>& g, vector<int>& flat, const vector<int>& val) {
        tin[u] = ++timer; 
        flat[timer - 1] = val[u];
        
        for (int v : g[u]) {
            if (v != p) dfs_path(v, u, g, flat, val);
        }
        tout[u] = ++timer; 
        flat[timer - 1] = -val[u];
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

/*
===============================================================================
    EULER TOUR MAPPING TO SEGMENT / FENWICK TREE & POPULAR VARIANTS
===============================================================================

1. HOW TO MAP SUBTREE FLATTENING (Variant A):
   - Preprocessing:
     ```cpp
     vector<int> flat(n);
     et.dfs_subtree(root, 0, graph, flat, val);
     // Now, building SegTree or Fenwick Tree over flat array:
     SegTree st(flat); // or Fenwick ft(n); for (int i = 0; i < n; i++) ft.update(i, flat[i]);
     ```
   - Point Update (Change node U value to X):
     ```cpp
     st.update(et.tin[u] - 1, x); // For SegTree
     // For Fenwick, update by delta = x - current_val[u]:
     ft.update(et.tin[u] - 1, x - current_val[u]);
     current_val[u] = x;
     ```
   - Subtree Query on Node U:
     ```cpp
     long long ans = st.query(et.tin[u] - 1, et.tout[u] - 1);
     ```

2. HOW TO MAP PATH FLATTENING (Variant B):
   - Preprocessing:
     ```cpp
     vector<int> flat(2 * n);
     et.dfs_path(root, 0, graph, flat, val);
     SegTree st(flat); // or Fenwick over 2*n size
     ```
   - Point Update (Change node U value to X):
     ```cpp
     // Update entry with new value, and exit with negative new value
     st.update(et.tin[u] - 1, x);
     st.update(et.tout[u] - 1, -x);
     // For Fenwick, update by delta = x - current_val[u]:
     ft.update(et.tin[u] - 1, x - current_val[u]);
     ft.update(et.tout[u] - 1, -(x - current_val[u]));
     current_val[u] = x;
     ```
   - Root-to-Node Path Query on Node U:
     ```cpp
     long long path_sum = st.query(0, et.tin[u] - 1);
     ```

===============================================================================
    POPULAR VARIANTS ASKED IN ASSESSMENT:
===============================================================================

A. Subtree Queries with Range Updates (CSES *Range Updates and Sums* style on trees):
   - "Add X to all nodes in the subtree of U."
   - Mapping: Contiguous range update on `[tin[U] - 1, tout[U] - 1]`.
   - Setup: Use a **Lazy Segment Tree** (with Range Add) over the flattened subtree array.

B. Path Queries between Any Two Nodes U and V:
   - "Query the path sum between node U and node V."
   - Mapping: The sum of values on path U <-> V is:
     `Path(root, U) + Path(root, V) - 2 * Path(root, LCA) + val[LCA]`
   - Setup: 
     1. Run `dfs_path()` to flatten root-paths.
     2. Query prefix path sum using `query(0, tin[U] - 1)` and `query(0, tin[V] - 1)`.
     3. Find LCA(U, V) using **Binary Lifting**.

C. Subtree Max / Min Queries:
   - "Find the maximum node value in the subtree of U."
   - Mapping: Query range `[tin[U] - 1, tout[U] - 1]` over a **Max Segment Tree** built on the flattened subtree array. (Note: Fenwick Tree cannot be used here because Max is non-invertible).
===============================================================================
*/



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
