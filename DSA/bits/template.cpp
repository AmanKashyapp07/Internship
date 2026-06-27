/*
==============================================================================
BIT MANIPULATION TEMPLATE (INTERVIEWS + OA)
==============================================================================

Most Important:
1. Check / Set / Clear / Toggle Bit
2. Count Set Bits
3. Lowest Set Bit
4. Remove Lowest Set Bit
5. Power of Two
6. XOR Tricks
7. Subset Enumeration
8. Submask Enumeration
9. Builtins

==============================================================================
*/

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// ============================================================================
// BASIC OPERATIONS
// ============================================================================

// Check ith bit (0-indexed)
inline bool isSet(int mask, int i) {
    return mask & (1 << i);
}

// Set ith bit
inline int setBit(int mask, int i) {
    return mask | (1 << i);
}

// Clear ith bit
inline int clearBit(int mask, int i) {
    return mask & ~(1 << i);
}

// Toggle ith bit
inline int toggleBit(int mask, int i) {
    return mask ^ (1 << i);
}

// Extract ith bit (0 or 1)
inline int getBit(int mask, int i) {
    return (mask >> i) & 1;
}

// ============================================================================
// COMMON TRICKS
// ============================================================================

// Remove lowest set bit
inline int removeLSB(int x) {
    return x & (x - 1);
}

// Value of lowest set bit
inline int lowestSetBit(int x) {
    return x & -x;
}

// Check power of 2
inline bool isPowerOfTwo(int x) {
    return x > 0 && !(x & (x - 1));
}

// ============================================================================
// BUILTINS
// ============================================================================

// Number of set bits
inline int popcount(int x) {
    return __builtin_popcount(x);
}

inline int popcountll(long long x) {
    return __builtin_popcountll(x);
}

// Position of MSB (0-indexed)
inline int msb(int x) {
    return 31 - __builtin_clz(x);
}

// Position of LSB (0-indexed)
inline int lsb(int x) {
    return __builtin_ctz(x);
}

// ============================================================================
// XOR PATTERNS
// ============================================================================

// Unique element (others appear twice)
int singleNumber(const vector<int>& nums) {
    int ans = 0;
    for (int x : nums) ans ^= x;
    return ans;
}

// Missing number from [0..n]
int missingNumber(const vector<int>& nums) {
    int n = nums.size();
    int ans = 0;

    for (int i = 0; i <= n; i++) ans ^= i;
    for (int x : nums) ans ^= x;

    return ans;
}

int xorinRange(int l, int r) {
    auto xorTo = [](int x) {
        if (x % 4 == 0) return x;
        if (x % 4 == 1) return 1;
        if (x % 4 == 2) return x + 1;
        return 0; // x % 4 == 3
    };
    return xorTo(r) ^ xorTo(l - 1);
}
// ============================================================================
// SUBSET ENUMERATION
// O(n * 2^n)
// ============================================================================

void enumerateSubsets(int n) {

    for (int mask = 0; mask < (1 << n); mask++) {

        // elements present in mask
        for (int i = 0; i < n; i++) {

            if (mask & (1 << i)) {

            }
        }
    }
}

// ============================================================================
// SUBMASK ENUMERATION
// Visits every submask of mask
// Total over all masks = O(3^n)
// ============================================================================

void enumerateSubmasks(int mask) {

    for (int sub = mask; sub; sub = (sub - 1) & mask) {

    }

    // include empty submask if needed
    // sub = 0
}

// ============================================================================
// BITMASK DP LOOP
// ============================================================================

void bitmaskDP(int n) {

    vector<int> dp(1 << n);

    for (int mask = 0; mask < (1 << n); mask++) {

        for (int bit = 0; bit < n; bit++) {

            if (!(mask & (1 << bit))) {

                int nextMask = mask | (1 << bit);

                // transition
            }
        }
    }
}

/*
==============================================================================
MUST REMEMBER
==============================================================================

Odd / Even          : x & 1
Multiply by 2       : x << 1
Divide by 2         : x >> 1

Check ith bit       : x & (1 << i)
Set ith bit         : x | (1 << i)
Clear ith bit       : x & ~(1 << i)
Toggle ith bit      : x ^ (1 << i)

Power of Two        : x > 0 && !(x & (x - 1))
Remove LSB          : x & (x - 1)
Lowest Set Bit      : x & (-x)

Count Set Bits      : __builtin_popcount(x)
MSB Position        : 31 - __builtin_clz(x)
LSB Position        : __builtin_ctz(x)

Subset Count        : 2^n
Submask Count       : 3^n (all masks)

==============================================================================
*/