#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Min Moves to Gather K Consecutive Ones      | Shifted Index + Median Prefix Sum | O(N)     | O(N)     |
 | 2  | 2D Prefix Sum Matrix                        | 2D Inclusion-Exclusion Prefix DP  | O(M * N) | O(M * N) |
 | 3  | Count Subarrays with Bitwise AND Equal to K | Hash Map DP on Frontier Values    | O(N * 30)| O(N)     |
 | 4  | Multiply Two 2D Matrices                    | 3-Nested Loop Dot Products        | O(M*N*P) | O(M * P) |
 | 5  | Max Components Tree Split with Equal Sum    | Divisors + Subtree DFS Cuts       | O(N*div) | O(N)     |
 | 6  | N x N MEX Grid Construction                 | Bitwise XOR Matrix (i ^ j)        | O(N^2)   | O(N^2)   |
 | 7  | Binary Lifting (LCA, K-th Ancestor)         | Ancestor Doubling Table up[u][j]  | O(N logN)| O(N logN)|
 | 8  | Cycle Detection & Reconstruction            | DFS Parent Trace / Bellman-Ford   | O(V+E)/VE| O(V)     |
 | 9  | Make Array Non-Decreasing (Slope Trick)     | Greedy Max-Heap Slope Inflection  | O(N logN)| O(N)     |
 ====================================================================================================
*/

// ============================================================
// 1. MINIMUM MOVES TO GATHER K CONSECUTIVE ONES
// ============================================================

