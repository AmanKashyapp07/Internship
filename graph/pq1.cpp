/**
 * CSES 1750 - Planets Queries I
 *
 * Description:
 * You are given a functional graph (each planet has one teleporter). Respond to queries:
 * starting from planet x, where will you be after k teleportations?
 *
 * Approach:
 * - Solve using Binary Lifting.
 * - Compute `up[i][j]`, which stores the planet reached from planet `i` after `2^j` steps.
 * - Transition: `up[i][j] = up[up[i][j-1]][j-1]`.
 * - Answer each query in O(log(k)) by decomposing k into powers of 2.
 *
 * Time Complexity: O((N + Q) * log(K))
 * Space Complexity: O(N * log(K))
 */

#include <iostream>
#include <vector>

using namespace std;

const int MAX_N = 200005;
const int LOG = 30;   // 2^30 > 1e9

int up[MAX_N][LOG];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    // Read direct teleporter
    for (int i = 1; i <= n; i++) {
        cin >> up[i][0];
    }

    // Binary lifting table
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            up[i][j] = up[ up[i][j - 1] ][j - 1];
        }
    }

    while (q--) {
        int x;
        long long k;
        cin >> x >> k;

        for (int j = 0; j < LOG; j++) {
            if (k & (1LL << j)) {
                x = up[x][j];
            }
        }

        cout << x << '\n';
    }

    return 0;
}