/**
 * LeetCode 312 - Burst Balloons
 *
 * Description:
 * You are given n balloons, indexed from 0 to n - 1. Each balloon is painted with a number on it.
 * You are asked to burst all the balloons. If you burst the i-th balloon, you will get nums[i - 1] * nums[i] * nums[i + 1] coins.
 * Return the maximum coins you can collect by bursting the balloons wisely.
 *
 * Approach:
 * - Interval Dynamic Programming (Matrix Chain Multiplication style) with Memoization.
 * - Let `maxCoins(i, j)` return the maximum coins collected by bursting all balloons in the range `[i, j]`.
 * - Transition: Try bursting balloon `k` (where `i <= k <= j`) last in this range.
 *   - The coins obtained: `balloon[i-1] * balloon[k] * balloon[j+1] + maxCoins(i, k-1) + maxCoins(k+1, j)`.
 *
 * Time Complexity: O(n^3)
 * Space Complexity: O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Solution {
public:
    vector<int> input;
    vector<vector<int>> memo;

    int solve(int i, int j) {
        // base case: we will also allow single balloon, then cost will be balloon[i-1]*balloon[i]*balloon[i+1]
        if (i > j) return 0; 
        if (memo[i][j] != -1) return memo[i][j];

        int ans = INT_MIN;
        for (int k = i; k <= j; k++) {
            int coins = input[k];
            if (i > 0) coins *= input[i - 1];
            if (j < (int)input.size() - 1) coins *= input[j + 1];

            ans = max(ans, coins + solve(i, k - 1) + solve(k + 1, j));
        }

        return memo[i][j] = ans;
    }

    int maxCoins(vector<int>& balloon) {
        input = balloon;
        int n = balloon.size();
        memo.assign(n, vector<int>(n, -1));
        return solve(0, n - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> balloon(n);
    for (int i = 0; i < n; i++) {
        cin >> balloon[i];
    }

    Solution solver;
    cout << solver.maxCoins(balloon) << "\n";
    return 0;
}
