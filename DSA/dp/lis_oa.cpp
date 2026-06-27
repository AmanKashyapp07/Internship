#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>
#include <string>
#include <unordered_map>

using namespace std;

/*
================================================================================
LIS (Longest Increasing Subsequence) OA & Interview Variants
This file contains LeetCode-style class implementations for the most popular LIS variants.
================================================================================
*/

// =============================================================================
// 1. Classic LIS - Length only (LeetCode 300)
// Time: O(N log N), Space: O(N)
// =============================================================================
class ClassicLIS {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) return 0;
        vector<int> temp; // Stores the active tails of increasing subsequences
        for (int x : nums) {
            auto it = lower_bound(temp.begin(), temp.end(), x);
            if (it == temp.end()) {
                temp.push_back(x); // Append if x is greater than all elements in temp
            } else {
                *it = x; // Replace the first element >= x with x to keep tail values small
            }
        }
        return temp.size();
    }
};

// =============================================================================
// 2. Print LIS - Reconstructing the Subsequence
// Time: O(N^2), Space: O(N)
// =============================================================================
class PrintLIS {
public:
    vector<int> getLIS(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return {};
        
        vector<int> dp(n, 1);
        vector<int> parent(n, -1);
        int maxLen = 1;
        int lastIndex = 0;
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i] && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            if (dp[i] > maxLen) {
                maxLen = dp[i];
                lastIndex = i;
            }
        }
        
        // Reconstruct the LIS path using parent pointers
        vector<int> lis;
        while (lastIndex != -1) {
            lis.push_back(nums[lastIndex]);
            lastIndex = parent[lastIndex];
        }
        reverse(lis.begin(), lis.end());
        return lis;
    }
};

// =============================================================================
// 3. Russian Doll Envelopes - 2D LIS (LeetCode 354)
// Time: O(N log N), Space: O(N)
// =============================================================================
class RussianDollEnvelopes {
public:
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        // Sort width ascending, and height descending if widths are equal
        sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] == b[0]) return a[1] > b[1]; // Height descending
            return a[0] < b[0]; // Width ascending
        });
        
        // Find 1D LIS on heights
        vector<int> temp;
        for (const auto& env : envelopes) {
            int h = env[1];
            auto it = lower_bound(temp.begin(), temp.end(), h);
            if (it == temp.end()) {
                temp.push_back(h);
            } else {
                *it = h;
            }
        }
        return temp.size();
    }
};

// =============================================================================
// 4. Maximum Sum Increasing Subsequence (MSIS)
// Time: O(N^2), Space: O(N)
// =============================================================================
class MaxSumLIS {
public:
    int maxSumIS(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        vector<int> dp(nums); // dp[i] stores max sum LIS ending at index i
        int maxSum = nums[0];
        
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i] && dp[j] + nums[i] > dp[i]) {
                    dp[i] = dp[j] + nums[i];
                }
            }
            maxSum = max(maxSum, dp[i]);
        }
        return maxSum;
    }
};

// =============================================================================
// 5. Longest Bitonic Subsequence
// Time: O(N^2), Space: O(N)
// =============================================================================
class LongestBitonicSubsequence {
public:
    int LongestBitonicSequence(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        vector<int> lis(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i] && lis[j] + 1 > lis[i]) {
                    lis[i] = lis[j] + 1;
                }
            }
        }
        
        vector<int> lds(n, 1);
        for (int i = n - 1; i >= 0; --i) {
            for (int j = n - 1; j > i; --j) {
                if (nums[j] < nums[i] && lds[j] + 1 > lds[i]) {
                    lds[i] = lds[j] + 1;
                }
            }
        }
        
        int maxBitonic = 0;
        for (int i = 0; i < n; ++i) {
            maxBitonic = max(maxBitonic, lis[i] + lds[i] - 1);
        }
        return maxBitonic;
    }
};

// =============================================================================
// 6. Minimum Number of Removals to Make Mountain Array (LeetCode 1671)
// Time: O(N^2) or O(N log N), Space: O(N)
// Note: We use O(N log N) here for optimal performance.
// =============================================================================
class MountainArrayRemovals {
private:
    vector<int> getLISLengths(const vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, 1);
        vector<int> temp;
        for (int i = 0; i < n; ++i) {
            auto it = lower_bound(temp.begin(), temp.end(), nums[i]);
            int idx = distance(temp.begin(), it);
            if (it == temp.end()) {
                temp.push_back(nums[i]);
            } else {
                *it = nums[i];
            }
            dp[i] = idx + 1;
        }
        return dp;
    }

