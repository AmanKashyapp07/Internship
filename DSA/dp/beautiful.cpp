/**
 * LeetCode 526 - Beautiful Arrangement
 *
 * Suppose you have n integers labeled from 1 to n.
 * A permutation perm (1-indexed) is beautiful if for every i (1 <= i <= n),
 * either perm[i] divides i or i divides perm[i].
 *
 * Return the number of beautiful arrangements that can be constructed.
 *
 * Approach: Bitmask DP + Memoization
 * Time Complexity: O(n * 2^n)
 * Space Complexity: O(2^n)
 */

#include <vector>
using namespace std;

class Solution {
public:
    int n;
    vector<int> memo;

    int dp(int mask) {
        int pos = __builtin_popcount(mask); // current position to fill (0-based)

        if (pos == n) {
            return 1;
        }

        if (memo[mask] != -1) {
            return memo[mask];
        }

        int count = 0;
        for (int num = 0; num < n; num++) {
            if ((mask & (1 << num)) == 0) { // number not used yet
                int val = num + 1;
                int idx = pos + 1;
                if (val % idx == 0 || idx % val == 0) {
                    count += dp(mask | (1 << num));
                }
            }
        }

        return memo[mask] = count;
    }

    int countArrangement(int n) {
        this->n = n;
        memo.assign(1 << n, -1);
        return dp(0);
    }
};