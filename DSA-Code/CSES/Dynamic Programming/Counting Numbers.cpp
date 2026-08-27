// Link: https://cses.fi/problemset/task/2220
#include <bits/stdc++.h>
using namespace std;

string num;
long long dp[20][11][2][2];

long long solve(int pos, int prev, bool started, bool tight) {
    if (pos == (int)num.size()) return 1;
    if (dp[pos][prev][started][tight] != -1) return dp[pos][prev][started][tight];

    int lim = tight ? num[pos] - '0' : 9;
    long long ans = 0;
    if (!started) ans += solve(pos + 1, 10, 0, tight && !lim);

    for (int d = started ? 0 : 1; d <= lim; d++) {
        if (!started || d != prev) ans += solve(pos + 1, d, 1, tight && d == lim);
    }
    return dp[pos][prev][started][tight] = ans;
}

long long countUpTo(long long x) {
    if (x < 0) return 0;
    num = to_string(x);
    memset(dp, -1, sizeof(dp));
    return solve(0, 10, 0, 1);
}

bool isValid(long long x) {
    string s = to_string(x);
    for (size_t i = 1; i < s.size(); i++) if (s[i] == s[i - 1]) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long a, b; cin >> a >> b;
    cout << countUpTo(b) - countUpTo(a) + isValid(a) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count integers in range $[a, b]$ where no two adjacent digits are equal (CSES 2220).
// - Approach: Digit DP (`solve(pos, prev, started, tight)`).
// - Intuition: Recursively build numbers digit-by-digit ensuring `d != prev`; `tight` flag constrains prefix bound, `started` handles leading zeros.
// - Complexity: Time: O(18 \cdot 10 \cdot 2 \cdot 2 \cdot 10) = O(\text{digits} \cdot 10), Space: O(1).