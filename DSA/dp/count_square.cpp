/**
 * LeetCode 1277 - Count Square Submatrices with All Ones
 *
 * Given an m x n binary matrix `mat` filled with 0s and 1s,
 * return the number of square submatrices that have all ones.
 *
 * Approach: 2D Dynamic Programming
 * dp[i][j] = largest square side length with bottom-right at (i, j)
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */

#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int countSquares(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        
        int m = matrix.size();
        int n = matrix[0].size();
        
        vector<vector<int>> dp(m, vector<int>(n, 0));
        int total = 0;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 1) {
                    if (i == 0 || j == 0) {
                        dp[i][j] = 1;
                    } else {
                        dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
                    }
                    total += dp[i][j];
                }
            }
        }
        
        return total;
    }
};