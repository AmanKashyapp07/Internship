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

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                    PROBLEM SUMMARY & COMPLEXITY TABLE: G3.CPP (Problems 45 - 66)                    
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 45 | Remove Invalid Parentheses                  | Backtracking DFS Pruning          | O(2^N)   | O(N)     |
 | 46 | Maximum Swap                                | Last Digit Occurrence Table       | O(N)     | O(1)     |
 | 47 | Largest Number                              | Custom Lexicographical Sort       | O(N logN) | O(N)     |
 | 48 | Connect Ropes With Minimum Cost             | Min-Heap Huffman Optimal Merge    | O(N logN) | O(N)     |
 | 49 | Kth Largest Element in an Array             | Min-Heap Fixed-Size Window        | O(N logK) | O(K)     |
 | 50 | Last Stone Weight II                        | 0/1 Knapsack Subset Sum Reduction | O(N * S) | O(S)     |
 | 51 | Furthest Building You Can Reach             | Min-Heap Ladders / Bricks Greed   | O(N logL) | O(L)     |
 | 52 | Minimum Refueling Stops                     | Max-Heap Past Gas Stations        | O(N logN) | O(N)     |
 | 53 | Trapping Rain Water II (3D)                 | Min-Heap Boundary Priority Queue  | O(MNlogMN | O(MN)    |
 | 54 | Maximum Average Pass Ratio                  | Max-Heap Marginal Gain Extraction | O(E logN) | O(N)     |
 | 55 | Total Cost to Hire K Workers                | Dual Min-Heaps Two-Ended Window   | O(K logC) | O(C)     |
 | 56 | Hand of Straights                           | Ordered Map Consecutive Grouping  | O(N logN) | O(N)     |
 | 57 | Valid Arrangement of Pairs                  | Hierholzer's Directed Euler Path  | O(V + E) | O(V + E) |
 | 58 | Patching Array                              | Greedy Reachable Range Doubling   | O(M+logN) | O(1)     |
 | 59 | Wiggle Subsequence                          | Greedy Alternating Extremes Peak  | O(N)     | O(1)     |
 | 60 | Monotone Increasing Digits                  | Reverse Inversion Scan + Suffix 9 | O(D)     | O(D)     |
 | 61 | Increasing Triplet Subsequence              | Two-Threshold Greedy Smallest     | O(N)     | O(1)     |
 | 62 | Maximum Length of Pair Chain                | Interval Scheduling by End Time   | O(N logN) | O(1)     |
 | 63 | Russian Doll Envelopes                      | Width Sort + Patience LIS         | O(N logN) | O(N)     |
 | 64 | Shortest Unsorted Continuous Subarray       | Two Pointers Running Min/Max Pass | O(N)     | O(1)     |
 | 65 | Huffman Encoding                            | Min-Heap Tree Merge + DFS Codes   | O(N logN) | O(N)     |
 | 66 | Fractional Knapsack                         | Value-to-Weight Ratio Descending  | O(N logN) | O(1)     |
 ====================================================================================================
*/

// =========================================================
// 45. REMOVE INVALID PARENTHESES [G-45]
// =========================================================

vector<string> removeInvalidParentheses(string s) {
    vector<string> ans;
    auto isValid = [](const string& str) {
        int cnt = 0;
        for (char c : str) {
            if (c == '(') cnt++;
            else if (c == ')' && --cnt < 0) return false;
        }
        return cnt == 0;
    };

    int l = 0, r = 0;
    for (char c : s) {
        if (c == '(') l++;
        else if (c == ')') {
            if (l > 0) l--;
            else r++;
        }
    }

    auto dfs = [&](auto& self, string cur, int start, int remL, int remR) -> void {
        if (remL == 0 && remR == 0) {
            if (isValid(cur)) ans.push_back(cur);
            return;
        }
        for (int i = start; i < (int)cur.size(); ++i) {
            if (i > start && cur[i] == cur[i - 1]) continue;
            if (remL > 0 && cur[i] == '(')
                self(self, cur.substr(0, i) + cur.substr(i + 1), i, remL - 1, remR);
            if (remR > 0 && cur[i] == ')')
                self(self, cur.substr(0, i) + cur.substr(i + 1), i, remL, remR - 1);
        }
    };

    dfs(dfs, s, 0, l, r);
    return ans;
}
// Interview Explanation:
// - Problem Statement: Remove minimum invalid parentheses and return all unique valid results.
// - Approach: Backtracking DFS with Exact Discard Count Pruning.
// - Intuition:
//   * Pass 1: compute the exact count of misplaced '(' (leftRem) and ')' (rightRem).
//   * Launch backtracking DFS tracking running balance open.
//   * Only branch to discard '(' if leftRem > 0; only discard ')' if rightRem > 0.
//   * Only keep ')' if open > 0 to maintain prefix validity.
//   * Deduplicate leaf results using a hash set.
// - Complexity: Time: O(2^N) bounded by exact removals, Space: O(N).


// =========================================================
// 46. MAXIMUM SWAP [G-46]
// =========================================================

int maximumSwap(int num) {
    string s = to_string(num);
    int last[10] = {0}, n = s.size();
    for (int i = 0; i < n; ++i) last[s[i] - '0'] = i;

    for (int i = 0; i < n; ++i) {
        for (int d = 9; d > s[i] - '0'; --d) {
            if (last[d] > i) {
                swap(s[i], s[last[d]]);
                return stoi(s);
            }
        }
    }
    return num;
}
// Interview Explanation:
// - Problem Statement: Swap two digits at most once to get maximum possible value.
// - Approach: Last Digit Occurrence Table + Greedy Left-to-Right Scan.
// - Intuition:
//   * Modifying more significant (leftmost) digits provides the greatest value increase.
//   * Precompute the last index of each digit 0..9.
//   * Scan digits from left to right.
//   * For each digit d, check if any larger digit (9 down to d+1) occurs at an index greater than current index i.
//   * Perform the swap with the largest available digit's last occurrence and return immediately.
// - Complexity: Time: O(N) where N <= 9 digits, Space: O(1).


// =========================================================
// 47. LARGEST NUMBER [G-47]
// =========================================================

string largestNumber(vector<int>& a) {
    vector<string> s;
    for (int x : a) s.push_back(to_string(x));
    sort(s.begin(), s.end(), [](auto& x, auto& y) { return x + y > y + x; });
    if (s[0] == "0") return "0";
    string ans = "";
    for (auto& str : s) ans += str;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Arrange list of non-negative integers such that they form the largest number.
// - Approach: Custom Lexicographical String Sorting.
// - Intuition:
//   * Standard integer or single-string comparison fails (e.g., "3" vs "30": "330" > "303").
//   * Define custom comparator: string a precedes b iff (a + b) > (b + a).
//   * This relation is transitive and defines a strict weak ordering.
//   * Sort strings under this comparator and concatenate.
//   * Handle edge case where highest element is "0" (return "0").
// - Complexity: Time: O(N log N * L), Space: O(N * L).


// =========================================================
// 48. CONNECT ROPES WITH MINIMUM COST [G-48]
// =========================================================

long long minCostToConnectRopes(vector<long long>& a) {
    priority_queue<long long, vector<long long>, greater<long long>> pq(a.begin(), a.end());
    long long ans = 0;
    while (pq.size() > 1) {
        long long x = pq.top(); pq.pop();
        long long y = pq.top(); pq.pop();
        ans += x + y;
        pq.push(x + y);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Connect n ropes with minimum total cost where cost to connect two ropes is sum of their lengths.
// - Approach: Min-Heap Greedy Huffman Optimal Merge Pattern.
// - Intuition:
//   * Ropes combined earlier contribute to multiple subsequent additions in the merge tree.
//   * To minimize total cost, shorter ropes should participate in more merges, while longer ropes should participate in fewer.
//   * Push all lengths into a min-heap.
//   * Repeatedly extract the two shortest ropes, combine them, accumulate cost, and push the combined rope back.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 49. KTH LARGEST ELEMENT IN AN ARRAY [G-49]
// =========================================================

int findKthLargest(vector<int>& a, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int x : a) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();
    }
    return pq.top();
}
// Interview Explanation:
// - Problem Statement: Find k-th largest element in an unsorted array.
// - Approach: Min-Heap Fixed-Size Window of Size k.
// - Intuition:
//   * Maintain the k largest elements seen so far in a min-heap.
//   * The top of the min-heap always holds the smallest among the k largest elements.
//   * When heap size exceeds k, evict pq.top().
//   * At the end of traversal, pq.top() is precisely the k-th largest element in the entire array.
// - Complexity: Time: O(N log K), Space: O(K).


// =========================================================
// 50. LAST STONE WEIGHT II [G-50]
// =========================================================

int lastStoneWeightII(vector<int>& stones) {
    int sum = accumulate(stones.begin(), stones.end(), 0);
    bitset<1501> dp = {1};
    for (int s : stones) dp |= dp << s;
    for (int i = sum / 2; i >= 0; --i)
        if (dp[i]) return sum - 2 * i;
    return 0;
}
// Interview Explanation:
// - Problem Statement: Minimize remaining stone weight after arbitrary smash orders.
// - Approach: 0/1 Knapsack Subset Sum Mathematical Reduction.
// - Intuition:
//   * Any sequence of stone smashes assigns '+' or '-' signs to each stone weight: sum(S1) - sum(S2).
//   * Minimizing the difference is equivalent to finding a subset S1 whose sum is as close to total / 2 as possible.
//   * Use 0/1 Knapsack boolean DP with target = total / 2.
//   * Best achievable subset sum i <= total / 2 yields minimal final weight (total - 2*i).
// - Complexity: Time: O(N * (Total / 2)), Space: O(Total / 2).


// =========================================================
// 51. FURTHEST BUILDING YOU CAN REACH [G-51]
// =========================================================

int furthestBuilding(vector<int>& h, int bricks, int ladders) {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < (int)h.size() - 1; ++i) {
        int diff = h[i + 1] - h[i];
        if (diff > 0) pq.push(diff);
        if ((int)pq.size() > ladders) {
            bricks -= pq.top();
            pq.pop();
        }
        if (bricks < 0) return i;
    }
    return h.size() - 1;
}
// Interview Explanation:
// - Problem Statement: Find furthest building reachable using limited bricks and ladders.
// - Approach: Min-Heap Greedy Allocation (Ladders for Largest Climbs, Bricks for Smallest).
// - Intuition:
//   * Ladders can cover any height difference regardless of magnitude; bricks scale linearly with height.
//   * Therefore, ladders should greedily be reserved for the largest climbs encountered.
//   * Tentatively assign ladders to all climbs by pushing climb heights into a min-heap.
//   * If ladder count is exceeded, convert the smallest climb seen so far into a brick expenditure (pq.top()).
//   * If required bricks exceed available supply, no further progress is possible; return building index i.
// - Complexity: Time: O(N log(Ladders)), Space: O(Ladders).


// =========================================================
// 52. MINIMUM REFUELING STOPS [G-52]
// =========================================================

int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
    priority_queue<int> pq;
    long long cur = startFuel;
    int ans = 0, i = 0, n = stations.size();
    while (cur < target) {
        while (i < n && stations[i][0] <= cur) pq.push(stations[i++][1]);
        if (pq.empty()) return -1;
        cur += pq.top(); pq.pop();
        ans++;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find minimum refueling stops to reach target starting with startFuel.
// - Approach: Max-Heap of Passed Gas Stations.
// - Intuition:
//   * Drive as far as possible with current fuel.
//   * Enqueue the fuel of every gas station passed along the way into a max-heap.
//   * When fuel is insufficient to reach the next station or target, retroactively refuel at the station with the largest fuel capacity seen so far.
//   * If the heap empties before reaching the destination, the target is unreachable; return -1.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 53. TRAPPING RAIN WATER II [G-53]
// =========================================================

int trapRainWater(vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size();
    if (m <= 2 || n <= 2) return 0;

    using T = tuple<int, int, int>; // {height, r, c}
    priority_queue<T, vector<T>, greater<T>> pq;
    vector<vector<bool>> vis(m, vector<bool>(n));

    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (r == 0 || r == m - 1 || c == 0 || c == n - 1) {
                pq.push({g[r][c], r, c});
                vis[r][c] = true;
            }
        }
    }

    int ans = 0, maxH = 0;
    int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};

    while (!pq.empty()) {
        auto [h, r, c] = pq.top(); pq.pop();
        maxH = max(maxH, h);

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && !vis[nr][nc]) {
                vis[nr][nc] = true;
                if (g[nr][nc] < maxH) ans += maxH - g[nr][nc];
                pq.push({g[nr][nc], nr, nc});
            }
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find volume of water trapped after rain in a 3D elevation matrix.
// - Approach: Min-Heap Boundary Priority Queue (Dijkstra-like Contraction).
// - Intuition:
//   * Water spill is bounded by the lowest point along the entire surrounding perimeter.
//   * Initialize a min-heap containing all boundary cells of the grid.
//   * Repeatedly pop the lowest boundary cell and maintain maxBoundary seen so far.
//   * For each unvisited neighbor, if its height is lower than maxBoundary, it traps (maxBoundary - height) water.
//   * Push the neighbor into the heap, effectively contracting the boundary inward.
// - Complexity: Time: O(M * N log(M * N)), Space: O(M * N).


