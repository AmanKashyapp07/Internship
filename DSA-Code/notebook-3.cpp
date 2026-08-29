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
const int MAXN = 1005;

struct Edge {
    int u, v;
    ll w;
};

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Reconstruct Cycle                           | Predecessor Backtracking Traversal| O(V)     | O(V)     |
 | 2  | Shortest Cycle Length (Girth)               | Multi-Source BFS on All Vertices  | O(V(V+E))| O(V)     |
 | 3  | Nodes in Cycles (Topological Peeling)       | In-Degree 0 Queue Cascade (Kahn)  | O(V + E) | O(V)     |
 | 4  | Bipartite Graph Check (DFS 2-Coloring)      | Alternating DFS 2-Coloring        | O(V + E) | O(V)     |
 | 5  | Shortest Path on Weighted DAG               | TopoSort + Linear Edge Relaxation | O(V + E) | O(V)     |
 | 6  | Range Bitwise AND                           | Binary Prefix Bit-Shifts          | O(log R) | O(1)     |
 | 7  | Longest Nice Subarray (Pairwise AND = 0)    | Sliding Window + Cumulative OR    | O(N)     | O(1)     |
 | 8  | Subarray Bitwise ORs                        | Set DP / Monotonic Frontier Values| O(N * 30)| O(N * 30)|
 | 9  | Count Total Set Bits (1 to N)               | Periodic Bit Position Math        | O(log N) | O(1)     |
 | 10 | Count Subsets with Sum K                    | 1D 0/1 Knapsack DP (Backwards)    | O(N * K) | O(K)     |
 | 11 | Minimum Subset Sum Difference               | Subset Sum DP <= Total / 2        | O(N*Total| O(Total) |
 | 12 | Get Money Sums                              | Reachable Subset Sums Boolean DP  | O(N*Total| O(Total) |
 | 13 | Count of Longest Increasing Subsequences    | 1D DP (Length & Count Arrays)     | O(N^2)   | O(N)     |
 | 14 | Dijkstra's Shortest Path                    | Min-Heap Priority Queue           | O(E logV)| O(V + E) |
 | 15 | Bellman-Ford Algorithm                      | DP Edge Relaxation (V - 1 Passes) | O(V * E) | O(V)     |
 | 16 | Shortest Path on Unweighted Graph (BFS)     | BFS Queue Level-by-Level Scan     | O(V + E) | O(V)     |
 | 17 | Floyd-Warshall All-Pairs Shortest Path      | Intermediate Vertex DP (k-loop)   | O(V^3)   | O(V^2)   |
 | 18 | Negative Cycle Detection (Floyd-Warshall)   | Diagonal Self-Distance Inspection | O(V^3)   | O(V^2)   |
 | 19 | Lexicographical Topological Sort            | Min-Heap Kahn's BFS               | O(VlogV+E| O(V)     |
 | 20 | Shortest Path on DAG via Topo Order         | TopoSort Sequential Relaxation    | O(V + E) | O(V)     |
 | 21 | Graph M-Coloring (Backtracking)             | Backtracking DFS with Safety Check| O(M^V)   | O(V)     |
 | 22 | Cheapest Flights within K Stops             | State-Extended Dijkstra (stops)   | O(E * K) | O(V * K) |
 | 23 | Shortest Path Visiting All Nodes            | Multi-Source Bitmask BFS          | O(V 2^V) | O(V 2^V) |
 | 24 | Shortest Common Supersequence (SCS) Length  | Reduction: \|S1\|+\|S2\| - LCS(S1,S2) | O(\|S1\|\|S2\|)| O(\|S1\|\|S2\|)|
 | 25 | Reconstruct Shortest Common Supersequence   | 2D LCS Table Backtracking         | O(\|S1\|\|S2\|)| O(\|S1\|\|S2\|)|
 | 26 | Minimum Window Subsequence                  | Forward Match + Backward Shrink   | O(\|S\|\|T\|)  | O(1)     |
 | 27 | Minimum Window Substring                    | Sliding Window with Match Counter | O(\|S\|+\|T\|) | O(\|S\|+\|T\|)|
 | 28 | Matrix Chain Multiplication                 | Interval DP over Chain Lengths    | O(N^3)   | O(N^2)   |
 | 29 | Distinct Subsequences                       | 1D Space-Optimized DP (Backwards) | O(\|S\|\|T\|)  | O(\|T\|) |
 | 30 | Maximum Sum BST in Binary Tree              | Post-Order Bottom-Up DFS          | O(N)     | O(H)     |
 | 31 | Remove Leaf Nodes with Target Value         | Post-Order Recursive Tree Pruning | O(N)     | O(H)     |
 | 32 | Minimum Extra Characters in String          | 1D Memoized DP + Hash Set         | O(N^2)   | O(N + D) |
 | 33 | Bounded Knapsack (Binary Power Split)       | Binary Split + 1D 0/1 Knapsack    | O(WlogK) | O(W)     |
 | 34 | LCS Length of 2 Permutations                | Index Map + Patience Sorting LIS  | O(N logN)| O(N)     |
 | 35 | Longest Common Increasing Subseq (LCIS)     | 1D DP with Optimal Prefix Tracker | O(N * M) | O(M)     |
 | 36 | Possible Path Lengths in DAG                | TopoSort + 2D Reachability DP     | O(VN+EN) | O(V * N) |
 ====================================================================================================
*/


// =========================================================
// 1. RECONSTRUCT CYCLE
// =========================================================

vi buildCycle(int start, const vi &parent) {
    vi cycle;
    int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];
        if (cur == start) break;
    }
    return cycle;
}
// Interview Explanation:
// - Problem Statement: Reconstruct a detected cycle sequence from a parent predecessor array.
// - Approach: Predecessor backtracking traversal until cycle start is revisited.
// - Intuition: Tracing parent pointers backward from collision node retraces the cycle loop until reaching start node again.
// - Complexity: Time: O(L) where L is cycle length (at most V), Space: O(L) to store cycle path.


