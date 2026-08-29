// Link: https://cses.fi/problemset/task/1712
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long pw(long long a, long long b, long long mod) {
    long long r = 1; a %= mod;
    for (; b; b >>= 1) { if (b & 1) r = r * a % mod; a = a * a % mod; }
    return r;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    while (n--) {
        long long a, b, c; cin >> a >> b >> c;
        // a^(b^c) mod MOD = a^(b^c mod (MOD-1)) mod MOD  [Fermat's little theorem]
        cout << pw(a, pw(b, c, MOD - 1), MOD) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute a^(b^c) mod 10^9+7 for n queries (CSES 1712).
// - Approach: Double Modular Exponentiation using Fermat's Little Theorem.
// - Intuition: By FLT, a^(MOD-1) ≡ 1 (mod MOD) so a^x ≡ a^(x mod MOD-1); reduce exponent first.
// - Complexity: Time: O(N log C), Space: O(1).