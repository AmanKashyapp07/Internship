/**
 * LeetCode 646 - Maximum Length of Pair Chain
 *
 * Description:
 * You are given an array of n pairs pairs where pairs[i] = [lefti, righti] and lefti < righti.
 * A pair p2 = [c, d] follows a pair p1 = [a, b] if b < c. A chain of pairs can be formed in this fashion.
 * Return the longest length of a chain that can be formed.
 *
 * Approach:
 * - Greedy interval scheduling (equivalent to Activity Selection Problem).
 * - Sort the pairs by their second element (right endpoint) in ascending order.
 * - Iterate through sorted pairs and pick a pair if its start point is greater than the end point of the last chosen pair in the chain.
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(1) if sorting in place, or O(n) for sorting workspace.
 */

#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        // Sort pairs by their SECOND element (right endpoint)
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });
        
        int chainCount = 0;
        long long curEnd = -1e18; // Use a tiny number as initial boundary
        
        for (const auto& pair : pairs) {
            // If the current pair starts after the previous one ends
            if (pair[0] > curEnd) {
                chainCount++;
                curEnd = pair[1]; // Move the boundary to this pair's end
            }
        }
        
        return chainCount;
    }
};