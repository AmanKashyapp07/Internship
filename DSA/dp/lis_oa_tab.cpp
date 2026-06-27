#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>
#include <cassert>

using namespace std;

/*
================================================================================
LIS (Longest Increasing Subsequence) Tabulation (Bottom-up 2D) Variants
================================================================================
*/

// =============================================================================
// LIS Tabulation Interface Functions
// =============================================================================

int lengthOfLISTab(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    
    for (int idx = n - 1; idx >= 0; --idx) {
        for (int prev_idx = idx - 1; prev_idx >= -1; --prev_idx) {
            int exclude = dp[idx + 1][prev_idx + 1];
            int include = 0;
            if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
                include = 1 + dp[idx + 1][idx + 1];
            }
            dp[idx][prev_idx + 1] = max(exclude, include);
        }
    }
    return dp[0][0];
}

vector<int> getLISTab(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    
    for (int idx = n - 1; idx >= 0; --idx) {
        for (int prev_idx = idx - 1; prev_idx >= -1; --prev_idx) {
            int exclude = dp[idx + 1][prev_idx + 1];
            int include = 0;
            if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
                include = 1 + dp[idx + 1][idx + 1];
            }
            dp[idx][prev_idx + 1] = max(exclude, include);
        }
    }
    
    vector<int> lis;
    int idx = 0, prev_idx = -1;
    while (idx < n) {
        int exclude = dp[idx + 1][prev_idx + 1];
        int include = 0;
        if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
            include = 1 + dp[idx + 1][idx + 1];
        }
        
        if (include >= exclude && (prev_idx == -1 || nums[idx] > nums[prev_idx])) {
            lis.push_back(nums[idx]);
            prev_idx = idx;
        }
        idx++;
    }
    return lis;
}

int maxEnvelopesTab(vector<vector<int>>& envelopes) {
    sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
        if (a[0] == b[0]) return a[1] > b[1];
        return a[0] < b[0];
    });
    
    int n = envelopes.size();
    vector<int> heights(n);
    for (int i = 0; i < n; ++i) heights[i] = envelopes[i][1];
    
    return lengthOfLISTab(heights);
}

int maxSumISTab(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    
    for (int idx = n - 1; idx >= 0; --idx) {
        for (int prev_idx = idx - 1; prev_idx >= -1; --prev_idx) {
            int exclude = dp[idx + 1][prev_idx + 1];
            int include = 0;
            if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
                include = nums[idx] + dp[idx + 1][idx + 1];
            }
            dp[idx][prev_idx + 1] = max(exclude, include);
        }
    }
    return dp[0][0];
}

int longestBitonicSequenceTab(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    
    vector<int> lis(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin(), nums.begin() + i + 1);
        lis[i] = lengthOfLISTab(sub);
    }
    
    vector<int> lds(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin() + i, nums.end());
        reverse(sub.begin(), sub.end());
        lds[i] = lengthOfLISTab(sub);
    }
    
    int maxBitonic = 0;
    for (int i = 0; i < n; ++i) {
        maxBitonic = max(maxBitonic, lis[i] + lds[i] - 1);
    }
    return maxBitonic;
}

int minimumMountainRemovalsTab(vector<int>& nums) {
    int n = nums.size();
    
    vector<int> lis(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin(), nums.begin() + i + 1);
        lis[i] = lengthOfLISTab(sub);
    }
    
    vector<int> lds(n, 1);
    for (int i = 0; i < n; ++i) {
        vector<int> sub(nums.begin() + i, nums.end());
        reverse(sub.begin(), sub.end());
        lds[i] = lengthOfLISTab(sub);
    }
    
    int maxMountainLen = 0;
    for (int i = 0; i < n; ++i) {
        if (lis[i] > 1 && lds[i] > 1) {
            maxMountainLen = max(maxMountainLen, lis[i] + lds[i] - 1);
        }
    }
    return n - maxMountainLen;
}

