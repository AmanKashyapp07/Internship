// Link: https://cses.fi/problemset/task/1712

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007;

ll power(ll a, ll b, ll mod) {
    ll res = 1;
    a %= mod;

    while (b) {
        if (b & 1)
            res = res * a % mod;

        a = a * a % mod;
        b >>= 1;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        ll a, b, c;
        cin >> a >> b >> c;

        ll exponent = power(b, c, MOD - 1);

        cout << power(a, exponent, MOD) << '\n';
    }
}