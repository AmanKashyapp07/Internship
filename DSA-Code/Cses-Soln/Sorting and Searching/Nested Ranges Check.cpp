// Link: https://cses.fi/problemset/task/2168
#include <bits/stdc++.h>
using namespace std;

struct Range { int l, r, idx; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<Range> ranges(n);
    for (int i = 0; i < n; i++) { cin >> ranges[i].l >> ranges[i].r; ranges[i].idx = i; }

    sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
        if (a.l == b.l) return a.r > b.r;
        return a.l < b.l;
    });

    vector<int> contains(n, 0), contained(n, 0);

    int maxRight = ranges[0].r;
    for (int i = 1; i < n; i++) {
        if (ranges[i].r <= maxRight) contained[ranges[i].idx] = 1;
        maxRight = max(maxRight, ranges[i].r);
    }

    int minRight = ranges[n - 1].r;
    for (int i = n - 2; i >= 0; i--) {
        if (ranges[i].r >= minRight) contains[ranges[i].idx] = 1;
        minRight = min(minRight, ranges[i].r);
    }

    for (int x : contains) cout << x << ' ';
    cout << '\n';
    for (int x : contained) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each range, determine if it contains another range and if it is contained by another range (CSES 2168).
// - Approach: Sorting by Left Bound (Ascending) & Right Bound (Descending) + Running Min/Max Sweeps.
// - Intuition: Sorting guarantees left bound order; running max of right bounds from left identifies contained ranges, running min from right identifies contains ranges.
// - Complexity: Time: O(N \log N), Space: O(N).