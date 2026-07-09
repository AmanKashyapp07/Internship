#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

using namespace std;

class Solution {
public:
    vector<int> arr;
    vector<int> memo;
    vector<int> par;
    vector<vector<int>> envelopeList;
    vector<int> lisVal;
    vector<int> ldsVal;
    vector<int> length;
    vector<int> cnt;
    vector<vector<int>> pairList;

    // =============================================================================
    // 1. Classic LIS - Length & String/Array Reconstruction
    // =============================================================================

    int lengthOfLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        arr = nums;
        memo.assign(n, 1);
        int maxLen = 1;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) {
                    memo[i] = max(memo[i], memo[j] + 1);
                }
            }
            maxLen = max(maxLen, memo[i]);
        }
        return maxLen;
    }

    vector<int> getLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return {};

        arr = nums;
        memo.assign(n, 1);
        par.assign(n, 0);
        for (int i = 0; i < n; ++i) par[i] = i;

        int lastIdx = 0;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i] && memo[j] + 1 > memo[i]) {
                    memo[i] = memo[j] + 1;
                    par[i] = j;
                }
            }
            if (memo[i] > memo[lastIdx]) {
                lastIdx = i;
            }
        }

        vector<int> lis;
        while (par[lastIdx] != lastIdx) {
            lis.push_back(arr[lastIdx]);
            lastIdx = par[lastIdx];
        }
        lis.push_back(arr[lastIdx]);

        reverse(lis.begin(), lis.end());
        return lis;
    }

    // =============================================================================
    // 2. Russian Doll Envelopes (2D LIS Variant)
    // =============================================================================

    int maxEnvelopes(vector<vector<int>>& envelopes) {
        if (envelopes.empty()) return 0;
        
        envelopeList = envelopes;
        sort(envelopeList.begin(), envelopeList.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] == b[0]) return a[1] > b[1];
            return a[0] < b[0];
        });
        
        int n = envelopeList.size();
        memo.assign(n, 1);
        int maxEnvs = 1;
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (envelopeList[j][1] < envelopeList[i][1]) {
                    memo[i] = max(memo[i], memo[j] + 1);
                }
            }
            maxEnvs = max(maxEnvs, memo[i]);
        }
        return maxEnvs;
    }

    // =============================================================================
    // 3. Maximum Sum Increasing Subsequence (MSIS)
    // =============================================================================

    int maxSumIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        arr = nums;
        memo.assign(arr.begin(), arr.end());
        int maxSum = memo[0];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) {
                    memo[i] = max(memo[i], memo[j] + arr[i]);
                }
            }
            maxSum = max(maxSum, memo[i]);
        }
        return maxSum;
    }

    // =============================================================================
    // 4. Longest Bitonic Subsequence (Optimized Pass)
    // =============================================================================

    int longestBitonicSequence(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        arr = nums;
        lisVal.assign(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) lisVal[i] = max(lisVal[i], lisVal[j] + 1);
            }
        }
        
        ldsVal.assign(n, 1);
        for (int i = n - 1; i >= 0; --i) {
            for (int j = n - 1; j > i; --j) {
                if (arr[j] < arr[i]) ldsVal[i] = max(ldsVal[i], ldsVal[j] + 1);
            }
        }
        
        int maxBitonic = 0;
        for (int i = 0; i < n; ++i) {
            maxBitonic = max(maxBitonic, lisVal[i] + ldsVal[i] - 1);
        }
        return maxBitonic;
    }

    // =============================================================================
    // 5. Minimum Number of Removals to Make Mountain Array (Optimized Pass)
    // =============================================================================

    int minimumMountainRemovals(const vector<int>& nums) {
        int n = nums.size();
        
        arr = nums;
        lisVal.assign(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) lisVal[i] = max(lisVal[i], lisVal[j] + 1);
            }
        }
        
        ldsVal.assign(n, 1);
        for (int i = n - 1; i >= 0; --i) {
            for (int j = n - 1; j > i; --j) {
                if (arr[j] < arr[i]) ldsVal[i] = max(ldsVal[i], ldsVal[j] + 1);
            }
        }
        
        int maxMountainLen = 0;
        for (int i = 0; i < n; ++i) {
            if (lisVal[i] > 1 && ldsVal[i] > 1) {
                maxMountainLen = max(maxMountainLen, lisVal[i] + ldsVal[i] - 1);
            }
        }
        return n - maxMountainLen;
    }

    // =============================================================================
    // 6. Number of Longest Increasing Subsequences
    // =============================================================================

    int findNumberOfLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        arr = nums;
        length.assign(n, 1);
        cnt.assign(n, 1);
        int maxLen = 1;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) {
                    if (length[j] + 1 > length[i]) {
                        length[i] = length[j] + 1;
                        cnt[i] = cnt[j];
                    } else if (length[j] + 1 == length[i]) {
                        cnt[i] += cnt[j];
                    }
                }
            }
            maxLen = max(maxLen, length[i]);
        }

        int totalLIS = 0;
        for (int i = 0; i < n; ++i) {
            if (length[i] == maxLen) {
                totalLIS += cnt[i];
            }
        }
        return totalLIS;
    }

    // =============================================================================
    // 7. Longest Non-Decreasing Subsequence (a_i <= a_j)
    // =============================================================================

    int lengthOfLNDS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        arr = nums;
        memo.assign(n, 1);
        int maxLen = 1;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] <= arr[i]) {
                    memo[i] = max(memo[i], memo[j] + 1);
                }
            }
            maxLen = max(maxLen, memo[i]);
        }
        return maxLen;
    }

    // =============================================================================
    // 8. Increasing Triplet Subsequence (Optimal O(N) Time, O(1) Space Approach)
    // =============================================================================

    bool increasingTriplet(const vector<int>& nums) {
        int first = INT_MAX, second = INT_MAX;
        for (int num : nums) {
            if (num <= first) {
                first = num;
            } else if (num <= second) {
                second = num;
            } else {
                return true;
            }
        }
        return false;
    }

    // =============================================================================
    // 9. Longest Chain of Pairs
    // =============================================================================

    int findLongestChain(vector<vector<int>>& pairs) {
        if (pairs.empty()) return 0;
        
        pairList = pairs;
        sort(pairList.begin(), pairList.end());

        int n = pairList.size();
        memo.assign(n, 1);
        int maxChain = 1;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (pairList[j][1] < pairList[i][0]) {
                    memo[i] = max(memo[i], memo[j] + 1);
                }
            }
            maxChain = max(maxChain, memo[i]);
        }
        return maxChain;
    }
};

