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

vector<int> a, b;
vector<vector<int>> dp;

int solve(int i, int j) {
    if (i == a.size() || j == b.size()) {
        return 0;
    }

    int &ans = dp[i][j];
    if (ans != -1) return ans;

    if (a[i] == b[j]) {
        return ans = 1 + solve(i + 1, j + 1);
    }

    return ans = max(
        solve(i + 1, j),
        solve(i, j + 1)
    );
}

vector<int> getLCS(int i, int j) {
    if (i == a.size() || j == b.size()) {
        return {};
    }

    if (a[i] == b[j]) {
        vector<int> res = getLCS(i + 1, j + 1);
        res.insert(res.begin(), a[i]);
        return res;
    }

    if (solve(i + 1, j) >= solve(i, j + 1)) {
        return getLCS(i + 1, j);
    }

    return getLCS(i, j + 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    a.resize(n);
    b.resize(m);

    for (int &x : a) cin >> x;
    for (int &x : b) cin >> x;

    dp.assign(n, vector<int>(m, -1));

    solve(0, 0);

    vector<int> lcs = getLCS(0, 0);

    cout << lcs.size() << '\n';
    for (int x : lcs) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
