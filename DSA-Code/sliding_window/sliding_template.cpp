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
    // 2. HELPER: SUBARRAYS WITH SUM AT MOST K
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
    // 3. LEETCODE 930: BINARY SUBARRAYS WITH SUM
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
    // 4. LEETCODE 1358: NUMBER OF SUBSTRINGS CONTAINING ALL THREE CHARACTERS
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
    // 5. LEETCODE 76: MINIMUM WINDOW SUBSTRING
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
    // Interview Explanation:
    // - Problem Statement: Find the minimum length substring of s that contains all characters of t (including duplicates).
    // - Approach: Two frequency vectors with sliding window contraction.
    // - Intuition: Expand right pointer until window contains all required character frequencies; then greedily contract left pointer while preserving validity to minimize length.
    // - Complexity: Time: O(N * 128) -> O(N), Space: O(1) fixed 128-element frequency arrays.


    // =====================================================================
    // 6. LEETCODE 727: MINIMUM WINDOW SUBSEQUENCE
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
    // Interview Explanation:
    // - Problem Statement: Find the shortest substring of s that contains t as a subsequence.
    // - Approach: Two-pointer forward matching + backward contraction scan.
    // - Intuition: Scan forward to locate the first complete match of t; scan backwards from the end of the match to find the tightest possible start index.
    // - Complexity: Time: O(N * M) worst case, Space: O(1) auxiliary space.


    // =====================================================================
    // 7. LEETCODE 1004: MAX CONSECUTIVE ONES III
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
    // Interview Explanation:
    // - Problem Statement: Find the maximum number of consecutive 1s in a binary array if you can flip at most k 0s to 1s.
    // - Approach: Sliding window maintaining at most k zeros.
    // - Intuition: Window expands rightwards; whenever zero count exceeds k, advance left pointer to expel the oldest zero, maintaining validity.
    // - Complexity: Time: O(N) two pointers advance at most N times, Space: O(1) auxiliary space.


    // =====================================================================
    // 8. LEETCODE 904: FRUIT INTO BASKETS (AT MOST 2 DISTINCT ELEMENTS)
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
    // Interview Explanation:
    // - Problem Statement: Find the length of the longest contiguous subarray containing at most 2 distinct integers.
    // - Approach: Sliding window with hash map tracking frequency of elements.
    // - Intuition: Expand window until distinct key count exceeds 2; shrink from left until only 2 distinct keys remain, updating max window size.
    // - Complexity: Time: O(N) linear time, Space: O(1) hash map has at most 3 entries.


    // =====================================================================
    // 9. LEETCODE 992: SUBARRAYS WITH K DIFFERENT INTEGERS
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
    // Interview Explanation:
    // - Problem Statement: Count the number of subarrays with exactly k different integers.
    // - Approach: Exact-K reduction via atMostKDistinct(k) - atMostKDistinct(k - 1).
    // - Intuition: Finding exactly K distinct elements is non-monotonic, but "at most K" is monotonic and easily solvable via sliding window.
    // - Complexity: Time: O(N) two linear passes, Space: O(K) hash map storage.


    // =====================================================================
    // 10. LEETCODE 424: LONGEST REPEATING CHARACTER REPLACEMENT
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
    // Interview Explanation:
    // - Problem Statement: Find the length of the longest substring containing the same letter after replacing at most k characters.
    // - Approach: Sliding window with max frequency tracking.
    // - Intuition: In window [left, right], non-majority characters count is window_len - maxFreq; shrink left when this exceeds k.
    // - Complexity: Time: O(N) linear time, Space: O(1) 26-element array.
};

// =========================================================================
// SECTION 2: ALTERNATIVE WINDOW MECHANISMS (COUNTING & SLICING)
// =========================================================================

