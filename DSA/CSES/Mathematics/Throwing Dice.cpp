// Link: https://cses.fi/problemset/task/1096

#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define vl vector<ll>
#define vvl vector<vl>

const ll MOD = 1e9 + 7;

//---------------------------------------------------------------
// Multiply two k x k matrices
//---------------------------------------------------------------
vvl multiply(const vvl &A, const vvl &B, int k) {
    vvl C(k, vl(k, 0));

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            for (int x = 0; x < k; x++) {
                C[i][j] = (C[i][j] + A[i][x] * B[x][j]) % MOD;
            }
        }
    }

    return C;
}

//---------------------------------------------------------------
// Computes A^p using Binary Exponentiation
//---------------------------------------------------------------
vvl power(vvl A, ll p, int k) {
    // Identity Matrix
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; i++)
        res[i][i] = 1;

    while (p) {
        if (p & 1)
            res = multiply(res, A, k);

        A = multiply(A, A, k);
        p >>= 1;
    }

    return res;
}

int main() {

    ll n;
    cin >> n;

    // dp[0] = 1 (empty sequence)
    if (n == 0) {
        cout << 1;
        return 0;
    }

    //---------------------------------------------------------------
    // Compute first 6 DP values normally
    //
    // dp[i] = dp[i-1] + dp[i-2] + ... + dp[i-6]
    //---------------------------------------------------------------
    vl dp(6, 0);
    dp[0] = 1;

    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 6 && j <= i; j++) {
            dp[i] = (dp[i] + dp[i - j]) % MOD;
        }
    }

    if (n <= 5) {
        cout << dp[n];
        return 0;
    }

    //---------------------------------------------------------------
    // State Vector:
    //
    // [ dp[i]   ]
    // [ dp[i-1] ]
    // [ dp[i-2] ]
    // [ dp[i-3] ]
    // [ dp[i-4] ]
    // [ dp[i-5] ]
    //
    // Transition:
    //
    // dp[i+1] = dp[i] + dp[i-1] + ... + dp[i-5]
    //
    // Therefore Transition Matrix is
    //
    // 1 1 1 1 1 1
    // 1 0 0 0 0 0
    // 0 1 0 0 0 0
    // 0 0 1 0 0 0
    // 0 0 0 1 0 0
    // 0 0 0 0 1 0
    //---------------------------------------------------------------
    vvl T(6, vl(6, 0));

    // First row = recurrence coefficients
    for (int j = 0; j < 6; j++)
        T[0][j] = 1;

    // Remaining rows simply shift the state down
    for (int i = 1; i < 6; i++)
        T[i][i - 1] = 1;

    //---------------------------------------------------------------
    // Base State corresponds to dp[5]
    //
    // [ dp5 ]
    // [ dp4 ]
    // [ dp3 ]
    // [ dp2 ]
    // [ dp1 ]
    // [ dp0 ]
    //---------------------------------------------------------------
    vl state = {
        dp[5],
        dp[4],
        dp[3],
        dp[2],
        dp[1],
        dp[0]
    };

    //---------------------------------------------------------------
    // We need dp[n].
    //
    // Base vector is at i = 5.
    // Therefore apply transition (n-5) times.
    //---------------------------------------------------------------
    vvl P = power(T, n - 5, 6);

    //---------------------------------------------------------------
    // New State = P * BaseState
    //
    // Only the first element is needed:
    //
    // dp[n] = first_row(P) * BaseState
    //---------------------------------------------------------------
    ll ans = 0;

    for (int j = 0; j < 6; j++) {
        ans = (ans + P[0][j] * state[j]) % MOD;
    }

    cout << ans;
}