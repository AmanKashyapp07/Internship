/**
 * LeetCode 1547 - Minimum Cost to Cut a Stick
 *
 * Description:
 * Given a wooden stick of length n and an array cuts where cuts[i] represents a position you must perform a cut at.
 * The cost of one cut is the length of the stick to be cut. Return the minimum total cost of the cuts.
 *
 * Approach:
 * - Interval Dynamic Programming (both Tabulation and Memoization).
 * - Add boundaries 0 and n to `cuts` array, then sort it.
 * - Let `dp[i][j]` represent the minimum cost to perform all cuts between `cuts[i]` and `cuts[j]`.
 * - Transition: Try making the first cut at position `k` (where `i < k < j`).
 *   - `dp[i][j] = min(dp[i][k] + dp[k][j] + (cuts[j] - cuts[i]))`.
 *
 * Time Complexity: O(m^3) where m = cuts.size()
 * Space Complexity: O(m^2)
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

#define all(x) (x).begin(), (x).end()

int memoized_solution(int i, int j, vector<int>& cuts, vector<vector<int>>& dp) {
    if (j-i == 1) return 0; // no cuts to be made between i and j

    if (dp[i][j] != -1) return dp[i][j];

    int ans = INT_MAX;

    for (int k = i + 1; k < j; k++) { // cut can be only be between i+1 inclusive and j-1 inclusive, because we cannot cut at the boundaries
        ans = min(
            ans,
            memoized_solution(i, k, cuts, dp) +
            memoized_solution(k, j, cuts, dp) +
            (cuts[j] - cuts[i]) // dp[i][k] represents the cost of cutting the left part, dp[k][j] represents the cost of cutting the right part, and (cuts[j] - cuts[i]) represents the cost of making the cut at position k
        );
    }

    if (ans == INT_MAX)
        ans = 0;

    return dp[i][j] = ans;
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> cuts(c);

    for (int i = 0; i < c; i++)
        cin >> cuts[i];

    cuts.push_back(0);
    cuts.push_back(n);

    sort(all(cuts));

    int m = cuts.size();

    vector<vector<int>> dp(m, vector<int>(m, 0));

    for (int len = 2; len < m; len++) 
    {
        for (int i = 0; i + len < m; i++)
        {
            int j = i + len;

            dp[i][j] = INT_MAX;

            for (int k = i + 1; k < j; k++) // cut can be only be between i+1 inclusive and j-1 inclusive, because we cannot cut at the boundaries
            {
                dp[i][j] = min(
                    dp[i][j],
                    dp[i][k] +
                    dp[k][j] +
                    (cuts[j] - cuts[i]) // dp[i][k] represents the cost of cutting the left part, dp[k][j] represents the cost of cutting the right part, and (cuts[j] - cuts[i]) represents the cost of making the cut at position k
                );
            }

            if (dp[i][j] == INT_MAX)
                dp[i][j] = 0;
        }
    }

    cout << dp[0][m - 1] << '\n';
    cout<< memoized_solution(0, m - 1, cuts, dp) << '\n';

    return 0;
}