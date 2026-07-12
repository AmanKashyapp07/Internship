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

int main() {
    int n;
    cin >> n;

    vector<int> coins(n);
    int totalSum = 0;

    for (int i = 0; i < n; i++) {
        cin >> coins[i];
        totalSum += coins[i];
    }

    vector<bool> dp(totalSum + 1, false);
    dp[0] = true;

    for (int coin : coins) {
        for (int sum = totalSum; sum >= coin; sum--) {
            dp[sum] = dp[sum] || dp[sum - coin];
        }
    }

    vector<int> ans;

    for (int sum = 1; sum <= totalSum; sum++) {
        if (dp[sum]) {
            ans.push_back(sum);
        }
    }

    cout << ans.size() << '\n';

    for (int x : ans) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
