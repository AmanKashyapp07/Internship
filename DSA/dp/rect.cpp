/**
 * CSES 1744 - Rectangle Cutting
 *
 * Description:
 * Given an a x b rectangle, find the minimum number of cuts to partition it into squares.
 *
 * Approach:
 * - 2D dynamic programming.
 * - Let `dp[i][j]` be the minimum cuts for an `i x j` rectangle.
 * - If `i == j`, `dp[i][j] = 0`.
 * - Otherwise, try all horizontal cuts: `min(dp[i][j], 1 + dp[k][j] + dp[i-k][j])` for `1 <= k < i`.
 * - Try all vertical cuts: `min(dp[i][j], 1 + dp[i][k] + dp[i][j-k])` for `1 <= k < j`.
 *
 * Time Complexity: O(a * b * (a + b))
 * Space Complexity: O(a * b)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int a, b; if (!(cin >> a >> b)) return 0;
    vector<vector<int>> dp(a + 1, vector<int>(b + 1, 1e9));
    // DP state: dp[i][j] stores the min cuts needed for an i x j rectangle
    for (int i = 1; i <= a; i++) {
        for (int j = 1; j <= b; j++) {
            if (i == j) dp[i][j] = 0; // Base case: already a square
            else {
                for (int k = 1; k < i; k++) dp[i][j] = min(dp[i][j], dp[k][j] + dp[i - k][j] + 1); // Horizontal cuts
                for (int k = 1; k < j; k++) dp[i][j] = min(dp[i][j], dp[i][k] + dp[i][j - k] + 1); // Vertical cuts
            }
        }
    }
    cout << dp[a][b] << "\n";
}