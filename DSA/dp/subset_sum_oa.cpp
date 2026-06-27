/**
 * Subset Sum DP Variants
 *
 * This file contains comprehensive implementations of all major Subset Sum 
 * dynamic programming variants commonly asked in software engineering interviews 
 * and online assessments (OAs). 
 *
 * Each variant is implemented with both:
 * 1. Memoization (Top-down)
 * 2. Tabulation (Bottom-up)
 *
 * Variants covered:
 * 1. Subset Sum Problem (Basic)
 * 2. Partition Equal Subset Sum (LeetCode 416)
 * 3. Count of Subsets with Target Sum (Zero-robust)
 * 4. Minimum Subset Sum Difference
 * 5. Target Sum (LeetCode 494)
 * 6. Count Subsets with Given Difference
 */

#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

// =============================================================================
// Helper Functions for Memoization
// =============================================================================

// Helper for Subset Sum / Partition / Min Difference memoization
bool subsetSumMemoHelper(int idx, int target, const vector<int>& nums, vector<vector<int>>& dp) {
    if (target == 0) return true;
    if (idx == nums.size()) return false;
    
    if (dp[idx][target] != -1) return dp[idx][target];
    
    // Option 1: Exclude
    bool exclude = subsetSumMemoHelper(idx + 1, target, nums, dp);
    
    // Option 2: Include
    bool include = false;
    if (nums[idx] <= target) {
        include = subsetSumMemoHelper(idx + 1, target - nums[idx], nums, dp);
    }
    
    return dp[idx][target] = (exclude || include);
}

// Helper for Count of Subsets / Target Sum / Difference memoization (zero-robust)
int countSubsetsMemoHelper(int idx, int target, const vector<int>& nums, vector<vector<int>>& dp) {
    if (idx == nums.size()) {
        return target == 0 ? 1 : 0;
    }
    
    if (dp[idx][target] != -1) return dp[idx][target];
    
    // Option 1: Exclude
    int exclude = countSubsetsMemoHelper(idx + 1, target, nums, dp);
    
    // Option 2: Include
    int include = 0;
    if (nums[idx] <= target) {
        include = countSubsetsMemoHelper(idx + 1, target - nums[idx], nums, dp);
    }
    
    return dp[idx][target] = exclude + include;
}

// =============================================================================
// 1. Subset Sum Problem
// =============================================================================

bool isSubsetSumPossibleMemo(const vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(target + 1, -1));
    return subsetSumMemoHelper(0, target, nums, dp);
}

bool isSubsetSumPossibleTab(const vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
    
    // Base Case: target 0 is always possible
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= target; j++) {
            dp[i][j] = dp[i - 1][j]; // Exclude
            if (nums[i - 1] <= j) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - nums[i - 1]]; // Include
            }
        }
    }
    return dp[n][target];
}

// =============================================================================
// 2. Partition Equal Subset Sum
// =============================================================================

bool canPartitionEqualSumMemo(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if (totalSum % 2 != 0) return false;
    return isSubsetSumPossibleMemo(nums, totalSum / 2);
}

bool canPartitionEqualSumTab(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if (totalSum % 2 != 0) return false;
    return isSubsetSumPossibleTab(nums, totalSum / 2);
}

// =============================================================================
// 3. Count of Subsets with Target Sum
// =============================================================================

int countSubsetsWithSumMemo(const vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(target + 1, -1));
    return countSubsetsMemoHelper(0, target, nums, dp);
}

int countSubsetsWithSumTab(const vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
    dp[0][0] = 1;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= target; j++) {
            dp[i][j] = dp[i - 1][j]; // Exclude
            if (nums[i - 1] <= j) {
                dp[i][j] += dp[i - 1][j - nums[i - 1]]; // Include
            }
        }
    }
    return dp[n][target];
}

// =============================================================================
// 4. Minimum Subset Sum Difference
// =============================================================================

int minSubsetSumDifferenceMemo(const vector<int>& nums) {
    int n = nums.size();
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    int halfSum = totalSum / 2;
    
    vector<vector<int>> dp(n, vector<int>(halfSum + 1, -1));
    
    // Find the largest possible subset sum <= totalSum / 2
    int maxPossibleSum = 0;
    for (int s = halfSum; s >= 0; s--) {
        if (subsetSumMemoHelper(0, s, nums, dp)) {
            maxPossibleSum = s;
            break;
        }
    }
    
    int s1 = maxPossibleSum;
    int s2 = totalSum - s1;
    return abs(s1 - s2);
}

int minSubsetSumDifferenceTab(const vector<int>& nums) {
    int n = nums.size();
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    int halfSum = totalSum / 2;
    
    vector<vector<bool>> dp(n + 1, vector<bool>(halfSum + 1, false));
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }
    
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
    
    int s1 = maxPossibleSum;
    int s2 = totalSum - s1;
    return abs(s1 - s2);
}

