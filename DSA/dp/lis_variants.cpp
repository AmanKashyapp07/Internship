/**
 * Problem: Longest Increasing Subsequence Variants
 * Reference implementations for LIS: O(N^2), path reconstruction, O(N log N) tails, and memoization.
 * Time: O(N log N) for tails, O(N^2) for standard. Space: O(N^2) for memoization, O(N) for others.
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 1. DP O(N^2)
int lis_n2(const vector<int>& arr) {
    int n = arr.size(); if (n == 0) return 0;
    vector<int> dp(n, 1); int max_len = 1;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) if (arr[i] > arr[j]) dp[i] = max(dp[i], dp[j] + 1);
        max_len = max(max_len, dp[i]);
    }
    return max_len;
}

// 2. DP O(N^2) + Path Reconstruction
vector<int> lis_n2_reconstruct(const vector<int>& arr) {
    int n = arr.size(); if (n == 0) return {};
    vector<int> dp(n, 1), parent(n, -1); int max_len = 1, end_idx = 0;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j] && dp[j] + 1 > dp[i]) { dp[i] = dp[j] + 1; parent[i] = j; }
        }
        if (dp[i] > max_len) { max_len = dp[i]; end_idx = i; }
    }
    vector<int> lis;
    for (int curr = end_idx; curr != -1; curr = parent[curr]) lis.push_back(arr[curr]);
    reverse(lis.begin(), lis.end());
    return lis;
}

// 3. Tail Array O(N log N) patience sorting
int lis_nlogn(const vector<int>& arr) {
    vector<int> tails;
    for (int x : arr) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}

// 4. Memoized Recursion: state represented by dp[idx][prev + 1] to handle prev = -1 offset
int lis_memo(int idx, int prev, const vector<int>& arr, vector<vector<int>>& dp) {
    if (idx == arr.size()) return 0;
    if (dp[idx][prev + 1] != -1) return dp[idx][prev + 1];
    int exclude = lis_memo(idx + 1, prev, arr, dp), include = 0;
    if (prev == -1 || arr[idx] > arr[prev]) include = 1 + lis_memo(idx + 1, idx, arr, dp);
    return dp[idx][prev + 1] = max(include, exclude);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "O(N^2) Length: " << lis_n2(arr) << "\n";
    cout << "Reconstructed LIS:";
    for (int x : lis_n2_reconstruct(arr)) cout << " " << x;
    cout << "\nO(N log N) Length: " << lis_nlogn(arr) << "\n";
    vector<vector<int>> dp(arr.size(), vector<int>(arr.size() + 1, -1));
    cout << "Length of LIS (Memoization): " << lis_memo(0, -1, arr, dp) << '\n';
}
