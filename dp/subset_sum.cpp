/**
 * Problem: Two Sets II (https://cses.fi/problemset/task/1093)
 * Count the number of ways to partition numbers 1..n into two sets of equal sum.
 * Time: O(N * Target) time, O(Target) space.
 */
#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1e9 + 7, INV2 = 500000004; // INV2 is modular inverse of 2 modulo MOD

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (cin >> n) {
        long long total = 1LL * n * (n + 1) / 2;
        if (total % 2 != 0) { cout << 0 << '\n'; return 0; }
        int target = total / 2;
        vector<int> dp(target + 1, 0); dp[0] = 1;
        // Knapsack DP: dp[s] stores the count of subsets summing to s
        for (int x = 1; x <= n; x++) {
            for (int s = target; s >= x; s--) dp[s] = (dp[s] + dp[s - x]) % MOD;
        }
        cout << 1LL * dp[target] * INV2 % MOD << '\n'; // Divide by 2 because partitions are symmetric pairs
    }
}