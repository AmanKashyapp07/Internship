// Link: https://cses.fi/problemset/task/2422
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    long long targetRank = (n * n + 1) / 2;
    long long low = 1, high = n * n, ans = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        long long count = 0;
        for (long long i = 1; i <= n; i++) {
            count += min(n, mid / i);
        }
        if (count >= targetRank) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the median value in an n x n multiplication table where cell (i, j) = i * j (CSES 2422).
// - Approach: Binary Search on Answer over range [1, N^2].
// - Intuition: Number of elements <= mid in row i is `min(n, mid / i)`; binary search finds smallest value with count >= (N^2 + 1)/2.
// - Complexity: Time: O(N \log(N^2)) = O(N \log N), Space: O(1).