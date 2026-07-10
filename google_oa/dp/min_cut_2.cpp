/**
 * Palindrome Partitioning II (LeetCode 132)
 * Minimum cuts needed to partition string into palindromic substrings.
 * Time: O(n^2), Space: O(n^2)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        if (n <= 1) return 0;

        // Precompute all palindromic substrings
        vector<vector<bool>> isPal(n, vector<bool>(n, false));

        for (int i = 0; i < n; ++i) {
            isPal[i][i] = true;
        }
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if(s[i]==s[j]){
                    if(len==2) isPal[i][j] = true;
                    else isPal[i][j] = isPal[i + 1][j - 1];
                }
                else{
                    isPal[i][j] = false;
                }
            }
        }

        // dp[i] = min cuts for prefix s[0..i]
        vector<int> dp(n, INT_MAX);

        for (int i = 0; i < n; ++i) {
            if (isPal[0][i]) {
                dp[i] = 0;
                continue;
            }
            for (int j = 1; j <= i; ++j) {
                if (isPal[j][i]) {
                    dp[i] = min(dp[i], 1 + dp[j - 1]);
                }
            }
        }

        return dp[n - 1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    Solution solver;
    cout << solver.minCut(s) << '\n';

    return 0;
}