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

class SolutionRemoveInvalidParentheses {
    unordered_set<string> validStrings;

    void dfs(const string& s, int idx, int leftRem, int rightRem, int open, string cur) {
        if (idx == (int)s.size()) {
            if (leftRem == 0 && rightRem == 0 && open == 0) {
                validStrings.insert(cur);
            }
            return;
        }

        char c = s[idx];
        if (c == '(') {
            // Discard '('
            if (leftRem > 0) dfs(s, idx + 1, leftRem - 1, rightRem, open, cur);
            // Keep '('
            dfs(s, idx + 1, leftRem, rightRem, open + 1, cur + c);
        } else if (c == ')') {
            // Discard ')'
            if (rightRem > 0) dfs(s, idx + 1, leftRem, rightRem - 1, open, cur);
            // Keep ')' if valid open bracket available
            if (open > 0) dfs(s, idx + 1, leftRem, rightRem, open - 1, cur + c);
        } else {
            dfs(s, idx + 1, leftRem, rightRem, open, cur + c);
        }
    }

public:
    vector<string> removeInvalidParentheses(string s) {
        int leftRem = 0, rightRem = 0;
        for (char c : s) {
            if (c == '(') leftRem++;
            else if (c == ')') {
                if (leftRem > 0) leftRem--;
                else rightRem++;
            }
        }

        validStrings.clear();
        dfs(s, 0, leftRem, rightRem, 0, "");
        return vector<string>(validStrings.begin(), validStrings.end());
    }
};
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
    int last[10] = {0};
    int n = s.size();

    for (int i = 0; i < n; ++i) {
        last[s[i] - '0'] = i;
    }

    for (int i = 0; i < n; ++i) {
        int d = s[i] - '0';
        for (int larger = 9; larger > d; --larger) {
            if (last[larger] > i) {
                swap(s[i], s[last[larger]]);
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

bool compareLargestNum(const string& a, const string& b) {
    return (a + b) > (b + a);
}

string largestNumber(vector<int>& a) {
    vector<string> s;
    for (int x : a) s.push_back(to_string(x));
    sort(s.begin(), s.end(), compareLargestNum);

    if (s[0] == "0") return "0";

    string ans = "";
    for (const string& str : s) ans += str;
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
    long long totalCost = 0;

    while (pq.size() > 1) {
        long long first = pq.top(); pq.pop();
        long long second = pq.top(); pq.pop();
        long long combined = first + second;
        totalCost += combined;
        pq.push(combined);
    }
    return totalCost;
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
    priority_queue<int, vector<int>, greater<int>> pq; // min-heap of size k
    for (int x : a) {
        pq.push(x);
        if ((int)pq.size() > k) {
            pq.pop();
        }
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
    int total = accumulate(stones.begin(), stones.end(), 0);
    int target = total / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int s : stones) {
        for (int i = target; i >= s; --i) {
            dp[i] = dp[i] || dp[i - s];
        }
    }

    for (int i = target; i >= 0; --i) {
        if (dp[i]) {
            return total - 2 * i;
        }
    }
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
    priority_queue<int, vector<int>, greater<int>> pq; // min-heap of ladder climbs

    for (int i = 0; i < (int)h.size() - 1; ++i) {
        int diff = h[i + 1] - h[i];
        if (diff > 0) {
            pq.push(diff);
            if ((int)pq.size() > ladders) {
                bricks -= pq.top();
                pq.pop();
            }
            if (bricks < 0) {
                return i;
            }
        }
    }
    return (int)h.size() - 1;
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
    priority_queue<int> pq; // max-heap of passed fuel capacities
    long long curReach = startFuel;
    int stops = 0, i = 0, n = stations.size();

    while (curReach < target) {
        while (i < n && stations[i][0] <= curReach) {
            pq.push(stations[i][1]);
            i++;
        }

        if (pq.empty()) return -1;

        curReach += pq.top();
        pq.pop();
        stops++;
    }
    return stops;
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

struct Cell3D {
    int h, r, c;
};

struct Cell3DCompare {
    bool operator()(const Cell3D& a, const Cell3D& b) const {
        return a.h > b.h;
    }
};

int trapRainWater(vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size();
    if (m <= 2 || n <= 2) return 0;

    priority_queue<Cell3D, vector<Cell3D>, Cell3DCompare> pq;
    vector<vector<bool>> vis(m, vector<bool>(n, false));

    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (r == 0 || r == m - 1 || c == 0 || c == n - 1) {
                pq.push({g[r][c], r, c});
                vis[r][c] = true;
            }
        }
    }

    int trapped = 0;
    int maxBoundary = 0;
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!pq.empty()) {
        auto [h, r, c] = pq.top();
        pq.pop();

        maxBoundary = max(maxBoundary, h);

        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && !vis[nr][nc]) {
                vis[nr][nc] = true;
                if (g[nr][nc] < maxBoundary) {
                    trapped += maxBoundary - g[nr][nc];
                }
                pq.push({g[nr][nc], nr, nc});
            }
        }
    }
    return trapped;
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

struct ClassGain {
    int p, t;
    double gain() const {
        return (double)(p + 1) / (t + 1) - (double)p / t;
    }
};

struct ClassGainCompare {
    bool operator()(const ClassGain& a, const ClassGain& b) const {
        return a.gain() < b.gain(); // max-heap by marginal gain
    }
};

