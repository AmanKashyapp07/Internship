/**
 * LeetCode 879 - Profitable Schemes
 *
 * Description:
 * There is a group of n members, and a list of various crimes they could commit.
 * The i-th crime generates a profit[i] and requires group[i] members to participate.
 * Find the number of profitable schemes that can be chosen such that the total profit
 * is at least minProfit, and the total members used is at most n.
 *
 * Approach:
 * - Dynamic programming / memoization.
 * - Let `helper(idx, members, currProfit)` compute the number of schemes starting from `idx` with `members` remaining and `currProfit` accumulated towards `minProfit`.
 * - Transition:
 *   - Skip the current crime: `helper(idx + 1, members, currProfit)`
 *   - Commit the current crime (if enough members remain): `helper(idx + 1, members - group[idx], min(currProfit + profit[idx], minProfit))`
 *
 * Time Complexity: O(groupSize * n * minProfit)
 * Space Complexity: O(groupSize * n * minProfit)
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution
{
public:
    vector<vector<vector<int>>> memo;
    vector<int> groupMembers;
    vector<int> profitValues;
    int minProfitRequired;

    int helper(int idx, int members, int currProfit)
    {
        if (idx == groupMembers.size())
            return currProfit >= minProfitRequired;

        if (memo[idx][members][currProfit] != -1)
            return memo[idx][members][currProfit];

        long long ans = helper(idx + 1, members, currProfit);

        if (groupMembers[idx] <= members)
        {
            ans += helper(
                idx + 1,
                members - groupMembers[idx],
                min(currProfit + profitValues[idx], minProfitRequired));
        }

        return memo[idx][members][currProfit] = ans % MOD;
    }

    int profitableSchemes(int n, int minProfit, vector<int> &group, vector<int> &profit)
    {
        groupMembers = group;
        profitValues = profit;
        minProfitRequired = minProfit;
        memo.assign(group.size(), vector<vector<int>>(n + 1, vector<int>(minProfit + 1, -1)));
        return helper(0, n, 0);
    }
};