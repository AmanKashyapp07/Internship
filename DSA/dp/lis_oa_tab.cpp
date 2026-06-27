#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

// =============================================================================
// 1. Classic LIS - Length & String/Array Reconstruction
// =============================================================================

int lengthOfLIS(const vector<int> &nums) {
  int n = nums.size();
  if (n == 0)
    return 0;

  vector<int> dp(n, 1);
  int maxLen = 1;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] < nums[i]) {
        dp[i] = max(dp[i], dp[j] + 1);
      }
    }
    maxLen = max(maxLen, dp[i]);
  }
  return maxLen;
}

vector<int> getLIS(const vector<int> &nums) {
  int n = nums.size();
  if (n == 0)
    return {};

  vector<int> dp(n, 1);
  vector<int> parent(n);
  for (int i = 0; i < n; ++i)
    parent[i] = i;

  int lastIdx = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] < nums[i] && dp[j] + 1 > dp[i]) {
        dp[i] = dp[j] + 1;
        parent[i] = j;
      }
    }
    if (dp[i] > dp[lastIdx]) {
      lastIdx = i;
    }
  }

  vector<int> lis;
  while (parent[lastIdx] != lastIdx) {
    lis.push_back(nums[lastIdx]);
    lastIdx = parent[lastIdx];
  }
  lis.push_back(nums[lastIdx]);

  reverse(lis.begin(), lis.end());
  return lis;
}

// =============================================================================
// 2. Russian Doll Envelopes (2D LIS Variant)
// =============================================================================

int maxEnvelopes(vector<vector<int>> &envelopes) {
  if (envelopes.empty())
    return 0;

  // Sort width ascending, and height descending if widths match
  sort(envelopes.begin(), envelopes.end(),
       [](const vector<int> &a, const vector<int> &b) {
         if (a[0] == b[0])
           return a[1] > b[1];
         return a[0] < b[0];
       });

  int n = envelopes.size();
  vector<int> dp(n, 1);
  int maxEnvs = 1;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (envelopes[j][1] < envelopes[i][1]) {
        dp[i] = max(dp[i], dp[j] + 1);
      }
    }
    maxEnvs = max(maxEnvs, dp[i]);
  }
  return maxEnvs;
}

// =============================================================================
// 3. Maximum Sum Increasing Subsequence (MSIS)
// =============================================================================

int maxSumIS(const vector<int> &nums) {
  int n = nums.size();
  if (n == 0)
    return 0;

  vector<int> dp(nums.begin(), nums.end());
  int maxSum = dp[0];

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] < nums[i]) {
        dp[i] = max(dp[i], dp[j] + nums[i]);
      }
    }
    maxSum = max(maxSum, dp[i]);
  }
  return maxSum;
}

// =============================================================================
// 4. Longest Bitonic Subsequence (Optimized Pass)
// =============================================================================

int longestBitonicSequence(const vector<int> &nums) {
  int n = nums.size();
  if (n == 0)
    return 0;

  vector<int> lis(n, 1);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] < nums[i])
        lis[i] = max(lis[i], lis[j] + 1);
    }
  }

  vector<int> lds(n, 1);
  for (int i = n - 1; i >= 0; --i) {
    for (int j = n - 1; j > i; --j) {
      if (nums[j] < nums[i])
        lds[i] = max(lds[i], lds[j] + 1);
    }
  }

  int maxBitonic = 0;
  for (int i = 0; i < n; ++i) {
    maxBitonic = max(maxBitonic, lis[i] + lds[i] - 1);
  }
  return maxBitonic;
}

// =============================================================================
// 5. Minimum Number of Removals to Make Mountain Array (Optimized Pass)
// =============================================================================

int minimumMountainRemovals(const vector<int> &nums) {
  int n = nums.size();

  vector<int> lis(n, 1);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] < nums[i])
        lis[i] = max(lis[i], lis[j] + 1);
    }
  }

  vector<int> lds(n, 1);
  for (int i = n - 1; i >= 0; --i) {
    for (int j = n - 1; j > i; --j) {
      if (nums[j] < nums[i])
        lds[i] = max(lds[i], lds[j] + 1);
    }
  }

  int maxMountainLen = 0;
  for (int i = 0; i < n; ++i) {
    // A mountain peak requires both a strictly increasing and decreasing side
    if (lis[i] > 1 && lds[i] > 1) {
      maxMountainLen = max(maxMountainLen, lis[i] + lds[i] - 1);
    }
  }
  return n - maxMountainLen;
}

