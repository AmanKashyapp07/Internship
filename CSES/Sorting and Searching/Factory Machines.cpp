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

using i128 = __int128_t;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, t;
    cin >> n >> t;

    vector<ll> a(n);
    for (ll i = 0; i < n; i++) {
        cin >> a[i];
    }

    ll low = 0, high = 1e18;
    ll answer = -1;

    while (low <= high) {
        ll mid = low + (high - low) / 2;

        i128 total = 0;
        for (ll x : a) {
            total += (i128)mid / x;
        }

        if (total >= (i128)t) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << answer << '\n';
    return 0;
}
