/**
 * Problem: Counting Numbers (https://cses.fi/problemset/task/2220)
 * Count numbers in range [a, b] where no two adjacent digits are equal.
 * Time: O(log10(B) * 10 * 2 * 2) time, O(log10(B)) space.
 */
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

long long dp[20][10][2][2];

long long solve(const string &s, int idx, int last, bool tight, bool leading) {
    if (idx == (int)s.size()) return 1;
    if (dp[idx][last][tight][leading] != -1) return dp[idx][last][tight][leading];
    long long ans = 0; int limit = tight ? s[idx] - '0' : 9;
    // Digit DP: iterate through valid digit choices
    for (int d = 0; d <= limit; d++) {
        if (leading && d == 0) {
            ans += solve(s, idx + 1, 0, tight && (d == limit), true);
        } else {
            if (!leading && d == last) continue; // No adjacent duplicate digits allowed
            ans += solve(s, idx + 1, d, tight && (d == limit), false);
        }
    }
    return dp[idx][last][tight][leading] = ans;
}

long long countValid(long long x) {
    if (x < 0) return 0;
    if (x == 0) return 1;
    string s = to_string(x); memset(dp, -1, sizeof(dp));
    return solve(s, 0, 0, true, true);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    long long a, b;
    if (cin >> a >> b) cout << countValid(b) - countValid(a - 1) << "\n";
}