double maxAverageRatio(vector<vector<int>>& classes, int extraStudents) {
    priority_queue<ClassGain, vector<ClassGain>, ClassGainCompare> pq;
    for (const auto& c : classes) {
        pq.push({c[0], c[1]});
    }

    while (extraStudents-- > 0) {
        auto top = pq.top();
        pq.pop();
        top.p++;
        top.t++;
        pq.push(top);
    }

    double sumRatio = 0.0;
    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        sumRatio += (double)top.p / top.t;
    }
    return sumRatio / classes.size();
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
    int n = costs.size();
    priority_queue<int, vector<int>, greater<int>> leftPq, rightPq;

    int l = 0, r = n - 1;

    for (int i = 0; i < candidates && l <= r; ++i) {
        leftPq.push(costs[l++]);
    }
    for (int i = 0; i < candidates && l <= r; ++i) {
        rightPq.push(costs[r--]);
    }

    long long ans = 0;

    while (k-- > 0) {
        int leftVal = leftPq.empty() ? INT_MAX : leftPq.top();
        int rightVal = rightPq.empty() ? INT_MAX : rightPq.top();

        if (leftVal <= rightVal) {
            ans += leftVal;
            leftPq.pop();
            if (l <= r) {
                leftPq.push(costs[l++]);
            }
        } else {
            ans += rightVal;
            rightPq.pop();
            if (l <= r) {
                rightPq.push(costs[r--]);
            }
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

    map<int, int> mp;
    for (int x : hand) mp[x]++;

    while (!mp.empty()) {
        int start = mp.begin()->first;
        for (int i = 0; i < groupSize; ++i) {
            int card = start + i;
            if (!mp.count(card)) return false;
            if (--mp[card] == 0) {
                mp.erase(card);
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

class SolutionValidArrangement {
    unordered_map<int, vector<int>> adj;
    vector<int> path;

    void dfs(int u) {
        auto& edges = adj[u];
        while (!edges.empty()) {
            int v = edges.back();
            edges.pop_back();
            dfs(v);
        }
        path.push_back(u);
    }

public:
    vector<vector<int>> validArrangement(vector<vector<int>>& pairs) {
        unordered_map<int, int> inDeg, outDeg;
        for (const auto& p : pairs) {
            adj[p[0]].push_back(p[1]);
            outDeg[p[0]]++;
            inDeg[p[1]]++;
        }

        int startNode = pairs[0][0];
        for (const auto& [node, out] : outDeg) {
            if (out - inDeg[node] == 1) {
                startNode = node;
                break;
            }
        }

        path.clear();
        dfs(startNode);
        reverse(path.begin(), path.end());

        vector<vector<int>> ans;
        for (int i = 0; i < (int)path.size() - 1; ++i) {
            ans.push_back({path[i], path[i + 1]});
        }
        return ans;
    }
};
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
            miss += nums[i];
            i++;
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
        if (a[i] > a[i - 1]) {
            up = down + 1;
        } else if (a[i] < a[i - 1]) {
            down = up + 1;
        }
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
        if (x <= first) {
            first = x;
        } else if (x <= second) {
            second = x;
        } else {
            return true; // x > second > first
        }
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

bool comparePairChainEnd(const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1];
}

int findLongestChain(vector<vector<int>>& pairs) {
    sort(pairs.begin(), pairs.end(), comparePairChainEnd);

    int ans = 0;
    int curEnd = -1e9;

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

bool compareEnvelopes(const vector<int>& a, const vector<int>& b) {
    if (a[0] != b[0]) return a[0] < b[0]; // width ascending
    return a[1] > b[1];                   // height descending
}

int maxEnvelopes(vector<vector<int>>& env) {
    sort(env.begin(), env.end(), compareEnvelopes);

    vector<int> tails;
    for (const auto& e : env) {
        int h = e[1];
        auto it = lower_bound(tails.begin(), tails.end(), h);
        if (it == tails.end()) {
            tails.push_back(h);
        } else {
            *it = h;
        }
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

class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();
        int left = -1, right = -1;
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
};
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
    char data;
    int freq;
    HuffmanNode *left, *right;
    HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

struct HuffmanNodeCompare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) const {
        return a->freq > b->freq; // min-heap
    }
};

void buildHuffmanCodes(HuffmanNode* root, string code, vector<string>& ans) {
    if (!root) return;
    if (!root->left && !root->right) {
        ans.push_back(code);
        return;
    }
    buildHuffmanCodes(root->left, code + "0", ans);
    buildHuffmanCodes(root->right, code + "1", ans);
}

vector<string> huffmanCodes(string s, vector<int>& f, int n) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, HuffmanNodeCompare> pq;
    for (int i = 0; i < n; ++i) {
        pq.push(new HuffmanNode(s[i], f[i]));
    }

    while (pq.size() > 1) {
        HuffmanNode* l = pq.top(); pq.pop();
        HuffmanNode* r = pq.top(); pq.pop();

        HuffmanNode* parent = new HuffmanNode('$', l->freq + r->freq);
        parent->left = l;
        parent->right = r;
        pq.push(parent);
    }

    vector<string> ans;
    buildHuffmanCodes(pq.top(), "", ans);
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

bool compareKnapsackRatio(const KnapsackItem& a, const KnapsackItem& b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

double fractionalKnapsack(int w, vector<KnapsackItem>& arr, int n) {
    sort(arr.begin(), arr.end(), compareKnapsackRatio);

    double totalVal = 0.0;
    for (int i = 0; i < n && w > 0; ++i) {
        if (w >= arr[i].weight) {
            totalVal += arr[i].value;
            w -= arr[i].weight;
        } else {
            totalVal += (double)arr[i].value * w / arr[i].weight;
            w = 0;
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

