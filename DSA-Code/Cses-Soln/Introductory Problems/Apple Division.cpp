// Link: https://cses.fi/problemset/task/1623
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> p(n);
    long long totalSum = 0;
    for (int i = 0; i < n; i++) { cin >> p[i]; totalSum += p[i]; }

    long long minDiff = LLONG_MAX;
    for (int mask = 0; mask < (1 << n); mask++) {
        long long s1 = 0;
        for (int i = 0; i < n; i++) if (mask & (1 << i)) s1 += p[i];
        minDiff = min(minDiff, abs(totalSum - 2 * s1));
    }
    cout << minDiff << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Divide n apples into two groups such that weight difference is minimized (CSES 1623).
// - Approach: Bitmask Subset Sum Enumeration ($2^N$).
// - Intuition: Since n <= 20, iterate all subset masks to compute subset sum s1; difference = |totalSum - 2*s1|.
// - Complexity: Time: O(N \cdot 2^N), Space: O(N).