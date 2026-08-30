// Link: https://cses.fi/problemset/task/3190
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    // dp[i] = number of valid partitions of first i elements
    // prefix[i] = dp[0] + ... + dp[i]
    vector<long long> dp(n + 1, 0), prefix(n + 1, 0);
    dp[0] = 1;
    prefix[0] = 1;

    unordered_map<int, int> last;
    int left = 1;

    for (int right = 1; right <= n; right++) {
        if (last.count(a[right])) left = max(left, last[a[right]] + 1);
        last[a[right]] = right;

        // Last segment starts at k in [left, right]: dp[right] = sum(dp[left-1...right-1])
        dp[right] = prefix[right - 1];
        if (left > 1) dp[right] = (dp[right] - prefix[left - 2] + MOD) % MOD;
        prefix[right] = (prefix[right - 1] + dp[right]) % MOD;
    }

    cout << dp[n] << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count ways to partition array into contiguous subarrays where each subarray has distinct elements modulo 10^9+7 (CSES 3190).
// - Approach: Sliding Window Two Pointers + Prefix-Sum Optimized 1D DP.
// - Intuition: Maintain earliest valid start index `left` using last-seen map; dp[right] sums valid split transitions in O(1) via prefix sum subtraction.
// - Complexity: Time: O(N), Space: O(N).
