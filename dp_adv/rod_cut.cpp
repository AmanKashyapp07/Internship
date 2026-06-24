/**
 * CSES Problem Set
 *
 * Problem: Rod Cutting
 *
 * Given a rod of length N and price[i] representing
 * the price of a rod piece of length (i + 1),
 * find the maximum obtainable value.
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

/* ---------------- Memoization ---------------- */

int solveMemo(int ind, int len,
              vector<int> &price,
              vector<vector<int>> &dp)
{
    if (ind == 0)
        return len * price[0];

    if (dp[ind][len] != -1)
        return dp[ind][len];

    int notTake = solveMemo(ind - 1, len, price, dp);

    int take = 0;
    int rodLength = ind + 1;

    if (rodLength <= len)
    {
        take = price[ind] +
               solveMemo(ind, len - rodLength,
                         price, dp);
    }

    return dp[ind][len] = max(take, notTake);
}

int rodCuttingMemo(vector<int> &price, int n)
{
    vector<vector<int>> dp(n,
                           vector<int>(n + 1, -1));

    return solveMemo(n - 1, n, price, dp);
}

/* ---------------- Tabulation ---------------- */

int rodCuttingTab(vector<int> &price, int n)
{
    vector<vector<int>> dp(n,
                           vector<int>(n + 1, 0));

    for (int len = 0; len <= n; len++)
        dp[0][len] = len * price[0];

    for (int ind = 1; ind < n; ind++)
    {
        int rodLength = ind + 1;

        for (int len = 0; len <= n; len++)
        {
            int notTake = dp[ind - 1][len];

            int take = 0;

            if (rodLength <= len)
            {
                take = price[ind] +
                       dp[ind][len - rodLength];
            }

            dp[ind][len] = max(take, notTake);
        }
    }

    return dp[n - 1][n];
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> price(n);

    for (int i = 0; i < n; i++)
        cin >> price[i];

    cout << "Memoization: "
         << rodCuttingMemo(price, n) << '\n';

    cout << "Tabulation: "
         << rodCuttingTab(price, n) << '\n';

    return 0;
}