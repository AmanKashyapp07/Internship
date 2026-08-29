// Link: https://cses.fi/problemset/task/3361
#include <bits/stdc++.h>
using namespace std;

bool check(double average, const vector<long long>& prefixA, const vector<long long>& prefixB, int& bestPrefixA, int& bestPrefixB) {
    int n = prefixA.size() - 1;
    double bestValueA = -1e100, bestValueB = -1e100;
    for (int i = 1; i <= n; i++) {
        double val = prefixA[i] - average * i;
        if (val > bestValueA) { bestValueA = val; bestPrefixA = i; }
    }
    for (int i = 1; i <= n; i++) {
        double val = prefixB[i] - average * i;
        if (val > bestValueB) { bestValueB = val; bestPrefixB = i; }
    }
    return bestValueA + bestValueB >= 0;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> prefixA(n + 1, 0), prefixB(n + 1, 0);
    for (int i = 1; i <= n; i++) { long long x; cin >> x; prefixA[i] = prefixA[i - 1] + x; }
    for (int i = 1; i <= n; i++) { long long x; cin >> x; prefixB[i] = prefixB[i - 1] + x; }

    int bestPrefixA = 1, bestPrefixB = 1;
    double low = 0, high = 1e9;
    for (int iter = 0; iter < 70; iter++) {
        double mid = (low + high) / 2;
        if (check(mid, prefixA, prefixB, bestPrefixA, bestPrefixB)) low = mid;
        else high = mid;
    }
    cout << bestPrefixA << " " << bestPrefixB << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find prefix lengths lenA and lenB from arrays A and B maximizing combined average `(sumA + sumB) / (lenA + lenB)` (CSES 3361).
// - Approach: Binary Search on Real Numbers (0..1e9) with Predicate Reformulation.
// - Intuition: `(sumA + sumB) / (lenA + lenB) >= avg` $\iff$ `(sumA - avg * lenA) + (sumB - avg * lenB) >= 0`; max values for A and B decouple into independent scans.
// - Complexity: Time: O(70 \cdot N) = O(N), Space: O(N).