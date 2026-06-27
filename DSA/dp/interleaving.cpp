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
    bool solve(int turn, int i, int j, int k, string &s1, string &s2, string &s3, vector<vector<vector<int>>> &dp) {
        if (k == s3.size()) return true; // if we have matched all characters of s3
        if (i == s1.size() && j == s2.size()) return false; // if we have exhausted both s1 and s2 but not matched all of s3

        if (dp[turn][i][j] != -1) return dp[turn][i][j];

        bool ans = false;
        if (turn == 0) { // turn of s1
            if (i < s1.size() && s1[i] == s3[k]) {
                ans = ans || solve(0, i + 1, j, k + 1, s1, s2, s3, dp);
                //also switch to s2
                ans=ans || solve(1, i+1, j, k + 1, s1, s2, s3, dp);
            }
        } else { // turn of s2
            if (j < s2.size() && s2[j] == s3[k]) {
                ans = ans || solve(1, i, j + 1, k + 1, s1, s2, s3, dp);
                ans = ans || solve(0, i, j + 1, k + 1, s1, s2, s3, dp);
            }
        }

        return dp[turn][i][j] = ans;
    }

    bool isInterleave(string s1, string s2, string s3) {
        
    }
};


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}