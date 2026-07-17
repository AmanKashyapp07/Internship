// Link: https://cses.fi/problemset/task/1620

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
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