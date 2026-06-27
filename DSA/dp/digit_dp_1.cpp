/**
 * LeetCode 233 - Number of Digit One
 *
 * Description:
 * Given an integer n, count the total number of digit 1 appearing in all non-negative integers less than or equal to n.
 *
 * Approach:
 * - Digit Dynamic Programming.
 * - Let `solve(pos, count1, started, tight)` count the number of 1s in all suffix combinations.
 * - Transition:
 *   - Skip leading zeros if the number has not started.
 *   - For any digit `d <= limit`, add `solve(pos + 1, count1 + (d == 1), true, tight && (d == limit))`.
 *
 * Time Complexity: O(log10(n) * 10 * log10(n))
 * Space Complexity: O(log10(n) * log10(n))
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    string num_;
    vector<vector<vector<vector<long long>>>> dp_;

    long long solve(int pos, int count1, bool started, bool tight) {

        // ---------------- Base Case ----------------
        if (pos == num_.size()) {
            // Return the total number of '1's accumulated in this valid number
            return count1;
        }

        if (dp_[pos][count1][started][tight] != -1)
            return dp_[pos][count1][started][tight];

        int limit = tight ? num_[pos] - '0' : 9;
        long long ans = 0;

        // ----------------------------------------------------
        // Option 1 : Skip this position (still leading zeros)
        // ----------------------------------------------------
        if (!started) {
            ans += solve(
                pos + 1,
                count1, // '1' is not placed, count remains same
                false,
                tight && (0 == limit)
            );
        }

        // ----------------------------------------------------
        // Option 2 : Start / Continue the number
        // ----------------------------------------------------
        for (int d = (started ? 0 : 1); d <= limit; d++) {

            ans += solve(
                pos + 1,
                count1 + (d == 1), // Increment count if the current digit is 1
                true,
                tight && (d == limit)
            );
        }

        return dp_[pos][count1][started][tight] = ans;
    }

    long long countDigitOne(long long n) {
        if (n < 0) return 0;
        
        num_ = to_string(n);
        dp_.assign(20, vector<vector<vector<long long>>>(20, vector<vector<long long>>(2, vector<long long>(2, -1))));

        return solve(
            0,
            0, // initial count of '1's is 0
            false,
            true
        );
    }
};

int main() {
    long long n = 13;
    Solution solver;
    cout << "Total 1s up to " << n << ": " << solver.countDigitOne(n) << endl; 
    // Output: 6 (from numbers: 1, 10, 11, 12, 13 -> notice 11 has two 1s)
    return 0;
}