/**
 * Problem: Mathematics Template
 * Link: N/A
 * Category: Mathematics
 * 
 * Description:
 * Helper functions for modular arithmetic, primes, factorizations, combinatorics, and matrices.
 * 
 * Logic/Approach:
 * Fast competitive programming mathematics template.
 */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

#define int long long

const int MOD = 1e9 + 7;

// ============================================================
// Binary Exponentiation
// ============================================================

int power(int a, int b, int mod = MOD) {
    int res = 1;
    a %= mod;

    while (b) {
        if (b & 1)
            res = res * a % mod;

        a = a * a % mod;
        b >>= 1;
    }

    return res;
}

// ============================================================
// Modular Inverse (Prime Modulus)
// ============================================================

int modInverse(int x, int mod = MOD) {
    return power(x, mod - 2, mod);
}

// ============================================================
// GCD
// ============================================================

int gcdll(int a, int b) {
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// ============================================================
// LCM
// ============================================================

int lcmll(int a, int b) {
    return a / gcdll(a, b) * b;
}

// ============================================================
// Prime Check
// ============================================================

bool isPrime(int n) {
    if (n < 2) return false;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }

    return true;
}

// ============================================================
// Sieve of Eratosthenes
// Returns all primes <= n
// ============================================================

vector<int> sieve(int n) {

    vector<bool> prime(n + 1, true);
    prime[0] = prime[1] = false;

    for (int i = 2; i * i <= n; i++) {
        if (prime[i]) {
            for (int j = i * i; j <= n; j += i)
                prime[j] = false;
        }
    }

    vector<int> primes;

    for (int i = 2; i <= n; i++) {
        if (prime[i])
            primes.push_back(i);
    }

    return primes;
}

// ============================================================
// Prime Factorization
// Returns {prime, exponent}
// ============================================================

vector<pair<int,int>> primeFactors(int n) {

    vector<pair<int,int>> factors;

    for (int p = 2; p * p <= n; p++) {

        if (n % p == 0) {

            int cnt = 0;

            while (n % p == 0) {
                cnt++;
                n /= p;
            }

            factors.push_back({p, cnt});
        }
    }

    if (n > 1)
        factors.push_back({n, 1});

    return factors;
}

// ============================================================
// All Divisors
// ============================================================

vector<int> getDivisors(int n) {

    vector<int> divs;

    for (int i = 1; i * i <= n; i++) {

        if (n % i == 0) {

            divs.push_back(i);

            if (i * i != n)
                divs.push_back(n / i);
        }
    }

    sort(divs.begin(), divs.end());

    return divs;
}

// ============================================================
// nCr Mod M
// ============================================================

vector<int> fact, invFact;

void initNCR(int n) {

    fact.assign(n + 1, 1);
    invFact.assign(n + 1, 1);

    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;

    invFact[n] = modInverse(fact[n]);

    for (int i = n; i >= 1; i--)
        invFact[i - 1] = invFact[i] * i % MOD;
}

int nCr(int n, int r) {

    if (r < 0 || r > n)
        return 0;

    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

// ============================================================

void solve() {

}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}