class Solution {
public:
    int peopleAwareOfSecret(int n, int delay, int forget) {
        const int MOD = 1e9 + 7;

        vector<long long> dp(n + 1, 0);
        vector<long long> diff(n + 2, 0);

        dp[1] = 1;

        if (1 + delay <= n) diff[1 + delay] = 1;
        if (1 + forget <= n) diff[1 + forget] = MOD - 1; // -1 mod MOD

        long long prefSum = 0;

        for (int i = 2; i <= n; i++) {
            prefSum = (prefSum + diff[i]) % MOD;
            dp[i] = prefSum;

            if (i + delay <= n)
                diff[i + delay] = (diff[i + delay] + dp[i]) % MOD;

            if (i + forget <= n)
                diff[i + forget] = (diff[i + forget] + MOD - dp[i]) % MOD;
        }

        long long ans = 0;
        for (int i = max(1, n - forget + 1); i <= n; i++)
            ans = (ans + dp[i]) % MOD;

        return ans;
    }
};