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

#define int long long
using namespace std;

int solve(int i, int j, vector<int>& a, vector<vector<int>>& dp) {
    if (i == j) return a[i];

    if (dp[i][j] != LLONG_MIN)
        return dp[i][j];

    return dp[i][j] = max(
        a[i] - solve(i + 1, j, a, dp),
        a[j] - solve(i, j - 1, a, dp)
    );
}

signed main() {
    int n;
    cin >> n;

    vector<int> a(n);
    int sum = 0;

    for (int &x : a) {
        cin >> x;
        sum += x;
    }

    vector<vector<int>> dp(n, vector<int>(n, LLONG_MIN));

    int diff = solve(0, n - 1, a, dp);

    cout << (sum + diff) / 2 << '\n';
}
