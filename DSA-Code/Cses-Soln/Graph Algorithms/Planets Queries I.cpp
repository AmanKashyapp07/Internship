// Link: https://cses.fi/problemset/task/1750
#include <bits/stdc++.h>
using namespace std;

const int LOG = 30;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<vector<int>> up(n + 1, vector<int>(LOG));
    for (int i = 1; i <= n; i++) cin >> up[i][0];

    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) up[i][j] = up[up[i][j - 1]][j - 1];
    }

    while (q--) {
        int x; long long k; cin >> x >> k;
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) x = up[x][j];
        }
        cout << x << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find destination planet from starting planet x after k teleports in a functional graph (CSES 1750).
// - Approach: Binary Lifting Precomputation (`up[i][j] = up[up[i][j-1]][j-1]`).
// - Intuition: Precomputing powers of 2 jumps enables answering queries for $k \le 10^9$ in $O(\log k)$ time.
// - Complexity: Time: O(N \log K + Q \log K), Space: O(N \log K).