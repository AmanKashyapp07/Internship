// Link: https://cses.fi/problemset/task/1074
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a.begin(), a.end());

    long long median = a[n / 2], cost = 0;
    for (long long x : a) cost += abs(x - median);

    cout << cost << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Equalize stick lengths with minimum total cost (CSES 1074).
// - Approach: Median Equalization.
// - Intuition: The median of a sorted array minimizes total absolute differences $\sum |a_i - \text{median}|$.
// - Complexity: Time: O(N \log N), Space: O(N).