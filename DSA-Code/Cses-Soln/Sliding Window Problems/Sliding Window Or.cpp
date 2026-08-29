// Link: https://cses.fi/problemset/task/3405
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    long long x, a, b, c; cin >> x >> a >> b >> c;

    vector<long long> arr(n);
    arr[0] = x;
    for (int i = 1; i < n; i++) arr[i] = (a * arr[i - 1] + b) % c;

    long long ans = 0;
    for (int bit = 0; bit < 32; bit++) {
        int cnt = 0;
        for (int i = 0; i < k; i++) cnt += (arr[i] >> bit) & 1;
        if (cnt) ans ^= (1LL << bit);

        for (int i = k; i < n; i++) {
            cnt -= (arr[i - k] >> bit) & 1;
            cnt += (arr[i] >> bit) & 1;
            if (cnt) ans ^= (1LL << bit);
        }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute the XOR sum of bitwise OR results across all sliding windows of size k (CSES 3405).
// - Approach: Bitwise Independent Sliding Window Frequency Counting (32 bits).
// - Intuition: Bit b of window OR is 1 iff set bit count in window $> 0$; maintain set bit count per bit position across sliding window transitions.
// - Complexity: Time: O(32 \cdot N) = O(N), Space: O(N).