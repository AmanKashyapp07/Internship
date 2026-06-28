/**
 * Problem: Bit Manipulation Master Template
 * Category: Bits & Binary Representations
 *
 * Description:
 * A comprehensive reference covering basic operations, built-ins, standard mathematical tricks, 
 * subset/submask enumeration, and genuine bitwise interview questions asked in OAs.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <utility>

using namespace std;

// =========================================================================
// 1. BASIC BIT-WISE OPERATIONS
// =========================================================================

// Check if the i-th bit (0-indexed) is set
bool isSet(int mask, int i) {
    return mask & (1 << i);
}

// Set the i-th bit
int setBit(int mask, int i) {
    return mask | (1 << i);
}

// Clear the i-th bit
int clearBit(int mask, int i) {
    return mask & ~(1 << i);
}

// Toggle the i-th bit
int toggleBit(int mask, int i) {
    return mask ^ (1 << i);
}

// Extract the value of the i-th bit (0 or 1), i is 0-indexed
int getBit(int mask, int i) {
    return (mask >> i) & 1;
}

// =========================================================================
// 2. STANDARD MATHEMETICAL TRICKS
// =========================================================================

// Remove the lowest set bit (e.g. 1100 -> 1000)
int removeLSB(int x) {
    return x & (x - 1);
}

// Extract the value of the lowest set bit (e.g. 1100 -> 0100)
int lowestSetBit(int x) {
    return x & -x;
}

// Check if a number is a power of two
bool isPowerOfTwo(int x) {
    return x > 0 && !(x & (x - 1));
}

vector<int> convertToBinary(int n) {
    vector<int> binary(32, 0);
    for(int i = 0; i < 32; i++) {
        binary[i] = (n >> i) & 1;
    }
    reverse(binary.begin(), binary.end());
    return binary;
}

int convertFromBinary(const vector<int>& binary) {
    int n = 0;
    for(int i = 0; i < binary.size(); i++) {
        n = (n << 1) | binary[i]; // n<<1 will become 00 , then it will be 01 or 00 depending on the binary[i] value, then it will be 010 or 000 depending on the binary[i] value, and so on.
    }
    return n;
}


// =========================================================================
// 3. GCC BUILT-INS (FAST ASSEMBLER METRICS)
// =========================================================================

// Number of set bits (popcount)
int popcount(int x) { return __builtin_popcount(x); }
int popcountll(long long x) { return __builtin_popcountll(x); }

// Index of the Most Significant Bit (0-indexed)
// __builtin_clz counts leading zeros; 31 - clz yields MSB index
int msb(int x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }

// Index of the Least Significant Bit (0-indexed)
// __builtin_ctz counts trailing zeros
int lsb(int x) { return x == 0 ? -1 : __builtin_ctz(x); }

// =========================================================================
// 4. GENUINE INTERVIEW & OA PROBLEMS
// =========================================================================


/**
 * LeetCode 137: Single Number II
 * Goal: Find the unique element where all other elements appear exactly thrice.
 * Strategy: Count set bits at each of the 32 positions. Sum % 3 yields the bit of the answer.
 * Time: O(32 * N) = O(N) | Space: O(1)
 * 
 */
int singleNumberII(const vector<int>& nums) {
    int ans = 0;
    for (int i = 0; i < 32; ++i) {
        int sum = 0;
        for (int x : nums) {
            if ((x >> i) & 1) sum++; // (x>>i) & 1 extracts the i-th bit of x
        }
        if (sum % 3 != 0) {
            ans |= (1 << i);
        }
    }
    return ans;
}

/**
 * LeetCode 260: Single Number III
 * Goal: Find the TWO unique elements where all other elements appear exactly twice.
 * Strategy: Find xorSum of all numbers. Pick the lowest set bit where they differ, and
 * partition elements into two groups to find each single number.
 * Time: O(N) | Space: O(1)
 */
pair<int, int> singleNumberIII(const vector<int>& nums) {
    int xorSum = 0;
    for (int x : nums)
        xorSum ^= x;

    int diff = xorSum & -xorSum;

    int a = 0, b = 0;
    for (int x : nums) {
        if (x & diff)
            a ^= x;
        else
            b ^= x;
    }

    return {a, b};
}
/**
 * LeetCode 201: Bitwise AND of Numbers Range
 * Goal: Find bitwise AND of all numbers in [left, right] inclusive.
 * Strategy: Find the common prefix of left and right by shifting right until they are equal.
 * Time: O(log R) | Space: O(1)
 * Trick is : AND of range [left, right] is equivalent to AND of all numbers from left to right, which means common prefix of left and right will be the result, all other bits will be zeroed out due to the presence of both 0 and 1 in that range.
 */
