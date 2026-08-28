#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;


using ll = long long;
using ull = unsigned long long;

const ll MOD = 1e9 + 7;

// ============================================================
// 1. GCD / LCM
// ============================================================

ll gcdLL(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

ll lcmLL(ll a, ll b) {
    return a / gcdLL(a, b) * b;
}
// Interview Explanation:
// - Problem Statement: Compute the Greatest Common Divisor (GCD) and Least Common Multiple (LCM) of two 64-bit integers.
// - Approach: Euclidean Algorithm via iterative remainder reduction; LCM(a, b) = (a / GCD(a, b)) * b.
// - Intuition: GCD(a, b) = GCD(b, a % b); dividing by GCD before multiplying prevents 64-bit integer overflow.
// - Complexity: Time: O(log(min(a, b))), Space: O(1) auxiliary space.


// ============================================================
// 2. Prime Check
// ============================================================

bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine whether a 64-bit integer n is prime.
// - Approach: Trial division up to sqrt(n).
// - Intuition: If n has a factor > sqrt(n), its paired factor must be < sqrt(n); testing divisibility up to sqrt(n) guarantees primality.
// - Complexity: Time: O(sqrt(n)), Space: O(1) auxiliary space.


// ============================================================
// 3. Sieve of Eratosthenes
// ============================================================

vector<bool> sieve(int n) {
    vector<bool> prime(n + 1, true);
    if (n >= 0) prime[0] = false;
    if (n >= 1) prime[1] = false;

    for (int i = 2; 1LL * i * i <= n; i++) {
        if (prime[i]) {
            for (int j = i * i; j <= n; j += i)
                prime[j] = false;
        }
    }
    return prime;
}
// Interview Explanation:
// - Problem Statement: Precompute primality for all integers from 0 up to n.
// - Approach: Sieve of Eratosthenes boolean array marking.
// - Intuition: Start marking multiples from i^2 because smaller multiples k * i (k < i) were already crossed out by smaller primes.
// - Complexity: Time: O(n \log \log n), Space: O(n) boolean vector.


// ============================================================
// 4. Prime Factorization
// ============================================================

vector<pair<ll, int>> primeFactors(ll n) {
    vector<pair<ll, int>> factors;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                cnt++;
            }
            factors.push_back({p, cnt});
        }
    }
    if (n > 1) factors.push_back({n, 1});
    return factors;
}
// Interview Explanation:
// - Problem Statement: Find all unique prime factors of n along with their exponent counts.
// - Approach: Trial division factor decomposition.
// - Intuition: Divide out all powers of each prime factor starting from 2; any remaining value > 1 after sqrt(n) is itself a prime.
// - Complexity: Time: O(sqrt(n)), Space: O(log n) for factor vector.


// ============================================================
// 5. Fast Power: a^b
// ============================================================

