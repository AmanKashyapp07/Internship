// Link: https://cses.fi/problemset/task/1717
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    if (n == 1) { cout << 0; return 0; }
    if (n == 2) { cout << 1; return 0; }
    const long long MOD = 1e9 + 7;
    long long a = 0, b = 1; // D(1), D(2)
    for (int i = 3; i <= n; i++) { long long c = (i - 1) * (a + b) % MOD; a = b; b = c; }
    cout << b << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count derangements of n elements mod 10^9+7 (CSES 1717).
// - Approach: Iterative DP using derangement recurrence D(n) = (n-1) * (D(n-1) + D(n-2)).
// - Intuition: Element 1 can swap with any of n-1 others; D(n) counts permutations with no fixed points.
// - Complexity: Time: O(N), Space: O(1).
