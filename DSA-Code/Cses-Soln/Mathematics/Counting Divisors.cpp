// Link: https://cses.fi/problemset/task/1713
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    const int MAXX = 1'000'000;
    vector<int> d(MAXX + 1, 0);
    for (int i = 1; i <= MAXX; i++)
        for (int j = i; j <= MAXX; j += i) d[j]++;
    int n; cin >> n;
    while (n--) { int x; cin >> x; cout << d[x] << '\n'; }
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each query x, count the number of divisors of x (CSES 1713).
// - Approach: Sieve-style precomputation — for each d, increment count for all multiples.
// - Intuition: Same idea as Sieve of Eratosthenes; total work is harmonic series O(N log N).
// - Complexity: Time: O(N log N + Q), Space: O(N).