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

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution {
public:
    string s1_, s2_, s3_;
    vector<vector<int>> dp_;

    bool solve(int i, int j) {
        if (i == s1_.size() && j == s2_.size())
            return true;

        if (dp_[i][j] != -1)
            return dp_[i][j];

        bool ans = false;

        if (i < s1_.size() && s1_[i] == s3_[i + j])
            ans |= solve(i + 1, j);

        if (j < s2_.size() && s2_[j] == s3_[i + j])
            ans |= solve(i, j + 1);

        return dp_[i][j] = ans;
    }

    bool isInterleave(string str1, string str2, string str3) {
        s1_ = str1;
        s2_ = str2;
        s3_ = str3;

        if (s1_.size() + s2_.size() != s3_.size())
            return false;

        dp_.assign(s1_.size() + 1, vector<int>(s2_.size() + 1, -1));

        return solve(0, 0);
    }
};