// =========================================================
// 2. SHORTEST CYCLE LENGTH (GIRTH)
// =========================================================

int findShortestCycle(int n, const vvi &g) {
    int min_cycle = 1e9;
    for (int src = 0; src < n; src++) {
        vi dist(n, -1), parent(n, -1);
        queue<int> q;
        dist[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                } else if (parent[u] != v) {
                    min_cycle = min(min_cycle, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    return min_cycle == 1e9 ? -1 : min_cycle;
}
// Interview Explanation:
// - Problem Statement: Find the length of the shortest cycle (girth) in an unweighted undirected graph, or -1 if acyclic.
// - Approach: Breadth-First Search (BFS) initiated from every vertex.
// - Intuition: BFS from node u expands level by level; the first cross-edge connecting to a visited non-parent node v discovers a minimal cycle of length dist[u] + dist[v] + 1.
// - Complexity: Time: O(V * (V + E)) running BFS from all V nodes, Space: O(V) for BFS queue and distance/parent arrays.


// =========================================================
// 3. NODES IN CYCLES (TOPOLOGICAL PEELING)
// =========================================================

vi getNodesInCycles(int n, const vvi &g, vi &indegree) {
    queue<int> q;
    vector<bool> is_acyclic(n, false);
    vi cyclic_nodes;

    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        is_acyclic[u] = true;
        for (int v : g[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }

    for (int i = 0; i < n; i++) {
        if (!is_acyclic[i]) cyclic_nodes.push_back(i);
    }
    return cyclic_nodes;
}
// Interview Explanation:
// - Problem Statement: Identify and return all nodes that are part of at least one cycle in a directed graph.
// - Approach: Topological Peeling via in-degree reduction (Kahn's Algorithm variant).
// - Intuition: Nodes with in-degree 0 cannot belong to any cycle; peeling them recursively eliminates all acyclic branches, leaving only cyclic nodes.
// - Complexity: Time: O(V + E) standard topological sort pass, Space: O(V) for queue and boolean tracking array.


// =========================================================
// 4. BIPARTITE GRAPH CHECK (DFS 2-COLORING)
// =========================================================

bool dfsBipartite(int u, int color, vi &colors, const vvi &g) {
    colors[u] = color;
    for (int v : g[u]) {
        if (colors[v] == -1) {
            if (!dfsBipartite(v, 1 - color, colors, g)) return false;
        } else if (colors[v] == color) {
            return false;
        }
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Check if a graph component is bipartite (2-colorable with no two adjacent vertices sharing the same color).
// - Approach: Recursive DFS 2-Coloring.
// - Intuition: Color start node 0; recursively assign neighbors opposite color 1 - color; if an adjacent neighbor already has the same color, an odd cycle exists.
// - Complexity: Time: O(V + E) visiting all reachable vertices and edges, Space: O(V) for colors array and recursion stack.


// =========================================================
// 5. SHORTEST PATH ON WEIGHTED DAG
// =========================================================

vi shortestPathDAG(int n, const vector<vector<pii>> &g, int src) {
    vi indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (auto &[v, wt] : g[u]) indeg[v]++;
    }

    vi dist(n, 1e9);
    dist[src] = 0;
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &[v, wt] : g[u]) {
            if (dist[u] != 1e9 && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Find single-source shortest paths in a directed acyclic graph (DAG) with edge weights in linear time.
// - Approach: Topological Sort via Kahn's BFS + DAG edge relaxation.
// - Intuition: In a DAG, processing nodes in topological order guarantees that all incoming paths to u are finalized before relaxing edges out of u.
// - Complexity: Time: O(V + E) linear DAG traversal, Space: O(V) for in-degree and distance arrays.


// =========================================================
// 6. RANGE BITWISE AND
// =========================================================

int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left != right) {
        left >>= 1;
        right >>= 1;
        shift++;
    }
    return left << shift;
}
// Interview Explanation:
// - Problem Statement: Compute the bitwise AND of all integers in the inclusive range [left, right].
// - Approach: Bit shift to find the common binary prefix of left and right.
// - Intuition: Lower bits flip between 0 and 1 across any numerical range; only the shared binary prefix survives the continuous bitwise AND.
// - Complexity: Time: O(log(right)) bit shifts (at most 32 operations), Space: O(1) auxiliary space.


// =========================================================
// 7. LONGEST NICE SUBARRAY (PAIRWISE AND = 0)
// =========================================================

int longestNiceSubarray(const vi &nums) {
    int n = nums.size(), l = 0, max_len = 0, mask = 0;
    for (int r = 0; r < n; r++) {
        while ((mask & nums[r]) != 0) {
            mask ^= nums[l++];
        }
        mask |= nums[r];
        max_len = max(max_len, r - l + 1);
    }
    return max_len;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest subarray where the bitwise AND of every pair of elements is 0.
// - Approach: Sliding Window maintaining an active bitwise OR bitmask.
// - Intuition: Pairwise AND = 0 implies each bit position is set by at most one number in the window; shrink from left when current number shares a set bit.
// - Complexity: Time: O(N) since left and right pointers advance at most N times, Space: O(1) auxiliary space.


// =========================================================
// 8. SUBARRAY BITWISE ORs
// =========================================================

int subarrayBitwiseORs(const vi &arr) {
    unordered_set<int> res, cur;
    for (int x : arr) {
        unordered_set<int> nxt = {x};
        for (int y : cur) nxt.insert(x | y);
        for (int z : nxt) res.insert(z);
        cur = nxt;
    }
    return res.size();
}
// Interview Explanation:
// - Problem Statement: Find the number of distinct bitwise OR values produced by all non-empty contiguous subarrays.
// - Approach: Dynamic Programming with Hash Set tracking frontier OR values.
// - Intuition: Bitwise OR is monotonically increasing as elements are accumulated, so cur contains at most 32 distinct values for 32-bit integers.
// - Complexity: Time: O(N * 30) = O(N log(max_val)), Space: O(N * 30) for result set and current frontier.


// =========================================================
// 9. COUNT TOTAL SET BITS (1 TO N)
// =========================================================

int countTotalSetBits(int n) {
    int total_ones = 0;
    for (int i = 0; i < 60; i++) {
        ll cycle_len = 1LL << (i + 1);
        ll cycles = (n + 1) / cycle_len;
        ll ones = 1LL << i;
        ll rem = (n + 1) % cycle_len;

        total_ones += cycles * ones;
        if (rem > ones) total_ones += (rem - ones);
    }
    return total_ones;
}
// Interview Explanation:
// - Problem Statement: Count the total number of set bits (1s) in the binary representations of all numbers from 1 to N.
// - Approach: Bitwise Position Math / Periodic Cycle Counting.
// - Intuition: The i-th bit alternates in periodic blocks of length 2^(i+1) with 2^i ones; count complete cycles plus leftover ones in [1, N].
// - Complexity: Time: O(log N) iterating through bit positions, Space: O(1) auxiliary space.


// =========================================================
// 10. COUNT SUBSETS WITH SUM K
// =========================================================

int countSubsetsWithSumK(const vi &nums, int k) {
    vi dp(k + 1, 0);
    dp[0] = 1;
    for (int num : nums) {
        for (int j = k; j >= num; j--) {
            dp[j] += dp[j - num];
        }
    }
    return dp[k];
}
// Interview Explanation:
// - Problem Statement: Count the number of subsets whose elements sum to exactly K.
// - Approach: 0/1 Knapsack Dynamic Programming with 1D space optimization.
// - Intuition: For each number, iterate backwards from K down to num so that each element is used at most once: dp[j] += dp[j - num].
// - Complexity: Time: O(N * K) 2D loop, Space: O(K) 1D rolling array.


// =========================================================
// 11. MINIMUM SUBSET SUM DIFFERENCE
// =========================================================

int minSubsetSumDifference(const vi &nums) {
    int total_sum = accumulate(nums.begin(), nums.end(), 0);
    vector<bool> dp(total_sum / 2 + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int j = total_sum / 2; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }

    for (int j = total_sum / 2; j >= 0; j--) {
        if (dp[j]) return total_sum - 2 * j;
    }
    return total_sum;
}
// Interview Explanation:
// - Problem Statement: Partition an array into two subsets such that the absolute difference of their sums is minimized.
// - Approach: Subset Sum boolean DP bounded by total_sum / 2.
// - Intuition: If one subset sums to S <= total/2, the other sums to total - S; find the largest reachable S to minimize total - 2S.
// - Complexity: Time: O(N * total_sum) bounded knapsack DP, Space: O(total_sum) boolean array.


// =========================================================
// 12. GET MONEY SUMS
// =========================================================

vi getMoneySums(const vi &coins) {
    int n = coins.size();
    if (n == 0) return {};
    int total = accumulate(coins.begin(), coins.end(), 0);
    vector<bool> dp(total + 1, false);
    dp[0] = true;

    for (int coin : coins) {
        for (int s = total; s >= coin; s--) {
            if (dp[s - coin]) dp[s] = true;
        }
    }

    vi possible;
    for (int s = 1; s <= total; s++) {
        if (dp[s]) possible.push_back(s);
    }
    return possible;
}
// Interview Explanation:
// - Problem Statement: Find all possible distinct non-zero sums that can be formed using a subset of the given coins.
// - Approach: Subset Sum boolean knapsack DP.
// - Intuition: Compute reachable subset sums by setting dp[s] = true if dp[s - coin] is true, iterating backwards from total sum.
// - Complexity: Time: O(N * total_sum), Space: O(total_sum) for boolean DP vector.


// =========================================================
// 13. COUNT OF LONGEST INCREASING SUBSEQUENCES (LIS)
// =========================================================

int countOfLIS(const vi &nums) {
    int n = nums.size();
    if (n == 0) return 0;
    vi len(n, 1), count(n, 1);
    int max_len = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1;
                    count[i] = count[j];
                } else if (len[j] + 1 == len[i]) {
                    count[i] += count[j];
                }
            }
        }
        max_len = max(max_len, len[i]);
    }

    int total = 0;
    for (int i = 0; i < n; i++) {
        if (len[i] == max_len) total += count[i];
    }
    return total;
}
// Interview Explanation:
// - Problem Statement: Find the number of longest increasing subsequences (LIS) in an array.
// - Approach: 1D Dynamic Programming tracking both LIS length and count.
// - Intuition: len[i] stores max LIS length ending at i, count[i] stores ways to achieve it; transition updates or accumulates counts when nums[i] > nums[j].
// - Complexity: Time: O(N^2) double loop over pairs, Space: O(N) for length and count arrays.


// =========================================================
// 14. DIJKSTRA'S SHORTEST PATH
// =========================================================

vi dijkstra(int n, const vector<vector<pii>> &g, int src) {
    vi dist(n, 1e9);
    dist[src] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto &[v, wt] : g[u]) {
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Find single-source shortest paths in a graph with non-negative edge weights.
// - Approach: Greedy shortest path using Min-Heap Priority Queue (std::greater).
// - Intuition: Extract node with minimum tentative distance; relax outgoing neighbors and push updated distances, discarding outdated stale heap states.
// - Complexity: Time: O((V + E) \log V) heap operations, Space: O(V + E) for adjacency list, priority queue, and distance vector.


// =========================================================
// 15. BELLMAN-FORD ALGORITHM
// =========================================================

vl bellmanFord(int n, const vector<Edge> &edges, int src) {
    vl dist(n + 1, 1e18);
    dist[src] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (const auto &e : edges) {
            if (dist[e.u] != 1e18 && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
            }
        }
    }

    for (const auto &e : edges) {
        if (dist[e.u] != 1e18 && dist[e.u] + e.w < dist[e.v]) {
            throw runtime_error("Negative cycle detected");
        }
    }
    return vl(dist.begin() + 1, dist.end());
}
// Interview Explanation:
// - Problem Statement: Find single-source shortest paths in graphs with negative edge weights and detect negative-weight cycles.
// - Approach: Dynamic Programming edge relaxation (N - 1 rounds).
// - Intuition: A shortest path has at most N - 1 edges; relaxing all edges N - 1 times discovers optimal paths, while an N-th relaxation identifies negative cycles.
// - Complexity: Time: O(V * E) edge relaxation rounds, Space: O(V) for distance array.


// =========================================================
// 16. SHORTEST PATH ON UNWEIGHTED GRAPH (BFS)
// =========================================================

vi shortestPathUnweighted(int n, const vvi &g, int src) {
    vi dist(n, 1e9);
    dist[src] = 0;
    queue<int> q;
    q.push(src);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == 1e9) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Find shortest path distances from a source node in an unweighted graph.
// - Approach: Breadth-First Search (BFS) layer-by-layer exploration.
// - Intuition: Since edge weights are uniformly 1, BFS guarantees that the first time a node is reached, it is reached with minimum edges.
// - Complexity: Time: O(V + E) linear queue traversal, Space: O(V) for distance array and queue.


// =========================================================
// 17. FLOYD-WARSHALL ALL-PAIRS SHORTEST PATH
// =========================================================

vvi floydWarshall(int n, const vvi &g) {
    vvi dist = g;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != 1e9 && dist[k][j] != 1e9) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Compute all-pairs shortest paths in a directed weighted graph with no negative cycles.
// - Approach: Dynamic Programming considering all intermediate vertices k in [0, n-1].
// - Intuition: For each pair (i, j), optimal path either avoids vertex k or passes through k: dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]).
// - Complexity: Time: O(V^3) triple nested loops, Space: O(V^2) for distance matrix.


// =========================================================
// 18. NEGATIVE CYCLE DETECTION VIA FLOYD-WARSHALL
// =========================================================

bool cycleDetectionFloyd(int n, const vvi &g) {
    vvi dist = floydWarshall(n, g);
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) return true;
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Detect whether a directed graph contains a negative cycle using the Floyd-Warshall algorithm.
// - Approach: Self-distance inspection after Floyd-Warshall DP.
// - Intuition: A negative cycle exists if and only if any vertex i has negative distance to itself (dist[i][i] < 0), indicating an infinite cost reduction loop.
// - Complexity: Time: O(V^3) Floyd-Warshall run, Space: O(V^2) for distance matrix.


// =========================================================
// 19. LEXICOGRAPHICAL TOPOLOGICAL SORT
// =========================================================

vi lexicographicalTopoSort(int n, const vvi &g, vi indegree) {
    priority_queue<int, vi, greater<int>> pq;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) pq.push(i);
    }

    vi order;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--indegree[v] == 0) pq.push(v);
        }
    }

    if ((int)order.size() != n) return {};
    return order;
}
// Interview Explanation:
// - Problem Statement: Find the lexicographically smallest topological ordering of a DAG using Kahn's algorithm.
// - Approach: Priority Queue (Min-Heap variant) Kahn's BFS.
// - Intuition: Using a min-heap priority queue instead of a FIFO queue ensures that whenever multiple vertices have in-degree 0, the smallest available vertex is selected next.
// - Complexity: Time: O(V \log V + E) priority queue operations, Space: O(V) for priority queue and in-degree vector.


