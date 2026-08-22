#include <bits/stdc++.h>
using namespace std;

string num;
long long dp[20][11][2][2];

long long solve(int pos, int prev, bool started, bool tight) {
    if (pos == num.size()) return 1;
    if (dp[pos][prev][started][tight] != -1) return dp[pos][prev][started][tight];

    int lim = tight ? num[pos] - '0' : 9;
    long long ans = 0;

    if (!started) ans += solve(pos + 1, 10, 0, tight && !lim);

    for (int d = started ? 0 : 1; d <= lim; d++)
        if (!started || d != prev)
            ans += solve(pos + 1, d, 1, tight && d == lim);

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
    for (int i = 1; i < s.size(); i++) if (s[i] == s[i - 1]) return 0;
    return 1;
}

int main() {
    long long a, b;
    cin >> a >> b;
    cout << countUpTo(b) - countUpTo(a) + isValid(a);
}