#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>
#include <cassert>

using namespace std;

/*
================================================================================
LIS (Longest Increasing Subsequence) Memoization (Top-down) Variants
================================================================================
*/

// =============================================================================
// Helper Functions for LIS Memoization
// =============================================================================

int classicLISMemoHelper(int idx, int prev_idx, const vector<int>& nums, vector<vector<int>>& dp) {
    if (idx == nums.size()) return 0;
    if (dp[idx][prev_idx + 1] != -1) return dp[idx][prev_idx + 1];
    
    int exclude = classicLISMemoHelper(idx + 1, prev_idx, nums, dp);
    int include = 0;
    if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
        include = 1 + classicLISMemoHelper(idx + 1, idx, nums, dp);
    }
    
    return dp[idx][prev_idx + 1] = max(exclude, include);
}

int maxSumISMemoHelper(int idx, int prev_idx, const vector<int>& nums, vector<vector<int>>& dp) {
    if (idx == nums.size()) return 0;
    if (dp[idx][prev_idx + 1] != -1) return dp[idx][prev_idx + 1];
    
    int exclude = maxSumISMemoHelper(idx + 1, prev_idx, nums, dp);
    int include = 0;
    if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
        include = nums[idx] + maxSumISMemoHelper(idx + 1, idx, nums, dp);
    }
    
    return dp[idx][prev_idx + 1] = max(exclude, include);
}

pair<int, int> numberOfLISMemoHelper(int idx, int prev_idx, const vector<int>& nums, vector<vector<pair<int, int>>>& dp) {
    if (idx == nums.size()) return {0, 1};
    if (dp[idx][prev_idx + 1].first != -1) return dp[idx][prev_idx + 1];
    
    pair<int, int> exclude = numberOfLISMemoHelper(idx + 1, prev_idx, nums, dp);
    
    pair<int, int> include = {0, 0};
    if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
        pair<int, int> nextRes = numberOfLISMemoHelper(idx + 1, idx, nums, dp);
        include = {1 + nextRes.first, nextRes.second};
    }
    
    int maxLen = max(exclude.first, include.first);
    int ways = 0;
    if (exclude.first == maxLen) ways += exclude.second;
    if (include.first == maxLen) ways += include.second;
    
    return dp[idx][prev_idx + 1] = {maxLen, ways};
}

int lndsMemoHelper(int idx, int prev_idx, const vector<int>& nums, vector<vector<int>>& dp) {
    if (idx == nums.size()) return 0;
    if (dp[idx][prev_idx + 1] != -1) return dp[idx][prev_idx + 1];
    
    int exclude = lndsMemoHelper(idx + 1, prev_idx, nums, dp);
    int include = 0;
    if (prev_idx == -1 || nums[idx] >= nums[prev_idx]) {
        include = 1 + lndsMemoHelper(idx + 1, idx, nums, dp);
    }
    
    return dp[idx][prev_idx + 1] = max(exclude, include);
}

int chainPairsMemoHelper(int idx, int prev_idx, const vector<vector<int>>& pairs, vector<vector<int>>& dp) {
    if (idx == pairs.size()) return 0;
    if (dp[idx][prev_idx + 1] != -1) return dp[idx][prev_idx + 1];
    
    int exclude = chainPairsMemoHelper(idx + 1, prev_idx, pairs, dp);
    int include = 0;
    if (prev_idx == -1 || pairs[idx][0] > pairs[prev_idx][1]) {
        include = 1 + chainPairsMemoHelper(idx + 1, idx, pairs, dp);
    }
    
    return dp[idx][prev_idx + 1] = max(exclude, include);
}

// =============================================================================
// LIS Memoization Interface Functions
// =============================================================================

int lengthOfLISMemo(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    return classicLISMemoHelper(0, -1, nums, dp);
}

vector<int> getLISMemo(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    classicLISMemoHelper(0, -1, nums, dp);
    
    vector<int> lis;
    int idx = 0, prev_idx = -1;
    while (idx < n) {
        int exclude = (idx + 1 < n) ? dp[idx + 1][prev_idx + 1] : 0;
        int include = 0;
        if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
            include = 1 + ((idx + 1 < n) ? dp[idx + 1][idx + 1] : 0);
        }
        
        if (include >= exclude && (prev_idx == -1 || nums[idx] > nums[prev_idx])) {
            lis.push_back(nums[idx]);
            prev_idx = idx;
        }
        idx++;
    }
    return lis;
}

