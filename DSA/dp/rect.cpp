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
    int size;
    int cols;
    vector<vector<int>> memo;

    int minCuts(int a, int b) {
        size = a;
        cols = b;
        memo.assign(size + 1, vector<int>(cols + 1, 1e9));

        // DP state: memo[i][j] stores the min cuts needed for an i x j rectangle
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= cols; j++) {
                if (i == j) memo[i][j] = 0; // Base case: already a square
                else {
                    for (int k = 1; k < i; k++) memo[i][j] = min(memo[i][j], memo[k][j] + memo[i - k][j] + 1); // Horizontal cuts
                    for (int k = 1; k < j; k++) memo[i][j] = min(memo[i][j], memo[i][k] + memo[i][j - k] + 1); // Vertical cuts
                }
            }
        }
        return memo[size][cols];
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