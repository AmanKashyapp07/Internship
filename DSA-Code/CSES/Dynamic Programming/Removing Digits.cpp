// Link: https://cses.fi/problemset/task/1637
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> dp(n + 1, -1);

    function<int(int)> solve = [&](int x) -> int {
        if (x == 0) return 0;
        if (dp[x] != -1) return dp[x];
        int ans = INT_MAX, tmp = x;
        while (tmp > 0) {
            int d = tmp % 10; tmp /= 10;
            if (d != 0) ans = min(ans, 1 + solve(x - d));
        }
        return dp[x] = ans;
    };

    cout << solve(n) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum steps to reduce n to 0 by subtracting one of its own digits (CSES 1637).
// - Approach: Top-Down Memoized DP (`dp[n] = 1 + min over digits`).
// - Intuition: Try removing each non-zero digit of current number; memoization avoids recomputing repeated subproblems.
// - Complexity: Time: O(N \cdot \log N), Space: O(N).