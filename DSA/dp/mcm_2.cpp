/**
 * Problem: Matrix Chain Multiplication (Tabulation)
 *
 * Description:
 * Given a sequence of matrices, find the most efficient way to multiply these matrices together.
 *
 * Approach:
 * - Interval Dynamic Programming with Bottom-Up Tabulation.
 * - Fill the DP table `dp[i][j]` (minimum multiplications for matrix range `i` to `j`) by increasing subproblem lengths.
 * - Transition: Try all split points `k` between `i` and `j-1`.
 *   - `dp[i][j] = min(dp[i][k] + dp[k+1][j] + arr[i-1] * arr[k] * arr[j])`.
 *
 * Time Complexity: O(n^3)
 * Space Complexity: O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;
using ll = long long;

class Solution {
public:
    vector<int> input;
    vector<vector<ll>> memo;

    ll matrixMultiplication(vector<int>& arr) {
        input = arr;
        int n = arr.size();
        memo.assign(n, vector<ll>(n, 0));

        for (int lengthVal = 2; lengthVal <= n - 1; lengthVal++) {
            for (int i = 1; i + lengthVal - 1 < n; i++) {
                int j = i + lengthVal - 1;
                memo[i][j] = LLONG_MAX;

                for (int k = i; k < j; k++) { // k is between i and j-1 inclusive
                    memo[i][j] = min(
                        memo[i][j],
                        memo[i][k] + memo[k + 1][j] + 1LL * input[i - 1] * input[k] * input[j]
                    );
                }
            }
        }
        return memo[1][n - 1];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    Solution solver;
    cout << solver.matrixMultiplication(arr) << '\n';

    return 0;
}