// Forward Suffix Counting Variant for LeetCode 1358
int numberOfSubstringsWindow1(string s) {
    vector<int> count(3, 0);
    int totalSubstrings = 0, l = 0, n = s.length();

    for (int r = 0; r < n; ++r) {
        count[s[r] - 'a']++;
        while (count[0] > 0 && count[1] > 0 && count[2] > 0) {
            totalSubstrings += (n - r);
            count[s[l++] - 'a']--;
        }
    }
    return totalSubstrings;
}
// Interview Explanation:
// - Problem Statement: Count substrings containing 'a', 'b', and 'c' using two-pointer forward suffix contribution.
// - Approach: Sliding window counting forward valid suffixes.
// - Intuition: When window s[l...r] becomes valid, every extension ending at r, r+1, ..., n-1 is also valid, contributing exactly n - r substrings per valid left boundary.
// - Complexity: Time: O(N) linear time, Space: O(1) 3-element frequency array.


// Backward Prefix Counting Variant for LeetCode 1358
int numberOfSubstringsWindow2(string s) {
    vector<int> cnt(3, 0);
    int l = 0, ans = 0;

    for (int r = 0; r < (int)s.size(); r++) {
        cnt[s[r] - 'a']++;
        while (cnt[0] && cnt[1] && cnt[2]) {
            cnt[s[l++] - 'a']--;
        }
        ans += l;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count substrings containing 'a', 'b', and 'c' using two-pointer left-pointer accumulation.
// - Approach: Sliding window counting valid backward prefixes.
// - Intuition: Advance left pointer while window contains all three characters; after shrinking, there are l valid starting positions [0...l-1] ending at current r.
// - Complexity: Time: O(N) linear time, Space: O(1) 3-element frequency array.

/*
 ====================================================================================================
             ULTIMATE LIVE INTERVIEW & OA CHEAT SHEET: SLIDING WINDOW & TWO POINTERS
 ====================================================================================================

 1. 4 CANONICAL SLIDING WINDOW ARCHETYPES:
    -------------------------------------------------------------------------------------------------
    Archetype               | Window Behavior             | Problems & Triggers
    -------------------------------------------------------------------------------------------------
    1. Fixed Window Size K  | Maintain r - l + 1 == K     | Max sum subarray of size K, Anagrams in string
    2. Variable Maximize    | Expand r; shrink l when BAD | Longest substring without repeat, Max 1s III
    3. Variable Minimize    | Expand r; shrink l when OK  | Minimum Window Substring, Min Size Subarray Sum
    4. Exact-K Subarrays    | atMost(K) - atMost(K - 1)   | Subarrays with sum K, Subarrays with K distinct
    -------------------------------------------------------------------------------------------------

 2. EXACT-K REDUCTION FORMULA (THE GOLDEN RULE):
    • Whenever a problem asks for "subarrays with EXACTLY K items/sum/distinct":
      Formula: `exact(K) = atMost(K) - atMost(K - 1)`
    • Why? Finding exact K is non-monotonic (expanding can make it valid or invalid).
      "At most K" is strictly monotonic (expanding never decreases sum/distinct count),
      allowing a simple O(N) sliding window!

 3. SUBSTRING & SUBARRAY COUNTING RULES:
    • "At most K" condition: Add `right - left + 1` at each step.
      - Reason: For window [left, right], all subarrays ending at right starting at left, left+1, ... right are valid.
    • "At least K" condition (e.g. Contains all a, b, c):
      - Method A (Forward suffixes): Add `n - right` while shrinking left.
      - Method B (Backward prefixes): Add `left` after shrinking.
      - Method C (Last Seen): Add `min(lastSeen[a], lastSeen[b], lastSeen[c]) + 1`.

 4. WHEN SLIDING WINDOW FAILS (CRITICAL OA TRAP):
    • Sliding window REQUIRES MONOTONICITY (all numbers are non-negative, or window expansion only increases count).
    • If an array contains NEGATIVE NUMBERS, sliding window FAILS because shrinking left can either increase or decrease sum!
    • Fallback: Use `Prefix Sum + Hash Map` (e.g. `pref[r] - pref[l] == K` in O(N) time and O(N) space).
 ====================================================================================================
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}