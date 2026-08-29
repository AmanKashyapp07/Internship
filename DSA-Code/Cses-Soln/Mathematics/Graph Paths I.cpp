// Link: https://cses.fi/problemset/task/1723
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
using Matrix = vector<vector<long long>>;

Matrix mul(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            if (A[i][k]) for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
    return C;
}

Matrix power(Matrix A, long long p) {
    int n = A.size();
    Matrix res(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) res[i][i] = 1;
    for (; p; p >>= 1) {
        if (p & 1) res = mul(res, A);
        A = mul(A, A);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; long long k; cin >> n >> m >> k;
    Matrix adj(n, vector<long long>(n, 0));
    while (m--) { int u, v; cin >> u >> v; u--; v--; adj[u][v]++; }
    Matrix ans = power(adj, k);
    cout << ans[0][n - 1] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count paths of length k from node 1 to node n in a directed graph modulo 10^9+7 (CSES 1723).
// - Approach: Matrix Exponentiation on Adjacency Matrix.
// - Intuition: (A^k)[i][j] represents the number of paths of length k from vertex i to vertex j.
// - Complexity: Time: O(N^3 log K), Space: O(N^2).