public:
    int minimumMountainRemovals(vector<int>& nums) {
        int n = nums.size();
        
        // LIS from left to right
        vector<int> lis = getLISLengths(nums);
        
        // LDS: LIS on reversed array
        vector<int> rev_nums(nums.rbegin(), nums.rend());
        vector<int> lds_rev = getLISLengths(rev_nums);
        vector<int> lds(n);
        for (int i = 0; i < n; ++i) {
            lds[i] = lds_rev[n - 1 - i];
        }
        
        int maxMountainLen = 0;
        for (int i = 0; i < n; ++i) {
            // A valid mountain peak must have at least one element on left and right
            if (lis[i] > 1 && lds[i] > 1) {
                maxMountainLen = max(maxMountainLen, lis[i] + lds[i] - 1);
            }
        }
        return n - maxMountainLen;
    }
};

// =============================================================================
// 7. Number of Longest Increasing Subsequences (LeetCode 673)
// Time: O(N^2), Space: O(N)
// =============================================================================
class NumberOfLIS {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return n;
        
        vector<int> lengths(n, 1);
        vector<int> counts(n, 1);
        int maxLen = 1;
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    if (lengths[j] + 1 > lengths[i]) {
                        lengths[i] = lengths[j] + 1;
                        counts[i] = counts[j];
                    } else if (lengths[j] + 1 == lengths[i]) {
                        counts[i] += counts[j];
                    }
                }
            }
            maxLen = max(maxLen, lengths[i]);
        }
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            if (lengths[i] == maxLen) {
                ans += counts[i];
            }
        }
        return ans;
    }
};

// =============================================================================
// 8. Longest Non-Decreasing Subsequence (a_i <= a_j)
// Time: O(N log N), Space: O(N)
// =============================================================================
class LongestNonDecreasingSubsequence {
public:
    int lengthOfLNDS(vector<int>& nums) {
        if (nums.empty()) return 0;
        vector<int> temp;
        for (int x : nums) {
            // Use upper_bound instead of lower_bound to allow duplicates
            auto it = upper_bound(temp.begin(), temp.end(), x);
            if (it == temp.end()) {
                temp.push_back(x);
            } else {
                *it = x;
            }
        }
        return temp.size();
    }
};

// =============================================================================
// 9. Increasing Triplet Subsequence (LeetCode 334)
// Time: O(N), Space: O(1)
// =============================================================================
class IncreasingTripletSubsequence {
public:
    bool increasingTriplet(vector<int>& nums) {
        int first = INT_MAX;
        int second = INT_MAX;
        for (int x : nums) {
            if (x <= first) {
                first = x;       // Found a new smallest element
            } else if (x <= second) {
                second = x;      // Found a new element greater than first
            } else {
                return true;     // Found an element greater than both first and second
            }
        }
        return false;
    }
};

// =============================================================================
// 10. Longest Chain of Pairs (LeetCode 646)
// Time: O(N log N), Space: O(1) if sorting in place
// =============================================================================
class LongestChainOfPairs {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        if (pairs.empty()) return 0;
        // Sort pairs ascending by their right endpoints (greedy interval scheduling approach)
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });
        
        int count = 1;
        int currEnd = pairs[0][1];
        
        for (size_t i = 1; i < pairs.size(); ++i) {
            if (pairs[i][0] > currEnd) {
                count++;
                currEnd = pairs[i][1];
            }
        }
        return count;
    }
};

// =============================================================================
// Main function for simple local verification
// =============================================================================
int main() {
    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    
    ClassicLIS lisSolver;
    cout << "Classic LIS length: " << lisSolver.lengthOfLIS(nums) << " (Expected: 4)" << endl;
    
    PrintLIS printer;
    vector<int> path = printer.getLIS(nums);
    cout << "Printed LIS path: ";
    for (int x : path) cout << x << " ";
    cout << endl;
    
    IncreasingTripletSubsequence triplet;
    vector<int> nums2 = {2, 1, 5, 0, 4, 6};
    cout << "Increasing Triplet exists: " << (triplet.increasingTriplet(nums2) ? "Yes" : "No") << " (Expected: Yes)" << endl;
    
    return 0;
}
