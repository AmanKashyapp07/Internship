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
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Solution {
public:
    vector<int> input;
    vector<vector<int>> memo;

    int solve(int i, int j) {
        if (j - i == 1) return 0; // no cuts to be made between i and j

        if (memo[i][j] != -1) return memo[i][j];

        int ans = INT_MAX;
        // cut can be only be between i+1 inclusive and j-1 inclusive, because we cannot cut at the boundaries
        for (int k = i + 1; k < j; k++) {
            ans = min(
                ans,
                solve(i, k) +
                solve(k, j) +
                (input[j] - input[i]) // cost of making the cut at position k
            );
        }

        if (ans == INT_MAX) ans = 0;
        return memo[i][j] = ans;
    }

    int minCostMemo(int n, vector<int>& cuts) {
        input = cuts;
        input.push_back(0);
        input.push_back(n);
        sort(input.begin(), input.end());

        int m = input.size();
        memo.assign(m, vector<int>(m, -1));
        return solve(0, m - 1);
    }

    int minCostTab(int n, vector<int>& cuts) {
        input = cuts;
        input.push_back(0);
        input.push_back(n);
        sort(input.begin(), input.end());

        int m = input.size();
        memo.assign(m, vector<int>(m, 0));

        for (int lengthVal = 2; lengthVal < m; lengthVal++) {
            for (int i = 0; i + lengthVal < m; i++) {
                int j = i + lengthVal;
                memo[i][j] = INT_MAX;

                for (int k = i + 1; k < j; k++) { // cut can only be between i+1 and j-1
                    memo[i][j] = min(
                        memo[i][j],
                        memo[i][k] + memo[k][j] + (input[j] - input[i])
                    );
                }

                if (memo[i][j] == INT_MAX) memo[i][j] = 0;
            }
        }
        return memo[0][m - 1];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    if (!(cin >> n >> c)) return 0;

    vector<int> cuts(c);
    for (int i = 0; i < c; i++) {
        cin >> cuts[i];
    }

    Solution solver;
    vector<int> cutsCopy = cuts;
    cout << solver.minCostTab(n, cuts) << '\n';
    cout << solver.minCostMemo(n, cutsCopy) << '\n';

    return 0;
}
