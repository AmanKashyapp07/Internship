/**
 * LeetCode 97 - Interleaving String (Turn-Based DP)
 *
 * Description:
 * Given strings s1, s2, and s3, find whether s3 is formed by an interleaving of s1 and s2.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - State `(turn, i, j)` represents whether `s3[i+j...]` can be formed by interleaving suffixes `s1[i...]` and `s2[j...]` given that the last choice was from `s1` (turn = 0) or `s2` (turn = 1).
 * - Transition matches the current character in s3 with `s1[i]` or `s2[j]` and branches accordingly.
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
    vector<vector<vector<int>>> dp_;

    bool solve(int turn, int i, int j, int k) {
        if (k == s3_.size()) return true; // if we have matched all characters of s3
        if (i == s1_.size() && j == s2_.size()) return false; // if we have exhausted both s1 and s2 but not matched all of s3

        if (dp_[turn][i][j] != -1) return dp_[turn][i][j];

        bool ans = false;
        if (turn == 0) { // turn of s1
            if (i < s1_.size() && s1_[i] == s3_[k]) {
                ans = ans || solve(0, i + 1, j, k + 1);
                // also switch to s2
                ans = ans || solve(1, i + 1, j, k + 1);
            }
        } else { // turn of s2
            if (j < s2_.size() && s2_[j] == s3_[k]) {
                ans = ans || solve(1, i, j + 1, k + 1);
                ans = ans || solve(0, i, j + 1, k + 1);
            }
        }

        return dp_[turn][i][j] = ans;
    }

    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;

        s1_ = s1;
        s2_ = s2;
        s3_ = s3;

        int m = s1.size();
        int n = s2.size();

        dp_.assign(2, vector<vector<int>>(m + 1, vector<int>(n + 1, -1)));

        // We can start with either s1's turn or s2's turn
        return solve(0, 0, 0, 0) || solve(1, 0, 0, 0);
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}