// =============================================================================
// 5. Target Sum (LeetCode 494)
// =============================================================================

int findTargetSumWaysMemo(const vector<int>& nums, int target) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if ((totalSum + target) % 2 != 0 || totalSum + target < 0) {
        return 0;
    }
    
    int subsetTarget = (totalSum + target) / 2; 
    return countSubsetsWithSumMemo(nums, subsetTarget);
}

int findTargetSumWaysTab(const vector<int>& nums, int target) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if ((totalSum + target) % 2 != 0 || totalSum + target < 0) {
        return 0;
    }
    
    int subsetTarget = (totalSum + target) / 2;
    return countSubsetsWithSumTab(nums, subsetTarget);
}

// =============================================================================
// 6. Count Subsets with Given Difference
// =============================================================================

int countSubsetsWithDifferenceMemo(const vector<int>& nums, int diff) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if ((totalSum + diff) % 2 != 0 || totalSum + diff < 0) {
        return 0;
    }
    
    int subsetTarget = (totalSum + diff) / 2;
    return countSubsetsWithSumMemo(nums, subsetTarget);
}

int countSubsetsWithDifferenceTab(const vector<int>& nums, int diff) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if ((totalSum + diff) % 2 != 0 || totalSum + diff < 0) {
        return 0;
    }
    
    int subsetTarget = (totalSum + diff) / 2;
    return countSubsetsWithSumTab(nums, subsetTarget);
}

// =============================================================================
// Driver Main for Verification
// =============================================================================
int main() {
    // 1. Test Case for Basic Subset Sum
    {
        vector<int> nums = {3, 34, 4, 12, 5, 2};
        int target = 9;
        assert(isSubsetSumPossibleMemo(nums, target) == true);
        assert(isSubsetSumPossibleTab(nums, target) == true);
        
        target = 30;
        assert(isSubsetSumPossibleMemo(nums, target) == false);
        assert(isSubsetSumPossibleTab(nums, target) == false);
        cout << "Variant 1 (Subset Sum) passed!" << endl;
    }

    // 2. Test Case for Partition Equal Subset Sum
    {
        vector<int> nums1 = {1, 5, 11, 5};
        vector<int> nums2 = {1, 2, 3, 5};
        assert(canPartitionEqualSumMemo(nums1) == true);
        assert(canPartitionEqualSumTab(nums1) == true);
        assert(canPartitionEqualSumMemo(nums2) == false);
        assert(canPartitionEqualSumTab(nums2) == false);
        cout << "Variant 2 (Partition Equal Subset Sum) passed!" << endl;
    }

    // 3. Test Case for Count of Subsets with Target Sum (handling zeroes)
    {
        vector<int> nums = {1, 2, 0, 3};
        int target = 3;
        // Expected subsets: {1, 2}, {1, 2, 0}, {3}, {3, 0} => 4 subsets
        assert(countSubsetsWithSumMemo(nums, target) == 4);
        assert(countSubsetsWithSumTab(nums, target) == 4);
        cout << "Variant 3 (Count Subsets with Sum) passed!" << endl;
    }

    // 4. Test Case for Minimum Subset Sum Difference
    {
        vector<int> nums = {1, 6, 11, 5};
        // Expected partitions: {1, 5, 6} (sum 12) and {11} (sum 11). Min diff = 1.
        assert(minSubsetSumDifferenceMemo(nums) == 1);
        assert(minSubsetSumDifferenceTab(nums) == 1);
        cout << "Variant 4 (Minimum Subset Sum Difference) passed!" << endl;
    }

    // 5. Test Case for Target Sum
    {
        vector<int> nums = {1, 1, 1, 1, 1};
        int target = 3;
        // There are 5 ways to assign signs to make target sum 3.
        assert(findTargetSumWaysMemo(nums, target) == 5);
        assert(findTargetSumWaysTab(nums, target) == 5);
        cout << "Variant 5 (Target Sum) passed!" << endl;
    }

    // 6. Test Case for Count Subsets with Given Difference
    {
        vector<int> nums = {1, 1, 2, 3};
        int diff = 1;
        // S1 - S2 = 1. S1 + S2 = 7. S1 = (7 + 1)/2 = 4. 
        // Subsets summing to 4: {1, 3} (using first 1), {1, 3} (using second 1), {1, 1, 2} => 3 ways.
        assert(countSubsetsWithDifferenceMemo(nums, diff) == 3);
        assert(countSubsetsWithDifferenceTab(nums, diff) == 3);
        cout << "Variant 6 (Count Subsets with Difference) passed!" << endl;
    }

    cout << "\nAll independent subset sum variants passed successfully!" << endl;
    return 0;
}
