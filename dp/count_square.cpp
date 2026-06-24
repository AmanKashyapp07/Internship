/**
 * Problem: Count Square Submatrices with All Ones (https://leetcode.com/problems/count-square-submatrices-with-all-ones/)
 * Find the total number of square submatrices that have all ones.
 * Time: O(M * N) time, O(M * N) space.
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
