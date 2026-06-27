/**
 * LeetCode 97 - Interleaving String
 *
 * Description:
 * Given strings s1, s2, and s3, find whether s3 is formed by an interleaving of s1 and s2.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - Let `solve(i, j)` return whether `s3[i+j...]` is an interleaving of `s1[i...]` and `s2[j...]`.
 * - Transition:
 *   - If `s1[i] == s3[i+j]`, try to match from `s1`: `solve(i+1, j)`.
 *   - If `s2[j] == s3[i+j]`, try to match from `s2`: `solve(i, j+1)`.
 *
 * Time Complexity: O(len(s1) * len(s2))
 * Space Complexity: O(len(s1) * len(s2))
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

class Solution {
public:
    string str1, str2, str3;
    vector<vector<int>> memo;

    bool solve(int i, int j) {
        if (i == str1.size() && j == str2.size())
            return true;

        if (memo[i][j] != -1)
            return memo[i][j];

        bool ans = false;

        if (i < str1.size() && str1[i] == str3[i + j])
            ans |= solve(i + 1, j);

        if (j < str2.size() && str2[j] == str3[i + j])
            ans |= solve(i, j + 1);

        return memo[i][j] = ans;
    }

    bool isInterleave(string str1_param, string str2_param, string str3_param) {
        str1 = str1_param;
        str2 = str2_param;
        str3 = str3_param;

        if (str1.size() + str2.size() != str3.size())
            return false;

        memo.assign(str1.size() + 1, vector<int>(str2.size() + 1, -1));

        return solve(0, 0);
    }
};