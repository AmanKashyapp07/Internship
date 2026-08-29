#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

// =========================================================
// 1. MAXIMUM SUM SUBARRAY OF SIZE K
// =========================================================

ll maxSubarraySumK(const vi &nums, int k) {
    int n = nums.size();
    if (n < k) return 0;
    ll currentSum = 0;
    for (int i = 0; i < k; ++i) currentSum += nums[i];
    ll maxSum = currentSum;
    for (int i = k; i < n; ++i) {
        currentSum += nums[i] - nums[i - k];
        maxSum = max(maxSum, currentSum);
    }
    return maxSum;
}
// Interview Explanation:
// - Problem Statement: Find maximum sum among all contiguous subarrays of fixed size k.
// - Approach: Fixed-size sliding window of length k.
// - Intuition: Compute sum of first k elements; slide window by adding incoming `nums[i]` and subtracting outgoing `nums[i-k]` in O(1) per step.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

// =========================================================
// 2. LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS (LEETCODE 3)
// =========================================================

int lengthOfLongestSubstring(string s) {
    int lastSeen[128];
    fill(lastSeen, lastSeen + 128, -1);
    int left = 0, ans = 0;
    for (int right = 0; right < (int)s.size(); right++) {
        left = max(left, lastSeen[(unsigned char)s[right]] + 1);
        lastSeen[(unsigned char)s[right]] = right;
        ans = max(ans, right - left + 1);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest substring without duplicate characters (LeetCode 3).
// - Approach: Variable-size sliding window with 128-element direct-access ASCII lookup table.
// - Intuition: When duplicate character is seen, advance `left` directly to `lastSeen[c] + 1` in O(1) time.
// - Complexity: Time: O(N) single pass, Space: O(1) fixed 128-byte array.

// =========================================================
// 3. MINIMUM SIZE SUBARRAY SUM (LEETCODE 209)
// =========================================================

int minSubArrayLen(int target, vi &nums) {
    int left = 0, sum = 0, minLen = INT_MAX;
    for (int right = 0; right < (int)nums.size(); right++) {
        sum += nums[right];
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);
            sum -= nums[left++];
        }
    }
    return minLen == INT_MAX ? 0 : minLen;
}
// Interview Explanation:
// - Problem Statement: Find minimal length of contiguous subarray whose sum is >= target (LeetCode 209).
// - Approach: Variable-size sliding window with left-pointer contraction.
// - Intuition: Expand `right` to meet target sum; shrink `left` while `sum >= target` to minimize window length.
// - Complexity: Time: O(N) linear time, Space: O(1).

// =========================================================
// 4. LONGEST SUBSTRING WITH AT MOST K DISTINCT CHARACTERS (LEETCODE 340)
// =========================================================

