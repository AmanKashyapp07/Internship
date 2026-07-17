// CSES Problem: <problem name>
// https://cses.fi/problemset/task/<id>

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
using namespace std;

using int64 = long long;

const long long MOD = 1e9 + 7;
const long long PHI = MOD - 1;   // Fermat exponent modulus

long long modPow(long long a, long long b, long long mod) {
    long long res = 1;

    while (b) {
        if (b & 1)
            res = (__int128)res * a % mod;

        a = (__int128)a * a % mod;
        b >>= 1;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    long long divisorCount = 1;      // answer 1
    long long divisorSum = 1;        // answer 2
    long long divisorProduct = 1;    // answer 3

    long long divisorCountMod = 1;   // divisor count modulo (MOD-1)

    for (int i = 0; i < n; i++) {
        long long p, k;
        cin >> p >> k;

        // -----------------------------
        // Number of divisors
        // d(N) *= (k + 1)
        // -----------------------------
        divisorCount =
            divisorCount * ((k + 1) % MOD) % MOD;

        // -----------------------------
        // Sum of divisors
        // (p^(k+1)-1)/(p-1)
        // -----------------------------
        long long numerator =
            (modPow(p, k + 1, MOD) - 1 + MOD) % MOD;

        long long denominatorInv =
            modPow(p - 1, MOD - 2, MOD);

        long long geometricSum =
            numerator * denominatorInv % MOD;

        divisorSum =
            divisorSum * geometricSum % MOD;

        // -------------------------------------------------
        // Product of divisors
        //
        // If current divisor product is P
        // and current divisor count is D,
        // after adding p^k: where k is the exponent of prime p
        //
        // P_new = P^(k+1)
        //         * p^( D * k*(k+1)/2 )
        //
        // Exponents are taken modulo MOD-1
        // (Fermat's theorem)
        // -------------------------------------------------

        long long triangle;

        if (k % 2 == 0)
            triangle = ((k / 2) % PHI) * ((k + 1) % PHI) % PHI;
        else
            triangle = (k % PHI) * (((k + 1) / 2) % PHI) % PHI;

        long long exponent =
            divisorCountMod * triangle % PHI;

        divisorProduct =
            modPow(divisorProduct, k + 1, MOD) *
            modPow(p, exponent, MOD) % MOD;

        // Update divisor count modulo MOD-1
        // for future exponent calculations
        divisorCountMod =
            divisorCountMod * ((k + 1) % PHI) % PHI;
    }

    cout << divisorCount << " "
         << divisorSum << " "
         << divisorProduct << '\n';

    return 0;
}