int maxEnvelopesMemo(vector<vector<int>>& envelopes) {
    sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
        if (a[0] == b[0]) return a[1] > b[1];
        return a[0] < b[0];
    });
    
    int n = envelopes.size();
    vector<int> heights(n);
    for (int i = 0; i < n; ++i) heights[i] = envelopes[i][1];
    
    return lengthOfLISMemo(heights);
}

int maxSumISMemo(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    return maxSumISMemoHelper(0, -1, nums, dp);
}

int longestBitonicSequenceMemo(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    
    vector<int> lis(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin(), nums.begin() + i + 1);
        vector<vector<int>> dp(sub.size(), vector<int>(sub.size() + 1, -1));
        lis[i] = classicLISMemoHelper(0, -1, sub, dp);
    }
    
    vector<int> lds(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin() + i, nums.end());
        reverse(sub.begin(), sub.end());
        vector<vector<int>> dp(sub.size(), vector<int>(sub.size() + 1, -1));
        lds[i] = classicLISMemoHelper(0, -1, sub, dp);
    }
    
    int maxBitonic = 0;
    for (int i = 0; i < n; ++i) {
        maxBitonic = max(maxBitonic, lis[i] + lds[i] - 1);
    }
    return maxBitonic;
}

int minimumMountainRemovalsMemo(vector<int>& nums) {
    int n = nums.size();
    
    vector<int> lis(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin(), nums.begin() + i + 1);
        vector<vector<int>> dp(sub.size(), vector<int>(sub.size() + 1, -1));
        lis[i] = classicLISMemoHelper(0, -1, sub, dp);
    }
    
    vector<int> lds(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin() + i, nums.end());
        reverse(sub.begin(), sub.end());
        vector<vector<int>> dp(sub.size(), vector<int>(sub.size() + 1, -1));
        lds[i] = classicLISMemoHelper(0, -1, sub, dp);
    }
    
    int maxMountainLen = 0;
    for (int i = 0; i < n; ++i) {
        if (lis[i] > 1 && lds[i] > 1) {
            maxMountainLen = max(maxMountainLen, lis[i] + lds[i] - 1);
        }
    }
    return n - maxMountainLen;
}

int findNumberOfLISMemo(vector<int>& nums) {
    int n = nums.size();
    vector<vector<pair<int, int>>> dp(n, vector<pair<int, int>>(n + 1, {-1, -1}));
    return numberOfLISMemoHelper(0, -1, nums, dp).second;
}

int lengthOfLNDSMemo(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    return lndsMemoHelper(0, -1, nums, dp);
}

bool increasingTripletMemo(vector<int>& nums) {
    return lengthOfLISMemo(nums) >= 3;
}

int findLongestChainMemo(vector<vector<int>>& pairs) {
    sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });
    int n = pairs.size();
    vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    return chainPairsMemoHelper(0, -1, pairs, dp);
}

// =============================================================================
// Local Verification for Memoization
// =============================================================================
int main() {
    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    
    assert(lengthOfLISMemo(nums) == 4);
    
    vector<int> lisMemo = getLISMemo(nums);
    assert(lisMemo.size() == 4);
    
    vector<vector<int>> envs = {{5, 4}, {6, 4}, {6, 7}, {2, 3}};
    assert(maxEnvelopesMemo(envs) == 3);
    
    vector<int> numsMSIS = {1, 101, 2, 3, 100, 4, 5};
    assert(maxSumISMemo(numsMSIS) == 106);
    
    vector<int> bitonicNums = {1, 11, 2, 10, 4, 5, 2, 1};
    assert(longestBitonicSequenceMemo(bitonicNums) == 6);
    
    vector<int> mountainNums = {2, 1, 1, 5, 6, 2, 3, 1};
    assert(minimumMountainRemovalsMemo(mountainNums) == 3);
    
    vector<int> numLISNums = {1, 3, 5, 4, 7};
    assert(findNumberOfLISMemo(numLISNums) == 2);
    
    vector<int> lndsNums = {1, 2, 2, 3};
    assert(lengthOfLNDSMemo(lndsNums) == 4);
    
    vector<int> tripletNums = {2, 1, 5, 0, 4, 6};
    assert(increasingTripletMemo(tripletNums) == true);
    
    vector<vector<int>> pairs = {{1, 2}, {7, 8}, {4, 5}};
    assert(findLongestChainMemo(pairs) == 3);
    
    cout << "All LIS Memoization tests passed successfully!" << endl;
    return 0;
}
