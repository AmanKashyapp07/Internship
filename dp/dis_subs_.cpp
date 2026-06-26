/**
 * LeetCode 115 - Distinct Subsequences (Memoized)
 *
 * Description:
 * Given two strings s and t, return the number of distinct subsequences of s which equals t.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - Let `solve(i, j)` return the number of distinct subsequences of `s[0...i]` matching `t[0...j]`.
 * - Transition:
 *   - Always try to skip `s[i]`: `solve(i - 1, j)`.
 *   - If `s[i] == t[j]`, we can also match them: `solve(i - 1, j - 1)`.
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
    int solve(int i, int j, string &s, string &t, vector<vector<long long>> &dp) {
        if (j < 0) return 1; // t is empty, one way to form it
        if (i < 0) return 0; // s is empty but t is not, no way to form it

        if (dp[i][j] != -1) return dp[i][j];

        long long ans = solve(i - 1, j, s, t, dp); // skip s[i]

        if (s[i] == t[j]) {
            ans += solve(i - 1, j - 1, s, t, dp); // match s[i] with t[j]
        }

        return dp[i][j] = ans;
    }
    int numDistinct(string s, string t) {
        int m = s.length();
        int n = t.length();
        vector<vector<long long>> dp(m + 1, vector<long long>(n + 1, -1));
        return solve(m - 1, n - 1, s, t, dp);
    }
};