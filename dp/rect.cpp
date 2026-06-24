/**
 * Problem: Rectangle Cutting (https://cses.fi/problemset/task/1744)
 * Find the minimum number of cuts to partition an a x b rectangle into squares.
 * Time: O(A * B * (A + B)) time, O(A * B) space.
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int a, b; if (!(cin >> a >> b)) return 0;
    vector<vector<int>> dp(a + 1, vector<int>(b + 1, 1e9));
    // DP state: dp[i][j] stores the min cuts needed for an i x j rectangle
    for (int i = 1; i <= a; i++) {
        for (int j = 1; j <= b; j++) {
            if (i == j) dp[i][j] = 0; // Base case: already a square
            else {
                for (int k = 1; k < i; k++) dp[i][j] = min(dp[i][j], dp[k][j] + dp[i - k][j] + 1); // Horizontal cuts
                for (int k = 1; k < j; k++) dp[i][j] = min(dp[i][j], dp[i][k] + dp[i][j - k] + 1); // Vertical cuts
            }
        }
    }
    cout << dp[a][b] << "\n";
}