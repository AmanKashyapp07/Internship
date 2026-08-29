// Link: https://cses.fi/problemset/task/1068
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    while (n != 1) {
        cout << n << ' ';
        if (n % 2 == 0) n /= 2;
        else n = 3 * n + 1;
    }
    cout << 1 << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Simulate Collatz conjecture starting from n: if n is even n/2, if odd 3n+1 (CSES 1068).
// - Approach: Direct simulation loop until n reaches 1.
// - Intuition: Uses 64-bit integer (`long long`) to prevent integer overflow during odd steps ($3n + 1$).
// - Complexity: Time: O(Steps), Space: O(1).