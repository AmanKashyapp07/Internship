// Link: https://cses.fi/problemset/task/1097
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> a(n);
    long long sum = 0;
    for (long long &x : a) { cin >> x; sum += x; }

    vector<vector<long long>> dp(n, vector<long long>(n, LLONG_MIN));

    function<long long(int, int)> solve = [&](int i, int j) -> long long {
        if (i == j) return a[i];
        if (dp[i][j] != LLONG_MIN) return dp[i][j];
        return dp[i][j] = max(a[i] - solve(i + 1, j), a[j] - solve(i, j - 1));
    };

    long long diff = solve(0, n - 1);
    cout << (sum + diff) / 2 << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Two players alternately take coins from either end; find the maximum score of the first player (CSES 1097).
// - Approach: Interval Dynamic Programming (Minimax with Memoization).
// - Intuition: `dp[i][j]` stores the maximum score DIFFERENCE the current player can achieve from coins $[i, j]$; $\text{firstScore} = (\text{totalSum} + \text{diff}) / 2$.
// - Complexity: Time: O(N^2), Space: O(N^2).