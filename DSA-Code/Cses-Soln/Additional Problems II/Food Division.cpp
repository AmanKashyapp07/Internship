// Link: https://cses.fi/problemset/task/1189
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<ll> a(n), b(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];

    vector<ll> p(n);
    ll current_diff = 0;
    for (int i = 0; i < n; ++i) {
        current_diff += (a[i] - b[i]);
        p[i] = current_diff;
    }

    vector<ll> neg_p(n);
    for (int i = 0; i < n; ++i) neg_p[i] = -p[i];
    sort(neg_p.begin(), neg_p.end());

    ll median = neg_p[n / 2];
    ll total_moves = 0;
    for (int i = 0; i < n; ++i) total_moves += abs(-p[i] - median);

    cout << total_moves << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum food transfers between adjacent children in a circle to achieve target distribution (CSES 1189).
// - Approach: Circular Prefix Differences + 1D Median Optimization.
// - Intuition: Total moves reduce to sum |x_n + P[i]| where x_n is the circular transfer variable; cost is minimized when x_n is set to the median of -P[i].
// - Complexity: Time: O(N log N), Space: O(N).
