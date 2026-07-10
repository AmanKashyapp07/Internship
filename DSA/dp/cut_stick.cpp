/**
 * LeetCode 1547 - Minimum Cost to Cut a Stick
 *
 * Given a wooden stick of length `n` and an array `cuts` containing positions where cuts must be made.
 * The cost of a cut is the current length of the stick being cut. Return the minimum total cost
 * to make all cuts.
 *
 * Approach: Interval Dynamic Programming (Tabulation)
 * Time Complexity: O(m³) where m = cuts.size() + 2
 * Space Complexity: O(m²)
 */

#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int minCost(int n, vector<int>& cuts) {
        // Add boundaries
        cuts.push_back(0);
        cuts.push_back(n);
        sort(cuts.begin(), cuts.end());
        
        int m = cuts.size();
        vector<vector<int>> dp(m, vector<int>(m, 0));
        
        // length = difference between indices i and j
        for (int len = 2; len < m; len++) {
            for (int i = 0; i + len < m; i++) {
                int j = i + len;
                dp[i][j] = INT_MAX;
                
                for (int k = i + 1; k < j; k++) {
                    dp[i][j] = min(dp[i][j], 
                                   dp[i][k] + dp[k][j] + (cuts[j] - cuts[i]));
                }
                
                if (dp[i][j] == INT_MAX) dp[i][j] = 0;
            }
        }
        
        return dp[0][m - 1];
    }
};