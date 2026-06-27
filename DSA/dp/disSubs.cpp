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

const int MOD = 1e9 + 7;

class Solution {
public:
    string str;
    int size;
    vector<long long> memo;
    vector<int> last;

    long long countDistinctSubsequences(string s) {
        str = s;
        size = s.size();

        memo.assign(size + 1, 0); // dp[i] will store the count of distinct subsequences for the first i characters of the string s
        last.assign(26, -1); // last[c] will store the last index where character 'c' appeared in the string s. Initialized to -1 for all characters.

        memo[0] = 1;

        for (int i = 0; i < size; i++) {
            memo[i + 1] = (2 * memo[i]) % MOD;

            int c = str[i] - 'a';
            if (last[c] != -1) {
                memo[i + 1] = (memo[i + 1] - memo[last[c]] + MOD) % MOD;
            }

            last[c] = i;
        }

        return (memo[size] - 1 + MOD) % MOD;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    if (!(cin >> s)) return 0;

    Solution solver;
    cout << solver.countDistinctSubsequences(s) << '\n';

    return 0;
}