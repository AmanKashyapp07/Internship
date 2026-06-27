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

class Solution {
public:
    vector<int> input;
    vector<vector<long long>> memo;

    long long solve(int i, int j) {
        if (i == j) return input[i];
        if (memo[i][j] != -1e18) return memo[i][j];
        long long takeLeft = input[i] - solve(i + 1, j);
        long long takeRight = input[j] - solve(i, j - 1);
        return memo[i][j] = max(takeLeft, takeRight); // Transition
    }

    long long getMaxScore(int n, vector<int>& a) {
        input = a;
        long long sum = 0;
        for (int x : input) {
            sum += x;
        }
        memo.assign(n, vector<long long>(n, -1e18)); // dp[i][j] tracks max score difference
        long long diff = solve(0, n - 1);
        return (sum + diff) / 2; // P1_score + P2_score = sum, P1_score - P2_score = diff
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (cin >> n) {
        vector<int> a(n); 
        for (int &x : a) { 
            cin >> x; 
        }
        Solution solver;
        cout << solver.getMaxScore(n, a) << '\n';
    }
    return 0;
}