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

const long long MOD = 1e9 + 7;
const long long INV2 = 500000004;

int main() {
    int n;
    cin >> n;

    long long total = 1LL * n * (n + 1) / 2;

    if (total & 1) {
        cout << 0 << '\n';
        return 0;
    }

    int target = total / 2;

    vector<long long> dp(target + 1, 0);
    dp[0] = 1;

    for (int x = 1; x <= n; x++) {
        for (int s = target; s >= x; s--) {
            dp[s] = (dp[s] + dp[s - x]) % MOD;
        }
    }

    cout << dp[target] * INV2 % MOD << '\n';
}
