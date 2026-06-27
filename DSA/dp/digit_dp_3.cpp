/**
 * LeetCode 2719 - Count of Integers
 *
 * Description:
 * Given two numeric strings num1 and num2 and two integers min_sum and max_sum.
 * Return the number of good integers. An integer x is good if:
 * 1. num1 <= x <= num2
 * 2. min_sum <= sum_of_digits(x) <= max_sum
 *
 * Approach:
 * - Digit Dynamic Programming with Memoization.
 * - Calculate `countUpTo(num2) - countUpTo(num1)` using Digit DP.
 * - The state `(pos, current_sum, started, tight)` accumulates the digit sum. If `current_sum > maxSum`, we prune the search.
 * - Safely adjust MOD arithmetic and check if `num1` itself is good to add it back.
 *
 * Time Complexity: O(log10(num2) * max_sum * 10)
 * Space Complexity: O(log10(num2) * max_sum)
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

const int MOD = 1e9 + 7;

class Solution {
public:
    string num_;
    int minSum_, maxSum_;
    vector<vector<vector<vector<long long>>>> dp_;

    long long solve(int pos, int current_sum, bool started, bool tight) {
        // Pruning: If the sum already exceeds maxSum, this path is invalid
        if (current_sum > maxSum_) return 0;

        // ---------------- Base Case ----------------
        if (pos == num_.size()) {
            // Return 1 if the accumulated digit sum falls within the valid range
            return (current_sum >= minSum_ && current_sum <= maxSum_) ? 1 : 0;
        }

        if (dp_[pos][current_sum][started][tight] != -1)
            return dp_[pos][current_sum][started][tight];

        int limit = tight ? num_[pos] - '0' : 9;
        long long ans = 0;

        // ----------------------------------------------------
        // Option 1 : Skip this position (still leading zeros)
        // ----------------------------------------------------
        if (!started) {
            ans = (ans + solve(
                pos + 1,
                current_sum, // sum remains 0
                false,
                tight && (0 == limit)
            )) % MOD;
        }

        // ----------------------------------------------------
        // Option 2 : Start / Continue the number
        // ----------------------------------------------------
        for (int d = (started ? 0 : 1); d <= limit; d++) {
            
            ans = (ans + solve(
                pos + 1,
                current_sum + d, // Add current digit to our running sum
                true,
                tight && (d == limit)
            )) % MOD;
        }

        return dp_[pos][current_sum][started][tight] = ans;
    }

    // Helper to calculate total good integers from 0 up to string x
    long long countUpTo(string x) {
        num_ = x;
        dp_.assign(25, vector<vector<vector<long long>>>(210, vector<vector<long long>>(2, vector<long long>(2, -1))));
        return solve(0, 0, false, true);
    }

    // Helper to check if num1 itself is a valid "good" integer
    bool isGood(string x) {
        int sum = 0;
        for (char c : x) {
            sum += (c - '0');
        }
        return sum >= minSum_ && sum <= maxSum_;
    }

    int countStrings(string num1, string num2, int min_sum, int max_sum) {
        minSum_ = min_sum;
        maxSum_ = max_sum;

        long long ans2 = countUpTo(num2);
        long long ans1 = countUpTo(num1);

        // Calculate (ans2 - ans1) % MOD safely
        long long result = (ans2 - ans1 + MOD) % MOD;

        // If num1 itself matches the condition, add it back since we excluded it
        if (isGood(num1)) {
            result = (result + 1) % MOD;
        }

        return result;
    }
};

int main() {
    string num1 = "1", num2 = "12";
    int min_sum = 1, max_sum = 8;
    Solution solver;
    cout << "Total good integers: " << solver.countStrings(num1, num2, min_sum, max_sum) << endl;
    // Output: 11 (Numbers: 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12)
    return 0;
}