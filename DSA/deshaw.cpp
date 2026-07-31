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



struct BinaryLifting {
    int LOG;
    vector<vector<int>> up;
    vector<int> depth;

    BinaryLifting(int n, int root, vector<vector<int>> &g)
        : LOG(32 - __builtin_clz(n)),up(n + 1, vector<int>(LOG, -1)),depth(n + 1) {

        dfs(root, -1, 0, g);

        for (int j = 1; j < LOG; j++)
            for (int i = 1; i <= n; i++)
                if (up[i][j - 1] != -1)
                    up[i][j] = up[up[i][j - 1]][j - 1];
    }

    void dfs(int u, int p, int d, vector<vector<int>> &g) {
        depth[u] = d;
        up[u][0] = p;
        for (int v : g[u]) if (v != p) dfs(v, u, d + 1, g);
    }

    int lift(int u, int k) {
        for (int j = 0; j < LOG && u != -1; j++)
            if (k & (1 << j))
                u = up[u][j];
        return u;
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);

        a = lift(a, depth[a] - depth[b]);
        if (a == b) return a;

        for (int j = LOG - 1; j >= 0; j--)
            if (up[a][j] != up[b][j])
                a = up[a][j], b = up[b][j];

        return up[a][0];
    }
};
/*
==================== BINARY LIFTING ====================

up[u][j]   = 2^j-th ancestor of u
info[u][j] = aggregate information from u to up[u][j]

-------------------- BUILD --------------------

up[u][j] = up[ up[u][j-1] ][j-1]

int parent = up[u][j-1];
info[u][j] = combine(info[u][j-1], info[parent][j-1]);

-------------------- QUERY --------------------

Whenever lifting a node upward:

ans = combine(ans, info[u][j]);
u = up[u][j];

-------------------- CUSTOMIZE --------------------

Only these two need to change:

ID = Identity element
combine(a, b)

-------------------- EXAMPLES --------------------

Max : ID = 0,         combine = max
Min : ID = INF,       combine = min
Sum : ID = 0,         combine = +
XOR : ID = 0,         combine = ^
GCD : ID = 0,         combine = gcd
AND : ID = ALL_BITS,  combine = &
OR  : ID = 0,         combine = |

Time:
Build : O(N log N)
Query : O(log N)

================================================
*/


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

vector<int> buildCycle(int s, int e, const vector<int>& par) {
    vector<int> cyc;
    for (int u = e; u != s; u = par[u]) cyc.push_back(u);
    cyc.push_back(s);
    reverse(cyc.begin(), cyc.end());
    return cyc;
}

class UndirectedCycle {
    int n, s = -1, e = -1;
    vector<vector<int>> g;
    vector<int> vis, par;

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v, u)) return true;
            } else {
                s = v, e = u;
                return true;
            }
        }
        return false;
    }

public:
    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v), g[v].push_back(u); }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++)
            if (!vis[i] && dfs(i, -1))
                return buildCycle(s, e, par);
        return {};
    }
};

class DirectedCycle {
    int n, s = -1, e = -1;
    vector<vector<int>> g;
    vector<int> vis, inPath, par;

    bool dfs(int u) {
        vis[u] = inPath[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v)) return true;
            } else if (inPath[v]) {
                s = v, e = u;
                return true;
            }
        }
        inPath[u] = 0;
        return false;
    }

public:
    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), inPath(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++)
            if (!vis[i] && dfs(i))
                return buildCycle(s, e, par);
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