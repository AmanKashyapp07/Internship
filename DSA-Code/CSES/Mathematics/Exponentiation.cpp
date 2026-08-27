// Link: https://cses.fi/problemset/task/1095
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
    while (n--) { long long a, b; cin >> a >> b; cout << pw(a, b) << '\n'; }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute a^b mod 10^9+7 for n queries (CSES 1095).
// - Approach: Binary Exponentiation (Fast Power).
// - Intuition: Square-and-multiply: if bit is set, multiply into result; halve exponent each step.
// - Complexity: Time: O(N log B), Space: O(1).