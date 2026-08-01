// Link: https://cses.fi/problemset/task/1097

#include <bits/stdc++.h>
#define int long long
using namespace std;

int solve(int i, int j, vector<int>& a, vector<vector<int>>& dp) {
    if (i == j) return a[i];

    if (dp[i][j] != LLONG_MIN)
        return dp[i][j];

    return dp[i][j] = max(
        a[i] - solve(i + 1, j, a, dp),
        a[j] - solve(i, j - 1, a, dp)
    );
} // finds max diff in score between two players, given optimal play from both sides.

signed main() {
    int n;
    cin >> n;

    vector<int> a(n);
    int sum = 0;

    for (int &x : a) {
        cin >> x;
        sum += x;
    }

    vector<vector<int>> dp(n, vector<int>(n, LLONG_MIN));

    int diff = solve(0, n - 1, a, dp);
    int score1 = (sum + diff) / 2; // score of first player
    int score2 = (sum - diff) / 2; // score of second player

    cout << (sum + diff) / 2 << '\n'; // find out the maximum score of the first player, given the total sum and the difference in scores.
}