// =========================================================
// 54. MAXIMUM AVERAGE PASS RATIO [G-54]
// =========================================================

double maxAverageRatio(vector<vector<int>>& classes, int extra) {
    auto gain = [](int p, int t) {
        return (double)(p + 1) / (t + 1) - (double)p / t;
    };

    priority_queue<tuple<double, int, int>> pq;
    for (auto& c : classes)
        pq.push({gain(c[0], c[1]), c[0], c[1]});

    while (extra--) {
        auto [_, p, t] = pq.top(); pq.pop();
        pq.push({gain(p + 1, t + 1), p + 1, t + 1});
    }

    double sum = 0;
    while (!pq.empty()) {
        auto [_, p, t] = pq.top(); pq.pop();
        sum += (double)p / t;
    }
    return sum / classes.size();
}
// Interview Explanation:
// - Problem Statement: Assign extraStudents to classes maximizing average pass ratio (passi / totali).
// - Approach: Max-Heap Marginal Gain Greedy Assignment.
// - Intuition:
//   * Adding 1 student increases a class pass ratio by marginal gain: (p+1)/(t+1) - p/t.
//   * To maximize overall average, each extra student must greedily be allocated to the class offering the highest marginal gain.
//   * Maintain a max-heap of classes prioritized by current marginal gain.
//   * In each step, assign 1 student to top class and reinsert with updated marginal gain.
// - Complexity: Time: O((N + Extra) log N), Space: O(N).


