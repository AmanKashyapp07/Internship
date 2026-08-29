// Link: https://cses.fi/problemset/task/1687
#include <bits/stdc++.h>
using namespace std;

const int LOG = 20;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<vector<int>> up(n + 1, vector<int>(LOG, -1));
    for (int i = 2; i <= n; i++) cin >> up[i][0];

    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            if (up[i][j - 1] != -1) up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }

    while (q--) {
        int x, k; cin >> x >> k;
        for (int j = 0; j < LOG; j++) {
            if ((k >> j) & 1) {
                x = (x == -1) ? -1 : up[x][j];
            }
        }
        cout << x << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the k-th ancestor of node x in a rooted tree for q queries (CSES 1687).
// - Approach: Binary Lifting Precomputation (`up[u][j] = up[up[u][j-1]][j-1]`).
// - Intuition: Decomposing k into binary bits allows jumping $2^j$ steps up the tree in $O(\log k)$ time per query.
// - Complexity: Time: O(N \log N + Q \log N), Space: O(N \log N).