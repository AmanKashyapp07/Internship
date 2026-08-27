// Link: https://cses.fi/problemset/task/1744
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int a, b; cin >> a >> b;
    vector<vector<int>> dp(a + 1, vector<int>(b + 1, 0));

    for (int i = 1; i <= a; i++) {
        for (int j = 1; j <= b; j++) {
            if (i == j) { dp[i][j] = 1; continue; }
            int ans = INT_MAX;
            for (int k = 1; k < i; k++) ans = min(ans, dp[k][j] + dp[i - k][j]);
            for (int k = 1; k < j; k++) ans = min(ans, dp[i][k] + dp[i][j - k]);
            dp[i][j] = ans;
        }
    }
    cout << dp[a][b] - 1 << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum cuts to split an a x b rectangle into 1 x 1 squares (CSES 1744).
// - Approach: Rectangle Interval DP (`dp[i][j] = min cuts needed for i x j rectangle`).
// - Intuition: Try every horizontal and vertical cut; a 1x1 square requires 0 cuts ($dp[i][i] = 1$ counts pieces). Answer is pieces minus 1.
// - Complexity: Time: O(A^2 B + AB^2), Space: O(A \cdot B).