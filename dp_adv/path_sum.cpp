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

int solveMemo(int i, int j, vector<vector<int>> &grid,
              vector<vector<int>> &dp)
{
    if (i == 0 && j == 0)
        return grid[0][0];

    if (i < 0 || j < 0)
        return INF;

    if (dp[i][j] != -1)
        return dp[i][j];

    int up = solveMemo(i - 1, j, grid, dp);
    int left = solveMemo(i, j - 1, grid, dp);

    return dp[i][j] = grid[i][j] + min(up, left);
}

int minPathSumMemo(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();

    vector<vector<int>> dp(m, vector<int>(n, -1));

    return solveMemo(m - 1, n - 1, grid, dp);
}

/* ---------------- Tabulation ---------------- */

int minPathSumTab(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();

    vector<vector<int>> dp(m, vector<int>(n, 0));

    dp[0][0] = grid[0][0];

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == 0 && j == 0)
                continue;

            int up = INF;
            int left = INF;

            if (i > 0)
                up = dp[i - 1][j];

            if (j > 0)
                left = dp[i][j - 1];

            dp[i][j] = grid[i][j] + min(up, left);
        }
    }

    return dp[m - 1][n - 1];
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vector<int>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };

    cout << "Memoization: " << minPathSumMemo(grid) << '\n';
    cout << "Tabulation: " << minPathSumTab(grid) << '\n';

    return 0;
}