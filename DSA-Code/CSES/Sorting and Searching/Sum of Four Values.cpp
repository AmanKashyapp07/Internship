// Link: https://cses.fi/problemset/task/1642
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long x; cin >> n >> x;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    unordered_map<long long, pair<int, int>> pairSum;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long need = x - (a[i] + a[j]);
            if (pairSum.count(need)) {
                auto [p, q] = pairSum[need];
                if (p != i && p != j && q != i && q != j) {
                    cout << p + 1 << ' ' << q + 1 << ' ' << i + 1 << ' ' << j + 1 << '\n';
                    return 0;
                }
            }
        }
        for (int k = 0; k < i; k++) pairSum[a[k] + a[i]] = {k, i};
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find 4 distinct indices such that $a[i] + a[j] + a[k] + a[l] = x$ (CSES 1642).
// - Approach: Pair Sum Hash Map ($O(N^2)$ time).
// - Intuition: Iterating over pairs $(i, j)$ and matching against stored pair sums $(k, i)$ populated up to previous index avoids duplicate index overlap.
// - Complexity: Time: O(N^2), Space: O(N^2).