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

class StriverSlidingWindow
{
public:
    // LeetCode 3: Longest Substring Without Repeating Characters
    // Strategy: Variable Window (Dynamic Shrink via Hashing Array)
    // Time Complexity: O(N) | Space Complexity: O(1) (fixed size ASCII index array)
    int lengthOfLongestSubstring(string s)
    {
        int lastSeen[128];
        fill(lastSeen, lastSeen + 128, -1);
        int left = 0, ans = 0;
        for (int right = 0; right < s.size(); right++)
        {
            left = max(left, lastSeen[s[right]] + 1);
            lastSeen[s[right]] = right;
            ans = max(ans, right - left + 1);
        }
        return ans;
    }

    // LeetCode 424: Longest Repeating Character Replacement
    // Strategy: Variable Window (Optimization via Max Frequency Preservation)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int characterReplacement(string s, int k){
        int freq[26] = {0};
        int left = 0, maxFreq = 0, ans = 0;
        for (int right = 0; right < s.size(); right++)
        {
            maxFreq = max(maxFreq, ++freq[s[right] - 'A']);
            while (right - left + 1 - maxFreq > k)
            {
                freq[s[left] - 'A']--;
                left++;
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }

    // Helper for LeetCode 930 & 1248: Counts subarrays with sum/count at most K
    // Strategy: Exact Count K = (At Most K) - (At Most K-1)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int numSubarraysAtMostK(vector<int> &nums, int k)
    {
        if (k < 0) return 0;
        int left = 0, sum = 0, ans = 0;
        for (int right = 0; right < nums.size(); right++)

        {
            sum += nums[right];
            while (sum > k) sum -= nums[left++];
            ans += right - left + 1; // Count of subarrays ending at 'right' with sum <= k, as l, l+1, ..., r are all valid starting points
        }
        return ans;
    }

    // LeetCode 930: Subarrays with Sum
    // Strategy: Difference of two at-most-K sliding windows.
    // Time Complexity: O(N) | Space Complexity: O(1)
    int numSubarraysWithSum(vector<int> &nums, int goal){
        return numSubarraysAtMostK(nums, goal) - numSubarraysAtMostK(nums, goal - 1);
    }
    

    // LeetCode 1358: Number of Substrings Containing All Three Characters ('a', 'b', 'c')
    // Strategy: Last Seen Anchoring (Right-to-Left Accumulation lookup)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int numberOfSubstrings(const string &s)
    {
        vector<int> lastSeen(3, -1);
        int totalCount = 0;
        for (int i = 0; i < s.length(); i++){
            lastSeen[s[i] - 'a'] = i;
            if (lastSeen[0] != -1 && lastSeen[1] != -1 && lastSeen[2] != -1){
                totalCount += min({lastSeen[0], lastSeen[1], lastSeen[2]}) + 1;
            }
        }
        return totalCount;
    }

    

    // Helper validation for Minimum Window Substring. Checks if current window satisfies target requirements.
    // Time Complexity: O(1) (fixed 128 elements iteration) | Space Complexity: O(1)
    bool isValid(const vector<int> &freq1, const vector<int> &freq2)
    {
        for (int i = 0; i < 128; i++)
        {
            if (freq2[i] < freq1[i])
                return false; // Window missing needed characters
        }
        return true;
    }

    // LeetCode 76: Minimum Window Substring
    // Strategy: Two Frequency vectors validation loop (Variable Window)
    // Time Complexity: O(N) | Space Complexity: O(128) -> O(1)
    string minWindow(const string &s, const string &t)
    {
        vector<int> freq1(128, 0), freq2(128, 0);
        for (char ch : t)
            freq1[ch]++; // Seed target requirements

        int left = 0, start = -1, minLen = INT_MAX;

        for (int right = 0; right < s.size(); right++)
        {
            freq2[s[right]]++; // Include character in active map

            // Shrink window while all character counts from 't' are fully satisfied
            while (isValid(freq1, freq2))
            {
                if (right - left + 1 < minLen)
                {
                    minLen = right - left + 1;
                    start = left;
                }
                freq2[s[left]]--; // Eject character from left
                left++;
            }
        }
        return start == -1 ? "" : s.substr(start, minLen);
    }

    // LeetCode 727: Minimum Window Subsequence
    // Strategy: Two-Pointer Forward Matching followed by Right-to-Left Minimization Scan
    // Time Complexity: O(N * M) worst-case | Space Complexity: O(1)
    string minWindowSubsequence(const string &s, const string &t)
    {
        int n = s.size(), m = t.size();
        int idx = -1, len = INT_MAX;
        int i = 0, j = 0;

        while (i < n)
        {
            if (s[i] == t[j]) j++; // Character match found, move string t pointer forward

            // If string t is completely matched as a subsequence within window
            if (j == m)
            {
                int end = i; // Lock the ending index of our valid window
                j--; // as j was incremented to m, we need to decrement it to point to the last character of t
                while (j >= 0)
                {
                    if (s[i] == t[j]) j--;
                    i--;
                } // we are doing this because we want to find the start index of the window, so we need to move i back to the point where we found the first character of t in s
                int start = i + 1; // Compute compressed start index

                if (end - start + 1 < len) len = end - start + 1, idx = start;
                
                // Reset standard search variables just past the start pointer to find alternative solutions
                i = start + 1;
                j = 0;
                continue;
            }
            i++;
        }
        return idx == -1 ? "" : s.substr(idx, len);
    }
};


int main()
{
    // Optimize standard I/O streams for Competitive Programming performance
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}