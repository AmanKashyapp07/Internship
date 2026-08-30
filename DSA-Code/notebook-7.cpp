#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
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
 | 1  | Course Schedule I (LeetCode 207)           | Kahn's Algorithm (Topological BFS)| O(V + E) | O(V + E) |
 | 2  | Course Schedule II (LeetCode 210)          | Kahn's Algorithm (Order Recovery) | O(V + E) | O(V + E) |
 | 3  | Parallel Courses (LeetCode 1136)           | Level-Order TopoSort (Kahn's BFS) | O(V + E) | O(V + E) |
 | 4  | Find Peak Element (LeetCode 162)           | Binary Search on Monotonic Slope  | O(log N) | O(1)     |
 | 5  | Find a Peak Element II (LeetCode 1901)     | Column Binary Search + Column Max | O(R logC)| O(1)     |
 | 6  | Pancake Sorting (LeetCode 969)             | Greedy Suffix Placement (2-Flips) | O(N^2)   | O(N)     |
 | 7  | Jump Game I (LeetCode 55)                  | Greedy Farthest Reachable Index   | O(N)     | O(1)     |
 | 8  | Jump Game II (LeetCode 45)                 | Greedy BFS Horizon Interval       | O(N)     | O(1)     |
 | 9  | Jump Game III (LeetCode 1306)              | Graph BFS Reachability            | O(N)     | O(N)     |
 | 10 | Jump Game IV (LeetCode 1345)              | BFS + Same-Value Pruning          | O(N)     | O(N)     |
 | 11 | Jump Game V (LeetCode 1340)               | Memoized DFS / DAG Longest Path   | O(N * D) | O(N)     |
 | 12 | Jump Game VI (LeetCode 1696)              | Monotonic Deque Sliding Window DP | O(N)     | O(N)     |
 | 13 | Jump Game VII (LeetCode 1871)             | Sliding Window Reachability DP    | O(N)     | O(N)     |
 | 14 | Stock I — 1 Transaction (LeetCode 121)     | Running Minimum Price Tracking    | O(N)     | O(1)     |
 | 15 | Stock II — Infinite Txns (LeetCode 122)    | Greedy Positive Slope Harvest     | O(N)     | O(1)     |
 | 16 | Stock III — At Most 2 Txns (LeetCode 123)  | 4-State Finite State Machine      | O(N)     | O(1)     |
 | 17 | Stock IV — At Most K Txns (LeetCode 188)   | 1D DP K-Transaction Compression   | O(N * K) | O(K)     |
 | 18 | Stock with Cooldown (LeetCode 309)         | 3-State Machine (Held, Sold, Rest)| O(N)     | O(1)     |
 | 19 | Stock with Transaction Fee (LeetCode 714)  | 2-State Machine (Cash, Hold)      | O(N)     | O(1)     |
 | 20 | House Robber I (LeetCode 198)              | 1D DP Space-Optimized (prev1,prev2)| O(N)     | O(1)     |
 | 21 | House Robber II (LeetCode 213)             | Circular Array 2-Pass Range DP    | O(N)     | O(1)     |
 | 22 | House Robber III (LeetCode 337)            | Post-Order Tree DP (Rob / Skip)   | O(N)     | O(H)     |
 | 23 | House Robber IV (LeetCode 2560)           | Binary Search on Min Capability   | O(N logM)| O(1)     |
 | 24 | Delete and Earn / Robber V (LeetCode 740)  | Frequency Array Reduction to DP   | O(N+maxA)| O(maxA)  |
 | 25 | Coin Change I — Fewest (LeetCode 322)      | 1D Unbounded Knapsack DP (Min)    | O(N * A) | O(A)     |
 | 26 | Coin Change II — Combinations (LC 518)     | 1D Unbounded Combination DP       | O(N * A) | O(A)     |
 | 27 | Coin Change Permutations (LeetCode 377)    | 1D Unbounded Permutation DP       | O(N * A) | O(A)     |
 | 28 | Bounded Coin Change (Limited Supply)       | Binary Splitting + 0/1 Knapsack   | O(A logK)| O(A)     |
 | 29 | Coin Change Path Reconstruction            | DP Predecessor Pointer Traceback  | O(N * A) | O(A)     |
 | 30 | Reachable Money Sums (CSES Money Sums)     | 0/1 Knapsack Boolean Reachability | O(N*sum) | O(sum)   |
 | 31 | Dutch National Flag (0, 1, 2 Sort)         | 3-Way In-Place Partitioning (DNF) | O(N)     | O(1)     |
 | 32 | Counting Sort (Stable Implementation)      | Prefix Frequency Cumulative Table | O(N + K) | O(N + K) |
 | 33 | Radix Sort (LSD Base-10)                   | Digit-by-Digit Stable Counting DP | O(D*(N+B)| O(N + B) |
 | 34 | Bucket Sort (Uniform Distribution)         | Range Bucketing + Local Sorting   | O(N) avg | O(N)     |
 | 35 | Cyclic Sort [1 to N]                       | In-Place Index Mapping Swap       | O(N)     | O(1)     |
 | 36 | Pigeonhole Sort                            | Direct Hole Placement & Retrieval | O(N+Range| O(N+Range|
 | 37 | Patience Sorting (LIS Extraction)          | Card Pile Lower-Bound Insertion   | O(N logN)| O(N)     |
 | 38 | 3-Way QuickSort (Bentley-McIlroy)          | DNF Partitioning for Duplicates   | O(N logN)| O(log N) |
 ====================================================================================================
*/

// ============================================================
// 1. COURSE SCHEDULE I — LeetCode 207
// ============================================================

bool canFinish(int n, vector<vector<int>>& prereqs) {
    vector<vector<int>> adj(n);
    vector<int> indeg(n, 0);
    for (auto& p : prereqs) { adj[p[1]].push_back(p[0]); indeg[p[0]]++; }
    queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    int completed = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        completed++;
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return completed == n;
}
// Interview Explanation:
// - Problem Statement: Determine if all courses can be finished given prerequisite pairs [course, prereq] (LeetCode 207).
// - Approach: Kahn's Algorithm for Topological Sorting (BFS in-degree reduction).
// - Intuition: Build directed graph prereq -> course. Process vertices with in-degree 0 in a queue, decrementing dependent in-degrees. If processed count equals n, graph is a DAG (no cycle).
// - Complexity: Time: O(V + E), Space: O(V + E).

// ============================================================
// 2. COURSE SCHEDULE II — LeetCode 210
// ============================================================

vector<int> findOrder(int n, vector<vector<int>>& prereqs) {
    vector<vector<int>> adj(n);
    vector<int> indeg(n, 0), order;
    for (auto& p : prereqs) { adj[p[1]].push_back(p[0]); indeg[p[0]]++; }
    queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return order.size() == n ? order : vector<int>{};
}
// Interview Explanation:
// - Problem Statement: Return a valid ordering of courses to take to finish all courses (LeetCode 210).
// - Approach: Kahn's Algorithm for Topological Sorting (BFS).
// - Intuition: Push nodes with in-degree 0 to the topological order and queue. Decrement child in-degrees. Return collected order if size equals n, else empty array (cycle detected).
// - Complexity: Time: O(V + E), Space: O(V + E).

// ============================================================
// 3. PARALLEL COURSES — LeetCode 1136
// ============================================================

int minimumSemesters(int n, vector<vector<int>>& relations) {
    vector<vector<int>> adj(n + 1);
    vector<int> indeg(n + 1, 0);
    for (auto& r : relations) { adj[r[0]].push_back(r[1]); indeg[r[1]]++; }
    queue<int> q;
    for (int i = 1; i <= n; i++) if (indeg[i] == 0) q.push(i);
    int semesters = 0, completed = 0;
    while (!q.empty()) {
        semesters++;
        for (int sz = q.size(); sz > 0; sz--) {
            int u = q.front(); q.pop();
            completed++;
            for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
        }
    }
    return completed == n ? semesters : -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum semesters to complete all courses where all eligible courses run in parallel (LeetCode 1136).
// - Approach: Level-Order BFS Topological Sort (Kahn's Algorithm).
// - Intuition: Each BFS layer represents courses taken concurrently in one semester. Count depth of DAG; return -1 if completed count != n (cycle).
// - Complexity: Time: O(V + E), Space: O(V + E).

// ============================================================
// 4. FIND PEAK ELEMENT — LeetCode 162
// ============================================================

int findPeakElement(vector<int>& nums) {
    int l = 0, r = nums.size() - 1;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] < nums[mid + 1]) l = mid + 1;
        else r = mid;
    }
    return l;
}
// Interview Explanation:
// - Problem Statement: Find any peak element index where nums[i] > nums[i-1] and nums[i] > nums[i+1] (LeetCode 162).
// - Approach: Binary Search on Monotonic Gradient / Slope.
// - Intuition: If nums[mid] < nums[mid + 1], an ascending slope guarantees a peak exists in the right half; otherwise, search left half.
// - Complexity: Time: O(log N), Space: O(1).

// ============================================================
// 5. FIND A PEAK ELEMENT II — LeetCode 1901
// ============================================================

vector<int> findPeakGrid(vector<vector<int>>& mat) {
    int m = mat.size(), n = mat[0].size(), lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2, maxR = 0;
        for (int r = 1; r < m; r++) if (mat[r][mid] > mat[maxR][mid]) maxR = r;
        int lVal = mid > 0 ? mat[maxR][mid - 1] : -1, rVal = mid + 1 < n ? mat[maxR][mid + 1] : -1;
        if (mat[maxR][mid] > lVal && mat[maxR][mid] > rVal) return {maxR, mid};
        if (rVal > mat[maxR][mid]) lo = mid + 1;
        else hi = mid - 1;
    }
    return {-1, -1};
}
// Interview Explanation:
// - Problem Statement: Find a strictly greater peak cell in an m x n 2D grid (LeetCode 1901).
// - Approach: Binary Search on Matrix Columns + Column-Max Element.
// - Intuition: Find global maximum of middle column. Since it is already greater than vertical neighbors, compare with left and right neighbors to navigate toward higher gradient.
// - Complexity: Time: O(M log N), Space: O(1).

// ============================================================
// 6. PANCAKE SORTING — LeetCode 969
// ============================================================

vi pancakeSort(vi &arr) {
    vi ans;
    for (int sz = arr.size(); sz > 1; sz--) {
        int maxIdx = max_element(arr.begin(), arr.begin() + sz) - arr.begin();
        if (maxIdx == sz - 1) continue;
        if (maxIdx > 0) { ans.push_back(maxIdx + 1); reverse(arr.begin(), arr.begin() + maxIdx + 1); }
        ans.push_back(sz); reverse(arr.begin(), arr.begin() + sz);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Sort array using 1-indexed prefix reversals (pancake flips) (LeetCode 969).
// - Approach: Greedy Suffix Placement (At most 2 flips per element).
// - Intuition: For current unsorted size, locate max element. Flip to index 0 (if not already there), then flip to current end position sz.
// - Complexity: Time: O(N^2), Space: O(N) flip sequence.

// ============================================================
// 7. JUMP GAME I — LeetCode 55
// ============================================================

bool canJump(vector<int>& nums) {
    int farthest = 0, n = nums.size();
    for (int i = 0; i < n; i++) {
        if (i > farthest) return false;
        farthest = max(farthest, i + nums[i]);
        if (farthest >= n - 1) return true;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if you can reach the last index starting from index 0 (LeetCode 55).
// - Approach: Greedy Farthest Reachable Position.
// - Intuition: At each step, if current index exceeds farthest reached, return false. Update farthest = max(farthest, i + nums[i]). If farthest >= n - 1, destination is reachable.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 8. JUMP GAME II — LeetCode 45
// ============================================================

int jump(vector<int>& nums) {
    int jumps = 0, curEnd = 0, farthest = 0, n = nums.size();
    for (int i = 0; i < n - 1; i++) {
        farthest = max(farthest, i + nums[i]);
        if (i == curEnd) { jumps++; curEnd = farthest; }
    }
    return jumps;
}
// Interview Explanation:
// - Problem Statement: Return the minimum number of jumps to reach the last index (LeetCode 45).
// - Approach: Greedy BFS / Level-by-Level Interval Expansion.
// - Intuition: Maintain current jump horizon (curEnd) and maximum reach from current range (farthest). When i reaches curEnd, increment jump count and update curEnd = farthest.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 9. JUMP GAME III — LeetCode 1306
// ============================================================

bool canReach(vector<int>& arr, int start) {
    int n = arr.size();
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(start); vis[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (arr[u] == 0) return true;
        for (int v : {u + arr[u], u - arr[u]}) {
            if (v >= 0 && v < n && !vis[v]) { vis[v] = true; q.push(v); }
        }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if you can reach any index with value 0 from start, jumping +/- arr[i] (LeetCode 1306).
// - Approach: Breadth-First Search (BFS) / Graph Reachability.
// - Intuition: Treat array indices as graph vertices with directed edges to i + arr[i] and i - arr[i]. Traverse with BFS and a visited set to detect a 0-valued node.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 10. JUMP GAME IV — LeetCode 1345
// ============================================================

int minJumps(vector<int>& arr) {
    int n = arr.size();
    unordered_map<int, vector<int>> pos;
    for (int i = 0; i < n; i++) pos[arr[i]].push_back(i);
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(0); vis[0] = true;
    int steps = 0;
    while (!q.empty()) {
        for (int sz = q.size(); sz > 0; sz--) {
            int u = q.front(); q.pop();
            if (u == n - 1) return steps;
            if (u - 1 >= 0 && !vis[u - 1]) { vis[u - 1] = true; q.push(u - 1); }
            if (u + 1 < n && !vis[u + 1]) { vis[u + 1] = true; q.push(u + 1); }
            if (pos.count(arr[u])) {
                for (int v : pos[arr[u]]) if (!vis[v]) { vis[v] = true; q.push(v); }
                pos.erase(arr[u]);
            }
        }
        steps++;
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum jumps to reach the last index jumping to i-1, i+1, or any j where arr[j] == arr[i] (LeetCode 1345).
// - Approach: Level-Order BFS with Value Group Pruning.
// - Intuition: Group indices by value in a hash map. During BFS expansion, push adjacent indices and all same-value indices, then clear the map entry to prevent O(N^2) redundant expansions.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 11. JUMP GAME V — LeetCode 1340
// ============================================================

int maxJumps(vector<int>& arr, int d) {
    int n = arr.size(), ans = 0;
    vector<int> dp(n, -1);
    function<int(int)> dfs = [&](int i) {
        if (dp[i] != -1) return dp[i];
        int best = 1;
        for (int j = i + 1; j < n && j <= i + d; j++) {
            if (arr[j] >= arr[i]) break;
            best = max(best, 1 + dfs(j));
        }
        for (int j = i - 1; j >= 0 && j >= i - d; j--) {
            if (arr[j] >= arr[i]) break;
            best = max(best, 1 + dfs(j));
        }
        return dp[i] = best;
    };
    for (int i = 0; i < n; i++) ans = max(ans, dfs(i));
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find maximum indices you can visit starting at any index, jumping <= d steps strictly to smaller values without stepping over >= values (LeetCode 1340).
// - Approach: Memoized DFS / Dynamic Programming on DAG.
// - Intuition: Valid jumps can only go to strictly smaller heights, forming a DAG (no cycles). Memoize dp[i] = max path length from index i across left/right ray searches.
// - Complexity: Time: O(N * D), Space: O(N).

// ============================================================
// 12. JUMP GAME VI — LeetCode 1696
// ============================================================

int maxResult(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> dp(n);
    deque<int> dq = {0};
    dp[0] = nums[0];
    for (int i = 1; i < n; i++) {
        while (!dq.empty() && dq.front() < i - k) dq.pop_front();
        dp[i] = nums[i] + dp[dq.front()];
        while (!dq.empty() && dp[dq.back()] <= dp[i]) dq.pop_back();
        dq.push_back(i);
    }
    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Maximum score reaching the last index starting from 0, jumping at most k steps (LeetCode 1696).
// - Approach: Dynamic Programming with Monotonic Deque Optimization.
// - Intuition: Transition dp[i] = nums[i] + max(dp[j]) for j in [i - k, i - 1]. Maintain sliding window maximum using a monotonic decreasing deque storing indices by dp values in O(1) amortized.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 13. JUMP GAME VII — LeetCode 1871
// ============================================================

bool canReach(string s, int minJump, int maxJump) {
    int n = s.size(), reach = 0;
    if (s[n - 1] != '0') return false;
    vector<bool> dp(n, false);
    dp[0] = true;
    for (int i = 1; i < n; i++) {
        if (i >= minJump && dp[i - minJump]) reach++;
        if (i > maxJump && dp[i - maxJump - 1]) reach--;
        if (s[i] == '0' && reach > 0) dp[i] = true;
    }
    return dp[n - 1];
}
// Interview Explanation:
// - Problem Statement: Check if last index can be reached where s[i] == '0' and jump range is [minJump, maxJump] (LeetCode 1871).
// - Approach: Sliding Window Reachability Count DP.
// - Intuition: dp[i] is true if s[i] == '0' and there is at least one true dp[j] in window [i - maxJump, i - minJump]. Maintain a running count `reach` of true states entering and exiting the valid window.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 14. BEST TIME TO BUY AND SELL STOCK I — LeetCode 121
// ============================================================

int maxProfitI(vector<int>& prices) {
    int minPrice = INT_MAX, maxProfit = 0;
    for (int p : prices) {
        minPrice = min(minPrice, p);
        maxProfit = max(maxProfit, p - minPrice);
    }
    return maxProfit;
}
// Interview Explanation:
// - Problem Statement: Maximize profit with at most 1 transaction (buy once, sell once) (LeetCode 121).
// - Approach: Single-Pass Running Minimum Price.
// - Intuition: On day i, maximum profit if selling today is prices[i] - minPriceSeenSoFar. Update minPrice and record max profit across all days.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 15. BEST TIME TO BUY AND SELL STOCK II — LeetCode 122
// ============================================================

int maxProfitII(vector<int>& prices) {
    int profit = 0;
    for (int i = 1; i < (int)prices.size(); i++)
        if (prices[i] > prices[i - 1]) profit += prices[i] - prices[i - 1];
    return profit;
}
// Interview Explanation:
// - Problem Statement: Maximize profit with unlimited transactions (LeetCode 122).
// - Approach: Greedy Positive Slope Accumulation.
// - Intuition: Any multi-day gain prices[j] - prices[i] equals the sum of consecutive daily gains. Greedily capture all positive daily increments prices[i] - prices[i - 1].
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 16. BEST TIME TO BUY AND SELL STOCK III — LeetCode 123
// ============================================================

int maxProfitIII(vector<int>& prices) {
    int buy1 = -1e9, sell1 = 0, buy2 = -1e9, sell2 = 0;
    for (int p : prices) {
        buy1 = max(buy1, -p);
        sell1 = max(sell1, buy1 + p);
        buy2 = max(buy2, sell1 - p);
        sell2 = max(sell2, buy2 + p);
    }
    return sell2;
}
// Interview Explanation:
// - Problem Statement: Maximize profit with at most 2 transactions (LeetCode 123).
// - Approach: 4-State Finite State Machine (buy1, sell1, buy2, sell2).
// - Intuition: Track optimal capital after 1st buy (-price), 1st sell (buy1 + price), 2nd buy (sell1 - price), and 2nd sell (buy2 + price).
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 17. BEST TIME TO BUY AND SELL STOCK IV — LeetCode 188
// ============================================================

int maxProfitIV(int k, vector<int>& prices) {
    int n = prices.size();
    if (n <= 1 || k <= 0) return 0;
    if (k >= n / 2) {
        int profit = 0;
        for (int i = 1; i < n; i++) if (prices[i] > prices[i - 1]) profit += prices[i] - prices[i - 1];
        return profit;
    }
    vector<int> buy(k + 1, -1e9), sell(k + 1, 0);
    for (int p : prices) {
        for (int j = 1; j <= k; j++) {
            buy[j] = max(buy[j], sell[j - 1] - p);
            sell[j] = max(sell[j], buy[j] + p);
        }
    }
    return sell[k];
}
// Interview Explanation:
// - Problem Statement: Maximize profit with at most K transactions (LeetCode 188).
// - Approach: DP State Compression / K-State Transitions.
// - Intuition: If k >= n / 2, equivalent to unlimited transactions (Stock II). Otherwise, maintain buy[j] = max balance after j buys, sell[j] = max profit after j sells.
// - Complexity: Time: O(N * K), Space: O(K).

// ============================================================
// 18. BEST TIME TO BUY AND SELL STOCK WITH COOLDOWN — LeetCode 309
// ============================================================

int maxProfitCooldown(vector<int>& prices) {
    int held = -1e9, sold = 0, rest = 0;
    for (int p : prices) {
        int prevSold = sold;
        sold = held + p;
        held = max(held, rest - p);
        rest = max(rest, prevSold);
    }
    return max(sold, rest);
}
// Interview Explanation:
// - Problem Statement: Maximize profit with unlimited transactions and a 1-day cooldown after selling (LeetCode 309).
// - Approach: 3-State Dynamic Programming (held, sold, rest).
// - Intuition: `held` = max(held, rest - p) [must buy from rest], `sold` = held + p [selling today], `rest` = max(rest, prevSold) [idle or cooldown ended].
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 19. BEST TIME TO BUY AND SELL STOCK WITH TRANSACTION FEE — LeetCode 714
// ============================================================

int maxProfitFee(vector<int>& prices, int fee) {
    int cash = 0, hold = -1e9;
    for (int p : prices) {
        cash = max(cash, hold + p - fee);
        hold = max(hold, cash - p);
    }
    return cash;
}
// Interview Explanation:
// - Problem Statement: Maximize profit with unlimited transactions and a transaction fee per sell (LeetCode 714).
// - Approach: 2-State Dynamic Programming (cash, hold).
// - Intuition: `cash` = max balance without holding a share (selling pays fee: hold + p - fee). `hold` = max balance holding a share (buying: cash - p).
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 20. HOUSE ROBBER I — LeetCode 198
// ============================================================

int robI(vector<int>& nums) {
    int prev1 = 0, prev2 = 0;
    for (int x : nums) {
        int cur = max(prev1, prev2 + x);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Find maximum amount of money you can rob without alerting police (no 2 adjacent houses) (LeetCode 198).
// - Approach: 1D Dynamic Programming with Space Optimization.
// - Intuition: At house i, choose between not robbing it (prev1) or robbing it (prev2 + nums[i]). Maintain two variables for O(1) space.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 21. HOUSE ROBBER II — LeetCode 213
// ============================================================

int robII(vector<int>& nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    auto robRange = [&](int l, int r) {
        int prev1 = 0, prev2 = 0;
        for (int i = l; i <= r; i++) {
            int cur = max(prev1, prev2 + nums[i]);
            prev2 = prev1; prev1 = cur;
        }
        return prev1;
    };
    return max(robRange(0, n - 2), robRange(1, n - 1));
}
// Interview Explanation:
// - Problem Statement: Maximize robbery money on a circular street where first and last houses are adjacent (LeetCode 213).
// - Approach: Split into Two Linear Subproblems.
// - Intuition: You cannot rob both house 0 and house n - 1. Thus the answer is max(robLinear(0...n-2), robLinear(1...n-1)).
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 22. HOUSE ROBBER III — LeetCode 337
// ============================================================

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x = 0, TreeNode *l = nullptr, TreeNode *r = nullptr) : val(x), left(l), right(r) {}
};

pair<int, int> dfsRobTree(TreeNode* root) {
    if (!root) return {0, 0};
    auto [lRob, lSkip] = dfsRobTree(root->left);
    auto [rRob, rSkip] = dfsRobTree(root->right);
    int robThis = root->val + lSkip + rSkip;
    int skipThis = max(lRob, lSkip) + max(rRob, rSkip);
    return {robThis, skipThis};
}

int robIII(TreeNode* root) {
    auto [r, s] = dfsRobTree(root);
    return max(r, s);
}
// Interview Explanation:
// - Problem Statement: Maximize robbery on a binary tree of houses where parent and child cannot both be robbed (LeetCode 337).
// - Approach: Post-Order Tree Dynamic Programming (Bottom-Up DFS).
// - Intuition: For each node, return a pair {robThis, skipThis}. If robThis, add root->val + leftSkip + rightSkip. If skipThis, take max(lRob, lSkip) + max(rRob, rSkip).
// - Complexity: Time: O(N), Space: O(H) recursion stack.

// ============================================================
// 23. HOUSE ROBBER IV — LeetCode 2560
// ============================================================

int minCapability(vector<int>& nums, int k) {
    int lo = *min_element(nums.begin(), nums.end()), hi = *max_element(nums.begin(), nums.end());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2, count = 0, n = nums.size();
        for (int i = 0; i < n; i++) {
            if (nums[i] <= mid) { count++; i++; }
        }
        if (count >= k) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
// Interview Explanation:
// - Problem Statement: Find minimum capability to rob at least k non-adjacent houses, where capability is max money stolen from any single house (LeetCode 2560).
// - Approach: Binary Search on Answer + Greedy Non-Adjacent Count.
// - Intuition: The predicate "can we steal >= k non-adjacent houses each <= cap" is monotonic. For a test cap `mid`, greedily take the first valid house and skip its adjacent neighbor.
// - Complexity: Time: O(N log(max - min)), Space: O(1).

// ============================================================
// 24. DELETE AND EARN (HOUSE ROBBER V) — LeetCode 740
// ============================================================

int deleteAndEarn(vector<int>& nums) {
    if (nums.empty()) return 0;
    int maxVal = *max_element(nums.begin(), nums.end());
    vector<int> sum(maxVal + 1, 0);
    for (int x : nums) sum[x] += x;
    int prev1 = 0, prev2 = 0;
    for (int i = 0; i <= maxVal; i++) {
        int cur = max(prev1, prev2 + sum[i]);
        prev2 = prev1; prev1 = cur;
    }
    return prev1;
}
// Interview Explanation:
// - Problem Statement: Maximize earned points by taking nums[i] and deleting all instances of nums[i]-1 and nums[i]+1 (LeetCode 740).
// - Approach: Reduction to House Robber over Value Buckets.
// - Intuition: Group numbers into total points per value sum[v] = v * freq[v]. Since taking value v destroys v-1 and v+1, adjacent values cannot be picked—exactly House Robber I on the value domain.
// - Complexity: Time: O(N + max(nums)), Space: O(max(nums)).

// ============================================================
// 25. COIN CHANGE I (FEWEST COINS) — LeetCode 322
// ============================================================

int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, 1e9);
    dp[0] = 0;
    for (int c : coins) {
        for (int x = c; x <= amount; x++) dp[x] = min(dp[x], dp[x - c] + 1);
    }
    return dp[amount] >= 1e9 ? -1 : dp[amount];
}
// Interview Explanation:
// - Problem Statement: Find fewest number of coins to make amount using infinite supply of given coin denominations (LeetCode 322).
// - Approach: 1D Unbounded Knapsack Dynamic Programming.
// - Intuition: For each amount x and coin c, dp[x] = min(dp[x], dp[x - c] + 1). Initialize dp[0] = 0 and rest to infinity.
// - Complexity: Time: O(N * amount), Space: O(amount).

// ============================================================
// 26. COIN CHANGE II (UNIQUE COMBINATIONS) — LeetCode 518
// ============================================================

int change(int amount, vector<int>& coins) {
    vector<int> dp(amount + 1, 0);
    dp[0] = 1;
    for (int c : coins) {
        for (int x = c; x <= amount; x++) dp[x] = (dp[x] + dp[x - c]) % MOD;
    }
    return dp[amount];
}
// Interview Explanation:
// - Problem Statement: Count total distinct combinations of coins that make up amount (order does not matter) (LeetCode 518).
// - Approach: 1D Unbounded Combination DP (Outer Coin Loop).
// - Intuition: Iterating over coins in the outer loop ensures coins are considered in fixed order, avoiding duplicate permutations like [1,2] and [2,1].
// - Complexity: Time: O(N * amount), Space: O(amount).

// ============================================================
// 27. COIN CHANGE PERMUTATIONS (ORDERED WAYS) — CSES / LeetCode 377
// ============================================================

int combinationSum4(vector<int>& coins, int amount) {
    vector<ll> dp(amount + 1, 0);
    dp[0] = 1;
    for (int x = 1; x <= amount; x++) {
        for (int c : coins) {
            if (x >= c) dp[x] = (dp[x] + dp[x - c]) % MOD;
        }
    }
    return dp[amount];
}
// Interview Explanation:
// - Problem Statement: Count total distinct ordered sequences of coins that sum to amount ([1,2] != [2,1]) (LeetCode 377).
// - Approach: 1D Unbounded Permutation DP (Outer Amount Loop).
// - Intuition: Iterating over target amounts in the outer loop allows any coin to be the last coin placed, counting all ordered arrangements.
// - Complexity: Time: O(N * amount), Space: O(amount).

// ============================================================
// 28. BOUNDED COIN CHANGE (LIMITED SUPPLY)
// ============================================================

int coinChangeBounded(vector<int>& coins, vector<int>& limits, int amount) {
    vector<int> dp(amount + 1, 1e9);
    dp[0] = 0;
    for (int i = 0; i < (int)coins.size(); i++) {
        int c = coins[i], lim = limits[i];
        for (int k = 1; lim > 0; k <<= 1) {
            int take = min(k, lim);
            int weight = take * c, cost = take;
            for (int x = amount; x >= weight; x--) dp[x] = min(dp[x], dp[x - weight] + cost);
            lim -= take;
        }
    }
    return dp[amount] >= 1e9 ? -1 : dp[amount];
}
// Interview Explanation:
// - Problem Statement: Find minimum coins to make amount where denomination coins[i] has limited count limits[i].
// - Approach: Binary Power Splitting + 0/1 Knapsack Backwards DP.
// - Intuition: Decompose item quantities into powers of 2 (1, 2, 4, ..., remainder) to reduce O(limit) transitions to O(log limit), then perform standard 0/1 backwards DP relaxation.
// - Complexity: Time: O(amount * sum(log(limit))), Space: O(amount).

// ============================================================
// 29. COIN CHANGE PATH RECONSTRUCTION (PRINT COINS)
// ============================================================

vector<int> reconstructCoins(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, 1e9), parent(amount + 1, -1);
    dp[0] = 0;
    for (int c : coins) {
        for (int x = c; x <= amount; x++) {
            if (dp[x - c] + 1 < dp[x]) {
                dp[x] = dp[x - c] + 1;
                parent[x] = c;
            }
        }
    }
    if (dp[amount] >= 1e9) return {};
    vector<int> res;
    for (int curr = amount; curr > 0; curr -= parent[curr]) res.push_back(parent[curr]);
    return res;
}
// Interview Explanation:
// - Problem Statement: Reconstruct and return the exact coin denominations used to make amount with fewest coins.
// - Approach: DP Predecessor Tracking + Backtracking Path Recovery.
// - Intuition: Maintain parent[x] storing the last coin denomination used to achieve optimal state dp[x]. Trace back from amount to 0 subtracting parent[curr].
// - Complexity: Time: O(N * amount), Space: O(amount).

// ============================================================
// 30. REACHABLE MONEY SUMS — CSES Money Sums
// ============================================================

vector<int> getReachableSums(vector<int>& coins) {
    int total = accumulate(coins.begin(), coins.end(), 0);
    vector<bool> dp(total + 1, false);
    dp[0] = true;
    for (int c : coins) {
        for (int x = total; x >= c; x--) {
            if (dp[x - c]) dp[x] = true;
        }
    }
    vector<int> sums;
    for (int x = 1; x <= total; x++) if (dp[x]) sums.push_back(x);
    return sums;
}
// Interview Explanation:
// - Problem Statement: Find all distinct possible sum values that can be formed using any subset of the coins (CSES Money Sums).
// - Approach: 0/1 Knapsack Boolean Reachability DP (Backwards Iteration).
// - Intuition: dp[x] is true if sum x can be formed. Transition backwards from total to c: dp[x] = dp[x] | dp[x - c]. Output all positive reachable indices.
// - Complexity: Time: O(N * sum(coins)), Space: O(sum(coins)).

// ============================================================
// 31. DUTCH NATIONAL FLAG SORT (0, 1, 2 SORT) — LeetCode 75
// ============================================================

void sortColors(vector<int>& nums) {
    int lo = 0, mid = 0, hi = (int)nums.size() - 1;
    while (mid <= hi) {
        if (nums[mid] == 0) swap(nums[lo++], nums[mid++]);
        else if (nums[mid] == 1) mid++;
        else swap(nums[mid], nums[hi--]);
    }
}
// Interview Explanation:
// - Problem Statement: Sort an array with elements 0, 1, and 2 in-place in a single pass (LeetCode 75).
// - Approach: Dijkstra's 3-Way Partitioning / Dutch National Flag.
// - Intuition: Maintain 3 pointers: lo (boundary for 0s), mid (current explorer), hi (boundary for 2s). When nums[mid]==0 swap with lo; if 2 swap with hi; if 1 just advance mid.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 32. COUNTING SORT (STABLE IMPLEMENTATION)
// ============================================================

vector<int> countingSort(vector<int>& arr) {
    if (arr.empty()) return {};
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int k = maxVal - minVal + 1;
    vector<int> count(k, 0), output(arr.size());
    for (int x : arr) count[x - minVal]++;
    for (int i = 1; i < k; i++) count[i] += count[i - 1];
    for (int i = (int)arr.size() - 1; i >= 0; i--) output[--count[arr[i] - minVal]] = arr[i];
    return output;
}
// Interview Explanation:
// - Problem Statement: Sort an integer array with bounded range [minVal, maxVal] stably in linear time.
// - Approach: Frequency Count Accumulation + Reverse Position Placement.
// - Intuition: Build prefix sum table of frequency counts to determine exact final output index for each key. Iterating backwards ensures stability for duplicate values.
// - Complexity: Time: O(N + K), Space: O(N + K).

// ============================================================
// 33. RADIX SORT (LSD DIGIT BY DIGIT)
// ============================================================

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = *max_element(arr.begin(), arr.end());
    for (long long exp = 1; maxVal / exp > 0; exp *= 10) {
        vector<int> output(arr.size()), count(10, 0);
        for (int x : arr) count[(x / exp) % 10]++;
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];
        for (int i = (int)arr.size() - 1; i >= 0; i--)
            output[--count[(arr[i] / exp) % 10]] = arr[i];
        arr = output;
    }
}
// Interview Explanation:
// - Problem Statement: Sort integer array in non-comparative linear time regardless of value magnitude.
// - Approach: Least Significant Digit (LSD) Radix Sort with Counting Sort Subroutine.
// - Intuition: Sort stably by each digit position from least to most significant (1s, 10s, 100s...). Stability ensures higher-order sorts preserve lower-order sortedness.
// - Complexity: Time: O(D * (N + B)), Space: O(N + B) where B=10 is base and D=digits.

// ============================================================
// 34. BUCKET SORT (UNIFORM DISTRIBUTION)
// ============================================================

void bucketSort(vector<float>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    vector<vector<float>> buckets(n);
    for (float x : arr) {
        int idx = min(n - 1, max(0, (int)(n * x)));
        buckets[idx].push_back(x);
    }
    int k = 0;
    for (int i = 0; i < n; i++) {
        sort(buckets[i].begin(), buckets[i].end());
        for (float x : buckets[i]) arr[k++] = x;
    }
}
// Interview Explanation:
// - Problem Statement: Sort elements uniformly distributed in range [0, 1) in expected linear time.
// - Approach: Scatter-Gather Bucket Sorting.
// - Intuition: Partition range into n equal sub-intervals (buckets). Distribute elements into buckets, sort individual buckets (typically with insertion sort), and concatenate.
// - Complexity: Time: O(N) average, O(N^2) worst, Space: O(N).

// ============================================================
// 35. CYCLIC SORT [1 TO N] (IN-PLACE INDEX MAPPING)
// ============================================================

void cyclicSort(vector<int>& nums) {
    int i = 0, n = nums.size();
    while (i < n) {
        int correctIdx = nums[i] - 1;
        if (nums[i] > 0 && nums[i] <= n && nums[i] != nums[correctIdx]) swap(nums[i], nums[correctIdx]);
        else i++;
    }
}
// Interview Explanation:
// - Problem Statement: Sort array containing integers in range [1, N] in-place in O(N) time with O(1) space.
// - Approach: Cyclic In-Place Index Mapping Swap.
// - Intuition: Each number x belongs at index x - 1. If nums[i] is not at its correct index, swap it there. Each swap places at least one number in its permanent position (at most N swaps).
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 36. PIGEONHOLE SORT (SLOT PLACEMENT)
// ============================================================

void pigeonholeSort(vector<int>& arr) {
    if (arr.empty()) return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;
    vector<vector<int>> holes(range);
    for (int x : arr) holes[x - minVal].push_back(x);
    int idx = 0;
    for (int i = 0; i < range; i++) {
        for (int x : holes[i]) arr[idx++] = x;
    }
}
// Interview Explanation:
// - Problem Statement: Sort integer array where range of key values is approximately equal to the number of elements.
// - Approach: Direct Pigeonhole / Bucket Slot Mapping.
// - Intuition: Allocate an array of holes corresponding to each possible key value in [minVal, maxVal]. Place keys directly into their matching hole and collect sequentially.
// - Complexity: Time: O(N + Range), Space: O(N + Range).

// ============================================================
// 37. PATIENCE SORTING (PILES & LIS EXTRACTION)
// ============================================================

vector<int> patienceSortLIS(vector<int>& arr) {
    vector<int> topCards;
    for (int x : arr) {
        auto it = lower_bound(topCards.begin(), topCards.end(), x);
        if (it == topCards.end()) topCards.push_back(x);
        else *it = x;
    }
    return topCards;
}
// Interview Explanation:
// - Problem Statement: Find Longest Increasing Subsequence and sort elements using card game patience mechanics.
// - Approach: Greedy Pile Placement + Binary Search (lower_bound).
// - Intuition: Place cards onto the leftmost pile whose top card is >= current card (or create new pile). The number of piles formed equals the length of the LIS (Greene's Theorem).
// - Complexity: Time: O(N log N), Space: O(N).

// ============================================================
// 38. 3-WAY QUICKSORT (DUTCH NATIONAL FLAG PARTITION)
// ============================================================

void quickSort3Way(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int pivot = arr[l], lt = l, gt = r, i = l + 1;
    while (i <= gt) {
        if (arr[i] < pivot) swap(arr[lt++], arr[i++]);
        else if (arr[i] > pivot) swap(arr[i], arr[gt--]);
        else i++;
    }
    quickSort3Way(arr, l, lt - 1);
    quickSort3Way(arr, gt + 1, r);
}
// Interview Explanation:
// - Problem Statement: Sort array containing massive numbers of duplicate elements in optimal O(N log K) time.
// - Approach: Bentley-McIlroy 3-Way Partition QuickSort.
// - Intuition: Partition array into three subarrays: [< pivot], [== pivot], and [> pivot]. Recurse only on the strictly smaller and strictly larger segments, leaving all duplicates unvisited.
// - Complexity: Time: O(N log N) average (O(N) with all duplicates), Space: O(log N) call stack.