// =============================================================================
// Local Verification Execution Block
// =============================================================================
int main() {
    Solution solver;

    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    assert(solver.lengthOfLIS(nums) == 4);
    
    vector<int> expectedLIS = {2, 3, 7, 18};
    assert(solver.getLIS(nums) == expectedLIS);
    
    vector<vector<int>> envs = {{5, 4}, {6, 4}, {6, 7}, {2, 3}};
    assert(solver.maxEnvelopes(envs) == 3);
    
    vector<int> msisNums = {1, 101, 2, 3, 100, 4, 5};
    assert(solver.maxSumIS(msisNums) == 106);
    
    vector<int> bitonicNums = {1, 11, 2, 10, 4, 5, 2, 1};
    assert(solver.longestBitonicSequence(bitonicNums) == 6);
    
    vector<int> mountainNums = {2, 1, 1, 5, 6, 2, 3, 1};
    assert(solver.minimumMountainRemovals(mountainNums) == 3);
    
    vector<int> countLISNums = {1, 3, 5, 4, 7};
    assert(solver.findNumberOfLIS(countLISNums) == 2);
    
    vector<int> lndsNums = {1, 4, 2, 2, 3};
    assert(solver.lengthOfLNDS(lndsNums) == 4);
    
    assert(solver.increasingTriplet(countLISNums) == true);
    
    vector<vector<int>> pairs = {{1, 2}, {2, 3}, {3, 4}};
    assert(solver.findLongestChain(pairs) == 2);
    
    cout << "All clean LIS Tabulation and optimized tests passed successfully!" << endl;
    return 0;
}