int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left < right) {
        left >>= 1; // both are shifted right until they are equal, which means we are finding the common prefix
        right >>= 1;
        shift++; // count how many bits we have shifted, which will be the number of zeros in the result
    }
    return left << shift;
}

int rangeBitwiseAndOptimized(int left, int right) {
    vector<int> a = convertToBinary(left);
    vector<int> b = convertToBinary(right);
    int flag = 0;
    for(int i = 0; i < 32; i++) {
        if(a[i] != b[i]) {
            flag = i;
            break;
        }
    }
    for(int i = flag; i < 32; i++) {
        a[i] = 0;
    }
    return convertFromBinary(a);
}


/**
 * LeetCode 338: Counting Bits
 * Goal: Return array where ans[i] is the number of 1-bits in i.
 * Strategy: DP relationship using dp[i] = dp[i >> 1] + (i & 1).
 * Time: O(N) | Space: O(N)
 */
vector<int> countBits(int n) {
    vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        dp[i] = __builtin_popcount(i);
    }
    return dp;
}

// XOR sum of numbers from 1 to x
int xorToX(int x) {
    if (x % 4 == 0) return x;
    if (x % 4 == 1) return 1;
    if (x % 4 == 2) return x + 1;
    if (x%4 == 3) return 0;
}

// XOR sum of numbers in range [l, r]
int xorInRange(int l, int r) {
    return xorToX(r) ^ xorToX(l - 1);
}

// =========================================================================
// 5. ENUMERATION LOOPS (BITMASK PARADIGMS)
// =========================================================================

// Enumerate all subsets of size n: O(n * 2^n)
void enumerateSubsets(int n) {
    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                // Element i is present in subset 'mask'
            }
        }
    }
}

// Enumerate all submasks of a specific mask: O(3^n) total over all masks
void enumerateSubmasks(int mask) {
    for (int sub = mask; sub; sub = (sub - 1) & mask) {
        // 'sub' is a valid submask of 'mask'
    }
    // Note: the loop terminates before processing the empty submask (sub = 0)
}

int maxConsecutiveOnes(const vector<int>& v) {
    int best = 0, cur = 0;

    for (int x : v) {
        if (x == 1) {
            cur++;
            best = max(best, cur);
        } else {
            cur = 0;
        }
    }

    return best;
}

int longestSubarrayWithMaxBitwiseAnd(const vector<int>& nums) {
    int maxi = *max_element(nums.begin(), nums.end());
    int n = nums.size();
    vector<int>temp(n, 0);

    for(int i = 0; i < n; i++) {
        if(nums[i] == maxi) {
            temp[i] = 1;
        }
    }

    return maxConsecutiveOnes(temp);
}

int longestNiceSubarray(const vector<int>& nums) {
    int n = nums.size();
    int left = 0, maxLength = 0;
    int mask = 0;
    for(int i=0; i<n; i++) {
        while((mask & nums[i]) != 0) { // While the current number shares any set bits with the accumulated mask, we need to shrink the window from the left to maintain the "nice" property (no overlapping bits).
            mask ^= nums[left];
            left++;
        }
        mask |= nums[i];
        maxLength = max(maxLength, i - left + 1);
    }
    return maxLength;
}

int subarrayBitwiseORs(vector<int>& arr) {
        unordered_set<int>s1;
        unordered_set<int>s2;
        for(auto c:arr){
            unordered_set<int>s3;
            s3.insert(c);
            for(auto v:s2) s3.insert(c|v);
            for(auto x:s3) s1.insert(x);
            s2=s3;
        }
        return s1.size();
}

int totalHammingDistance(vector<int>& nums) {
    int n = nums.size();
    long long total = 0;
    for (int i = 0; i < 32; ++i) {
        long long countOnes = 0;
        for (int num : nums) {
            if ((num >> i) & 1) {
                countOnes++;
            }
        } // countOnes is number of elements with the i-th bit set to 1
        long long countZeros = n - countOnes;
        total += countOnes * countZeros; // Each pair contributes to the Hamming distance
    }
    return total;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}