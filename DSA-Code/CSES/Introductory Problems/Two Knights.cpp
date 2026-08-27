// Link: https://cses.fi/problemset/task/1072
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    for (long long k = 1; k <= n; k++) {
        long long totalWays = (k * k) * (k * k - 1) / 2;
        long long attackingWays = 4 * (k - 1) * (k - 2);
        cout << totalWays - attackingWays << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each k=1..n, count ways to place two knights on a k x k board so they don't attack (CSES 1072).
// - Approach: Combinatorial Complement Counting — Total pairs C(k^2, 2) minus attacking pairs.
// - Intuition: Knights attack iff placed on opposite corners of a 2x3 or 3x2 block; a k x k board contains (k-1)(k-2) blocks of 2x3 and (k-2)(k-1) blocks of 3x2, each having 2 attacking pairs.
// - Complexity: Time: O(N), Space: O(1).