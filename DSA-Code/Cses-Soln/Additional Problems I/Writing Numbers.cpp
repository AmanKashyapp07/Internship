// Link: https://cses.fi/problemset/task/1086
#include <bits/stdc++.h>
using namespace std;

class DigitDP {
    string s;
    long long memo[20][2][2][200];
    int target_digit;

    long long dp(int pos, bool started, bool tight, int count) {
        if (pos == s.size()) return count;
        if (memo[pos][started][tight][count] != -1) return memo[pos][started][tight][count];
        int limit = tight ? s[pos] - '0' : 9;
        long long ans = 0;
        if (!started) ans += dp(pos + 1, false, tight && (0 == limit), count);
        for (int d = (started ? 0 : 1); d <= limit; d++) {
            int new_count = count + (d == target_digit ? 1 : 0);
            ans += dp(pos + 1, true, tight && (d == limit), new_count);
        }
        return memo[pos][started][tight][count] = ans;
    }
public:
    long long count_digit(long long n, int digit) {
        if (n <= 0) return 0;
        s = to_string(n); target_digit = digit;
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

bool check(long long mid, long long n) {
    DigitDP solver;
    for (int d = 0; d <= 9; d++) {
        if (solver.count_digit(mid, d) > n) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; if (!(cin >> n)) return 0;
    long long low = 1, high = 1e18, ans = 1;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (check(mid, n)) { ans = mid; low = mid + 1; }
        else { high = mid - 1; }
    }
    cout << ans << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum upper integer X such that every digit 0-9 appears at most n times when writing numbers 1..X (CSES 1086).
// - Approach: Binary Search on Answer range [1, 10^18] + Digit DP counting.
// - Intuition: Digit DP computes frequency of digit d in range 1..mid in O(length * 10) state transitions; binary search finds maximum valid X.
// - Complexity: Time: O(60 \cdot 10 \cdot D \log_{10} X) where D = 10, Space: O(1) state space.