// =========================================================
// 20. SHORTEST PATH ON DAG VIA TOPOLOGICAL ORDER
// =========================================================

vi dpOnDAG(int n, const vector<vector<pii>> &g, int src) {
    vi indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (auto &[v, wt] : g[u]) indeg[v]++;
    }

    queue<int> q;
    vi topo_order;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo_order.push_back(u);
        for (auto &[v, wt] : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }

    vi dist(n, 1e9);
    dist[src] = 0;
    for (int u : topo_order) {
        if (dist[u] != 1e9) {
            for (auto &[v, wt] : g[u]) {
                dist[v] = min(dist[v], dist[u] + wt);
            }
        }
    }
    return dist;
}
// Interview Explanation:
// - Problem Statement: Compute single-source shortest paths on a weighted DAG using topological ordering DP.
// - Approach: Topological Sort followed by sequential vertex relaxation.
// - Intuition: Visiting vertices in topological order guarantees that all predecessor paths to a node are evaluated before relaxing its outgoing edges.
// - Complexity: Time: O(V + E) linear DAG traversal, Space: O(V) for topological order and distance vectors.


// =========================================================
// 21. GRAPH M-COLORING (BACKTRACKING)
// =========================================================

struct GraphMColouring {
    bool isValid(int node, int color, const vvi &g, const vi &colors) {
        for (int v : g[node]) {
            if (colors[v] == color) return false;
        }
        return true;
    }

