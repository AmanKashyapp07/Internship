#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long modPow(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    cin >> n >> m;

    long long N = n + m - 1;

    vector<long long> fact(N + 1), invFact(N + 1);

    fact[0] = 1;
    for (long long i = 1; i <= N; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }

    invFact[N] = modPow(fact[N], MOD - 2);

    for (long long i = N; i >= 1; i--) {
        invFact[i - 1] = invFact[i] * i % MOD;
    }

    long long ans = fact[N];
    ans = ans * invFact[m] % MOD;
    ans = ans * invFact[n - 1] % MOD;

    cout << ans << '\n';
    return 0;
}