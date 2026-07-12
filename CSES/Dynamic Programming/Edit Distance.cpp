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

string a, b;
vector<vector<int>> dp;

int solve(int i, int j) {
    if (i == (int)a.size()) return b.size() - j;
    if (j == (int)b.size()) return a.size() - i;

    int &ans = dp[i][j];
    if (ans != -1) return ans;

    if (a[i] == b[j]) {
        return ans = solve(i + 1, j + 1);
    }

    return ans = 1 + min({
        solve(i + 1, j),     // remove
        solve(i, j + 1),     // add
        solve(i + 1, j + 1)  // replace
    });
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> a >> b;

    dp.assign(a.size(), vector<int>(b.size(), -1));

    cout << solve(0, 0) << '\n';
    return 0;
}
