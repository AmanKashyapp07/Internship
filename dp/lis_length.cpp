/**
 * CSES 1145 - Longest Increasing Subsequence
 *
 * Description:
 * Given an array of n integers, find the length of the longest increasing subsequence.
 *
 * Approach:
 * - O(n log n) dynamic programming using binary search.
 * - Maintain a `tails` array where `tails[i]` stores the smallest tail of all increasing subsequences of length `i+1` found so far.
 * - For each element `x`, use `lower_bound` to find its placement in `tails` and update or append it.
 * - The LIS length is the size of the `tails` array.
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; if (!(cin >> n)) return 0;
    vector<long long> tails;
    // Patience sorting: tails[i] is the smallest tail of increasing subsequences of length i+1
    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x; // Overwrite tail to keep elements as small as possible
    }
    cout << tails.size() << '\n';
}