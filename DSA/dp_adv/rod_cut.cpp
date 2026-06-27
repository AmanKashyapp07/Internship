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

class Solution {
public:
    vector<int> coinValues;
    vector<vector<int>> memo;

    /* ---------------- Memoization ---------------- */

    int solve(int ind, int lengthVal) {
        if (ind == 0) return lengthVal * coinValues[0];
        if (memo[ind][lengthVal] != -1) return memo[ind][lengthVal];

        int notTake = solve(ind - 1, lengthVal);
        int take = 0;
        int rodLength = ind + 1;

        if (rodLength <= lengthVal) {
            take = coinValues[ind] + solve(ind, lengthVal - rodLength);
        }

        return memo[ind][lengthVal] = max(take, notTake);
    }

    int rodCuttingMemo(vector<int> &price, int n) {
        coinValues = price;
        memo.assign(n, vector<int>(n + 1, -1));
        return solve(n - 1, n);
    }

    /* ---------------- Tabulation ---------------- */

    int rodCuttingTab(vector<int> &price, int n) {
        coinValues = price;
        memo.assign(n, vector<int>(n + 1, 0));

        for (int lengthVal = 0; lengthVal <= n; lengthVal++) {
            memo[0][lengthVal] = lengthVal * coinValues[0];
        }

        for (int ind = 1; ind < n; ind++) {
            int rodLength = ind + 1;
            for (int lengthVal = 0; lengthVal <= n; lengthVal++) {
                int notTake = memo[ind - 1][lengthVal];
                int take = 0;
                if (rodLength <= lengthVal) {
                    take = coinValues[ind] + memo[ind][lengthVal - rodLength];
                }
                memo[ind][lengthVal] = max(take, notTake);
            }
        }

        return memo[n - 1][n];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> price(n);
    for (int i = 0; i < n; i++) {
        cin >> price[i];
    }

    Solution solver;
    vector<int> priceCopy = price;
    cout << "Memoization: " << solver.rodCuttingMemo(price, n) << '\n';
    cout << "Tabulation: " << solver.rodCuttingTab(priceCopy, n) << '\n';

    return 0;
}
