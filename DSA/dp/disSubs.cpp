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
    string s_;
    int n_;
    vector<long long> dp_;
    vector<int> last_;

    long long countDistinctSubsequences(string s) {
        s_ = s;
        n_ = s.size();

        dp_.assign(n_ + 1, 0); // dp[i] will store the count of distinct subsequences for the first i characters of the string s
        last_.assign(26, -1); // last[c] will store the last index where character 'c' appeared in the string s. Initialized to -1 for all characters.

        dp_[0] = 1;

        for (int i = 0; i < n_; i++) {
            dp_[i + 1] = (2 * dp_[i]) % MOD;

            int c = s_[i] - 'a';
            if (last_[c] != -1) {
                dp_[i + 1] = (dp_[i + 1] - dp_[last_[c]] + MOD) % MOD;
            }

            last_[c] = i;
        }

        return (dp_[n_] - 1 + MOD) % MOD;
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