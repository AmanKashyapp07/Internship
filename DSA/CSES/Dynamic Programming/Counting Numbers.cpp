// Link: https://cses.fi/problemset/task/2220

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

string num;

// dp[pos][prev_digit][started][tight]
// Max length for a 64-bit integer (long long) is 20 digits.
// prev_digit ranges from 0 to 9. We can use 10 to represent "no previous digit".
long long dp[20][11][2][2];

long long solve(int pos, int prev_digit, bool started, bool tight) {

    // ---------------- Base Case ----------------
    if (pos == num.size()) {
        // A valid sequence has been formed. 
        // (Even if it's all zeros, we count it as the number 0).
        return 1;
    }

    if (dp[pos][prev_digit][started][tight] != -1)
        return dp[pos][prev_digit][started][tight];

    int limit = tight ? num[pos] - '0' : 9;
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

        // Put your conditions here.
        // Condition: The current digit cannot be the same as the graphacent previous digit.
        if (started && d == prev_digit) continue;

        ans += solve(
            pos + 1,
            d, // The current digit becomes the prev_digit for the next position
            true,
            tight && (d == limit)
        );
    }

    return dp[pos][prev_digit][started][tight] = ans;
}

// Helper to calculate total valid integers from 0 up to string x
long long countUpTo(long long x) {
    if (x < 0) return 0;
    num = to_string(x);
    memset(dp, -1, sizeof(dp));
    return solve(0, 10, false, true);
}

// Helper to check if 'a' itself has no graphacent matching digits
bool isValid(long long x) {
    string s = to_string(x);
    for (size_t i = 1; i < s.length(); i++) {
        if (s[i] == s[i - 1]) return false;
    }
    return true;
}

int main() {
    long long a, b;
    if (cin >> a >> b) {
        long long ansB = countUpTo(b);
        long long ansA = countUpTo(a);
        
        long long result = ansB - ansA;
        if (isValid(a)) {
            result++;
        }
        
        cout << result << endl;
    }
    return 0;
}