// =============================================================================
// 6. Number of Longest Increasing Subsequences
// =============================================================================

int findNumberOfLIS(const vector<int> &nums) {
  int n = nums.size();
  if (n == 0)
    return 0;

  vector<int> len(n, 1), count(n, 1);
  int maxLen = 1;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] < nums[i]) {
        if (len[j] + 1 > len[i]) {
          len[i] = len[j] + 1;
          count[i] = count[j];
        } else if (len[j] + 1 == len[i]) {
          count[i] += count[j];
        }
      }
    }
    maxLen = max(maxLen, len[i]);
  }

  int totalLIS = 0;
  for (int i = 0; i < n; ++i) {
    if (len[i] == maxLen) {
      totalLIS += count[i];
    }
  }
  return totalLIS;
}

// =============================================================================
// 7. Longest Non-Decreasing Subsequence (a_i <= a_j)
// =============================================================================

int lengthOfLNDS(const vector<int> &nums) {
  int n = nums.size();
  if (n == 0)
    return 0;

  vector<int> dp(n, 1);
  int maxLen = 1;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (nums[j] <= nums[i]) {
        dp[i] = max(dp[i], dp[j] + 1);
      }
    }
    maxLen = max(maxLen, dp[i]);
  }
  return maxLen;
}

// =============================================================================
// 8. Increasing Triplet Subsequence (Optimal O(N) Time, O(1) Space Approach)
// =============================================================================

bool increasingTriplet(const vector<int> &nums) {
  int first = INT_MAX, second = INT_MAX;
  for (int num : nums) {
    if (num <= first) {
      first = num;
    } else if (num <= second) {
      second = num;
    } else {
      return true; // Found an element greater than both first and second
    }
  }
  return false;
}

// =============================================================================
// 9. Longest Chain of Pairs
// =============================================================================

int findLongestChain(vector<vector<int>> &pairs) {
  if (pairs.empty())
    return 0;
  sort(pairs.begin(), pairs.end());

  int n = pairs.size();
  vector<int> dp(n, 1);
  int maxChain = 1;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (pairs[j][1] < pairs[i][0]) {
        dp[i] = max(dp[i], dp[j] + 1);
      }
    }
    maxChain = max(maxChain, dp[i]);
  }
  return maxChain;
}

// =============================================================================
// Local Verification Execution Block
// =============================================================================
int main() {
  vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
  assert(lengthOfLIS(nums) == 4);

  vector<int> expectedLIS = {2, 3, 7, 18};
  assert(getLIS(nums) == expectedLIS);

  vector<vector<int>> envs = {{5, 4}, {6, 4}, {6, 7}, {2, 3}};
  assert(maxEnvelopes(envs) == 3);

  vector<int> msisNums = {1, 101, 2, 3, 100, 4, 5};
  assert(maxSumIS(msisNums) == 106);

  vector<int> bitonicNums = {1, 11, 2, 10, 4, 5, 2, 1};
  assert(longestBitonicSequence(bitonicNums) == 6);

  vector<int> mountainNums = {2, 1, 1, 5, 6, 2, 3, 1};
  assert(minimumMountainRemovals(mountainNums) == 3);

  vector<int> countLISNums = {1, 3, 5, 4, 7};
  assert(findNumberOfLIS(countLISNums) == 2);

  vector<int> lndsNums = {1, 4, 2, 2, 3};
  assert(lengthOfLNDS(lndsNums) == 4);

  assert(increasingTriplet(countLISNums) == true);

  vector<vector<int>> pairs = {{1, 2}, {2, 3}, {3, 4}};
  assert(findLongestChain(pairs) == 2);

  cout << "All clean LIS Tabulation and optimized tests passed successfully!"
       << endl;
  return 0;
}