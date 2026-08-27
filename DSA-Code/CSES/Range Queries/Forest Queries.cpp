// Link: https://cses.fi/problemset/task/1652
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<vector<int>> pref(n + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= n; i++) {
        string s; cin >> s;
        for (int j = 1; j <= n; j++) {
            pref[i][j] = (s[j-1]=='*') + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1];
        }
    }
    while (q--) {
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        cout << pref[r2][c2] - pref[r1-1][c2] - pref[r2][c1-1] + pref[r1-1][c1-1] << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count trees ('*') in rectangular subgrids of an n x n forest (CSES 1652).
// - Approach: 2D Prefix Sum (inclusion-exclusion).
// - Intuition: `pref[r2][c2] - pref[r1-1][c2] - pref[r2][c1-1] + pref[r1-1][c1-1]` counts '*' in O(1) per query.
// - Complexity: Time: O(N^2 + Q), Space: O(N^2).