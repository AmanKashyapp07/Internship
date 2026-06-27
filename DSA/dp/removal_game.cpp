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
    vector<int> a_;
    vector<vector<long long>> dp_;

    long long solve(int i, int j) {
        if (i == j) return a_[i];
        if (dp_[i][j] != -1e18) return dp_[i][j];
        long long take_left = a_[i] - solve(i + 1, j);
        long long take_right = a_[j] - solve(i, j - 1);
        return dp_[i][j] = max(take_left, take_right); // Transition
    }

    long long getMaxScore(int n, vector<int>& a) {
        a_ = a;
        long long sum = 0;
        for (int x : a_) {
            sum += x;
        }
        dp_.assign(n, vector<long long>(n, -1e18)); // dp[i][j] tracks max score difference
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