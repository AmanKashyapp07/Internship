/**
 * CSES 1097 - Removal Game
 *
 * Description:
 * Find the maximum score player 1 can achieve from an array `a` if players take turns removing numbers from ends.
 *
 * Approach:
 * - Interval Dynamic Programming.
 * - Let `dp[i][j]` be the maximum score difference (player1 - player2) for subarray `a[i...j]`.
 * - Transition: `dp[i][j] = max(a[i] - dp[i+1][j], a[j] - dp[i][j-1])`.
 * - Player 1 score = `(sum + dp[0][n-1]) / 2`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long long solve(int i, int j, const vector<int>& a, vector<vector<long long>>& dp) {
    if (i == j) return a[i];
    if (dp[i][j] != -1e18) return dp[i][j];
    long long take_left = a[i] - solve(i + 1, j, a, dp);
    long long take_right = a[j] - solve(i, j - 1, a, dp);
    return dp[i][j] = max(take_left, take_right); // Transition
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (cin >> n) {
        vector<int> a(n); long long sum = 0;
        for (int &x : a) { cin >> x; sum += x; }
        vector<vector<long long>> dp(n, vector<long long>(n, -1e18)); // dp[i][j] tracks max score difference
        long long diff = solve(0, n - 1, a, dp);
        cout << (sum + diff) / 2 << '\n'; // P1_score + P2_score = sum, P1_score - P2_score = diff
    }
}