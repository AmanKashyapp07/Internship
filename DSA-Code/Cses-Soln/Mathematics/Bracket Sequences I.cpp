// Link: https://cses.fi/problemset/task/2064
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long pw(long long a, long long b) {
    long long r = 1; a %= MOD;
    for (; b; b >>= 1) { if (b & 1) r = r * a % MOD; a = a * a % MOD; }
    return r;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    if (n % 2) { cout << 0; return 0; }
    int k = n / 2;
    vector<long long> fact(n + 1), inv(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i % MOD;
    inv[n] = pw(fact[n], MOD - 2);
    for (int i = n - 1; i >= 0; i--) inv[i] = inv[i+1] * (i+1) % MOD;
    // Catalan(k) = C(2k, k) / (k+1)
    long long ans = fact[n] * inv[k] % MOD * inv[k] % MOD * pw(k + 1, MOD - 2) % MOD;
    cout << ans;
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count the number of valid bracket sequences of length n (CSES 2064).
// - Approach: Catalan Number formula — C(n, n/2) / (n/2 + 1) mod 10^9+7.
// - Intuition: Valid bracket sequences of length 2k are counted by the k-th Catalan number; odd n gives 0.
// - Complexity: Time: O(N), Space: O(N).