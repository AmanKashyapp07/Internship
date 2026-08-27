// Link: https://cses.fi/problemset/task/1082
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
const long long INV2 = 500000004;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    long long ans = 0, l = 1;

    while (l <= n) {
        long long q = n / l;
        long long r = n / q;
        long long count = (r - l + 1) % MOD;
        long long sumRange = ((l + r) % MOD) * count % MOD * INV2 % MOD;
        ans = (ans + (q % MOD) * sumRange % MOD) % MOD;
        l = r + 1;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute Σ σ(i) for 1 <= i <= n modulo 10^9+7 (CSES 1082).
// - Approach: Quotient Block Decomposition (Square Root Decomposition).
// - Intuition: Total sum = Σ d * ⌊n/d⌋. Since ⌊n/d⌋ takes at most 2√n distinct values, process contiguous ranges [l, r] with constant quotient in O(√n) total steps.
// - Complexity: Time: O(√N), Space: O(1).