ll minMoves(const vi &nums, int k) {
    vl pos;
    for (int i = 0; i < (int)nums.size(); i++) if (nums[i]) pos.push_back(i);
    int m = pos.size();
    vl shifted(m), pref(m + 1, 0);
    for (int i = 0; i < m; i++) {
        shifted[i] = pos[i] - i;
        pref[i + 1] = pref[i] + shifted[i];
    }
    ll ans = 1e18;
    for (int l = 0; l + k <= m; l++) {
        int r = l + k - 1, mid = (l + r) / 2;
        ll median = shifted[mid];
        ans = min(ans, median * (mid - l) - (pref[mid] - pref[l]) + (pref[r + 1] - pref[mid + 1]) - median * (r - mid));
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the minimum moves to group any K consecutive 1s together in a binary array (LC 1703).
// - Approach: Shifted indices transformation + Prefix sum median cost evaluation.
// - Intuition: For 1s at positions p_0, p_1, ..., shifting by p_i - i transforms grouping adjacent elements into gathering coordinates to their median, solvable via prefix sums.
// - Complexity: Time: O(N) single pass and sliding window, Space: O(N) for shifted coordinates and prefix array.


// ============================================================
// 2. 2D PREFIX SUM MATRIX
// ============================================================

vvi prefixSum2D(const vvi &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vvi pref(m, vi(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            pref[i][j] = matrix[i][j] + (i > 0 ? pref[i - 1][j] : 0) + (j > 0 ? pref[i][j - 1] : 0) - (i > 0 && j > 0 ? pref[i - 1][j - 1] : 0);
        }
    }
    return pref;
}
// Interview Explanation:
// - Problem Statement: Construct a 2D prefix sum table supporting O(1) submatrix sum queries.
// - Approach: Inclusion-Exclusion 2D Dynamic Programming.
// - Intuition: Each cell sum is matrix[i][j] + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1], adding top and left sums and removing double-counted diagonal.
// - Complexity: Time: O(M * N) filling 2D grid, Space: O(M * N) for prefix sum matrix.


// ============================================================
// 3. COUNT SUBARRAYS WITH BITWISE AND EQUAL TO K
// ============================================================

ll countSubarraysWithAND(const vi &nums, int k) {
    ll ans = 0;
    unordered_map<int, ll> prev, cur;
    for (int x : nums) {
        cur.clear(); cur[x]++;
        for (auto [v, cnt] : prev) cur[v & x] += cnt;
        ans += cur[k];
        prev = std::move(cur);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count the number of subarrays whose bitwise AND equals K.
// - Approach: Hash Map DP aggregating unique prefix AND results.
// - Intuition: As elements are added, cumulative bitwise AND can change value at most 30 times; maintain counts of distinct AND values ending at previous index and transition.
// - Complexity: Time: O(N * 30) = O(N log(max_val)), Space: O(N) for hash maps.


// ============================================================
// 4. MULTIPLY TWO 2D MATRICES
// ============================================================

vvi multiplyMatrices2D(const vvi &A, const vvi &B) {
    int m = A.size(), n = A[0].size(), p = B[0].size();
    vvi C(m, vi(p, 0));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            for (int k = 0; k < n; k++) C[i][j] += A[i][k] * B[k][j];
    return C;
}
// Interview Explanation:
// - Problem Statement: Multiply two 2D matrices A of dimensions M x N and B of dimensions N x P.
// - Approach: Standard 3-nested loop matrix multiplication.
// - Intuition: Each entry C[i][j] = sum(A[i][k] * B[k][j]) computes the dot product of row i of A and column j of B.
// - Complexity: Time: O(M * N * P) triple loop, Space: O(M * P) for result matrix.


// ============================================================
// 5. MAXIMUM COMPONENTS TREE SPLIT WITH EQUAL SUM
// ============================================================

static bool ok_comp;
ll dfsMaxComp(int u, int p, const vvi &g, const vi &val, ll target) {
    ll sum = val[u];
    for (int v : g[u]) if (v != p) sum += dfsMaxComp(v, u, g, val, target);
    if (sum == target) return 0;
    if (sum > target) ok_comp = false;
    return sum;
}

int maxComponents(const vi &val, const vvi &g) {
    ll total = accumulate(val.begin(), val.end(), 0LL);
    vl divisors;
    for (ll d = 1; d * d <= total; d++) {
        if (total % d == 0) {
            divisors.push_back(d);
            if (d * d != total) divisors.push_back(total / d);
        }
    }
    sort(divisors.begin(), divisors.end());
    for (ll target : divisors) {
        ok_comp = true;
        if (dfsMaxComp(0, -1, g, val, target) == 0 && ok_comp) return total / target - 1;
    }
    return 0;
}
// Interview Explanation:
// - Problem Statement: Find the maximum number of connected components a tree can be partitioned into such that each component has equal node sum (LC 2440).
// - Approach: Factorization of total tree sum + Subtree sum DFS verification.
// - Intuition: Number of components must divide total sum; test divisors in ascending order; DFS returns 0 when subtree reaches target sum, effectively cutting the component.
// - Complexity: Time: O(N * divisors(total_sum)), Space: O(N) for recursion stack.


// ============================================================
// 6. N x N MEX GRID CONSTRUCTION
// ============================================================

vvi mexGridConstruction(int n) {
    vvi grid(n, vi(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) grid[i][j] = i ^ j;
    return grid;
}
// Interview Explanation:
// - Problem Statement: Construct an N x N grid where cell (i, j) contains i ^ j (Nim-sum matrix).
// - Approach: Bitwise XOR Matrix Construction.
// - Intuition: XOR addition ensures every row and column contains a permutation of values, satisfying MEX (minimum excluded) grid properties.
// - Complexity: Time: O(N^2) filling grid, Space: O(N^2) for grid matrix.


// ============================================================
// 7. BINARY LIFTING (LCA, K-TH ANCESTOR, PATH QUERIES)
// ============================================================

struct BinaryLifting {
    static const int LOG = 21;
    int n, root;
    vector<array<int, LOG>> up;
    vector<int> depth;

    BinaryLifting(int n, int root, const vector<vector<int>>& g) : n(n), root(root), up(n + 1), depth(n + 1, 0) {
        vector<int> q = {root}; up[root][0] = 0;
        int head = 0;
        while (head < (int)q.size()) {
            int u = q[head++];
            for (int v : g[u]) {
                if (v == up[u][0]) continue;
                up[v][0] = u; depth[v] = depth[u] + 1; q.push_back(v);
            }
        }
        for (int j = 1; j < LOG; j++)
            for (int u = 1; u <= n; u++) up[u][j] = up[up[u][j - 1]][j - 1];
    }

    int kthAncestor(int u, int k) {
        for (int j = 0; j < LOG && u != 0; j++) if (k & (1 << j)) u = up[u][j];
        return u == 0 ? -1 : u;
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        a = kthAncestor(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int j = LOG - 1; j >= 0; j--) if (up[a][j] != up[b][j]) { a = up[a][j]; b = up[b][j]; }
        return up[a][0];
    }

    int dist(int a, int b) { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
};
// Interview Explanation:
// - Problem Statement: Perform efficient tree queries including LCA, distance, K-th ancestor, and path aggregates.
// - Approach: Binary Lifting table up[node][k] doubling ancestor jumps (2^k).
// - Intuition: Precompute 2^k-th ancestors via up[u][j] = up[up[u][j-1]][j-1]; any ancestor jump can be decomposed into binary powers of 2 in O(log N) time.
// - Complexity: Time: O(N \log N) preprocessing, O(\log N) per query, Space: O(N \log N) table storage.


// ============================================================
// 8. CYCLE DETECTION & RECONSTRUCTION (UNDIRECTED / DIRECTED / NEGATIVE)
// ============================================================

vi buildCycle(int s, int e, const vi &par) {
    vi cyc; cyc.push_back(e);
    while (true) { cyc.push_back(s); if (s == e) break; s = par[s]; }
    reverse(cyc.begin(), cyc.end());
    return cyc;
}

struct UndirectedCycle {
    int n, s = -1, e = -1; vvi g; vi vis, par;
    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) { par[v] = u; if (dfs(v, u)) return true; }
            else { s = v; e = u; return true; }
        }
        return false;
    }
    vi getCycle() {
        for (int i = 1; i <= n; i++) if (!vis[i] && dfs(i, -1)) return buildCycle(s, e, par);
        return {};
    }
};

struct DirectedCycle {
    int n, s = -1, e = -1; vvi g; vi vis, in_path, par;
    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), in_path(n + 1), par(n + 1, -1) {}
    void addEdge(int u, int v) { g[u].push_back(v); }
    bool dfs(int u) {
        vis[u] = in_path[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) { par[v] = u; if (dfs(v)) return true; }
            else if (in_path[v]) { s = v; e = u; return true; }
        }
        in_path[u] = 0;
        return false;
    }
    vi getCycle() {
        for (int i = 1; i <= n; i++) if (!vis[i] && dfs(i)) return buildCycle(s, e, par);
        return {};
    }
};

struct EdgeItem { int u, v; ll w; };

vi findNegativeCycle(int n, const vector<EdgeItem> &edges) {
    vl dist(n + 1, 0); vi parent(n + 1, -1); int last_relaxed = -1;
    for (int i = 1; i <= n; i++) {
        last_relaxed = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                last_relaxed = e.v;
            }
        }
    }
    if (last_relaxed == -1) return {};
    for (int i = 0; i < n; i++) last_relaxed = parent[last_relaxed];
    return buildCycle(last_relaxed, parent[last_relaxed], parent);
}
// Interview Explanation:
// - Problem Statement: Detect and reconstruct cycles in undirected, directed, and edge-weighted graphs.
// - Approach: DFS with parent / path arrays (Undirected/Directed) and Bellman-Ford (Negative cycles).
// - Intuition: Back-edge collision records start and end vertices; backtracking through parent pointers recovers the exact cyclic path sequence.
// - Complexity: Time: O(V + E) for DFS, O(V * E) for Bellman-Ford, Space: O(V) for visited and parent arrays.


// ============================================================
// 9. MAKE ARRAY NON-DECREASING (SLOPE TRICK)
// ============================================================

ll makeArrayNonDecreasing(const vi &nums) {
    ll total_cost = 0;
    priority_queue<int> max_heap;
    for (int x : nums) {
        max_heap.push(x);
        if (max_heap.top() > x) {
            total_cost += max_heap.top() - x;
            max_heap.pop();
            max_heap.push(x);
        }
    }
    return total_cost;
}
// Interview Explanation:
// - Problem Statement: Find minimum operations to make an array non-decreasing where each step can increment or decrement an element by 1.
// - Approach: Greedy Slope Trick using a Max-Heap.
// - Intuition: If current element x < heap.top(), moving top down to x minimizes cost; push x twice (once for slope inflection point, once for value adjustment).
// - Complexity: Time: O(N \log N) heap operations, Space: O(N) for priority queue.
