/**
 * Generic Template - Digit Dynamic Programming
 *
 * Description:
 * A template file outlining the standard recursive structure of Digit DP.
 *
 * Approach:
 * - Digits are processed left-to-right (from most significant to least significant).
 * - Parameters:
 *   - `pos`: current digit index.
 *   - `tight`: boolean flag indicating if the choices are bounded by the input number prefix.
 *   - `started`: boolean flag to handle leading zeros (optional, depends on if the number of digits/leading zero configuration affects the problem state).
 *   - `extra_state`: problem-specific tracking variables (e.g. current sum, counts of specific digits, divisibility states).
 *
 * Time Complexity: N/A
 * Space Complexity: N/A
 */

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

string num;

// dp dimensions depend on the problem.
// Example:
// dp[pos][extra_state][started][tight]

long long dp[20][...][2][2];

long long solve(int pos,
                /* extra states */,
                bool started,
                bool tight) {

    // ---------------- Base Case ----------------

    if (pos == num.size()) {

        // Return according to the problem.

        // Example 1:
        // return started;

        // Example 2:
        // return (sum >= L && sum <= R);

        // Example 3:
        // return cnt;

        // Example 4:
        // return 1;
    }

    if (dp[pos][...][started][tight] != -1)
        return dp[pos][...][started][tight];

    int limit = tight ? num[pos] - '0' : 9;

    long long ans = 0;

    // ----------------------------------------------------
    // Option 1 : Skip this position (still leading zeros)
    // ----------------------------------------------------

    if (!started) {

        ans += solve(
            pos + 1,
            /* same state */,
            false,
            tight && (0 == limit)
        );
    }

    // ----------------------------------------------------
    // Option 2 : Start / Continue the number
    // ----------------------------------------------------

    for (int d = (started ? 0 : 1); d <= limit; d++) {

        // Put your conditions here.

        // Example:
        // if(previousDigit == d) continue;
        // if(sum+d>maxSum) continue;

        ans += solve(
            pos + 1,

            /* update extra state */

            true,

            tight && (d == limit)
        );
    }

    return dp[pos][...][started][tight] = ans;
}

long long count(string x) {

    num = x;

    memset(dp, -1, sizeof(dp));

    return solve(
        0,
        /* initial state */,
        false,
        true
    );
}