// Link: https://cses.fi/problemset/task/3355
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    const int MAX_N = 10000000;
    const int LIM = sqrt(MAX_N);
    vector<pair<int, int>> twoSq(MAX_N + 1, {-1, -1});

    for (int a = 0; a <= LIM; a++) {
        int a2 = a * a;
        for (int b = a; b <= LIM; b++) {
            int s = a2 + b * b;
            if (s > MAX_N) break;
            if (twoSq[s].first == -1) twoSq[s] = {a, b};
        }
    }

    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        for (int s = 0; s <= n; s++) {
            if (twoSq[s].first != -1 && twoSq[n - s].first != -1) {
                auto [a, b] = twoSq[s];
                auto [c, d] = twoSq[n - s];
                cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
                break;
            }
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Express n as sum of 4 squares: a^2 + b^2 + c^2 + d^2 (Lagrange's Four-Square Theorem) (CSES 3355).
// - Approach: Precomputed Two-Square Lookup Table + Meet-in-the-middle splitting into n = s + (n-s).
// - Intuition: Lagrange's theorem guarantees every positive integer is sum of 4 squares; precomputing 2-square sums allows constant-time checks for pair combinations.
// - Complexity: Time: O(MAX_N + T \cdot N), Space: O(MAX_N).