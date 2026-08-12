
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

class BipartiteGraph
{
private:
    int n, L, R;
    vector<vector<int>> graph;     // For 2-coloring (Undirected)
    vector<vector<int>> g_match; // For Matching (Directed: L -> R)
    vector<int> color;
    vector<int> match_R, match_L, vis;

    bool dfs_color(int u, int c)
    {
        color[u] = c;
        for (int v : graph[u])
        {
            if (color[v] == -1)
            {
                if (!dfs_color(v, c ^ 1))
                    return false;
            }
            else if (color[v] == c)
            {
                return false;
            }
        }
        return true;
    }

    bool augment(int u)
    {
        if (vis[u])
            return false;
        vis[u] = 1;
        for (int v : g_match[u])
        {
            if (match_R[v] == 0 || augment(match_R[v]))
            {
                match_R[v] = u;
                match_L[u] = v; // Track left matching for Kőnig's
                return true;
            }
        }
        return false;
    }

    void dfs_vertex_cover(int u, vector<bool> &vis_L, vector<bool> &vis_R)
    {
        vis_L[u] = true;
        for (int v : g_match[u])
        {
            if (!vis_R[v])
            {
                vis_R[v] = true;
                if (match_R[v] != 0 && !vis_L[match_R[v]])
                {
                    dfs_vertex_cover(match_R[v], vis_L, vis_R);
                }
            }
        }
    }

public:
    // Initialize for Bipartite checking (1-indexed, size N)
    BipartiteGraph(int total_nodes) : n(total_nodes), L(0), R(0), graph(total_nodes + 1), color(total_nodes + 1, -1) {}

    // Initialize for Matching (1-indexed, L and R sizes)
    BipartiteGraph(int left_size, int right_size)
        : n(left_size + right_size),
          L(left_size),
          R(right_size),
          g_match(left_size + 1),
          match_R(right_size + 1, 0),
          match_L(left_size + 1, 0) {}

    // Add undirected edge for Bipartite coloring check
    void add_undirected_edge(int u, int v)
    {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Add directed edge (Left -> Right) for Matching algorithms
    void add_matching_edge(int left, int right)
    {
        g_match[left].push_back(right);
    }

    // ---------------------------------------------------------
    // 1. BIPARTITE CHECK (2-Coloring)
    // ---------------------------------------------------------
    bool is_bipartite()
    {
        for (int i = 1; i <= n; i++)
        {
            if (color[i] == -1 && !dfs_color(i, 0))
            {
                return false;
            }
        }
        return true;
    }

    // ---------------------------------------------------------
    // 2. MAXIMUM MATCHING (Kuhn's Algorithm - O(VE))
    // ---------------------------------------------------------
    int maximum_matching()
    {
        int matches = 0;
        match_R.assign(R + 1, 0);
        match_L.assign(L + 1, 0);

        for (int i = 1; i <= L; i++)
        {
            vis.assign(L + 1, 0);
            if (augment(i))
                matches++;
        }
        return matches;
    }

    // ---------------------------------------------------------
    // 3. MINIMUM VERTEX COVER (Kőnig's Theorem)
    // ---------------------------------------------------------
    pair<vector<int>, vector<int>> minimum_vertex_cover()
    {
        maximum_matching();

        vector<bool> vis_L(L + 1, false), vis_R(R + 1, false);

        // Start DFS from all unmatched nodes in Left
        for (int i = 1; i <= L; i++)
        {
            if (match_L[i] == 0 && !vis_L[i])
            {
                dfs_vertex_cover(i, vis_L, vis_R);
            }
        }

        vector<int> cover_L, cover_R;
        // MVC = (Unvisited Left) + (Visited Right)
        for (int i = 1; i <= L; i++)
            if (!vis_L[i])
                cover_L.push_back(i);
        for (int i = 1; i <= R; i++)
            if (vis_R[i])
                cover_R.push_back(i);

        return {cover_L, cover_R};
    }

    // ---------------------------------------------------------
    // 4. MAXIMUM INDEPENDENT SET (Complement of Min Vertex Cover)
    // ---------------------------------------------------------
    pair<vector<int>, vector<int>> maximum_independent_set()
    {
        auto [cover_L, cover_R] = minimum_vertex_cover();

        vector<bool> in_cover_L(L + 1, false), in_cover_R(R + 1, false);
        for (int u : cover_L)
            in_cover_L[u] = true;
        for (int v : cover_R)
            in_cover_R[v] = true;

        vector<int> ind_L, ind_R;
        // MIS = All Vertices - Minimum Vertex Cover
        for (int i = 1; i <= L; i++)
            if (!in_cover_L[i])
                ind_L.push_back(i);
        for (int i = 1; i <= R; i++)
            if (!in_cover_R[i])
                ind_R.push_back(i);

        return {ind_L, ind_R};
    }
};

struct AVLTree {
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    void updateHeight(Node* node) {
        if (node)
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    int getBalance(Node* node) {
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateRight(Node* root) {
        Node* newRoot = root->left;
        Node* subtree = newRoot->right;

        newRoot->right = root;
        root->left = subtree;

        updateHeight(root);
        updateHeight(newRoot);

        return newRoot;
    }

    Node* rotateLeft(Node* root) {
        Node* newRoot = root->right;
        Node* subtree = newRoot->left;

        newRoot->left = root;
        root->right = subtree;

        updateHeight(root);
        updateHeight(newRoot);

        return newRoot;
    }

    Node* insert(Node* root, int value) {
        if (!root) return new Node(value);

        if (value < root->value)
            root->left = insert(root->left, value);
        else if (value > root->value)
            root->right = insert(root->right, value);
        else
            return root;

        updateHeight(root);

        // LL
        if (getBalance(root) > 1 && value < root->left->value)
            return rotateRight(root);

        // RR
        if (getBalance(root) < -1 && value > root->right->value)
            return rotateLeft(root);

        // LR
        if (getBalance(root) > 1 && value > root->left->value) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // RL
        if (getBalance(root) < -1 && value < root->right->value) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    int sumOfLeaveNodes(Node* root) {
        if (!root) return 0;
        if (!root->left && !root->right) return root->value;
        return sumOfLeaveNodes(root->left) + sumOfLeaveNodes(root->right);
    }

};

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int nodes;
    vector<vector<Edge>> adj;
    vector<int> level, it;

    Dinic(int n): nodes(n), adj(n + 1), level(n + 1), it(n + 1) {}

    // directed edge
    void addEdge(int u, int v, ll cap) {
        adj[u].push_back({v, (int)adj[v].size(), cap});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    }

    // build level graph
    bool bfs(int src, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(src), level[src] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : adj[u])
                if (e.cap && level[e.to] == -1)
                    level[e.to] = level[u] + 1, q.push(e.to);
        }
        return level[sink] != -1;
    }

