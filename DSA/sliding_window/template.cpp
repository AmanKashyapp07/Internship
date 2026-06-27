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
    int lengthOfLongestSubstring(const string &s)
    {
        // Tracks the absolute last seen index of every ASCII character. Initialized to -1.
        vector<int> prev(128, -1);
        int left = 0, maxLength = 0;

        for (int right = 0; right < s.length(); right++)
        {
            // CRITICAL STEP: If the current character was seen AFTER or AT the current left boundary,
            // it means a duplicate exists in our current window. Warp 'left' directly past the old instance.
            if (prev[s[right]] >= left)
            {
                left = prev[s[right]] + 1;
            }

            // Record/Update the current character's latest position mapping
            prev[s[right]] = right;

            // Calculate current valid window size: (right - left + 1)
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    // LeetCode 1004: Max Consecutive Ones III
    // Strategy: Variable Window (Counting Constraints)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int longestOnes(const vector<int> &nums, int k)
    {
        int left = 0, zeroes = 0, maxLength = 0;

        for (int right = 0; right < nums.size(); right++)
        {
            // Expand Window: track total invalid elements (0s) introduced
            if (nums[right] == 0)
                zeroes++;

            // Shrink Window: If invalid count exceeds our allowance 'k', contract from left
            while (zeroes > k)
            {
                if (nums[left] == 0)
                    zeroes--;
                left++; // Gradually slide the left boundary forward
            }

            // Window is guaranteed valid here because the while loop resolved any violation
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    // LeetCode 904: Fruit Into Baskets
    // Finds the longest subarray with at most 2 distinct elements.
    // Strategy: Variable Window (Stateful Frequency Map tracking types)
    // Time Complexity: O(N) | Space Complexity: O(1) (frequency map max size is 3 elements)
    int totalFruit(const vector<int> &fruits)
    {
        unordered_map<int, int> count; // Map to track {Fruit_Type -> Frequency_In_Window}
        int left = 0, maxFruits = 0;

        for (int right = 0; right < fruits.size(); right++)
        {
            count[fruits[right]]++; // Insert current item into window

            // If unique element types cross the threshold of 2, shrink the window
            while (count.size() > 2)
            {
                count[fruits[left]]--;
                if (count[fruits[left]] == 0)
                {
                    count.erase(fruits[left]); // Entirely wipe element out when count drops to 0
                }
                left++; // Shift left pointer up
            }
            maxFruits = max(maxFruits, right - left + 1);
        }
        return maxFruits;
    }

    // LeetCode 424: Longest Repeating Character Replacement
    // Strategy: Variable Window (Optimization via Max Frequency Preservation)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int characterReplacement(const string &s, int k)
    {
        vector<int> count(26, 0);
        int left = 0, maxFreq = 0, maxLength = 0;

        for (int right = 0; right < s.length(); right++)
        {
            count[s[right] - 'A']++;
            // Track historical peak frequency of any single character inside the window.
            // Optimization: We don't need to recalculate maxFreq downward when shrinking left!
            maxFreq = max(maxFreq, count[s[right] - 'A']);

            // Invalid condition: (Total characters in window) - (Dominant character count) > k allowed swaps
            if ((right - left + 1) - maxFreq > k)
            {
                count[s[left] - 'A']--;
                left++; // Discard one element from left
            }
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    // Helper for LeetCode 930 & 1248: Counts subarrays with sum/count at most K
    // Strategy: Exact Count K = (At Most K) - (At Most K-1)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int numSubarraysAtMostK(const vector<int> &nums, int k)
    {
        if (k < 0)
            return 0;
        int left = 0, count = 0, currentSum = 0;

        for (int right = 0; right < nums.size(); right++)
        {
            currentSum += nums[right];

            // Shrink window while it breaks the upper bound sum 'k'
            while (currentSum > k)
            {
                currentSum -= nums[left++];
            }

            // KEY INSIGHT: The number of valid subarrays ending exactly at 'right'
            // is equivalent to the total length of the current window.
            count += (right - left + 1);
        }
        return count;
    }

    // LeetCode 930: Subarrays with Sum
    // Strategy: Difference of two at-most-K sliding windows.
    // Time Complexity: O(N) | Space Complexity: O(1)
    int numSubarraysWithSum(const vector<int> &nums, int goal)
    {
        // Returns the exact combinations matching target 'goal'
        return numSubarraysAtMostK(nums, goal) - numSubarraysAtMostK(nums, goal - 1);
    }

    // LeetCode 1358: Number of Substrings Containing All Three Characters ('a', 'b', 'c')
    // Strategy: Last Seen Anchoring (Right-to-Left Accumulation lookup)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int numberOfSubstrings(const string &s)
    {
        vector<int> lastSeen(3, -1); // Index tracker for 'a', 'b', and 'c'
        int totalCount = 0;

        for (int i = 0; i < s.length(); i++)
        {
            lastSeen[s[i] - 'a'] = i; // Register current position

            // If all 3 characters have appeared at least once somewhere behind us
            if (lastSeen[0] != -1 && lastSeen[1] != -1 && lastSeen[2] != -1)
            {
                // KEY CONCEPT: The element among the 3 that appeared furthest to the left controls
                // the minimum boundary. Every index from 0 up to that min element can serve as a
                // valid start index for a substring ending at 'i'.
                totalCount += min({lastSeen[0], lastSeen[1], lastSeen[2]}) + 1;
            }
        }
        return totalCount;
    }

    // LeetCode 1423: Maximum Points You Can Obtain from Cards
    // Strategy: Inverted Sliding Window (Complementary Subarray Minimum Sum)
    // Time Complexity: O(N) | Space Complexity: O(1)
    int maxScore(const vector<int> &cardPoints, int k)
    {
        int n = cardPoints.size(), windowSize = n - k;
        int totalSum = 0, currentWindowSum = 0;

        for (int x : cardPoints)
            totalSum += x;
        if (k == n)
            return totalSum; // Taking all elements

        // Initialize the first sliding window of size (n - k)
        for (int i = 0; i < windowSize; i++)
        {
            currentWindowSum += cardPoints[i];
        }
        int minWindowSum = currentWindowSum;

        // Slide the fixed window of size (n - k) across the array
        for (int i = windowSize; i < n; i++)
        {
            currentWindowSum += cardPoints[i] - cardPoints[i - windowSize]; // Slide action
            minWindowSum = min(minWindowSum, currentWindowSum);
        }

        // Max score from outer ends = Total Array Sum - Minimum Interior Window Sum
        return totalSum - minWindowSum;
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

    // Helper for LeetCode 992: Counts subarrays with at most k distinct elements.
    // Time Complexity: O(N) | Space Complexity: O(K)
    int subarraysWithAtMostKDistinct(const vector<int> &nums, int k)
    {
        unordered_map<int, int> freq; // Map to track {Element -> Frequency_In_Window}
        int left = 0, count = 0;

        for (int right = 0; right < nums.size(); right++)
        {
            freq[nums[right]]++; // Insert current item into window

            // If the number of distinct elements exceeds k, shrink the window from the left
            while (freq.size() > k)
            {
                freq[nums[left]]--;
                if (freq[nums[left]] == 0)
                {
                    freq.erase(nums[left]); // Remove element from map if its count becomes 0
                }
                left++;
            }

            // Total subarrays ending at 'right' with at most k distinct elements is the window size
            count += (right - left + 1);
        }
        return count;
    }

    // LeetCode 727: Minimum Window Subsequence
    // Strategy: Two-Pointer Forward Matching followed by Right-to-Left Minimization Scan
    // Time Complexity: O(N * M) worst-case | Space Complexity: O(1)
    string minWindowSubsequence(const string &s, const string &t)
    {
        int n = s.size(), m = t.size();
        int bestStart = -1, bestLen = INT_MAX;
        int i = 0, j = 0;

        while (i < n)
        {
            if (s[i] == t[j])
                j++; // Character match found, move string t pointer forward

            // If string t is completely matched as a subsequence within window
            if (j == m)
            {
                int end = i; // Lock the ending index of our valid window
                j--;

                // CRITICAL REVERSE SCAN: Backtrack from right to left to find optimal start index
                // This eliminates unnecessary matching prefixes from the left side of the window
                while (j >= 0)
                {
                    if (s[i] == t[j])
                        j--;
                    i--;
                }
                int start = i + 1; // Compute compressed start index

                if (end - start + 1 < bestLen)
                {
                    bestLen = end - start + 1;
                    bestStart = start;
                }
                // Reset standard search variables just past the start pointer to find alternative solutions
                i = start + 1;
                j = 0;
                continue;
            }
            i++;
        }
        return bestStart == -1 ? "" : s.substr(bestStart, bestLen);
    }
};

// =========================================================================
// SECTION 2: ADVANCED SYSTEM PROBLEMS (FIXED & DYNAMIC METRICS OVER RANGES)
// =========================================================================

class AdvancedSlidingWindow
{
public:
    /**
     * ========================================================
     *              Sliding Window Minimum
     * ========================================================
     *
     * Idea:
     * Maintain an increasing monotonic deque.
     *
     * Time  : O(n)
     * Space : O(k)
     * ========================================================
     */

    vector<int> sliding_window_min(vector<int> &arr, int k)
    {
        int n = arr.size();

        deque<int> dq;
        vector<int> ans;

        for (int R = 0; R < n; R++)
        {
            int L = R - k + 1;

            // 1. Remove expired indices
            while (!dq.empty() && dq.front() < L)
                dq.pop_front();

            // 2. Maintain increasing deque
            while (!dq.empty() && arr[dq.back()] >= arr[R])
                dq.pop_back();

            // 3. Insert current index
            dq.push_back(R);

            // 4. Window becomes valid
            if (R >= k - 1)
                ans.push_back(arr[dq.front()]);
        }

        return ans;
    }
    // Advanced Pattern A: Sliding Window Maximum
    // Strategy: Monotonic Queue (Deque storing indices maintaining decreasing values)
    // Time Complexity: O(N) | Space Complexity: O(K)
    /**
     * ========================================================
     *              Sliding Window Maximum
     * ========================================================
     *
     * Idea:
     * Maintain a decreasing monotonic deque.
     *
     * Time  : O(n)
     * Space : O(k)
     * ========================================================
     */

    vector<int> sliding_window_max(vector<int> &arr, int k)
    {
        int n = arr.size();

        deque<int> dq;
        vector<int> ans;

        for (int R = 0; R < n; R++)
        {
            int L = R - k + 1;

            // 1. Remove expired indices
            while (!dq.empty() && dq.front() < L)
                dq.pop_front();

            // 2. Maintain decreasing deque
            while (!dq.empty() && arr[dq.back()] <= arr[R])
                dq.pop_back();

            // 3. Insert current index
            dq.push_back(R);

            // 4. Window becomes valid
            if (R >= k - 1)
                ans.push_back(arr[dq.front()]);
        }

        return ans;
    }
    // Advanced Pattern B: Sliding Window Bitwise XOR Aggregates
    // Strategy: Fixed Window Invalidation Optimization using XOR inverse property (A ^ B ^ A = B)
    // Time Complexity: O(N) | Space Complexity: O(1) (excluding output vector)
    vector<int> sliding_window_xor(const vector<int> &a, int k)
    {
        vector<int> res;
        int xr = 0;

        // Process initial window frame
        for (int i = 0; i < k; i++)
            xr ^= a[i];
        res.push_back(xr);

        // Slide the window: Apply inverse property. (A ^ B ^ A = B)
        for (int i = k; i < (int)a.size(); i++)
        {
            xr ^= a[i - k]; // Undo/Eject oldest element out of current evaluation
            xr ^= a[i];     // Apply/Inject new incoming element
            res.push_back(xr);
        }
        return res;
    }

    // Advanced Pattern C: Sliding Window Distinct Values Count
    // Strategy: Fixed sliding window with a frequency map tracking unique elements.
    // Time Complexity: O(N log K) | Space Complexity: O(K)
    vector<int> sliding_window_distinct(const vector<int> &a, int k)
    {
        vector<int> res;
        map<int, int> freq;

        // Seed initial frame
        for (int i = 0; i < k; i++)
            freq[a[i]]++;
        res.push_back(freq.size());

        // Process remaining sliding segments
        for (int i = k; i < (int)a.size(); i++)
        {
            if (--freq[a[i - k]] == 0)
            {
                freq.erase(a[i - k]); // Wipe key entirely to keep map size strictly mapped to uniques
            }
            freq[a[i]]++;
            res.push_back(freq.size());
        }
        return res;
    }
};

// =========================================================================
// PIPELINE RUNNER
// =========================================================================

int main()
{
    // Optimize standard I/O streams for Competitive Programming performance
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}