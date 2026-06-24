/**
 * Problem: Exponentiation II
 * Link: https://cses.fi/problemset/task/1712
 * Category: Mathematics
 * 
 * Description:
 * Compute a^(b^c) modulo 10^9+7.
 * 
 * Logic/Approach:
 * Fermat's Little Theorem: compute b^c mod (10^9+6), then a^exponent mod 10^9+7.
 */

#include <iostream>
using namespace std;

#define int long long

const int MOD = 1e9 + 7;

int power(int a, int b, int mod) {
    int res = 1;
    a %= mod;

    while (b) {
        if (b & 1)
            res = res * a % mod;

        a = a * a % mod;
        b >>= 1;
    }

    return res;
}

void solve() {
    int a, b, c;
    cin >> a >> b >> c;

    int exponent = power(b, c, MOD - 1);

    cout << power(a, exponent, MOD) << '\n';
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--)
        solve();

    return 0;
}