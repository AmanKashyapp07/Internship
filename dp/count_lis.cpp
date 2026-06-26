/**
 * LeetCode 673 - Number of Longest Increasing Subsequence
 *
 * Description:
 * Given an integer array nums, return the number of longest increasing subsequences.
 *
 * Approach:
 * - Dynamic programming with two arrays:
 *   - `len[i]` stores the length of LIS ending at index `i`.
 *   - `count[i]` stores the number of LIS of length `len[i]` ending at index `i`.
 * - For each element `nums[i]`, iterate through predecessors `nums[j]`:
 *   - If `nums[j] < nums[i]`:
 *     - If `len[j] + 1 > len[i]`, update `len[i] = len[j] + 1` and reset `count[i] = count[j]`.
 *     - If `len[j] + 1 == len[i]`, add `count[j]` to `count[i]`.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];
    vector<int> len(n, 1); vector<long long> cnt(n, 1);
    int maxLen = 1;
    // DP state: len[i] is the length of LIS ending at i, cnt[i] is the count of such LIS
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1; cnt[i] = cnt[j];
                } else if (len[j] + 1 == len[i]) {
                    cnt[i] += cnt[j];
                }
            }
        }
        maxLen = max(maxLen, len[i]);
    }
    long long ans = 0;
    for (int i = 0; i < n; i++) if (len[i] == maxLen) ans += cnt[i];
    cout << ans << '\n';
}