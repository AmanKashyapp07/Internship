/**
 * Problem: Removal Game (https://cses.fi/problemset/task/1097)
 * Find the maximum score of player 1 in a game where elements are removed from ends.
 * Time: O(N^2) time, O(N^2) space.
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