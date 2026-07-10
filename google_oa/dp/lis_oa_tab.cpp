#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    // 1. Longest Increasing Subsequence - Length
    int lengthOfLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        vector<int> dp(n, 1);
        int maxLen = 1;

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < i; ++j)
                if (nums[j] < nums[i])
                    dp[i] = max(dp[i], dp[j] + 1);

        for (int len : dp) maxLen = max(maxLen, len);
        return maxLen;
    }
    // important - tail sort

    // 2. Efficient LIS using Tails Array (Patience Sorting) - O(n log n)
    int lengthOfLISBinary(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        vector<int> tails;  // tails[i] = smallest tail of all increasing subsequences of length i+1

        for (int num : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), num);
            if (it == tails.end()) {
                tails.push_back(num);
            } else {
                *it = num;
            }
        }
        return tails.size();
    } // always prefer this method in OA 

    // 2. Longest Increasing Subsequence - Reconstruct
    vector<int> getLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return {};

        vector<int> dp(n, 1), parent(n);
        for (int i = 0; i < n; ++i) parent[i] = i;

        int last = 0;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i] && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            if (dp[i] > dp[last]) last = i; // did we just find a longer increasing subsequence?
        }

        vector<int> lis;
        while(last != parent[last]) {
            lis.push_back(nums[last]);
            last = parent[last];
        }
        lis.push_back(nums[last]);

        reverse(lis.begin(), lis.end());
        return lis;
    }
    bool comparator(const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first == b.first) return a.second > b.second;
        return a.first < b.first;
    }
    
    // Russian Doll Envelopes - O(n log n) using Tails Array
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        if (envelopes.empty()) return 0;

        sort(envelopes.begin(), envelopes.end(), comparator);
        vector<int> tails;

        for (auto& env : envelopes) {
            int height = env[1];
            
            // Find the first position in tails where height can replace
            auto it = lower_bound(tails.begin(), tails.end(), height);
            if (it == tails.end()) {
                tails.push_back(height);
            } else {
                *it = height;
            }
        }

        return tails.size();
    }
    // 4. Maximum Sum Increasing Subsequence
    int maxSumIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        vector<int> dp = nums;
        int maxSum = nums[0];

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < i; ++j)
                if (nums[j] < nums[i])
                    dp[i] = max(dp[i], dp[j] + nums[i]);

        for (int sum : dp) maxSum = max(maxSum, sum);
        return maxSum;
    }

    // 5. Longest Bitonic Subsequence
    int longestBitonicSequence(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        vector<int> lis(n, 1);
        vector<int> lds(n, 1);

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < i; ++j)
                if (nums[j] < nums[i])
                    lis[i] = max(lis[i], lis[j] + 1);

        vector<int> temp;
        temp = nums;
        reverse(temp.begin(), temp.end());
        for(int i=0;i<n;i++){
            for(int j=0;j<i;j++){
                if(temp[j]<temp[i]){
                    lds[i] = max(lds[i], lds[j]+1);
                }
            }
        }

        int maxBit = 0;
        for (int i = 0; i < n; ++i)
            maxBit = max(maxBit, lis[i] + lds[i] - 1);

        return maxBit;
    } // principle = LIS + LDS - 1, because the peak element is counted twice

    // 6. Minimum Removals to Make Mountain Array
    int minimumMountainRemovals(const vector<int>& nums) {
        int n = nums.size();
        vector<int> lis(n, 1), lds(n, 1);

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < i; ++j)
                if (nums[j] < nums[i])
                    lis[i] = max(lis[i], lis[j] + 1);

        for (int i = n - 1; i >= 0; --i)
            for (int j = n - 1; j > i; --j)
                if (nums[j] < nums[i])
                    lds[i] = max(lds[i], lds[j] + 1);

        int maxMountain = 0;
        for (int i = 0; i < n; ++i)
            if (lis[i] > 1 && lds[i] > 1)
                maxMountain = max(maxMountain, lis[i] + lds[i] - 1);

        return n - maxMountain;
    } // same principle : Max Mountains = LIS + LDS - 1, then min removals = n - maxMountain

    // 7. Number of Longest Increasing Subsequences
    int findNumberOfLIS(const vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        vector<int> length(n, 1), count(n, 1);
        int maxLen = 1;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    if (length[j] + 1 > length[i]) {
                        length[i] = length[j] + 1;
                        count[i] = count[j];
                    } else if (length[j] + 1 == length[i]) {
                        count[i] += count[j];
                    }
                }
            }
            maxLen = max(maxLen, length[i]);
        }

        int total = 0;
        for (int i = 0; i < n; ++i)
            if (length[i] == maxLen) total += count[i];

        return total;
    }

};

int main() {
    
}