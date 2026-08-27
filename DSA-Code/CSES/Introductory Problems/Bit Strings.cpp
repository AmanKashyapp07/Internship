// Link: https://cses.fi/problemset/task/1617
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    long long ans = 1, base = 2;
    for (; n; n >>= 1) {
        if (n & 1) ans = ans * base % MOD;
        base = base * base % MOD;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Calculate 2^n modulo 10^9+7 (CSES 1617).
// - Approach: Binary Exponentiation (Fast Power).
// - Intuition: Total bit strings of length n is 2^n; compute efficiently using exponentiation by squaring in O(log n).
// - Complexity: Time: O(log N), Space: O(1).