    // blocking flow
    ll dfs(int u, int sink, ll flow) {
        if (u == sink || !flow) return flow;
        for (int &i = it[u]; i < adj[u].size(); i++) {
            auto &e = adj[u][i];
            if (level[e.to] != level[u] + 1 || !e.cap) continue;
            ll pushed = dfs(e.to, sink, min(flow, e.cap));
            if (pushed)
                return e.cap -= pushed, adj[e.to][e.rev].cap += pushed, pushed;
        }
        return 0;
    }

    // max flow
    ll maxFlow(int src, int sink) {
        ll flow = 0, pushed;
        while (bfs(src, sink)) {
            fill(it.begin(), it.end(), 0);
            while ((pushed = dfs(src, sink, LLONG_MAX)))
                flow += pushed;
        }
        return flow;
    }
};
// maximum flow = capacity of minimum cut
// the absolute max amount of water you can push through network equals the capacity of narrowst bottleneck that cuts off source from sink
// when to apply - when you have a flow network and want to find the maximum amount of flow that can be sent from source to sink or when you want to find the minimum cut in a flow network.
// // Pattern:
// - Maximum amount that can be sent from source to sink.
// - Minimum cut / minimum edges to disconnect s and t.
// - Multiple agents/items moving through a network with capacities.
// - Assignment/matching problems (workers-jobs, students-schools, etc.).
// - Edge/vertex-disjoint paths.
// - Transform constraints into capacities on a graph.



// Computes Z-array of string s using binary search + FastHash (longest common prefix of s and s[i..]).
// Time Complexity: O(N log N), Space Complexity: O(N).
vi z_array(string s) {
    int n = s.size(); vi z(n, 0); if (n == 0) return z;
    FastHash fh(s); z[0] = n;
    for (int i = 1; i < n; i++) {
        int low = 1, high = n - i, ans = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (fh.get(0, mid - 1) == fh.get(i, i + mid - 1)) { ans = mid; low = mid + 1; }
            else high = mid - 1;
        }
        z[i] = ans;
    }
    return z;
}

// Computes KMP Prefix function (pi-array) using the Z-array values.
// Time Complexity: O(N), Space Complexity: O(N).
vi pi_array(string s) {
    vi z = z_array(s); int n = s.size(); vi pi(n, 0);
    for (int i = 1; i < n; i++) {
        for (int len = z[i] - 1; len >= 0; --len) {
            if (pi[i + len] != 0) break;
            pi[i + len] = len + 1;
        }
    }
    return pi;
}

// Computes odd-length and even-length palindrome radii for all centers using FastHash.
// Time Complexity: O(N log N), Space Complexity: O(N).
pair<vi, vi> manacherr(string s) {
    int n = s.size(); vi d1(n, 1), d2(n, 0); if (n == 0) return {d1, d2};
    FastHash fh(s); string s_rev = s; reverse(s_rev.begin(), s_rev.end()); FastHash fh_rev(s_rev);
    auto is_pal = [&](int l, int r) { return l >= 0 && r < n && l <= r && fh.get(l, r) == fh_rev.get(n - 1 - r, n - 1 - l); };
    for (int i = 0; i < n; i++) {
        int low = 2, high = min(i + 1, n - i);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid + 1, i + mid - 1)) { d1[i] = mid; low = mid + 1; } else high = mid - 1;
        }
        low = 1; high = min(i, n - i);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid, i + mid - 1)) { d2[i] = mid; low = mid + 1; } else high = mid - 1;
        }
    }
    return {d1, d2};
}
