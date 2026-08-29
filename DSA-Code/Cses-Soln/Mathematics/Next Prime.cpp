// Link: https://cses.fi/problemset/task/3396
#include <bits/stdc++.h>
using namespace std;

bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long d = 3; d * d <= n; d += 2) if (n % d == 0) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) {
        long long n; cin >> n;
        long long x = n + 1;
        if (x <= 2) { cout << 2 << '\n'; continue; }
        if (x % 2 == 0) x++;
        while (!isPrime(x)) x += 2;
        cout << x << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each query n, find the smallest prime strictly greater than n (CSES 3396).
// - Approach: Trial division primality test + linear scan from n+1 upward (odd numbers only).
// - Intuition: By Bertrand's postulate a prime always exists in (n, 2n]; odd-step scanning halves the checks.
// - Complexity: Time: O(T \cdot \sqrt{P}) per query where P is the answer, Space: O(1).