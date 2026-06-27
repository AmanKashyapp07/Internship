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
    vector<pair<pair<int, int>, int>> pairList;
    vector<ll> memo;

    ll solve(int i) {
        if (i >= (int)pairList.size()) return 0;
        if (memo[i] != -1) return memo[i];

        auto target = make_pair(make_pair(pairList[i].first.second, 0), 0);
        int nextIndex = lower_bound(pairList.begin() + i + 1, pairList.end(), target) - pairList.begin();

        ll includeProfit = pairList[i].second + solve(nextIndex);
        ll excludeProfit = solve(i + 1);

        return memo[i] = max(includeProfit, excludeProfit);
    }

    ll getMaxProfit(vector<pair<pair<int, int>, int>>& jobs) {
        pairList = jobs;
        sort(pairList.begin(), pairList.end());
        int n = pairList.size();
        memo.assign(n, -1);
        return solve(0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<pair<pair<int, int>, int>> jobs;
    for (int i = 0; i < n; i++) {
        int s, e, p;
        cin >> s >> e >> p;
        jobs.push_back({{s, e}, p});
    }

    Solution solver;
    cout << solver.getMaxProfit(jobs) << '\n';

    return 0;
}