int findNumberOfLISTab(vector<int>& nums) {
    int n = nums.size();
    vector<vector<pair<int, int>>> dp(n + 1, vector<pair<int, int>>(n + 1, {0, 0}));
    
    for (int prev_idx = 0; prev_idx <= n; ++prev_idx) {
        dp[n][prev_idx] = {0, 1};
    }
    
    for (int idx = n - 1; idx >= 0; --idx) {
        for (int prev_idx = idx - 1; prev_idx >= -1; --prev_idx) {
            pair<int, int> exclude = dp[idx + 1][prev_idx + 1];
            
            pair<int, int> include = {0, 0};
            if (prev_idx == -1 || nums[idx] > nums[prev_idx]) {
                pair<int, int> nextRes = dp[idx + 1][idx + 1];
                include = {1 + nextRes.first, nextRes.second};
            }
            
            int maxLen = max(exclude.first, include.first);
            int ways = 0;
            if (exclude.first == maxLen) ways += exclude.second;
            if (include.first == maxLen) ways += include.second;
            
            dp[idx][prev_idx + 1] = {maxLen, ways};
        }
    }
    return dp[0][0].second;
}

int lengthOfLNDSTab(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    
    for (int idx = n - 1; idx >= 0; --idx) {
        for (int prev_idx = idx - 1; prev_idx >= -1; --prev_idx) {
            int exclude = dp[idx + 1][prev_idx + 1];
            int include = 0;
            if (prev_idx == -1 || nums[idx] >= nums[prev_idx]) {
                include = 1 + dp[idx + 1][idx + 1];
            }
            dp[idx][prev_idx + 1] = max(exclude, include);
        }
    }
    return dp[0][0];
}

bool increasingTripletTab(vector<int>& nums) {
    return lengthOfLISTab(nums) >= 3;
}

int findLongestChainTab(vector<vector<int>>& pairs) {
    sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });
    int n = pairs.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    
    for (int idx = n - 1; idx >= 0; --idx) {
        for (int prev_idx = idx - 1; prev_idx >= -1; --prev_idx) {
            int exclude = dp[idx + 1][prev_idx + 1];
            int include = 0;
            if (prev_idx == -1 || pairs[idx][0] > pairs[prev_idx][1]) {
                include = 1 + dp[idx + 1][idx + 1];
            }
            dp[idx][prev_idx + 1] = max(exclude, include);
        }
    }
    return dp[0][0];
}

// =============================================================================
// Local Verification for Tabulation
// =============================================================================
int main() {
    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    
    assert(lengthOfLISTab(nums) == 4);
    
    vector<int> lisTab = getLISTab(nums);
    assert(lisTab.size() == 4);
    
    vector<vector<int>> envs = {{5, 4}, {6, 4}, {6, 7}, {2, 3}};
    assert(maxEnvelopesTab(envs) == 3);
    
    vector<int> numsMSIS = {1, 101, 2, 3, 100, 4, 5};
    assert(maxSumISTab(numsMSIS) == 106);
    
    vector<int> bitonicNums = {1, 11, 2, 10, 4, 5, 2, 1};
    assert(longestBitonicSequenceTab(bitonicNums) == 6);
    
    vector<int> mountainNums = {2, 1, 1, 5, 6, 2, 3, 1};
    assert(minimumMountainRemovalsTab(mountainNums) == 3);
    
    vector<int> numLISNums = {1, 3, 5, 4, 7};
    assert(findNumberOfLISTab(numLISNums) == 2);
    
    vector<int> lndsNums = {1, 2, 2, 3};
    assert(lengthOfLNDSTab(lndsNums) == 4);
    
    vector<int> tripletNums = {2, 1, 5, 0, 4, 6};
    assert(increasingTripletTab(tripletNums) == true);
    
    vector<vector<int>> pairs = {{1, 2}, {7, 8}, {4, 5}};
    assert(findLongestChainTab(pairs) == 3);
    
    cout << "All LIS Tabulation tests passed successfully!" << endl;
    return 0;
}
