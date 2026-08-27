// Link: https://cses.fi/problemset/task/1085
#include <bits/stdc++.h>
using namespace std;

bool canDivide(const vector<long long>& a, int k, long long maxSum) {
    int parts = 1; long long cur = 0;
    for (long long x : a) {
        if (cur + x <= maxSum) cur += x;
        else { parts++; cur = x; }
    }
    return parts <= k;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<long long> a(n);
    long long low = 0, high = 0;
    for (int i = 0; i < n; i++) { cin >> a[i]; low = max(low, a[i]); high += a[i]; }

    long long ans = high;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (canDivide(a, k, mid)) { ans = mid; high = mid - 1; }
        else low = mid + 1;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Divide an array into k contiguous subarrays minimizing the maximum subarray sum (CSES 1085).
// - Approach: Binary Search on Answer predicate `canDivide(maxSum)`.
// - Intuition: The minimum possible maximum sum ranges between $\max(a_i)$ and $\sum a_i$; feasibility checking runs in linear $O(N)$ time.
// - Complexity: Time: O(N \log(\sum A_i)), Space: O(N).