int lengthOfLongestSubstringKDistinct(string s, int k) {
    if (k == 0 || s.empty()) return 0;
    unordered_map<char, int> freq;
    int left = 0, maxLen = 0;
    for (int right = 0; right < (int)s.size(); ++right) {
        freq[s[right]]++;
        while ((int)freq.size() > k) {
            freq[s[left]]--;
            if (freq[s[left]] == 0) freq.erase(s[left]);
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of longest substring containing at most k distinct characters (LeetCode 340).
// - Approach: Sliding window with character frequency map.
// - Intuition: Expand `right` adding characters; whenever distinct keys exceed `k`, shrink `left` until size becomes `<= k`.
// - Complexity: Time: O(N), Space: O(K) hash map entries.

// =========================================================
// 5. MINIMUM WINDOW SUBSTRING (LEETCODE 76)
// =========================================================

string minWindow(string s, string t) {
    if (s.empty() || t.empty() || s.size() < t.size()) return "";
    int targetFreq[128] = {0}, windowFreq[128] = {0};
    for (char c : t) targetFreq[(unsigned char)c]++;

    int required = 0;
    for (int i = 0; i < 128; i++) if (targetFreq[i] > 0) required++;

    int formed = 0, left = 0;
    int minLen = INT_MAX, startIdx = 0;

    for (int right = 0; right < (int)s.size(); right++) {
        char c = s[right];
        windowFreq[(unsigned char)c]++;
        if (targetFreq[(unsigned char)c] > 0 && windowFreq[(unsigned char)c] == targetFreq[(unsigned char)c]) {
            formed++;
        }
        while (left <= right && formed == required) {
            if (right - left + 1 < minLen) {
                minLen = right - left + 1;
                startIdx = left;
            }
            char leftChar = s[left];
            windowFreq[(unsigned char)leftChar]--;
            if (targetFreq[(unsigned char)leftChar] > 0 && windowFreq[(unsigned char)leftChar] < targetFreq[(unsigned char)leftChar]) {
                formed--;
            }
            left++;
        }
    }
    return minLen == INT_MAX ? "" : s.substr(startIdx, minLen);
}
// Interview Explanation:
// - Problem Statement: Find minimum window substring of s containing all characters of t in O(N) time (LeetCode 76).
// - Approach: Two-Pointer Variable Sliding Window with Direct-Access Hash Table.
// - Intuition: Expand `right` to fulfill requirement count `formed == required`; shrink `left` greedily to minimize window size.
// - Complexity: Time: O(N + M), Space: O(1) fixed 128-byte array.

// =========================================================
// 6. SUBARRAYS WITH K DISTINCT INTEGERS (LEETCODE 992)
// =========================================================

int atMostKDistinct(const vi &nums, int k) {
    if (k < 0) return 0;
    unordered_map<int, int> count;
    int left = 0, ans = 0;
    for (int right = 0; right < (int)nums.size(); right++) {
        count[nums[right]]++;
        while ((int)count.size() > k) {
            count[nums[left]]--;
            if (count[nums[left]] == 0) count.erase(nums[left]);
            left++;
        }
        ans += (right - left + 1);
    }
    return ans;
}

int subarraysWithKDistinct(vi &nums, int k) {
    return atMostKDistinct(nums, k) - atMostKDistinct(nums, k - 1);
}
// Interview Explanation:
// - Problem Statement: Count subarrays with exactly k distinct integers (LeetCode 992).
// - Approach: Inclusion-Exclusion Reduction: $	ext{exact}(K) = 	ext{atMost}(K) - 	ext{atMost}(K - 1)$.
// - Intuition: Finding `exact(K)` directly is challenging with two pointers; reducing to `atMost(K)` makes the window monotonic and contractible.
// - Complexity: Time: O(N) two linear passes, Space: O(K) hash map entries.

// =========================================================
// 7. ANAGRAMS (LC 438) & PERMUTATION IN STRING (LC 567)
// =========================================================

vi findAnagrams(string s, string p) {
    int n = s.size(), m = p.size();
    if (n < m) return {};
    vi pFreq(26, 0), sFreq(26, 0), ans;
    for (int i = 0; i < m; i++) {
        pFreq[p[i] - 'a']++;
        sFreq[s[i] - 'a']++;
    }
    if (pFreq == sFreq) ans.push_back(0);
    for (int i = m; i < n; i++) {
        sFreq[s[i] - 'a']++;
        sFreq[s[i - m] - 'a']--;
        if (pFreq == sFreq) ans.push_back(i - m + 1);
    }
    return ans;
}

bool checkInclusion(string s1, string s2) {
    int n = s1.size(), m = s2.size();
    if (m < n) return false;
    vi f1(26, 0), f2(26, 0);
    for (int i = 0; i < n; i++) {
        f1[s1[i] - 'a']++;
        f2[s2[i] - 'a']++;
    }
    if (f1 == f2) return true;
    for (int i = n; i < m; i++) {
        f2[s2[i] - 'a']++;
        f2[s2[i - n] - 'a']--;
        if (f1 == f2) return true;
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Find all start indices of p's anagrams in s (LC 438), and check if s2 contains permutation of s1 (LC 567).
// - Approach: Fixed Sliding Window of size |p| with 26-element frequency vector comparison.
// - Complexity: Time: O(N), Space: O(1) 26-element frequency buffers.

// =========================================================
// 8. LONGEST REPEATING CHARACTER REPLACEMENT (LEETCODE 424)
// =========================================================

int characterReplacement(string s, int k) {
    int count[26] = {0};
    int maxCount = 0, left = 0, maxLen = 0;
    for (int right = 0; right < (int)s.size(); right++) {
        count[s[right] - 'A']++;
        maxCount = max(maxCount, count[s[right] - 'A']);
        // Condition: (window_len - max_freq) > k means too many replacements needed
        while ((right - left + 1) - maxCount > k) {
            count[s[left] - 'A']--;
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of longest substring with same letter after at most k character replacements (LeetCode 424).
// - Approach: Variable-size Sliding Window tracking most frequent character in current window `maxCount`.
// - Intuition: `(window_len - maxCount)` is the number of characters that need replacement; if this exceeds `k`, shrink `left`.
// - Complexity: Time: O(N), Space: O(1) 26-element array.

// =========================================================
// 9. MAX CONSECUTIVE ONES III (LEETCODE 1004)
// =========================================================

int longestOnes(vi &nums, int k) {
    int left = 0, zeroCount = 0, maxLen = 0;
    for (int right = 0; right < (int)nums.size(); right++) {
        if (nums[right] == 0) zeroCount++;
        while (zeroCount > k) {
            if (nums[left] == 0) zeroCount--;
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Longest contiguous subarray containing only 1s after flipping at most k zeroes (LeetCode 1004).
// - Approach: Sliding Window maintaining `zeroCount <= k`.
// - Complexity: Time: O(N), Space: O(1).

// =========================================================
// 10. FRUIT INTO BASKETS (LEETCODE 904)
// =========================================================

int totalFruit(vi &fruits) {
    unordered_map<int, int> basket;
    int left = 0, maxFruits = 0;
    for (int right = 0; right < (int)fruits.size(); right++) {
        basket[fruits[right]]++;
        while ((int)basket.size() > 2) {
            basket[fruits[left]]--;
            if (basket[fruits[left]] == 0) basket.erase(fruits[left]);
            left++;
        }
        maxFruits = max(maxFruits, right - left + 1);
    }
    return maxFruits;
}
// Interview Explanation:
// - Problem Statement: Find maximum length subarray with at most 2 distinct fruit types (LeetCode 904).
// - Approach: Sliding window maintaining hash map size `<= 2`.
// - Complexity: Time: O(N), Space: O(1) max 3 map entries.

// =========================================================
// 11. SUBARRAY SUM EQUALS K (LEETCODE 560)
// =========================================================

int subarraySum(vi &nums, int k) {
    unordered_map<int, int> prefixFreq;
    prefixFreq[0] = 1;
    int currentSum = 0, count = 0;
    for (int x : nums) {
        currentSum += x;
        if (prefixFreq.count(currentSum - k)) {
            count += prefixFreq[currentSum - k];
        }
        prefixFreq[currentSum]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total continuous subarrays whose sum equals k (LeetCode 560).
// - Approach: Prefix Sum Hash Map tracking frequencies of prefix sums.
// - Intuition: $P[j] - P[i-1] = k \iff P[i-1] = P[j] - k$. Querying frequency of `currentSum - k` resolves count in O(1).
// - Complexity: Time: O(N), Space: O(N) hash map memory.

// =========================================================
// 12. CONTIGUOUS ARRAY - EQUAL 0s & 1s (LEETCODE 525)
// =========================================================

int findMaxLengthEqual01(vi &nums) {
    unordered_map<int, int> prefixIndex;
    prefixIndex[0] = -1; // Base condition for full prefix match
    int sum = 0, maxLen = 0;
    for (int i = 0; i < (int)nums.size(); ++i) {
        sum += (nums[i] == 1) ? 1 : -1;
        if (prefixIndex.count(sum)) {
            maxLen = max(maxLen, i - prefixIndex[sum]);
        } else {
            prefixIndex[sum] = i;
        }
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find maximum length of contiguous subarray with equal number of 0s and 1s (LeetCode 525).
// - Approach: Prefix Sum with Transform: Map 0 $	o$ -1, 1 $	o$ 1.
// - Intuition: Equal 0s and 1s corresponds to subarray sum == 0. Store earliest occurrence index of each prefix sum.
// - Complexity: Time: O(N), Space: O(N).

// =========================================================
// 13. SUBARRAY SUMS DIVISIBLE BY K (LEETCODE 974)
// =========================================================

int subarraysDivByK(vi &nums, int k) {
    unordered_map<int, int> modFreq;
    modFreq[0] = 1;
    int currentSum = 0, count = 0;
    for (int x : nums) {
        currentSum += x;
        int rem = ((currentSum % k) + k) % k; // Normalized positive modulo
        if (modFreq.count(rem)) {
            count += modFreq[rem];
        }
        modFreq[rem]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count number of subarrays with sum divisible by k (LeetCode 974).
// - Approach: Prefix Sum Modulo Frequency Map.
// - Intuition: $(P[j] - P[i-1]) \pmod k = 0 \iff P[j] \equiv P[i-1] \pmod k$. Equal remainder prefixes form valid subarrays.
// - Complexity: Time: O(N), Space: O(K) modulo map.

// =========================================================
// 14. DIFFERENCE ARRAY / RANGE ADDITION (LEETCODE 370)
// =========================================================

vi getModifiedArray(int length, vvi &updates) {
    vi diff(length + 1, 0);
    for (auto &u : updates) {
        int start = u[0], end = u[1], val = u[2];
        diff[start] += val;
        diff[end + 1] -= val;
    }
    vi res(length);
    int runningSum = 0;
    for (int i = 0; i < length; ++i) {
        runningSum += diff[i];
        res[i] = runningSum;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Apply multiple range increments $[start, end, val]$ on an array of length N in O(1) per update (LeetCode 370).
// - Approach: Difference Array (Sweep-line delta array).
// - Intuition: `diff[start] += val` propagates increase to all elements $\ge start$; `diff[end+1] -= val` stops propagation beyond $end$. Running prefix sum reconstructs final array in O(N).
// - Complexity: Time: O(N + Q) where Q is number of updates, Space: O(N) difference buffer.
