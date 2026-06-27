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

class Solution {
public:
    int a_;
    int b_;
    vector<vector<int>> dp_;

    int minCuts(int a, int b) {
        a_ = a;
        b_ = b;
        dp_.assign(a_ + 1, vector<int>(b_ + 1, 1e9));

        // DP state: dp[i][j] stores the min cuts needed for an i x j rectangle
        for (int i = 1; i <= a_; i++) {
            for (int j = 1; j <= b_; j++) {
                if (i == j) dp_[i][j] = 0; // Base case: already a square
                else {
                    for (int k = 1; k < i; k++) dp_[i][j] = min(dp_[i][j], dp_[k][j] + dp_[i - k][j] + 1); // Horizontal cuts
                    for (int k = 1; k < j; k++) dp_[i][j] = min(dp_[i][j], dp_[i][k] + dp_[i][j - k] + 1); // Vertical cuts
                }
            }
        }
        return dp_[a_][b_];
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int a, b;
    if (!(cin >> a >> b)) return 0;
    
    Solution solver;
    cout << solver.minCuts(a, b) << "\n";
    return 0;
}