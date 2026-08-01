#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cstring>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vvl = vector<vector<int>>;
using vvl = vector<vector<ll>>;
using vi = vector<int>;
using vl = vector<ll>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;


// Multiply two k x k matrices
vvl multiply(const vvl& A, const vvl& B, int k) {
    vvl C(k, vl(k, 0));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int m = 0; m < k; ++m) {
                C[i][j] = (C[i][j] + A[i][m] * B[m][j]) % MOD;
            }
        }
    }
    return C;
}

// Compute (A^p) % MOD in O(k^3 log p)
vvl power(vvl A, long long p, int k) {
    // Initialize identity matrix
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; ++i) res[i][i] = 1;

    while (p > 0) {
        if (p & 1) res = multiply(res, A, k);
        A = multiply(A, A, k);
        p >>= 1;
    }
    return res;
} // here k is the size of the square matrix A and p is the exponent. The function returns A raised to the power p modulo MOD.

ll countPathOfLengthK(vvl& adj, int src, int dest, long long k) {
    int n = adj.size();
    vvl res = power(adj, k, n);
    return res[src][dest];
} // this function counts the number of paths of length k from src to dest in a directed graph represented by its adjacency matrix adj. The result is returned modulo MOD.

int calculateNthFiboacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    vvl F = {{1, 1}, {1, 0}};
    vvl res = power(F, n - 1, 2);
    return res[0][0];
} // this function calculates the nth Fibonacci number using matrix exponentiation. The result is returned modulo MOD.