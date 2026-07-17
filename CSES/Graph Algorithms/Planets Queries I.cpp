/**
 * CSES - Planets Queries I
 *
 * Given a functional graph, answer:
 * Starting from planet x, where do we arrive after k teleportations?
 * functional graph: each node has exactly one outgoing edge, so we are using binary lifting to jump in powers of 2.
 * Binary Lifting:
 * up[i][j] = node reached after 2^j moves from i
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