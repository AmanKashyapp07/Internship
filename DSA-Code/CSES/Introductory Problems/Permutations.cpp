// Link: https://cses.fi/problemset/task/1070
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    if (n == 2 || n == 3) { cout << "NO SOLUTION\n"; return 0; }
    for (int i = 2; i <= n; i += 2) cout << i << ' ';
    for (int i = 1; i <= n; i += 2) cout << i << " \n"[i >= n - 1];
    return 0;
}

// Interview Explanation:
// - Problem Statement: Construct a permutation of 1..n such that no two adjacent elements differ by 1 (CSES 1070).
// - Approach: Constructive algorithm — print all evens first, then all odds.
// - Intuition: Separating evens and odds guarantees adjacent difference is at least 2 everywhere except junction; junction difference between highest even and 1 is >= 2 for n >= 4.
// - Complexity: Time: O(N), Space: O(1).