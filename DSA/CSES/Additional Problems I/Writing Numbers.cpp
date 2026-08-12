// Link: https://cses.fi/problemset/task/1086

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

class DigitDP {
    string s;
    // memo[pos][started][tight][count]
    // pos <= 20, started <= 2, tight <= 2, count <= 200
    long long memo[20][2][2][200]; 
    int target_digit;

    long long dp(int pos, bool started, bool tight, int count) {
        if (pos == s.size()) return count; 
        if (memo[pos][started][tight][count] != -1) return memo[pos][started][tight][count];

        int limit = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        // Option 1: Place a leading zero (if not started yet)
        if (!started) {
            ans += dp(pos + 1, false, tight && (0 == limit), count);
        }

        // Option 2: Place a digit d
        for (int d = (started ? 0 : 1); d <= limit; d++) {
            int new_count = count + (d == target_digit ? 1 : 0);
            ans += dp(pos + 1, true, tight && (d == limit), new_count);
        }

        return memo[pos][started][tight][count] = ans;
    }

public:
    // Returns how many times `digit` appears in numbers from 1 to `n`
    long long count_digit(long long n, int digit) {
        if (n <= 0) return 0;
        s = to_string(n);
        target_digit = digit;
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

// Returns true if all digits 0-9 appear at most 'n' times when writing numbers 1..mid
bool check(long long mid, long long n) {
    DigitDP solver;
    for (int d = 0; d <= 9; d++) {
        if (solver.count_digit(mid, d) > n) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    if (!(cin >> n)) return 0;

    // Binary search range for the answer X
    long long low = 1, high = 1e18;
    long long ans = 1;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (check(mid, n)) {
            ans = mid;
            low = mid + 1; // Try to write a larger number
        } else {
            high = mid - 1; // Limit exceeded, search smaller range
        }
    }

    cout << ans << "\n";

    return 0;
}