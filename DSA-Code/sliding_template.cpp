// Ultimate Sliding Window & Two-Pointer Blueprint for Interviews & Advanced CP.
// Contains key patterns from LeetCode (Striver SDE Sheet), Codeforces, and CodeChef.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <climits>

using namespace std;

// =========================================================================
// SECTION 1: INTERVIEW & OA PATTERNS (STRIVER'S SDE SHEET CORE)
// =========================================================================

class StriverSlidingWindow {
public:
    // =====================================================================
    // 1. LEETCODE 3: LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS
    // =====================================================================
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
    // - Problem Statement: Find the length of the longest substring without duplicate characters.
    // - Approach: Variable-size sliding window with a 128-element direct-access ASCII lookup table.
    // - Intuition: When a duplicate character is encountered, jump the left pointer directly to lastSeen[c] + 1 to skip the previous occurrence in O(1).
    // - Complexity: Time: O(N) single pass through the string, Space: O(1) fixed 128-byte array.


    // =====================================================================
    // 2. LEETCODE 209: MINIMUM SIZE SUBARRAY SUM
    // =====================================================================
    int minSubArrayLen(int target, vector<int>& nums) {
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
    // - Problem Statement: Find the minimal length of a contiguous subarray whose sum is >= target (LeetCode 209).
    // - Approach: Variable-size sliding window with left-pointer contraction when sum >= target.
    // - Intuition: Expand right pointer to reach target sum; greedily shrink left pointer while sum >= target to find minimum window length.
    // - Complexity: Time: O(N) linear time (each pointer advances at most N times), Space: O(1).


    // =====================================================================
    // 3. LEETCODE 438: FIND ALL ANAGRAMS IN A STRING
    // =====================================================================
    vector<int> findAnagrams(string s, string p) {
        int n = s.size(), m = p.size();
        if (n < m) return {};
        vector<int> pFreq(26, 0), sFreq(26, 0), ans;
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
    // Interview Explanation:
    // - Problem Statement: Find all start indices of p's anagrams in s (LeetCode 438).
    // - Approach: Fixed sliding window of size m with 26-element frequency vector equality checks.
    // - Intuition: Sliding a window of fixed size |p| adds incoming s[i] and subtracts outgoing s[i-m]; comparing frequency arrays takes O(26) = O(1).
    // - Complexity: Time: O(N), Space: O(1) 26-element vectors.


    // =====================================================================
    // 4. LEETCODE 567: PERMUTATION IN STRING
    // =====================================================================
    bool checkInclusion(string s1, string s2) {
        int n = s1.size(), m = s2.size();
        if (m < n) return false;
        vector<int> f1(26, 0), f2(26, 0);
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
    // - Problem Statement: Check if s2 contains a permutation of s1 as a substring (LeetCode 567).
    // - Approach: Fixed sliding window of size |s1| matching character frequencies.
    // - Intuition: A substring is a permutation of s1 iff its frequency array matches s1's frequency array exactly.
    // - Complexity: Time: O(M), Space: O(1).


    // =====================================================================
    // 5. HELPER: SUBARRAYS WITH SUM AT MOST K
    // =====================================================================
    int numSubarraysAtMostK(vector<int> &nums, int k) {
        if (k < 0) return 0;
        int left = 0, sum = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            sum += nums[right];
            while (sum > k) sum -= nums[left++];
            ans += right - left + 1;
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Count the total number of subarrays whose element sum is at most K in an array of non-negative integers.
    // - Approach: Monotonic sliding window expansion with left-pointer contraction (sum > k).
    // - Intuition: For any valid window [l, r], every subarray ending at r starting from l, l+1, ..., r is also valid, adding exactly r - l + 1 subarrays.
    // - Complexity: Time: O(N) since left and right pointers advance at most N times, Space: O(1) auxiliary space.


    // =====================================================================
    // 6. LEETCODE 930: BINARY SUBARRAYS WITH SUM
    // =====================================================================
    int numSubarraysWithSum(vector<int> &nums, int goal) {
        return numSubarraysAtMostK(nums, goal) - numSubarraysAtMostK(nums, goal - 1);
    }
    // Interview Explanation:
    // - Problem Statement: Count the number of non-empty subarrays with sum equal to goal.
    // - Approach: Exact-K reduction via atMost(goal) - atMost(goal - 1).
    // - Intuition: Subarrays with exact sum K equal all subarrays with sum <= K minus all subarrays with sum <= K - 1, turning a non-monotonic condition into two monotonic sliding windows.
    // - Complexity: Time: O(N) two linear passes, Space: O(1) auxiliary space.


    // =====================================================================
    // 7. LEETCODE 1358: NUMBER OF SUBSTRINGS CONTAINING ALL THREE CHARACTERS
    // =====================================================================
    int numberOfSubstrings(const string &s) {
        vector<int> lastSeen(3, -1);
        int totalCount = 0;
        for (int i = 0; i < (int)s.length(); i++) {
            lastSeen[s[i] - 'a'] = i;
            if (lastSeen[0] != -1 && lastSeen[1] != -1 && lastSeen[2] != -1) {
                totalCount += min({lastSeen[0], lastSeen[1], lastSeen[2]}) + 1;
            }
        }
        return totalCount;
    }
    // Interview Explanation:
    // - Problem Statement: Count the number of substrings containing at least one occurrence of 'a', 'b', and 'c'.
    // - Approach: Last seen index tracking for each character.
    // - Intuition: With i as the right end, the closest required character to the left is min(lastSeen[0], lastSeen[1], lastSeen[2]); any start index from 0 up to that minimum forms a valid substring.
    // - Complexity: Time: O(N) single pass, Space: O(1) fixed 3-element vector.


    // =====================================================================
    // 8. LEETCODE 76: MINIMUM WINDOW SUBSTRING
    // =====================================================================
    bool isValid(const vector<int> &freq1, const vector<int> &freq2) {
        for (int i = 0; i < 128; i++) {
            if (freq2[i] < freq1[i]) return false;
        }
        return true;
    }

    string minWindow(const string &s, const string &t) {
        vector<int> freq1(128, 0), freq2(128, 0);
        for (char ch : t) freq1[(unsigned char)ch]++;

        int left = 0, start = -1, minLen = INT_MAX;

        for (int right = 0; right < (int)s.size(); right++) {
            freq2[(unsigned char)s[right]]++;

            while (isValid(freq1, freq2)) {
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    start = left;
                }
                freq2[(unsigned char)s[left]]--;
                left++;
            }
        }
        return start == -1 ? "" : s.substr(start, minLen);
    }

    // =====================================================================
    // 9. LEETCODE 727: MINIMUM WINDOW SUBSEQUENCE
    // =====================================================================
    string minWindowSubsequence(const string &s, const string &t) {
        int n = s.size(), m = t.size();
        int idx = -1, len = INT_MAX;
        int i = 0, j = 0;

        while (i < n) {
            if (s[i] == t[j]) j++;

            if (j == m) {
                int end = i;
                j--;
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                int start = i + 1;
                if (end - start + 1 < len) {
                    len = end - start + 1;
                    idx = start;
                }
                i = start + 1;
                j = 0;
                continue;
            }
            i++;
        }
        return idx == -1 ? "" : s.substr(idx, len);
    }

    // =====================================================================
    // 10. LEETCODE 1004: MAX CONSECUTIVE ONES III
    // =====================================================================
    int longestOnes(vector<int> &nums, int k) {
        int left = 0, zeros = 0, maxLen = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            if (nums[right] == 0) zeros++;
            while (zeros > k) {
                if (nums[left++] == 0) zeros--;
            }
            maxLen = max(maxLen, right - left + 1);
        }
        return maxLen;
    }

    // =====================================================================
    // 11. LEETCODE 904: FRUIT INTO BASKETS
    // =====================================================================
    int totalFruit(vector<int> &fruits) {
        unordered_map<int, int> count;
        int left = 0, maxLen = 0;
        for (int right = 0; right < (int)fruits.size(); right++) {
            count[fruits[right]]++;
            while ((int)count.size() > 2) {
                count[fruits[left]]--;
                if (count[fruits[left]] == 0) count.erase(fruits[left]);
                left++;
            }
            maxLen = max(maxLen, right - left + 1);
        }
        return maxLen;
    }

    // =====================================================================
    // 12. LEETCODE 992: SUBARRAYS WITH K DIFFERENT INTEGERS
    // =====================================================================
    int atMostKDistinct(vector<int> &nums, int k) {
        unordered_map<int, int> freq;
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            freq[nums[right]]++;
            while ((int)freq.size() > k) {
                freq[nums[left]]--;
                if (freq[nums[left]] == 0) freq.erase(nums[left]);
                left++;
            }
            ans += right - left + 1;
        }
        return ans;
    }

    int subarraysWithKDistinct(vector<int> &nums, int k) {
        return atMostKDistinct(nums, k) - atMostKDistinct(nums, k - 1);
    }

    // =====================================================================
    // 13. LEETCODE 424: LONGEST REPEATING CHARACTER REPLACEMENT
    // =====================================================================
    int characterReplacement(string s, int k) {
        vector<int> freq(26, 0);
        int left = 0, maxFreq = 0, maxLen = 0;
        for (int right = 0; right < (int)s.size(); right++) {
            maxFreq = max(maxFreq, ++freq[s[right] - 'A']);
            while ((right - left + 1) - maxFreq > k) {
                freq[s[left++] - 'A']--;
            }
            maxLen = max(maxLen, right - left + 1);
        }
        return maxLen;
    }
};

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: SLIDING WINDOW & TWO POINTERS
 ====================================================================================================

 1. 4 CANONICAL SLIDING WINDOW ARCHETYPES:
    -------------------------------------------------------------------------------------------------
    Archetype               | Window Behavior             | Problems & Triggers
    -------------------------------------------------------------------------------------------------
    1. Fixed Window Size K  | Maintain r - l + 1 == K     | Find Anagrams, Permutation in String
    2. Variable Maximize    | Expand r; shrink l when BAD | Longest substring without repeat, Max 1s III
    3. Variable Minimize    | Expand r; shrink l when OK  | Minimum Size Subarray Sum, Min Window Substring
    4. Exact-K Subarrays    | atMost(K) - atMost(K - 1)   | Subarrays with sum K, Subarrays with K distinct
    -------------------------------------------------------------------------------------------------

 2. EXACT-K REDUCTION FORMULA (THE GOLDEN RULE):
    • Formula: `exact(K) = atMost(K) - atMost(K - 1)`
    • "At most K" is strictly monotonic, turning exact-K into two O(N) sliding windows.
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}