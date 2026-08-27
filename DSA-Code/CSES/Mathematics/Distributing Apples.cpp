// Link: https://cses.fi/problemset/task/1716
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
    long long n, m; cin >> n >> m;
    long long N = n + m - 1;
    vector<long long> fact(N + 1), inv(N + 1);
    fact[0] = 1;
    for (int i = 1; i <= N; i++) fact[i] = fact[i - 1] * i % MOD;
    inv[N] = pw(fact[N], MOD - 2);
    for (int i = N - 1; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1) % MOD;

    long long ans = fact[N] * inv[m] % MOD * inv[n - 1] % MOD;
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count ways to distribute m identical apples to n children (CSES 1716).
// - Approach: Stars and Bars theorem — C(n + m - 1, m) mod 10^9+7.
// - Intuition: Placing m stars (apples) and n-1 bars (dividers) in a line gives C(n+m-1, m) combinations.
// - Complexity: Time: O(N + M), Space: O(N + M).