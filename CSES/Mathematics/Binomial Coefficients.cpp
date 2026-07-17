#include <bits/stdc++.h>
using namespace std;

const int MAXA = 1'000'000;
const long long MOD = 1'000'000'007LL;

long long modPow(long long a, long long b) {
    long long res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> fact(MAXA + 1);
    vector<long long> invFact(MAXA + 1);

    fact[0] = 1;
    for (int i = 1; i <= MAXA; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }

    invFact[MAXA] = modPow(fact[MAXA], MOD - 2);

    for (int i = MAXA; i >= 1; i--) {
        invFact[i - 1] = invFact[i] * i % MOD;
    }

    int n;
    cin >> n;

    while (n--) {
        int a, b;
        cin >> a >> b;

        long long ans = fact[a];
        ans = ans * invFact[b] % MOD;
        ans = ans * invFact[a - b] % MOD;

        cout << ans << '\n';
    }

    return 0;
}