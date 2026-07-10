#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

class Solution {
public:
    // Core Tabulation Functions

    bool isSubsetSumPossibleTab(const vector<int>& nums, int target) {
        int n = nums.size();
        if (target < 0) return false;

        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false)); // dp[i][t] = true if we can achieve sum t using first i elements
        for(int i=0;i<=n;i++) dp[i][0] = true; // 0 sum is always possible with empty subset

        for(int i=1;i<=n;i++){
            for(int t=0;t<=target;t++){
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t)
                    dp[i][t] = dp[i][t] || dp[i - 1][t - nums[i - 1]];
            }
        }
        return dp[n][target];
    }

    int countSubsetsWithSumTab(const vector<int>& nums, int target) {
        int n = nums.size();
        if (target < 0) return 0;

        vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
        dp[0][0] = 1;

        for (int i = 1; i <= n; ++i) {
            for (int t = 0; t <= target; ++t) {
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t)
                    dp[i][t] += dp[i - 1][t - nums[i - 1]];
            }
        }
        return dp[n][target];
    }

    int minSubsetSumDifferenceTab(const vector<int>& nums) {
        int total = accumulate(nums.begin(), nums.end(), 0);
        int half = total / 2;
        int n = nums.size();

        vector<vector<bool>> dp(n + 1, vector<bool>(half + 1, false));
        for (int i = 0; i <= n; ++i) dp[i][0] = true;

        for (int i = 1; i <= n; ++i) {
            for (int t = 0; t <= half; ++t) {
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t)
                    dp[i][t] = dp[i][t] || dp[i - 1][t - nums[i - 1]];
            }
        }

        for(int sum=half; sum>=0; --sum){
            if(dp[n][sum]){
                return total - 2*sum;
            }
        }
        return total;
    }
};

int main() {
}