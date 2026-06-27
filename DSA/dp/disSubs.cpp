/**
 * CSES Problem Set
 * 
 * Problem: Distinct Subsequences (String)
 * Link: https://cses.fi/problemset/task/1145
 * Here, distinct subsequences mean that we count each unique subsequence only once, even if it can be formed in multiple ways from the original string. For example, for the string "aaa", the distinct subsequences are "", "a", "aa", and "aaa", totaling 4 distinct subsequences.
 * Category: String Algorithms
 * 
 * Description:
 * Count distinct subsequences of a string modulo 10^9+7.
 * 
 * Logic/Approach:
 * DP state transition with last occurrence subtractor to handle duplicates.
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    const int MOD = 1e9 + 7;
    int n = s.size();

    vector<long long> dp(n + 1); // dp[i] will store the count of distinct subsequences for the first i characters of the string s
    vector<int> last(26, -1); // last[c] will store the last index where character 'c' appeared in the string s. Initialized to -1 for all characters.

    dp[0] = 1;

    for (int i = 0; i < n; i++) {
        dp[i + 1] = (2 * dp[i]) % MOD;

        int c = s[i] - 'a';
        if (last[c] != -1) dp[i + 1] = (dp[i + 1] - dp[last[c]] + MOD) % MOD;

        last[c] = i;
    }

    cout << (dp[n] - 1 + MOD) % MOD << '\n';
}