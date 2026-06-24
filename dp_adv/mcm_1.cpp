/**
 * Matrix Chain Multiplication
 * Recursive + Memoization
 *
 * dp[i][j] = Minimum cost to multiply matrices from i to j
 *
 * Transition:
 * dp[i][j] = min(
 *      dp[i][k] +
 *      dp[k+1][j] +
 *      arr[i-1] * arr[k] * arr[j]
 * )
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

#define all(x) (x).begin(), (x).end()

vector<vector<ll>> dp;

ll solve(int i, int j, vector<int>& arr)
{
    if (i == j) return 0; // base case: if there is only one matrix, no multiplication is needed

    if (dp[i][j] != -1)
        return dp[i][j];

    ll ans = LLONG_MAX;

    for (int k = i; k < j; k++)
    {
        ll cost =
            solve(i, k, arr) +
            solve(k + 1, j, arr) +
            1LL * arr[i - 1] * arr[k] * arr[j]; // cost will be solve(i, k) + solve(k+1, j) + arr[i-1]*arr[k]*arr[j] where solve(i,k) is min cost to multiply matrices from i to k and solve(k+1,j) is min cost to multiply matrices from k+1 to j and arr[i-1]*arr[k]*arr[j] is cost to multiply the two matrices obtained from solve(i,k) and solve(k+1,j)

        ans = min(ans, cost);
    }

    return dp[i][j] = ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arr(n);

    for (int i = 0; i < n; i++)
        cin >> arr[i];

    dp.assign(n, vector<ll>(n, -1));

    cout << solve(1, n - 1, arr) << '\n';

    return 0;
}