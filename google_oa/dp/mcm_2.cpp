/**
 * Matrix Chain Multiplication (Tabulation)
 * Find minimum cost to multiply chain of matrices.
 * Time: O(n^3), Space: O(n^2)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

class Solution {
public:
    ll matrixMultiplication(vector<int>& arr) {
        int n = arr.size();
        if (n <= 2) return 0;

        vector<vector<ll>> dp(n, vector<ll>(n, 0)); // dp[i][j] = minimum cost to multiply matrices from i to j

        for (int len = 2; len < n; ++len) {
            for (int i = 1; i+len<=n; ++i) { // we cannot start i from 0 because we need arr[i-1] for cost calculation
                int j = i + len - 1;
                dp[i][j] = LLONG_MAX;

                for (int k = i; k < j; ++k) {
                    ll cost = dp[i][k] + dp[k + 1][j] + 1LL * arr[i - 1] * arr[k] * arr[j];
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }
        return dp[1][n - 1];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> arr(n);
    for (int &x : arr) cin >> x;

    Solution solver;
    cout << solver.matrixMultiplication(arr) << '\n';

    return 0;
}