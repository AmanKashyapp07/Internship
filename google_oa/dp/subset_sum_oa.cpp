#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> arr;
    vector<vector<int>> memo;

    // =============================================================================
    // MEMOIZATION ARCHITECTURE (Top-Down)
    // =============================================================================

    bool subsetSumMemoHelper(int idx, int target) {
        if (target == 0) return true;
        if (idx == arr.size()) return false;
        if (memo[idx][target] != -1) return memo[idx][target];
        
        bool exclude = subsetSumMemoHelper(idx + 1, target);
        bool include = false;
        if (arr[idx] <= target) {
            include = subsetSumMemoHelper(idx + 1, target - arr[idx]);
        }
        return memo[idx][target] = (exclude || include);
    }

    int countSubsetsMemoHelper(int idx, int target) {
        if (idx == arr.size()) return target == 0 ? 1 : 0;
        if (memo[idx][target] != -1) return memo[idx][target];
        
        int exclude = countSubsetsMemoHelper(idx + 1, target);
        int include = 0;
        if (arr[idx] <= target) {
            include = countSubsetsMemoHelper(idx + 1, target - arr[idx]);
        }
        return memo[idx][target] = exclude + include;
    }

    // --- Public Memoization Interfaces ---

    bool isSubsetSumPossibleMemo(const vector<int>& nums, int target) {
        arr = nums;
        int n = nums.size();
        memo.assign(n, vector<int>(target + 1, -1));
        return subsetSumMemoHelper(0, target);
    }

    bool canPartitionEqualSumMemo(const vector<int>& nums) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if (totalSum % 2 != 0) return false;
        return isSubsetSumPossibleMemo(nums, totalSum / 2);
    }

    int countSubsetsWithSumMemo(const vector<int>& nums, int target) {
        arr = nums;
        int n = nums.size();
        memo.assign(n, vector<int>(target + 1, -1));
        return countSubsetsMemoHelper(0, target);
    }

    int minSubsetSumDifferenceMemo(const vector<int>& nums) {
        arr = nums;
        int n = nums.size();
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        int halfSum = totalSum / 2;
        
        memo.assign(n, vector<int>(halfSum + 1, -1));
        subsetSumMemoHelper(0, halfSum);
        
        int maxPossibleSum = 0;
        for (int s = halfSum; s >= 0; s--) {
            if (subsetSumMemoHelper(0, s)) {
                maxPossibleSum = s;
                break;
            }
        }
        return abs(maxPossibleSum - (totalSum - maxPossibleSum));
    }

    int findTargetSumWaysMemo(const vector<int>& nums, int target) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if ((totalSum + target) % 2 != 0 || totalSum + target < 0) return 0;
        return countSubsetsWithSumMemo(nums, (totalSum + target) / 2);
    }

    int countSubsetsWithDifferenceMemo(const vector<int>& nums, int diff) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if ((totalSum + diff) % 2 != 0 || totalSum + diff < 0) return 0;
        return countSubsetsWithSumMemo(nums, (totalSum + diff) / 2);
    }

    // =============================================================================
    // TABULATION ARCHITECTURE (Bottom-Up)
    // =============================================================================

    bool isSubsetSumPossibleTab(const vector<int>& nums, int target) {
        int n = nums.size();
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
        for (int i = 0; i <= n; i++) dp[i][0] = true;
        
        for (int i = 1; i <= n; i++) {
            for (int t = 1; t <= target; t++) {
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t) {
                    dp[i][t] = dp[i][t] || dp[i - 1][t - nums[i - 1]];
                }
            }
        }
        return dp[n][target];
    }

    bool canPartitionEqualSumTab(const vector<int>& nums) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if (totalSum % 2 != 0) return false;
        return isSubsetSumPossibleTab(nums, totalSum / 2);
    }

    int countSubsetsWithSumTab(const vector<int>& nums, int target) {
        int n = nums.size();
        vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
        dp[0][0] = 1;
        
        for (int i = 1; i <= n; i++) {
            for (int t = 0; t <= target; t++) {
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t) {
                    dp[i][t] += dp[i - 1][t - nums[i - 1]];
                }
            }
        }
        return dp[n][target];
    }

    int minSubsetSumDifferenceTab(const vector<int>& nums) {
        int n = nums.size();
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        int halfSum = totalSum / 2;
        
        vector<vector<bool>> dp(n + 1, vector<bool>(halfSum + 1, false));
        for (int i = 0; i <= n; i++) dp[i][0] = true;
        
        for (int i = 1; i <= n; i++) {
            for (int t = 1; t <= halfSum; t++) {
                dp[i][t] = dp[i - 1][t];
                if (nums[i - 1] <= t) {
                    dp[i][t] = dp[i][t] || dp[i - 1][t - nums[i - 1]];
                }
            }
        }
        
        int maxPossibleSum = 0;
        for (int s = halfSum; s >= 0; s--) {
            if (dp[n][s]) {
                maxPossibleSum = s;
                break;
            }
        }
        return abs(maxPossibleSum - (totalSum - maxPossibleSum));
    }

    int findTargetSumWaysTab(const vector<int>& nums, int target) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if ((totalSum + target) % 2 != 0 || totalSum + target < 0) return 0;
        return countSubsetsWithSumTab(nums, (totalSum + target) / 2);
    }

    int countSubsetsWithDifferenceTab(const vector<int>& nums, int diff) {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        if ((totalSum + diff) % 2 != 0 || totalSum + diff < 0) return 0;
        return countSubsetsWithSumTab(nums, (totalSum + diff) / 2);
    }
};

// =============================================================================
// VERIFICATION ENVIRONMENT
// =============================================================================
int main() {
    Solution solver;

    vector<int> nums1 = {2, 3, 7, 8, 10};
    assert(solver.isSubsetSumPossibleMemo(nums1, 11) == true);
    assert(solver.isSubsetSumPossibleTab(nums1, 11) == true);
    assert(solver.isSubsetSumPossibleMemo(nums1, 14) == false);
    assert(solver.isSubsetSumPossibleTab(nums1, 14) == false);

    vector<int> nums2 = {1, 5, 11, 5};
    assert(solver.canPartitionEqualSumMemo(nums2) == true);
    assert(solver.canPartitionEqualSumTab(nums2) == true);

    vector<int> nums3 = {1, 2, 3, 3};
    assert(solver.countSubsetsWithSumMemo(nums3, 6) == 3);
    assert(solver.countSubsetsWithSumTab(nums3, 6) == 3);

    vector<int> nums4 = {1, 6, 11, 5};
    assert(solver.minSubsetSumDifferenceMemo(nums4) == 1);
    assert(solver.minSubsetSumDifferenceTab(nums4) == 1);

    vector<int> nums5 = {1, 1, 2, 3};
    assert(solver.countSubsetsWithDifferenceMemo(nums5, 1) == 3);
    assert(solver.countSubsetsWithDifferenceTab(nums5, 1) == 3);

    cout << "All Tier-1 Subset Sum and Partitioning variants passed successfully!" << endl;
    return 0;
}