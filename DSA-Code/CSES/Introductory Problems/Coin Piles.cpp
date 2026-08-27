// Link: https://cses.fi/problemset/task/1754
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) {
        long long a, b; cin >> a >> b;
        if ((a + b) % 3 == 0 && a <= 2 * b && b <= 2 * a) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Empty two coin piles by repeatedly removing (1, 2) or (2, 1) coins (CSES 1754).
// - Approach: Direct Mathematical Invariant validation.
// - Intuition: Each move removes 3 coins total, so (a+b) must be divisible by 3; neither pile can exceed double the other because max reduction ratio is 2:1.
// - Complexity: Time: O(1) per test case, Space: O(1).
