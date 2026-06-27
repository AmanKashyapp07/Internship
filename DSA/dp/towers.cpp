/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
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

int helper(int i, int j, vector<vector<int>>&dp){
    if(i>= dp.size() || j>=dp.size()) return 1;
    
    if(dp[i][j] != -1) return dp[i][j];
    int case1 = helper(i+1, j+1, dp); // extending both
    int case2 = helper(i+1, j, dp); // extending only i
    int case3 = helper(i, j+1, dp); // extending only j
    int case4 = 0;
    if(i==j) case4 = helper(i+1, j+1, dp); // extending both and adding 1 to the count
    return dp[i][j] = max({case1, case2, case3, case4});
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--)
    {
        // Write your code here
        int n;
        cin >> n;
       vector<vector<int>> dp(n, vector<int>(n, -1));


    }
    return 0;
}
