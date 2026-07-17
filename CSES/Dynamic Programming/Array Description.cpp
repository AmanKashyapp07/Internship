// Link: https://cses.fi/problemset/task/1746

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

int n, m;
vector<int> a;
vector<vector<int>> dp;
int solve(int i, int prev)
{
    if (i == n)
        return 1;

    if (dp[i][prev + 1] != -1)
        return dp[i][prev + 1];

    long long ans = 0;

    if (a[i] != 0)
    {
        if (prev == -1 || abs(a[i] - prev) <= 1)
            ans = solve(i + 1, a[i]);
    }
    else
    {
        if (prev == -1)
        {
            for (int cur = 1; cur <= m; cur++)
            {
                ans = (ans + solve(i + 1, cur)) % MOD; // trying each possible value for the current position and recursively solving for the next position.
            }
        }
        else
        {
            for (int cur = max(1, prev - 1); cur <= min(m, prev + 1); cur++)
            {
                ans = (ans + solve(i + 1, cur)) % MOD;
            }
        }
    }

    return dp[i][prev + 1] = ans;
}

int main()
{

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    a.resize(n);
    for (int &x : a)
        cin >> x;

    dp.assign(n, vector<int>(m + 2, -1));

    long long ans1 = solve(0, -1);
    cout << ans1 << "\n";
}