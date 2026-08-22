#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int MOD = 1e9 + 7;

class Solution {
public:
    int countDistinctSubsequences(string s) {
    int n = s.size();
    vector<long long> dp(n);        // dp[i] = distinct subsequences for first i+1 chars
    vector<int> last(26, -1);       // last occurrence index of each character
    
    for (int i = 0; i < n; i++) {
        dp[i] = (i == 0 ? 1 : (2 * dp[i-1]) % MOD);  // empty + append/skip
        
        int c = s[i] - 'a';
        if (last[c] != -1) {
            dp[i] = (dp[i] - (last[c] == 0 ? 1 : dp[last[c]-1]) + MOD) % MOD;
        }
        
        last[c] = i;
    }

    return (dp[n-1] - 1 + MOD) % MOD;  // exclude empty subsequence
}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    
    Solution solver;
    cout << solver.countDistinctSubsequences(s) << '\n';
    
    return 0;
}