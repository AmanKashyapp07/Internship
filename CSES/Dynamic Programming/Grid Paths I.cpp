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

const int MOD = 1e9 + 7;

int n;
vector<string> grid;
vector<vector<int>> dp;

int solve(int i, int j) {
    if (i >= n || j >= n) return 0;
    if (grid[i][j] == '*') return 0;

    if (i == n - 1 && j == n - 1) return 1;

    if (dp[i][j] != -1) return dp[i][j];

    long long down = solve(i + 1, j);
    long long right = solve(i, j + 1);

    return dp[i][j] = (down + right) % MOD;
}

int main() {
    cin >> n;

    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    dp.assign(n, vector<int>(n, -1));

    cout << solve(0, 0) << '\n';
}
