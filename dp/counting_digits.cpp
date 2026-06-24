/**
 * Problem: Counting Numbers (Digit DP Variant) (https://cses.fi/problemset/task/2220)
 * Count numbers in range [a, b] where no two adjacent digits are equal.
 * Time: O(log10(B) * 10 * 2 * 2) time, O(log10(B)) space.
 */
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

long long dp[20][2][11][2];

long long solve(int idx, bool tight, int last, bool started, string &s) {
    if (idx == (int)s.size()) return 1;
    long long &ans = dp[idx][tight][last + 1][started];
    if (ans != -1) return ans;
    ans = 0; int limit = tight ? s[idx] - '0' : 9;
    for (int digit = 0; digit <= limit; digit++) {
        bool newTight = tight && (digit == limit);
        if (!started && digit == 0) {
            ans += solve(idx + 1, newTight, -1, false, s);
        } else {
            if (digit == last) continue;
            ans += solve(idx + 1, newTight, digit, true, s);
        }
    }
    return ans;
}

long long countValid(long long x) {
    if (x < 0) return 0;
    string s = to_string(x); memset(dp, -1, sizeof(dp));
    return solve(0, true, -1, false, s);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    long long a, b;
    if (cin >> a >> b) cout << countValid(b) - countValid(a - 1) << '\n';
}