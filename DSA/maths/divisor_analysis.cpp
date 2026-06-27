/**
 * Problem: Divisor Analysis
 * Link: https://cses.fi/problemset/task/2182
 * Category: Mathematics
 * 
 * Description:
 * Calculate the count, sum, and product of divisors of a number.
 * 
 * Logic/Approach:
 * Arithmetic formulas combined with Fermat's Little Theorem.
 */

#include <iostream>
#include <vector>

using namespace std;

// Modulo constants
const long long MOD = 1e9 + 7;
const long long PHI = MOD - 1;   // Used for exponents (Fermat's Little Theorem)

/**
 * Computes (a^b) % mod using binary exponentiation.
 * Handles large intermediate products using __int128 to prevent overflow.
 */
long long modPow(long long a, long long b, long long mod) {
    long long res = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1)
            res = (__int128)res * a % mod;
        a = (__int128)a * a % mod;
        b >>= 1;
    }
    return res;
}

int main() {
    // Optimize standard I/O operations for performance
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Running answers tracked modulo 10^9 + 7
    long long divisorCount = 1;      
    long long divisorSum = 1;        
    long long divisorProduct = 1;    

    // We must track the total divisor count modulo (MOD - 1) 
    // because it will serve as an exponent in the product formula.
    long long divisorCountModPhi = 1;   

    for (int i = 0; i < n; i++) {
        long long p, k;
        cin >> p >> k;

        // -------------------------------------------------------------
        // 1. NUMBER OF DIVISORS
        // -------------------------------------------------------------
        // By the fundamental theorem of arithmetic, if N = p1^k1 * p2^k2...
        // Total Divisors = (k1 + 1) * (k2 + 1) * ...
        // We update this dynamically with each new prime factor.
        // -------------------------------------------------------------
        divisorCount = divisorCount * ((k + 1) % MOD) % MOD;

        // -------------------------------------------------------------
        // 2. SUM OF DIVISORS
        // -------------------------------------------------------------
        // The sum of divisors for p^k is a geometric series: 1 + p + p^2 + ... + p^k
        // Formula: (p^(k+1) - 1) / (p - 1)
        // We use Fermat's Little Theorem for modular division: (p - 1)^(MOD - 2)
        // -------------------------------------------------------------
        long long numerator = (modPow(p, k + 1, MOD) - 1 + MOD) % MOD;
        long long denominatorInv = modPow(p - 1, MOD - 2, MOD);
        long long currentPrimeSum = numerator * denominatorInv % MOD;

        divisorSum = divisorSum * currentPrimeSum % MOD;

        // -------------------------------------------------------------
        // 3. PRODUCT OF DIVISORS (Iterative DP Transition)
        // -------------------------------------------------------------
        // Let P = current product of all divisors processed before this prime.
        // Let D = current number of divisors processed before this prime.
        //
        // When introducing p^k, each old divisor 'd' generates (k+1) new divisors:
        // d*p^0, d*p^1, d*p^2, ..., d*p^k
        //
        // Multiplying all combinations across all D old divisors yields:
        // P_new = P^(k+1) * p^(D * (0 + 1 + 2 + ... + k))
        // P_new = P^(k+1) * p^(D * [k*(k+1)/2])
        //
        // Exponents must be computed modulo PHI (10^9 + 6).
        // -------------------------------------------------------------
        
        // Safely compute arithmetic progression sum: k*(k+1)/2 modulo PHI.
        // Division by 2 must happen BEFORE applying modulo.
        long long triangleSum;
        if (k % 2 == 0) {
            triangleSum = ((k / 2) % PHI) * ((k + 1) % PHI) % PHI;
        } else {
            triangleSum = (k % PHI) * (((k + 1) / 2) % PHI) % PHI;
        }

        // Total power of prime 'p' introduced in this step
        long long primeExponent = (divisorCountModPhi * triangleSum) % PHI; // divisorCountModPhi is D in our formula which holds the total divisor count modulo PHI up to this point, why modulo PHI? because it will be used as an exponent in the next iteration, and we need to ensure it doesn't exceed PHI to maintain correctness of modular exponentiation.

        // Update the global running product
        long long term1 = modPow(divisorProduct, k + 1, MOD);
        long long term2 = modPow(p, primeExponent, MOD);
        divisorProduct = (term1 * term2) % MOD;

        // -------------------------------------------------------------
        // UPDATE EXPONENT TRACKER
        // -------------------------------------------------------------
        // Update our divisor count tracker modulo (MOD - 1) for the next iteration.
        divisorCountModPhi = divisorCountModPhi * ((k + 1) % PHI) % PHI; // we are multiplying by (k + 1) because each new prime factor contributes (k + 1) new divisors, and we need to keep track of the total number of divisors modulo PHI for future exponent calculations.
    }

    // Print final answers modulo 10^9 + 7
    cout << divisorCount << " " << divisorSum << " " << divisorProduct << "\n";

    return 0;
}

// all about fermat theorem  -
// Fermat's Little Theorem states that if p is a prime and a is an integer not divisible by p, then a^(p-1) ≡ 1 (mod p).
