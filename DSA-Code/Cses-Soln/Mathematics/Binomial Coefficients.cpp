// Link: https://cses.fi/problemset/task/1079
#include <bits/stdc++.h>
using namespace std;

const int MAXA = 1'000'000;
const long long MOD = 1'000'000'007LL;

long long pw(long long a, long long b) {
    long long r = 1; a %= MOD;
    for (; b; b >>= 1) { if (b & 1) r = r * a % MOD; a = a * a % MOD; }
    return r;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    vector<long long> fact(MAXA + 1), inv(MAXA + 1);
    fact[0] = 1;
    for (int i = 1; i <= MAXA; i++) fact[i] = fact[i-1] * i % MOD;
    inv[MAXA] = pw(fact[MAXA], MOD - 2);
    for (int i = MAXA; i >= 1; i--) inv[i-1] = inv[i] * i % MOD;

    int n; cin >> n;
    while (n--) {
        int a, b; cin >> a >> b;
        cout << fact[a] * inv[b] % MOD * inv[a-b] % MOD << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute C(a, b) = a! / (b! * (a-b)!) mod 10^9+7 for n queries (CSES 1079).
// - Approach: Precompute factorials + inverse factorials using Fermat's little theorem.
// - Intuition: Precompute `fact[i]` and `invFact[i]` once in O(N); each query is O(1) multiplication.
// - Complexity: Time: O(MAX + N), Space: O(MAX).