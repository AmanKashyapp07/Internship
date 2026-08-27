// Link: https://cses.fi/problemset/task/1071
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) {
        long long y, x; cin >> y >> x;
        long long z = max(y, x);
        long long z2 = (z - 1) * (z - 1);
        long long ans = 0;
        if (z % 2 == 0) ans = (y == z) ? z * z - x + 1 : z2 + y;
        else            ans = (x == z) ? z * z - y + 1 : z2 + x;
        cout << ans << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find number at row y, column x in an infinite numerical spiral grid (CSES 1071).
// - Approach: O(1) Mathematical layer/square formula based on max(y, x).
// - Intuition: The layer max(y, x) contains values bounded by (z-1)^2 + 1 and z^2; parity of z determines progression direction along layer boundary.
// - Complexity: Time: O(1) per query, Space: O(1).