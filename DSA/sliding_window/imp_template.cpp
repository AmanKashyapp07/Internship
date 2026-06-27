/**
 * Problem: Sliding Window & Two-Pointer Ultimate Template
 * Categories: Interview LeetCode (Striver) & Advanced CP Range Queries
 * * Description:
 * A production-grade master template containing implementations for all core
 * sliding window variations, including variable-size windows, count-difference optimizations,
 * monotonic queues, and running window statistics (Min, Median, Distinct, Mex, Cost).
 */

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
// SECTION 1: STRIVER'S DSA SHEET CORE (INTERVIEW & OA PATTERNS)
// =========================================================================

class StriverSlidingWindow
{
public:
    /**
     * 3. Longest Substring Without Repeating Characters
     * Time: O(N) | Space: O(128) -> O(1)
     */
    int lengthOfLongestSubstring(string s)
    {
        vector<int> lastIdx(128, -1);
        int left = 0, maxLength = 0;
        for (int right = 0; right < s.length(); right++)
        {
            if (lastIdx[s[right]] >= left)
            {
                left = lastIdx[s[right]] + 1; // Jump 'left' ahead of the repeat
            }
            lastIdx[s[right]] = right;
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    /**
     * 1004. Max Consecutive Ones III
     * Time: O(N) | Space: O(1)
     */
    int longestOnes(const vector<int> &nums, int k)
    {
        int left = 0, zeroes = 0, maxLength = 0;
        for (int right = 0; right < nums.size(); right++)
        {
            if (nums[right] == 0)
                zeroes++;
            while (zeroes > k)
            {
                if (nums[left] == 0)
                    zeroes--;
                left++;
            }
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    /**
     * 904. Fruit Into Baskets (Longest Subarray with at most 2 Distinct Elements)
     * Time: O(N) | Space: O(1)
     */
    int totalFruit(const vector<int> &fruits)
    {
        unordered_map<int, int> count;
        int left = 0, maxFruits = 0;
        for (int right = 0; right < fruits.size(); right++)
        {
            count[fruits[right]]++;
            while (count.size() > 2)
            {
                count[fruits[left]]--;
                if (count[fruits[left]] == 0)
                    count.erase(fruits[left]);
                left++;
            }
            maxFruits = max(maxFruits, right - left + 1);
        }
        return maxFruits;
    }

    /**
     * 424. Longest Repeating Character Replacement
     * Time: O(N) | Space: O(1)
     */
    int characterReplacement(string s, int k)
    {
        vector<int> count(26, 0);
        int left = 0, maxFreq = 0, maxLength = 0;
        for (int right = 0; right < s.length(); right++)
        {
            count[s[right] - 'A']++;
            maxFreq = max(maxFreq, count[s[right] - 'A']);

            if ((right - left + 1) - maxFreq > k)
            {
                count[s[left] - 'A']--;
                left++;
            }
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }

    /**
     * 930 / 1248. Helper for Binary Subarrays with Sum & Nice Subarrays
     * Counts subarrays with AT MOST 'k' tracking metrics
     */
    int numSubarraysAtMostK(const vector<int> &nums, int k)
    {
        if (k < 0)
            return 0;
        int left = 0, count = 0, currentSum = 0;
        for (int right = 0; right < nums.size(); right++)
        {
            currentSum += nums[right];
            while (currentSum > k)
            {
                currentSum -= nums[left++];
            }
            count += (right - left + 1);
        }
        return count;
    }

    int numSubarraysWithSum(const vector<int> &nums, int goal)
    {
        return numSubarraysAtMostK(nums, goal) - numSubarraysAtMostK(nums, goal - 1);
    }

    /**
     * 1358. Number of Substrings Containing All Three Characters
     * Time: O(N) | Space: O(1)
     */
    int numberOfSubstrings(string s)
    {
        vector<int> lastSeen(3, -1);
        int totalCount = 0;
        for (int i = 0; i < s.length(); i++)
        {
            lastSeen[s[i] - 'a'] = i;
            if (lastSeen[0] != -1 && lastSeen[1] != -1 && lastSeen[2] != -1)
            {
                totalCount += min({lastSeen[0], lastSeen[1], lastSeen[2]}) + 1;
            }
        }
        return totalCount;
    }

    /**
     * 1423. Maximum Points You Can Obtain from Cards
     * Time: O(N) | Space: O(1)
     */
    int maxScore(const vector<int> &cardPoints, int k)
    {
        int n = cardPoints.size(), windowSize = n - k;
        int totalSum = 0, currentWindowSum = 0;
        for (int x : cardPoints)
            totalSum += x;
        if (k == n)
            return totalSum;

        for (int i = 0; i < windowSize; i++)
            currentWindowSum += cardPoints[i];
        int minWindowSum = currentWindowSum;

        for (int i = windowSize; i < n; i++)
        {
            currentWindowSum += cardPoints[i] - cardPoints[i - windowSize];
            minWindowSum = min(minWindowSum, currentWindowSum);
        }
        return totalSum - minWindowSum;
    }

    /**
     * 76. Minimum Window Substring
     * Time: O(N) | Space: O(128)
     */
    bool isValid(vector<int> &freq1, vector<int> &freq2)
    {
        for (int i = 0; i < 128; i++)
        {
            if (freq2[i] < freq1[i])
                return false;
        }
        return true;
    }

    string minWindow(string s, string t)
    {

        vector<int> freq1(128, 0), freq2(128, 0);
        for (char ch : t)
            freq1[ch]++;
        int left = 0;
        int start = -1;
        int minLen = INT_MAX;
        for (int right = 0; right < s.size(); right++)
        {
            freq2[s[right]]++;
            while (isValid(freq1, freq2))
            {
                if (right - left + 1 < minLen)
                {
                    minLen = right - left + 1;
                    start = left;
                }
                freq2[s[left]]--;
                left++;
            }
        }
        return start == -1 ? "" : s.substr(start, minLen);
    }

    int subarraysWithAtMostKDistinct(vector<int> &nums, int k)
    {
        unordered_map<int, int> freq;
        int left = 0, count = 0;
        for (int right = 0; right < nums.size(); right++)
        {
            freq[nums[right]]++;
            while (freq.size() > k)
            {
                freq[nums[left]]--;
                if (freq[nums[left]] == 0)
                    freq.erase(nums[left]);
                left++;
            }
            count += (right - left + 1);
        }
        return count;
    }

    string minWindowSubsequence(string s, string t)
    {
        int n = s.size(), m = t.size();

        int bestStart = -1, bestLen = INT_MAX;
        int i = 0, j = 0;

        while (i < n)
        {
            if (s[i] == t[j])
                j++;

            if (j == m)
            {
                int end = i;
                j--;

                while (j >= 0)
                {
                    if (s[i] == t[j])
                        j--;
                    i--;
                }

                int start = i + 1;

                if (end - start + 1 < bestLen)
                {
                    bestLen = end - start + 1;
                    bestStart = start;
                }

                i = start + 1; // next search starts after window start
                j = 0;
                continue;
            }

            i++;
        }

        return bestStart == -1 ? "" : s.substr(bestStart, bestLen);
    }

    // =========================================================================
    // SECTION 2: ADVANCED SYSTEM PROBLEMS (FIXED WINDOW RANGE TRACKING)
    // =========================================================================

    class AdvancedSlidingWindow
    {
    public:
        /**
         * Pattern A: Sliding Window Minimum / Maximum
         * Uses a Monotonic Deque to find mins/maxes for every window of size K.
         * Time: O(N) | Space: O(K)
         */
        // Sliding Window Minimum
        vector<int> sliding_window_min(const vector<int> &a, int k)
        {
            deque<int> dq; // Stores indices
            vector<int> res;

            for (int i = 0; i < (int)a.size(); i++)
            {
                // Remove indices outside current window
                if (!dq.empty() && dq.front() <= i - k)
                    dq.pop_front();

                // Maintain increasing order
                while (!dq.empty() && a[dq.back()] >= a[i])
                    dq.pop_back();

                dq.push_back(i);

                // Window formed
                if (i >= k - 1)
                    res.push_back(a[dq.front()]);
            }

            return res;
        }
        /**
         * Pattern B: Sliding Window Bitwise XOR / OR
         * Uses bit frequency counts across the window to compute dynamic bitwise aggregates.
         * Time: O(N * 30) | Space: O(30)
         */
        // Sliding Window Maximum
        vector<int> sliding_window_max(const vector<int> &a, int k)
        {
            deque<int> dq; // Stores indices
            vector<int> res;

            for (int i = 0; i < (int)a.size(); i++)
            {
                // Remove indices outside current window
                if (!dq.empty() && dq.front() <= i - k)
                    dq.pop_front();

                // Maintain decreasing order
                while (!dq.empty() && a[dq.back()] <= a[i])
                    dq.pop_back();

                dq.push_back(i);

                // Window formed
                if (i >= k - 1)
                    res.push_back(a[dq.front()]);
            }

            return res;
        }
        // Sliding Window XOR
        vector<int> sliding_window_xor(const vector<int> &a, int k)
        {
            vector<int> res;
            int xr = 0;

            for (int i = 0; i < k; i++)
                xr ^= a[i];
            res.push_back(xr);

            for (int i = k; i < (int)a.size(); i++)
            {
                xr ^= a[i - k]; // Remove old element
                xr ^= a[i];     // Add new element

                res.push_back(xr);
            }

            return res;
        }
        /**
         * Pattern C: Sliding Window Distinct Values Count
         * Time: O(N) | Space: O(K)
         */
        // Sliding Window Distinct Count
        vector<int> sliding_window_distinct(const vector<int> &a, int k)
        {
            vector<int> res;
            map<int, int> freq;

            for (int i = 0; i < k; i++)
                freq[a[i]]++;
            res.push_back(freq.size());

            for (int i = k; i < (int)a.size(); i++)
            {
                if (--freq[a[i - k]] == 0)
                    freq.erase(a[i - k]);

                freq[a[i]]++;

                res.push_back(freq.size());
            }

            return res;
        }

        /**
         * Pattern D: Sliding Window Median
         * Uses two balanced multisets with an implicit pointer/iterator balance mapping.
         * Time: O(N log K) | Space: O(K)
         */
        vector<double> slidingWindowMedian(const vector<int> &nums, int k)
        {
            multiset<int> low, high;
            vector<double> medians;

            auto balance = [&]()
            {
                if (low.size() > high.size() + 1)
                {
                    high.insert(*low.rbegin());
                    low.erase(prev(low.end()));
                }
                else if (low.size() < high.size())
                {
                    low.insert(*high.begin());
                    high.erase(high.begin());
                }
            };

            auto add = [&](int num)
            {
                if (low.empty() || num <= *low.rbegin())
                    low.insert(num);
                else
                    high.insert(num);
                balance();
            };

            auto removeNode = [&](int num)
            {
                auto it = low.find(num);
                if (it != low.end())
                    low.erase(it);
                else
                    high.erase(high.find(num));
                balance();
            };

            for (int i = 0; i < nums.size(); i++)
            {
                add(nums[i]);
                if (i >= k)
                    removeNode(nums[i - k]);
                if (i >= k - 1)
                {
                    if (k % 2 != 0)
                        medians.push_back(*low.rbegin());
                    else
                        medians.push_back(((double)*low.rbegin() + *high.begin()) / 2.0);
                }
            }
            return medians;
        }

        /**
         * Pattern E: Sliding Window Transformation Cost
         * Calculates costs to transform elements inside a window to their median.
         * Time: O(N log K) | Space: O(K)
         */
        vector<long long> slidingWindowCost(const vector<int> &nums, int k)
        {
            multiset<int> low, high;
            long long sumLow = 0, sumHigh = 0;
            vector<long long> costs;

            auto balance = [&]()
            {
                if (low.size() > high.size() + 1)
                {
                    int val = *low.rbegin();
                    sumLow -= val;
                    sumHigh += val;
                    high.insert(val);
                    low.erase(prev(low.end()));
                }
                else if (low.size() < high.size())
                {
                    int val = *high.begin();
                    sumHigh -= val;
                    sumLow += val;
                    low.insert(val);
                    high.erase(high.begin());
                }
            };

            auto add = [&](int num)
            {
                if (low.empty() || num <= *low.rbegin())
                {
                    low.insert(num);
                    sumLow += num;
                }
                else
                {
                    high.insert(num);
                    sumHigh += num;
                }
                balance();
            };

            auto removeNode = [&](int num)
            {
                auto it = low.find(num);
                if (it != low.end())
                {
                    sumLow -= num;
                    low.erase(it);
                }
                else
                {
                    sumHigh -= num;
                    high.erase(high.find(num));
                }
                balance();
            };

            for (int i = 0; i < nums.size(); i++)
            {
                add(nums[i]);
                if (i >= k)
                    removeNode(nums[i - k]);
                if (i >= k - 1)
                {
                    long long median = *low.rbegin();
                    // Mathematical formulation: Cost = (median * count(low) - sum(low)) + (sum(high) - median * count(high))
                    long long cost = (median * low.size() - sumLow) + (sumHigh - median * high.size());
                    costs.push_back(cost);
                }
            }
            return costs;
        }
    };

    // =========================================================================
    // PIPELINE RUNNER
    // =========================================================================
    int main()
    {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        return 0;
    }