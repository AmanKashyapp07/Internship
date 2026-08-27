// Link: https://cses.fi/problemset/task/1631
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    long long sum = 0, mx = 0;
    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        sum += x;
        mx = max(mx, x);
    }
    cout << max(2 * mx, sum) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum total time for two readers to read all n books without reading the same book simultaneously (CSES 1631).
// - Approach: Mathematical Max Criterion `max(2 * max_book, sum_books)`.
// - Intuition: If the largest book takes $> \text{sum} - \text{max}$, the bottleneck reader waiting for that single book forces time $2 \cdot \text{max}$; otherwise total sum is achieved.
// - Complexity: Time: O(N), Space: O(1).
