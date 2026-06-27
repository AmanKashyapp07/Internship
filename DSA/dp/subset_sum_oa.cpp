#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

// =============================================================================
// MEMOIZATION ARCHITECTURE (Top-Down)
// =============================================================================

bool subsetSumMemoHelper(int idx, int target, const vector<int>& nums, vector<vector<int>>& dp) {
    if (target == 0) return true;
    if (idx == nums.size()) return false;
    if (dp[idx][target] != -1) return dp[idx][target];
    
    bool exclude = subsetSumMemoHelper(idx + 1, target, nums, dp);
    bool include = false;
    if (nums[idx] <= target) {
        include = subsetSumMemoHelper(idx + 1, target - nums[idx], nums, dp);
    }
    return dp[idx][target] = (exclude || include);
}

int countSubsetsMemoHelper(int idx, int target, const vector<int>& nums, vector<vector<int>>& dp) {
    if (idx == nums.size()) return target == 0 ? 1 : 0;
    if (dp[idx][target] != -1) return dp[idx][target];
    
    int exclude = countSubsetsMemoHelper(idx + 1, target, nums, dp);
    int include = 0;
    if (nums[idx] <= target) {
        include = countSubsetsMemoHelper(idx + 1, target - nums[idx], nums, dp);
    }
    return dp[idx][target] = exclude + include;
}

// --- Public Memoization Interfaces ---

bool isSubsetSumPossibleMemo(const vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(target + 1, -1));
    return subsetSumMemoHelper(0, target, nums, dp);
}

bool canPartitionEqualSumMemo(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if (totalSum % 2 != 0) return false;
    return isSubsetSumPossibleMemo(nums, totalSum / 2);
}

int countSubsetsWithSumMemo(const vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(target + 1, -1));
    return countSubsetsMemoHelper(0, target, nums, dp);
}

int minSubsetSumDifferenceMemo(const vector<int>& nums) {
    int n = nums.size();
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    int halfSum = totalSum / 2;
    
    vector<vector<int>> dp(n, vector<int>(halfSum + 1, -1));
    // Single optimization pass initializes table elements sequentially
    subsetSumMemoHelper(0, halfSum, nums, dp);
    
    int maxPossibleSum = 0;
    for (int s = halfSum; s >= 0; s--) {
        if (subsetSumMemoHelper(0, s, nums, dp)) {
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
        for (int j = 1; j <= target; j++) {
            dp[i][j] = dp[i - 1][j];
            if (nums[i - 1] <= j) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - nums[i - 1]];
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
        for (int j = 0; j <= target; j++) {
            dp[i][j] = dp[i - 1][j];
            if (nums[i - 1] <= j) {
                dp[i][j] += dp[i - 1][j - nums[i - 1]];
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
        for (int j = 1; j <= halfSum; j++) {
            dp[i][j] = dp[i - 1][j];
            if (nums[i - 1] <= j) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - nums[i - 1]];
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


// =============================================================================
// Verification Execution Block
// =============================================================================
int main() {
    // 1. Basic Subset Sum Validation
    {
        vector<int> nums = {3, 34, 4, 12, 5, 2};
        assert(isSubsetSumPossibleMemo(nums, 9) == true);
        assert(isSubsetSumPossibleTab(nums, 9) == true);
        assert(isSubsetSumPossibleMemo(nums, 30) == false);
        assert(isSubsetSumPossibleTab(nums, 30) == false);
    }

    // 2. Partition Equal Subset Sum Validation
    {
        vector<int> nums1 = {1, 5, 11, 5};
        vector<int> nums2 = {1, 2, 3, 5};
        assert(canPartitionEqualSumMemo(nums1) == true);
        assert(canPartitionEqualSumTab(nums1) == true);
        assert(canPartitionEqualSumMemo(nums2) == false);
        assert(canPartitionEqualSumTab(nums2) == false);
    }

    // 3. Count Subsets Validation (Zero Handling Checked)
    {
        vector<int> nums = {1, 2, 0, 3};
        assert(countSubsetsWithSumMemo(nums, 3) == 4);
        assert(countSubsetsWithSumTab(nums, 3) == 4);
    }

    // 4. Minimum Subset Sum Difference Validation
    {
        vector<int> nums = {1, 6, 11, 5};
        assert(minSubsetSumDifferenceMemo(nums) == 1);
        assert(minSubsetSumDifferenceTab(nums) == 1);
    }

    // 5. Target Sum Assignment Validation
    {
        vector<int> nums = {1, 1, 1, 1, 1};
        assert(findTargetSumWaysMemo(nums, 3) == 5);
        assert(findTargetSumWaysTab(nums, 3) == 5);
    }

    // 6. Count Subsets with Given Difference Validation
    {
        vector<int> nums = {1, 1, 2, 3};
        assert(countSubsetsWithDifferenceMemo(nums, 1) == 3);
        assert(countSubsetsWithDifferenceTab(nums, 1) == 3);
    }

    cout << "All clear Subset Sum Memoization and Tabulation tests passed successfully!" << endl;
    return 0;
}