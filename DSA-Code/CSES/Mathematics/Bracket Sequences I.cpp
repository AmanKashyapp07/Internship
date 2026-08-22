// Link: https://cses.fi/problemset/task/2064

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long power(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    int n;
    cin >> n;

    if (n % 2) {
        cout << 0;
        return 0;
    }

    int k = n / 2;

    vector<long long> fact(n + 1), invFact(n + 1);

    fact[0] = 1;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;

    invFact[n] = power(fact[n], MOD - 2);

    for (int i = n - 1; i >= 0; i--)
        invFact[i] = invFact[i + 1] * (i + 1) % MOD;

    long long nCr = fact[n];
    nCr = nCr * invFact[k] % MOD;
    nCr = nCr * invFact[k] % MOD;

    long long ans = nCr * power(k + 1, MOD - 2) % MOD;

    cout << ans;
}