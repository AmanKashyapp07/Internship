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

vector<int> dp;

int solve(int n) {
    if (n == 0) return 0;

    if (dp[n] != -1) return dp[n];

    int ans = INT_MAX;
    int x = n;

    while (x > 0) {
        int digit = x % 10;
        x /= 10;

        if (digit != 0) {
            ans = min(ans, 1 + solve(n - digit));
        }
    }

    return dp[n] = ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    dp.assign(n + 1, -1);

    cout << solve(n) << '\n';

    return 0;
}
