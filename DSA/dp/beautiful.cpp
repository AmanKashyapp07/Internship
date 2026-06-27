/**
 * LeetCode 526 - Beautiful Arrangement
 *
 * Description:
 * Suppose you have n integers labeled 1 to n. A permutation of these n integers perm (1-indexed) is
 * considered a beautiful arrangement if for every 1 <= i <= n, either perm[i] is divisible by i or i is divisible by perm[i].
 * Given an integer n, return the number of beautiful arrangements you can construct.
 *
 * Approach:
 * - Bitmask Dynamic Programming with Memoization.
 * - Let `solve(mask)` compute the number of beautiful arrangements for the prefix of length `i = __builtin_popcount(mask)`.
 * - Iterate through each number `num` (0-based) from 0 to n - 1. If it's not placed yet:
 *   - Check if `(num + 1) % (i + 1) == 0` or `(i + 1) % (num + 1) == 0`.
 *   - If so, transition recursively to `solve(mask | (1 << num))`.
 *
 * Time Complexity: O(2^n * n)
 * Space Complexity: O(2^n)
 */

#include <vector>
using namespace std;

class Solution {
public:
    int size;
    vector<int> memo;

    int solve(int mask) {
        int i = __builtin_popcount(mask);   // current position (0-based)

        if (i == size)
            return 1;

        if (memo[mask] != -1)
            return memo[mask];

        int ans = 0;

        for (int num = 0; num < size; num++) {   // numbers are 0-based
            if ((mask & (1 << num)) == 0) {

                // actual number = num + 1
                // actual position = i + 1
                if ((num + 1) % (i + 1) == 0 || (i + 1) % (num + 1) == 0) {
                    ans += solve(mask | (1 << num));
                }
            }
        }

        return memo[mask] = ans;
    }

    int countArrangement(int n) {
        size = n;
        memo.assign(1 << n, -1);
        return solve(0);
    }
};