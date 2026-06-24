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

int maxCoins(int i, int j, vector<int>& balloon, vector<vector<int>>& dp) {
    if (i > j) return 0; // we will also allow single balloon, then cost will be balloon[i-1]*balloon[i]*balloon[i+1] if i>0 and i<n-1, else if i==0 then cost will be balloon[i]*balloon[i+1], else if i==n-1 then cost will be balloon[i-1]*balloon[i]
    if (dp[i][j] != -1) return dp[i][j];

    int ans = LLONG_MIN;
    for (int k = i; k <= j; k++) {
        int coins = balloon[k];
        if (i > 0) coins *= balloon[i - 1];
        if (j < balloon.size() - 1) coins *= balloon[j + 1];

        ans = max(ans, coins + maxCoins(i, k - 1, balloon, dp) + maxCoins(k + 1, j, balloon, dp));
    }

    return dp[i][j] = ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int> balloon(n);
    for (int i = 0; i < n; i++) cin >> balloon[i];

    vector<vector<int>> dp(n, vector<int>(n, -1));
    cout << maxCoins(0, n - 1, balloon, dp) << endl;
    return 0;
}
