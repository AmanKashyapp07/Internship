// Link: https://cses.fi/problemset/task/3403
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<int> a(n), b(m);
    for (int &x : a) cin >> x;
    for (int &x : b) cin >> x;

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    vector<int> lcs;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) { lcs.push_back(a[i - 1]); i--; j--; }
        else if (dp[i - 1][j] >= dp[i][j - 1]) i--;
        else j--;
    }
    reverse(lcs.begin(), lcs.end());

    cout << lcs.size() << '\n';
    for (int x : lcs) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find and print the longest common subsequence of two integer sequences a and b (CSES 3403).
// - Approach: 2D Dynamic Programming Table + Backtracking Path Recovery.
// - Intuition: `dp[i][j]` tracks LCS of prefixes $a[0..i-1]$ and $b[0..j-1]$; backtracking matrix cell decisions reconstructs the sequence.
// - Complexity: Time: O(N \cdot M), Space: O(N \cdot M).