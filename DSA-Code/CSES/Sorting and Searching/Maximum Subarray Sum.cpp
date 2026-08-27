// Link: https://cses.fi/problemset/task/1643
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    long long x; cin >> x;
    long long cur = x, ans = x;

    for (int i = 1; i < n; i++) {
        cin >> x;
        cur = max(x, cur + x);
        ans = max(ans, cur);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum non-empty subarray sum (CSES 1643).
// - Approach: Kadane's Algorithm ($O(N)$ dynamic programming).
// - Intuition: `cur = max(x, cur + x)` decides whether to extend previous subarray or start fresh at index i.
// - Complexity: Time: O(N), Space: O(1).