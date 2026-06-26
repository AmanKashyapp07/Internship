/**
 * LeetCode 1277 - Count Square Submatrices with All Ones
 *
 * Description:
 * Given an m x n matrix of ones and zeros, return how many square submatrices have all ones.
 *
 * Approach:
 * - 2D dynamic programming.
 * - Let `dp[i][j]` represent the side length of the largest square submatrix of all ones with its bottom-right corner at `(i, j)`.
 * - Transition: `dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1` if `matrix[i][j] == 1`.
 * - The sum of all elements in the DP table gives the total number of square submatrices of all ones.
 *
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int countSquares(int m, int n, vector<vector<int>>& matrix) {
    vector<vector<int>> dp(m, vector<int>(n, 0));
    int count = 0;
    // DP state: dp[i][j] is the size of the largest square submatrix with bottom-right corner at (i, j)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                if (i == 0 || j == 0) dp[i][j] = 1;
                else dp[i][j] = min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]}) + 1; // Transition
                count += dp[i][j];
            }
        }
    }
    return count;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int m, n; cin >> m >> n;
    vector<vector<int>> matrix(m, vector<int>(n));
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) cin >> matrix[i][j];
    cout << countSquares(m, n, matrix) << '\n';
}