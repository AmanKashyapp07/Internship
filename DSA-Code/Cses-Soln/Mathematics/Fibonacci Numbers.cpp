// Link: https://cses.fi/problemset/task/1722
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

pair<long long, long long> fib(long long n) {
    if (n == 0) return {0, 1};
    auto [a, b] = fib(n / 2);
    long long c = a * ((2 * b % MOD - a + MOD) % MOD) % MOD;
    long long d = (a * a % MOD + b * b % MOD) % MOD;
    if (n % 2 == 0) return {c, d};
    return {d, (c + d) % MOD};
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    cout << fib(n).first << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute the n-th Fibonacci number F(n) modulo 10^9+7 for n up to 10^18 (CSES 1722).
// - Approach: Fast Doubling method returning {F(k), F(k+1)} in O(log n).
// - Intuition: Uses fast doubling identities: F(2k) = F(k)[2F(k+1) - F(k)], F(2k+1) = F(k)^2 + F(k+1)^2.
// - Complexity: Time: O(log N), Space: O(log N) recursion stack.