// Link: https://cses.fi/problemset/task/1149
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; if (!(cin >> s)) return 0;
    int n = s.size();
    vector<long long> dp(n + 1, 0);
    dp[0] = 1;
    vector<int> last(26, 0);

    for (int i = 1; i <= n; i++) {
        int c = s[i - 1] - 'a';
        dp[i] = (2 * dp[i - 1]) % MOD;
        if (last[c] != 0) {
            dp[i] = (dp[i] - dp[last[c] - 1] + MOD) % MOD;
        }
        last[c] = i;
    }
    cout << (dp[n] - 1 + MOD) % MOD << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count the number of non-empty distinct subsequences of string s modulo 10^9+7 (CSES 1149).
// - Approach: Dynamic Programming with Last Sighting Deduplication (`dp[i] = 2 * dp[i-1] - dp[last[c]-1]`).
// - Intuition: Appending s[i] doubles existing choice count; subtracting choice count before its last occurrence eliminates duplicate subsequences.
// - Complexity: Time: O(N), Space: O(N).