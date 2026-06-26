/**
 * LeetCode 902 - Numbers At Most N Given Digit Set
 *
 * Description:
 * Given an array of digits, we can write numbers using these digits, using each digit as many times as we want.
 * Return the number of positive integers that can be generated that are less than or equal to a given integer n.
 *
 * Approach:
 * - Digit Dynamic Programming.
 * - Map the allowed digits into a fast-lookup boolean table `isAllowed`.
 * - Use a state `(pos, started, tight)` where:
 *   - Skip positions to allow forming numbers of lengths smaller than the length of `n`.
 *   - If the number has started, iterate only over elements `d <= limit` that are allowed in the digits set.
 *
 * Time Complexity: O(log10(n) * 10)
 * Space Complexity: O(log10(n))
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

string num;
bool isAllowed[10]; // To quickly check if a digit is in the allowed set

// dp[pos][started][tight]
// Max length of N is 10^9, so at most 10 digits.
long long dp[20][2][2];

long long solve(int pos, bool started, bool tight) {

    // ---------------- Base Case ----------------
    if (pos == num.size()) {
        // Return 1 if a valid number has started (not all leading zeros), otherwise 0.
        return started;
    }

    if (dp[pos][started][tight] != -1)
        return dp[pos][started][tight];

    int limit = tight ? num[pos] - '0' : 9;
    long long ans = 0;

    // ----------------------------------------------------
    // Option 1 : Skip this position (still leading zeros)
    // ----------------------------------------------------
    if (!started) {
        ans += solve(
            pos + 1,
            false,
            tight && (0 == limit)
        );
    }

    // ----------------------------------------------------
    // Option 2 : Start / Continue the number
    // ----------------------------------------------------
    for (int d = (started ? 0 : 1); d <= limit; d++) {

        // Put your conditions here.
        // Condition: The digit MUST be present in the allowed digits array.
        if (!isAllowed[d]) continue;

        ans += solve(
            pos + 1,
            true,
            tight && (d == limit)
        );
    }

    return dp[pos][started][tight] = ans;
}

int atMostNGivenDigitSet(vector<string>& digits, int n) {
    // Reset allowed list
    memset(isAllowed, false, sizeof(isAllowed));
    for (const string& d : digits) {
        isAllowed[d[0] - '0'] = true;
    }

    num = to_string(n);
    memset(dp, -1, sizeof(dp));

    return solve(
        0,
        false,
        true
    );
}

int main() {
    vector<string> digits = {"1", "3", "5", "7"};
    int n = 100;
    cout << "Total numbers: " << atMostNGivenDigitSet(digits, n) << endl;
    // Output: 20
    return 0;
}