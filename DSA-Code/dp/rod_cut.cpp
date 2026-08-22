/**
 * Problem: Rod Cutting
 *
 * Given a rod of length n and an array price[0..n-1] where price[i]
 * represents the price of a piece of length (i + 1),
 * find the maximum value that can be obtained by cutting the rod
 * into pieces and selling them.
 *
 * This is a classic unbounded knapsack variant.
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int rodCutting(vector<int>& price, int n) {
        if (n == 0) return 0;
        
        // dp[i][j] = max value using first (i+1) piece lengths for rod of length j
        vector<vector<int>> dp(n, vector<int>(n + 1, 0));
        
        // Base case: only length 1 pieces available
        for (int len = 0; len <= n; len++) {
            dp[0][len] = len * price[0];
        }
        
        for (int i = 1; i < n; i++) {
            int pieceLen = i + 1;
            for (int len = 0; len <= n; len++) {
                int notTake = dp[i - 1][len];
                int take = 0;
                if (pieceLen <= len) {
                    take = price[i] + dp[i][len - pieceLen];
                }
                dp[i][len] = max(take, notTake);
            }
        }
        
        return dp[n - 1][n];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> price(n);
    for (int i = 0; i < n; i++) {
        cin >> price[i];
    }
    
    Solution solver;
    cout << solver.rodCutting(price, n) << '\n';
    
    return 0;
}