ll power(ll a, ll b) {
    ll ans = 1;
    while (b > 0) {
        if (b & 1) ans *= a;
        a *= a;
        b >>= 1;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Compute a^b in logarithmic time for integers.
// - Approach: Binary Exponentiation by squaring.
// - Intuition: Decompose b into binary powers of 2; multiply answer by current base whenever the lowest bit of b is set.
// - Complexity: Time: O(log b), Space: O(1) auxiliary space.


// ============================================================
// 6. Modular Power: (a^b) % MOD
// ============================================================

ll modPow(ll a, ll b, ll mod = MOD) {
    a %= mod;
    ll ans = 1;
    while (b > 0) {
        if (b & 1) ans = ans * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Compute (a^b) % MOD in logarithmic time.
// - Approach: Modular exponentiation by squaring.
// - Intuition: Modulo distributes over multiplication; reduce at each squaring and accumulation step to avoid overflow.
// - Complexity: Time: O(log b), Space: O(1) auxiliary space.


// ============================================================
// 7. Modular Inverse
// ============================================================

ll modInverse(ll a) {
    return modPow(a, MOD - 2);
}
// Interview Explanation:
// - Problem Statement: Compute the modular multiplicative inverse of a modulo prime MOD.
// - Approach: Fermat's Little Theorem (a^(MOD - 2) % MOD).
// - Intuition: For prime MOD, a^(MOD - 1) = 1 (mod MOD), so a * a^(MOD - 2) = 1 (mod MOD).
// - Complexity: Time: O(log MOD), Space: O(1) auxiliary space.


// ============================================================
// 8. Factorial + nCr
// ============================================================

vector<ll> fact, invFact;

void precomputeFactorials(int n) {
    fact.resize(n + 1);
    invFact.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;

    invFact[n] = modInverse(fact[n]);
    for (int i = n; i >= 1; i--)
        invFact[i - 1] = invFact[i] * i % MOD;
}

ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}
// Interview Explanation:
// - Problem Statement: Compute combinations nCr % MOD in O(1) query time after precomputation.
// - Approach: Factorial and inverse factorial precomputation with linear backward propagation.
// - Intuition: Precompute fact[n]; compute invFact[n] = modInverse(fact[n]), then fill invFact[i-1] = invFact[i] * i % MOD in O(N).
// - Complexity: Time: O(N) preprocessing, O(1) per nCr query, Space: O(N) array storage.


// ============================================================
// 9. Digit Functions
// ============================================================

int countDigits(ll n) {
    if (n == 0) return 1;
    n = abs(n);
    int cnt = 0;
    while (n) {
        cnt++;
        n /= 10;
    }
    return cnt;
}

ll reverseNumber(ll n) {
    ll rev = 0;
    while (n) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}

bool isPalindrome(ll n) {
    if (n < 0) return false;
    return n == reverseNumber(n);
}

int digitSum(ll n) {
    n = abs(n);
    int sum = 0;
    while (n) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int digitalRoot(ll n) {
    n = abs(n);
    if (n == 0) return 0;
    return 1 + (n - 1) % 9;
}
// Interview Explanation:
// - Problem Statement: Perform fundamental digit manipulations: count, reverse, palindrome check, digit sum, and digital root.
// - Approach: Decimal modulo/division loops; digital root via modulo 9 congruence formula (1 + (n - 1) % 9).
// - Intuition: Extracting n % 10 and n / 10 isolates individual digits in order; any number is congruent to its digit sum modulo 9.
// - Complexity: Time: O(log10 n) for digit loops, O(1) for digital root, Space: O(1) auxiliary space.


// ============================================================
// 10. Bit: Check kth Bit
// ============================================================

bool isSet(int n, int k) {
    return (n & (1 << k)) != 0;
}
// Interview Explanation:
// - Problem Statement: Check if the k-th bit of an integer is set (1).
// - Approach: Bitwise AND with a single-bit mask (1 << k).
// - Intuition: Mask 1 << k isolates bit position k; non-zero result means the k-th bit is active.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 11. Bit: Set kth Bit
// ============================================================

int setBit(int n, int k) {
    return n | (1 << k);
}
// Interview Explanation:
// - Problem Statement: Turn ON (set to 1) the k-th bit of an integer.
// - Approach: Bitwise OR with (1 << k).
// - Intuition: x | (1 << k) sets the k-th bit to 1 while leaving all other bits unchanged.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 12. Bit: Clear kth Bit
// ============================================================

int clearBit(int n, int k) {
    return n & ~(1 << k);
}
// Interview Explanation:
// - Problem Statement: Turn OFF (clear to 0) the k-th bit of an integer.
// - Approach: Bitwise AND with negated mask ~(1 << k).
// - Intuition: ~(1 << k) has 0 at position k and 1 everywhere else, forcing the k-th bit to 0.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 13. Bit: Toggle kth Bit
// ============================================================

int toggleBit(int n, int k) {
    return n ^ (1 << k);
}
// Interview Explanation:
// - Problem Statement: Flip (toggle) the k-th bit of an integer (0 to 1, or 1 to 0).
// - Approach: Bitwise XOR with (1 << k).
// - Intuition: XORing with 1 inverts the bit, while XORing with 0 preserves it.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 14. Power of 2
// ============================================================

bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
// Interview Explanation:
// - Problem Statement: Determine whether an integer n is a power of 2 in O(1) time.
// - Approach: Bitwise subtraction trick n & (n - 1) == 0.
// - Intuition: A power of 2 has exactly one bit set; subtracting 1 flips that bit and sets all lower bits, so n & (n - 1) clears it to 0.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 15. Count Set Bits
// ============================================================

int countSetBits(unsigned int n) {
    int cnt = 0;
    while (n) {
        n &= (n - 1);
        cnt++;
    }
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Count the number of set bits (1s) in the binary representation of an integer.
// - Approach: Brian Kernighan's Algorithm.
// - Intuition: Each iteration of n &= (n - 1) clears the lowest set bit; the loop runs in exactly the number of set bits rather than 32 times.
// - Complexity: Time: O(number of set bits) <= O(32), Space: O(1).


// ============================================================
// 16. Lowest Set Bit
// ============================================================

int lowestSetBit(int n) {
    return n & -n;
}
// Interview Explanation:
// - Problem Statement: Isolate the lowest (rightmost) set bit of an integer as an isolated power-of-two value.
// - Approach: Two's complement bitwise AND n & -n.
// - Intuition: In two's complement, -n = ~n + 1; this flips all bits to the left of the lowest set bit and preserves the lowest set bit.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 17. Remove Lowest Set Bit
// ============================================================

int removeLowestSetBit(int n) {
    return n & (n - 1);
}
// Interview Explanation:
// - Problem Statement: Clear (turn off) the lowest set bit of an integer.
// - Approach: Bitwise AND n & (n - 1).
// - Intuition: Subtracting 1 flips the rightmost set bit and all bits to its right; ANDing with n clears that bit while leaving higher bits intact.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 18. Single Number
// ============================================================

int singleNumber(const vector<int>& nums) {
    int ans = 0;
    for (int x : nums) ans ^= x;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Given an array where every element appears twice except one, find that single element.
// - Approach: Cumulative Bitwise XOR reduction.
// - Intuition: XOR is commutative and self-inverting (x ^ x = 0 and x ^ 0 = x); all duplicate pairs cancel out, leaving only the unique number.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.


// ============================================================
// 19. Two Unique Numbers
// ============================================================

pair<int, int> twoSingleNumbers(const vector<int>& nums) {
    int xorsum = 0;
    for (int x : nums) xorsum ^= x;

    int bit = xorsum & -xorsum;
    int a = 0, b = 0;
    for (int x : nums) {
        if (x & bit) a ^= x;
        else b ^= x;
    }
    return {a, b};
}
// Interview Explanation:
// - Problem Statement: Given an array where every element appears twice except two unique numbers, find both numbers.
// - Approach: Cumulative XOR + Rightmost set bit partitioning.
// - Intuition: Total XOR equals a ^ b; any set bit in this XOR indicates a bit where a and b differ; partition numbers by this bit to isolate a and b.
// - Complexity: Time: O(N) two passes, Space: O(1) auxiliary space.


// ============================================================
// 20. Missing Number
// ============================================================

int missingNumber(const vector<int>& nums) {
    int n = nums.size();
    int ans = n;
    for (int i = 0; i < n; i++)
        ans ^= i ^ nums[i];
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the missing number in an array containing n distinct numbers from [0, n].
// - Approach: Bitwise XOR matching against complete range [0, n].
// - Intuition: XOR all array elements with all indices 0 to n; all present numbers cancel out with their index counterparts, leaving the missing number.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.


// ============================================================
// 21. XOR from 1 to n
// ============================================================

int xor1ToN(int n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        default: return 0;
    }
}
// Interview Explanation:
// - Problem Statement: Compute the XOR sum of all integers from 1 to n in O(1) time.
// - Approach: Modulo 4 periodicity of consecutive XOR sums.
// - Intuition: Consecutive groups of four XOR to 0 (4k ^ (4k+1) ^ (4k+2) ^ (4k+3) = 0); cumulative sum repeats every 4 numbers.
// - Complexity: Time: O(1) constant time, Space: O(1) auxiliary space.


// ============================================================
// 22. XOR Range [l, r]
// ============================================================

int xorRange(int l, int r) {
    return xor1ToN(r) ^ xor1ToN(l - 1);
}
// Interview Explanation:
// - Problem Statement: Compute the XOR sum of all integers in the range [l, r] in O(1) time.
// - Approach: Prefix XOR difference xor1ToN(r) ^ xor1ToN(l - 1).
// - Intuition: XOR is its own inverse; XORing prefix sums up to r and l-1 cancels out elements from 1 to l-1, isolating range [l, r].
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 23. Power of 4
// ============================================================

bool isPowerOfFour(int n) {
    if (!isPowerOfTwo(n)) return false;
    return (n & 0x55555555) != 0;
}
// Interview Explanation:
// - Problem Statement: Check whether an integer is a power of 4 in O(1) time.
// - Approach: Power of 2 check + Odd bit position bitmask 0x55555555.
// - Intuition: A power of 4 is a power of 2 with its single set bit located at an even bit position (0, 2, 4, ...); 0x55555555 masks these positions.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 24. Even / Odd
// ============================================================

bool isEven(int n) {
    return (n & 1) == 0;
}

bool isOdd(int n) {
    return (n & 1) != 0;
}
// Interview Explanation:
// - Problem Statement: Check if an integer is even or odd using bitwise operations.
// - Approach: Test lowest bit n & 1.
// - Intuition: The least significant bit represents 2^0 = 1; it is 0 for even numbers and 1 for odd numbers.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 25. Swap Using XOR
// ============================================================

void swapXOR(int& a, int& b) {
    a ^= b;
    b ^= a;
    a ^= b;
}
// Interview Explanation:
// - Problem Statement: Swap two integer variables in-place without using a temporary variable.
// - Approach: Three-step XOR swap (a ^= b; b ^= a; a ^= b;).
// - Intuition: Uses self-inversion of XOR to interchange values through bitwise difference encoding.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 26. Reverse 32 Bits
// ============================================================

uint32_t reverseBits(uint32_t n) {
    uint32_t ans = 0;
    for (int i = 0; i < 32; i++) {
        ans <<= 1;
        ans |= (n & 1);
        n >>= 1;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Reverse the 32-bit binary representation of an unsigned integer.
// - Approach: Bit shift loop extracting lowest bit and appending to result.
// - Intuition: Shift result left and OR with n & 1; shift n right, repeating across all 32 bits.
// - Complexity: Time: O(1) exactly 32 iterations, Space: O(1).


// ============================================================
// 27. Binary Representation
// ============================================================

string toBinary(unsigned int n) {
    if (n == 0) return "0";
    string s;
    while (n) {
        s += char('0' + (n & 1));
        n >>= 1;
    }
    reverse(s.begin(), s.end());
    return s;
}
// Interview Explanation:
// - Problem Statement: Convert an unsigned integer into its binary string representation.
// - Approach: Repeated division by 2 and remainder extraction.
// - Intuition: Extract lowest bit with n & 1, append to string, shift right by 1, and reverse output string.
// - Complexity: Time: O(log n), Space: O(log n) for string output.


// ============================================================
// 28. Gray Code
// ============================================================

vector<int> grayCode(int n) {
    vector<int> ans;
    for (int i = 0; i < (1 << n); i++)
        ans.push_back(i ^ (i >> 1));
    return ans;
}
// Interview Explanation:
// - Problem Statement: Generate the n-bit reflected Gray code sequence where adjacent numbers differ by exactly one bit.
// - Approach: Formula i ^ (i >> 1).
// - Intuition: Shifting right and XORing with original index toggles bits so that every increment of i flips exactly one bit in Gray code.
// - Complexity: Time: O(2^n), Space: O(2^n) for sequence vector.


// ============================================================
// 29. Generate All Subsets Using Bitmask
// ============================================================

vector<vector<int>> subsets(const vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> ans;
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> curr;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) curr.push_back(nums[i]);
        }
        ans.push_back(curr);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Generate all 2^n subsets (power set) of an array of numbers.
// - Approach: Bitmask counting from 0 to 2^n - 1.
// - Intuition: Each integer mask represents a subset where bit i indicates whether nums[i] is included.
// - Complexity: Time: O(n * 2^n), Space: O(n * 2^n) for all subsets.


// ============================================================
// 30. Iterate Through All Submasks
// ============================================================

void processSubmasks(int mask) {
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // Process submask here
    }
}
// Interview Explanation:
// - Problem Statement: Efficiently enumerate all submasks of a given bitmask without generating irrelevant values.
// - Approach: Submask decrement trick sub = (sub - 1) & mask.
// - Intuition: Subtracting 1 from submask and ANDing with original mask skips all non-submasks, iterating all valid submasks in strictly decreasing order.
// - Complexity: Time: O(2^k) where k is set bits in mask (O(3^N) across all masks), Space: O(1).


// ============================================================
// 31. Opposite Signs
// ============================================================

bool oppositeSigns(int a, int b) {
    return (a < 0) != (b < 0);
}
// Interview Explanation:
// - Problem Statement: Check if two non-zero integers have opposite arithmetic signs without multiplying.
// - Approach: Boolean comparison (a < 0) != (b < 0).
// - Intuition: Multiplying can cause 32-bit signed overflow; comparing sign conditions directly avoids overflow entirely.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 32. Absolute Value Using Bits
// ============================================================

int absoluteValue(int x) {
    int mask = x >> 31;
    return (x ^ mask) - mask;
}
// Interview Explanation:
// - Problem Statement: Compute the absolute value of an integer without conditional branching.
// - Approach: Sign bit arithmetic shift (x ^ mask) - mask where mask = x >> 31.
// - Intuition: For positive x, mask = 0 and result is x; for negative x, mask = -1 = ~0, transforming x to ~x - (-1) = ~x + 1 = -x.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 33. Next Power of 2
// ============================================================

int nextPowerOfTwo(int n) {
    if (n <= 1) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}
// Interview Explanation:
// - Problem Statement: Round up an integer to the nearest power of 2 greater than or equal to n.
// - Approach: Bit-smearing via successive bit shifts and bitwise ORs.
// - Intuition: Smearing 1s across bit positions 1, 2, 4, 8, 16 sets all bits below the highest bit to 1; adding 1 carries over to the next power of 2.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 34. Built-in Bit Functions
// ============================================================

int builtInCountBits(unsigned int n) {
    return __builtin_popcount(n);
}

int builtInCountBitsLL(unsigned long long n) {
    return __builtin_popcountll(n);
}

int trailingZeros(unsigned int n) {
    if (n == 0) return -1;
    return __builtin_ctz(n);
}

int leadingZeros(unsigned int n) {
    if (n == 0) return -1;
    return __builtin_clz(n);
}

int bitParity(unsigned int n) {
    return __builtin_parity(n);
}
// Interview Explanation:
// - Problem Statement: Efficiently compute popcount, trailing zeros, leading zeros, and parity using compiler intrinsics.
// - Approach: GCC/Clang builtins (__builtin_popcount, __builtin_ctz, __builtin_clz, __builtin_parity).
// - Intuition: Maps directly to single hardware CPU instructions (e.g. POPCNT, TZCNT, LZCNT) for maximum execution speed.
// - Complexity: Time: O(1) hardware instructions, Space: O(1).


// ============================================================
// 35. Perfect Square
// ============================================================

bool isPerfectSquare(ll n) {
    if (n < 0) return false;
    ll r = sqrtl(n);
    while (r * r < n) r++;
    while (r * r > n) r--;
    return r * r == n;
}
// Interview Explanation:
// - Problem Statement: Determine whether a 64-bit integer n is a perfect square.
// - Approach: Integer square root with rounding compensation.
// - Intuition: Floating-point sqrtl can introduce precision errors for large 64-bit values; compensating with adjacent integer adjustments ensures exact square checking.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 36. Fast Fibonacci
// ============================================================

pair<ll, ll> fibonacci(ll n) {
    if (n == 0) return {0, 1};
    auto [a, b] = fibonacci(n / 2);
    ll c = a * (2 * b - a);
    ll d = a * a + b * b;
    if (n % 2 == 0) return {c, d};
    return {d, c + d};
}
// Interview Explanation:
// - Problem Statement: Compute the n-th Fibonacci number in O(log n) time using Fast Doubling.
// - Approach: Fast Doubling recurrence returning {F(n), F(n+1)}.
// - Intuition: Using identities F(2k) = F(k)[2F(k+1) - F(k)] and F(2k+1) = F(k)^2 + F(k+1)^2 computes Fibonacci in logarithmic steps without matrix overhead.
// - Complexity: Time: O(log n) recursion steps, Space: O(log n) call stack.


// ============================================================
// 37. Squared Distance
// ============================================================

ll distanceSquared(ll x1, ll y1, ll x2, ll y2) {
    ll dx = x1 - x2;
    ll dy = y1 - y2;
    return dx * dx + dy * dy;
}
// Interview Explanation:
// - Problem Statement: Compute the squared Euclidean distance between two 2D points.
// - Approach: Formula (x1 - x2)^2 + (y1 - y2)^2.
// - Intuition: Working with squared distances avoids floating-point inaccuracies and costly square roots when comparing lengths.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 38. Cross Product
// ============================================================

ll cross(ll ax, ll ay, ll bx, ll by) {
    return ax * by - ay * bx;
}
// Interview Explanation:
// - Problem Statement: Compute the 2D cross product of vectors A and B.
// - Approach: Determinant formula ax * by - ay * bx.
// - Intuition: Represents the signed area of the parallelogram formed by the two vectors; positive means counter-clockwise rotation from A to B.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 39. Orientation of A, B, C
// ============================================================

ll orientation(ll ax, ll ay, ll bx, ll by, ll cx, ll cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}
// Interview Explanation:
// - Problem Statement: Determine whether three points A, B, C form a clockwise turn, counter-clockwise turn, or are collinear.
// - Approach: Signed cross product of vectors AB and BC.
// - Intuition: Orientation value > 0 indicates counter-clockwise (left turn), < 0 clockwise (right turn), and == 0 collinearity.
// - Complexity: Time: O(1), Space: O(1).


// ============================================================
// 40. Modulo 2^k
// ============================================================

int modPowerOfTwo(int x, int k) {
    return x & ((1 << k) - 1);
}
// Interview Explanation:
// - Problem Statement: Compute x % 2^k using bitwise operations.
// - Approach: Bitwise AND with mask x & ((1 << k) - 1).
// - Intuition: The remainder modulo 2^k is represented exactly by the lower k bits; (1 << k) - 1 masks these bits in O(1).
// - Complexity: Time: O(1), Space: O(1).

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: BIT MANIPULATION, MATH & GEOMETRY
 ====================================================================================================

 1. BIT MANIPULATION ESSENTIAL FORMULAS:
    | Operation                                   | Bitwise Expression                  | Meaning / Core Utility                |
    |:--------------------------------------------|:------------------------------------|:--------------------------------------|
    | Check k-th bit                              | (n & (1 << k)) != 0                 | Returns true if k-th bit is set       |
    | Set k-th bit                                | n | (1 << k)                        | Turns on bit k                        |
    | Clear k-th bit                              | n & ~(1 << k)                       | Turns off bit k                       |
    | Toggle k-th bit                             | n ^ (1 << k)                        | Inverts bit k                         |
    | Power of 2 check                            | n > 0 && (n & (n - 1)) == 0         | Exactly one bit set                   |
    | Lowest set bit                              | n & -n                              | Isolates rightmost set bit            |
    | Remove lowest set bit                       | n & (n - 1)                         | Clears rightmost set bit              |
    | Modulo 2^k                                  | n & ((1 << k) - 1)                  | Equivalent to n % 2^k                 |
    | Multiply / Divide by 2^k                    | n << k / n >> k                     | Fast power-of-two scaling             |
    | Submask enumeration                         | sub = (sub - 1) & mask              | Enumerates all subsets in O(2^popcnt) |
    | Fast popcount (number of 1s)                | __builtin_popcount(n)               | Single CPU hardware instruction       |
    | Trailing / Leading zeros                    | __builtin_ctz(n) / __builtin_clz(n) | Undefined if n == 0 (guard it!)       |

 2. 2D COMPUTATIONAL GEOMETRY PRIMITIVES:
    • Cross Product: `cross(A, B) = ax * by - ay * bx`
    • Orientation of (A, B, C): `(bx - ax)*(cy - ay) - (by - ay)*(cx - ax)`
      - Result > 0: Counter-Clockwise (Left turn)
      - Result < 0: Clockwise (Right turn)
      - Result == 0: Collinear points
    • Squared Distance: Always compare `(x1-x2)^2 + (y1-y2)^2` to avoid floating-point square root errors.

 3. LIVE INTERVIEW & OA GOTCHAS:
    • Shift Overflow: `1 << k` overflows 32-bit signed integer when $k \ge 31$. ALWAYS write `1LL << k` for 64-bit bits.
    • Builtin Zero Guard: `__builtin_clz(0)` and `__builtin_ctz(0)` cause UNDEFINED BEHAVIOR on x86/ARM CPUs. Always handle 0 explicitly!
    • Negative Modulo: `(a % MOD + MOD) % MOD` is required in C++ because `-5 % 3 == -2` (not +1).
    • Fermat's Little Theorem constraint: Modular inverse `a^(MOD-2)` is valid ONLY if `MOD` is prime and `gcd(a, MOD) == 1`.
 ====================================================================================================
*/