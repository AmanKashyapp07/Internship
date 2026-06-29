/**
 * LeetCode 132 - Palindrome Partitioning II (Memoization)
 *
 * Description:
 * Given a string s, partition s such that every substring of the partition is a palindrome.
 * Return the minimum cuts needed for a palindrome partitioning of s.
 *
 * Approach:
 * - Dynamic Programming with Memoization.
 * - First precompute a 2D table `isPalindrome[i][j]` representing whether `s[i...j]` is a palindrome.
 * - Let `solve(i)` return the minimum cuts for suffix `s[0...i]`.
 * - Transition: Try matching any palindrome ending at `i`: `1 + solve(j - 1)` for all `j <= i` where `s[j...i]` is a palindrome.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

class Solution {
public:
    vector<int> memo;
    vector<vector<bool>> isPal;
    string str;
    int size;

    int solve(int i) {
        if (i < 0) return 0;
        if (memo[i] != -1) return memo[i];
        if (isPal[0][i]) return memo[i] = 0;

        int ans = i; // worst case
        for (int j = i; j >= 0; j--) {
            if (isPal[j][i]) {
                ans = min(ans, 1 + solve(j - 1));
            }
        }

        return memo[i] = ans;
    }

    int minCut(string s) {
        str = s;
        size = s.size();
        isPal.assign(size, vector<bool>(size, false));

        for (int i = 0; i < size; i++) {
            isPal[i][i] = true;
        }

        for (int lengthVal = 2; lengthVal <= size; lengthVal++) {
            for (int i = 0; i + lengthVal - 1 < size; i++) {
                int j = i + lengthVal - 1;
                if (lengthVal == 2) {
                    isPal[i][j] = (str[i] == str[j]);
                } else {
                    isPal[i][j] = (str[i] == str[j]) && isPal[i + 1][j - 1];
                }
            }
        }

        memo.assign(size, -1);
        return solve(size - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    Solution solver;
    cout << solver.minCut(s) << '\n';

    return 0;
}
