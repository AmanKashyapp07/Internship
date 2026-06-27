/**
 * LeetCode 233 - Number of Digit One (Class Variant)
 *
 * Description:
 * Given an integer n, count the total number of digit 1 appearing in all non-negative integers less than or equal to n.
 *
 * Approach:
 * - Digit Dynamic Programming with Memoization.
 * - Uses a Solution class wrapping the DFS state `(pos, cnt, tight)`.
 * - Accumulates the count of '1's placed so far in `cnt` and returns `cnt` when all digits are processed.
 *
 * Time Complexity: O(log10(n) * 10 * log10(n))
 * Space Complexity: O(log10(n) * log10(n))
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
#include <cstring>

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
    string s;
    int n;

    int dp[12][12][2]; // dp[pos][cnt][tight]
    int solve(int pos, int cnt, int tight) {
        if (pos == n)
            return cnt;

        if (dp[pos][cnt][tight] != -1)
            return dp[pos][cnt][tight];

        int limit = tight ? (s[pos] - '0') : 9;
        int ans = 0;

        for (int d = 0; d <= limit; d++) {
            ans += solve(
                pos + 1,
                cnt + (d == 1),
                tight && (d == limit)
            );
        }

        return dp[pos][cnt][tight] = ans;
    }

    int countDigitOne(int num) {
        s = to_string(num);
        n = s.size();

       
        memset(dp, -1, sizeof(dp));


        return solve(0, 0, 1);
    }
};