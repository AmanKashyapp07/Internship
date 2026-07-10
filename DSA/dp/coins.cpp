/**
 * CSES 1745 - Money Sums
 *
 * Description:
 * You are given n coins with positive integer values. 
 * What are all the possible sums you can form using these coins?
 * Output the count of distinct possible sums (excluding 0) followed by the sums in ascending order.
 *
 * Approach: 0/1 Knapsack DP (Subset Sum)
 * Time Complexity: O(n * sum)
 * Space Complexity: O(sum)
 */

#include <vector>
using namespace std;

class Solution {
public:
    vector<int> getMoneySums(vector<int>& coins) {
        int n = coins.size();
        if (n == 0) return {};
        
        int total = 0;
        for (int c : coins) {
            total += c;
        }
        
        vector<bool> dp(total + 1, false);
        dp[0] = true;
        
        // Update DP table backwards to use each coin at most once
        for (int coin : coins) {
            for (int sum = total; sum >= coin; sum--) {
                if (dp[sum - coin]) {
                    dp[sum] = true;
                }
            }
        }
        
        vector<int> possibleSums;
        for (int s = 1; s <= total; s++) {
            if (dp[s]) {
                possibleSums.push_back(s);
            }
        }
        
        return possibleSums;
    }
};