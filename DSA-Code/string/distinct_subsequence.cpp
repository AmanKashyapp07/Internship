/**
 * Problem: Count Distinct Subsequences
 * 
 * Given a string s, count the number of distinct subsequences of s
 * (excluding the empty subsequence). Return the result modulo 10^9 + 7.
 * 
 * Approach: Dynamic Programming with last occurrence tracking
 * - dp[i] = number of distinct subsequences using first i characters
 * - When adding s[i-1], we can append it to all existing subsequences (double)
 * - Subtract duplicates caused by previous occurrence of the same character
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    int n = s.length();
    const long long MOD = 1000000007;

    // dp[i]: count of distinct subsequences from first i characters (1 based index)
    vector<long long> dp(n + 1, 0);
    dp[0] = 1;  // empty subsequence

    // last[ch]: last index where character ch appeared (1-based)
    vector<int> last(26, 0);

    for (int i = 1; i <= n; ++i) {
        char ch = s[i - 1];
        int idx = ch - 'a';

        // Double the previous count (append or not append current char)
        dp[i] = (2 * dp[i - 1]) % MOD;

        // Remove duplicate subsequences from previous occurrence
        if (last[idx] != 0) {
            dp[i] = (dp[i] - dp[last[idx] - 1] + MOD) % MOD; // why last[idx] - 1? Because last[idx] is 1-based index, we want to subtract the count of subsequences before the last occurrence
        }

        // Update last occurrence
        last[idx] = i;
    }

    // Subtract 1 to exclude the empty subsequence
    long long ans = (dp[n] - 1 + MOD) % MOD;
    cout << ans << "\n";

    return 0;
}