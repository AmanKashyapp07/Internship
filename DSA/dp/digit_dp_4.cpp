/**
 * CSES 2220 - Counting Numbers
 *
 * Description:
 * Count the number of integers in the range [a, b] where no two adjacent digits are equal.
 *
 * Approach:
 * - Digit Dynamic Programming.
 * - Compute `countUpTo(b) - countUpTo(a - 1)`.
 * - State `(pos, prev_digit, started, tight)` tracks:
 *   - `prev_digit` (range 0..9, with 10 representing no previous digit).
 *   - Check that the current digit `d != prev_digit` if the number has already started.
 *
 * Time Complexity: O(log10(b) * 10 * 10)
 * Space Complexity: O(log10(b) * 10)
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

    long long solve(int pos, int prev_digit, bool started, bool tight) {

        // ---------------- Base Case ----------------
        if (pos == num_.size()) {
            // A valid sequence has been formed. 
            // (Even if it's all zeros, we count it as the number 0).
            return 1;
        }

        if (dp_[pos][prev_digit][started][tight] != -1)
            return dp_[pos][prev_digit][started][tight];

        int limit = tight ? num_[pos] - '0' : 9;
        long long ans = 0;

        // ----------------------------------------------------
        // Option 1 : Skip this position (still leading zeros)
        // ----------------------------------------------------
        if (!started) {
            ans += solve(
                pos + 1,
                10, // No valid previous digit yet because the number hasn't started
                false,
                tight && (0 == limit)
            );
        }

        // ----------------------------------------------------
        // Option 2 : Start / Continue the number
        // ----------------------------------------------------
        for (int d = (started ? 0 : 1); d <= limit; d++) {

            // Condition: The current digit cannot be the same as the adjacent previous digit.
            if (started && d == prev_digit) continue;

            ans += solve(
                pos + 1,
                d, // The current digit becomes the prev_digit for the next position
                true,
                tight && (d == limit)
            );
        }

        return dp_[pos][prev_digit][started][tight] = ans;
    }

    // Helper to calculate total valid integers from 0 up to string x
    long long countUpTo(long long x) {
        if (x < 0) return 0;
        num_ = to_string(x);
        dp_.assign(20, vector<vector<vector<long long>>>(11, vector<vector<long long>>(2, vector<long long>(2, -1))));
        return solve(0, 10, false, true);
    }

    // Helper to check if 'a' itself has no adjacent matching digits
    bool isValid(long long x) {
        string s = to_string(x);
        for (size_t i = 1; i < s.length(); i++) {
            if (s[i] == s[i - 1]) return false;
        }
        return true;
    }
};

int main() {
    long long a, b;
    if (cin >> a >> b) {
        Solution solver;
        long long ansB = solver.countUpTo(b);
        long long ansA = solver.countUpTo(a);
        
        long long result = ansB - ansA;
        if (solver.isValid(a)) {
            result++;
        }
        
        cout << result << endl;
    }
    return 0;
}