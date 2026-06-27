/**
 * Problem: Rod Cutting
 *
 * Description:
 * Given a rod of length n and prices price[i] for pieces of length i + 1,
 * find the maximum value obtainable by cutting up the rod and selling the pieces.
 *
 * Approach:
 * - Dynamic Programming (both Memoization and Tabulation).
 * - Equivalent to Unbounded Knapsack problem where elements are lengths `1..n` and capacities are `n`.
 * - Let `dp[ind][len]` be the max value considering pieces up to length `ind+1` with total rod length limit `len`.
 * - Transition: `dp[ind][len] = max(dp[ind-1][len], price[ind] + dp[ind][len - rodLength])`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/* ---------------- Memoization ---------------- */

int solveMemo(int ind, int len, vector<int> &price, vector<vector<int>> &dp) {
    if (ind == 0) return len * price[0];
    if (dp[ind][len] != -1) return dp[ind][len];

    int notTake = solveMemo(ind - 1, len, price, dp);
    int take = 0;
    int rodLength = ind + 1;

    if (rodLength <= len) {
        take = price[ind] + solveMemo(ind, len - rodLength, price, dp);
    }

    return dp[ind][len] = max(take, notTake);
}

int rodCuttingMemo(vector<int> &price, int n) {
    vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    return solveMemo(n - 1, n, price, dp);
}

/* ---------------- Tabulation ---------------- */

int rodCuttingTab(vector<int> &price, int n) {
    vector<vector<int>> dp(n, vector<int>(n + 1, 0));

    for (int len = 0; len <= n; len++) {
        dp[0][len] = len * price[0];
    }

    for (int ind = 1; ind < n; ind++) {
        int rodLength = ind + 1;
        for (int len = 0; len <= n; len++) {
            int notTake = dp[ind - 1][len];
            int take = 0;
            if (rodLength <= len) {
                take = price[ind] + dp[ind][len - rodLength];
            }
            dp[ind][len] = max(take, notTake);
        }
    }

    return dp[n - 1][n];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> price(n);
    for (int i = 0; i < n; i++) {
        cin >> price[i];
    }

    cout << "Memoization: " << rodCuttingMemo(price, n) << '\n';
    cout << "Tabulation: " << rodCuttingTab(price, n) << '\n';

    return 0;
}
