// Link: https://cses.fi/problemset/task/1618
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    long long zeros = 0;
    for (long long p = 5; p <= n; p *= 5) zeros += n / p;
    cout << zeros << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Calculate number of trailing zeros in n! (CSES 1618).
// - Approach: Legendre's Formula for prime factor 5 in n!.
// - Intuition: Trailing zeros are created by 2 * 5 pairs; since prime factor 2 is abundant, count prime factors of 5: ⌊n/5⌋ + ⌊n/25⌋ + ⌊n/125⌋ + ...
// - Complexity: Time: O(log_5 N), Space: O(1).