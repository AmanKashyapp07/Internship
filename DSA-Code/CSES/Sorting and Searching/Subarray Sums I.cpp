// Link: https://cses.fi/problemset/task/1660
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long x; cin >> n >> x;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    long long ans = 0, sum = 0;
    int l = 0;
    for (int r = 0; r < n; r++) {
        sum += a[r];
        while (sum > x) sum -= a[l++];
        if (sum == x) ans++;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count total subarrays with sum equal to x (positive elements) (CSES 1660).
// - Approach: Two Pointers / Variable Sliding Window ($O(N)$).
// - Intuition: Since all elements are positive, expanding right pointer increases sum and shrinking left pointer decreases sum monotonically.
// - Complexity: Time: O(N), Space: O(N).