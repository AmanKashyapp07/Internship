// Link: https://cses.fi/problemset/task/1096
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
using Matrix = vector<vector<long long>>;

Matrix mul(const Matrix& A, const Matrix& B) {
    Matrix C(6, vector<long long>(6, 0));
    for (int i = 0; i < 6; i++)
        for (int k = 0; k < 6; k++)
            if (A[i][k]) for (int j = 0; j < 6; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
    return C;
}

Matrix power(Matrix A, long long p) {
    Matrix res(6, vector<long long>(6, 0));
    for (int i = 0; i < 6; i++) res[i][i] = 1;
    for (; p; p >>= 1) {
        if (p & 1) res = mul(res, A);
        A = mul(A, A);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    if (n == 0) { cout << 1; return 0; }

    vector<long long> dp(6, 0); dp[0] = 1;
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= 6 && j <= i; j++)
            dp[i] = (dp[i] + dp[i - j]) % MOD;

    if (n <= 5) { cout << dp[n]; return 0; }

    Matrix T(6, vector<long long>(6, 0));
    for (int j = 0; j < 6; j++) T[0][j] = 1;
    for (int i = 1; i < 6; i++) T[i][i - 1] = 1;

    Matrix P = power(T, n - 5);
    vector<long long> state = {dp[5], dp[4], dp[3], dp[2], dp[1], dp[0]};

    long long ans = 0;
    for (int j = 0; j < 6; j++) ans = (ans + P[0][j] * state[j]) % MOD;
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count ways to reach sum n by throwing a 6-sided die modulo 10^9+7 (CSES 1096).
// - Approach: 6x6 Matrix Exponentiation on Linear Recurrence dp[n] = Σ dp[n-j] for j=1..6.
// - Intuition: Transition matrix shifts previous 6 states and calculates new state as sum; binary exponentiation evaluates n-th term in O(6^3 log N).
// - Complexity: Time: O(6^3 log N), Space: O(6^2).