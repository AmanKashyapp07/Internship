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

int minimumCoins(int sum, vector<int>& coins, vector<int>& dp) {
    if (sum == 0) {
        return 0;
    }

    if (sum < 0) {
        return INF;
    }

    if (dp[sum] != -1) {
        return dp[sum];
    }

    int answer = INF;

    for (int coin : coins) {
        answer = min(answer, 1 + minimumCoins(sum - coin, coins, dp));
    }

    return dp[sum] = answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    vector<int> dp(x + 1, -1);

    int answer = minimumCoins(x, coins, dp);

    cout << (answer == INF ? -1 : answer) << '\n';

    return 0;
}
