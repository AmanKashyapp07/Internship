#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <cmath>
#include <climits>
#include <cstring>
#include <tuple>
#include <utility>

using namespace std;
using ll = long long;
#define LC (2 * u)
#define RC (2 * u + 1)

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


struct BinaryLifting {
    int LOG;
    vector<vector<int>> up;
    vector<int> depth;

    BinaryLifting(int n, int root, const vector<vector<int>> &g) {
        LOG = 32 - __builtin_clz(n);
        up.assign(n + 1, vector<int>(LOG, 0));
        depth.assign(n + 1, 0);
        dfs(root, root, 0, g);
    }

    void dfs(int u, int p, int d, const vector<vector<int>> &g) {
        depth[u] = d;
        up[u][0] = p;
        for (int j = 1; j < LOG; j++) {
            up[u][j] = up[up[u][j - 1]][j - 1];
        }
        for (int v : g[u]) {
            if (v != p) dfs(v, u, d + 1, g);
        }
    }

    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        // 1. Lift U to the same depth as V
        for (int j = LOG - 1; j >= 0; j--) {
            if (depth[u] - (1 << j) >= depth[v]) u = up[u][j];
        }
        if (u == v) return u;
        // 2. Lift both nodes together right below the LCA
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return up[u][0];
    }
};

// ============================================================================
// THEME 4: DIGIT DP
// ============================================================================

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

/*
--------------------------------------------------------------------------------
DIGIT DP TIPS & TRICKS:
--------------------------------------------------------------------------------
- Range [L, R] Queries: Always query `solve(R) - solve(L - 1)`.
- The 'started' flag tracks if we have placed any non-zero digits yet (ignores leading zeros).
- The 'tight' flag restricts us to not exceed the prefix of the bound number.
- Reset the memo array inside the solve function before running the DP.
--------------------------------------------------------------------------------
*/

// ============================================================================
// THEME 5: SUM OVER SUBSETS (SOS) DP
// ============================================================================

struct SOS {
    int B = 20; // max bits
    int N = 1 << B; // max masks
    vector<int> f, sub, sup; // f means frequency of each mask, sub = sum over subsets, sup = sum over supersets

    SOS() : f(N), sub(N), sup(N) {}
    // subsets mean all masks y such that (y | x) == x
    // supersets mean all masks y such that (y & x) == x
    void add(int x) { f[x]++; } // add frequency of mask x

    // Build SOS DP
    void build() {
        sub = sup = f;
        for(auto b = 0; b < B; b++) // for each bit position
            for (int m = 0; m < N; m++)
                if (m >> b & 1) sub[m] += sub[m ^ (1 << b)]; // If bit b is set in m, add the subset without bit b
                else            sup[m] += sup[m | (1 << b)]; // If bit b is not set in m, add the superset with bit b
    }

    // (y | x) == x
    int subsets(int x) { return sub[x]; }
    // (y & x) == x
    int supersets(int x) { return sup[x]; }

    // (y & x) != 0 -> n - disjoint -> n - countSubsets((N-1)^x)
    int disjoint(int x) { return sub[(N - 1) ^ x]; } // disjoint means all masks y such that (y & x) == 0
    int intersect(int x, int n) { return n - disjoint(x); } // intersect means all masks y such that (y & x) != 0
};

/*
--------------------------------------------------------------------------------
SOS DP TIPS & TRICKS:
--------------------------------------------------------------------------------
- Use when masks are <= 20-22 bits.
- common relationships:
  - (y | x) == x  -> subset query
  - (y & x) == x  -> superset query
  - (y & x) == 0  -> disjoint = countSubsets((N-1)^x)
  - (y & x) != 0  -> intersection = N - disjoint
--------------------------------------------------------------------------------
*/


// ============================================================================
// THEME 7: BOTTOM-UP INTERVAL DP
// ============================================================================

long long solveIntervalDPBottomUp(int n) {
    vector<vector<long long>> dp(n, vector<long long>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l+len<=n; l++) {
            int r = l + len - 1;

            dp[l][r] = LLONG_MAX;

            for (int k = l; k < r; k++) {

                long long cur =
                    dp[l][k] +
                    dp[k + 1][r]
                    /* + problem-specific cost(l,k,r) */;

                dp[l][r] = min(dp[l][r], cur);
            }
        }
    }

    return dp[0][n - 1];
}

/*
--------------------------------------------------------------------------------
INTERVAL DP TIPS & TRICKS:
--------------------------------------------------------------------------------
- Matrix Chain Multiplication:
  `dp[l][r] = min(dp[l][k] + dp[k+1][r] + arr[l] * arr[k+1] * arr[r+1])`
- Merge Stones / Slimes:
  `dp[l][r] = min(dp[l][k] + dp[k+1][r]) + prefixSum(l, r)`
- Burst Balloons (Choose LAST balloon k in [l, r]):
  `dp[l][r] = max(dp[l][k-1] + dp[k+1][r] + a[l-1] * a[k] * a[r+1])`
--------------------------------------------------------------------------------
*/

