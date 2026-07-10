/**
 * LeetCode 115 - Distinct Subsequences (Tabulation)
 *
 * Description:
 * Given two strings s and t, return the number of distinct subsequences of s which equals t.
 *
 * Approach:
 * - Dynamic Programming with Tabulation (Bottom-Up).
 * - dp[i][j] = number of distinct subsequences of s[0...i-1] that match t[0...j-1]
 * - Base cases:
 *   - dp[0][0] = 1 (empty string has one way to form empty subsequence)
 *   - dp[i][0] = 1 for all i (empty pattern can always be formed)
 *   - dp[0][j] = 0 for j > 0 (non-empty pattern cannot be formed from empty string)
 * - Transition:
 *   - dp[i][j] = dp[i-1][j]  (skip s[i-1])
 *   - If s[i-1] == t[j-1], add dp[i-1][j-1]
 *
 * Time Complexity: O(m * n) where m = s.length(), n = t.length().
 * Space Complexity: O(m * n)
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution {
public:
    int numDistinct(string s, string t) {
        int m = s.size();
        int n = t.size();
        
        if (n == 0) return 1;
        if (m == 0) return 0;
        
        // dp[i][j] = number of ways to form t[0..j-1] using s[0..i-1]
        vector<vector<long long>> dp(m + 1, vector<long long>(n + 1, 0));
        
        // Base cases
        for (int i = 0; i <= m; ++i) {
            dp[i][0] = 1;  // Empty pattern can always be formed
        }
        // dp[0][j] remains 0 for j > 0 (already initialized)
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                // Skip current character of s
                dp[i][j] = dp[i - 1][j];
                
                // If characters match, add the ways where we use this character
                if (s[i - 1] == t[j - 1]) {
                    dp[i][j] += dp[i - 1][j - 1];
                }
            }
        }
        
        return dp[m][n];
    }
};