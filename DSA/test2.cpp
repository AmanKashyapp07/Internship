#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;

const int MOD = 1e9 + 7;

struct MatrixExponentiation {

    // Matrix multiplication
    //
    // C = A × B
    //
    // C[i][j] = Σ A[i][k] * B[k][j]
    //
    // Graph interpretation:
    // A[i][k] = #paths from i -> k
    // B[k][j] = #paths from k -> j
    //
    // Therefore,
    // C[i][j] = Σ (#ways i->k) × (#ways k->j)
    //         = total #ways from i -> j by joining paths through every k.
    static vvi multiply(const vvi &A, const vvi &B) {
        int n = A.size();
        int m = B[0].size();
        int p = B.size();

        vvi C(n, vi(m, 0));

        for (int i = 0; i < n; i++) {
            for (int k = 0; k < p; k++) {
                if (A[i][k] == 0) continue;

                for (int j = 0; j < m; j++) {
                    C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % MOD;
                }
            }
        }

        return C;
    }

    // Computes A^b
    //
    // If A is the adjacency matrix,
    //
    // A^1 -> paths of length 1
    // A^2 -> paths of length 2
    // A^3 -> paths of length 3
    // ...
    // A^k -> paths of length k
    //
    // Thus (A^k)[i][j] gives the number of paths
    // from i to j using exactly k edges.
    static vvi power(vvi A, long long b) {
        int n = A.size();

        // Identity matrix
        // A^0 = I
        vvi res(n, vi(n, 0));
        for (int i = 0; i < n; i++)
            res[i][i] = 1;

        while (b) {
            if (b & 1)
                res = multiply(res, A);

            A = multiply(A, A);
            b >>= 1;
        }

        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    long long k;
    cin >> n >> m >> k;

    // Adjacency matrix
    //
    // adj[i][j] = number of direct edges from i -> j.
    //
    // Multiple edges are allowed, so we increment.
    vvi adj(n, vi(n, 0));

    while (m--) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        adj[u][v] = (adj[u][v] + 1) % MOD;
    }

    // ans = A^k
    vvi ans = MatrixExponentiation::power(adj, k);

    // (A^k)[0][n-1]
    // = number of paths from node 1 to node n
    // using exactly k edges.
    cout << ans[0][n - 1] << '\n';

    return 0;
}