// ============================================================================
// THEME 8: GRAPH CYCLES
// ============================================================================

vector<int> buildCycle(int startNode, int endNode, const vector<int>& parent) {
    vector<int> cycle;
    int cur = endNode;
    while (cur != startNode) {
        cycle.push_back(cur);
        cur = parent[cur];
    }
    cycle.push_back(startNode);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}
    
class UndirectedCycle
{
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> graph;
    vector<int> vis, parent;

    bool dfs(int u, int p)
    {
        vis[u] = 1;
        for (int v : graph[u])
        {
            if (v == p)
                continue;
            if (!vis[v])
            {
                parent[v] = u;
                if (dfs(v, u))
                    return true;
            }
            else
            {
                startNode = v;
                endNode = u;
                return true;
            }
        }
        return false;
    }

public:
    UndirectedCycle(int n) : n(n), graph(n + 1), vis(n + 1, 0), parent(n + 1, -1) {}

    void addEdge(int u, int v)
    {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> getCycle()
    {
        for (int i = 1; i <= n; i++)
        {
            if (!vis[i] && dfs(i, -1))
            {
                return buildCycle(startNode, endNode, parent);
            }
        }
        return {};
    }
};

class DirectedCycle
{
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> graph;
    vector<bool> vis, inPath;
    vector<int> parent;

    bool dfs(int u)
    {
        vis[u] = true;
        inPath[u] = true;
        for (int v : graph[u])
        {
            if (!vis[v])
            {
                parent[v] = u;
                if (dfs(v))
                    return true;
            }
            else if (inPath[v])
            {
                startNode = v;
                endNode = u;
                return true;
            }
        }
        inPath[u] = false;
        return false;
    }

public:
    DirectedCycle(int n) : n(n), graph(n + 1), vis(n + 1, false), inPath(n + 1, false), parent(n + 1, -1) {}

    void addEdge(int u, int v)
    {
        graph[u].push_back(v);
    }

    vector<int> getCycle()
    {
        for (int i = 1; i <= n; i++)
        {
            if (!vis[i] && dfs(i))
            {
                return buildCycle(startNode, endNode, parent);
            }
        }
        return {};
    }
};

struct Edge {
    int u, v;
    ll w;
};

vector<int> findNegativeCycle(int n, const vector<Edge> &edges)
{
    vector<ll> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);
    int lastRelaxedNode = -1;

    for (int i = 1; i <= n; i++)
    {
        lastRelaxedNode = -1;
        for (const auto &e : edges)
        {
            if (dist[e.u] + e.w < dist[e.v])
            {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                lastRelaxedNode = e.v;
            }
        }
    }
    if (lastRelaxedNode == -1) return {};
    while(n--) lastRelaxedNode = parent[lastRelaxedNode];   
    return buildCycle(lastRelaxedNode, parent[lastRelaxedNode], parent);
}

/*
--------------------------------------------------------------------------------
GRAPH CYCLES TIPS & TRICKS:
--------------------------------------------------------------------------------
- Undirected cycle detection must skip the immediate parent node `v == p` in traversal.
- Directed cycle detection requires keeping track of the current recursion stack path `inPath` or using 3-state visited array.
- Negative Cycle: Run Bellman-Ford $N$ times. If any edge is relaxed in the $N$-th iteration, a negative cycle exists. Walk back $N$ times from the last relaxed node to ensure you are inside the cycle before reconstructing.
--------------------------------------------------------------------------------
*/

// ============================================================================
// THEME 9: SLOPE TRICK (NON-DECREASING ARRAY COST)
// ============================================================================

int IncreasingArray2(vector<int>& nums) {
    long long totalCost = 0;
    priority_queue<int> medianHeap;
    for(auto x:nums){
        medianHeap.push(x);
        if(medianHeap.top() > x){
            totalCost += medianHeap.top() - x;
            medianHeap.pop();
            medianHeap.push(x);
        }
    }

    return totalCost;
}

/*
--------------------------------------------------------------------------------
SLOPE TRICK TIPS & TRICKS:
--------------------------------------------------------------------------------
- Used to compute the minimum cost to make an array non-decreasing (increasing elements or decreasing elements).
- Every new number is pushed into a max-heap as a candidate median.
- If the heap top is greater than the current element, it violates monotonicity. We accumulate the cost difference and swap the heap top.
--------------------------------------------------------------------------------
*/
