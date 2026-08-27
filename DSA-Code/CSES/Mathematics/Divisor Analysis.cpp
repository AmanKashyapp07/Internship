// Link: https://cses.fi/problemset/task/2182
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
const long long PHI = MOD - 1;

long long pw(long long a, long long b, long long mod) {
    long long r = 1; a %= mod;
    for (; b; b >>= 1) {
        if (b & 1) r = (__int128)r * a % mod;
        a = (__int128)a * a % mod;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    long long num = 1, sum = 1, prod = 1, numModPhi = 1;

    for (int i = 0; i < n; i++) {
        long long p, k; cin >> p >> k;
        num = num * (k + 1) % MOD;

        long long geom = (pw(p, k + 1, MOD) - 1 + MOD) % MOD * pw(p - 1, MOD - 2, MOD) % MOD;
        sum = sum * geom % MOD;

        long long tri = (k % 2 == 0) ? ((k / 2) % PHI) * ((k + 1) % PHI) % PHI : (k % PHI) * (((k + 1) / 2) % PHI) % PHI;
        long long exp = numModPhi * tri % PHI;
        prod = pw(prod, k + 1, MOD) * pw(p, exp, MOD) % MOD;
        numModPhi = numModPhi * ((k + 1) % PHI) % PHI;
    }

    cout << num << ' ' << sum << ' ' << prod << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Given prime factorization of N, compute count, sum, and product of all divisors of N mod 10^9+7 (CSES 2182).
// - Approach: Multiplicative number-theoretic formulas (Geometric sum for sum, Fermat's exponent reduction mod (MOD-1) for product).
// - Intuition: d(N)=∏(k_i+1); σ(N)=∏(p_i^(k_i+1)-1)/(p_i-1); Product uses running divisor count exponentiation modulo MOD-1.
// - Complexity: Time: O(N log MOD), Space: O(1).