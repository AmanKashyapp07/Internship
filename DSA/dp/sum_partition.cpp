/**
 * LeetCode 1043 - Partition Array for Maximum Sum
 *
 * Description:
 * Given an integer array arr, partition the array into (contiguous) subarrays of length at most k.
 * After partitioning, each subarray has their values changed to become the maximum value of that subarray.
 * Return the largest sum of the given array after partitioning.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - Let `solve(i)` represent the maximum partition sum for suffix `[0...i]`.
 * - Transition: For each length `len` from 1 to `k`, partition the last subarray ending at `i`.
 *   - `solve(i) = max(solve(j - 1) + max_ele * (i - j + 1))` where `j` ranges from `i` down to `i - k + 1`.
 *
 * Time Complexity: O(n * k)
 * Space Complexity: O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Solution {
public:
    vector<int> input;
    int limit;
    vector<int> memo;

    int solve(int i) {
        if (i < 0) return 0;
        if (memo[i] != -1) return memo[i];

        int ans = 0;
        int maxEle = 0;
        int cnt = limit;
        for (int j = i; j >= 0 && cnt > 0; j--) {
            maxEle = max(maxEle, input[j]);
            ans = max(ans, solve(j - 1) + maxEle * (i - j + 1));
            cnt--;
        }
        return memo[i] = ans;
    }

    int maxSumAfterPartitioning(vector<int>& arr, int k) {
        input = arr;
        limit = k;
        int n = arr.size();
        memo.assign(n, -1);
        return solve(n - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k, n;
    if (!(cin >> k >> n)) return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    Solution solver;
    cout << solver.maxSumAfterPartitioning(arr, k) << "\n";

    return 0;
}