// =========================================================
// 55. TOTAL COST TO HIRE K WORKERS [G-55]
// =========================================================

long long totalCost(vector<int>& costs, int k, int candidates) {
    priority_queue<int, vector<int>, greater<int>> pq1, pq2;
    int i = 0, j = (int)costs.size() - 1;
    long long ans = 0;

    while (k--) {
        while ((int)pq1.size() < candidates && i <= j) pq1.push(costs[i++]);
        while ((int)pq2.size() < candidates && i <= j) pq2.push(costs[j--]);

        int a = pq1.empty() ? INT_MAX : pq1.top();
        int b = pq2.empty() ? INT_MAX : pq2.top();

        if (a <= b) {
            ans += a;
            pq1.pop();
        } else {
            ans += b;
            pq2.pop();
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Hire k workers with minimum total cost choosing from first or last candidates workers in each step.
// - Approach: Dual Min-Heaps with Two-Ended Inward Window.
// - Intuition:
//   * Maintain two min-heaps: leftPq storing up to candidates workers from the front, and rightPq from the back.
//   * Compare the tops of both heaps. Select the worker with the smaller cost (tie-broken by left heap).
//   * Replenish the chosen heap with the next unselected worker from inward pointers (l or r).
//   * Repeats k times in O(K log(candidates)) total time.
// - Complexity: Time: O(K log(Candidates)), Space: O(Candidates).


// =========================================================
// 56. HAND OF STRAIGHTS [G-56]
// =========================================================

bool isNStraightHand(vector<int>& hand, int groupSize) {
    if ((int)hand.size() % groupSize != 0) return false;
    map<int, int> count;
    for (int x : hand) count[x]++;
    for (auto [x, c] : count) {
        if (c > 0) {
            for (int i = 0; i < groupSize; ++i) {
                if (count[x + i] < c) return false;
                count[x + i] -= c;
            }
        }
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Reorder cards into groups of groupSize where each group consists of consecutive cards.
// - Approach: Ordered Map Frequency Greedy Extraction.
// - Intuition:
//   * Total cards must be divisible by groupSize.
//   * The smallest remaining card start must form the base of the next group [start, start + groupSize - 1].
//   * Count frequencies in an ordered map.
//   * Repeatedly inspect mp.begin()->first; verify and decrement frequencies for the subsequent (groupSize - 1) cards.
//   * If any consecutive card is missing, valid grouping is impossible.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 57. VALID ARRANGEMENT OF PAIRS [G-57]
// =========================================================

vector<vector<int>> validArrangement(vector<vector<int>>& pairs) {
    unordered_map<int, vector<int>> adj;
    unordered_map<int, int> deg;
    for (const auto& p : pairs) {
        adj[p[0]].push_back(p[1]);
        deg[p[0]]++;
        deg[p[1]]--;
    }

    int start = pairs[0][0];
    for (const auto& [u, d] : deg) {
        if (d == 1) {
            start = u;
            break;
        }
    }

    vector<int> path, st = {start};
    while (!st.empty()) {
        int u = st.back();
        if (!adj[u].empty()) {
            int v = adj[u].back();
            adj[u].pop_back();
            st.push_back(v);
        } else {
            path.push_back(u);
            st.pop_back();
        }
    }
    reverse(path.begin(), path.end());

    vector<vector<int>> ans;
    ans.reserve(pairs.size());
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
        ans.push_back({path[i], path[i + 1]});
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Arrange pairs such that end of pair i equals start of pair i + 1 for all pairs.
// - Approach: Hierholzer's Algorithm for Directed Eulerian Path.
// - Intuition:
//   * Each pair [u, v] forms a directed edge u -> v. The problem asks for an Eulerian path visiting every edge exactly once.
//   * In a directed Eulerian graph, the start node has outDegree - inDegree == 1 (or any node if all degrees balanced).
//   * Use Hierholzer's algorithm: traverse edges, pop used edges, push to path upon backtracking (post-order).
//   * Reversing path yields the complete valid Eulerian trail.
// - Complexity: Time: O(V + E), Space: O(V + E).


// =========================================================
// 58. PATCHING ARRAY [G-58]
// =========================================================

int minPatches(vector<int>& nums, int n) {
    long long miss = 1;
    int patches = 0, i = 0, m = nums.size();

    while (miss <= n) {
        if (i < m && nums[i] <= miss) {
            miss += nums[i++];
        } else {
            miss += miss; // greedily patch miss itself
            patches++;
        }
    }
    return patches;
}
// Interview Explanation:
// - Problem Statement: Find minimum patches added to sorted array nums so any number in [1, n] can be formed by a subset.
// - Approach: Greedy Range Doubling Invariant.
// - Intuition:
//   * Let miss be the smallest number in [1, n] that cannot currently be formed.
//   * The current formed range is [1, miss - 1].
//   * If nums[i] <= miss, incorporating nums[i] extends the reachable range to [1, miss + nums[i] - 1]; update miss += nums[i].
//   * If nums[i] > miss (or array exhausted), we must patch miss greedily; adding miss doubles reach to [1, 2*miss - 1].
// - Complexity: Time: O(M + log N), Space: O(1).


// =========================================================
// 59. WIGGLE SUBSEQUENCE [G-59]
// =========================================================

int wiggleMaxLength(vector<int>& a) {
    int n = a.size();
    if (n < 2) return n;

    int up = 1, down = 1;
    for (int i = 1; i < n; ++i) {
        if (a[i] > a[i - 1]) up = down + 1;
        else if (a[i] < a[i - 1]) down = up + 1;
    }
    return max(up, down);
}
// Interview Explanation:
// - Problem Statement: Find length of longest subsequence with strictly alternating positive and negative differences.
// - Approach: Greedy Alternating Peak / Valley Tracking.
// - Intuition:
//   * Maintain up (length of longest wiggle ending with an upward slope) and down (ending with a downward slope).
//   * If a[i] > a[i-1], an upward slope can extend any valid downward sequence: up = down + 1.
//   * If a[i] < a[i-1], a downward slope can extend any valid upward sequence: down = up + 1.
//   * Consecutive rises or falls naturally update the extreme without artificially inflating the alternating count.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 60. MONOTONE INCREASING DIGITS [G-60]
// =========================================================

int monotoneIncreasingDigits(int n) {
    string s = to_string(n);
    int mark = s.size();

    for (int i = (int)s.size() - 1; i > 0; --i) {
        if (s[i - 1] > s[i]) {
            s[i - 1]--;
            mark = i;
        }
    }

    for (int i = mark; i < (int)s.size(); ++i) {
        s[i] = '9';
    }
    return stoi(s);
}
// Interview Explanation:
// - Problem Statement: Find largest integer <= n whose digits are monotonically increasing.
// - Approach: Reverse Inversion Scan + Suffix 9s Greedy Substitution.
// - Intuition:
//   * Scan digits from right to left.
//   * Whenever s[i-1] > s[i], monotonicity is violated. Decrement s[i-1]-- and record split index mark = i.
//   * Decrementing s[i-1] allows all subsequent digits to be set to the maximum possible digit '9' without exceeding n.
//   * Set all digits from mark to end to '9'.
// - Complexity: Time: O(D) where D is number of digits (<= 10), Space: O(D).


// =========================================================
// 61. INCREASING TRIPLET SUBSEQUENCE [G-61]
// =========================================================

bool increasingTriplet(vector<int>& nums) {
    int first = INT_MAX, second = INT_MAX;

    for (int x : nums) {
        if (x <= first) first = x;
        else if (x <= second) second = x;
        else return true; // x > second > first
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if there exists a triplet of indices i < j < k with nums[i] < nums[j] < nums[k].
// - Approach: Two-Threshold Greedy Smallest Values Tracking.
// - Intuition:
//   * Maintain the smallest value first and second smallest value second in an increasing pair.
//   * If current x <= first, greedily update first = x.
//   * Else if x <= second, update second = x (a smaller upper bound for the second element is always preferable).
//   * If x > second, we have found an element greater than both first and second; return true!
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 62. MAXIMUM LENGTH OF PAIR CHAIN [G-62]
// =========================================================

int findLongestChain(vector<vector<int>>& pairs) {
    sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1];
    });

    int ans = 0, curEnd = -1e9;
    for (const auto& p : pairs) {
        if (p[0] > curEnd) {
            ans++;
            curEnd = p[1];
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find longest chain of pairs where pair [c, d] can follow [a, b] iff b < c.
// - Approach: Interval Scheduling by Earliest End Coordinate.
// - Intuition:
//   * Equivalent to classic interval scheduling / activity selection.
//   * Sort pairs ascending by their second element pairs[i][1].
//   * Greedily append pairs that finish earliest to leave maximum room for future chain links.
//   * Increment count whenever p[0] > curEnd and update curEnd = p[1].
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 63. RUSSIAN DOLL ENVELOPES [G-63]
// =========================================================

int maxEnvelopes(vector<vector<int>>& env) {
    sort(env.begin(), env.end(), [](const auto& a, const auto& b) {
        return a[0] == b[0] ? a[1] > b[1] : a[0] < b[0];
    });

    vector<int> tails;
    for (const auto& e : env) {
        int h = e[1];
        auto it = lower_bound(tails.begin(), tails.end(), h);
        if (it == tails.end()) tails.push_back(h);
        else *it = h;
    }
    return tails.size();
}
// Interview Explanation:
// - Problem Statement: Find maximum envelopes that can be Russian-dolled (one inside another).
// - Approach: Dual-Key Sorting + Patience Sorting (LIS) on Heights.
// - Intuition:
//   * Sort envelopes primarily by width ascending, and secondarily by height descending.
//   * Height descending prevents envelopes of the same width from nesting inside each other.
//   * With widths sorted, the problem reduces strictly to finding the Longest Increasing Subsequence (LIS) on heights.
//   * Solve LIS in O(N log N) using binary search (std::lower_bound) on tails array.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 64. SHORTEST UNSORTED CONTINUOUS SUBARRAY [G-64]
// =========================================================

int findUnsortedSubarray(vector<int>& nums) {
    int n = nums.size(), left = -1, right = -1;
    int mx = INT_MIN, mn = INT_MAX;

    for (int i = 0; i < n; i++) {
        // Left -> Right
        mx = max(mx, nums[i]);
        if (nums[i] < mx) right = i;

        // Right -> Left
        int j = n - 1 - i;
        mn = min(mn, nums[j]);
        if (nums[j] > mn) left = j;
    }

    return right == -1 ? 0 : right - left + 1;
}
// Interview Explanation:
// - Problem Statement: Find shortest continuous subarray whose sorting sorts the whole array.
// - Approach: Two Pointers Running Min/Max Extremes Sweep.
// - Intuition:
//   * In a sorted array, every element a[i] must be >= max(a[0..i-1]).
//   * Forward pass: if a[i] < curMax, a[i] is misplaced; rightmost such index is right.
//   * Backward pass: if a[i] > curMin, a[i] is misplaced; leftmost such index is left.
//   * If no misplaced elements exist (right == -1), array is already sorted; return 0.
//   * Otherwise, the required subarray length is right - left + 1.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 65. HUFFMAN ENCODING [G-65]
// =========================================================

struct HuffmanNode {
    int freq;
    HuffmanNode *left = nullptr, *right = nullptr;
    HuffmanNode(int f, HuffmanNode* l = nullptr, HuffmanNode* r = nullptr) : freq(f), left(l), right(r) {}
};

vector<string> huffmanCodes(string s, vector<int>& f, int n) {
    auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; };
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> pq(cmp);
    for (int x : f) pq.push(new HuffmanNode(x));

    while (pq.size() > 1) {
        auto* l = pq.top(); pq.pop();
        auto* r = pq.top(); pq.pop();
        pq.push(new HuffmanNode(l->freq + r->freq, l, r));
    }

    vector<string> ans;
    auto dfs = [&](auto& self, HuffmanNode* node, string code) -> void {
        if (!node) return;
        if (!node->left && !node->right) {
            ans.push_back(code.empty() ? "0" : code);
            return;
        }
        self(self, node->left, code + "0");
        self(self, node->right, code + "1");
    };
    dfs(dfs, pq.top(), "");
    return ans;
}
// Interview Explanation:
// - Problem Statement: Generate optimal prefix Huffman codes for characters given their frequencies.
// - Approach: Min-Heap Optimal Binary Tree Construction.
// - Intuition:
//   * More frequent characters should receive shorter bit sequences.
//   * Maintain a min-heap of tree nodes.
//   * Greedily combine the two nodes with the lowest frequencies into a new parent node with combined frequency.
//   * Repeat until 1 root node remains.
//   * Preorder traversal outputs prefix codes ('0' for left edge, '1' for right edge).
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 66. FRACTIONAL KNAPSACK [G-66]
// =========================================================

struct KnapsackItem {
    int value, weight;
};

double fractionalKnapsack(int w, vector<KnapsackItem>& arr, int n) {
    sort(arr.begin(), arr.end(), [](const auto& a, const auto& b) {
        return 1LL * a.value * b.weight > 1LL * b.value * a.weight;
    });

    double totalVal = 0.0;
    for (const auto& item : arr) {
        if (w <= 0) break;
        if (w >= item.weight) {
            totalVal += item.value;
            w -= item.weight;
        } else {
            totalVal += (double)item.value * w / item.weight;
            break;
        }
    }
    return totalVal;
}
// Interview Explanation:
// - Problem Statement: Maximize total value in knapsack of capacity w allowing fractional item takes.
// - Approach: Greedy Value-to-Weight Ratio Descending Sort.
// - Intuition:
//   * Items with higher value density (value / weight) give the most profit per unit capacity.
//   * Sort items descending by value-to-weight ratio.
//   * Greedily take whole items as long as remaining capacity permits.
//   * For the final fitting item, take the exact fraction required to fill the remaining capacity w.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