    void dfs(int node, int m, const vvi &g, vi &colors, int &count) {
        if (node == (int)g.size()) {
            count++;
            return;
        }
        for (int color = 1; color <= m; color++) {
            if (isValid(node, color, g, colors)) {
                colors[node] = color;
                dfs(node + 1, m, g, colors, count);
                colors[node] = 0;
            }
        }
    }

    int countColorings(int n, int m, const vvi &g) {
        vi colors(n, 0);
        int count = 0;
        dfs(0, m, g, colors, count);
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Determine whether an undirected graph can be colored with at most M colors such that no two adjacent vertices share the same color.
// - Approach: Backtracking DFS trying all colors 1 to M per vertex.
// - Intuition: Assign color to current vertex if valid with respect to colored neighbors; recursively color next vertex, backtracking (resetting color to 0) upon conflict.
// - Complexity: Time: O(M^V) worst-case search tree, Space: O(V) for recursion stack and colors vector.


// =========================================================
// 22. CHEAPEST FLIGHTS WITHIN K STOPS
// =========================================================

int findCheapestPrice(int n, const vector<vector<int>> &flights, int src, int dst, int k) {
    vector<vector<pii>> g(n);
    for (auto &f : flights) g[f[0]].push_back({f[1], f[2]});

    int max_flights = k + 1;
    vvi dist(n, vi(max_flights + 1, 1e9));
    using T = tuple<int, int, int>; // {cost, node, flights_taken}
    priority_queue<T, vector<T>, greater<T>> pq;

    dist[src][0] = 0;
    pq.push({0, src, 0});

    while (!pq.empty()) {
        auto [cost, u, taken] = pq.top(); pq.pop();
        if (cost > dist[u][taken]) continue;
        if (u == dst) return cost;

        for (auto &[v, price] : g[u]) {
            if (taken + 1 <= max_flights && cost + price < dist[v][taken + 1]) {
                dist[v][taken + 1] = cost + price;
                pq.push({cost + price, v, taken + 1});
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the cheapest flight price from source to destination with at most K stops.
// - Approach: State-extended Dijkstra / BFS on state (cost, node, stops).
// - Intuition: Model vertices as (node, stops_taken); relax outgoing flights only if stops + 1 <= K + 1 and new cost improves the recorded state distance.
// - Complexity: Time: O(E * K) state transitions, Space: O(V * K) for distance table and priority queue.


// =========================================================
// 23. SHORTEST PATH VISITING ALL NODES (BITMASK BFS)
// =========================================================

int shortestPathVisitingAllNodes(int n, const vvi &g) {
    int target_mask = (1 << n) - 1;
    queue<pair<int, int>> q;
    vvi dist(n, vi(1 << n, 1e9));

    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i});
        dist[i][1 << i] = 0;
    }

    while (!q.empty()) {
        auto [u, mask] = q.front(); q.pop();
        int d = dist[u][mask];
        if (mask == target_mask) return d;

        for (int v : g[u]) {
            int nxt_mask = mask | (1 << v);
            if (dist[v][nxt_mask] > d + 1) {
                dist[v][nxt_mask] = d + 1;
                q.push({v, nxt_mask});
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the shortest path length that visits every vertex in an unweighted undirected graph (can revisit nodes and edges).
// - Approach: Multi-source BFS on State Space (node, bitmask).
// - Intuition: State is (current_node, visited_mask); initialize queue with all nodes at distance 0 with mask 1 << i; shortest path to mask == (1 << n) - 1 is minimal.
// - Complexity: Time: O(V * 2^V) states visited by BFS, Space: O(V * 2^V) for visited distance matrix.


// =========================================================
// 24. SHORTEST COMMON SUPERSEQUENCE (SCS) LENGTH
// =========================================================

int shortestCommonSubsequenceLength(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();
    vvi dp(n + 1, vi(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return n + m - dp[n][m];
}
// Interview Explanation:
// - Problem Statement: Find the length of the shortest common supersequence (SCS) of two strings.
// - Approach: Reduction to Longest Common Subsequence (LCS) via formula |S1| + |S2| - LCS(S1, S2).
// - Intuition: The supersequence must contain characters of both strings; characters common to both (LCS) only need to appear once, giving length N + M - LCS.
// - Complexity: Time: O(|S1| * |S2|) 2D DP, Space: O(|S1| * |S2|) table space.


// =========================================================
// 25. RECONSTRUCT SHORTEST COMMON SUPERSEQUENCE (SCS)
// =========================================================

string SCS(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();
    vvi dp(n + 1, vi(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    string scs;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            scs += s1[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            scs += s1[i - 1];
            i--;
        } else {
            scs += s2[j - 1];
            j--;
        }
    }
    while (i > 0) { scs += s1[i - 1]; i--; }
    while (j > 0) { scs += s2[j - 1]; j--; }

    reverse(scs.begin(), scs.end());
    return scs;
}
// Interview Explanation:
// - Problem Statement: Reconstruct and return the shortest common supersequence string of two strings s1 and s2.
// - Approach: 2D LCS Table Backtracking.
// - Intuition: Fill LCS table; backtrack from (N, M); if characters match, append once and move diagonally; else move toward larger LCS cell appending the non-matching character.
// - Complexity: Time: O(|S1| * |S2|) table fill and backtrack, Space: O(|S1| * |S2|) for DP matrix.


// =========================================================
// 26. MINIMUM WINDOW SUBSEQUENCE
// =========================================================

string minWindowSubsequence(const string &s, const string &t) {
    if (t.empty()) return "";
    int n = s.size(), m = t.size();
    int min_len = 1e9, start_idx = -1;
    int i = 0, j = 0;

    while (i < n) {
        if (s[i] == t[j]) {
            j++;
            if (j == m) {
                int end = i + 1;
                j--;
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                i++; j++;
                if (end - i < min_len) {
                    min_len = end - i;
                    start_idx = i;
                }
            }
        }
        i++;
    }
    return start_idx == -1 ? "" : s.substr(start_idx, min_len);
}
// Interview Explanation:
// - Problem Statement: Find the minimum length substring of s that contains t as a subsequence.
// - Approach: Two-pointer forward match + backward window contraction.
// - Intuition: Scan forward until all characters of t are matched in s; then scan backward from end to start to find the tightest left boundary for that occurrence.
// - Complexity: Time: O(|S| * |T|) forward search and backward contraction, Space: O(1) auxiliary space.


// =========================================================
// 27. MINIMUM WINDOW SUBSTRING
// =========================================================

string minWindowSubstring(const string &s, const string &t) {
    if (t.empty()) return "";
    unordered_map<char, int> need, window;
    for (char c : t) need[c]++;

    int l = 0, r = 0, valid = 0, start = 0, min_len = 1e9;
    while (r < (int)s.size()) {
        char c = s[r++];
        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) valid++;
        }
        while (valid == (int)need.size()) {
            if (r - l < min_len) {
                start = l;
                min_len = r - l;
            }
            char d = s[l++];
            if (need.count(d)) {
                if (window[d] == need[d]) valid--;
                window[d]--;
            }
        }
    }
    return min_len == 1e9 ? "" : s.substr(start, min_len);
}
// Interview Explanation:
// - Problem Statement: Find the minimum window substring of s that contains all characters of t (including duplicates).
// - Approach: Sliding Window with frequency hash maps and valid match counter.
// - Intuition: Expand right pointer until window satisfies character requirements (valid == need.size()); then contract left pointer while preserving validity to minimize window length.
// - Complexity: Time: O(|S| + |T|) each character processed at most twice, Space: O(|S| + |T|) for frequency maps.


// =========================================================
// 28. MATRIX CHAIN MULTIPLICATION
// =========================================================

int matrixChainOrder(const vi &p) {
    int n = p.size() - 1;
    vvi dp(n, vi(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = 1e9;
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1]);
            }
        }
    }
    return dp[0][n - 1];
}
// Interview Explanation:
// - Problem Statement: Find the minimum scalar multiplications needed to multiply a chain of matrices.
// - Approach: Interval / Range Dynamic Programming over chain lengths 2 to N.
// - Intuition: For chain i to j, try all split points k in [i, j-1]: dp[i][j] = min(dp[i][k] + dp[k+1][j] + p[i]*p[k+1]*p[j+1]).
// - Complexity: Time: O(N^3) interval DP triple loop, Space: O(N^2) for DP table.


// =========================================================
// 29. DISTINCT SUBSEQUENCES
// =========================================================

int numDistinct(const string &s, const string &t) {
    int n = s.size(), m = t.size();
    vector<unsigned long long> dp(m + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        for (int j = m; j >= 1; j--) {
            if (s[i - 1] == t[j - 1]) dp[j] += dp[j - 1];
        }
    }
    return dp[m];
}
// Interview Explanation:
// - Problem Statement: Count the number of distinct subsequences of s that equal t.
// - Approach: 1D Space-Optimized Dynamic Programming.
// - Intuition: dp[j] represents ways to form prefix t[0...j-1]; when s[i-1] == t[j-1], dp[j] += dp[j-1] (choice to match or skip current character).
// - Complexity: Time: O(|S| * |T|) 2D loop, Space: O(|T|) rolling array with unsigned long long.


// =========================================================
// 30. MAXIMUM SUM BST IN BINARY TREE
// =========================================================

struct Element {
    int key, height;
    Element *left, *right;
    Element(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

struct MaxSumBST {
    int ans = 0;

    tuple<bool, int, int, int> dfs(TreeNode *node) {
        if (!node) return {true, INT_MAX, INT_MIN, 0};

        auto [l_bst, l_min, l_max, l_sum] = dfs(node->left);
        auto [r_bst, r_min, r_max, r_sum] = dfs(node->right);

        if (l_bst && r_bst && node->val > l_max && node->val < r_min) {
            int sum = l_sum + r_sum + node->val;
            ans = max(ans, sum);
            return {true, min(node->val, l_min), max(node->val, r_max), sum};
        }
        return {false, 0, 0, 0};
    }

    int maxSumBST(TreeNode *root) {
        ans = 0;
        dfs(root);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find the maximum sum of node keys among all valid Binary Search Tree (BST) subtrees.
// - Approach: Post-order Bottom-Up DFS returning {is_bst, min_val, max_val, sum}.
// - Intuition: Current node forms a valid BST if both subtrees are valid BSTs and node->val > left_max and node->val < right_min; update global answer with valid subtree sum.
// - Complexity: Time: O(N) single traversal visiting each node once, Space: O(H) recursion stack space.


// =========================================================
// 31. REMOVE LEAF NODES WITH TARGET VALUE
// =========================================================

TreeNode* removeLeafNodes(TreeNode *root, int target) {
    if (!root) return nullptr;
    root->left = removeLeafNodes(root->left, target);
    root->right = removeLeafNodes(root->right, target);
    if (!root->left && !root->right && root->val == target) return nullptr;
    return root;
}
// Interview Explanation:
// - Problem Statement: Delete all leaf nodes with a given target value repeatedly until no such leaves remain.
// - Approach: Post-order Recursive DFS.
// - Intuition: Process left and right children first so that parent nodes whose children were deleted become new leaves before being evaluated.
// - Complexity: Time: O(N) bottom-up pass, Space: O(H) recursion stack space.


// =========================================================
// 32. MINIMUM EXTRA CHARACTERS IN STRING
// =========================================================

struct MinExtraChar {
    int n;
    vi dp;
    unordered_set<string> st;

    int solve(int i, const string &s) {
        if (i == n) return 0;
        if (dp[i] != -1) return dp[i];

        int ans = 1 + solve(i + 1, s);
        string cur = "";
        for (int j = i; j < n; j++) {
            cur += s[j];
            if (st.count(cur)) {
                ans = min(ans, solve(j + 1, s));
            }
        }
        return dp[i] = ans;
    }

    int minExtraChar(const string &s, const vector<string> &dictionary) {
        n = s.size();
        st.clear();
        for (const string &word : dictionary) st.insert(word);
        dp.assign(n, -1);
        return solve(0, s);
    }
};
// Interview Explanation:
// - Problem Statement: Find the minimum number of extra characters left over after breaking a string into dictionary words.
// - Approach: 1D Dynamic Programming with Memoization + Hash Set lookup.
// - Intuition: At index i, either treat s[i] as an extra character (1 + solve(i + 1)), or match any valid dictionary prefix s[i...j] and transition to solve(j + 1).
// - Complexity: Time: O(N^2) subproblem evaluation, Space: O(N + D) for memoization and dictionary set.


// =========================================================
// 33. BOUNDED KNAPSACK (BINARY SPLIT) & 0/1 KNAPSACK
// =========================================================

void addItemBinarySplit(int w, int v, int k, vi &weights, vi &values) {
    for (int take = 1; k > 0; take <<= 1) {
        int cnt = min(take, k);
        weights.push_back(cnt * w);
        values.push_back(cnt * v);
        k -= cnt;
    }
}

int knapsack01(int n, int W, const vi &weights, const vi &values) {
    vi dp(W + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int w = W; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[W];
}
// Interview Explanation:
// - Problem Statement: Solve the Bounded Knapsack problem where item i has weight w, value v, and count k.
// - Approach: Binary Power Splitting (1, 2, 4, ..., rem) + 1D 0/1 Knapsack DP.
// - Intuition: Decompose quantity k into powers of 2 items, reducing the problem from O(N * K * W) to O(N log K * W) standard 0/1 knapsack.
// - Complexity: Time: O(W * sum(log K)) knapsack transitions, Space: O(W) rolling 1D DP vector.


// =========================================================
// 34. LCS LENGTH OF 2 PERMUTATIONS (LIS REDUCTION)
// =========================================================

int LCSLengthOf2Permutations(const vi &a, const vi &b) {
    unordered_map<int, int> pos;
    int n = a.size();
    for (int i = 0; i < n; i++) pos[a[i]] = i;

    vi dp;
    for (int x : b) {
        if (!pos.count(x)) continue;
        auto it = lower_bound(dp.begin(), dp.end(), pos[x]);
        if (it == dp.end()) dp.push_back(pos[x]);
        else *it = pos[x];
    }
    return dp.size();
}
// Interview Explanation:
// - Problem Statement: Find the length of the Longest Common Subsequence of two permutations of numbers 1 to N.
// - Approach: Permutation Index Mapping + Longest Increasing Subsequence (LIS) via Patience Sorting.
// - Intuition: Map each element of a to its position; transform b by replacing elements with their index in a; the LCS of a and b is isomorphic to the LIS of transformed array.
// - Complexity: Time: O(N \log N) binary search with lower_bound, Space: O(N) for position map and LIS vector.


// =========================================================
// 35. LONGEST COMMON INCREASING SUBSEQUENCE (LCIS)
// =========================================================

vi LCIS(const vi &a, const vi &b) {
    int n = a.size(), m = b.size();
    vi dp(m, 0), parent(m, -1);

    for (int i = 0; i < n; i++) {
        int best_len = 0, best_prev = -1;
        for (int j = 0; j < m; j++) {
            if (a[i] == b[j]) {
                if (best_len + 1 > dp[j]) {
                    dp[j] = best_len + 1;
                    parent[j] = best_prev;
                }
            } else if (a[i] > b[j]) {
                if (dp[j] > best_len) {
                    best_len = dp[j];
                    best_prev = j;
                }
            }
        }
    }

    int end = max_element(dp.begin(), dp.end()) - dp.begin();
    if (dp[end] == 0) return {};

    vi ans;
    while (end != -1) {
        ans.push_back(b[end]);
        end = parent[end];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find and reconstruct the Longest Common Increasing Subsequence (LCIS) of two arrays.
// - Approach: 1D Dynamic Programming with running optimal prefix tracking.
// - Intuition: For each a[i], track best_len among elements b[j] < a[i]; when a[i] == b[j], extend dp[j] = best_len + 1 and record parent for backtracking.
// - Complexity: Time: O(N * M) nested loops, Space: O(M) for DP and parent tracking vectors.


// =========================================================
// 36. POSSIBLE PATH LENGTHS IN DAG
// =========================================================

vi topoSortHelper(int n, const vvi &g) {
    vi indeg(n + 1, 0);
    for (int u = 1; u <= n; u++) {
        for (int v : g[u]) indeg[v]++;
    }
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (indeg[i] == 0) q.push(i);
    }
    vi order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return order;
}

vi possibleLengths(int n, const vvi &g) {
    vi topo = topoSortHelper(n, g);
    static bool dp[MAXN][MAXN];
    memset(dp, 0, sizeof(dp));
    dp[1][0] = true;

    for (int u : topo) {
        for (int v : g[u]) {
            for (int len = 0; len < n; len++) {
                if (dp[u][len]) dp[v][len + 1] = true;
            }
        }
    }

    vi ans;
    for (int len = 0; len < n; len++) {
        if (dp[n][len]) ans.push_back(len);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find all possible path lengths from node 1 to node N in a directed acyclic graph.
// - Approach: Topological Sort + 2D Reachability DP dp[node][len].
// - Intuition: Transition dp[v][len + 1] = true if dp[u][len] is true for edge (u, v); processing in topological order guarantees optimal DP propagation.
// - Complexity: Time: O(V * N + E * N) bitset/boolean transitions, Space: O(V * N) for DP state matrix.
