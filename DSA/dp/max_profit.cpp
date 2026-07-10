/**
 * CSES 1140 / LeetCode 1235 - Projects / Job Scheduling
 *
 * Description:
 * There are n projects. Each project has a start day, end day, and a reward.
 * Choose projects such that no two projects overlap and the total reward is maximized.
 *
 * Approach:
 * - Dynamic Programming with Memoization and Binary Search.
 * - Sort projects by start day.
 * - Let `maxProfit(i)` be the maximum reward from projects in suffix `[i...]`.
 * - Transition:
 *   - Option 1 (exclude): `maxProfit(i + 1)`
 *   - Option 2 (include): `reward[i] + maxProfit(nextIndex)` where `nextIndex` is the first project starting after project `i` ends (found using `lower_bound`).
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

class Solution {
public:
    long long getMaxProfit(vector<pair<pair<int, int>, int>>& jobs) {
        sort(jobs.begin(), jobs.end());
        int n = jobs.size();
        
        vector<int> start(n), end(n), reward(n);
        for (int i = 0; i < n; i++) {
            start[i] = jobs[i].first.first;
            end[i] = jobs[i].first.second;
            reward[i] = jobs[i].second;
        }
        
        vector<long long> dp(n + 1, 0);
        
        for (int i = n - 1; i >= 0; i--) {
            int next = lower_bound(start.begin(), start.end(), end[i]) - start.begin();
            dp[i] = max(dp[i + 1], reward[i] + dp[next]);
        }
        
        return dp[0];
    }
};

