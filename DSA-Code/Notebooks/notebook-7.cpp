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
 | 1  | Jump Game III (LeetCode 1306)              | Graph BFS Reachability            | O(N)     | O(N)     |
 | 2  | Jump Game IV (LeetCode 1345)              | BFS + Same-Value Pruning          | O(N)     | O(N)     |
 | 3  | Jump Game V (LeetCode 1340)               | Memoized DFS / DAG Longest Path   | O(N * D) | O(N)     |
 | 4  | Jump Game VII (LeetCode 1871)             | Sliding Window Reachability DP    | O(N)     | O(N)     |
 | 5  | House Robber IV (LeetCode 2560)           | Binary Search on Min Capability   | O(N logM)| O(1)     |
 | 6  | Bounded Coin Change (Limited Supply)       | Binary Splitting + 0/1 Knapsack   | O(A logK)| O(A)     |
 | 7  | Coin Change Path Reconstruction            | DP Predecessor Pointer Traceback  | O(N * A) | O(A)     |
 ====================================================================================================
*/

// ============================================================
// 1. JUMP GAME III — LeetCode 1306
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
// 2. JUMP GAME IV — LeetCode 1345
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
// - Intuition: Group indices by value in a hash map. During BFS expansion, push adjacent indices and all same-value indices, then clear the map entry to prevent redundant expansions.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 3. JUMP GAME V — LeetCode 1340
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
// 4. JUMP GAME VII — LeetCode 1871
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
// - Intuition: dp[i] is true if s[i] == '0' and there is at least one true dp[j] in window [i - maxJump, i - minJump]. Maintain a running count reach of true states entering and exiting the valid window.
// - Complexity: Time: O(N), Space: O(N).


// ============================================================
// 5. HOUSE ROBBER IV — LeetCode 2560
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
// - Intuition: The predicate "can we steal >= k non-adjacent houses each <= cap" is monotonic. For a test cap mid, greedily take the first valid house and skip its adjacent neighbor.
// - Complexity: Time: O(N log(max - min)), Space: O(1).


// ============================================================
// 6. BOUNDED COIN CHANGE (LIMITED SUPPLY)
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
// 7. COIN CHANGE PATH RECONSTRUCTION (PRINT COINS)
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