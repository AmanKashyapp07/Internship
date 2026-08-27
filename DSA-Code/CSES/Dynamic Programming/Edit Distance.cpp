// Link: https://cses.fi/problemset/task/1639
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string a, b; cin >> a >> b;
    int m = a.size(), n = b.size();

    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    for (int i = 0; i <= m; i++) dp[i][n] = m - i;
    for (int j = 0; j <= n; j++) dp[m][j] = n - j;

    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            if (a[i] == b[j]) dp[i][j] = dp[i + 1][j + 1];
            else dp[i][j] = 1 + min({dp[i + 1][j], dp[i][j + 1], dp[i + 1][j + 1]});
        }
    }
    cout << dp[0][0] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum edit operations (insert, delete, replace) to transform string a to string b (CSES 1639).
// - Approach: Levenshtein Edit Distance Dynamic Programming.
// - Intuition: `dp[i][j]` considers matching character, or taking minimum of remove (`i+1, j`), add (`i, j+1`), and replace (`i+1, j+1`) operations.
// - Complexity: Time: O(|A| \cdot |B|), Space: O(|A| \cdot |B|).