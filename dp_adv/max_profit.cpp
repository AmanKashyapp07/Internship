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
#include <climits>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

ll maxProfit(int i,
             vector<pair<pair<int, int>, int>>& jobs,
             vector<ll>& dp) {
    if (i >= (int)jobs.size()) return 0;

    if (dp[i] != -1) return dp[i];

    int nextIndex = lower_bound(
                        jobs.begin() + i + 1,
                        jobs.end(),
                        make_pair(make_pair(jobs[i].first.second, 0), 0))
                    - jobs.begin();

    ll includeProfit = jobs[i].second + maxProfit(nextIndex, jobs, dp);
    ll excludeProfit = maxProfit(i + 1, jobs, dp);

    return dp[i] = max(includeProfit, excludeProfit);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<pair<int, int>, int>> jobs;

    for (int i = 0; i < n; i++) {
        int s, e, p;
        cin >> s >> e >> p;
        jobs.push_back({{s, e}, p});
    }

    sort(jobs.begin(), jobs.end());

    vector<ll> dp(n, -1);

    cout << maxProfit(0, jobs, dp) << '\n';

    return 0;
}