/**
 * Problem: Number of Longest Increasing Subsequences (https://leetcode.com/problems/number-of-longest-increasing-subsequence/)
 * Find the total number of longest increasing subsequences in an array.
 * Time: O(N^2) time, O(N) space.
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