#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#endif
using namespace std;

using ll = long long;
using vi = vector<int>;
using vvi = vector<vector<int>>;

// =========================================================
// 1. TWO SUM
// =========================================================

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    for (int i = 0; i < (int)nums.size(); ++i) {
        int complement = target - nums[i];
        if (seen.find(complement) != seen.end()) {
            return {seen[complement], i};
        }
        seen[nums[i]] = i;
    }
    return {};
}
// Interview Explanation:
// - Problem Statement: Find two indices whose values sum to target.
// - Approach: Single-pass Hash Map Complement Lookup.
// - Intuition: For each number x, query if (target - x) was already seen in O(1).
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 2. CONTAINS DUPLICATE
// =========================================================

bool containsDuplicate(vector<int>& nums) {
    unordered_set<int> seen;
    for (int x : nums) {
        if (seen.count(x)) return true;
        seen.insert(x);
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if array contains any duplicates.
// - Approach: Hash Set presence checking.
// - Intuition: Insert elements into set; return true immediately on collision.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 3. VALID ANAGRAM
// =========================================================

bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;
    int count[26] = {0};
    for (char c : s) count[c - 'a']++;
    for (char c : t) {
        if (--count[c - 'a'] < 0) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if string t is an anagram of s.
// - Approach: 26-character frequency bucket counter.
// - Intuition: Increment on s, decrement on t; if count drops below 0, strings differ.
// - Complexity: Time: O(N), Space: O(1) (26 integers).


// =========================================================
// 4. GROUP ANAGRAMS
// =========================================================

vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        string key(26, '0');
        for (char c : s) key[c - 'a']++;
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, list] : groups) {
        result.push_back(std::move(list));
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Group strings that are anagrams of each other.
// - Approach: Canonical 26-byte Frequency String as Hash Map Key.
// - Intuition: Anagrams share identical character frequency signatures; map each to its bucket.
// - Complexity: Time: O(N * K), Space: O(N * K).


// =========================================================
// 5. PRODUCT OF ARRAY EXCEPT SELF
// =========================================================

vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    int prefix = 1;
    for (int i = 0; i < n; ++i) {
        res[i] = prefix;
        prefix *= nums[i];
    }
    int suffix = 1;
    for (int i = n - 1; i >= 0; --i) {
        res[i] *= suffix;
        suffix *= nums[i];
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compute array where output[i] is product of all elements except nums[i] in O(1) auxiliary space without division.
// - Approach: 2-Pass Prefix and Suffix Product accumulation in output array.
// - Intuition: res[i] = prefix[i-1] * suffix[i+1].
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 6. MAXIMUM SUBARRAY (KADANE'S ALGORITHM)
// =========================================================

int maxSubArray(vector<int>& nums) {
    int maxSoFar = nums[0];
    int currMax = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        currMax = max(nums[i], currMax + nums[i]);
        maxSoFar = max(maxSoFar, currMax);
    }
    return maxSoFar;
}
// Interview Explanation:
// - Problem Statement: Find contiguous subarray with the largest sum.
// - Approach: Kadane's Dynamic Programming Algorithm.
// - Intuition: At each step, either extend the previous subarray or start a fresh one at nums[i].
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 7. MAJORITY ELEMENT
// =========================================================

int majorityElement(vector<int>& nums) {
    int candidate = 0, count = 0;
    for (int x : nums) {
        if (count == 0) candidate = x;
        count += (x == candidate) ? 1 : -1;
    }
    return candidate;
}
// Interview Explanation:
// - Problem Statement: Find element that appears more than N/2 times in O(1) space.
// - Approach: Boyer-Moore Voting Algorithm.
// - Intuition: Majority element's occurrences outweigh all other elements combined; cancel out pairs of distinct elements.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 8. LONGEST CONSECUTIVE SEQUENCE
// =========================================================

int longestConsecutive(vector<int>& nums) {
    unordered_set<int> numSet(nums.begin(), nums.end());
    int longest = 0;
    for (int x : numSet) {
        if (!numSet.count(x - 1)) {
            int curr = x, streak = 1;
            while (numSet.count(curr + 1)) {
                curr++;
                streak++;
            }
            longest = max(longest, streak);
        }
    }
    return longest;
}
// Interview Explanation:
// - Problem Statement: Find length of longest consecutive elements sequence in unsorted array.
// - Approach: Hash Set with Sequence Starter Filtering.
// - Intuition: Only expand forward from true sequence starters (where x - 1 is not in set) to guarantee linear time.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 9. SUBARRAY SUM EQUALS K
// =========================================================

int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> prefixFreq;
    prefixFreq[0] = 1;
    int prefixSum = 0, count = 0;
    for (int x : nums) {
        prefixSum += x;
        if (prefixFreq.count(prefixSum - k)) {
            count += prefixFreq[prefixSum - k];
        }
        prefixFreq[prefixSum]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total subarrays whose sum equals k.
// - Approach: Prefix Sum + Frequency Hash Map.
// - Intuition: sum(i..j) = prefix[j] - prefix[i-1] = k => prefix[i-1] = prefix[j] - k.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 10. MERGE SORTED ARRAY
// =========================================================

void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    int p1 = m - 1, p2 = n - 1, p = m + n - 1;
    while (p2 >= 0) {
        if (p1 >= 0 && nums1[p1] > nums2[p2]) {
            nums1[p--] = nums1[p1--];
        } else {
            nums1[p--] = nums2[p2--];
        }
    }
}
// Interview Explanation:
// - Problem Statement: Merge nums2 into nums1 in-place as one sorted array.
// - Approach: Backward 3-Pointer Placement.
// - Intuition: Fill nums1 from the back to prevent overwriting unmerged elements in nums1.
// - Complexity: Time: O(M + N), Space: O(1).


// =========================================================
// 11. LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS
// =========================================================

int lengthOfLongestSubstring(string s) {
    vector<int> lastIdx(128, -1);
    int left = 0, maxLen = 0;
    for (int right = 0; right < (int)s.size(); ++right) {
        char c = s[right];
        if (lastIdx[c] >= left) {
            left = lastIdx[c] + 1;
        }
        lastIdx[c] = right;
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of longest substring without duplicate characters.
// - Approach: Sliding Window with Last-Seen Index Table.
// - Intuition: When duplicate is seen, jump left directly past previous duplicate position.
// - Complexity: Time: O(N), Space: O(1) (128 ASCII array).


// =========================================================
// 12. LONGEST REPEATING CHARACTER REPLACEMENT
// =========================================================

int characterReplacement(string s, int k) {
    vector<int> count(26, 0);
    int left = 0, maxFreq = 0, maxLen = 0;
    for (int right = 0; right < (int)s.size(); ++right) {
        maxFreq = max(maxFreq, ++count[s[right] - 'A']);
        while ((right - left + 1) - maxFreq > k) {
            count[s[left] - 'A']--;
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Longest substring containing same letter after at most k replacements.
// - Approach: Sliding Window + Max Frequency Tracker.
// - Intuition: Window is valid if (windowLen - maxFreq) <= k. Shrink left when invalid.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 13. PERMUTATION IN STRING
// =========================================================

bool checkInclusion(string s1, string s2) {
    if (s1.size() > s2.size()) return false;
    vector<int> count1(26, 0), count2(26, 0);
    for (size_t i = 0; i < s1.size(); ++i) {
        count1[s1[i] - 'a']++;
        count2[s2[i] - 'a']++;
    }
    if (count1 == count2) return true;
    for (size_t i = s1.size(); i < s2.size(); ++i) {
        count2[s2[i] - 'a']++;
        count2[s2[i - s1.size()] - 'a']--;
        if (count1 == count2) return true;
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Check if s2 contains a permutation of s1.
// - Approach: Fixed Sliding Window of size |s1|.
// - Intuition: Compare frequency arrays of size 26 in O(1) as the window moves across s2.
// - Complexity: Time: O(|s2| * 26), Space: O(1).


// =========================================================
// 14. FIND ALL ANAGRAMS IN A STRING
// =========================================================

vector<int> findAnagrams(string s, string p) {
    if (s.size() < p.size()) return {};
    vector<int> pCount(26, 0), sCount(26, 0), res;
    for (size_t i = 0; i < p.size(); ++i) {
        pCount[p[i] - 'a']++;
        sCount[s[i] - 'a']++;
    }
    if (pCount == sCount) res.push_back(0);
    for (size_t i = p.size(); i < s.size(); ++i) {
        sCount[s[i] - 'a']++;
        sCount[s[i - p.size()] - 'a']--;
        if (pCount == sCount) res.push_back(i - p.size() + 1);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Return all start indices of p's anagrams in s.
// - Approach: Fixed Sliding Window of size |p|.
// - Intuition: Sliding window maintains character frequencies; compare with target profile in O(1).
// - Complexity: Time: O(|s| * 26), Space: O(1).


// =========================================================
// 15. MINIMUM WINDOW SUBSTRING
// =========================================================

string minWindow(string s, string t) {
    if (s.empty() || t.empty() || s.size() < t.size()) return "";
    unordered_map<char, int> targetFreq, windowFreq;
    for (char c : t) targetFreq[c]++;
    int have = 0, need = targetFreq.size();
    int minLen = INT_MAX, startIdx = -1, left = 0;

    for (int right = 0; right < (int)s.size(); ++right) {
        char c = s[right];
        windowFreq[c]++;
        if (targetFreq.count(c) && windowFreq[c] == targetFreq[c]) have++;

        while (have == need) {
            if (right - left + 1 < minLen) {
                minLen = right - left + 1;
                startIdx = left;
            }
            char leftChar = s[left];
            windowFreq[leftChar]--;
            if (targetFreq.count(leftChar) && windowFreq[leftChar] < targetFreq[leftChar]) have--;
            left++;
        }
    }
    return startIdx == -1 ? "" : s.substr(startIdx, minLen);
}
// Interview Explanation:
// - Problem Statement: Find smallest substring in s containing all characters of t.
// - Approach: Sliding Window with 'have == need' Match Counter.
// - Intuition: Expand right until all character demands are met; shrink left to minimize window.
// - Complexity: Time: O(|s| + |t|), Space: O(1).


// =========================================================
// 16. VALID PALINDROME
// =========================================================

bool isPalindrome(string s) {
    int left = 0, right = (int)s.size() - 1;
    while (left < right) {
        while (left < right && !isalnum(s[left])) left++;
        while (left < right && !isalnum(s[right])) right--;
        if (tolower(s[left]) != tolower(s[right])) return false;
        left++;
        right--;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Check if string is palindrome considering only alphanumeric characters and ignoring case.
// - Approach: Two Pointers inward scan with isalnum filter.
// - Intuition: Skip non-alphanumeric characters from both ends and compare lowercase characters.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 17. LONGEST PALINDROMIC SUBSTRING
// =========================================================

string longestPalindrome(string s) {
    if (s.empty()) return "";
    int start = 0, maxLen = 0;

    auto expand = [&](int l, int r) {
        while (l >= 0 && r < (int)s.size() && s[l] == s[r]) {
            l--;
            r++;
        }
        if (r - l - 1 > maxLen) {
            maxLen = r - l - 1;
            start = l + 1;
        }
    };

    for (int i = 0; i < (int)s.size(); ++i) {
        expand(i, i);     // Odd length
        expand(i, i + 1); // Even length
    }
    return s.substr(start, maxLen);
}
// Interview Explanation:
// - Problem Statement: Find longest palindromic substring in s.
// - Approach: Expand Around Center (2N-1 centers).
// - Intuition: Every palindrome expands symmetrically around its single-char center or two-char center.
// - Complexity: Time: O(N^2), Space: O(1).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Two Sum                                     | Complement Hash Map Lookup        | O(N)     | O(N)     |
 | 2  | Contains Duplicate                          | Hash Set Lookup                   | O(N)     | O(N)     |
 | 3  | Valid Anagram                               | Character Frequency Bucket Count  | O(N)     | O(1)     |
 | 4  | Group Anagrams                              | 26-Char Frequency Key Hashing     | O(N * K) | O(N * K) |
 | 5  | Product of Array Except Self                | Prefix & Suffix Products Array    | O(N)     | O(1)     |
 | 6  | Maximum Subarray                            | Kadane's Algorithm (Running Max)  | O(N)     | O(1)     |
 | 7  | Majority Element                            | Boyer-Moore Voting Algorithm      | O(N)     | O(1)     |
 | 8  | Longest Consecutive Sequence                | Hash Set Boundary Expansion       | O(N)     | O(N)     |
 | 9  | Subarray Sum Equals K                       | Prefix Sum + Frequency Hash Map   | O(N)     | O(N)     |
 | 10 | Merge Sorted Array                          | Backward In-Place Two Pointers    | O(M + N) | O(1)     |
 | 11 | Longest Substring Without Repeating Chars   | Dynamic Sliding Window (Last Seen)| O(N)     | O(1)     |
 | 12 | Longest Repeating Character Replacement     | Sliding Window + Max Frequency    | O(N)     | O(1)     |
 | 13 | Permutation in String                       | Fixed-Size Sliding Window         | O(N * 26)| O(1)     |
 | 14 | Find All Anagrams in a String               | Fixed-Size Sliding Window         | O(N * 26)| O(1)     |
 | 15 | Minimum Window Substring                    | Match Counter ('have == need')    | O(N + M) | O(1)     |
 | 16 | Valid Palindrome                            | Two Pointers Inward Scan           | O(N)     | O(1)     |
 | 17 | Longest Palindromic Substring               | Expand Around Center (2N-1 centers)| O(N^2)   | O(1)     |
 ====================================================================================================
*/
