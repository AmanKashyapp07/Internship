/**
 * Matrix Chain Multiplication
 * Bottom-Up Tabulation
 *
 * dp[i][j] = Minimum cost to multiply matrices from i to j
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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arr(n);

    for (int i = 0; i < n; i++)
        cin >> arr[i];

    vector<vector<ll>> dp(n, vector<ll>(n, 0));

    for (int len = 2; len <= n - 1; len++)
    {
        for (int i = 1; i + len - 1 < n; i++)
        {
            int j = i + len - 1;

            dp[i][j] = LLONG_MAX;

            for (int k = i; k < j; k++) // k is between i and j-1 inclusive because for last matrix, it needs to be multiplied with the previous matrix, so we cannot have k = j
            {
                dp[i][j] = min(
                    dp[i][j],
                    dp[i][k] +
                        dp[k + 1][j] +
                        1LL * arr[i - 1] * arr[k] * arr[j]);
            }
        }
    }

    cout << dp[1][n - 1] << '\n';

    return 0;
}