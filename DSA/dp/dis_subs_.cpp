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

    string s_, t_;
    int m_, n_;
    vector<vector<long long>> dp_;

    int solve(int i, int j) {

        if (j < 0)
            return 1;

        if (i < 0)
            return 0;

        if (dp_[i][j] != -1)
            return dp_[i][j];

        long long ans = solve(i - 1, j);

        if (s_[i] == t_[j])
            ans += solve(i - 1, j - 1);

        return dp_[i][j] = ans;
    }

    int numDistinct(string str1, string str2) {

        s_ = str1;
        t_ = str2;

        m_ = s_.size();
        n_ = t_.size();

        dp_.assign(m_, vector<long long>(n_, -1));

        return solve(m_ - 1, n_ - 1);
    }
};